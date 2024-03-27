# Intro to Zephyr OS

This repo is for learning [Zephyr OS](https://www.zephyrproject.org/) and its uses. From Zephyr's about page, some of the main uses of Zephyr are the following:

  * Highly configurable and modular
  * Supports cooperative and preemptive threading
  * Memory and resources are typically statically allocated, benefitting long-running systems
  * An integrated device driver interface

This repo will be using the STM32F411E discovery board, but can be updated to work with any supported boards. This is developed in VS Code and debugged using the Cortex-Debug 

## Setup

Following the setup from [Practical Zephyr - Zephyr Basics (Part 1)](https://interrupt.memfault.com/blog/practical_zephyr_basics#practical-zephyr-zephyr-basics-part-1) and [Zephyr Git Repo](https://github.com/zephyrproject-rtos/zephyr). After importing the Zephyr repo, a setup script is created instead of the `env.sh` script, provided by Zephyr. This allows the setup be used outside of the Zephyr root environment. The setup script used for this application:

```
#/bin/bash

USER_DIR=${USER}
ZEPHYR_INSTALL_DIR=${USER_DIR}/zephyrproject
ZEPHYR_VERSION=0.16.4

export ZEPHYR_TOOLCHAIN_VARIANT=zephyr
export ZEPHYR_SDK_INSTALL_DIR=${USER_DIR}/zephyr-sdk-${ZEPHYR_VERSION}
source $ZEPHYR_INSTALL_DIR/zephyr/zephyr-env.sh
west zephyr-export
```

## Creating the application

Zephyr allows for different application types depending on the location of the application in relation to Zephyr's sources. The different types of applications are: freestanding, workspace, and Zephyr repository. All types of application types are explained in the Memfault article [^1]. 

[^1]: https://interrupt.memfault.com/blog/practical_zephyr_basics#practical-zephyr-zephyr-basics-part-1

To create the freestanding application for the `button` project, the skeleton of the application is as follows:
```
button
|- inc
|  -- main.h
|  -- button.h
|  -- encoder.h
|  -- led.h
|- src
|  -- main.c
|  -- button.c
|  -- encoder.c
|  -- led.c
|- CMakeLists.txt
|- prj.conf
```

## Building, flashing, and debugging

### Building
For this application, Zephyr's west will be used to help manage Zephyr extensions. West needs to know which board is being used to know which kconfig and devicetree to use. Kconfig, the configuration system, contains the symbols that specify which dependencies are needed to make the configurations valid. More about kconfigs can be read in the Configuration System [^2] portion of the Zephyr documentation.

[^2]: https://docs.zephyrproject.org/latest/build/kconfig/index.html

To build with west for the STM32F411E Discovery Board, navigate to the workspace directory and use the following command:

```
west build -b stm32f411e_disco -d ../build
```

Where `stm32f411e_disco` is the board that is found in the device tree. The board can also be passed as an environmental variable in the setup file. Note that after the build directory has been created, it is no longer necessary to pass the board as an argument.

```
export BOARD=stm32f411e_disco
```

### Flashing

Now that the build directory has been created, the board can now be flashed. To flash the board using west, running west flash will program the board:

```
west flash -d ../build
```

### Debugging

To debug in VS Code, the [Cortex-Debug](https://marketplace.visualstudio.com/items?itemName=marus25.cortex-debug) extension is used to debug the board through the STLink. STLink can be installed through the 
[STLink Github](https://github.com/stlink-org/stlink/tree/testing) and additional information on how to setup debugging in VS Code for STLink and JLink can be found in [Debugging STM32 in VSCODE with stlink and jlink | VIDEO 45](https://www.youtube.com/watch?v=g2Kf6RbdrIs&t=266s).
