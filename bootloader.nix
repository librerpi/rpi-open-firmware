{ lib, config, pkgs, ... }:

{
  options = {
    boot.loader.openpi = {
      enable = lib.mkOption {
        type = lib.types.bool;
        default = false;
      };
    };
  };
  config = lib.mkIf config.boot.loader.openpi.enable {
    system.boot.loader.id = "openpi";
    system.build.installBootLoader = pkgs.substituteAll {
      src = ./install-openpi.sh;
      name = "install-openpi.sh";
      isExecutable = true;
      bootdir = (import ./. {}).bootdir;
    };
  };
}
