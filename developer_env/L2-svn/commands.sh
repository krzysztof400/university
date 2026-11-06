# Krzysztof Zając, indeks: 279757
# Lista 2 SVN

# ==========================================
# Zadanie 1
# ==========================================
mkdir -p zad1 && cd zad1
svnadmin create repo1
REPO1="file://$(pwd)/repo1"
svn checkout $REPO1 wc1
svn checkout $REPO1 wc2

# Inicjalizacja pliku w pierwszej kopii
cd wc1
echo "Wspolna poczatkowa zawartosc" > konflikt.txt
svn add konflikt.txt
svn commit -m "Zadanie 1: Inicjalizacja pliku"

# Zmiana w drugiej kopii
cd ../wc2
svn update
echo "Zmiana wykonana w kopii nr 2" > konflikt.txt
svn commit -m "Zadanie 1: Modyfikacja w kopii wc2"

# Konfliktowa zmiana w pierwszej kopii i wywołanie konfliktu
cd ../wc1
echo "Konfliktowa zmiana w kopii nr 1" > konflikt.txt
echo "--- Wywolanie konfliktu (svn update --accept postpone) ---"
svn update --accept postpone

# Prezentacja plików pomocniczych i znaczników
echo "--- Pliki w katalogu po wystapieniu konfliktu ---"
ls -la
echo "--- Zawartosc pliku z markerami konfliktu (konflikt.txt) ---"
cat konflikt.txt
echo "--- Zawartosc plikow pomocniczych SVN ---"
cat konflikt.txt.*

# Ręczne rozwiązanie
echo "--- Reczne rozwiazanie konfliktu ---"
echo "Rozwiazana ostateczna wersja tekstu" > konflikt.txt
svn resolved konflikt.txt
svn commit -m "Zadanie 1: Reczne rozwiazanie konfliktu"

# Widoczny ślad działań w repozytorium
svn log -v
cd ../..


# ==========================================
# Zadanie 2
# ==========================================
mkdir -p zad2 && cd zad2
svnadmin create repo2
REPO2="file://$(pwd)/repo2"
svn checkout $REPO2 wc_full
cd wc_full
mkdir -p projekt/modulA/src projekt/modulB/src projekt/docs
touch projekt/modulA/src/a.cpp projekt/modulB/src/b.cpp projekt/docs/manual.txt
svn add projekt
svn commit -m "Utworzenie struktury projektu"
cd ..

# Sprawdzanie sparse directories
svn checkout $REPO2 wc_sparse --depth empty
cd wc_sparse
echo "--- Zawartosc (empty) ---"
find . -not -path '*/\.svn*' | sort
svn update --set-depth immediates projekt
echo "--- Zawartosc (immediates dla projekt) ---"
find . -not -path '*/\.svn*' | sort
svn update --set-depth infinity projekt/modulA
echo "--- Zawartosc (infinity dla modulA) ---"
find . -not -path '*/\.svn*' | sort

# Widoczny ślad działań w repozytorium
svn log -v
cd ../..


# ==========================================
# Zadanie 3
# ==========================================
mkdir -p zad3 && cd zad3
svnadmin create repo3
REPO3="file://$(pwd)/repo3"
svn checkout $REPO3 wc
cd wc
echo '#!/bin/bash\necho "Test"' > skrypt.sh
svn add skrypt.sh
svn commit -m "Dodanie skryptu bez atrybutu executable"
ls -l skrypt.sh

# Ustawienie własności svn:executable
svn propset svn:executable '*' skrypt.sh
svn commit -m "Ustawienie atrybutu svn:executable"
ls -l skrypt.sh

# Usunięcie własności svn:executable
svn propdel svn:executable skrypt.sh
svn commit -m "Usuniecie atrybutu svn:executable"
ls -l skrypt.sh

# Widoczny ślad działań w repozytorium
svn log -v
cd ../..


# ==========================================
# Zadanie 4
# ==========================================
mkdir -p zad4 && cd zad4
svnadmin create repo4
REPO4="file://$(pwd)/repo4"
svn checkout $REPO4 wc
cd wc
mkdir externals
svn add externals
svn propset svn:externals "https://repo.cs.pwr.edu.pl/info/SP/l1/ l1_ext" externals
svn commit -m "Konfiguracja svn:externals"
svn update
ls -la externals
cd ..

# Checkout z ignorowaniem externals
svn checkout $REPO4 wc_ignore --ignore-externals
ls -la wc_ignore/externals

# Widoczny ślad działań w repozytorium
cd wc
svn log -v
cd ../..


# ==========================================
# Zadanie 5
# ==========================================
mkdir -p zad5 && cd zad5
svnadmin create repo5
REPO5="file://$(pwd)/repo5"
svn checkout $REPO5 wc1
svn checkout $REPO5 wc2

cd wc1
echo "Sekretne dane" > plik.txt
svn add plik.txt
svn commit -m "Dodanie pliku do blokowania"

# Zakładanie blokady
svn lock plik.txt -m "Zablokowane przez wc1"
svn info plik.txt | grep Lock

cd ../wc2
svn update
# Próba blokady i wymuszenie/podkradzenie (force lock)
svn lock plik.txt || true
svn lock plik.txt --force
svn info plik.txt | grep Lock

cd ../wc1
# Zrywanie (break) blokady po stronie serwera/URL
svn unlock $REPO5/plik.txt --force
svn info plik.txt | grep Lock || echo "Brak blokady - pomyslnie zdjeta"

# Widoczny ślad działań w repozytorium
svn log -v
cd ../..


# ==========================================
# Zadanie 6
# ==========================================
mkdir -p zad6 && cd zad6
svnadmin create repo6
REPO6="file://$(pwd)/repo6"
svn checkout $REPO6 wc
cd wc

# 1. Dodanie katalogów a i b z a.txt i b.txt
mkdir a b
echo "plik a" > a/a.txt
echo "plik b" > b/b.txt
svn add a b
svn commit -m "Rev 1: Dodanie a.txt i b.txt"

# 2. Zamiana plików miejscami i modyfikacja
svn move b/b.txt a/b.txt
svn move a/a.txt b/a.txt
echo "linia 2" >> a/b.txt
echo "linia 2" >> b/a.txt
svn commit -m "Rev 2: Przeniesienie i modyfikacja"

# 3. Dopisanie tekstu do a.txt (obecnie w b/a.txt) i zmiana nazwy na c.txt
echo "linia 3" >> b/a.txt
svn move b/a.txt b/c.txt
svn commit -m "Rev 3: Modyfikacja i zmiana nazwy na c.txt"

# 4. Dopisanie tekstu do b.txt (obecnie w a/b.txt) i zmiana nazwy na a.txt
echo "linia 3" >> a/b.txt
svn move a/b.txt a/a.txt
svn commit -m "Rev 4: Modyfikacja i zmiana nazwy na a.txt"

# 5. Dodanie nowego pliku b.txt w podkatalogu b
echo "nowy plik b" > b/b.txt
svn add b/b.txt
svn commit -m "Rev 5: Nowy plik b.txt w b"

# Demonstracja operative-revision vs peg-revision
echo "--- svn cat -r 1 dla b/c.txt (Operative Revision bez Peg - błąd, nie istniał pod ta nazwa) ---"
svn cat -r 1 $REPO6/b/c.txt || echo "Oczekiwany błąd: b/c.txt nie istniał w rev 1"

echo "--- svn cat z Peg Revision (@HEAD) - SVN śledzi historię obiektu b/c.txt wstecz do rev 1 ---"
svn cat -r 1 $REPO6/b/c.txt@HEAD

# Widoczny ślad działań w repozytorium
svn log -v
cd ../..


# ==========================================
# Zadanie 7
# ==========================================
mkdir -p zad7 && cd zad7

cat << 'EOF' > ustaw_mime.sh
#!/bin/bash
# Skrypt ustawiajacy svn:mime-type="text/plain;charset=utf-8" dla plikow pasujacych do wzorca

if [ "$#" -ne 2 ]; then
    echo "Uzycie: $0 <URL_REPOZYTORIUM> <WZOREC>"
    exit 1
fi

URL="$1"
PATTERN="$2"

TMPDIR=$(mktemp -d)
svn checkout -q "$URL" "$TMPDIR"

find "$TMPDIR" -type f -name "$PATTERN" ! -path '*/\.svn/*' | while read plik; do
    svn propset svn:mime-type "text/plain;charset=utf-8" "$plik"
done

svn commit -m "Zadanie 7: Ustawienie svn:mime-type text/plain;charset=utf-8" "$TMPDIR"
rm -rf "$TMPDIR"
EOF

chmod +x ustaw_mime.sh

# Pobranie bezwzględnej ścieżki do repozytorium z Zadania 6
REPO6_ABS="file://$(cd ../zad6 && pwd)/repo6"

# Uruchomienie skryptu dla plików *.txt w repozytorium z Zadania 6
./ustaw_mime.sh "$REPO6_ABS" "*.txt"

# Weryfikacja ustawionej własności mime-type
echo "--- Weryfikacja własności svn:mime-type w repozytorium z Zadania 6 ---"
svn proplist -v "$REPO6_ABS/a/a.txt"
svn proplist -v "$REPO6_ABS/b/c.txt"

# Widoczny ślad działań w repozytorium dla Zadania 7
svn log -v "$REPO6_ABS"
cd ..