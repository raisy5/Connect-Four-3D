#pragma once

/**
 * @file Move.h
 * @brief Un coup = colonne + profondeur du bâtonnet choisi.
 *
 * colonne    : 0=A, 1=B, 2=C, 3=D
 * profondeur : 0 à 4
 *
 * FORME CANONIQUE : constructeur, copie, affectation, destructeur.
 * operator== nécessaire pour la comparaison des killer moves.
 */
class Move {
public:
    Move(int col = 0, int prof = 0);
    Move(const Move& other);
    Move& operator=(const Move& other);
    ~Move();

    int  getColonne()    const;
    int  getProfondeur() const;
    bool operator==(const Move& o) const;

private:
    int m_colonne;
    int m_profondeur;
};
