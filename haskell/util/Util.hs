module Util where

import System.IO
import System.Environment

byChars :: String -> (String -> (String, String)) -> IO ()
byChars fileName part12 = do
    file <- openFile fileName ReadMode
    contents <- hGetContents file
    let (part1, part2) = part12 contents
    putStr "Part 1: "
    putStrLn part1
    putStr "Part 2: "
    putStrLn part2
    hClose file

bindUserArg fun part12 = do
    args <- getArgs
    fun (head args) part12

callBoth :: Show b => (a -> b) -> (a -> b) -> (String -> a) -> String -> (String, String)
callBoth part1 part2 parseInput inputString = 
    (show $ part1 input, show $ part2 input) 
    where input = parseInput inputString
