module GPIO where

import           Data.Bits
import           Data.Word
import           Foreign.Storable
import           GHC.Ptr

data GPIO
data RPI
data HVS

toGPIO :: Ptr RPI -> Ptr GPIO
toGPIO p = (castPtr p) `plusPtr` 0x200000

toHVS :: Ptr RPI -> Ptr HVS
toHVS p = (castPtr p) `plusPtr` 0x400000

data AltMode = AltIn | AltOut | Alt0 | Alt1 | Alt2 | Alt3 | Alt4 | Alt5 deriving Show

newtype Pin = Pin Int deriving Show

instance Num Pin where
  fromInteger p = Pin $ fromIntegral p

instance Enum Pin where
  succ (Pin p) = Pin $ succ p
  fromEnum (Pin p) = p
  toEnum p = Pin p

instance Bounded Pin where
  minBound = Pin 0
  maxBound = Pin 59

rawModeToAltMode :: Word32 -> AltMode
rawModeToAltMode 0 = AltIn
rawModeToAltMode 1 = AltOut
rawModeToAltMode 2 = Alt5
rawModeToAltMode 3 = Alt4
rawModeToAltMode 4 = Alt0
rawModeToAltMode 5 = Alt1
rawModeToAltMode 6 = Alt2
rawModeToAltMode 7 = Alt3

getPinAltMode :: Ptr GPIO -> Pin -> IO AltMode
getPinAltMode addr (Pin pin) = do
  let (bank, row) = pin `divMod` 10
  reg <- peek (addr `plusPtr` (4 * bank))
  pure $ rawModeToAltMode $ (shiftR reg (3 * row)) .&. 7

getPinStates :: Ptr GPIO -> IO [(Pin, AltMode)]
getPinStates addr = do
  let
    allPins = [ minBound .. maxBound ]
    f :: Pin -> IO (Pin, AltMode)
    f pin = do
      mode <- getPinAltMode addr pin
      --fprint ("pin "%d%" is in mode "%shown%"\n") (fromEnum pin) mode
      pure (pin, mode)
  mapM f allPins
