{-# LANGUAGE ExistentialQuantification #-}


module Types (Name(FileChoice, MainMenu, Menu1), Dialog(Dialog, dRender, dHandleEvent), AppState(AppState, asLastMsg, asDialogStack, asReplyChan, asMmioWindow), CustomEvent(CustEvtStateUpdate), DialogReply(DialogReplyHalt, DialogReplyContinue, DialogReplyLiftIO), StateUpdate(StateUpdate)) where

import           Brick (Widget, EventM, BrickEvent)
import           Brick.BChan (BChan)
import           GHC.Ptr
import           GPIO

data Name = MainMenu | FileChoice | Menu1 | None deriving (Eq, Ord, Show)

data AppState = AppState
  { asReplyChan :: BChan CustomEvent
  , asLastMsg :: String
  , asDialogStack :: Dialog
  , asMmioWindow :: Ptr RPI
  }

data DialogReply = DialogReplyHalt AppState | DialogReplyContinue Dialog | DialogReplyLiftIO (IO Dialog)

data Dialog = Dialog
  { dRender :: AppState -> [ Widget Name ]
  , dHandleEvent :: AppState -> BrickEvent Name CustomEvent -> EventM Name DialogReply
  }

data StateUpdate = StateUpdate [(Pin, AltMode)] deriving Show

data CustomEvent = CustEvtStateUpdate StateUpdate  deriving Show
