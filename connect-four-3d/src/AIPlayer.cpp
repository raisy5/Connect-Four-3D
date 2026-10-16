#include "AIPlayer.h"
#include <iostream>

AIPlayer::AIPlayer(Couleur c, const std::string& n, Strategy* strat)
    : Player(c, n), strategy(strat) {}

AIPlayer::AIPlayer(const AIPlayer& other)
    : Player(other), strategy(other.strategy) {}

AIPlayer& AIPlayer::operator=(const AIPlayer& other) {
    if (this != &other) {
        Player::operator=(other);
        strategy = other.strategy;
    }
    return *this;
}

AIPlayer::~AIPlayer() {
    delete strategy;
}

Move AIPlayer::jouer(const Board& board) {
    const BitBoard& bb = board.getBitBoard();
    std::cout << nom << " reflechit...\n";
    Move coup = strategy->meilleurCoup(bb, couleur);
    std::cout << nom << " joue : colonne " << (char)('A' + coup.getColonne())
              << ", profondeur " << coup.getProfondeur() << "\n\n";
    return coup;
}
