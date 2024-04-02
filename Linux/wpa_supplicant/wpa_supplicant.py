#!/usr/bin/python3

import argparse
import asyncio
import dbus_next
import logging
import pathlib


logger = logging.getLogger(pathlib.Path(__file__).name)


def _syntax():
    parser = argparse.ArgumentParser(
        description="A simple program to learn how to do D-Bus programming on wpa_supplicant"
    )

    parser.add_argument("wifi_ifname")

    return parser


class dbus_ids(object):
    # Top level: DBus "broker" address.
    DBUS_BROKER_ADDRESS = "/var/run/dbus/system_bus_socket"

    # DBus names (i.e., connections/services/applications)
    DBUS_SERVICE_NAME_SUPPLICANT1 = "fi.w1.wpa_supplicant1"
    SUPPLICANT_DBUS_SERVICE = DBUS_SERVICE_NAME_SUPPLICANT1

    # Object paths (for DBus objects)
    SUPPLICANT_OBJ_PATH = "/fi/w1/wpa_supplicant1"
    SUPPLICANT_DBUS_OPATH = SUPPLICANT_OBJ_PATH

    # Object's interfaces.
    SUPPLICANT1_DBUS_INTERFACE = "fi.w1.wpa_supplicant1"
    SUPPLICANT_DBUS_INTERFACE = SUPPLICANT1_DBUS_INTERFACE

    INTERFACE_DBUS_INTERFACE = "fi.w1.wpa_supplicant1.Interface"
    NETWORK_DBUS_INTERFACE = "fi.w1.wpa_supplicant1.Network"
    BSS_DBUS_INTERFACE = "fi.w1.wpa_supplicant1.BSS"
    DBUS_PROPERTIES_INTERFACE = "org.freedesktop.DBus.Properties"


async def get_dbus_obj_proxy(*, bus3, dbus_service_name, dbus_obj_path):
    introspection = await bus3.introspect(dbus_service_name, dbus_obj_path)

    proxy = bus3.get_proxy_object(dbus_service_name, dbus_obj_path, introspection)

    return proxy


async def _ensure_interface(*, supplicant_proxy, ifname):
    # DBus interface: "fi.w1.wpa_supplicant1" -> "/fi/w1/wpa_supplicant1" ->
    #   "fi.w1.wpa_supplicant1"
    supplicant_iface = supplicant_proxy.get_interface("fi.w1.wpa_supplicant1")

    # If wpa_supplicant already knows about this WiFi network interface, then
    # use the existing interface object. Otherwise, create a new one.
    try:
        return await supplicant_iface.call_get_interface(ifname)

    except dbus_next.errors.DBusError as e:
        if e.type != "fi.w1.wpa_supplicant1.InterfaceUnknown":
            raise

    # Create a new interface.
    try:
        return await supplicant_iface.call_create_interface(
            {
                "Ifname": dbus_next.Variant("s", ifname),
                "Driver": dbus_next.Variant("s", "nl80211"),
            }
        )
    except dbus_next.errors.DBusError as e:
        if e.type != "fi.w1.wpa_supplicant1.InterfaceExists":
            raise

        # This means we raced someone else and they managed to create the interface at
        # the same time we were doing so. Why even bother handling this case? Let's not.
        raise


async def _init(ifname):
    bus3 = await dbus_next.aio.MessageBus(bus_type=dbus_next.BusType.SYSTEM).connect()
    supplicant1_proxy = await get_dbus_obj_proxy(
        bus3=bus3,
        dbus_service_name=dbus_ids.DBUS_SERVICE_NAME_SUPPLICANT1,
        dbus_obj_path="/fi/w1/wpa_supplicant1",
    )

    ifname_obj_path = await _ensure_interface(
        supplicant_proxy=supplicant1_proxy, ifname=ifname
    )
    logger.info("Using WiFi network interface: %s", ifname_obj_path)

    ifname_proxy = await get_dbus_obj_proxy(
        dbus_service_name=dbus_ids.DBUS_SERVICE_NAME_SUPPLICANT1,
        dbus_obj_path=ifname_obj_path,
    )
    ifname_iface = ifname_proxy.get_interface("fi.w1.wpa_supplicant1.Interface")


async def _async_main(ifname):
    # Initialize.
    await _init(ifname)


def main(wifi_ifname):
    # Set up the logging.
    logging.basicConfig(
        datefmt="%FT%H:%M:%S",
        format="%(asctime)s [%(levelname)s] %(name)s: %(message)s",
        level=logging.DEBUG,
    )

    loop = asyncio.get_event_loop()
    loop.run_until_complete(_async_main(ifname=wifi_ifname))


if __name__ == "__main__":
    main(**vars(_syntax().parse_args()))
