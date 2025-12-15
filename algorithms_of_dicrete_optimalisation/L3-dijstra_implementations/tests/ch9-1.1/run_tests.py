import csv
import glob
import os
import subprocess

# --- KONFIGURACJA ---
# Ścieżki do plików wykonywalnych (względem tego skryptu)
# Zakładam, że są one dwa katalogi wyżej, tak jak w poprzednich krokach
EXECUTABLES = {
    "Dijkstra": "../../dijkstra",
    "Dial": "../../dial",
    "RadixHeap": "../../radixheap",
}

INPUT_DIR = "inputs"
RESULTS_FILE = "results.csv"
TIMEOUT_SEC = 300  # Maksymalny czas na jeden test (5 minut)

# Rodziny do przetestowania [cite: 122-126]
FAMILIES = [
    "Random4-n",
    "Random4-C",
    "Long-n",
    "Long-C",
    "Square-n",
    "Square-C",
    "USA-road-d",
]


def parse_time(output_file):
    """Parsuje plik wynikowy .res w poszukiwaniu linii 't <czas>'."""
    try:
        with open(output_file, "r") as f:
            for line in f:
                if line.startswith("t "):
                    return float(line.strip().split()[1])
    except Exception as e:
        print(f"  Błąd odczytu wyniku: {e}")
    return None


def get_params_from_filename(filename):
    """Wyciąga parametry n i k z nazwy pliku np. Random4-n.10.0.gr -> n=10."""
    base = os.path.basename(filename)
    parts = base.split(".")
    # Format zazwyczaj: Family.param1.param2.gr
    if len(parts) >= 3:
        return parts[1]  # To jest zazwyczaj wykładnik potęgi 2
    return "0"


def run_single_test(algo_name, exe_path, graph_file, ss_file, out_file):
    cmd = [exe_path, "-d", graph_file, "-ss", ss_file, "-oss", out_file]

    # Dla Diala omijamy grafy z dużymi wagami (C), bo może braknąć pamięci/czasu
    #  Dial ma złożoność zależną od C
    if algo_name == "Dial" and "C" in graph_file and "Long" not in graph_file:
        # Sprawdźmy czy C jest duże (powyżej 2^10 może być wolno/dużo pamięci)
        try:
            c_param = int(get_params_from_filename(graph_file))
            if c_param > 8:  # Arbitralny limit dla bezpieczeństwa
                return None
        except:
            pass

    try:
        # Uruchomienie procesu
        subprocess.run(
            cmd,
            check=True,
            timeout=TIMEOUT_SEC,
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
        return parse_time(out_file)
    except subprocess.TimeoutExpired:
        print(f"  [TIMEOUT] {algo_name} przekroczył {TIMEOUT_SEC}s.")
        return "TIMEOUT"
    except subprocess.CalledProcessError:
        print(f"  [ERROR] {algo_name} zwrócił błąd.")
        return "ERROR"
    except FileNotFoundError:
        print(f"  [ERROR] Nie znaleziono pliku wykonywalnego: {exe_path}")
        return "MISSING_EXE"


def main():
    # Przygotuj plik CSV
    with open(RESULTS_FILE, "w", newline="") as csvfile:
        fieldnames = ["Family", "Param", "Algorithm", "Time_ms"]
        writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
        writer.writeheader()

        for family in FAMILIES:
            print(f"\n--- Testowanie rodziny: {family} ---")
            family_dir = os.path.join(INPUT_DIR, family)

            if not os.path.exists(family_dir):
                print(f"Katalog {family_dir} nie istnieje. Pomijam.")
                continue

            # Znajdź wszystkie pliki .gr
            graph_files = sorted(glob.glob(os.path.join(family_dir, "*.gr")))

            for graph_file in graph_files:
                # Znajdź odpowiadający plik .ss
                base_name = os.path.splitext(graph_file)[0]
                ss_file = base_name + ".ss"

                if not os.path.exists(ss_file):
                    print(f"Brak pliku źródeł dla {graph_file}. Pomijam.")
                    continue

                param = get_params_from_filename(graph_file)
                print(f"Processing {os.path.basename(graph_file)} (Param: {param})...")

                for algo, exe in EXECUTABLES.items():
                    out_res = f"temp_{algo}.res"
                    time_val = run_single_test(algo, exe, graph_file, ss_file, out_res)

                    if time_val is not None and isinstance(time_val, float):
                        print(f"  {algo}: {time_val} ms")
                        writer.writerow(
                            {
                                "Family": family,
                                "Param": param,
                                "Algorithm": algo,
                                "Time_ms": time_val,
                            }
                        )

                    # Sprzątanie
                    if os.path.exists(out_res):
                        os.remove(out_res)

    print(f"\nTesty zakończone. Wyniki w {RESULTS_FILE}")


if __name__ == "__main__":
    main()
