{ stdenv, tlsf  }:
let
  baremetal = stdenv.targetPlatform.system == "arm-none" || stdenv.targetPlatform.system == "vc4-none";
in stdenv.mkDerivation {
  name = "common";
  src = stdenv.lib.cleanSource ./.;
  propagatedBuildInputs = stdenv.lib.optional baremetal tlsf;
  enableParallelBuilding = true;
  hardeningDisable = [ "fortify" "stackprotector" ];
  dontStrip = true;
  BAREMETAL = baremetal;
}
