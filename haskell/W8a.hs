module W8a (SimpleTree (Leaf, Node), tree1, tree2, tree3) where

import qualified Data.Tree as T hiding (Tree)

-- Typy rekurencyjne

data SimpleTree a = Leaf a | Node (SimpleTree a) (SimpleTree a) deriving (Eq)

instance Show a => Show (SimpleTree a) where
  show (Leaf x) = show x
  show (Node l r) = "(" ++ show l ++ " " ++ show r ++ ")"

tree1 = Node (Leaf 1) (Node (Leaf 2) (Leaf 3))
tree2 = Node tree1 tree1
tree3 = Node tree2 (Leaf 10)

instance Functor SimpleTree where
    fmap f (Leaf x) = Leaf (f x)
    fmap f (Node l r) = Node (fmap f l) (fmap f r)

height :: SimpleTree a -> Int
height (Leaf _) = 1
height (Node l r) = 1 + max (height l) (height r)

minHeight :: SimpleTree a -> Int
minHeight (Leaf _) = 1
minHeight (Node l r) = 1 + min (minHeight l) (minHeight r)

balancedTreeQuestion :: SimpleTree a -> Bool
balancedTreeQuestion (Leaf _) = True
balancedTreeQuestion (Node l r) = abs (height l - height r) <= 1 && balancedTreeQuestion l && balancedTreeQuestion r

-- toDatatree (Leaf x) = T.Node x []
-- toDatatree (Node l r) = T.Node undefined [toDatatree l, toDatatree r]

-- showTree t = putStrLn $ T.drawTree (toDatatree (fmap show t))