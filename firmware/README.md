### RP2040 Wired Sculpt Keyboard QMK Firmware

1. Probably start here: https://docs.qmk.fm/newbs#overview
    1. Install QMK cli.
    1. Set up QMK env (`qmk setup` or `qmk setup <github_username>/qmk_firmware` to clone your personal fork.).
    1. `qmk console` with the keyboard plugged in and you should see output: `Ψ Console Connected: Plenary Design plenary_design_ms_sculpt (FEED:0000:1)`.
1. `qmk env` and make sure `$QMK_HOME` looks correct.
1. Symlink or copy `plenary_design_ms_sculpt` directory into the `qmk_firwmare/keyboards`.
    1. (Executed in this directory) `ln -s "$(pwd)/plenary_design_ms_sculpt" "$(qmk env | grep 'QMK_HOME' | cut -d'"' -f2)/keyboards/plenary_design_ms_sculpt"`.
1. `qmk list-keyboards | grep plenary` should list `plenary_design_ms_sculpt` if all is working.
1. `qmk compile -kb plenary_design_ms_sculpt -km default`.
1. Place the keyboard into DFU/Bootloader Mode. (Hold down `left shift` + `b` and then plug in the keyboard usb, you should see an external drive mount).
1. Locate the firmware file (probably in root of the `QMK_HOME`), and with the keyboard in DFU (Bootloader) Mode, drag and drop the `.uf2` firmware file into the mounted drive. It will reboot the keyboard and you should see it connect in the qmk console.
