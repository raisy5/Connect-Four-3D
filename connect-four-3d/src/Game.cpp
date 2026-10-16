#include "Game.h"
#include <iostream>

Game::Game(Player* j0, Player* j1) : tourCourant(0) {
    joueurs[0] = j0;
    joueurs[1] = j1;
}

Game::Game(const Game& other) : plateau(other.plateau), tourCourant(other.tourCourant) {
    joueurs[0] = other.joueurs[0];
    joueurs[1] = other.joueurs[1];
}

Game& Game::operator=(const Game& other) {
    if (this != &other) {
        plateau     = other.plateau;
        tourCourant = other.tourCourant;
        joueurs[0]  = other.joueurs[0];
        joueurs[1]  = other.joueurs[1];
    }
    return *this;
}

Game::~Game() {
    delete joueurs[0];
    delete joueurs[1];
}

Couleur Game::lancer() {
    std::cout << "=== Puissance 4 3D ===\n";
    std::cout << "O = Blancs   X = Noirs\n";
    std::cout << "Colonnes : A B C D    Profondeur : 0 a 4\n\n";

    while (true) {
        Player* joueurActif = joueurs[tourCourant];
        Couleur couleur     = joueurActif->getCouleur();

        plateau.afficher();
        Move coup = joueurActif->jouer(plateau);
        plateau.placerPerle(coup, couleur);

        if (plateau.getBitBoard().estGagne(
                coup.getColonne(), coup.getProfondeur(), couleur)) {
            plateau.afficher();
            std::cout << "*** " << joueurActif->getNom() << " gagne ! ***\n";
            return couleur;
        }
        if (plateau.estPlein()) {
            plateau.afficher();
            std::cout << "*** Match nul ! ***\n";
            return VIDE;
        }
        tourCourant = 1 - tourCourant;
    }
}
