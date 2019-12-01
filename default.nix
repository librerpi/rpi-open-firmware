let
  # temporary hack, this rev is upstream
  pkgs = import (builtins.fetchTarball https://github.com/input-output-hk/nixpkgs/archive/0ee0489d42e.tar.gz) {};
  vc4 = pkgs.pkgsCross.vc4;
  arm = pkgs.pkgsCross.arm-embedded;
  aarch64 = pkgs.pkgsCross.aarch64-multiplatform;
  arm_chainloader = arm.stdenv.mkDerivation {
    name = "chainloader";
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
    name = "firmware";
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
  uboot = aarch64.ubootRaspberryPi3_64bit;
}
