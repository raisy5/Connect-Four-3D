/**
 * @file BitBoard.cpp
 * @brief Implémentation du moteur bitboard.
 */
#include "BitBoard.h"
#include <cstring>
#include <random>

// ── Définitions des membres statiques ────────────────────────────────────────
u128             BitBoard::MASQUES[122];
std::vector<int> BitBoard::MASQUES_PAR_CASE[80];
bool             BitBoard::masquesInitialises = false;
u64              BitBoard::ZOBRIST_TABLE[80][2];
bool             BitBoard::zobristInitialise  = false;
int              BitBoard::nbMasquesTotal     = 0;
u64              BitBoard::ZOBRIST_TRAIT      = 0;

// ─────────────────────────────────────────────────────────────────────────────

void BitBoard::initMasques() {
    static const int DIRS[13][3] = {
        {1,0,0},{0,1,0},{0,0,1},
        {1,1,0},{1,-1,0},{1,0,1},{1,0,-1},
        {0,1,1},{0,1,-1},
        {1,1,1},{1,1,-1},{1,-1,1},{1,-1,-1}
    };

    int  nb = 0;
    u128 dejaVu[300];
    int  nbDV = 0;

    for (int d = 0; d < 13; d++) {
        int dc = DIRS[d][0], dp = DIRS[d][1], dn = DIRS[d][2];
        for (int c = 0; c < 4; c++)
        for (int p = 0; p < 5; p++)
        for (int n = 0; n < 4; n++) {
            bool valide = true;
            int  idx[4];
            for (int i = 0; i < 4; i++) {
                int nc=c+dc*i, np=p+dp*i, nn=n+dn*i;
                if (nc<0||nc>=4||np<0||np>=5||nn<0||nn>=4) { valide=false; break; }
                idx[i] = indice(nc, np, nn);
            }
            if (!valide) continue;

            u128 masque = 0;
            for (int i = 0; i < 4; i++) masque |= (u128)1 << idx[i];

            bool doublon = false;
            for (int m = 0; m < nbDV; m++)
                if (dejaVu[m] == masque) { doublon = true; break; }
            if (doublon) continue;

            dejaVu[nbDV++] = masque;
            MASQUES[nb] = masque;
            for (int i = 0; i < 4; i++)
                MASQUES_PAR_CASE[idx[i]].push_back(nb);
            nb++;
        }
    }
    nbMasquesTotal = nb;
}

void BitBoard::initZobrist() {
    std::mt19937_64 rng(123456789ULL);
    for (int i = 0; i < 80; i++) {
        ZOBRIST_TABLE[i][0] = rng();
        ZOBRIST_TABLE[i][1] = rng();
    }
    ZOBRIST_TRAIT = rng();
}

// ── Forme canonique ───────────────────────────────────────────────────────────

BitBoard::BitBoard() : blanc(0), noir(0), hash(0) {
    memset(niveauLibre, 0, sizeof(niveauLibre));
    if (!masquesInitialises) { initMasques(); masquesInitialises = true; }
    if (!zobristInitialise)  { initZobrist(); zobristInitialise  = true; }
}

BitBoard::BitBoard(const BitBoard& other)
    : blanc(other.blanc), noir(other.noir), hash(other.hash)
{
    memcpy(niveauLibre, other.niveauLibre, sizeof(niveauLibre));
}

BitBoard& BitBoard::operator=(const BitBoard& other) {
    if (this != &other) {
        blanc = other.blanc;
        noir  = other.noir;
        hash  = other.hash;
        memcpy(niveauLibre, other.niveauLibre, sizeof(niveauLibre));
    }
    return *this;
}

BitBoard::~BitBoard() {}

// ── Accesseurs statiques ──────────────────────────────────────────────────────

int BitBoard::indice(int col, int prof, int niv) {
    return col * 20 + prof * 4 + niv;
}

u128 BitBoard::getMasque(int m) {
    return MASQUES[m];
}

const std::vector<int>& BitBoard::getMasquesPourCase(int idx) {
    return MASQUES_PAR_CASE[idx];
}

int BitBoard::nbMasques() {
    return nbMasquesTotal;
}
u64 BitBoard::getZobristTrait() {
    return ZOBRIST_TRAIT;
}


// ── Do/Undo ───────────────────────────────────────────────────────────────────

void BitBoard::poser(int col, int prof, Couleur coul) {
    int niv = niveauLibre[col][prof];
    int idx = indice(col, prof, niv);
    if (coul == BLANC) blanc |= (u128)1 << idx;
    else               noir  |= (u128)1 << idx;
    hash ^= ZOBRIST_TABLE[idx][coul - 1];
    niveauLibre[col][prof]++;
}

void BitBoard::annuler(int col, int prof, Couleur coul) {
    niveauLibre[col][prof]--;
    int niv = niveauLibre[col][prof];
    int idx = indice(col, prof, niv);
    if (coul == BLANC) blanc &= ~((u128)1 << idx);
    else               noir  &= ~((u128)1 << idx);
    hash ^= ZOBRIST_TABLE[idx][coul - 1];
}

// ── Requêtes ──────────────────────────────────────────────────────────────────

bool BitBoard::estBatonnetPlein(int col, int prof) const {
    return niveauLibre[col][prof] == 4;
}

bool BitBoard::plateauPlein() const {
    for (int c = 0; c < 4; c++)
        for (int p = 0; p < 5; p++)
            if (niveauLibre[c][p] < 4) return false;
    return true;
}

std::vector<Move> BitBoard::getCoupsPossibles() const {
    std::vector<Move> coups;
    for (int c = 0; c < 4; c++)
        for (int p = 0; p < 5; p++)
            if (!estBatonnetPlein(c, p))
                coups.push_back(Move(c, p));
    return coups;
}

bool BitBoard::estGagne(int col, int prof, Couleur coul) const {
    int niv = niveauLibre[col][prof] - 1;
    int idx = indice(col, prof, niv);
    u128 b  = (coul == BLANC) ? blanc : noir;
    for (int m : MASQUES_PAR_CASE[idx])
        if ((b & MASQUES[m]) == MASQUES[m]) return true;
    return false;
}

bool BitBoard::getBit(int idx, Couleur coul) const {
    u128 b = (coul == BLANC) ? blanc : noir;
    return (b >> idx) & 1;
}

int BitBoard::getNiveauActuel(int col, int prof) const {
    return niveauLibre[col][prof] - 1;
}

int BitBoard::getNiveauLibre(int col, int prof) const {
    return niveauLibre[col][prof];
}

u64 BitBoard::getHash() const   { return hash;  }
u128 BitBoard::getNoir() const  { return noir;  }
u128 BitBoard::getBlanc() const { return blanc; }
