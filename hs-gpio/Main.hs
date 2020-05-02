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
  res <- mmap_helper 1 2 3 4 5 0x1234567890
  res <- mmap_helper6 1 2 3 4 5 0x1234567890
  res <- mmap_helper5 1 2 3 4 0x12345678
  print res
  -- mmap2(NULL, 25165824, PROT_READ|PROT_WRITE, MAP_SHARED, 3, 0) =
  pure (ptr, fd)

foreign import ccall unsafe "mmap_helper"
  mmap_helper :: CInt -> CInt -> CInt -> CInt -> CInt -> COff -> IO (Ptr ())

foreign import ccall unsafe "mmap_helper6"
  mmap_helper6 :: CInt -> CInt -> CInt -> CInt -> CInt -> COff -> IO (Ptr ())

foreign import ccall unsafe "mmap_helper5"
  mmap_helper5 :: CInt -> CInt -> CInt -> CInt -> COff -> IO (Ptr ())
