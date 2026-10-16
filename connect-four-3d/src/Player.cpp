#include "Player.h"

Player::Player(Couleur c, const std::string& n) : couleur(c), nom(n) {}

Player::Player(const Player& other) : couleur(other.couleur), nom(other.nom) {}

Player& Player::operator=(const Player& other) {
    if (this != &other) {
        couleur = other.couleur;
        nom     = other.nom;
    }
    return *this;
}

Player::~Player() {}

Couleur     Player::getCouleur() const { return couleur; }
std::string Player::getNom()     const { return nom;     }
