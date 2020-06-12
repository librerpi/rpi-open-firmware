{
  imports = [ ./nixos.nix ];
  fileSystems = {
    "/" = {
      device = "/dev/mmcblk0p2";
      fsType = "ext4";
    };
    "/boot" = {
      device = "/dev/mmcblk0p1";
      fsType = "vfat";
      options = [ "nofail" ];
    };
  };
}
