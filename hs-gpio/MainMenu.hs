{-# LANGUAGE NamedFieldPuns #-}

module MainMenu where

import           AltModeViewer (spawnAltModeViewer)
import           Brick
import qualified Brick.Widgets.Border as B
import qualified Brick.Widgets.List as L
import qualified Graphics.Vty as V
import           PlayWithDither (spawnPlayWithDither)
import           Types

data MenuChoices = ShowAltModes | PlayWithDither

data MainMenuState = MainMenuState
  { psMenuState :: L.List Name MenuChoices
  , psCallback :: AppState -> EventM Name DialogReply
  }

renderMainMenu :: MainMenuState -> AppState -> [ Widget Name ]
renderMainMenu MainMenuState{psMenuState} _ = [ root ]
  where
    root = vBox [ menu ]
    menu = B.borderWithLabel (str "Menu") $ padLeftRight 1 $ L.renderList renderRow True psMenuState
    renderRow :: Bool -> MenuChoices -> Widget Name
    renderRow _ ShowAltModes = str "1: show gpio alt modes"
    renderRow _ PlayWithDither = str "2: play with hvs dither config"

handleEvents :: MainMenuState -> AppState -> BrickEvent Name CustomEvent -> EventM Name DialogReply
handleEvents mms@MainMenuState{psMenuState,psCallback} as@AppState{asMmioWindow,asReplyChan} event = do
  let
    openThing :: EventM Name DialogReply
    openThing = do
      case L.listSelectedElement psMenuState of
        Just (_index, item) -> case item of
          ShowAltModes -> do
            spawnAltModeViewer asMmioWindow asReplyChan (pure $ mkMainMenu mms)
          PlayWithDither -> do
            spawnPlayWithDither asMmioWindow asReplyChan (pure $ DialogReplyContinue $ mkMainMenu mms)
  case event of
    VtyEvent (V.EvKey (V.KChar 'q') []) -> do
      psCallback as
    VtyEvent (V.EvKey V.KEnter []) -> do
      openThing
    VtyEvent evt -> do
      newlist <- L.handleListEventVi L.handleListEvent evt psMenuState
      pure $ DialogReplyContinue $ mkMainMenu mms { psMenuState = newlist }

mkMainMenu :: MainMenuState -> Dialog
mkMainMenu s = Dialog { dRender = renderMainMenu s, dHandleEvent = handleEvents s }
