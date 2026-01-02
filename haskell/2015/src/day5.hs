module Main where

import Util
import GHC.Utils.Misc
import Data.List
import Data.Maybe

parseInput = filter ((/= 0) . length) . lines

isVowel c = isJust $ elemIndex c "aeiou"

property1 = (>= 3) . length . (filter isVowel)

property2 (x:y:xs)
    | x == y    = True
    | null xs   = False
    | otherwise = property2 (y:xs)

property3 ('a':'b':_) = False
property3 ('c':'d':_) = False
property3 ('p':'q':_) = False
property3 ('x':'y':_) = False
property3 (_:xs) = property3 xs
property3 [] = True

isNice s = and $ map ($ s) [property1, property2, property3]

part1 = length . filter isNice

property4 (x:y:xs)
    | isInfixOf [x,y] xs    = True
    | null xs               = False
    | otherwise             = property4 (y:xs)

property5 (x:y:z:xs)
    | x == z    = True
    | null xs   = False
    | otherwise = property5 (y:z:xs)

isNice2 s = and $ map ($ s) [property4, property5]
part2 = length . filter isNice2

main = aoc part1 part2 parseInput
