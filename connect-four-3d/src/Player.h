#pragma once
#include <string>
#include "Move.h"
#include "Board.h"
#include "Couleur.h"

/**
 * @file Player.h
 * @brief Classe abstraite représentant un joueur.
 * FORME CANONIQUE complète + destructeur virtuel pour polymorphisme.
 */
class Player {
public:
    Player(Couleur c, const std::string& n);
    Player(const Player& other);
    Player& operator=(const Player& other);
    virtual ~Player();

    virtual Move jouer(const Board& board) = 0;

    Couleur     getCouleur() const;
    std::string getNom()     const;

protected:
    Couleur     couleur;
    std::string nom;
};
