-- zip
zip' [] _ =  []
zip' _ [] = []
zip' (x:xs) (y:ys) = (x,y): (zip' xs ys)

--zipWith 