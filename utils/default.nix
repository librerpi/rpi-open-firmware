{ stdenv, common, raspberrypi-tools }:

stdenv.mkDerivation {
  name = "utils";
  buildInputs = [ common raspberrypi-tools ];
  src = ./.;
}
