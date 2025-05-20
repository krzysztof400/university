{-|
Simple Eliza-like chatbot specialized in science fiction books.
Gathers user information during the conversation and stores it to a file at the end.
-}

{-# LANGUAGE OverloadedStrings #-}

module Main where

import System.IO (hFlush, stdout)
import Data.Maybe (fromMaybe)
import Data.List (isInfixOf)
import qualified Data.Text as T
import qualified Data.Text.IO as TIO
import Control.Monad (when)
import System.Directory (doesFileExist)

-- | Data structure to hold user info
data UserInfo = UserInfo
  { uiName        :: Maybe T.Text
  , uiFavoriteBook:: Maybe T.Text
  , uiFavoriteAuthor :: Maybe T.Text
  } deriving (Show)

-- | Initial empty user info
emptyInfo :: UserInfo
emptyInfo = UserInfo Nothing Nothing Nothing

-- | Update user info based on input
updateInfo :: UserInfo -> T.Text -> UserInfo
updateInfo ui input = ui'
  where
    lower = T.toLower input
    ui1 = case matchPrefix "my name is " lower of
      Just name -> ui { uiName = Just name }
      Nothing   -> ui
    ui2 = case matchPrefix "i love the book " lower of
      Just bk   -> ui1 { uiFavoriteBook = Just bk }
      Nothing   -> ui1
    ui3 = case matchPrefix "i love author " lower of
      Just au   -> ui2 { uiFavoriteAuthor = Just au }
      Nothing   -> ui2
    ui' = ui3

-- | Simple prefix matcher
matchPrefix :: T.Text -> T.Text -> Maybe T.Text
matchPrefix pat txt
  | pat `T.isPrefixOf` txt = let rest = T.strip $ T.drop (T.length pat) txt
                             in Just rest
  | otherwise = Nothing

-- | Generate a response based on input and current info
respond :: UserInfo -> T.Text -> T.Text
respond ui input
  | any (`T.isInfixOf` lower) ["book","read"]
      = "Have you read any recent SF novels lately?"
  | any (`T.isInfixOf` lower) ["author","writer"]
      = "Who is your favorite science fiction author?"
  | any (`T.isInfixOf` lower) ["favorite book"]
      = case uiFavoriteBook ui of
          Just bk -> "You told me your favorite book is " <> bk <> ". That's a great choice!"
          Nothing -> "What's your favorite science fiction book?"
  | "bye" `T.isInfixOf` lower || "exit" `T.isInfixOf` lower
      = "Goodbye! I'll remember our chat."
  | otherwise
      = defaultResponses
  where
    lower = T.toLower input
    defaultResponses = "Interesting! Tell me more about your taste in science fiction."

-- | Main conversation loop
conversation :: UserInfo -> IO UserInfo
conversation ui = do
  putStr "You: "
  hFlush stdout
  line <- TIO.getLine
  let ui' = updateInfo ui line
      reply = respond ui' line
  TIO.putStrLn $ "Eliza: " <> reply
  if any (`T.isInfixOf` T.toLower line) ["bye","exit"]
    then return ui'
    else conversation ui'

-- | Write user info to a file
saveInfo :: FilePath -> UserInfo -> IO ()
saveInfo path ui = do
  let content = T.unlines
        [ "Name: " <> fromMaybe "<unknown>" (uiName ui)
        , "Favorite Book: " <> fromMaybe "<unknown>" (uiFavoriteBook ui)
        , "Favorite Author: " <> fromMaybe "<unknown>" (uiFavoriteAuthor ui)
        ]
  TIO.writeFile path content
  putStrLn $ "User info saved to " ++ path

main :: IO ()
main = do
  putStrLn "Welcome to Eliza (Science Fiction Edition)!"
  putStrLn "Type 'bye' or 'exit' to end the chat."
  ui <- conversation emptyInfo
  saveInfo "userinfo.txt" ui
