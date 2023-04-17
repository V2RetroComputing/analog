![V2Logo](docs/v2logo.png)

# ∀2 Retro Computing Analog VGA Card & PicoPal
![PicoPal Logo](docs/PicoPalLogo.png)


### Repo reorg
This repo is now the root of a multi-project tree.
Continue to use this repo to report issues with the hardware or firmware.


### Building the project files
Clone this repo and issue the following command to pull in the submodules
```
# git submodule update --init
```

The build.sh script in the analog-firmware project will build all the firmware variants.
If you wish to build a single variant, check the build.sh script for the syntax for the build folders.

When setting up a new card or upgrading from the older monolithic firmware, it is neccessary to use the flash preloader to format the configuration areas and install the fonts.  This only has to be done once, or if you want to restore the default fonts after uploading new ones.


### Sub-projects
[Flash Preloader](https://github.com/V2RetroComputing/analog-preload) |
[Firmware](https://github.com/V2RetroComputing/analog-firmware) |
[Utilities](https://github.com/V2RetroComputing/analog-utilities) |
[Hardware](https://github.com/V2RetroComputing/analog-hardware) |
[PicoPal V1](https://github.com/V2RetroComputing/picopal-v1)


### Acknowledgements
This is a fork of Mark Aikens [Apple II VGA](https://github.com/markadev/AppleII-VGA/) project with a new board design,
custom logic in an ATF16V8C to handle card select lines, and upgraded firmware with many new features.

Full details on purchasing and screenshots are available at [∀2 Retro Computing](https://www.v2retrocomputing.com/).

