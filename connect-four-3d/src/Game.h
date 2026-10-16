#pragma once
#include "Board.h"
#include "Player.h"
#include "Couleur.h"

/**
 * @file Game.h
 * @brief Orchestre la partie du début à la fin.
 * OWNERSHIP : Game libère les deux Player* dans son destructeur.
 * FORME CANONIQUE complète.
 */
class Game {
public:
    Game(Player* j0, Player* j1);
    Game(const Game& other);
    Game& operator=(const Game& other);
    ~Game();

    Couleur lancer();

private:
    Board   plateau;
    Player* joueurs[2];
    int     tourCourant;
};
