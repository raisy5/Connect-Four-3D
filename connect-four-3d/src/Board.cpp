#include "Board.h"
#include <iostream>
#include <cstring>

// --------------------------------------------------------------------------
//  Board : plateau complet du Puissance 4 3D (grille 4x5x4 + bitboard miroir).
//  grille[col][prof][niv] = cases lisibles pour l'affichage.
//  niveauLibre[col][prof] = prochain niveau libre de chaque batonnet.
//  bitboard = meme info en bits, pour l'IA.
// --------------------------------------------------------------------------

// Constructeur : plateau vide, tous les niveaux libres a 0.
Board::Board() {
    memset(niveauLibre, 0, sizeof(niveauLibre));
}

// Constructeur de copie : recopie bitboard, niveaux et grille.
Board::Board(const Board& other) : bitboard(other.bitboard) {
    memcpy(niveauLibre, other.niveauLibre, sizeof(niveauLibre));
    for (int c = 0; c < 4; c++)
    for (int p = 0; p < 5; p++)
    for (int n = 0; n < 4; n++)
        grille[c][p][n] = other.grille[c][p][n];
}

// Operateur d'affectation : meme recopie, avec garde anti auto-affectation.
Board& Board::operator=(const Board& other) {
    if (this != &other) {
        bitboard = other.bitboard;
        memcpy(niveauLibre, other.niveauLibre, sizeof(niveauLibre));
        for (int c = 0; c < 4; c++)
        for (int p = 0; p < 5; p++)
        for (int n = 0; n < 4; n++)
            grille[c][p][n] = other.grille[c][p][n];
    }
    return *this;
}

Board::~Board() {}

// --------------------------------------------------------------------------
// placerPerle : pose une perle ; elle tombe au niveau libre le plus bas.
// On met a jour la grille, le bitboard, puis on incremente le niveau libre.
// --------------------------------------------------------------------------
void Board::placerPerle(Move coup, Couleur coul) {
    int col  = coup.getColonne();
    int prof = coup.getProfondeur();
    int n    = niveauLibre[col][prof];
    grille[col][prof][n].occuper(coul);
    bitboard.poser(col, prof, coul);
    niveauLibre[col][prof]++;
}

// Un batonnet est plein quand ses 4 niveaux sont occupes.
bool Board::estBatonnetPlein(int col, int prof) const {
    return niveauLibre[col][prof] == 4;
}

// Le plateau est plein si tous les batonnets le sont (= match nul possible).
bool Board::estPlein() const {
    for (int c = 0; c < 4; c++)
        for (int p = 0; p < 5; p++)
            if (!estBatonnetPlein(c, p)) return false;
    return true;
}

// Acces en lecture a une case precise.
const Cell& Board::getCell(int col, int prof, int niv) const {
    return grille[col][prof][niv];
}

// Niveau de la derniere perle posee (-1 si le batonnet est vide).
int Board::getNiveauActuel(int col, int prof) const {
    return niveauLibre[col][prof] - 1;
}

// Donne le bitboard interne (utilise par l'IA).
const BitBoard& Board::getBitBoard() const {
    return bitboard;
}

// --------------------------------------------------------------------------
// afficher : rendu texte du plateau dans le terminal.
// Niveaux affiches du haut (3) vers le bas (0) ; O = Blanc, X = Noir, . = vide.
// --------------------------------------------------------------------------
void Board::afficher() const {
    const char COLS[] = "ABCD";
    std::cout << "\n         ";
    for (int p = 0; p < 5; p++) std::cout << "  Prof." << p << "    ";
    std::cout << "\n";

    for (int n = 3; n >= 0; n--) {              // chaque niveau, du plus haut au plus bas
        std::cout << " Niv " << n << " |";
        for (int p = 0; p < 5; p++) {           // chaque profondeur
            for (int c = 0; c < 4; c++) {       // chaque colonne
                if      (!grille[c][p][n].estOccupee())         std::cout << " .";
                else if (grille[c][p][n].getCouleur() == BLANC) std::cout << " O";
                else                                             std::cout << " X";
            }
            std::cout << "  |";
        }
        std::cout << "\n";
    }

    // Ligne de separation + en-tete des colonnes.
    std::cout << "       +";
    for (int p = 0; p < 5; p++) std::cout << "---------+";
    std::cout << "\n         ";
    for (int p = 0; p < 5; p++) {
        std::cout << " ";
        for (int c = 0; c < 4; c++) std::cout << " " << COLS[c];
        std::cout << "   ";
    }
    std::cout << "\n\n  O = Blanc   X = Noir   . = vide\n\n";
}