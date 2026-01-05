from pymongo import MongoClient
from datetime import datetime
import getpass
import urllib.parse

# Konfiguracja połączenia
password = getpass.getpass("Wprowadź hasło do bazy danych: ")
password = urllib.parse.quote_plus(password)

uri = f"mongodb+srv://krzysztof400:{password}@krzysztof400.uun4lbq.mongodb.net/?retryWrites=true&w=majority"

try:
    client = MongoClient(uri)
    client.admin.command('ping')
    print("Połączono z MongoDB Atlas!")
    
    db = client.library
    
except Exception as e:
    print(f"Wystąpił błąd połączenia: {e}")

# 1. Wyszukaj wszystkie książki napisane po polsku w gatunku "Fantasy"
print("--- Książki Fantasy po polsku ---")
cursor = db.books.find({"language": "Polski", "genres": "Fantasy"})
for doc in cursor:
    print(f"Tytuł: {doc['title']}")

# 2. Książki, których średnia ocena w recenzjach to co najmniej 4
print("\n--- Książki ze średnią oceną >= 4 ---")
pipeline_avg = [
    {"$group": {"_id": "$book_id", "avgRating": {"$avg": "$rating"}}},
    {"$match": {"avgRating": {"$gte": 4}}},
    {"$lookup": {
        "from": "books",
        "localField": "_id",
        "foreignField": "_id",
        "as": "book_info"
    }},
    {"$project": {"book_info.title": 1, "avgRating": 1}}
]
for doc in db.reviews.aggregate(pipeline_avg):
    print(doc)

# 3. Wyszukaj książki autora o nazwisku "Lem"
print("\n--- Książki Stanisława Lema ---")
pipeline_lem = [
    {"$match": {"name.last": "Lem"}},
    {"$lookup": {
        "from": "books",
        "localField": "_id",
        "foreignField": "author",
        "as": "books"
    }},
    {"$project": {"books.title": 1}}
]
for doc in db.authors.aggregate(pipeline_lem):
    print(doc)

# 4. Książki polskich autorów + nazwiska + średnia ocena
print("\n--- Polscy autorzy i oceny ich książek ---")
pipeline_pl = [
    {"$lookup": {
        "from": "authors",
        "localField": "author",
        "foreignField": "_id",
        "as": "author"
    }},
    {"$unwind": "$author"},
    {"$match": {"author.country": "Polska"}},
    {"$lookup": {
        "from": "reviews",
        "localField": "_id",
        "foreignField": "book_id",
        "as": "reviews"
    }},
    {"$project": {
        "title": 1,
        "author_lastname": "$author.name.last",
        "avg_rating": {"$avg": "$reviews.rating"}
    }}
]
for doc in db.books.aggregate(pipeline_pl):
    print(doc)

# 5. Nazwisko autora oraz liczba napisanych książek
print("\n--- Liczba książek per autor ---")
pipeline_count = [
    {"$lookup": {
        "from": "books",
        "localField": "_id",
        "foreignField": "author",
        "as": "books"
    }},
    {"$project": {
        "lastname": "$name.last",
        "book_count": {"$size": "$books"}
    }}
]
for doc in db.authors.aggregate(pipeline_count):
    print(f"{doc['lastname']}: {doc['book_count']}")

# 6. Średnia ocena książek każdego autora
print("\n--- Średnia ocena autora ---")
pipeline_auth_avg = [
    {"$lookup": {
        "from": "reviews",
        "localField": "_id",
        "foreignField": "book_id",
        "as": "reviews"
    }},
    {"$unwind": "$reviews"},
    {"$lookup": {
        "from": "authors",
        "localField": "author",
        "foreignField": "_id",
        "as": "author_info"
    }},
    {"$unwind": "$author_info"},
    {"$group": {
        "_id": "$author_info._id",
        "lastname": {"$first": "$author_info.name.last"},
        "avg_rating": {"$avg": "$reviews.rating"}
    }}
]
for doc in db.books.aggregate(pipeline_auth_avg):
    print(f"{doc['lastname']}: {doc['avg_rating']}")

# 7. Znajdź autorów, którzy nie otrzymali żadnej nagrody
print("\n--- Autorzy bez nagród ---")
no_awards = db.authors.find({
    "$or": [
        {"awards": {"$size": 0}},
        {"awards": {"$exists": False}}
    ]
})
for doc in no_awards:
    print(doc['name']['last'])

# 8. Policz ile książek przypada na każdy gatunek literacki
print("\n--- Liczba książek wg gatunku ---")
pipeline_genres = [
    {"$unwind": "$genres"},
    {"$group": {
        "_id": "$genres",
        "count": {"$sum": 1}
    }}
]
for doc in db.books.aggregate(pipeline_genres):
    print(f"{doc['_id']}: {doc['count']}")

# 9. Znajdź osobę, która napisała najwięcej recenzji
print("\n--- Najaktywniejszy recenzent ---")
pipeline_top_reviewer = [
    {"$group": {
        "_id": "$reviewer",
        "count": {"$sum": 1}
    }},
    {"$sort": {"count": -1}},
    {"$limit": 1}
]
for doc in db.reviews.aggregate(pipeline_top_reviewer):
    print(f"{doc['_id']} napisał {doc['count']} recenzji")

# 10. Policz średnią ocenę książek w zależności od języka
print("\n--- Średnia ocena wg języka ---")
pipeline_lang = [
    {"$lookup": {
        "from": "reviews",
        "localField": "_id",
        "foreignField": "book_id",
        "as": "reviews"
    }},
    {"$unwind": "$reviews"},
    {"$group": {
        "_id": "$language",
        "avg_rating": {"$avg": "$reviews.rating"}
    }}
]
for doc in db.books.aggregate(pipeline_lang):
    print(f"{doc['_id']}: {doc['avg_rating']}")