# pi-pico-stock-console

A stock ticker mini console built with the pi pico 2 w

## Build

Needs [mise](https://mise.jdx.dev) and `git`.

```sh
mise run bootstrap
cmake -S . -B build -DPICO_BOARD=pico2_w
cmake --build build
```

`bootstrap` installs cmake, ninja and python via mise, then the Pico SDK, Arm
toolchain, Risc-V toolchain, pioasm, picotool and OpenOCD into `.pico-sdk/`.
