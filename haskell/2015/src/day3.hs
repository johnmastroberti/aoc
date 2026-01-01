module Main where

import Util
import Data.List

data Direction = North | South | East | West
parseDirection :: Char -> Direction
parseDirection '^' = North
parseDirection '>' = East
parseDirection 'v' = South
parseDirection '<' = West

type Point = (Int, Int)
move :: Point -> Direction -> Point
move (a,b) North = (a, b+1)
move (a,b) South = (a, b-1)
move (a,b) East  = (a+1, b)
move (a,b) West  = (a-1, b)

unique :: Eq a => [a] -> [a]
unique (x:y:xs) = if x == y then unique (x:xs) else x:(unique (y:xs))
unique [x] = [x]
unique [] = []

countUnique :: Ord a => [a] -> Int
countUnique = length . unique . sort

separateMovesImpl (santa, robo) (x:y:xs) = separateMovesImpl (santa ++ [x], robo ++ [y]) xs
separateMovesImpl separated [] = separated
separateMoves = separateMovesImpl ([], [])

doMoves = scanl move (0,0)

parseInput = map parseDirection
part1 = countUnique . doMoves
part2 moves = let 
    (santa, robo) = separateMoves moves
    visits = (doMoves santa) ++ (doMoves robo)
    in countUnique visits

main = aoc part1 part2 parseInput
