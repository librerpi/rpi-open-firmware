{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE NamedFieldPuns #-}

module Main where

--import System.RaspberryPi.GPIO
import GHC.Ptr
import Data.Word
import MMAP
import System.Posix.IO.ByteString
import System.Posix.Types
import           Foreign.C.Types
import           Foreign.Storable
import           Formatting
import           Formatting.ShortFormatters
import           Data.Bits
import           Brick
import           Brick.Main
import qualified Graphics.Vty as V
import           Brick.BChan (BChan, newBChan, writeBChan)
import qualified Brick.Widgets.List as L
import qualified Brick.AttrMap as A
import Brick.Forms (focusedFormInputAttr, invalidFormInputAttr)
import Data.List
import Control.Concurrent.Async
import Control.Concurrent

newtype Pin = Pin Int deriving Show

instance Bounded Pin where
  minBound = Pin 0
  maxBound = Pin 59

instance Num Pin where
  fromInteger p = Pin $ fromIntegral p

instance Enum Pin where
  succ (Pin p) = Pin $ succ p
  fromEnum (Pin p) = p
  toEnum p = Pin p

theMap :: A.AttrMap
theMap = A.attrMap V.defAttr
  [ (L.listAttr,            V.white `on` V.blue)
  , (L.listSelectedAttr,    V.blue `on` V.white)
  , (focusedFormInputAttr, V.black `on` V.yellow)
  , (invalidFormInputAttr, V.white `on` V.red)
  , (alt0Attr            , V.red `on` V.black)
  , (alt1Attr            , V.green `on` V.black)
  , (alt2Attr            , V.yellow `on` V.black)
  , (alt3Attr            , V.blue `on` V.black)
  , (alt4Attr            , V.magenta `on` V.black)
  , (alt5Attr            , V.cyan `on` V.black)
  ]

alt0Attr = attrName "alt0"
alt1Attr = attrName "alt1"
alt2Attr = attrName "alt2"
alt3Attr = attrName "alt3"
alt4Attr = attrName "alt4"
alt5Attr = attrName "alt5"

main :: IO ()
main = do
  (addr, fd) <- openGPIO
  print addr
  dumpFsel addr
  getPinAltMode addr 14 >>= print
  getPinAltMode addr 15 >>= print
  eventChan <- newBChan 10
  replyChan <- newBChan 10
  let
    mkVty = V.mkVty V.defaultConfig
    app :: App State StateUpdate ()
    app = App
      { appDraw = drawEverything
      , appChooseCursor = showFirstCursor
      , appHandleEvent = handleEvent
      , appStartEvent = pure
      , appAttrMap = const theMap
      }
  pinModes <- getPinStates addr
  vty <- mkVty
  bgthread <- async $ backgroundThread addr eventChan
  finalState <- customMain vty mkVty (Just eventChan) app (State 0 pinModes "")
  print finalState

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

backgroundThread :: Ptr GPIO -> BChan StateUpdate -> IO ()
backgroundThread addr eventChan = do
  newState <- getPinStates addr
  writeBChan eventChan $ StateUpdate newState
  threadDelay 1000000
  backgroundThread addr eventChan

data StateUpdate = StateUpdate [(Pin, AltMode)] deriving Show

handleEvent :: State -> BrickEvent () StateUpdate -> EventM () (Next State)
handleEvent s (VtyEvent (V.EvKey (V.KChar 'q') [])) = halt s
handleEvent s (AppEvent (StateUpdate modes)) = continue $ s { pinModeList = modes }
handleEvent s@State{eventCount} e = do
  if eventCount > 5 then
    halt s
  else
    continue $ s
    { eventCount = eventCount + 1
    , debugmsg = show e
    }

drawEverything :: State -> [Widget ()]
drawEverything s@State{pinModeList,debugmsg} = [ hBox [ padLeftRight 1 $ vBox leftList, padLeftRight 1 $ vBox rightList, str debugmsg ] ]
  where
    (left, right) = partition (\(Pin p,_) -> p < 32) pinModeList
    leftList = map pinToRow left
    rightList = map pinToRow right

pinToRow :: (Pin, AltMode) -> Widget ()
pinToRow (Pin pin, mode) = addAttr mode $ txt $ sformat (d % " " % shown) pin mode

addAttr :: AltMode -> Widget () -> Widget ()
addAttr Alt0 w = withAttr alt0Attr w
addAttr Alt1 w = withAttr alt1Attr w
addAttr Alt2 w = withAttr alt2Attr w
addAttr Alt3 w = withAttr alt3Attr w
addAttr Alt4 w = withAttr alt4Attr w
addAttr Alt5 w = withAttr alt5Attr w
addAttr _ w = w

data State = State
  { eventCount :: Int
  , pinModeList :: [(Pin, AltMode)]
  , debugmsg :: String
  } deriving Show

--app :: App _ _ _
--app = 

openGPIO :: IO (Ptr GPIO, Fd)
openGPIO = do
  fd <- openFd "/dev/gpiomem" ReadWrite Nothing defaultFileFlags
  ptr <- c_mmap_helper fd
  pure (ptr, fd)

foreign import ccall unsafe "c_mmap_helper" c_mmap_helper :: Fd -> IO (Ptr GPIO)

data GPIO

dumpFsel :: Ptr GPIO -> IO ()
dumpFsel addr = do
  let
    f :: Int -> IO Word32
    f bank = do
      reg <- peek (addr `plusPtr` (4 * bank))
      fprint (d % " " % x % "\n") bank reg
      pure reg
  f 0
  f 1
  f 2
  f 3
  f 4
  f 5
  pure ()

data AltMode = AltIn | AltOut | Alt0 | Alt1 | Alt2 | Alt3 | Alt4 | Alt5 deriving Show

getPinAltMode :: Ptr GPIO -> Pin -> IO AltMode
getPinAltMode addr (Pin pin) = do
  let
    bank :: Int
    row :: Int
    (bank, row) = pin `divMod` 10
  reg <- peek (addr `plusPtr` (4 * bank))
  let
    rawMode = (shiftR reg (3 * row)) .&. 7
    mode = rawModeToAltMode rawMode
  --fprint ("bank: "%d%" row: "%d%" reg: "%x%" rawMode: "%d%" mode: "%shown%"\n") bank row (reg :: Word32) rawMode mode
  pure mode

rawModeToAltMode :: Word32 -> AltMode
rawModeToAltMode 0 = AltIn
rawModeToAltMode 1 = AltOut
rawModeToAltMode 2 = Alt5
rawModeToAltMode 3 = Alt4
rawModeToAltMode 4 = Alt0
rawModeToAltMode 5 = Alt1
rawModeToAltMode 6 = Alt2
rawModeToAltMode 7 = Alt3
