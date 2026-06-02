#!/usr/bin/env python3
import argparse
import csv
from pathlib import Path

import matplotlib.pyplot as plt


def load_points(csv_path: Path):
    x = []
    y = []

    with csv_path.open("r", newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            x.append(int(row["speed_param"]))
            y.append(float(row["cm_s"]))

    return x, y


def main():
    parser = argparse.ArgumentParser(
        description="Rysuje wykres predkosci [cm/s] od parametru Wheels::setSpeed()."
    )
    parser.add_argument("csv", type=Path, help="Plik CSV z kolumnami speed_param,cm_s")
    parser.add_argument(
        "-o",
        "--output",
        type=Path,
        default=Path("speed_curve.png")
        help="Plik wynikowy PNG",
    )
    args = parser.parse_args()

    x, y = load_points(args.csv)

    if not x:
        raise SystemExit("Brak danych w pliku CSV")

    plt.figure(figsize=(9, 5))
    plt.plot(x, y, marker="o", linewidth=2)
    plt.title("Zaleznosc predkosci od Wheels::setSpeed")
    plt.xlabel("Parametr setSpeed")
    plt.ylabel("Predkosc [cm/s]")
    plt.grid(True, linestyle="--", alpha=0.5)
    plt.tight_layout()
    plt.savefig(args.output, dpi=160)
    print(f"Zapisano wykres do: {args.output}")


if __name__ == "__main__":
    main()
