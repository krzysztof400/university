import json
import pulp
from pulp import LpProblem, LpVariable, LpMaximize, LpMinimize, lpSum, LpStatus, value
import sys

def load_data(filename):
    with open(filename, 'r', encoding='utf-8') as f:
        data = json.load(f)
    return data

def solve_and_save(model, solver_name="CBC", lp_filename="model.lp", solver_msg=True):
    model.writeLP(lp_filename)

    if solver_name.upper() == "GLPK":
        solver = pulp.GLPK_CMD(msg=solver_msg)
    elif solver_name.upper() == "CBC":
        solver = pulp.PULP_CBC_CMD(msg=solver_msg)
    else:
        solver = None

    print(f"--- Rozwiązywanie modelu {model.name} ---")
    model.solve(solver)

    print("Status:", LpStatus[model.status])
    print("\nZmienne:")
    for v in model.variables():
        if v.varValue > 1e-6:
            print(v.name, "=", v.varValue)
    
    print("\nWynik (funkcja celu):")
    print(f"{model.objective.name} = {value(model.objective)}")
    print(f"--- Koniec modelu {model.name} ---")

def build_model_task1(data):
    model = LpProblem(name="Minimalizacja_kosztow_paliwa", sense=LpMinimize)

    dostawcy = data["dostawcy"]
    lotniska = data["lotniska"]
    koszty = data["koszty"]
    podaż = data["podaż"]
    popyt = data["popyt"]

    zmienne = LpVariable.dicts("Paliwo", 
                               [(d, l) for d in dostawcy for l in lotniska], 
                               lowBound=0, 
                               cat='Continuous')

    obj = lpSum(zmienne[(d, l)] * koszty[l][d] 
                for d in dostawcy for l in lotniska)
    model += obj, "Calkowity_koszt_dostaw"

    for d in dostawcy:
        model += lpSum(zmienne[(d, l)] for l in lotniska) <= podaż[d], f"Podaż_dostawcy_{d}"

    for l in lotniska:
        model += lpSum(zmienne[(d, l)] for d in dostawcy) == popyt[l], f"Popyt_lotniska_{l}"

    return model, zmienne

def build_model_task2(data):
    model = LpProblem(name="Maksymalizacja_zysku_produkcji", sense=LpMaximize)
    
    produkty = data["products"]
    maszyny = data["machines"]
    czas_produkcji = data["production_time"]

    nazwy_produktow = list(produkty.keys())
    nazwy_maszyn = list(maszyny.keys())

    vars = {}
    for p_nazwa, p_info in produkty.items():
        vars[p_nazwa] = LpVariable(name=p_nazwa, 
                                   lowBound=p_info.get("lb", 0), 
                                   upBound=p_info.get("ub", None), 
                                   cat='Continuous')

    czas_pracy_maszyn = {m_nazwa: 0 for m_nazwa in nazwy_maszyn}
    for m_idx, m_nazwa in enumerate(nazwy_maszyn):
        czas_pracy_maszyn[m_nazwa] = lpSum(
            vars[p_nazwa] * czas_produkcji[p_idx][m_idx] 
            for p_idx, p_nazwa in enumerate(nazwy_produktow)
        )

    zysk_jednostkowy = {p: p_info["price"] - p_info["materials_cost"] 
                        for p, p_info in produkty.items()}

    calkowity_koszt_maszyn = lpSum(
        (maszyny[m_nazwa]["running_cost"] / 60.0) * czas_pracy_maszyn[m_nazwa] 
        for m_nazwa in nazwy_maszyn
    )

    obj = lpSum(zysk_jednostkowy[p] * vars[p] for p in nazwy_produktow) - calkowity_koszt_maszyn
    model += obj, "Calkowity_Zysk"

    for m_nazwa, m_info in maszyny.items():
        model += czas_pracy_maszyn[m_nazwa] <= m_info["availability_time"], f"Czas_pracy_maszyny_{m_nazwa}"

    return model, vars

def build_model_task3(data):
    model = LpProblem(name="Minimalizacja_kosztow_produkcji_i_magazynowania", sense=LpMinimize)

    okresy = data["okresy"]
    dane_okresowe = data["dane_okresowe"]
    
    poj_magazynu = data["pojemnosc_magazynu"]
    koszt_magazynowania = data["koszt_magazynowania_jednostki"]
    magazyn_pocz = data["magazyn_poczatkowy"]
    poj_prod_normalna = data["pojemnosc_produkcji_normalnej"]

    prod_normalna = LpVariable.dicts("Produkcja_normalna", okresy, lowBound=0, cat='Continuous')
    prod_nadwym = LpVariable.dicts("Produkcja_nadwymiarowa", okresy, lowBound=0, cat='Continuous')
    magazyn = LpVariable.dicts("Magazyn", [0] + okresy, lowBound=0, cat='Continuous')

    model += magazyn[0] == magazyn_pocz, "Magazyn_poczatek"

    koszt_prod = lpSum(
        prod_normalna[j] * dane_okresowe[str(j)]["koszt_normalny"] + 
        prod_nadwym[j] * dane_okresowe[str(j)]["koszt_nadwym"]
        for j in okresy
    )
    koszt_magaz = lpSum(
        magazyn[j] * koszt_magazynowania
        for j in okresy
    )
    
    model += koszt_prod + koszt_magaz, "Calkowity_koszt"

    for j in okresy:
        js = str(j)
        
        model += (magazyn[j-1] + prod_normalna[j] + prod_nadwym[j] - dane_okresowe[js]["popyt"] 
                  == magazyn[j]), f"Bilans_magazynu_okres_{j}"

        model += prod_normalna[j] <= poj_prod_normalna, f"Limit_prod_normalnej_{j}"

        model += prod_nadwym[j] <= dane_okresowe[js]["limit_nadwym"], f"Limit_prod_nadwym_{j}"
        
        model += magazyn[j] <= poj_magazynu, f"Limit_magazynu_{j}"

    return model, {"prod_normalna": prod_normalna, "prod_nadwym": prod_nadwym, "magazyn": magazyn}

def build_model_task4(data):
    model = LpProblem(name="Najkrotsza_sciezka_z_limitem_czasu", sense=LpMinimize)

    wezly = data["wezly"]
    luki = data["luki"]
    start = data["start"]
    koniec = data["koniec"]
    limit_czasu = data["limit_czasu"]

    dane_lukow = {}
    for luk_dane in data["luki"]:
        if len(luk_dane) == 4:
            z, do, koszt, czas = luk_dane
            dane_lukow[(z, do)] = {"koszt": koszt, "czas": czas}
        elif len(luk_dane) == 5:
            _nazwa, z, do, koszt, czas = luk_dane
            dane_lukow[(z, do)] = {"koszt": koszt, "czas": czas}

    x = LpVariable.dicts("Sciezka", dane_lukow.keys(), cat='Binary')

    model += lpSum(dane_lukow[luk]["koszt"] * x[luk] for luk in dane_lukow), "Calkowity_koszt_sciezki"

    model += lpSum(dane_lukow[luk]["czas"] * x[luk] for luk in dane_lukow) <= limit_czasu, "Limit_czasu"

    for w in wezly:
        przeplyw_wchodzacy = lpSum(x[(i, j)] for (i, j) in dane_lukow if j == w)
        przeplyw_wychodzacy = lpSum(x[(i, j)] for (i, j) in dane_lukow if i == w)

        if w == start:
            model += przeplyw_wychodzacy - przeplyw_wchodzacy == 1, f"Przeplyw_start_{w}"
        elif w == koniec:
            model += przeplyw_wchodzacy - przeplyw_wychodzacy == 1, f"Przeplyw_koniec_{w}"
        else:
            model += przeplyw_wchodzacy - przeplyw_wychodzacy == 0, f"Przeplyw_posredni_{w}"

    return model, x

def build_model_task5(data):
    model = LpProblem(name="Minimalizacja_liczby_radiowozow", sense=LpMinimize)

    dzielnice = data["dzielnice"]
    zmiany = data["zmiany"]

    zmienne = {}
    for d in dzielnice:
        for z in zmiany:
            min_val = data["min_radiowozy"][d][z]
            max_val = data["max_radiowozy"][d][z]
            zmienne[(d, z)] = LpVariable(name=f"Radiowozy_{d}_{z}",
                                         lowBound=min_val,
                                         upBound=max_val,
                                         cat='Integer')

    model += lpSum(zmienne[(d, z)] for d in dzielnice for z in zmiany), "Calkowita_liczba_przydzialow"

    for z in zmiany:
        model += lpSum(zmienne[(d, z)] for d in dzielnice) >= data["min_lacznie_zmiana"][z], f"Min_lacznie_zmiana_{z}"
    
    for d in dzielnice:
        model += lpSum(zmienne[(d, z)] for z in zmiany) >= data["min_lacznie_dzielnica"][d], f"Min_lacznie_dzielnica_{d}"

    return model, zmienne

def build_model_task6(data, k):
    m = data["m"]
    n = data["n"]
    kontenery = [tuple(k) for k in data["kontenery"]]
    
    model = LpProblem(name=f"Rozmieszczenie_kamer_k={k}", sense=LpMinimize)

    pozycje = [(r, c) for r in range(m) for c in range(n)]
    pozycje_kamer = [p for p in pozycje if p not in kontenery]
    
    cam = LpVariable.dicts("Kamera", pozycje_kamer, cat='Binary')

    model += lpSum(cam[p] for p in pozycje_kamer), "Liczba_kamer"

    for (kr, kc) in kontenery:
        widzace_kamery = []
        for (pr, pc) in pozycje_kamer:
            if (pr == kr and abs(pc - kc) <= k) or (pc == kc and abs(pr - kr) <= k):
                widzace_kamery.append(cam[(pr, pc)])
        
        if widzace_kamery:
            model += lpSum(widzace_kamery) >= 1, f"Pokrycie_kontenera_{kr}_{kc}"
        else:
            print(f"OSTRZEŻENIE: Kontener w ({kr},{kc}) nie może być przez nic widziany!")
            model += 1 == 0, f"Niespelnialne_pokrycie_{kr}_{kc}"

    return model, cam

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Użycie: python rozwiazania_lp.py <numer_zadania>")
        print("Dostępne zadania: 1, 2, 3, 4a, 4b, 5, 6")
        sys.exit(1)

    numer_zadania_str = sys.argv[1]
    
    if numer_zadania_str == "1":
        data_file = "data_ex1.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task1(data)
        nazwa_pliku_lp = "model_ex1.lp"

    elif numer_zadania_str == "2":
        data_file = "data_ex2.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task2(data)
        nazwa_pliku_lp = "model_ex2.lp"

    elif numer_zadania_str == "3":
        data_file = "data_ex3.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task3(data)
        nazwa_pliku_lp = "model_ex3.lp"

    elif numer_zadania_str == "4a":
        data_file = "data_ex4a.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task4(data)
        nazwa_pliku_lp = "model_ex4a.lp"
        
    elif numer_zadania_str == "4b":
        data_file = "data_ex4b.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task4(data)
        nazwa_pliku_lp = "model_ex4b.lp"
        
    elif numer_zadania_str == "5":
        data_file = "data_ex5.json"
        data = load_data(data_file)
        prob, vars_ = build_model_task5(data)
        nazwa_pliku_lp = "model_ex5.lp"
        
    elif numer_zadania_str == "6":
        data_file = "data_ex6.json"
        data = load_data(data_file)
        k_zasięg = 1
        if len(sys.argv) >= 3:
            try:
                k_zasięg = int(sys.argv[2])
            except ValueError:
                print("Drugi argument (zasięg k) musi być liczbą całkowitą.")
                sys.exit(1)
        prob, vars_ = build_model_task6(data, k=k_zasięg)
        nazwa_pliku_lp = f"model_ex6_k={k_zasięg}.lp"
    else:
        print(f"Nieznany numer zadania: {numer_zadania_str}")
        sys.exit(1)

    solve_and_save(prob, solver_name="CBC", lp_filename=nazwa_pliku_lp, solver_msg=True)
