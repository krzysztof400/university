#!/bin/bash

# Nazwa pliku wykonywalnego i dane
EXECUTABLE="./tsp_solve"
DATA_FILE="../data/qa194.tsp"
ITERATIONS=100

# Sprawdzenie czy plik istnieje
if [ ! -f "$EXECUTABLE" ]; then
    echo "Błąd: Nie znaleziono pliku $EXECUTABLE"
    exit 1
fi

# Tablica kombinacji: "wyspy crossover"
# 0/1 dla wysp, 0/1 dla crossover (0=OX1, 1=PMX)
configs=("0 0" "0 1" "1 0" "1 1")

echo "Rozpoczynam testy dla $ITERATIONS iteracji..."
echo "------------------------------------------"

for config in "${configs[@]}"; do
    set -- $config
    islands=$1
    crossover=$2
    
    echo "Uruchamiam: Wyspy=$islands, Crossover=$([ $crossover -eq 0 ] && echo "OX1" || echo "PMX")"
    
    # Uruchomienie programu
    $EXECUTABLE $DATA_FILE $islands $crossover
    
    echo "------------------------------------------"
done

echo "Wszystkie testy zakończone."