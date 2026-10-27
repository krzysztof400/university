#!/usr/bin/env python3
# Autor: Krzysztof Zając
# Obliczenia naukowe, Lista 1: wykresy do sprawozdania.
# Użycie: python3 plots.py [output/all.txt]  ->  fig/zad2.pdf, fig/zad5.pdf, fig/zad6.pdf
# Dane do zad. 5 i 6 są wczytywane z wydruku programów zad5.jl i zad6.jl.

import sys
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import numpy as np
from matplotlib.ticker import NullLocator

SRC = Path(sys.argv[1] if len(sys.argv) > 1 else "output/all.txt")
OUT = Path("fig")
OUT.mkdir(exist_ok=True)

plt.rcParams.update({"font.size": 8, "axes.grid": True, "grid.alpha": 0.3,
                     "legend.fontsize": 7, "savefig.bbox": "tight"})


def load(path):
    """Parsuje wydruk zad5.jl (7 kolumn) i zad6.jl (5 kolumn).
    Parametry: path - plik tekstowy z wydrukiem. Zwraca (z5, z6) jako tablice numpy."""
    z5, z6 = [], []
    for line in path.read_text(encoding="utf-8").splitlines():
        t = line.split()
        if not t or not t[0].isdigit():
            continue
        try:
            v = [float(s) for s in t]
        except ValueError:
            continue
        if len(v) == 7:
            z5.append(v)
        elif len(v) == 5:
            z6.append(v)
    return np.array(z5), np.array(z6)


def pos(a):
    """Wartości niedodatnie -> NaN (nie da się ich narysować w skali log)."""
    return np.where(a > 0, a, np.nan)


def save(fig, name):
    fig.savefig(OUT / f"{name}.pdf")
    fig.savefig(OUT / f"{name}.png", dpi=200)
    plt.close(fig)
    print("zapisano", OUT / f"{name}.pdf")


def fig_zad2():
    """Odstęp między sąsiednimi liczbami Float64 w funkcji x."""
    x = np.logspace(-2, 3, 4000, base=2)
    fig, ax = plt.subplots(figsize=(3.4, 2.5))
    ax.loglog(x, np.spacing(x))
    for e in range(-2, 3):
        ax.text(2.0 ** (e + 0.5), 2.0 ** (e - 52) * 1.5, f"$2^{{{e - 52}}}$", ha="center")
    ticks = [0.25, 0.5, 1, 2, 4, 8]
    ax.set_xticks(ticks)
    ax.set_xticklabels(["1/4", "1/2", "1", "2", "4", "8"])
    ax.xaxis.set_minor_locator(NullLocator())
    ax.yaxis.set_minor_locator(NullLocator())
    ax.set_xlabel("$x$")
    ax.set_ylabel("odstęp do następnej liczby")
    save(fig, "zad2")


def fig_zad5(z5):
    """Błąd względny f i g oraz stosunek f/g dla x = 8^-k, k = 1..30."""
    d = z5[z5[:, 0] <= 30]
    k, f, g, rf, rg = d[:, 0], d[:, 2], d[:, 3], d[:, 5], d[:, 6]
    fig, (a, b) = plt.subplots(1, 2, figsize=(7.2, 2.7))

    a.semilogy(k, pos(rf), "o-", ms=3, label="$\\delta_f$")
    a.semilogy(k, pos(rg), "s-", ms=3, label="$\\delta_g$")
    a.axhline(2.0 ** -53, color="k", ls=":", lw=0.8, label="$u=2^{-53}$")
    a.axvline(9, color="r", ls="--", lw=0.8)
    a.text(9.3, 1e-9, "$f=0$ od $k=9$", color="r")
    a.set_ylim(1e-20, 5)
    a.set_xlabel("$k$  ($x=8^{-k}$)")
    a.set_ylabel("błąd względny")
    a.legend(loc="upper left")

    b.plot(k, f / g, "o-", ms=3)
    b.axhline(1, color="k", ls=":", lw=0.8)
    b.set_xlabel("$k$  ($x=8^{-k}$)")
    b.set_ylabel("$f(x)\\,/\\,g(x)$")
    b.set_ylim(-0.1, 1.2)
    fig.tight_layout()
    save(fig, "zad5")


def fig_zad6(z6):
    """Błąd pochodnej różnicowej w funkcji h = 2^-n, n = 0..54."""
    n, err = z6[:, 0], z6[:, 3]
    h = 2.0 ** (-n)
    c = abs(-np.sin(1.0) - 9 * np.cos(3.0)) / 2          # |f''(1)|/2
    hh = 2.0 ** (-np.linspace(0, 54, 400))
    i = int(np.nanargmin(err))

    fig, ax = plt.subplots(figsize=(5.2, 3.0))
    ax.loglog(h, pos(err), "o-", ms=3, label="$|f'(x_0)-\\tilde f'(x_0)|$")
    ax.loglog(hh, c * hh, "--", lw=0.9, label=f"obcięcie: ${c:.2f}\\,h$".replace(".", "{,}"))
    ax.loglog(hh, 1e-16 / hh, ":", lw=1.2, label="zaokrąglenia: $10^{-16}/h$")
    ax.plot(h[i], err[i], "r*", ms=10, label=f"minimum: $n={int(n[i])}$")
    ax.axvline(2.0 ** -53, color="gray", ls="--", lw=0.8)
    ax.text(2.0 ** -53 * 1.3, 2e-9, "$1+h=1$", rotation=90, color="gray", va="bottom")
    ax.invert_xaxis()
    ax.set_xlabel("$h=2^{-n}$")
    ax.set_ylabel("błąd bezwzględny")
    ax.legend(loc="upper center")
    save(fig, "zad6")


if __name__ == "__main__":
    if not SRC.exists():
        sys.exit(f"brak pliku {SRC} - najpierw uruchom ./run.sh")
    z5, z6 = load(SRC)
    if len(z5) == 0 or len(z6) == 0:
        sys.exit("nie znaleziono tabel zad5/zad6 w pliku wejściowym")
    fig_zad2()
    fig_zad5(z5)
    fig_zad6(z6)