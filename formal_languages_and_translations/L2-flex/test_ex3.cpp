#include <iostream>
#include <string>

/**
 * TEST 1: Komentarz Doxygen (Blokowy JavaDoc)
 * Powinien zostac zachowany tylko z flaga -d
 */
void testDoxygen() {
    /// TEST 2: Komentarz Doxygen (Liniowy z 3 slashami)
    //! TEST 3: Komentarz Doxygen (Liniowy z wykrzyknikiem)
    /*! TEST 4: Komentarz Doxygen (Blokowy Qt style) */
    
    std::cout << "Doxygen section passed." << std::endl;
}

int main() {
    // TEST 5: Zwykły komentarz liniowy - powinien zniknąć
    /* TEST 6: Zwykły komentarz blokowy - powinien zniknąć */
    
    /* TEST 7: Komentarz blokowy
       w wielu
       liniach - powinien zniknąć */

    // ==========================================
    // TEST 8: PUŁAPKI (Stringi i znaki)
    // Te elementy NIE MOGĄ zostać usunięte!
    // ==========================================

    std::string s1 = "To jest string, nie komentarz: // zostaw mnie";
    std::string s2 = "To tez string: /* nie usuwaj mnie */";
    std::string s3 = "Doxygen w stringu: /** zostaw */ oraz /// zostaw";
    
    // Testowanie znaków ucieczki (escaped quotes)
    std::string s4 = "Znak cudzyslowia \" // to nadal string";
    char c1 = '"';  // Znak cudzysłowia
    char c2 = '\''; // Znak apostrofu
    char c3 = '/';  // Znak slash

    // Testowanie URL (częsty błąd parserów - traktują http:// jako komentarz)
    std::string url = "https://google.com"; 

    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << url << std::endl;

    // ==========================================
    // TEST 9: EDGE CASES (Dziwne formatowanie)
    // ==========================================

    int a = 1; /* Komentarz w srodku linii */ int b = 2;
    
    // Komentarz z kontynuacją linii (line splicing) \
    int c = 3; to_linia_jest_zakomentowana_przez_backslash_wyzej;
    
    /****** Dziwny nagłówek z wieloma gwiazdkami ******/
    
    /* Pusty blok obok */ /**/
    
    if (a == 1) {
        // Wcięty komentarz
        return 0;
    }

    return 1;
}