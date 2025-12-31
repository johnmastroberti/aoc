module Main where

import Data.List
import Data.Maybe
import Util

parenToInt :: Char -> Int
parenToInt '(' = 1
parenToInt ')' = -1
parenToInt _ = 0

inputToInts s = map parenToInt s

part1 input = foldl (+) 0 input
part2 input = elemIndex (-1) $ scanl (+) 0 input

main = bindUserArg byChars $ callBoth part1 (fromJust . part2) inputToInts
