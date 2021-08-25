{-# LANGUAGE NamedFieldPuns #-}

module PromptString (spawnPromptString) where

import           Brick (EventM, Widget, BrickEvent(VtyEvent), padLeftRight, str, withDefAttr, vBox)
import           Brick.Widgets.Center (center)
import qualified Graphics.Vty as V
import qualified Brick.Widgets.Border as B
import           Brick.Forms (focusedFormInputAttr, invalidFormInputAttr)

import Types (Name, DialogReply(DialogReplyContinue), Name, Dialog(dRender, dHandleEvent, Dialog), CustomEvent, AppState)

data PromptString = PromptString
  { psTitle :: String
  , psValidator :: String -> Bool
  , psCallback :: String -> EventM Name DialogReply
  , psCurrentString :: String
  }

spawnPromptString :: String -> (String -> Bool) -> (String -> EventM Name DialogReply) -> EventM Name DialogReply
spawnPromptString title validator callback = do
  let
    state = PromptString title validator callback ""
  pure $ DialogReplyContinue $ mkPromptString state

mkPromptString :: PromptString -> Dialog
mkPromptString state = Dialog { dRender = renderUI state, dHandleEvent = handleEvents state }

renderUI :: PromptString -> AppState -> [ Widget Name ]
renderUI PromptString{psCurrentString,psValidator,psTitle} _astate = [ center root ]
  where
    root :: Widget Name
    root = B.borderWithLabel (str psTitle) $ vBox [ pad, padLeftRight 1 $ currentStr, pad ]
    attr = if (psValidator psCurrentString) then focusedFormInputAttr else invalidFormInputAttr
    currentStr = withDefAttr attr currentStr'
    currentStr' = str psCurrentString
    pad = str $ replicate (length psTitle) ' '

handleEvents :: PromptString -> AppState -> BrickEvent Name CustomEvent -> EventM Name DialogReply
handleEvents pstate@PromptString{psCallback,psCurrentString} _astate event = do
  case event of
    VtyEvent (V.EvKey V.KEnter []) -> do
      -- TODO, allows invalid strings passed to callback
      psCallback psCurrentString
    VtyEvent (V.EvKey (V.KChar char) []) -> do
      pure $ DialogReplyContinue $ mkPromptString $ pstate { psCurrentString = psCurrentString <> [char] }
    VtyEvent (V.EvKey V.KBS []) -> do
      let
        size = length psCurrentString
        newstring = take (size - 1) psCurrentString
      pure $ DialogReplyContinue $ mkPromptString $ pstate { psCurrentString = newstring }
    other -> do
      pure $ DialogReplyContinue $ mkPromptString $ pstate { psTitle = show other }
