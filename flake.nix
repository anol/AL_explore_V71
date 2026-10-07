{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/26.05";
  };
  outputs =
    { nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in
    {
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          cmake
          openocd
          gcc-arm-embedded
          tio
          (pkgs.python3.withPackages (
            python-pkgs: with python-pkgs; [
              argparse
              pyserial
              time
              crcmod
              numpy
              readchar
              dash
            ]
          ))
        ];
      };
    };
}
