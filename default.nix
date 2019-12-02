let
  # temporary hack, this rev is upstream
  pkgs = import (builtins.fetchTarball https://github.com/input-output-hk/nixpkgs/archive/0ee0489d42e.tar.gz) {};
  lib = pkgs.lib;
  vc4 = pkgs.pkgsCross.vc4.extend overlay;
  arm = pkgs.pkgsCross.armv7l-hf-multiplatform.extend overlay;
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
        mkdir -p $out/nix-support
        cp build/arm_chainloader.bin{,.elf} $out/
        $OBJDUMP -S build/arm_chainloader.bin.elf > $out/chainloader.S
        cat <<EOF > $out/nix-support/hydra-metrics
        arm_chainloader.bin $(stat --printf=%s $out/arm_chainloader.bin) bytes
        EOF
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
        mkdir -pv $out/nix-support
        cp build/bootcode.bin{,.elf} $out/
        ln -sv ${arm.chainloader} $out/arm
        cat <<EOF > $out/nix-support/hydra-metrics
        bootcode.bin $(stat --printf=%s $out/bootcode.bin) bytes
        EOF
      '';
    };
  };
  bootdir = pkgs.runCommand "bootdir" { buildInputs = [ pkgs.dtc ]; } ''
    mkdir $out
    cd $out
    cp ${vc4.firmware}/bootcode.bin .
    echo console=ttyAMA0,115200 > cmdline.txt
    dtc ${./rpi.dts} -o rpi.dtb
    cp ${aarch64.linux_rpi3}/Image zImage
  '';
  helper = pkgs.writeShellScript "helper" ''
    set -e
    set -x
    mount -v /dev/mmcblk0p1 /mnt
    cp -v ${bootdir}/* /mnt/
    umount /mnt
  '';
in {
  inherit bootdir helper;
  aarch64 = {
    inherit (aarch64) ubootRaspberryPi3_64bit linux_rpi3;
  };
  vc4 = {
    inherit (vc4) tlsf firmware common;
  };
  arm = {
    inherit (arm) tlsf chainloader common linux_rpi2;
  };
}
