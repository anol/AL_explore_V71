# How to un-brick NORM
## First ...
1. Check the power supply: it should be set on 36V and show approx. 64mA (approx. 30mA when in halt)
2. Power cycle the JTAG dongle "JLink": unplug/replug the USB connector.

-------------
## The Watchdog reboots repeatedly due to application stuck in HardFaultMemManage
### Problem
You are not able to load a new application build, that has a fix, into the Filestore.
### Solution
Force the Bootloader to do a fallback to the Utility program.
### Procedure
Use J-Link Commander to corrupt the Filestore index, 
to make the bootloader fail loading the image and to do the fallback.
The Filestore is located in MRAM-2, starting at address 0x8000'0000.

`J-Link>mem32 0x80000000 1`

`80000000 = 332E5346`

`J-Link>w4 0x80000000 0`

`Writing 00000000 -> 80000000`


-------------
## The Bootloader is partly overwritten
### Problem
The Bootloader fails to run, but ends in a busy loop or HardFaultMemManage.
### Solution
Use J-Link Commander to load a fresh Bootloader image.
### Procedure
First halt the processor and load the new image, then do a reset and go.

`J-Link>h`

`PC = 21006626, CycleCnt = 488478E6`

`R0 = ....`

`J-Link>loadfile C:\Users\AndersEmilOlsen\Documents\GNORM\Target_NORM_EM_bootloader_4_1_0.hex`

`Downloading file [C:\Users\AndersEmilOlsen\Documents\GNORM\Target_NORM_EM_bootloader_4_1_0.hex]...`

`O.K.`

`J-Link>r`

`Reset delay: 0 ms`

`Reset type NORMAL: Resets core & peripherals ...`

`J-Link>g`


-------------
## The Application image is bad and the Utility program doesn't work
### Problem
You would like to load a fixed application into the Filestore, but fallback doesn't work.
### Solution
Load a new Utility program image into the embedded flash.
### Procedure:
Use the MPLAB X IDE from Microchip and import the Utility program .ELF-file as a new project.
Then use "*Program Device for Production Main Project*".

Please note: MPLAB is not completely compatible with the ELF produced by the ARM GNU Toolchain, 
and as a consequence, 
you need to delete and re-import the ELF-based project if you want to flash a second time. 


-------------
