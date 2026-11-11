# pi-pico-stock-console

A stock ticker mini console built with the pi pico 2 w

## Build

Needs [mise](https://mise.jdx.dev) and `git`.

```sh
mise run bootstrap
mise run configure
mise run build
mise run flash
```

`bootstrap` installs cmake, ninja and python via mise, then the Pico SDK, Arm
toolchain, Risc-V toolchain, pioasm, picotool and OpenOCD into `.pico-sdk/`.

## Monitor

Use screen to read from serial, either through usb virtual or actual uart:

```sh
picocom -b 115200 /dev/tty.usbmodem* 
```

Baud rate is ignored on the usb virtual serial port. In pico-com use C-a, C-x to exit cleanly.

## TODO

- [x] Print through usb / debug port
- [x] Join WiFi network
- [x] Make HTTPS request to get the current price of a stock
- [x] Print raw response to USB
- [x] Extremely simple parser to extract price
- [x] Print just price to USB
- [ ] Minimal http client (lwip wrapper)
- [ ] Display driver with raw framebuffer interface
- [ ] Simple graphics API
  - [ ] BitMap font text
  - [ ] Lines
  - [ ] Rctangles (filled and non-filled)
  - [ ] Circles (filled and non-filled)
- [ ] Expanded parser to handle intraday sample arrays
- [ ] Show intraday stock graph for CRWV
