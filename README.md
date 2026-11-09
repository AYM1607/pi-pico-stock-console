# pi-pico-stock-console

A stock ticker mini console built with the pi pico 2 w

## Build

Needs [mise](https://mise.jdx.dev) and `git`.

```sh
mise run bootstrap
mise run configure
mise run build
mise run program
```

`bootstrap` installs cmake, ninja and python via mise, then the Pico SDK, Arm
toolchain, Risc-V toolchain, pioasm, picotool and OpenOCD into `.pico-sdk/`.
