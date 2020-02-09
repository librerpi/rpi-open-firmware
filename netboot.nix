{
  networking = {
    #useNetworkd = true;
  };
  #systemd.network.enable = true;
  #systemd.network.links.eth0.enable = true;
  #systemd.network.networks.eth0.networkConfig.KeepConfiguration = "dhcp";
  boot.initrd = {
    extraUtilsCommands = ''
      copy_bin_and_libs ${(import pkgs.path { system = "armv7l-linux"; overlays = [ overlay2 ]; }).openiscsi}/bin/iscsistart
      copy_bin_and_libs ${pkgs.strace}/bin/strace $out/bin/strace
      cp -v ${pkgs.glibc}/lib/libpthread* $out/lib/
      cp ${pkgs.stdenv.cc.cc}/armv7l-unknown-linux-gnueabihf/lib/libgcc_s.so $out/lib/ -v
    '';
    extraUtilsCommandsTest = ''
      strace --version
    '';
    network = {
      enable = true;
      postCommands = ''
        iscsistart -t iqn.2020-01.com.angeldsis:rpi-image1.img -a 192.168.2.61 -i iqn.2015-09.com.example:3255a7223b2 -g 0
      '';
      ssh = {
        enable = true;
        authorizedKeys = [
          "ssh-rsa AAAAB3NzaC1yc2EAAAADAQABAAABAQC34wZQFEOGkA5b0Z6maE3aKy/ix1MiK1D0Qmg4E9skAA57yKtWYzjA23r5OCF4Nhlj1CuYd6P1sEI/fMnxf+KkqqgW3ZoZ0+pQu4Bd8Ymi3OkkQX9kiq2coD3AFI6JytC6uBi6FaZQT5fG59DbXhxO5YpZlym8ps1obyCBX0hyKntD18RgHNaNM+jkQOhQ5OoxKsBEobxQOEdjIowl2QeEHb99n45sFr53NFqk3UCz0Y7ZMf1hSFQPuuEC/wExzBBJ1Wl7E1LlNA4p9O3qJUSadGZS4e5nSLqMnbQWv2icQS/7J8IwY0M8r1MsL8mdnlXHUofPlG1r4mtovQ2myzOx clever@nixos"
        ];
      };
    };
  };
}
