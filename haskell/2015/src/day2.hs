module Main where

import Util
import GHC.Utils.Misc
import Data.List
import Control.Monad

doSplits :: String -> [[String]]
doSplits = map (split 'x') . split '\n' 

convertToInts :: [[String]] -> [[Int]]
convertToInts = map (map read) . filter ((>= 2) . length)

data Box = Box Int Int Int
instance Show Box where
    show (Box l w h) = join [show l, "x", show w, "x", show h]

convertToBox :: [Int] -> Box
convertToBox (l:w:h:_) = Box l w h

areas (Box l w h) = sort [l*w, l*h, w*h]
perimeters (Box l w h) = sort [2*(l+w), 2*(l+h), 2*(w+h)]
volume (Box l w h) = l * w * h

totalArea = liftA2 (+) (double . sum) head . areas where
    double = (*) 2
ribbonLength = liftA2 (+) (head . perimeters) volume

parseInput = (map convertToBox) . convertToInts . doSplits
part1 = sum . map totalArea
part2 = sum . map ribbonLength

main = bindUserArg byChars $ callBoth part1 part2 parseInput
