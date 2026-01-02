module Main where

import Util
import GHC.Utils.Misc
import Data.List
import Data.Ord

type Point = (Int, Int)
origin = (0,0)
data Operation = On | Off | Toggle | None
data Instruction = Instruction Operation Point Point

listToPoint (x:y:[]) = (x,y)
parsePoint = listToPoint . map read . split ','
toIndex (x,y) = 1000*x + y

parseLine = parseLineImpl . words
parseLineImpl ("turn":"on":p1:"through":p2:_) = Instruction On (parsePoint p1) (parsePoint p2)
parseLineImpl ("turn":"off":p1:"through":p2:_) = Instruction Off (parsePoint p1) (parsePoint p2)
parseLineImpl ("toggle":p1:"through":p2:_) = Instruction Toggle (parsePoint p1) (parsePoint p2)
parseLineImpl _ = Instruction None origin origin

parseInput = map parseLine . lines

minmax x y 
    | x < y     = (x,y)
    | otherwise = (y,x)

invert 1 = 0
invert 0 = 1

newValues1 :: Operation -> [Int] -> [Int]
newValues1 On old       = replicate (length old) 1
newValues1 Off old      = replicate (length old) 0
newValues1 Toggle old   = map invert old

newValues2 :: Operation -> [Int] -> [Int]
newValues2 On       = map (+ 1)
newValues2 Off      = map ((max 0) . (flip (-) 1))
newValues2 Toggle   = map (+ 2)

newRow newValues op n1 n2 old = let
    (front, middleBack) = splitAt n1 old
    (middle, back) = splitAt (n2 - n1 + 1) middleBack
    newMiddle = newValues op middle
    in front ++ newMiddle ++ back

doOperation newValues grid (Instruction op (a,b) (c,d)) = let
    (x1,x2) = minmax a c
    (y1,y2) = minmax b d
    (front, middleBack) = splitAt y1 grid
    (middle, back) = splitAt (y2 - y1 + 1) middleBack
    newMiddle = map (newRow newValues op x1 x2) middle
    in front ++ newMiddle ++ back

doAll :: (Operation -> [Int] -> [Int]) -> [[Int]] -> [Instruction] -> [[Int]]
doAll newValues grid (op:ops) = doAll newValues (doOperation newValues grid op) ops
doAll _ grid _ = grid

day6 newValues = sum . map sum . doAll newValues (replicate 1000 (replicate 1000 0))

part1 = day6 newValues1

part2 = day6 newValues2

main = aoc part1 part2 parseInput
