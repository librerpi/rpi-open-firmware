{ mkDerivation, base, bcm2835, bytestring, fetchgit, stdenv }:
mkDerivation {
  pname = "HPi";
  version = "0.7.0";
  src = fetchgit {
    url = "https://github.com/WJWH/HPi";
    sha256 = "111dddmpil4d55lxrz7fpwqyw8g38p1rr74c8k9g88y5vpq622hv";
    rev = "4f35045f97ca57812cda0e659d096c723c18284b";
    fetchSubmodules = true;
  };
  libraryHaskellDepends = [ base bytestring ];
  librarySystemDepends = [ bcm2835 ];
  homepage = "https://github.com/WJWH/HPi";
  description = "GPIO, I2C and SPI functions for the Raspberry Pi";
  license = stdenv.lib.licenses.bsd3;
}
