#pragma once
#include <vector>
#include <cstdint>
#include "Move.h"
#include "Couleur.h"

typedef unsigned __int128 u128;
typedef uint64_t          u64;

/**
 * @file BitBoard.h
 * @brief Représentation bitboard du plateau — moteur interne de l'IA.
 */
class BitBoard {
public:
    BitBoard();
    BitBoard(const BitBoard& other);
    BitBoard& operator=(const BitBoard& other);
    ~BitBoard();

    static int                     indice(int col, int prof, int niv);
    static u128                    getMasque(int m);
    static const std::vector<int>& getMasquesPourCase(int idx);
    static int                     nbMasques();
    static u64                     getZobristTrait();  // getter au lieu de membre public

    void poser(int col, int prof, Couleur coul);
    void annuler(int col, int prof, Couleur coul);

    bool              estBatonnetPlein(int col, int prof)       const;
    bool              plateauPlein()                             const;
    std::vector<Move> getCoupsPossibles()                       const;
    bool              estGagne(int col, int prof, Couleur coul) const;
    bool              getBit(int idx, Couleur coul)             const;
    int               getNiveauActuel(int col, int prof)        const;
    int               getNiveauLibre(int col, int prof)         const;
    u64               getHash()                                  const;
    u128              getNoir()                                  const;
    u128              getBlanc()                                 const;

private:
    u128 blanc;
    u128 noir;
    int  niveauLibre[4][5];
    u64  hash;

    static u128             MASQUES[122];
    static std::vector<int> MASQUES_PAR_CASE[80];
    static bool             masquesInitialises;
    static u64              ZOBRIST_TABLE[80][2];
    static bool             zobristInitialise;
    static int              nbMasquesTotal;
    static u64              ZOBRIST_TRAIT;

    static void initMasques();
    static void initZobrist();
};
