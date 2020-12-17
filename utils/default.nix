{ stdenv, common, raspberrypi-tools, libdrm, pkgconfig }:

stdenv.mkDerivation {
  name = "utils";
  nativeBuildInputs = [ pkgconfig ];
  buildInputs = [ common raspberrypi-tools libdrm ];
  src = stdenv.lib.cleanSource ./.;
  dontStrip = true;
  enableParallelBuilding = true;
}
