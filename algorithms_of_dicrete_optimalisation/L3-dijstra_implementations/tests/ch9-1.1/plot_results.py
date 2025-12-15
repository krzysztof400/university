import os

import matplotlib.pyplot as plt
import pandas as pd

RESULTS_FILE = "results.csv"
OUTPUT_DIR = "plots"


def generate_plots():
    if not os.path.exists(RESULTS_FILE):
        print(f"Brak pliku {RESULTS_FILE}. Uruchom najpierw run_tests.py.")
        return

    df = pd.read_csv(RESULTS_FILE)

    if not os.path.exists(OUTPUT_DIR):
        os.makedirs(OUTPUT_DIR)

    # Pobierz unikalne rodziny
    families = df["Family"].unique()

    for family in families:
        # Tworzymy kopię, aby uniknąć SettingWithCopyWarning
        subset = df[df["Family"] == family].copy()

        # Próba konwersji parametru na int
        try:
            subset["Param"] = subset["Param"].astype(int)
            subset = subset.sort_values("Param")
            is_numeric = True
        except ValueError:
            # Jeśli parametr jest tekstem (np. 'NY' dla USA-road), pomijamy wykres liniowy
            print(f"--- Rodzina {family}: Parametr nieliczbowy (np. 'NY').")
            print("    Pomięto generowanie wykresu (dane należy umieścić w tabeli).")
            continue

        plt.figure(figsize=(10, 6))

        algorithms = subset["Algorithm"].unique()

        for algo in algorithms:
            algo_data = subset[subset["Algorithm"] == algo]
            plt.plot(algo_data["Param"], algo_data["Time_ms"], marker="o", label=algo)

        plt.title(f"Czas działania vs Parametr dla {family}")
        plt.xlabel("Parametr (Wykładnik potęgi 2 dla N lub C)")
        plt.ylabel("Czas (ms)")
        plt.legend()
        plt.grid(True)
        plt.yscale("log")  # Skala logarytmiczna

        filename = os.path.join(OUTPUT_DIR, f"{family}_plot.png")
        plt.savefig(filename)
        print(f"Wygenerowano wykres: {filename}")
        plt.close()


if __name__ == "__main__":
    generate_plots()
