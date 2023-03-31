# Light-Emitting Diode

## LEDs don't follow Ohm's law

See this [_accounting for LED resistance_](https://electronics.stackexchange.com/q/76367).

An LED's current/voltage relationship graph is as follows:

![LED current/voltage relationship graph](https://i.stack.imgur.com/gcbdC.png)

- LEDs do have resistance. It's small, but not insignificant. Only in some situations would it be valid to simply say they have no resistance.
- If the voltage across the diode is greater than `Vd`, the diode behaves like a constant voltage drop (i.e. it will allow whatever current through to maintain `V = Vd`).
- If the voltage is less than `Vd` but greater than the breakdown voltage `Vbr`, the diode doesn't conduct.
- If the reverse bias voltage is above the breakdown voltage `Vbr`, the diode again becomes conducting, and will allow whatever current through to maintain `V = Vbr`.
