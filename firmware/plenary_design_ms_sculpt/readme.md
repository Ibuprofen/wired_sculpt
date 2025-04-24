# plenary_design_ms_sculpt

![plenary_design_ms_sculpt](imgur.com image replace me!)

*A short description of the keyboard/project*

* Keyboard Maintainer: [Tony Rieker](https://github.com/Ibuprofen)
* Hardware Supported: *The PCBs, controllers supported*
* Hardware Availability: *Links to where you can find this hardware*

Make example for this keyboard (after setting up your build environment):

    make plenary_design_ms_sculpt:default

Flashing example for this keyboard:

    make plenary_design_ms_sculpt:default:flash

See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).

## Bootloader

Enter the bootloader in 2 ways:

* **Bootmagic reset**: Hold down left spacebar and `b` and then plug in the keyboard usb cable. The board will then appear as a removable drive, allowing you to upload firmware.
* **Physical reset button**: To enter the RP2040 bootloader, you need to press and hold the BOOTSEL (or BOOT) button while simultaneously pressing and releasing the RESET button, and then release the BOOTSEL button. The board will then appear as a removable drive, allowing you to upload firmware.
