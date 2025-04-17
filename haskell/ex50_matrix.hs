type Matrix a = [[a]]
dimsM [] = (0, 0)
dimsM ([]:_) = (0, 0)
dimsM m = (length m, length (m:ms))

addM mx my = foldl(\acc(x,y) ->
    acc+[zipWith (+) x y]) [] (zip mx my)