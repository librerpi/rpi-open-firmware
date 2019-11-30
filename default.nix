let
  pkgs = import (builtins.fetchTarball https://github.com/nixos/nixpkgs/archive/0ee0489d42e.tar.gz) {};
  vc4 = pkgs.pkgsCross.vc4;
  arm = pkgs.pkgsCross.arm-embedded;
  arm_chainloader = arm.stdenv.mkDerivation {
    name = "name";
    src = ./.;
    postUnpack = ''
      sourceRoot=$sourceRoot/arm_chainloader
    '';
    installPhase = ''
      mkdir $out
      cp build/arm_chainloader.bin{,.elf} $out/
    '';
  };
  firmware = vc4.stdenv.mkDerivation {
    name = "name";
    src = ./.;
    preBuild = ''
      ln -sv ${arm_chainloader} arm_chainloader/build
    '';
    installPhase = ''
      mkdir $out
      cp build/bootcode.bin{,.elf} $out/
    '';
  };
in {
  inherit arm_chainloader firmware;
}
