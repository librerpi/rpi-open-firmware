{ pkgs, lib, ... }:

let
  sources = import ./nix/sources.nix;
in {
  imports = [
    (sources.nixpkgs + "/nixos/modules/profiles/minimal.nix")
  ];
  nixpkgs = {
    system = "armv7l-linux";
    overlays = [
      (self: super: {
        p11-kit = super.p11-kit.overrideDerivation (old: {
          doCheck = false;
        });
        e2fsprogs = super.e2fsprogs.overrideDerivation (old: {
          doCheck = false;
        });
      })
    ];
  };
  fileSystems = {
    "/" = {
      device = "/dev/sda1";
    };
  };
  boot = {
    loader = {
      grub.enable = false;
    };
    initrd = {
      extraUtilsCommands = ''
        copy_bin_and_libs ${pkgs.strace}/bin/strace
        cp ${pkgs.stdenv.cc.cc}/armv7l-unknown-linux-gnueabihf/lib/libgcc_s.so $out/lib/ -v
      '';
      network = {
        enable = true;
        ssh = {
          enable = true;
          authorizedKeys = [
            "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC34wZQFEOGkA5b0Z6maE3aKy/ix1MiK1D0Qmg4E9skAA57yKtWYzjA23r5OCF4Nhlj1CuYd6P1sEI/fMnxf+KkqqgW3ZoZ0+pQu4Bd8Ymi3OkkQX9kiq2coD3AFI6JytC6uBi6FaZQT5fG59DbXhxO5YpZlym8ps1obyCBX0hyKntD18RgHNaNM+jkQOhQ5OoxKsBEobxQOEdjIowl2QeEHb99n45sFr53NFqk3UCz0Y7ZMf1hSFQPuuEC/wExzBBJ1Wl7E1LlNA4p9O3qJUSadGZS4e5nSLqMnbQWv2icQS/7J8IwY0M8r1MsL8mdnlXHUofPlG1r4mtovQ2myzOx clever@nixos"
          ];
        };
      };
    };
  };
  fonts.fontconfig.enable = false;
  security.polkit.enable = false;
  services.udisks2.enable = lib.mkForce false;
}
