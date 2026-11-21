#!/usr/bin/env python3
"""Figures for the report (list 3).

Usage: python3 plots.py [output/all.txt]
Parses the tables printed by zad*.jl ('# title' line, CSV header, rows, blank line)
and writes fig/zad3, zad4, zad5, zad6 as .pdf and .png.
"""
import os
import sys

import numpy as np
import matplotlib

matplotlib.use("Agg")
import matplotlib.pyplot as plt

plt.rcParams.update({
    "font.size": 8, "axes.titlesize": 8, "legend.fontsize": 7,
    "axes.grid": True, "grid.alpha": 0.3, "mathtext.default": "regular",
})

DELTA3 = 1e-4  # tolerance from the task text (zad. 3), not present in the output


def parse(path):
    """Return {table title: {column: np.ndarray}}; numeric columns as float, others as object."""
    tables = {}
    state = {"title": None, "header": None, "rows": []}

    def flush():
        title, header, rows = state["title"], state["header"], state["rows"]
        if title is None or header is None:
            return
        for r in rows:
            if len(r) != len(header):
                raise ValueError(f"table '{title}': row has {len(r)} fields, header {len(header)}")
        cols = {}
        for j, name in enumerate(header):
            vals = [r[j] for r in rows]
            try:
                cols[name] = np.array(vals, dtype=float)
            except ValueError:
                cols[name] = np.array(vals, dtype=object)
        tables[title] = cols

    with open(path, encoding="utf-8") as fh:
        for line in fh:
            line = line.rstrip("\r\n")
            if line.startswith("#"):
                flush()
                state.update(title=line[1:].strip(), header=None, rows=[])
            elif not line.strip():
                flush()
                state.update(title=None, header=None, rows=[])
            elif state["title"] is not None:
                if state["header"] is None:
                    state["header"] = line.split(",")
                else:
                    state["rows"].append(line.split(","))
    flush()
    return tables


def logsafe(a):
    """|a| with zeros replaced by NaN (for log axes)."""
    a = np.abs(np.asarray(a, dtype=float))
    return np.where(a > 0, a, np.nan)


def pick(tables, prefix):
    """Table whose title starts with prefix and has the smallest tol=... suffix."""
    keys = [k for k in tables if k.startswith(prefix)]
    if not keys:
        raise KeyError(f"no table with prefix '{prefix}'")
    return tables[min(keys, key=lambda k: float(k.split("=")[1]))]


def save(fig, name):
    os.makedirs("fig", exist_ok=True)
    fig.savefig(f"fig/{name}.pdf", bbox_inches="tight")
    fig.savefig(f"fig/{name}.png", dpi=200, bbox_inches="tight")
    plt.close(fig)


def fig_zad3(t):
    b, n = t["zad3_bisection"], t["zad3_newton"]
    fig, (a1, a2) = plt.subplots(1, 2, figsize=(6.4, 2.5), gridspec_kw={"width_ratios": [1.3, 1]})
    x = np.linspace(-0.2, 2.2, 300)
    a1.plot(x, np.exp(x), label="$y=e^x$")
    a1.plot(x, 3 * x, label="$y=3x$")
    a1.plot(b["r"], b["y"], "o", mfc="none", ms=8, label="bisekcja")
    a1.plot(n["r"], n["y"], "x", ms=6, label="Newton")
    a1.set_xlabel("$x$")
    a1.set_ylabel("$y$")
    a1.set_ylim(-0.5, 9)
    a1.legend(loc="upper left")
    a1.set_title("(a) punkty przecięcia")

    k = np.arange(len(b["root"]))
    for off, d, lab, mk in ((-0.12, b, "bisekcja", "o"), (0.12, n, "Newton", "x")):
        a2.plot(k + off, d["abs_err_r"], mk, mfc="none", label=lab)
        for xi, e, i in zip(k + off, d["abs_err_r"], d["it"]):
            a2.annotate(f"it={int(i)}", (xi, e), textcoords="offset points", xytext=(0, 5),
                        ha="center", fontsize=6)
    a2.axhline(DELTA3, ls="--", color="k", lw=0.8, label=r"$\delta=10^{-4}$")
    a2.set_yscale("log")
    a2.set_xticks(k)
    a2.set_xticklabels([f"pierwiastek {int(r)}" for r in b["root"]])
    a2.set_xlim(-0.5, len(k) - 0.5)
    a2.set_ylabel(r"$|\tilde r-r_{ref}|$")
    a2.set_title("(b) błąd i liczba iteracji")
    a2.set_ylim(1e-6, 4e-4)
    a2.legend(loc="lower left")
    fig.tight_layout()
    save(fig, "zad3")


def scan_panel(ax, d, rref, ylab, title, symlog):
    pos = np.arange(len(d["x0"]))
    ok = d["err"] == 0
    ax.plot(pos[ok], d["r"][ok], "o", ms=4, label="err=0")
    ax.plot(pos[~ok], d["r"][~ok], "x", ms=6, color="C3", label="err=2")
    ax.axhline(rref, ls=":", color="gray", lw=0.9)
    for p, r, i in zip(pos, d["r"], d["it"]):
        ax.annotate(f"{int(i)}", (p, r), textcoords="offset points", xytext=(0, 4),
                    ha="center", fontsize=6)
    ax.set_xticks(pos)
    ax.set_xticklabels([f"{v:g}" for v in d["x0"]])
    if symlog:
        ax.set_yscale("symlog", linthresh=10)
        ax.set_yticks([-1e16, -1e12, -1e8, -1e4, 0])
        ax.set_yticklabels(["$-10^{16}$", "$-10^{12}$", "$-10^{8}$", "$-10^{4}$", "0"])
        ax.set_ylim(top=100)
        ax.minorticks_off()
    ax.set_xlabel("$x_0$")
    ax.set_ylabel(ylab)
    ax.set_title(title)
    ax.legend(loc="upper left")


def fig_zad4(t):
    pd, tr, main = t["zad4_plotdata"], t["zad4_trace"], t["zad4_main"]
    s1, s2 = t["zad4a_newton_f1"], t["zad4b_newton_f2"]
    root = {f: main["r_ref"][main["func"] == f][0] for f in ("f1", "f2")}
    fig, ax = plt.subplots(2, 2, figsize=(6.4, 4.7))

    a = ax[0, 0]
    a.plot(pd["x"], pd["f1"], label="$f_1=e^{1-x}-1$")
    a.plot(pd["x"], pd["f2"], label="$f_2=xe^{-x}$")
    a.axhline(0, color="k", lw=0.6)
    a.plot([root["f1"], root["f2"]], [0, 0], "ko", ms=3)
    v1 = s2["v"][s2["x0"] == 1.0][0]
    a.plot([1.0], [v1], "s", color="C3", ms=4)
    a.annotate("$x_0=1$, err=2", (1.0, v1), textcoords="offset points", xytext=(6, 4), fontsize=6)
    a.set_xlim(-1, 6)
    a.set_ylim(-1.2, 1.2)
    a.set_xlabel("$x$")
    a.set_ylabel("$f(x)$")
    a.set_title("(a) funkcje i miejsca zerowe")
    a.legend(loc="upper right")

    a = ax[0, 1]
    for name, c, lab in (("f1", "C0", "$f_1$"), ("f2", "C1", "$f_2$")):
        m = tr["func"] == name
        x0 = tr["x0"][m][0]
        a.plot(tr["n"][m], tr["x"][m], "o-", ms=3, color=c, label=f"{lab}, $x_0={x0:g}$")
        a.axhline(root[name], ls=":", color=c, lw=0.9)
    a.set_xlabel("$n$")
    a.set_ylabel("$x_n$")
    a.set_title("(b) iteraty Newtona")
    a.legend(loc="center right")

    scan_panel(ax[1, 0], s1, root["f1"], "wynik $r$ (symlog)",
               "(c) $f_1$: wynik wg $x_0$ (liczby: it)", True)
    scan_panel(ax[1, 1], s2, root["f2"], "wynik $r$",
               "(d) $f_2$: wynik wg $x_0$ (liczby: it)", False)
    fig.tight_layout()
    save(fig, "zad4")


def fig_zad5(t):
    s = t["zad5b_summary"]
    methods = [("bisection", "bisekcja", "ratio_p1", r"$|e_{n}|/|e_{n-1}|$"),
               ("secant", "sieczne", "ratio_phi", r"$|e_{n}|/|e_{n-1}|^{\varphi}$"),
               ("newton", "Newton", "ratio_p2", r"$|e_{n}|/|e_{n-1}|^{2}$")]
    fig = plt.figure(figsize=(6.4, 4.5))
    gs = fig.add_gridspec(2, 3)
    top = fig.add_subplot(gs[0, :])
    for i, (key, lab, rc, ylab) in enumerate(methods):
        d = pick(t, f"zad5b_{key}_tol=")
        tol = min(s["tol"][s["method"] == key])
        top.semilogy(d["n"], logsafe(d["e"]), "o-", ms=3, label=lab, color=f"C{i}")
        b = fig.add_subplot(gs[1, i])
        good = np.abs(d["e"]) >= 1e-15
        b.plot(d["n"], d[rc], "-", lw=0.5, color=f"C{i}")
        b.plot(d["n"][good], d[rc][good], "o", ms=3, color=f"C{i}")
        b.plot(d["n"][~good], d[rc][~good], "o", ms=3, mfc="none", color="gray")
        const = s["theory_const"][(s["method"] == key) & np.isclose(s["tol"], tol)][0]
        b.axhline(const, ls="--", color="k", lw=0.8)
        if key != "newton":
            b.set_yscale("log")
        b.set_xlabel("$n$")
        b.set_ylabel(ylab)
        b.set_title(f"({'bcd'[i]}) {lab}")
    top.set_xlabel("$n$")
    top.set_ylabel(r"$|e_n|=|x_n-r|$")
    top.set_title("(a) błąd kolejnych przybliżeń")
    top.legend()
    fig.tight_layout()
    save(fig, "zad5")


def fig_zad6(t):
    c, m, s = t["zad6a_newton_classic"], t["zad6b_newton_modified"], t["zad6_summary"]
    c_cl = s["theory_const"][s["method"] == "classic"][0]
    c_mod = s["theory_const"][s["method"] == "modified"][0]
    fig, ax = plt.subplots(1, 3, figsize=(6.4, 2.4))
    ax[0].semilogy(c["n"], logsafe(c["e"]), "o-", ms=2.5, label="klasyczna")
    ax[0].semilogy(m["n"], logsafe(m["e"]), "s-", ms=3, label="zmodyfikowana")
    ax[0].set_xlabel("$n$")
    ax[0].set_ylabel("$|e_n|$")
    ax[0].set_title("(a) błąd")
    ax[0].legend()
    ax[1].plot(c["n"], c["ratio_p1"], "o-", ms=2.5)
    ax[1].axhline(c_cl, ls="--", color="k", lw=0.8)
    ax[1].set_ylim(0.55, 0.7)
    ax[1].set_xlabel("$n$")
    ax[1].set_ylabel("$|e_n|/|e_{n-1}|$")
    ax[1].set_title("(b) klasyczna")
    ax[2].semilogy(m["n"], m["ratio_p2"], "s-", ms=3, label="zmodyfikowana")
    ax[2].semilogy(c["n"], c["ratio_p2"], "o-", ms=2.5, label="klasyczna")
    ax[2].axhline(c_mod, ls="--", color="k", lw=0.8)
    ax[2].set_xlabel("$n$")
    ax[2].set_ylabel("$|e_n|/|e_{n-1}|^2$")
    ax[2].set_title("(c) iloraz kwadratowy")
    ax[2].legend(loc="upper left")
    fig.tight_layout()
    save(fig, "zad6")


def main():
    path = sys.argv[1] if len(sys.argv) > 1 else "output/all.txt"
    t = parse(path)
    for fn in (fig_zad3, fig_zad4, fig_zad5, fig_zad6):
        fn(t)
    print("tables:", len(t), "figures: fig/zad3..zad6")


if __name__ == "__main__":
    main()