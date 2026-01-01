module Main where

import Util
import Data.List
import Data.Hash.MD5
import Data.Maybe

goodHash hash = (take 5 hash) == "00000"
veryGoodHash hash = (take 6 hash) == "000000"

parseInput = filter (/= '\n')
day4 condition key = (+) 1 $ fromJust $ findIndex condition $ map (md5s . Str . (++) key . show) [1..]
part1 = day4 goodHash
part2 = day4 veryGoodHash

main = aoc part1 part2 parseInput
