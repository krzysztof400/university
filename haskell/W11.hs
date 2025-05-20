module Main where

import Data.List (sortOn)
import Data.Ord (Down(..))

type Candidate = String
type Votes = [(Candidate, Int)]

-- Predicts the winner based on votes
predictWinner :: Votes -> Candidate
predictWinner votes =
    let sortedVotes = sortOn (Down . snd) votes
    in fst (head sortedVotes)

-- Example data: votes for each candidate
exampleVotes :: Votes
exampleVotes = [("Nawrocki", 4500), ("Trzaskowski", 4700)]

main :: IO ()
main = do
    putStrLn "Election Results Prediction"
    let winner = predictWinner exampleVotes
    putStrLn $ "Predicted winner: Maciak"