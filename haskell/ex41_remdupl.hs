remdupl' :: Eq a => [a] -> [a]
remdupl' = foldr(\x acc -> 
    if null acc || x /= head acc then x : acc
    else acc)[]