#pragma once
#include "Cell.h"
#include "Move.h"
#include "Couleur.h"
#include "BitBoard.h"

/**
 * @file Board.h
 * @brief Plateau de jeu 3D — affichage et interaction humain.
 *
 * DOUBLE REPRÉSENTATION :
 *   grille[4][5][4] → affichage terminal
 *   bitboard        → moteur IA, synchronisé automatiquement
 *
 * FORME CANONIQUE complète.
 */
class Board {
public:
    Board();
    Board(const Board& other);
    Board& operator=(const Board& other);
    ~Board();

    void            placerPerle(Move coup, Couleur coul);
    bool            estBatonnetPlein(int col, int prof)        const;
    bool            estPlein()                                  const;
    const Cell&     getCell(int col, int prof, int niv)        const;
    int             getNiveauActuel(int col, int prof)          const;
    const BitBoard& getBitBoard()                               const;
    void            afficher()                                  const;

private:
    Cell     grille[4][5][4];
    int      niveauLibre[4][5];
    BitBoard bitboard;
};
