A nix flake has been provided to manage all software dependancies (you can install nix here: https://nix.dev/install-nix.html). With nix installed, run 'nix develop'. This will download and build all the required tools, and make then available to that dev shell. This will not install them globally on your device, and will not override any version of the toolchains you already have.

If you use direnv (or nix-direnv [https://github.com/nix-community/nix-direnv])then the included .envrc file will automatically start the nix shell 

The hex file can be programmed onto the board with the following command
    sudo openocd -f interface/cmsis-dap.cfg -f src/Board/V71_EK/atmel_samv71_xplained_ultra.cfg -c "adapter speed 5000" -c "program bin/V71_EK_hello_world.elf verify reset exit"

The board can be interfaced with over a serial monitor, for example:
    sudo tio -b 115200 /dev/ttyACM0 --local-echo
