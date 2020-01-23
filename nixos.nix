{ pkgs, lib, ... }:

{
  imports = [
    <nixpkgs/nixos/modules/profiles/minimal.nix>
  ];
  nixpkgs.crossSystem.system = "armv7l-linux";
  fileSystems = {
    "/" = {
      device = "/dev/sda1";
    };
  };
  boot.loader = {
    grub.enable = false;
  };
  fonts.fontconfig.enable = false;
  security.polkit.enable = false;
  services.udisks2.enable = lib.mkForce false;
}
