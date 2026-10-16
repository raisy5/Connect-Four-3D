#include "Move.h"

// --- Move ------
// Represente UN coup : une colonne (0..3) et une profondeur (0..4).
// Le niveau n'est pas stocke : la perle tombe toute seule au niveau libre.

// Constructeur : cree un coup a partir d'une colonne et d'une profondeur.
Move::Move(int col, int prof) : m_colonne(col), m_profondeur(prof) {}

// Constructeur de copie : recopie les deux champs.
Move::Move(const Move& other)
    : m_colonne(other.m_colonne), m_profondeur(other.m_profondeur) {}

// Operateur d'affectation : recopie avec garde anti auto-affectation.
Move& Move::operator=(const Move& other) {
    if (this != &other) {
        m_colonne     = other.m_colonne;
        m_profondeur  = other.m_profondeur;
    }
    return *this;
}

Move::~Move() {}

// Accesseurs en lecture.
int  Move::getColonne()    const { return m_colonne;    }
int  Move::getProfondeur() const { return m_profondeur; }

// Egalite : deux coups sont identiques si meme colonne ET meme profondeur.
// Utilise par l'IA pour comparer un coup aux "killer moves".
bool Move::operator==(const Move& o) const {
    return m_colonne == o.m_colonne && m_profondeur == o.m_profondeur;
}