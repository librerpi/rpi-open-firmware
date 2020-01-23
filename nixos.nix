{ lib, ... }:

{
  nixpkgs.crossSystem.system = "armv7l-linux";
  fileSystems = {
    "/" = {
      device = "/dev/sda1";
    };
  };
  boot.loader = {
    grub.enable = false;
  };
}
