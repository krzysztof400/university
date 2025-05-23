
```markdown
# ElizaBot – Chatbot o filmach w Haskellu

## Opis
Eliza to prosty chatbot napisany w Haskellu, który prowadzi rozmowę o filmach na podstawie zdefiniowanych wzorców tekstowych.  
Wykorzystuje wyrażenia regularne, losowy wybór odpowiedzi i działa w terminalu w trybie REPL.  
Kod ma modularną strukturę, co ułatwia dodawanie nowych reguł i rozwój bota.

## Struktura katalogów

```

eliza-bot/  
├── src/  
│ ├── Main.hs -- Punkt wejścia  
│ ├── Bot/Engine.hs -- Silnik dopasowywania wzorców  
│ ├── Bot/Memory.hs -- Pamięć rozmowy  
│ ├── Bot/Types.hs -- Typy danych (np. BotState, UserFact)  
│ └── Bot/Patterns.hs -- Lista wzorców konwersacyjnych  
├── data/  
│ └── knowledge_base.json -- Baza wiedzy (o filmach)  
├── test/  
│ └── BotSpec.hs -- Testy jednostkowe  
├── README.md  
└── stack.yaml / cabal.project -- Konfiguracja projektu

```

## Używane biblioteki

- `base` – standardowa biblioteka Haskella  
- `regex-compat` – dopasowywanie wzorców  
- `random` – losowy wybór odpowiedzi  
- `text` – wydajne operacje na tekstach  
- `containers` – struktury danych (np. `Map`)  
- `tasty` – framework do testów jednostkowych  

## Podział obowiązków

- **I/O, typy, konfiguracja** – wspólnie  
- **Silnik, wzorce** – Krzysztof  
- **Pamięć, testy, baza wiedzy + pomoc z silnikiem i wzorcami** – Wit  

## Plan pracy

- **Tydzień 1** – I/O, typy, konfiguracja, ustalenie wzorców i struktur danych (wspólnie)  
- **Tydzień 2–3** – implementacja poszczególnych części przez członków zespołu  

```
