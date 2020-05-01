{ stdenv, fetchurl }:

stdenv.mkDerivation rec {
  version = "1.64";
  pname = "bcm2835";

  src = fetchurl {
    url = "http://www.airspayce.com/mikem/bcm2835/bcm2835-${version}.tar.gz";
    sha256 = "06s81540iz4vsh0cm6jwah2x0hih79v42pfa4pgr8kcbv56158h6";
  };

  # We need this because for some reason, the Makefile doesn't use env var AR
  preConfigure = ''
    makeFlags="$makeFlags AR=$AR"
  '';

  meta = with stdenv.lib; {
    description = "C library for Broadcom BCM 2835 as used in Raspberry Pi";
    homepage = http://www.airspayce.com/mikem/bcm2835/;
  };
}

