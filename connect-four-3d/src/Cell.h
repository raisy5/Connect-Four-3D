#pragma once
#include "Couleur.h"

/**
 * @file Cell.h
 * @brief Une case individuelle du plateau 3D.
 *
 * ENCAPSULATION : m_occupee et m_couleur sont privés.
 * FORME CANONIQUE : constructeur, copie, affectation, destructeur.
 */
class Cell {
public:
    Cell();
    Cell(const Cell& other);
    Cell& operator=(const Cell& other);
    ~Cell();

    bool    estOccupee() const;
    Couleur getCouleur() const;
    void    occuper(Couleur c);
    void    vider();

private:
    bool    m_occupee;
    Couleur m_couleur;
};
