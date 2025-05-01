module W8b where

import W8a
import Prelude hiding (Semigroup, Monoid, mempty, (<>))

class Semigroup m where
  (<>) :: m -> m -> m
    -- Wymaganie: (<>) jest łączne

class Semigroup m => Monoid m where 
    mempty :: m
    -- Wymaganie: mempty jest elementem neutralnym dla (<>) i mempty <> x = x <> mempty = x
    -- Wymaganie: mappend jest łączne

instance Semigroup [a] where
    (<>) = (++)

instance Monoid [a] where
    mempty = []
    -- Wymaganie: mempty jest elementem neutralnym dla (<>)
    -- mempty <> x = x
    -- x <> mempty = x
    -- Wymaganie: mappend jest łączne

newtype Sum a = Sum { getSum :: a }
  deriving (Eq, Ord, Read, Show)
instance Num a => Semigroup (Sum a) where
    Sum x <> Sum y = Sum (x + y)
instance Num a => Monoid (Sum a) where
    mempty = Sum 0
data SimpleTree a = Leaf a | Node (SimpleTree a) (SimpleTree a)

instance Foldable SimpleTree where
    foldMap f (Leaf x) = f x
    foldMap f (Node l r) = (foldMap f l) <> (foldMap f r)