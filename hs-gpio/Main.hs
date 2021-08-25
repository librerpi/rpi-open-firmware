{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE NamedFieldPuns #-}

module Main where

--import System.RaspberryPi.GPIO
import GHC.Ptr
import System.Posix.IO.ByteString
import System.Posix.Types
import           Foreign.C.Types
import           Brick
import qualified Graphics.Vty as V
import           Brick.BChan (newBChan)
import qualified Brick.Widgets.List as L
import qualified Brick.AttrMap as A
import Brick.Forms (focusedFormInputAttr, invalidFormInputAttr)
import GPIO
import Data.Word
import qualified Data.Vector as V

import Types (Name(MainMenu), Dialog(dHandleEvent, dRender), DialogReply(DialogReplyContinue,DialogReplyHalt,DialogReplyLiftIO), CustomEvent, AppState(AppState, asDialogStack))
import AltModeViewer (alt0Attr,alt1Attr,alt2Attr,alt3Attr,alt4Attr,alt5Attr)
import           MainMenu

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

drawUI :: AppState -> [ Widget Name ]
drawUI state = (dRender . asDialogStack) state state

handleEvent :: AppState -> BrickEvent Name CustomEvent -> EventM Name (Next (AppState))
handleEvent state@AppState{asDialogStack} event = do
  let
    thing :: IO Dialog -> IO AppState
    thing ioact = do
      dlg <- ioact
      pure $ state { asDialogStack = dlg }
    go :: DialogReply -> EventM Name (Next AppState)
    go reply = do
      case reply of
        DialogReplyHalt s -> halt s
        DialogReplyContinue dlg -> continue $ state { asDialogStack = dlg }
        DialogReplyLiftIO ioact -> suspendAndResume (thing ioact)
  newDlg <- (dHandleEvent asDialogStack) state event
  go newDlg

main :: IO ()
main = do
  (addr, _fd) <- openMMIO
  let gpio = toGPIO addr
  eventChan <- newBChan 10
  --replyChan <- newBChan 10
  let
    mkVty = V.mkVty V.defaultConfig
    app :: App AppState CustomEvent Name
    app = App
      --{ appDraw = drawEverything
      { appDraw = drawUI
      , appHandleEvent = handleEvent
      , appChooseCursor = showFirstCursor
      , appStartEvent = pure
      , appAttrMap = const theMap
      }
    dlg :: Dialog
    dlg = mkMainMenu $ MainMenuState
      { psMenuState = L.list MainMenu (V.fromList [ ShowAltModes, PlayWithDither ]) 1
      , psCallback = \as -> do
        pure $ DialogReplyHalt as
      }
  vty <- mkVty
  _finalState <- customMain vty mkVty (Just eventChan) app (AppState eventChan "" dlg addr)
  pure ()


data MMIOMethod = GpioMem | RawMem

openGPIO :: MMIOMethod -> IO (Ptr GPIO, Fd)
openGPIO GpioMem = do
  fd <- openFd "/dev/gpiomem" ReadWrite Nothing defaultFileFlags
  ptr <- c_mmap_helper 0 4096 fd
  pure (ptr, fd)

openMMIO :: IO (Ptr RPI, Fd)
openMMIO = do
  fd <- openFd "/dev/mem" ReadWrite Nothing defaultFileFlags
  ptr <- c_mmap_helper 0xfe000000 (1024*1024*16) fd
  pure (ptr, fd)

foreign import ccall unsafe "c_mmap_helper" c_mmap_helper :: Word32 -> Word32 -> Fd -> IO (Ptr a)


