{ kernelOverride ? null
}:

let
  sources = import ./nix/sources.nix;
  haskellNix = import sources."haskell.nix" {};
  pkgs = import sources.nixpkgs haskellNix.nixpkgsArgs;
  lib = pkgs.lib;
  vc4 = pkgs.pkgsCross.vc4.extend overlay;
  arm = pkgs.pkgsCross.arm-embedded.extend overlay;
  arm7 = pkgs.pkgsCross.armv7l-hf-multiplatform.extend overlay;
  arm6 = pkgs.pkgsCross.raspberryPi.extend overlay;
  aarch64 = pkgs.pkgsCross.aarch64-multiplatform.extend overlay;
  arm64 = pkgs.pkgsCross.aarch64-embedded.extend overlay;
  x86_64 = pkgs.extend overlay;
  trimHaskellNixTree = input: filter:
  let
    f1 = k: v: {
      cexes = input.config.hsPkgs.${k}.components.exes;
    };
  in lib.mapAttrs f1 filter;
  overlay = self: super: {
    bcm2835 = self.callPackage ./bcm2835.nix {};
    tlsf = self.stdenv.mkDerivation {
      name = "tlsf";
      src = lib.cleanSource ./tlsf;
    };
    #linux_rpi2 = super.linux_rpi2.overrideAttrs (drv: {
      #nativeBuildInputs = drv.nativeBuildInputs ++ (with self; [ ncurses pkgconfig ]);
    #});
    common = self.callPackage ./common {};
    # nix-shell -p haskellPackages.cabal-install haskellPackages.ghc --run "cabal new-configure"
    # nt/bin/plan-to-nix --output . --plan-json dist-newstyle/cache/plan.json --cabal-project cabal.project
    pkgSet = self.haskell-nix.mkCabalProjectPkgSet {
      plan-pkgs = import ./pkgs.nix;
    };
    uart-manager = self.stdenv.mkDerivation {
      name = "uart-manager";
      src = ./uart-manager;
    };
    utils = self.callPackage ./utils {};
    notc = self.stdenv.mkDerivation {
      name = "notc";
      src = lib.cleanSource ./notc;
      propagatedBuildInputs = [];
      enableParallelBuilding = true;
      hardeningDisable = [ "fortify" "stackprotector" ];
      dontStrip = true;
    };
    libdrm = super.libdrm.override { withValgrind = false; };
    chainloader = arm.stdenv.mkDerivation {
      name = "chainloader";
      src = lib.cleanSource ./arm_chainloader;
      buildInputs = [ self.tlsf self.common self.notc ];
      enableParallelBuilding = true;
      installPhase = ''
        #$OBJDUMP -t build/arm_chainloader.bin.elf | sort -rk4
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
    bootdir = self.buildPackages.runCommandCC "bootdir" { buildInputs = [ pkgs.dtc ]; } ''
      mkdir $out
      cd $out
      cp ${vc4.firmware}/bootcode.bin bootcode.bin
      echo print-fatal-signals=1 console=ttyAMA0,115200 earlyprintk loglevel=7 root=/dev/mmcblk0p2 printk.devkmsg=on > cmdline.txt
      builddtb() {
        cc -x assembler-with-cpp -E $1 -o temp
        egrep -v '^#' < temp > temp2
        dtc temp2 -o $2
        rm temp temp2
      }
      builddtb ${./rpi2.dts} rpi2.dtb
      builddtb ${./rpi3.dts} rpi3.dtb
      ${if kernelOverride == null then ''
        cp ${arm7.linux_rpi2}/zImage zImage
      '' else ''
        cp ${kernelOverride} zImage
      ''}
      echo bootdir is $out
    '';
    diskImage = let
      zImage = pkgs.fetchurl {
        url = "https://ext.earthtools.ca/videos/private/rpi/zImage-2020-05-19";
        sha256 = "09kijy3rrwzf6zgrq3pbww9267b1dr0s9rippz7ygk354lr3g7c8";
      };
      eval = import (self.path + "/nixos") {
        configuration = {
          imports = [
            ./nixos.nix
            (pkgs.path + "/nixos/modules/installer/cd-dvd/sd-image.nix")
          ];
          users.users.root.initialPassword = "password";
          sdImage = {
            firmwareSize = 128;
            populateRootCommands = ''
            '';
            populateFirmwareCommands = ''
              for file in rpi2.dtb rpi3.dtb bootcode.bin; do
                cp -v ${self.bootdir}/$file firmware/
              done
              cp -vL ${zImage} firmware/zImage
              cp -vL ${eval.config.system.build.toplevel}/initrd firmware/initrd
              echo "systemConfig=${eval.config.system.build.toplevel} init=${eval.config.system.build.toplevel}/init $(cat ${eval.config.system.build.toplevel}/kernel-params)" > firmware/cmdline.txt
            '';
          };
        };
      };
    in eval.config.system.build.sdImage;
    helper = pkgs.writeShellScript "helper" ''
      set -e
      set -x
      mount -v /dev/mmcblk0p1 /mnt
      cp -v ${self.bootdir}/* /mnt/
      ls -ltrh /mnt/
      umount /mnt
    '';
    test-script = pkgs.writeShellScript "test-script" ''
      #!${self.stdenv.shell}

      ${self.qemu}/bin/qemu-system-x86_64 -kernel ${self.linux}/bzImage -initrd ${self.initrd}/initrd -nographic -append 'console=ttyS0,115200'
    '';
  };
  dtbFiles = pkgs.runCommand "dtb-files" { buildInputs = [ pkgs.dtc pkgs.strace pkgs.stdenv.cc.cc ]; src = ./dts; } ''
    unpackPhase
    cd $sourceRoot
    mkdir $out
    gcc -v -E -x assembler-with-cpp bcm2837-rpi-3-b.dts -o preprocessed.dts -I ${arm7.linux_rpi2.src}/include/
    cat preprocessed.dts | egrep -v '^#' > $out/rpi3b.dts
    echo compiling
    dtc $out/rpi3b.dts -o $out/rpi3b.dtb
  '';
  nixos = (import (sources.nixpkgs + "/nixos") { configuration = ./nixos-with-fs.nix; });
  testcycle = pkgs.writeShellScript "testcycle" ''
    set -e
    scp ${vc4.firmware}/bootcode.bin root@router.localnet:/tftproot/open-firmware/bootcode.bin
    exec ${x86_64.uart-manager}/bin/uart-manager
  '';
  filterArmUserlandPackages = input: {
    inherit (input) initrd bcm2835 busybox openssl linux_rpi2 diskImage bootdir utils nix raspberrypi-tools;
    #inherit (aarch64) ubootRaspberryPi3_64bit linux_rpi3 bcm2835;
    hs = trimHaskellNixTree input.pkgSet { hs-gpio = true; };
  };
in pkgs.lib.fix (self: {
  inherit dtbFiles testcycle;
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
  arm6 = filterArmUserlandPackages arm6;
  arm7 = filterArmUserlandPackages arm7;
  aarch64 = filterArmUserlandPackages aarch64;
  x86_64 = {
    inherit (x86_64) test-script uart-manager;
    haskell-nix = {
      inherit (x86_64.haskell-nix) nix-tools;
    };
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
  test1 = pkgs.callPackage (pkgs.path + "/nixos/lib/make-system-tarball.nix") {
    contents = [];
    storeContents = [
      { symlink = "/gdb"; object = arm6.gdb; }
      { symlink = "/strace"; object = arm6.strace; }
    ];
  };
})
