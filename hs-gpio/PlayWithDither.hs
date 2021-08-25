{-# LANGUAGE OverloadedStrings #-}
{-# LANGUAGE NamedFieldPuns #-}

module PlayWithDither (spawnPlayWithDither) where

import           Brick hiding (render)
import           Brick.BChan (BChan)
import           Control.Monad
import           Data.Bits
import           Data.Word
import           Foreign.Storable
import           Formatting hiding (base)
import           Formatting.ShortFormatters hiding (s, f)
import qualified Graphics.Vty as V
import           GHC.Ptr
import           GPIO
import           Types

data HvsChannel = Hvs0 | Hvs1 | Hvs2 deriving Show

data State = State
  { sHvsChannel :: HvsChannel
  , sCallback :: EventM Name DialogReply
  , sHVS :: Ptr HVS
  , sHwState :: HvsState
  }

data DitherConfig = DitherConfig Word32 (DitherChannelConfig, DitherChannelConfig, DitherChannelConfig) deriving Show
data DitherChannelConfig = DitherChannelConfig
  { dcMode :: DitherMode
  , dcDepth :: DitherDepth
  } deriving Show

data DitherMode = DitherNone | DitherAccum | DitherRNG1 | DitherRNG2 deriving (Show, Enum, Bounded, Eq)

data DitherDepth = Dither888 | Dither666 | Dither565 | Dither555 deriving (Show, Enum, Bounded, Eq)

instance Storable DitherConfig where
  peek p = do
    w32 <- peek (castPtr p)
    let
      decodeM 0 = DitherNone
      decodeM 1 = DitherAccum
      decodeM 2 = DitherRNG1
      decodeM 3 = DitherRNG2
      decodeD 0 = Dither888
      decodeD 1 = Dither666
      decodeD 2 = Dither565
      decodeD 3 = Dither555
      decode :: Word32 -> DitherChannelConfig
      decode w = DitherChannelConfig (decodeM $ w .&. 0x3) (decodeD $ (w `shiftR` 2) .&. 0x3)
      get chan = decode $ (w32 `shiftR` (chan * 4)) .&. 0xf
    pure $ DitherConfig w32 (get 0, get 1, get 2)
  poke p (DitherConfig _ (DitherChannelConfig mode depth, _, _)) = do
    let
      m = case mode of
        DitherNone -> 0
        DitherAccum -> 1
        DitherRNG1 -> 2
        DitherRNG2 -> 3
      d = case depth of
        Dither888 -> 0
        Dither666 -> 1
        Dither565 -> 2
        Dither555 -> 3
      reg :: Ptr Word32
      reg = castPtr p
      val :: Word32
      val = (m .|. (d `shiftL` 2))
    when (m /= 0) $ poke reg val

toDitherCfg :: Ptr HVS -> Ptr DitherConfig
toDitherCfg p = castPtr (p `plusPtr` 0x14)

getDList :: Ptr HVS -> HvsChannel -> IO Word32
getDList hvs chan = peekByteOff hvs offset
  where
    offset = case chan of
      Hvs0 -> 0x20
      Hvs1 -> 0x24
      Hvs2 -> 0x28

getChannelConfigWindow :: Ptr HVS -> HvsChannel -> Ptr HvsChannelConfig
getChannelConfigWindow hvs chan = castPtr (hvs `plusPtr` offset)
  where
    offset = case chan of
      Hvs0 -> 0x40
      Hvs1 -> 0x50
      Hvs2 -> 0x60

data HvsState = HvsState
  { dlist :: Word32
  , cfg :: HvsChannelConfig
  , sDither :: DitherConfig
  } deriving Show

data HvsChannelConfig = HvsChannelConfig
  { ctrl :: Word32
  , background :: Word32
  , stat :: Word32
  , base :: Word32
  , height :: Word32
  , pi4width :: Word32
  } deriving Show

instance Storable HvsChannelConfig where
  peek configWindow = do
    ctrl <- peekByteOff configWindow 0
    background <- peekByteOff configWindow 4
    stat <- peekByteOff configWindow 8
    base <- peekByteOff configWindow 12
    let
      height = ctrl .&. 0xfff
      pi4width = (ctrl `shiftR` 16) .&. 0x1fff
    pure $ HvsChannelConfig{ctrl,background,stat,base,height,pi4width}

refreshHwState :: Ptr HVS -> HvsChannel -> IO HvsState
refreshHwState hvs chan = do
  dlist <- getDList hvs chan
  let configWindow = getChannelConfigWindow hvs chan
  cfg <- peek configWindow
  sDither <- peek (toDitherCfg hvs)
  pure $ HvsState{dlist,cfg,sDither}

spawnPlayWithDither :: Ptr RPI -> BChan CustomEvent -> EventM Name DialogReply -> EventM Name DialogReply
spawnPlayWithDither mmioWindow bchan callback = do
  pure $ DialogReplyLiftIO $ do
    hwstate <- refreshHwState (toHVS mmioWindow) Hvs0
    pure $ mkPlayWithDither $ State Hvs0 callback (toHVS mmioWindow) hwstate

mkPlayWithDither :: State -> Dialog
mkPlayWithDither s = Dialog { dRender = render s, dHandleEvent = handleEvents s }

modChannel :: HvsChannel -> (DitherChannelConfig -> DitherChannelConfig) -> DitherConfig -> DitherConfig
modChannel Hvs0 mod (DitherConfig w (a, b, c)) = DitherConfig w (mod a, b, c)
modChannel Hvs1 mod (DitherConfig w (a, b, c)) = DitherConfig w (a, mod b, c)
modChannel Hvs2 mod (DitherConfig w (a, b, c)) = DitherConfig w (a, b, mod c)

getChannel :: HvsChannel -> DitherConfig -> DitherChannelConfig
getChannel Hvs0 (DitherConfig w (a, _, _)) = a
getChannel Hvs1 (DitherConfig w (_, b, _)) = b
getChannel Hvs2 (DitherConfig w (_, _, c)) = c

ssucc :: (Bounded a, Enum a, Eq a) => a -> a
ssucc a = if (a == maxBound) then a else succ a

spred :: (Bounded a, Enum a, Eq a) => a -> a
spred a = if (a == minBound) then a else pred a

handleEvents :: State -> AppState -> BrickEvent Name CustomEvent -> EventM Name DialogReply
handleEvents s@State{sCallback,sHVS,sHwState,sHvsChannel} _ (VtyEvent (V.EvKey k [])) = do
  let
    ditherWriteback :: (DitherChannelConfig -> DitherChannelConfig) -> EventM Name DialogReply
    ditherWriteback mod = do
      let
        new_cfg = modChannel sHvsChannel mod (sDither sHwState)
      pure $ DialogReplyLiftIO $ do
        poke (toDitherCfg sHVS) new_cfg
        hwstate <- refreshHwState sHVS sHvsChannel
        pure $ mkPlayWithDither $ s { sHwState = hwstate }
  case k of
    V.KUp -> ditherWriteback (\c@DitherChannelConfig{dcMode} -> c { dcMode = ssucc dcMode })
    V.KDown -> ditherWriteback (\c@DitherChannelConfig{dcMode} -> c { dcMode = spred dcMode })
    V.KLeft -> ditherWriteback (\c@DitherChannelConfig{dcDepth} -> c { dcDepth = spred dcDepth})
    V.KRight -> ditherWriteback (\c@DitherChannelConfig{dcDepth} -> c { dcDepth = ssucc dcDepth})
    V.KChar key -> do
      case key of
        'q' -> sCallback
        '1' -> hvsRefresh $ s { sHvsChannel = Hvs0 }
        '2' -> hvsRefresh $ s { sHvsChannel = Hvs1 }
        '3' -> hvsRefresh $ s { sHvsChannel = Hvs2 }

hvsRefresh :: State -> EventM Name DialogReply
hvsRefresh s@State{sHVS,sHvsChannel} = do
  pure $ DialogReplyLiftIO $ do
    hwstate <- refreshHwState sHVS sHvsChannel
    pure $ mkPlayWithDither $ s { sHwState = hwstate }

render :: State -> AppState -> [Widget n]
render State{sHvsChannel,sHwState} as = [ vBox [ currentChannel, res, debug, dither ] ]
  where
    currentChannel = str $ show sHvsChannel
    res = txt $ sformat ("resolution: " % d % "x" % d) (pi4width . cfg $ sHwState) (height .  cfg $ sHwState)
    dither = str $ show $ getChannel sHvsChannel $ sDither $ sHwState
    debug = strWrap $ show sHwState
