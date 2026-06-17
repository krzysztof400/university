# L6 Remote - Cable Setup

To make this project work, connect the IR receiver module as follows:

- `VCC` to `5V`
- `GND` to `GND`
- `InfraRed` / signal output to `Arduino pin 2`

Make sure the receiver is mounted in the correct orientation, with the `-` mark on the module matching `GND` on the connector board.

The car motors are already configured in the sketch through the `Wheels` class, so you only need to keep the motor driver wiring consistent with the pins used in `setup()`.

For testing:

- open the Serial Monitor to read the received IR codes,
- watch the LCD, which shows the last received code and unlock status,
- enter the PIN with the remote and confirm with `ENTER` before driving.
