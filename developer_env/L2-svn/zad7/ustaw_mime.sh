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
