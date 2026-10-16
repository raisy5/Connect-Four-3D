#include "Cell.h"

// --------------------------------------------------------------------------
//  Cell : une case du plateau. Deux infos seulement :
//    m_occupee = la case contient-elle une perle ?
//    m_couleur = sa couleur (VIDE / BLANC / NOIR).
// --------------------------------------------------------------------------

// Constructeur : case vide par defaut.
Cell::Cell() : m_occupee(false), m_couleur(VIDE) {}

// Constructeur de copie : recopie les deux champs.
Cell::Cell(const Cell& other)
    : m_occupee(other.m_occupee), m_couleur(other.m_couleur) {}

// Operateur d'affectation : recopie avec garde anti auto-affectation.
Cell& Cell::operator=(const Cell& other) {
    if (this != &other) {
        m_occupee = other.m_occupee;
        m_couleur = other.m_couleur;
    }
    return *this;
}

Cell::~Cell() {}

// Accesseurs en lecture.
bool    Cell::estOccupee() const { return m_occupee; }
Couleur Cell::getCouleur() const { return m_couleur; }

// Pose une perle de couleur c dans la case.
void Cell::occuper(Couleur c) {
    m_occupee = true;
    m_couleur = c;
}

// Vide la case (remet a l'etat initial).
void Cell::vider() {
    m_occupee = false;
    m_couleur = VIDE;
}