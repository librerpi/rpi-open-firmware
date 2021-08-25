{-# LANGUAGE NamedFieldPuns #-}

module FileChooser (spawnFileChooser) where

import System.Directory (canonicalizePath, getDirectoryContents, doesDirectoryExist)
import Control.Monad.IO.Class (liftIO)

import Types (Name(FileChoice), Dialog(Dialog,dRender, dHandleEvent), AppState, CustomEvent, DialogReply(DialogReplyContinue))
import Brick (EventM, Widget, BrickEvent(VtyEvent), str, padLeftRight)
import qualified Brick.Widgets.Border as B
import qualified Brick.Widgets.List as L
import qualified Graphics.Vty as V
import qualified Data.Vector as V
import Data.List (sort)

data FileEntry = Directory String | File String deriving (Show, Eq)

instance Ord FileEntry where
  compare (Directory a) (Directory b) = compare a b
  compare (File a) (File b) = compare a b
  compare (Directory _) (File _) = LT
  compare (File _) (Directory _) = GT

data ChooserState = ChooserState
  { csCallback :: Maybe String -> EventM Name DialogReply
  , csCurrentDir :: FilePath
  , csList :: L.List Name FileEntry
  }

generateCurrentListing :: FilePath -> IO (L.List Name FileEntry)
generateCurrentListing dir = do
  let
    toFileEntry :: FilePath -> IO FileEntry
    toFileEntry path = do
      isDir <- doesDirectoryExist (dir <> "/" <> path)
      pure $ if isDir then Directory path else File path
  files <- getDirectoryContents dir
  files' <- mapM toFileEntry files
  pure $ L.list FileChoice (V.fromList $ sort files') 1

spawnFileChooser :: FilePath -> (Maybe String -> EventM Name DialogReply) -> EventM Name Dialog
spawnFileChooser startingDirectory callback = do
  currentDir <- liftIO $ canonicalizePath startingDirectory
  listState <- liftIO $ generateCurrentListing currentDir
  pure $ mkFileChooser $ ChooserState callback currentDir listState

mkFileChooser :: ChooserState -> Dialog
mkFileChooser state = Dialog { dRender = renderFileChooser state, dHandleEvent = handleEventFileChooser state }

renderFileChooser :: ChooserState -> AppState -> [ Widget Name ]
renderFileChooser state _as = [ box ]
  where
    box :: Widget Name
    box = B.borderWithLabel (str $ csCurrentDir state) $ padLeftRight 1 $ L.renderList renderRow True (csList state)
    renderRow :: Bool -> FileEntry -> Widget Name
    renderRow _ (Directory name) = str (name <> "/")
    renderRow _ (File name) = str name

handleEventFileChooser :: ChooserState -> AppState -> BrickEvent Name CustomEvent -> EventM Name DialogReply
handleEventFileChooser state@ChooserState{csList,csCallback} _as event = do
  case event of
    VtyEvent (V.EvKey (V.KChar 'q') []) -> do
      csCallback Nothing
    VtyEvent (V.EvKey V.KEnter []) -> do
      case L.listSelectedElement csList of
        Just (_index, File name ) ->
          csCallback $ Just name
        _ -> csCallback Nothing
    VtyEvent evt -> do
      newlist <- L.handleListEventVi L.handleListEvent evt csList
      pure $ DialogReplyContinue $ mkFileChooser $ state { csList = newlist }
    _ -> do
      pure $ DialogReplyContinue $ mkFileChooser state
