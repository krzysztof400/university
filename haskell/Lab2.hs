import Data.List (nub, delete)

-- Zadanie 13
-- Funkcja phi Eulera

-- phi n = length [x | x <- [1..n], gcd x n == 1]
-- phi::Int->[Int]->Int
-- phi n [] = 0
-- phi n (x:xs) = if (gcd n x) == 1 then 1 + phi n xs else phi n xs


-- Zadanie 14
-- Liczby doskonale mniejsze od 1000

divisors n = [d | d <- [1..n], n `mod` d == 0]
isPerfect n = sum (divisors n) == n
perfects k = [n | n <- [1..k], isPerfect n]











-- Zadanie 15
-- Liczby zaprzyjaźnione

divisorSum :: Int -> Int
divisorSum n = sum (nub [x + (if x /= n `div` x then n `div` x else 0) 
                          | x <- [1..floor (sqrt (fromIntegral n))], n `mod` x == 0]) - n

divisorSums :: Int -> [(Int, Int)]
divisorSums n = [(x, divisorSum x) | x <- [1..n]]

areBefriended :: [(Int, Int)] -> Int -> Int -> Bool
areBefriended ds n k = lookup n ds == Just k && lookup k ds == Just n && k /= n

befriended :: Int -> [(Int, Int)]
befriended n = let ds = divisorSums n 
               in [(k, l) | (k, l) <- ds, k > l, areBefriended ds k l]


-- Zadanie 17
-- Funkcja nub

-- nub' [] = []
-- nub'(x, xs) = x:nub'[n | n<-xs, n/=x]

-- Zadanie 20
-- Funckcja splits

splits :: [a] -> [([a], [a])]
splits xs = [(take i xs, drop i xs) | i <- [0..length xs]]


splitsv2 :: [a] -> [([a], [a])]
splitsv2 xs = go [] xs
  where
    go acc [] = [(acc, [])]
    go acc (y:ys) = (acc, y:ys) : go (acc ++ [y]) ys

-- splitsv3 :: [a] -> [([a], [a])]
-- splitsv3 xs = zip(inits xs) (tails xs)

-- Zadanie 22
-- Funkcja permutations

permutations :: [a] -> [[a]]
permutations [] = [[]]
permutations xs = [x:ys | (x, rest) <- select xs, ys <- permutations rest]

select :: [a] -> [(a, [a])]
select [] = []
select (y:ys) = (y, ys) : [(z, y:zs) | (z, zs) <- select ys]