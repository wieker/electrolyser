{-# LANGUAGE CPP #-}
{-# LANGUAGE GeneralizedNewtypeDeriving, DerivingStrategies #-}
{-# LANGUAGE RecordWildCards #-}
module Top where

-- Thanks to Dr. Gergő Érdi:
--   https://github.com/clash-lang/clash-compiler/issues/461

import Clash.Prelude hiding (lift)
import Control.Monad.State
import Data.Word
import Control.Monad.RWS
import Data.Monoid

-- See: https://github.com/clash-lang/clash-compiler/pull/2511
{-# CLASH_OPAQUE topEntity #-}
{-# ANN topEntity
  (Synthesize
    { t_name   = "blinker"
    , t_inputs = [ PortName "xtal_in"
                 , PortName "A0"
                 , PortName "A1"
                 ]
    , t_output = PortName "LED1"
    }) #-}
topEntity
    :: Clock System
    -> Reset System
    -> Signal System Bit
    -> ( Signal System Bit
       )
topEntity clk rst = exposeClockResetEnable comb clk rst enableGen
    where
        comb input = mealy blinkerT (0, 0) input

blinkerT :: (BitVector 32, Bit) -> Bit -> (((BitVector 32), Bit), Bit)
blinkerT (cntr, leds) _ = ((cntr', leds'),leds)
  where
    -- clock frequency = 100e6  (100 MHz)
    -- led update rate = 333e-3 (every 333ms)
    cnt_max = 48000000

    cntr' | cntr == cnt_max = 0
          | otherwise       = cntr + 1

    leds' | cntr == 0       = 1 + leds
          | otherwise       = leds