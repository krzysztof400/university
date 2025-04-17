f x = 3.5 *x*(1-x)

safeHead [] = Nothing
safeHead (x:_) = Just x

-- usuwanie "non-ascii"
removeNonAscii ch = if elem ch ['a'..'z'] then ch
                    else ' '

-- usuwanie StopWords
filterSW slowa