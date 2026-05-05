#!/bin/bash

# Tablica z nazwami plików (bez rozszerzenia .cpp)
FILES=("gausian_ring" "orders" "polynomials")

for file in "${FILES[@]}"; do
    echo "========================================"
    echo "⚙️ Kompilacja ${file}.cpp..."
    
    # Kompilacja do pliku wykonywalnego o tej samej nazwie
    g++ -std=c++17 "${file}.cpp" -o "${file}"
    
    # Sprawdzenie, czy kompilacja się powiodła (kod wyjścia 0)
    if [ $? -eq 0 ]; then
        echo "✅ Kompilacja udana. Uruchamianie ${file}:"
        echo "----------------------------------------"
        ./"${file}"
        echo -e "\n"
    else
        echo "❌ BŁĄD: Kompilacja pliku ${file}.cpp nie powiodła się!"
    fi
done

echo "========================================"
echo "Zakończono wykonywanie wszystkich skryptów."