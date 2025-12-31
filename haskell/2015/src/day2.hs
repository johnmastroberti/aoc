module Main where

import Util
import GHC.Utils.Misc

parseInput = map (split 'x') . split '\n' 
part1 _ = 0
part2 _ = 0

main = bindUserArg byChars $ callBoth part1 part2 parseInput
