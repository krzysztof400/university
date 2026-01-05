from pymongo import MongoClient
from datetime import datetime
import urllib.parse
import getpass

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

# Czyszczenie kolekcji przed testem
db.authors.drop()
db.books.drop()
db.reviews.drop()

# 1. Dodawanie Autorów
authors_data = [
    {
        "name": {"first": "Andrzej", "last": "Sapkowski"},
        "country": "Polska",
        "birth": datetime(1948, 6, 21),
        "death": None, # Data śmierci nieznana/żyje 
        "awards": [{"name": "Paszport Polityki", "year": 1997}]
    },
    {
        "name": {"first": "Stanisław", "last": "Lem"},
        "country": "Polska",
        "birth": datetime(1921, 9, 12),
        "death": datetime(2006, 3, 27),
        "awards": [{"name": "Order Orła Białego", "year": 1996}]
    },
    {
        "name": {"first": "J.R.R.", "last": "Tolkien"},
        "country": "UK",
        "birth": datetime(1892, 1, 3),
        "death": datetime(1973, 9, 2),
        "awards": []
    },
    {
        "name": {"first": "J.K.", "last": "Rowling"},
        "country": "UK",
        "birth": datetime(1965, 7, 31),
        "death": None, # Brak pola awards (test braku pola)
    }
]

res_authors = db.authors.insert_many(authors_data)
# Pobranie ID wstawionych autorów do referencji
sapkowski_id = res_authors.inserted_ids[0]
lem_id = res_authors.inserted_ids[1]
tolkien_id = res_authors.inserted_ids[2]
rowling_id = res_authors.inserted_ids[3]

# 2. Dodawanie Książek (referencje + poddokumenty)
books_data = [
    {
        "title": "Ostatnie życzenie",
        "isbn": "978-83-7578-063-5",
        "publication_year": 1993,
        "language": "Polski",
        "author": sapkowski_id,
        "publisher": {"name": "SuperNOWA", "country": "Polska"},
        "genres": ["Fantasy", "Opowiadania"]
    },
    {
        "title": "Miecz przeznaczenia",
        "isbn": "978-83-7578-064-2",
        "publication_year": 1992,
        "language": "Polski",
        "author": sapkowski_id,
        "publisher": {"name": "SuperNOWA", "country": "Polska"},
        "genres": ["Fantasy", "Opowiadania"]
    },
    {
        "title": "Krew elfów",
        "isbn": "978-83-7578-065-9",
        "publication_year": 1994,
        "language": "Polski",
        "author": sapkowski_id,
        "publisher": {"name": "SuperNOWA", "country": "Polska"},
        "genres": ["Fantasy", "Powieść"]
    },
    {
        "title": "Solaris",
        "isbn": "978-83-08-04905-1",
        "publication_year": 1961,
        "language": "Polski",
        "author": lem_id,
        "publisher": {"name": "Wydawnictwo Literackie", "country": "Polska"},
        "genres": ["Sci-Fi", "Filozoficzna"]
    },
    {
        "title": "The Hobbit",
        "isbn": "978-0-261-10295-6",
        "publication_year": 1937,
        "language": "English",
        "author": tolkien_id,
        "publisher": {"name": "George Allen & Unwin", "country": "UK"},
        "genres": ["Fantasy"]
    }
]

res_books = db.books.insert_many(books_data)
wiedzmin_id = res_books.inserted_ids[0] # ID "Ostatnie życzenie"

# 3. Dodawanie Recenzji
reviews_data = [
    {
        "book_id": wiedzmin_id,
        "reviewer": "JanKowalski",
        "rating": 5,
        "text": "Arcydzieło!"
    },
    {
        "book_id": wiedzmin_id,
        "reviewer": "AnnaNowak",
        "rating": 2, # Ocena poniżej 3
        "text": "Nie podoba mi się styl."
    },
    {
        "book_id": wiedzmin_id,
        "reviewer": "JanKowalski", # Ten sam autor recenzji
        "rating": 5,
        "text": "Po ponownym przeczytaniu nadal super."
    }
]
db.reviews.insert_many(reviews_data)

print("Dane zostały pomyślnie załadowane.")