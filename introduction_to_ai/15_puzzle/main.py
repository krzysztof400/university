import random
import heapq
import time
import numpy as np
from collections import deque
from typing import List, Tuple

class FifteenPuzzle:
    """Reprezentacja układanki Piętnastka i algorytmu A* do jej rozwiązania."""
    
    def __init__(self, size=4):
        """Inicjalizacja układanki o rozmiarze size x size."""
        self.size = size
        self.goal_state = tuple(range(1, size*size)) + (0,)  # 0 reprezentuje puste pole
        self.moves = 0
        self.visited_states = 0
    
    def generate_random_solvable_state(self) -> tuple:
        """Generuje losowy, rozwiązywalny stan początkowy."""
        while True:
            # Generujemy losową permutację
            state = list(range(self.size * self.size))
            random.shuffle(state)
            
            # Upewniamy się, że puste pole (0) jest w prawym dolnym rogu
            zero_idx = state.index(0)
            state[zero_idx], state[-1] = state[-1], state[0]
            
            # Sprawdzamy, czy permutacja jest rozwiązywalna
            if self.is_solvable(state):
                return tuple(state)
            
    def is_solvable(self, state) -> bool:
        """Sprawdza, czy dana permutacja jest rozwiązywalna."""
        # Zliczamy inwersje
        inversions = 0
        for i in range(len(state)):
            if state[i] == 0:
                continue
            for j in range(i+1, len(state)):
                if state[j] != 0 and state[i] > state[j]:
                    inversions += 1
        
        # Sprawdzamy warunek rozwiązywalności
        # Dla planszy 4x4 z pustym polem w prawym dolnym rogu
        # (pozycja (-1) - wiersz 4), liczba inwersji musi być parzysta
        return inversions % 2 == 0
    
    def get_blank_position(self, state) -> Tuple[int, int]:
        """Zwraca pozycję (wiersz, kolumna) pustego pola."""
        idx = state.index(0)
        return idx // self.size, idx % self.size
    
    def get_neighbors(self, state) -> List[tuple]:
        """Zwraca wszystkie możliwe następne stany."""
        neighbors = []
        row, col = self.get_blank_position(state)
        
        # Możliwe ruchy: góra, dół, lewo, prawo
        directions = [(-1, 0), (1, 0), (0, -1), (0, 1)]
        
        for dr, dc in directions:
            new_row, new_col = row + dr, col + dc
            
            # Sprawdzamy, czy nowa pozycja jest na planszy
            if 0 <= new_row < self.size and 0 <= new_col < self.size:
                # Tworzymy nowy stan
                new_state = list(state)
                old_idx = row * self.size + col
                new_idx = new_row * self.size + new_col
                
                # Zamieniamy puste pole z kafelkiem
                new_state[old_idx], new_state[new_idx] = new_state[new_idx], new_state[old_idx]
                
                neighbors.append(tuple(new_state))
        
        return neighbors
    
    def misplaced_tiles(self, state) -> int:
        """Heurystyka 1: Liczba kafelków nie na swoim miejscu."""
        count = 0
        for i in range(len(state)):
            if state[i] != 0 and state[i] != self.goal_state[i]:
                count += 1
        return count
    
    def manhattan_distance(self, state) -> int:
        """Heurystyka 2: Suma odległości Manhattan każdego kafelka od jego docelowej pozycji."""
        distance = 0
        for i in range(len(state)):
            if state[i] != 0:  # Pomijamy puste pole
                # Pozycja aktualna
                curr_row, curr_col = i // self.size, i % self.size
                
                # Pozycja docelowa
                goal_idx = self.goal_state.index(state[i])
                goal_row, goal_col = goal_idx // self.size, goal_idx % self.size
                
                # Dodajemy odległość Manhattan
                distance += abs(curr_row - goal_row) + abs(curr_col - goal_col)
        
        return distance
    
    def solve(self, initial_state, heuristic_func) -> Tuple[List[tuple], int, int]:
        """
        Rozwiązuje układankę używając algorytmu A*.
        
        Args:
            initial_state: Stan początkowy układanki
            heuristic_func: Funkcja heurystyki do użycia
            
        Returns:
            Tuple[List[tuple], int, int]: Ścieżka rozwiązania, liczba kroków, liczba odwiedzonych stanów
        """
        self.visited_states = 0
        
        # Kolejka priorytetowa dla A*
        open_set = []
        
        # Słownik przechowujący najniższy koszt dla każdego stanu
        g_score = {initial_state: 0}
        
        # Słownik przechowujący poprzedni stan dla każdego odwiedzonego stanu
        came_from = {}
        
        # Ocena F = G + H dla początkowego stanu
        f_score = g_score[initial_state] + heuristic_func(initial_state)
        
        # Dodajemy stan początkowy do kolejki (priorytet, licznik, stan)
        count = 0  # Licznik dla stabilnego sortowania przy równych priorytetach
        heapq.heappush(open_set, (f_score, count, initial_state))
        
        # Zbioró stanów już odwiedzonych
        closed_set = set()
        
        while open_set:
            # Pobieramy stan o najniższej ocenie F
            _, _, current = heapq.heappop(open_set)
            
            # Inkrementujemy licznik odwiedzonych stanów
            self.visited_states += 1
            
            # Jeśli znaleźliśmy rozwiązanie
            if current == self.goal_state:
                # Odtwarzamy ścieżkę
                path = []
                while current in came_from:
                    path.append(current)
                    current = came_from[current]
                path.append(initial_state)
                path.reverse()
                
                return path, len(path) - 1, self.visited_states
            
            # Dodajemy bieżący stan do zamkniętego zbioru
            closed_set.add(current)
            
            # Sprawdzamy wszystkie sąsiednie stany
            for neighbor in self.get_neighbors(current):
                # Pomijamy stany już sprawdzone
                if neighbor in closed_set:
                    continue
                
                # Obliczamy nowy koszt G
                tentative_g_score = g_score[current] + 1
                
                # Jeśli sąsiedni stan nie był jeszcze odwiedzony lub znaleźliśmy lepszą ścieżkę
                if neighbor not in g_score or tentative_g_score < g_score[neighbor]:
                    # Aktualizujemy ścieżkę
                    came_from[neighbor] = current
                    g_score[neighbor] = tentative_g_score
                    
                    # Obliczamy nowy koszt F
                    f_score = tentative_g_score + heuristic_func(neighbor)
                    
                    # Dodajemy sąsiedni stan do kolejki
                    count += 1
                    heapq.heappush(open_set, (f_score, count, neighbor))
        
        # Jeśli nie znaleziono rozwiązania
        return None, 0, self.visited_states
    
    def print_state(self, state):
        """Wyświetla stan układanki w formie macierzy."""
        for i in range(self.size):
            row = state[i*self.size:(i+1)*self.size]
            print(" ".join(f"{n:2d}" if n != 0 else "  " for n in row))
        print()
    
    def get_move_sequence(self, path) -> List[int]:
        """
        Zwraca sekwencję kafelków, które należy przesunąć.
        Dla każdego kroku określa numer kafelka, który został przesunięty na puste pole.
        """
        moves = []
        for i in range(1, len(path)):
            prev_state = path[i-1]
            curr_state = path[i]
            
            # Znajdujemy indeks pustego pola w poprzednim stanie
            blank_idx = prev_state.index(0)
            
            # Znajdujemy, który kafelek został przesunięty na puste pole
            diff_idx = None
            for j in range(len(prev_state)):
                if j != blank_idx and prev_state[j] != curr_state[j] and curr_state[j] == 0:
                    diff_idx = j
                    break
            
            if diff_idx is not None:
                # Dodajemy numer przesuniętego kafelka
                moves.append(prev_state[diff_idx])
        
        return moves

    def run_experiments(self, num_trials=50):
        """
        Przeprowadza eksperymenty porównujące różne heurystyki.
        
        Args:
            num_trials: Liczba prób dla każdej heurystyki
        """
        results = {
            "misplaced": {"steps": [], "visited": [], "time": []},
            "manhattan": {"steps": [], "visited": [], "time": []}
        }
        
        for i in range(num_trials):
            print(f"Próba {i+1}/{num_trials}")
            
            # Generujemy losowy stan początkowy
            initial_state = self.generate_random_solvable_state()
            
            # Rozwiązujemy używając heurystyki "misplaced tiles"
            start_time = time.time()
            path, steps, visited = self.solve(initial_state, self.misplaced_tiles)
            end_time = time.time()
            
            if path:
                results["misplaced"]["steps"].append(steps)
                results["misplaced"]["visited"].append(visited)
                results["misplaced"]["time"].append(end_time - start_time)
            
            # Rozwiązujemy używając heurystyki "manhattan distance"
            start_time = time.time()
            path, steps, visited = self.solve(initial_state, self.manhattan_distance)
            end_time = time.time()
            
            if path:
                results["manhattan"]["steps"].append(steps)
                results["manhattan"]["visited"].append(visited)
                results["manhattan"]["time"].append(end_time - start_time)
        
        # Podsumowanie wyników
        report = "=== Raport z eksperymentów ===\n\n"
        
        report += "Heurystyka: Liczba kafelków nie na swoim miejscu\n"
        report += f"- Średnia liczba kroków: {np.mean(results['misplaced']['steps']):.2f}\n"
        report += f"- Średnia liczba odwiedzonych stanów: {np.mean(results['misplaced']['visited']):.2f}\n"
        report += f"- Średni czas rozwiązania: {np.mean(results['misplaced']['time']):.4f} s\n\n"
        
        report += "Heurystyka: Odległość Manhattan\n"
        report += f"- Średnia liczba kroków: {np.mean(results['manhattan']['steps']):.2f}\n"
        report += f"- Średnia liczba odwiedzonych stanów: {np.mean(results['manhattan']['visited']):.2f}\n"
        report += f"- Średni czas rozwiązania: {np.mean(results['manhattan']['time']):.4f} s\n"
        
        return report

def main():
    puzzle = FifteenPuzzle()
    
    # Generujemy losowy stan początkowy (z pustym polem w prawym dolnym rogu)
    initial_state = puzzle.generate_random_solvable_state()
    
    print("Stan początkowy:")
    puzzle.print_state(initial_state)
    
    # Rozwiązujemy używając heurystyki "misplaced tiles"
    print("\nRozwiązywanie używając heurystyki 'misplaced tiles'...")
    start_time = time.time()
    path_misplaced, steps_misplaced, visited_misplaced = puzzle.solve(initial_state, puzzle.misplaced_tiles)
    end_time = time.time()
    
    if path_misplaced:
        moves = puzzle.get_move_sequence(path_misplaced)
        print(f"Rozwiązanie znalezione w {steps_misplaced} krokach!")
        print(f"Odwiedzone stany: {visited_misplaced}")
        print(f"Czas rozwiązania: {end_time - start_time:.4f} s")
        print("Sekwencja ruchów (kafelki do przesunięcia):", moves)
    else:
        print("Nie znaleziono rozwiązania!")
    
    # Rozwiązujemy używając heurystyki "manhattan distance"
    print("\nRozwiązywanie używając heurystyki 'manhattan distance'...")
    start_time = time.time()
    path_manhattan, steps_manhattan, visited_manhattan = puzzle.solve(initial_state, puzzle.manhattan_distance)
    end_time = time.time()
    
    if path_manhattan:
        moves = puzzle.get_move_sequence(path_manhattan)
        print(f"Rozwiązanie znalezione w {steps_manhattan} krokach!")
        print(f"Odwiedzone stany: {visited_manhattan}")
        print(f"Czas rozwiązania: {end_time - start_time:.4f} s")
        print("Sekwencja ruchów (kafelki do przesunięcia):", moves)
    else:
        print("Nie znaleziono rozwiązania!")
    
    # Uruchomienie eksperymentów dla pełnego raportu
    print("\nCzy chcesz przeprowadzić pełne eksperymenty? [T/n]")
    choice = input().lower()
    if choice != 'n':
        print("\nUruchamianie eksperymentów (to może zająć kilka minut)...")
        print("Używamy mniejszej liczby prób (10) dla demonstracji.")
        report = puzzle.run_experiments(num_trials=10)
        print("\n" + report)

if __name__ == "__main__":
    main()