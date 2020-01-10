{ kernelOverride ? null
}:

let
  # temporary hack, this rev is upstream
  pkgs = import
  (builtins.fetchTarball https://github.com/input-output-hk/nixpkgs/archive/0ee0489d42e.tar.gz)
  #/home/clever/apps/rpi/nixpkgs
  {};
  lib = pkgs.lib;
  vc4 = pkgs.pkgsCross.vc4.extend overlay;
  arm = pkgs.pkgsCross.arm-embedded.extend overlay;
  arm7 = pkgs.pkgsCross.armv7l-hf-multiplatform.extend overlay;
  arm6 = pkgs.pkgsCross.raspberryPi.extend overlay;
  aarch64 = pkgs.pkgsCross.aarch64-multiplatform;
  x86_64 = pkgs.extend overlay;
  overlay = self: super: {
    tlsf = self.stdenv.mkDerivation {
      name = "tlsf";
      src = lib.cleanSource ./tlsf;
    };
    #linux_rpi2 = super.linux_rpi2.overrideAttrs (drv: {
      #nativeBuildInputs = drv.nativeBuildInputs ++ (with self; [ ncurses pkgconfig ]);
    #});
    common = self.stdenv.mkDerivation {
      name = "common";
      src = lib.cleanSource ./common;
      propagatedBuildInputs = [ self.tlsf ];
      enableParallelBuilding = true;
    };
    notc = self.stdenv.mkDerivation {
      name = "notc";
      src = lib.cleanSource ./notc;
      propagatedBuildInputs = [];
      enableParallelBuilding = true;
    };
    chainloader = arm.stdenv.mkDerivation {
      name = "chainloader";
      src = lib.cleanSource ./arm_chainloader;
      buildInputs = [ self.tlsf self.common self.notc ];
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
      src = lib.cleanSource ./firmware;
      buildInputs = [ self.common self.notc ];
      preBuild = ''
        mkdir arm_chainloader
        ln -s ${arm.chainloader} arm_chainloader/build
      '';
      enableParallelBuilding = true;
      dontPatchELF = true;
      dontStrip = true;
      installPhase = ''
        mkdir -p $out/nix-support
        cp build/bootcode.{bin,elf} $out/
        cp start4.elf $out/
        ln -s ${arm.chainloader} $out/arm
        $OBJDUMP -d $out/bootcode.elf > $out/bootcode.S
        #$STRIP $out/start4.elf
        cat <<EOF > $out/nix-support/hydra-metrics
        bootcode.bin $(stat --printf=%s $out/bootcode.bin) bytes
        bootcode.elf $(stat --printf=%s $out/bootcode.elf) bytes
        EOF
      '';
    };
    script = pkgs.writeTextFile {
      name = "init";
      text = ''
        #!${self.busybox}/bin/ash
        export PATH=/bin
        mknod /dev/kmsg c 1 11
        exec > /dev/kmsg 2>&1
        mount -t proc proc proc
        mount -t sysfs sys sys
        mount -t devtmpfs dev dev
        mount -t debugfs debugfs /sys/kernel/debug
        exec > /dev/ttyAMA0 2>&1 < /dev/ttyAMA0
        /bin/sh > /dev/ttyAMA0 < /dev/ttyAMA0
        echo sh failed
      '';
      executable = true;
    };
    myinit = self.stdenv.mkDerivation {
      name = "myinit";
      nativeBuildInputs = [ x86_64.nukeReferences ];
      buildCommand = ''
        $CC ${./my-init.c} -o $out
        nuke-refs -e ${self.stdenv.cc.libc.out} $out
      '';
    };
    initrd = self.makeInitrd {
      contents = [
        {
          object = "${self.busybox}/bin";
          symlink = "/bin";
        }
        {
          object = self.script;
          symlink = "/init";
        }
      ];
    };
    test-script = pkgs.writeShellScript "test-script" ''
      #!${self.stdenv.shell}

      ${self.qemu}/bin/qemu-system-x86_64 -kernel ${self.linux}/bzImage -initrd ${self.initrd}/initrd -nographic -append 'console=ttyS0,115200'
    '';
  };
  bootdir = pkgs.runCommand "bootdir" { buildInputs = [ pkgs.dtc ]; } ''
    mkdir $out
    cd $out
    cp ${vc4.firmware}/bootcode.elf bootcode.bin
    echo print-fatal-signals=1 console=ttyAMA0,115200 earlyprintk loglevel=7 root=/dev/mmcblk0p2 printk.devkmsg=on > cmdline.txt
    dtc ${./rpi3.dts} -o rpi.dtb
    #cp {./bcm2837-rpi-3-b.dtb} rpi.dtb
    ${if kernelOverride == null then ''
      cp ${arm7.linux_rpi2}/zImage zImage
    '' else ''
      cp ${kernelOverride} zImage
    ''}
    echo bootdir is $out
  '';
  dtbFiles = pkgs.runCommand "dtb-files" { buildInputs = [ pkgs.dtc pkgs.strace pkgs.stdenv.cc.cc ]; src = ./dts; } ''
    unpackPhase
    cd $sourceRoot
    mkdir $out
    gcc -v -E -x assembler-with-cpp bcm2837-rpi-3-b.dts -o preprocessed.dts -I ${arm7.linux_rpi2.src}/include/
    cat preprocessed.dts | egrep -v '^#' > $out/rpi3b.dts
    echo compiling
    dtc $out/rpi3b.dts -o $out/rpi3b.dtb
  '';
  helper = pkgs.writeShellScript "helper" ''
    set -e
    set -x
    mount -v /dev/mmcblk0p1 /mnt
    cp -v ${bootdir}/* /mnt/
    ls -ltrh /mnt/
    umount /mnt
  '';
in pkgs.lib.fix (self: {
  inherit bootdir helper dtbFiles;
  aarch64 = {
    inherit (aarch64) ubootRaspberryPi3_64bit linux_rpi3;
  };
  vc4 = {
    inherit (vc4) tlsf firmware common notc;
  };
  arm = {
    inherit (arm) tlsf chainloader common notc;
  };
  arm6 = {
    inherit (arm6) initrd;
  };
  arm7 = {
    inherit (arm7) linux_rpi2 busybox initrd;
  };
  x86_64 = {
    inherit (x86_64) test-script;
  };
  # make $makeFlags menuconfig
  # time make $makeFlags zImage -j8
  kernelShell = arm7.linux_rpi2.overrideDerivation (drv: {
    nativeBuildInputs = drv.nativeBuildInputs ++ (with x86_64; [ ncurses pkgconfig ]);
    shellHook = ''
      addToSearchPath PKG_CONFIG_PATH ${x86_64.ncurses.dev}/lib/pkgconfig
      echo to configure: 'make $makeFlags menuconfig'
      echo to build: 'time make $makeFlags zImage -j8'
    '';
  });
})
