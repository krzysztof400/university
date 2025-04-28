// Klasa obsługująca grę Wisielec
class HangmanGame {
  constructor() {
      // Słowa do odgadnięcia
      this.words = [
        'HISTORIA', 'WIEK', 'CYWILIZACJA', 'REWOLUCJA', 'WOJNA', 
        'IMPERYUM', 'MONARCHIA', 'DEMOKRACJA', 'NARÓD', 'KULTURA', 
        'WYNALAZEK', 'ZABYTKI', 'TRADYCJA', 'WIELKA WYPRAWA', 'ŚREDNIOWIECZE',
        'RENESANS', 'OŚWIECENIE', 'WOJNA ŚWIATOWA', 'PAŃSTWO', 'RZĄDY',
        'MARCHIA', 'REFORMACJA', 'ZJEDNOCZENIE', 'KRÓLESTWO'
    ];
    
      // Polski alfabet
      this.alphabet = 'AĄBCĆDEĘFGHIJKLŁMNŃOÓPQRSŚTUVWXYZŹŻ'.split('');
      
      // Poziom trudności (liczba prób)
      this.maxAttempts = 9;
      
      // Elementy DOM
      this.wordDisplay = document.getElementById('word-display');
      this.keyboard = document.getElementById('keyboard');
      this.message = document.getElementById('message');
      this.newGameBtn = document.getElementById('new-game');
      this.cancelGameBtn = document.getElementById('cancel-game');
      this.canvas = document.getElementById('hangman-canvas');
      this.ctx = this.canvas.getContext('2d');
      
      // Inicjalizacja gry
      this.initGame();
      
      // Dodanie obsługi zdarzeń
      this.newGameBtn.addEventListener('click', () => this.startNewGame());
      this.cancelGameBtn.addEventListener('click', () => this.cancelGame());
      
      // Obsługa klawiatury fizycznej
      document.addEventListener('keydown', (e) => {
          if (this.gameActive) {
              const key = e.key.toUpperCase();
              if (this.alphabet.includes(key) && !this.usedLetters.includes(key)) {
                  this.checkLetter(key);
              }
          }
      });
  }
  
  initGame() {
      // Sprawdzenie czy jest zapisany stan gry
      const savedState = localStorage.getItem('hangmanGameState');
      
      if (savedState) {
          try {
              const state = JSON.parse(savedState);
              this.currentWord = state.currentWord;
              this.displayWord = state.displayWord;
              this.usedLetters = state.usedLetters;
              this.wrongAttempts = state.wrongAttempts;
              this.gameActive = state.gameActive;
              
              // Aktualizacja interfejsu
              this.updateDisplay();
              this.updateKeyboard();
              this.drawHangman();
              
              // Sprawdzenie stanu gry
              if (!this.gameActive) {
                  this.showResult();
              }
          } catch (e) {
              console.error('Błąd podczas wczytywania stanu gry:', e);
              this.startNewGame();
          }
      } else {
          this.startNewGame();
      }
  }
  
  startNewGame() {
      // Losowanie słowa
      this.currentWord = this.words[Math.floor(Math.random() * this.words.length)];
      
      // Inicjalizacja zmiennych
      this.displayWord = Array(this.currentWord.length).fill('_');
      this.usedLetters = [];
      this.wrongAttempts = 0;
      this.gameActive = true;
      
      // Aktualizacja interfejsu
      this.updateDisplay();
      this.createKeyboard();
      this.clearHangman();
      
      // Ukrycie wiadomości
      this.message.style.display = 'none';
      
      // Zapisanie stanu gry
      this.saveGameState();
  }
  
  cancelGame() {
      // Anulowanie gry
      this.gameActive = false;
      this.message.textContent = 'Gra została anulowana. Szukane słowo to: ' + this.currentWord;
      this.message.className = 'message lose';
      this.message.style.display = 'block';
      
      // Wyłączenie klawiatury
      this.disableKeyboard();
      
      // Usunięcie stanu gry
      localStorage.removeItem('hangmanGameState');
  }
  
  updateDisplay() {
      // Aktualizacja wyświetlanego słowa
      this.wordDisplay.innerHTML = '';
      
      for (let letter of this.displayWord) {
          const letterSpace = document.createElement('div');
          letterSpace.className = 'letter-space';
          letterSpace.textContent = letter !== '_' ? letter : '';
          this.wordDisplay.appendChild(letterSpace);
      }
  }
  
  createKeyboard() {
      // Tworzenie klawiatury
      this.keyboard.innerHTML = '';
      
      for (let letter of this.alphabet) {
          const key = document.createElement('button');
          key.className = 'key';
          key.textContent = letter;
          
          if (this.usedLetters.includes(letter)) {
              key.classList.add('used');
          } else {
              key.addEventListener('click', () => {
                  if (this.gameActive && !this.usedLetters.includes(letter)) {
                      this.checkLetter(letter);
                  }
              });
          }
          
          this.keyboard.appendChild(key);
      }
  }
  
  updateKeyboard() {
      // Aktualizacja klawiatury (bez ponownego tworzenia)
      this.createKeyboard();
  }
  
  checkLetter(letter) {
      // Dodanie litery do użytych
      this.usedLetters.push(letter);
      
      // Sprawdzenie czy litera jest w słowie
      let letterFound = false;
      
      for (let i = 0; i < this.currentWord.length; i++) {
          if (this.currentWord[i] === letter) {
              this.displayWord[i] = letter;
              letterFound = true;
          }
      }
      
      // Aktualizacja klawiatury
      this.updateKeyboard();
      
      // Jeśli litera nie jest w słowie, zwiększamy liczbę błędów
      if (!letterFound) {
          this.wrongAttempts++;
          this.drawHangman();
      }
      
      // Aktualizacja wyświetlanego słowa
      this.updateDisplay();
      
      // Sprawdzenie czy gra się zakończyła
      this.checkGameStatus();
      
      // Zapisanie stanu gry
      this.saveGameState();
  }
  
  checkGameStatus() {
      // Sprawdzenie czy gracz wygrał
      if (!this.displayWord.includes('_')) {
          this.gameActive = false;
          this.message.textContent = 'Gratulacje! Odgadłeś słowo: ' + this.currentWord;
          this.message.className = 'message win';
          this.message.style.display = 'block';
          this.disableKeyboard();
          return;
      }
      
      // Sprawdzenie czy gracz przegrał
      if (this.wrongAttempts >= this.maxAttempts) {
          this.gameActive = false;
          this.message.textContent = 'Przegrałeś! Prawidłowe słowo to: ' + this.currentWord;
          this.message.className = 'message lose';
          this.message.style.display = 'block';
          this.disableKeyboard();
          return;
      }
  }
  
  disableKeyboard() {
      // Wyłączenie klawiatury
      const keys = document.querySelectorAll('.key');
      keys.forEach(key => {
          key.classList.add('used');
      });
  }
  
  showResult() {
      // Wyświetlenie wyniku gry
      if (!this.displayWord.includes('_')) {
          this.message.textContent = 'Gratulacje! Odgadłeś słowo: ' + this.currentWord;
          this.message.className = 'message win';
      } else {
          this.message.textContent = 'Przegrałeś! Prawidłowe słowo to: ' + this.currentWord;
          this.message.className = 'message lose';
      }
      this.message.style.display = 'block';
      this.disableKeyboard();
  }
  
  saveGameState() {
      // Zapisanie stanu gry w localStorage
      const gameState = {
          currentWord: this.currentWord,
          displayWord: this.displayWord,
          usedLetters: this.usedLetters,
          wrongAttempts: this.wrongAttempts,
          gameActive: this.gameActive
      };
      
      localStorage.setItem('hangmanGameState', JSON.stringify(gameState));
  }
  
  clearHangman() {
      // Czyszczenie rysunku wisielca
      this.ctx.clearRect(0, 0, this.canvas.width, this.canvas.height);
  }
  
  drawHangman() {
      // Czyszczenie rysunku
      this.clearHangman();
      
      // Rysowanie elementów wisielca w zależności od liczby błędów
      this.ctx.lineWidth = 4;
      this.ctx.strokeStyle = '#333';
      
      // Podstawa
      if (this.wrongAttempts >= 1) {
          this.ctx.beginPath();
          this.ctx.moveTo(20, 180);
          this.ctx.lineTo(180, 180);
          this.ctx.stroke();
      }
      
      // Słupek pionowy
      if (this.wrongAttempts >= 2) {
          this.ctx.beginPath();
          this.ctx.moveTo(40, 180);
          this.ctx.lineTo(40, 20);
          this.ctx.stroke();
      }
      
      // Belka górna
      if (this.wrongAttempts >= 3) {
          this.ctx.beginPath();
          this.ctx.moveTo(40, 20);
          this.ctx.lineTo(120, 20);
          this.ctx.stroke();
      }
      
      // Linka
      if (this.wrongAttempts >= 4) {
          this.ctx.beginPath();
          this.ctx.moveTo(120, 20);
          this.ctx.lineTo(120, 40);
          this.ctx.stroke();
      }
      
      // Głowa
      if (this.wrongAttempts >= 5) {
          this.ctx.beginPath();
          this.ctx.arc(120, 55, 15, 0, Math.PI * 2);
          this.ctx.stroke();
      }
      
      // Tułów
      if (this.wrongAttempts >= 6) {
          this.ctx.beginPath();
          this.ctx.moveTo(120, 70);
          this.ctx.lineTo(120, 120);
          this.ctx.stroke();
      }
      
      // Lewa ręka
      if (this.wrongAttempts >= 7) {
          this.ctx.beginPath();
          this.ctx.moveTo(120, 80);
          this.ctx.lineTo(100, 100);
          this.ctx.stroke();
      }
      
      // Prawa ręka
      if (this.wrongAttempts >= 8) {
          this.ctx.beginPath();
          this.ctx.moveTo(120, 80);
          this.ctx.lineTo(140, 100);
          this.ctx.stroke();
      }
      
      // Lewa noga
      if (this.wrongAttempts >= 9) {
          this.ctx.beginPath();
          this.ctx.moveTo(120, 120);
          this.ctx.lineTo(100, 150);
          this.ctx.stroke();
      }
      
      // Prawa noga (maksymalna liczba błędów)
      if (this.wrongAttempts >= 10) {
          this.ctx.beginPath();
          this.ctx.moveTo(120, 120);
          this.ctx.lineTo(140, 150);
          this.ctx.stroke();
      }
  }
}

// Inicjalizacja gry
document.addEventListener('DOMContentLoaded', () => {
  const game = new HangmanGame();
});