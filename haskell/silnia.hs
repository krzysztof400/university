-- silnia.hs
module Main where

-- Funkcja do liczenia silni
silnia :: Integer -> Integer
silnia 0 = 1
silnia n = n * silnia (n - 1)

-- Przykład użycia
main :: IO ()
main = do
    putStrLn "Podaj liczbę (lub wpisz 'exit' aby zakończyć):"
    input <- getLine
    if input == "exit"
        then putStrLn "Koniec programu."
        else do
            let number = read input :: Integer
            putStrLn $ "Silnia " ++ show number ++ " to " ++ show (silnia number)
            main