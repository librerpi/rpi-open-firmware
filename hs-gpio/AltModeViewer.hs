{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE NamedFieldPuns #-}

module AltModeViewer where

import           Brick
import           Brick.BChan (BChan, writeBChan)
import           Control.Concurrent
import           Control.Concurrent.Async
import           Control.Monad
import           Data.List
import           Formatting
import           Formatting.ShortFormatters hiding (s, f)
import qualified Graphics.Vty as V
import           GHC.Ptr
import           GPIO
import           Types

data AltModeState = AltModeState
  { eventCount :: Int
  , pinModeList :: [(Pin, AltMode)]
  , debugmsg :: String
  , psCallback :: EventM Name DialogReply
  }

handleEvent :: AltModeState -> AppState -> BrickEvent Name CustomEvent -> EventM Name DialogReply
handleEvent AltModeState{psCallback} _ (VtyEvent (V.EvKey (V.KChar 'q') [])) = psCallback
handleEvent s _ (AppEvent (CustEvtStateUpdate (StateUpdate modes))) = pure $ DialogReplyContinue $ mkAltModeViewer $ s { pinModeList = modes }
handleEvent s@AltModeState{eventCount,psCallback} _ e = do
  if eventCount > 5 then
    psCallback
  else
    pure $ DialogReplyContinue $ mkAltModeViewer s
      { eventCount = eventCount + 1
      , debugmsg = show e
      }

drawEverything :: AltModeState -> AppState -> [Widget n]
drawEverything AltModeState{pinModeList,debugmsg} _ = [ hBox [ padLeftRight 1 $ vBox leftList, padLeftRight 1 $ vBox rightList, str debugmsg ] ]
  where
    (leftcol, rightcol) = partition (\(Pin p,_) -> p < 32) pinModeList
    leftList = map pinToRow leftcol
    rightList = map pinToRow rightcol

mkAltModeViewer :: AltModeState -> Dialog
mkAltModeViewer s = Dialog { dRender = drawEverything s, dHandleEvent = handleEvent s }

spawnAltModeViewer :: Ptr RPI -> BChan CustomEvent -> IO Dialog -> EventM Name DialogReply
spawnAltModeViewer addr eventChan callback = pure $ DialogReplyLiftIO $ do
  pinModes <- getPinStates $ toGPIO addr
  bgthread <- async $ backgroundThread (toGPIO addr) eventChan
  let
    s :: AltModeState
    s = AltModeState
      { eventCount = 0
      , pinModeList = pinModes
      , debugmsg = ""
      , psCallback = do
        pure $ DialogReplyLiftIO $ do
          cancel bgthread
          callback
      }
  return $ mkAltModeViewer s

backgroundThread :: Ptr GPIO -> BChan CustomEvent -> IO ()
backgroundThread addr eventChan = forever $ do
  newState <- getPinStates addr
  writeBChan eventChan $ CustEvtStateUpdate $ StateUpdate newState
  threadDelay 1000000

pinToRow :: (Pin, AltMode) -> Widget n
pinToRow (Pin pin, mode) = addAttr mode $ txt $ sformat (d % " " % shown) pin mode

addAttr :: AltMode -> Widget n -> Widget n
addAttr Alt0 w = withAttr alt0Attr w
addAttr Alt1 w = withAttr alt1Attr w
addAttr Alt2 w = withAttr alt2Attr w
addAttr Alt3 w = withAttr alt3Attr w
addAttr Alt4 w = withAttr alt4Attr w
addAttr Alt5 w = withAttr alt5Attr w
addAttr _ w = w

alt0Attr, alt1Attr, alt2Attr, alt3Attr, alt4Attr, alt5Attr :: AttrName
alt0Attr = attrName "alt0"
alt1Attr = attrName "alt1"
alt2Attr = attrName "alt2"
alt3Attr = attrName "alt3"
alt4Attr = attrName "alt4"
alt5Attr = attrName "alt5"
