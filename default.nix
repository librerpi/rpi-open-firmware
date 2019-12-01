let
  # temporary hack, this rev is upstream
  pkgs = import (builtins.fetchTarball https://github.com/input-output-hk/nixpkgs/archive/0ee0489d42e.tar.gz) {};
  lib = pkgs.lib;
  vc4 = pkgs.pkgsCross.vc4.extend overlay;
  arm = pkgs.pkgsCross.arm-embedded.extend overlay;
  aarch64 = pkgs.pkgsCross.aarch64-multiplatform;
  overlay = self: super: {
    tlsf = self.stdenv.mkDerivation {
      name = "tlsf";
      src = lib.cleanSource ./tlsf;
    };
    common = self.stdenv.mkDerivation {
      name = "common";
      src = lib.cleanSource ./common;
      propagatedBuildInputs = [ self.tlsf ];
      enableParallelBuilding = true;
    };
    chainloader = arm.stdenv.mkDerivation {
      name = "chainloader";
      src = lib.cleanSource ./arm_chainloader;
      buildInputs = [ self.tlsf self.common ];
      enableParallelBuilding = true;
      installPhase = ''
        $OBJDUMP -t build/arm_chainloader.bin.elf | sort -rk4 | head -n15
        mkdir $out
        cp build/arm_chainloader.bin{,.elf} $out/
        $OBJDUMP -S build/arm_chainloader.bin.elf > $out/chainloader.S
      '';
    };
    firmware = vc4.stdenv.mkDerivation {
      name = "firmware";
      src = lib.cleanSource ./.;
      buildInputs = [ self.common ];
      preBuild = ''
        ln -sv ${arm.chainloader} arm_chainloader/build
      '';
      installPhase = ''
        mkdir $out
        cp build/bootcode.bin{,.elf} $out/
        ln -sv ${arm.chainloader} $out/arm
      '';
    };
  };
in {
  uboot = aarch64.ubootRaspberryPi3_64bit;
  vc4 = {
    inherit (vc4) tlsf firmware common;
  };
  arm = {
    inherit (arm) tlsf chainloader common;
  };
}
