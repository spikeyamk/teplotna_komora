# Teplotná komora - Tímový projekt
## Quickstart

### Prerequisites

- cmake >= v3.22.0
- ninja >= v1.11.0
- gcc-arm-none-eabi >= v12.3.0
- doxygen >= 1.12.0 (optional for docs generation)
- PuTTY/minicom or similar program to read serial COM port
- STM32CubeProg >= v2.17.0 used for flashing FW
- STM32CubeIDE (optional Eclipse based IDE used for programming and debugging STM32 projects)
- STM32CubeMX (optional auto project generator)

#### Installation Windows

You can use the `winget` pkg manager to get the previously mentioned prerequisites via command:
```
winget install Kitware.CMake Ninja-build.Ninja Arm.GnuArmEmbeddedToolchain DimitriVanHeesch.Doxygen PuTTY.PuTTY
```

Make sure to restart the PowerShell session after installation or add additional missing installation directories to your `$PATH` or run the PowerShell script under `misc\env.ps1` that adds `gcc-arm-none-eabi` and `STM32CubeProg` installed in default locations temporarily to your `$PATH` until you close the current PowerShell session.

#### Installation Ubuntu

```
# apt install cmake ninja gcc-arm-none-eabi doxygen minicom
```

#### STM32Cube Installation

If you plan on using `STM32CubeIDE` `STM32CubeProg` and  `gcc-arm-none-eabi` comes bundled with it.

Go ahead and create a [myST](https://my.st.com) account, the stupid ST webiste won't let you download any STM32Cube Development Tools without it.

You can get them under following links:
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html#st-get-software)
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html#st-get-software)
- [STM32CubeProg](https://www.st.com/en/development-tools/stm32cubeprog.html#st-get-software)

### Usage

#### Configure

Configure the project by issuing following command:
```
cmake -S . -B build -G Ninja
```

#### Compile

Compile the project by issuing following command:
```
cmake --build build -j [num of CPU threads]
```

#### Flash

To flash the firmware first connect the STLink via USB to your PC and issue command:
```
cmake --build build --target flash
```

#### Read COM on Windows

Press key combination `Win + R` and run: `devmgmt.msc`. Under section `Ports (COM and LPT)` look for `Silicon Labs (COM6)`. Launch `PuTTY` and select the `Serial` radio button, enter `COM6` and baudrate `115200`.

#### Read COM on Ubuntu

List available serial ports:
```
$ ls /dev/ttyACM*
```

Launch minicom and listen to `/dev/ttyACM0`:
```
$ minicom -D /dev/ttyACM0
```

#### Build docs

```
cmake --build build --target docs
```