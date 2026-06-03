import matplotlib.pyplot as plt
import numpy as np

# 1. Podstawowe parametry krzywej
k = 16  # Liczba płatków
a = 0.5  # Promień rdzenia
b = 4.0  # Długość płatka

# 2. Parametry kształtu
potega = 0.1  # Kształt płatka (bardzo niska wartość daje szerokie, płaskie płatki)

# 3. Druga warstwa
rysuj_tlo = True
skala_tla = 1.0
przesuniecie_tla = np.pi / k

# --- KOLORY ---
czerwien_tla = "#bc002d"  # Czerwień japońskiej flagi
brazowe_zloto_kolo = "#a67b27"  # Tło pod kwiatem (brązowo-złotawy)
zloto_tlo = "#d4af37"  # Wypełnienie dolnych płatków
zloto_glowny = "#ffd700"  # Wypełnienie głównych płatków i rdzenia

# ==========================================
# RYSOWANIE
# ==========================================

# --- NAPRAWA "NIEDOCIĄGNIĘTYCH" LINII ---
# Tworzymy bazową siatkę
theta_base = np.linspace(0, 2 * np.pi, 3000)

# Wyliczamy dokładne matematyczne dołki (minima) dla płatków głównych i tła
wciecia_glowne = np.array([(np.pi + 2 * i * np.pi) / k for i in range(k)])
wciecia_tla = wciecia_glowne - przesuniecie_tla

# Doklejamy te idealne punkty do siatki i sortujemy, by linia rysowała się po kolei
theta = np.unique(np.sort(np.concatenate([theta_base, wciecia_glowne, wciecia_tla])))
# ----------------------------------------


def promien(t):
    # Sprowadzamy cosinus z [-1, 1] do przedziału [0, 1]
    baza = (np.cos(k * t) + 1.0) / 2.0
    # Podnoszenie do potęgi ułamkowej poszerza płatki
    return a + b * (baza**potega)


fig, ax = plt.subplots(subplot_kw={"projection": "polar"}, figsize=(8, 8))

# Koło w brązowo-złotawym odcieniu, na którym leży cała chryzantema
max_promien = a + b
ax.fill(theta, [max_promien] * len(theta), color=zloto_tlo, zorder=0)

# Rysowanie tła (warstwa pod spodem)
if rysuj_tlo:
    r_tlo = promien(theta) * skala_tla
    theta_tlo = theta + przesuniecie_tla
    ax.fill(theta_tlo, r_tlo, color=zloto_tlo, alpha=1.0, zorder=1)
    ax.plot(
        theta_tlo,
        r_tlo,
        color=czerwien_tla,
        linewidth=4.5,
        solid_joinstyle="round",
        zorder=2,
    )

# Rysowanie głównej warstwy (wyżej)
r_glowna = promien(theta)
ax.fill(theta, r_glowna, color=zloto_tlo, alpha=1.0, zorder=3)
ax.plot(
    theta,
    r_glowna,
    color=czerwien_tla,
    linewidth=5.5,
    solid_joinstyle="round",
    zorder=4,
)

# Środek chryzantemy (wyraźne kółko - najwyższy zorder na samym wierzchu)
ax.fill(theta, [a] * len(theta), color=zloto_tlo, alpha=1.0, zorder=5)
ax.plot(
    theta,
    [a] * len(theta),
    color=czerwien_tla,
    linewidth=4.5,
    solid_joinstyle="round",
    zorder=6,
)

# Estetyka wykresu
ax.set_yticklabels([])
ax.set_xticklabels([])
ax.grid(False)
ax.spines["polar"].set_visible(False)

# Kolor tła płótna i osi (czerwień japońskiej flagi)
fig.patch.set_facecolor(czerwien_tla)
ax.set_facecolor(czerwien_tla)

plt.show()
