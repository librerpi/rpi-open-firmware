module Main where

import System.RaspberryPi.GPIO

main :: IO ()
main = do
  putStrLn "Hello, Haskell!"
  withGPIO $ do
    let
      f :: Pin -> IO ()
      f pin = do
        level <- readPin pin
        print pin
        print level
    f Pin03
