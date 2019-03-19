# Power Management

## Overview

I started to learn about the power management because the [Small PC](http://smallpc.com/pcswoutfans.php) we were using for the project seemed to suffer from performance downgrade due to poor CPU radiation.

Thermal status monitoring is part of power management. Modern power management (as of 2019) technology is `Advanced Configuration and Power Interface (ACPI)`. See [1] for its latest specification (released in January 2019).

## Notes

According to [1]:

> The ACPI thermal model is based around conceptual platform regions called thermal zones that physically contain devices, thermal sensors, and cooling controls. Generally speaking, the entire platform is one large thermal zone, but the platform can be partitioned into several ACPI thermal zones if necessary to enable optimal thermal management.

Figure 11-61 "ACPI Thermal Zone" in [1] visualizes the concept.

The command to show the thermal zones' temperature is `paste <(cat /sys/class/thermal/thermal_zone*/type) <(cat /sys/class/thermal/thermal_zone*/temp) | column -s $'\t' -t | sed 's/\(.\)..$/.\1°C/'` which gives output like below:

```
acpitz        88.0°C
acpitz        27.8°C
acpitz        29.8°C
x86_pkg_temp  89.0°C
```

## References

- [1] [Advanced Configuration and Power Interface (ACPI) Specification (Version 6.3, January 2019)](https://uefi.org/sites/default/files/resources/ACPI_6_3_final_Jan30.pdf)
- [2] [Generic Thermal Sysfs driver How To](https://www.kernel.org/doc/Documentation/thermal/sysfs-api.txt)
- [3] [ACPI Device Tree - Representation of ACPI Namespace](https://www.kernel.org/doc/Documentation/acpi/namespace.txt)
