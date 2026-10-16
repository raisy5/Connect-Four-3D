#pragma once
#include "BitBoard.h"
#include "Move.h"
#include "Couleur.h"

/**
 * @file Strategy.h
 * @brief Interface abstraite pour les algorithmes de jeu.
 * FORME CANONIQUE : constructeur, copie, affectation, destructeur virtuel.
 */
class Strategy {
public:
    Strategy();
    Strategy(const Strategy& other);
    Strategy& operator=(const Strategy& other);
    virtual ~Strategy();

    virtual Move meilleurCoup(const BitBoard& bb, Couleur coulIA) = 0;
};
