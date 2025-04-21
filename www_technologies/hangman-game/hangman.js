const alphabet = "AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŹŻ".split("");
const secretWord = "ABAŻUR";  // w praktyce losujesz z listy

const wordContainer  = document.getElementById("word-container");
const tilesContainer = document.getElementById("tiles-container");

// 1) Generujemy sloty dla liter
function renderSlots(word) {
  wordContainer.innerHTML = "";  // czyścimy
  for (let i = 0; i < word.length; i++) {
    const slot = document.createElement("div");
    slot.classList.add("letter-slot");
    // opcjonalnie: slot.dataset.index = i;
    wordContainer.appendChild(slot);
  }
}

// 2) Generujemy kafelki z literami
function renderTiles() {
  tilesContainer.innerHTML = "";
  alphabet.forEach(letter => {
    const btn = document.createElement("div");
    btn.classList.add("tile");
    btn.textContent = letter;
    btn.addEventListener("click", () => handleGuess(letter, btn));
    tilesContainer.appendChild(btn);
  });
}

// 3) Obsługa kliknięcia
function handleGuess(letter, btnElement) {
  if (btnElement.classList.contains("disabled")) return;
  btnElement.classList.add("disabled");

  const indexes = [];
  for (let i = 0; i < secretWord.length; i++) {
    if (secretWord[i] === letter) indexes.push(i);
  }

  if (indexes.length) {
    btnElement.classList.add("correct");
    // wstawiamy literki w odpowiednie sloty
    indexes.forEach(i => {
      const slot = wordContainer.children[i];
      slot.textContent = letter;
    });
  } else {
    btnElement.classList.add("wrong");
    // tutaj np. rysujemy kolejny element szubienicy
    drawHangmanPart();
  }

  checkWinOrLose();
}

// Inicjalizacja
renderSlots(secretWord);
renderTiles();
