{ stdenv, common }:

stdenv.mkDerivation {
  name = "utils";
  buildInputs = [ common ];
  src = ./.;
}
