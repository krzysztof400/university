import random
import numpy as np
import matplotlib.pyplot as plt
import time
import math
from typing import List, Tuple, Dict

# Implementacja klasycznego QuickSort
def partition(arr: List[int], low: int, high: int, comparisons: List[int]) -> int:
    """
    Funkcja dzieląca tablicę na dwie części i zwracająca indeks pivota.
    """
    pivot = arr[high]
    i = low - 1
    
    for j in range(low, high):
        comparisons[0] += 1  # Zwiększamy licznik porównań
        if arr[j] <= pivot:
            i += 1
            arr[i], arr[j] = arr[j], arr[i]
    
    arr[i+1], arr[high] = arr[high], arr[i+1]
    return i + 1

def quicksort(arr: List[int], low: int, high: int, comparisons: List[int]) -> None:
    """
    Klasyczny algorytm QuickSort.
    """
    if low < high:
        pivot_index = partition(arr, low, high, comparisons)
        quicksort(arr, low, pivot_index - 1, comparisons)
        quicksort(arr, pivot_index + 1, high, comparisons)

# Implementacja Dual-Pivot QuickSort ze strategią COUNT
def dual_pivot_partition(arr: List[int], low: int, high: int, comparisons: List[int]) -> Tuple[int, int]:
    """
    Funkcja dzieląca tablicę na trzy części przy użyciu dwóch pivotów.
    Zwraca indeksy pivotów.
    Implementacja strategii COUNT.
    """
    # Upewniamy się, że pivoty są we właściwej kolejności (p < q)
    comparisons[0] += 1
    if arr[low] > arr[high]:
        arr[low], arr[high] = arr[high], arr[low]
    
    p = arr[low]    # Pierwszy pivot (mniejszy)
    q = arr[high]   # Drugi pivot (większy)
    
    # Inicjalizacja indeksów dla elementów mniejszych od p, między p i q oraz większych od q
    left_idx = low + 1
    right_idx = high - 1
    current_idx = left_idx
    
    # Liczniki elementów małych (mniejszych od p) i dużych (większych od q)
    small_count = 0
    large_count = 0
    
    while current_idx <= right_idx:
        # Strategia COUNT: wybór odpowiedniego pivota do porównania
        if large_count <= small_count:
            # Najpierw porównujemy z pierwszym pivotem (p)
            comparisons[0] += 1
            if arr[current_idx] < p:
                # Element mniejszy od p
                arr[current_idx], arr[left_idx] = arr[left_idx], arr[current_idx]
                left_idx += 1
                current_idx += 1
                small_count += 1
            else:
                # Element większy lub równy p, sprawdzamy q
                comparisons[0] += 1
                if arr[current_idx] > q:
                    # Element większy od q
                    while arr[right_idx] > q and current_idx < right_idx:
                        comparisons[0] += 1
                        right_idx -= 1
                        large_count += 1
                    
                    arr[current_idx], arr[right_idx] = arr[right_idx], arr[current_idx]
                    right_idx -= 1
                    
                    # Sprawdzamy zamieniony element w stosunku do p
                    comparisons[0] += 1
                    if arr[current_idx] < p:
                        arr[current_idx], arr[left_idx] = arr[left_idx], arr[current_idx]
                        left_idx += 1
                        small_count += 1
                    
                    current_idx += 1
                else:
                    # Element pomiędzy p i q
                    current_idx += 1
        else:
            # Najpierw porównujemy z drugim pivotem (q)
            comparisons[0] += 1
            if arr[current_idx] > q:
                # Element większy od q
                while arr[right_idx] > q and current_idx < right_idx:
                    comparisons[0] += 1
                    right_idx -= 1
                    large_count += 1
                
                arr[current_idx], arr[right_idx] = arr[right_idx], arr[current_idx]
                right_idx -= 1
                
                # Sprawdzamy zamieniony element w stosunku do p
                comparisons[0] += 1
                if arr[current_idx] < p:
                    arr[current_idx], arr[left_idx] = arr[left_idx], arr[current_idx]
                    left_idx += 1
                    small_count += 1
                
                current_idx += 1
            else:
                # Element mniejszy lub równy q, sprawdzamy p
                comparisons[0] += 1
                if arr[current_idx] < p:
                    # Element mniejszy od p
                    arr[current_idx], arr[left_idx] = arr[left_idx], arr[current_idx]
                    left_idx += 1
                    current_idx += 1
                    small_count += 1
                else:
                    # Element pomiędzy p i q
                    current_idx += 1
    
    # Ustawiamy pivoty na właściwe pozycje
    left_idx -= 1
    right_idx += 1
    
    arr[low], arr[left_idx] = arr[left_idx], arr[low]
    arr[high], arr[right_idx] = arr[right_idx], arr[high]
    
    return left_idx, right_idx

def dual_pivot_quicksort(arr: List[int], low: int, high: int, comparisons: List[int]) -> None:
    """
    Algorytm Dual-Pivot QuickSort ze strategią COUNT.
    """
    if low < high:
        # Podział tablicy na trzy części
        p_index, q_index = dual_pivot_partition(arr, low, high, comparisons)
        
        # Rekurencyjne sortowanie trzech podtablic
        dual_pivot_quicksort(arr, low, p_index - 1, comparisons)
        dual_pivot_quicksort(arr, p_index + 1, q_index - 1, comparisons)
        dual_pivot_quicksort(arr, q_index + 1, high, comparisons)

# Funkcje do generowania testowych tablic
def generate_random_array(n: int) -> List[int]:
    return [random.randint(1, 1000) for _ in range(n)]

def generate_sorted_ascending(n: int) -> List[int]:
    return list(range(1, n + 1))

def generate_sorted_descending(n: int) -> List[int]:
    return list(range(n, 0, -1))

# Funkcja do testowania algorytmów
def test_algorithms(array_sizes: List[int]) -> Dict:
    results = {
        'quicksort': {
            'random': [],
            'ascending': [],
            'descending': []
        },
        'dual_pivot': {
            'random': [],
            'ascending': [],
            'descending': []
        }
    }
    
    for n in array_sizes:
        # Test dla losowej tablicy
        random_arr = generate_random_array(n)
        
        # QuickSort na losowej tablicy
        arr_copy = random_arr.copy()
        comparisons = [0]
        start_time = time.time()
        quicksort(arr_copy, 0, n-1, comparisons)
        end_time = time.time()
        results['quicksort']['random'].append({
            'n': n,
            'comparisons': comparisons[0],
            'time': end_time - start_time,
            'comp_per_nlogn': comparisons[0] / (n * math.log2(n)) if n > 0 else 0
        })
        
        # Dual-Pivot QuickSort na losowej tablicy
        arr_copy = random_arr.copy()
        comparisons = [0]
        start_time = time.time()
        dual_pivot_quicksort(arr_copy, 0, n-1, comparisons)
        end_time = time.time()
        results['dual_pivot']['random'].append({
            'n': n,
            'comparisons': comparisons[0],
            'time': end_time - start_time,
            'comp_per_nlogn': comparisons[0] / (n * math.log2(n)) if n > 0 else 0
        })
        
        # Test dla tablicy posortowanej rosnąco
        asc_arr = generate_sorted_ascending(n)
        
        # QuickSort na tablicy posortowanej rosnąco
        arr_copy = asc_arr.copy()
        comparisons = [0]
        start_time = time.time()
        quicksort(arr_copy, 0, n-1, comparisons)
        end_time = time.time()
        results['quicksort']['ascending'].append({
            'n': n,
            'comparisons': comparisons[0],
            'time': end_time - start_time,
            'comp_per_nlogn': comparisons[0] / (n * math.log2(n)) if n > 0 else 0
        })
        
        # Dual-Pivot QuickSort na tablicy posortowanej rosnąco
        arr_copy = asc_arr.copy()
        comparisons = [0]
        start_time = time.time()
        dual_pivot_quicksort(arr_copy, 0, n-1, comparisons)
        end_time = time.time()
        results['dual_pivot']['ascending'].append({
            'n': n,
            'comparisons': comparisons[0],
            'time': end_time - start_time,
            'comp_per_nlogn': comparisons[0] / (n * math.log2(n)) if n > 0 else 0
        })
        
        # Test dla tablicy posortowanej malejąco
        desc_arr = generate_sorted_descending(n)
        
        # QuickSort na tablicy posortowanej malejąco
        arr_copy = desc_arr.copy()
        comparisons = [0]
        start_time = time.time()
        quicksort(arr_copy, 0, n-1, comparisons)
        end_time = time.time()
        results['quicksort']['descending'].append({
            'n': n,
            'comparisons': comparisons[0],
            'time': end_time - start_time,
            'comp_per_nlogn': comparisons[0] / (n * math.log2(n)) if n > 0 else 0
        })
        
        # Dual-Pivot QuickSort na tablicy posortowanej malejąco
        arr_copy = desc_arr.copy()
        comparisons = [0]
        start_time = time.time()
        dual_pivot_quicksort(arr_copy, 0, n-1, comparisons)
        end_time = time.time()
        results['dual_pivot']['descending'].append({
            'n': n,
            'comparisons': comparisons[0],
            'time': end_time - start_time,
            'comp_per_nlogn': comparisons[0] / (n * math.log2(n)) if n > 0 else 0
        })
    
    return results

# Funkcja do wizualizacji wyników
def plot_results(results: Dict, array_sizes: List[int]) -> None:
    # Wykresy liczby porównań
    plt.figure(figsize=(12, 8))
    
    # Dane losowe
    plt.subplot(2, 2, 1)
    plt.title('Liczba porównań - Dane losowe')
    plt.plot(array_sizes, [res['comparisons'] for res in results['quicksort']['random']], 'bo-', label='QuickSort')
    plt.plot(array_sizes, [res['comparisons'] for res in results['dual_pivot']['random']], 'ro-', label='Dual-Pivot QuickSort')
    plt.xlabel('Rozmiar tablicy (n)')
    plt.ylabel('Liczba porównań')
    plt.legend()
    plt.grid(True)
    
    # Dane posortowane rosnąco
    plt.subplot(2, 2, 2)
    plt.title('Liczba porównań - Dane posortowane rosnąco')
    plt.plot(array_sizes, [res['comparisons'] for res in results['quicksort']['ascending']], 'bo-', label='QuickSort')
    plt.plot(array_sizes, [res['comparisons'] for res in results['dual_pivot']['ascending']], 'ro-', label='Dual-Pivot QuickSort')
    plt.xlabel('Rozmiar tablicy (n)')
    plt.ylabel('Liczba porównań')
    plt.legend()
    plt.grid(True)
    
    # Dane posortowane malejąco
    plt.subplot(2, 2, 3)
    plt.title('Liczba porównań - Dane posortowane malejąco')
    plt.plot(array_sizes, [res['comparisons'] for res in results['quicksort']['descending']], 'bo-', label='QuickSort')
    plt.plot(array_sizes, [res['comparisons'] for res in results['dual_pivot']['descending']], 'ro-', label='Dual-Pivot QuickSort')
    plt.xlabel('Rozmiar tablicy (n)')
    plt.ylabel('Liczba porównań')
    plt.legend()
    plt.grid(True)
    
    # Stosunek liczby porównań do n*log(n)
    plt.subplot(2, 2, 4)
    plt.title('Stała przy n*log₂(n) dla liczby porównań')
    plt.plot(array_sizes, [res['comp_per_nlogn'] for res in results['quicksort']['random']], 'bo-', label='QuickSort (losowe)')
    plt.plot(array_sizes, [res['comp_per_nlogn'] for res in results['dual_pivot']['random']], 'ro-', label='Dual-Pivot (losowe)')
    plt.xlabel('Rozmiar tablicy (n)')
    plt.ylabel('Liczba porównań / (n*log₂(n))')
    plt.legend()
    plt.grid(True)
    
    plt.tight_layout()
    plt.savefig('quicksort_comparison.png')
    plt.show()

# Główna funkcja testująca
def main():
    # Testujemy dla n = 8 i n = 32 zgodnie z zadaniem
    array_sizes = [8, 32]
    
    # Dodatkowe rozmiary dla lepszej wizualizacji trendu
    extended_sizes = [8, 16, 32, 64, 128, 256, 512, 1024]
    
    # Uruchamiamy testy
    results = test_algorithms(extended_sizes)
    
    # Wyświetlamy wyniki dla n = 8 i n = 32
    print("Wyniki dla zadanych rozmiarów tablic (n = 8, 32):")
    print("=" * 80)
    
    for n in array_sizes:
        print(f"\nRozmiar tablicy: n = {n}")
        print("-" * 50)
        
        # Znajdź indeks dla danego rozmiaru tablicy
        idx = extended_sizes.index(n)
        
        # Dane losowe
        qs_random = results['quicksort']['random'][idx]
        dp_random = results['dual_pivot']['random'][idx]
        print(f"Dane losowe:")
        print(f"  QuickSort:        {qs_random['comparisons']} porównań, {qs_random['comp_per_nlogn']:.3f} * n*log₂(n)")
        print(f"  Dual-Pivot QS:    {dp_random['comparisons']} porównań, {dp_random['comp_per_nlogn']:.3f} * n*log₂(n)")
        print(f"  Poprawa:          {(1 - dp_random['comparisons']/qs_random['comparisons']) * 100:.2f}%")
        
        # Dane posortowane rosnąco
        qs_asc = results['quicksort']['ascending'][idx]
        dp_asc = results['dual_pivot']['ascending'][idx]
        print(f"Dane posortowane rosnąco:")
        print(f"  QuickSort:        {qs_asc['comparisons']} porównań, {qs_asc['comp_per_nlogn']:.3f} * n*log₂(n)")
        print(f"  Dual-Pivot QS:    {dp_asc['comparisons']} porównań, {dp_asc['comp_per_nlogn']:.3f} * n*log₂(n)")
        print(f"  Poprawa:          {(1 - dp_asc['comparisons']/qs_asc['comparisons']) * 100:.2f}%")
        
        # Dane posortowane malejąco
        qs_desc = results['quicksort']['descending'][idx]
        dp_desc = results['dual_pivot']['descending'][idx]
        print(f"Dane posortowane malejąco:")
        print(f"  QuickSort:        {qs_desc['comparisons']} porównań, {qs_desc['comp_per_nlogn']:.3f} * n*log₂(n)")
        print(f"  Dual-Pivot QS:    {dp_desc['comparisons']} porównań, {dp_desc['comp_per_nlogn']:.3f} * n*log₂(n)")
        print(f"  Poprawa:          {(1 - dp_desc['comparisons']/qs_desc['comparisons']) * 100:.2f}%")
    
    # Wyświetlamy średnią stałą przy n*log(n) dla różnych typów danych
    print("\nŚrednia stała przy n*log₂(n) dla liczby porównań:")
    print("=" * 80)
    avg_qs_random = np.mean([res['comp_per_nlogn'] for res in results['quicksort']['random']])
    avg_dp_random = np.mean([res['comp_per_nlogn'] for res in results['dual_pivot']['random']])
    print(f"  QuickSort (dane losowe):          {avg_qs_random:.3f}")
    print(f"  Dual-Pivot QuickSort (dane losowe): {avg_dp_random:.3f}")
    
    # Wizualizacja wyników
    plot_results(results, extended_sizes)

if __name__ == "__main__":
    main()