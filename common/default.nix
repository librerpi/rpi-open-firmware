{ stdenv, tlsf, lib }:
let
  baremetal = stdenv.targetPlatform.system == "arm-none" || stdenv.targetPlatform.system == "vc4-none";
  lut = {
    vc4-none = "vpu";
    aarch64-linux = "aarch64";
    aarch64-none = "aarch64";
    arm-none = "arm32";
    armv7l-linux = "arm32";
    armv6l-linux = "arm32";
  };
in stdenv.mkDerivation {
  name = "common";
  src = lib.cleanSource ./.;
  propagatedBuildInputs = lib.optional baremetal tlsf;
  enableParallelBuilding = true;
  hardeningDisable = [ "fortify" "stackprotector" ];
  dontStrip = true;
  BAREMETAL = baremetal;
  targetArch = lut.${stdenv.targetPlatform.system};
}
