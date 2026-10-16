#pragma once
#include "Player.h"
#include "Strategy.h"

/**
 * @file AIPlayer.h
 * @brief Joueur IA — délègue le calcul à une Strategy.
 * OWNERSHIP : AIPlayer libère Strategy* dans son destructeur.
 * FORME CANONIQUE complète.
 */
class AIPlayer : public Player {
public:
    AIPlayer(Couleur c, const std::string& n, Strategy* strat);
    AIPlayer(const AIPlayer& other);
    AIPlayer& operator=(const AIPlayer& other);
    ~AIPlayer();

    Move jouer(const Board& board) override;

private:
    Strategy* strategy;
};
