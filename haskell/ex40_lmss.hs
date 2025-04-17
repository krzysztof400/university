import Data.List (tails)

lmss' (x:xs) = x : longest (map lmss' (snd (descTails x (tails xs))))
    where
        longest = foldr (\y arr ->
            if length y > length arr then y
            else arr
            ) []
        descTails x = foldr (\ys (minHead, acc) ->
            case ys of
                [] -> (minHead, acc)
                (y:_) ->
                    if y > x && y < minHead then (y, acc ++ [ys])
                    else (minHead, acc)
            ) (maxBound, [])