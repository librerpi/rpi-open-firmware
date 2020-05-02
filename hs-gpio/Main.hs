{-# LANGUAGE OverloadedStrings  #-}

module Main where

import System.RaspberryPi.GPIO
import GHC.Ptr
import Data.Word
import MMAP
import System.Posix.IO.ByteString
import System.Posix.Types
import           Foreign.C.Types

main :: IO ()
main = do
  putStrLn "Hello, Haskell!"
  addr <- openGPIO
  print addr

openGPIO :: IO (Ptr (), Fd)
openGPIO = do
  fd <- openFd "/dev/gpiomem" ReadWrite Nothing defaultFileFlags
  ptr <- c_mmap nullPtr 0x1800000 (protRead <> protWrite) (mkMmapFlags mapShared mempty) fd 0
  res <- mmap_helper nullPtr 0x1800000 (protRead <> protWrite) (mkMmapFlags mapShared mempty) fd 0
  print res
  -- mmap2(NULL, 25165824, PROT_READ|PROT_WRITE, MAP_SHARED, 3, 0) =
  pure (ptr, fd)

foreign import ccall unsafe "mmap_helper"
  mmap_helper :: Ptr () -> CSize -> ProtOption -> MmapFlags -> Fd -> COff -> IO (Ptr ())
