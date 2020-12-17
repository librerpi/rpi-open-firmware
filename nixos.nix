{ pkgs, lib, config, ... }:

let
  sources = import ./nix/sources.nix;
  self = import ./. {};
  customKernel = pkgs.linux_rpi2.override {
    extraConfig = ''
      DEBUG_LL y
      DEBUG_BCM2836 y
      DEBUG_UART_PL01X y
      DEBUG_UNCOMPRESS y
      EARLY_PRINTK y
      RASPBERRYPI_FIRMWARE n
    '';
  };
  customKernelPackages = pkgs.linuxPackagesFor customKernel;
in {
  imports = [
    (sources.nixpkgs + "/nixos/modules/profiles/minimal.nix")
    ./bootloader.nix
  ];
  environment = {
    systemPackages = [
      self.arm7.hs.hs-gpio.cexes.hs-gpio
      pkgs.i2c-tools
      pkgs.screen
      pkgs.ncdu
      pkgs.usbutils
      self.arm7.utils
      pkgs.dtc
      #pkgs.gdb
    ];
    etc.busybox.source = pkgs.busybox;
  };
  nixpkgs = {
    crossSystem.system = "armv7l-linux";
    overlays = [
      (self: super: {
        libgpgerror = self.callPackage "${sources.nixpkgs-old}/pkgs/development/libraries/libgpg-error/default.nix" {};
      })
    ];
  };
  networking = {
    useDHCP = false;
    interfaces.eth0.useDHCP = true;
    nameservers = [ "192.168.2.1" ];
    firewall.enable = false;
  };
  boot = {
    kernelPackages = customKernelPackages;
    kernelParams = [
      "print-fatal-signals=1"
      "console=ttyAMA0,115200"
      "earlyprintk"
      "printk.devkmsg=on"
      "boot.shell_on_fail"
      ''dyndbg="file bcm2835-mailbox.c +p"''
      "iomem=relaxed"
    ];
    loader = {
      grub.enable = false;
      openpi = {
        enable = true;
      };
    };
  };
  fonts.fontconfig.enable = false;
  security.polkit.enable = false;
  security.rngd.enable = false;
  services = {
    udisks2.enable = lib.mkForce false;
    openssh.enable = true;
    ntp.enable = false; # cross-compile breaks it
    avahi.enable = true;
    prometheus.exporters.node.enable = true;
  };
  programs.bash.enableCompletion = false;
  users.users.root = {
    openssh.authorizedKeys.keys = [
      "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC34wZQFEOGkA5b0Z6maE3aKy/ix1MiK1D0Qmg4E9skAA57yKtWYzjA23r5OCF4Nhlj1CuYd6P1sEI/fMnxf+KkqqgW3ZoZ0+pQu4Bd8Ymi3OkkQX9kiq2coD3AFI6JytC6uBi6FaZQT5fG59DbXhxO5YpZlym8ps1obyCBX0hyKntD18RgHNaNM+jkQOhQ5OoxKsBEobxQOEdjIowl2QeEHb99n45sFr53NFqk3UCz0Y7ZMf1hSFQPuuEC/wExzBBJ1Wl7E1LlNA4p9O3qJUSadGZS4e5nSLqMnbQWv2icQS/7J8IwY0M8r1MsL8mdnlXHUofPlG1r4mtovQ2myzOx clever@nixos"
    ];
  };
}
