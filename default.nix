{ kernelOverride ? null
}:

let
  sources = import ./nix/sources.nix;
  pkgs = import sources.nixpkgs {};
  lib = pkgs.lib;
  vc4 = pkgs.pkgsCross.vc4.extend overlay;
  arm = pkgs.pkgsCross.arm-embedded.extend overlay;
  arm7 = pkgs.pkgsCross.armv7l-hf-multiplatform.extend overlay;
  arm6 = pkgs.pkgsCross.raspberryPi.extend overlay;
  aarch64 = pkgs.pkgsCross.aarch64-multiplatform.extend overlay;
  arm64 = pkgs.pkgsCross.aarch64-embedded.extend overlay;
  x86_64 = pkgs.extend overlay;
  hsoverlay = hself: hsuper: {
    HPi = hself.callPackage ./HPi.nix {};
  };
  overlay = self: super: {
    bcm2835 = self.callPackage ./bcm2835.nix {};
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
      hardeningDisable = [ "fortify" "stackprotector" ];
      dontStrip = true;
    };
    myHsPkgs = self.haskellPackages.extend hsoverlay;
    uart-manager = self.stdenv.mkDerivation {
      name = "uart-manager";
      src = ./uart-manager;
    };
    pll-inspector = self.stdenv.mkDerivation {
      name = "pll-inspector";
      unpackPhase = ''
        mkdir source
        cp ${./pll-inspector.cpp} source/pll-inspector.cpp
        sourceRoot=source
      '';
      buildPhase = "$CXX pll-inspector.cpp -o pll-inspector -fpermissive";
      installPhase = ''
        mkdir -pv $out/bin
        cp pll-inspector $out/bin/
      '';
    };
    notc = self.stdenv.mkDerivation {
      name = "notc";
      src = lib.cleanSource ./notc;
      propagatedBuildInputs = [];
      enableParallelBuilding = true;
      hardeningDisable = [ "fortify" "stackprotector" ];
      dontStrip = true;
    };
    chainloader = arm.stdenv.mkDerivation {
      name = "chainloader";
      src = lib.cleanSource ./arm_chainloader;
      buildInputs = [ self.tlsf self.common self.notc ];
      enableParallelBuilding = true;
      installPhase = ''
        $OBJDUMP -t build/arm_chainloader.bin.elf | sort -rk4
        mkdir -p $out/nix-support
        cp build/arm_chainloader.bin{,.elf} build/chainloader.map $out/
        $OBJDUMP -S build/arm_chainloader.bin.elf > $out/chainloader.S
        cat <<EOF > $out/nix-support/hydra-metrics
        arm_chainloader.bin $(stat --printf=%s $out/arm_chainloader.bin) bytes
        EOF
      '';
    };
    chainloader64 = arm64.stdenv.mkDerivation {
      name = "chainloader64";
      src = lib.cleanSource ./arm64;
      buildInputs = [ self.common self.notc ];
      hardeningDisable = [ "fortify" "stackprotector" ];
      enableParallelBuilding = true;
      dontStrip = true;
    };
    firmware = vc4.stdenv.mkDerivation {
      name = "firmware";
      src = lib.cleanSource ./firmware;
      buildInputs = [ self.common self.notc ];
      nativeBuildInputs = [ x86_64.uart-manager ];
      preBuild = ''
        mkdir -p arm_chainloader
        rm arm_chainloader/build || true
        ln -s ${arm.chainloader} arm_chainloader/build
      '';
      enableParallelBuilding = true;
      dontPatchELF = true;
      dontStrip = true;
      installPhase = ''
        mkdir -p $out/nix-support
        cp build/bootcode.{bin,elf,map} $out/
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
    raspi-gpio = self.stdenv.mkDerivation {
      name = "raspi-gpio";
      src = self.fetchFromGitHub {
        owner = "RPi-Distro";
        repo = "raspi-gpio";
        rev = "4edfde183ff3ac9ed66cdc015ae25e45f3a5502d";
        sha256 = "0246m7sh04nbdqmvfgkf456ah0c07qhy0ij99dyqy906df3rvjgy";
      };
    };
    initrd-tools = self.buildEnv {
      name = "initrd-tools";
      paths = [ self.raspi-gpio self.busybox ];
    };
    initrd = self.makeInitrd {
      contents = [
        {
          object = "${self.initrd-tools}/bin";
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
    cp ${vc4.firmware}/bootcode.bin bootcode.bin
    echo print-fatal-signals=1 console=ttyAMA0,115200 earlyprintk loglevel=7 root=/dev/mmcblk0p2 printk.devkmsg=on > cmdline.txt
    dtc ${./rpi2.dts} -o rpi2.dtb
    dtc ${./rpi3.dts} -o rpi3.dtb
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
  nixos = (import (sources.nixpkgs + "/nixos") { configuration = ./nixos.nix; });
  testcycle = pkgs.writeShellScript "testcycle" ''
    set -e
    scp ${vc4.firmware}/bootcode.bin root@router.localnet:/tftproot/open-firmware/bootcode.bin
    exec ${x86_64.uart-manager}/bin/uart-manager
  '';
in pkgs.lib.fix (self: {
  inherit bootdir helper dtbFiles testcycle;
  aarch64 = {
    inherit (aarch64) ubootRaspberryPi3_64bit linux_rpi3 bcm2835;
  };
  arm64 = {
    inherit (arm64) chainloader64 common;
  };
  vc4 = {
    inherit (vc4) tlsf firmware common notc;
    #gdb = vc4.buildPackages.gdb;
  };
  arm = {
    inherit (arm) tlsf chainloader common notc;
  };
  arm6 = {
    inherit (arm6) initrd bcm2835;
  };
  arm7 = {
    inherit (arm7) linux_rpi2 busybox initrd openssl pll-inspector bcm2835;
    myHsPkgs = {
      inherit (arm7.myHsPkgs) HPi;
    };
  };
  x86_64 = {
    inherit (x86_64) test-script uart-manager;
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
  aarch64-shell = x86_64.stdenv.mkDerivation {
    name = "aarch64-shell";
    buildInputs = [
      x86_64.ddd
      arm7.buildPackages.gdb
      aarch64.buildPackages.gdb
      x86_64.telnet
    ];
  };
  uboot-shell = aarch64.ubootRaspberryPi3_64bit.overrideAttrs (drv: {
    # export NIX_BUILD_LDFLAGS="${NIX_BUILD_LDFLAGS} -lncursesw"
    # make $makeFlags
    nativeBuildInputs = drv.nativeBuildInputs ++ (with x86_64; [ ncurses pkgconfig ]);
  });
  nixos = {
    inherit (nixos) system;
    inherit (nixos.config.system.build) initialRamdisk;
  };
})
