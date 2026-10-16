/**
 * @file main.cpp
 * @brief Point d'entrée — deux IA s'affrontent en local.
 */
#include <cstdlib>
#include <ctime>
#include <iostream>
#include "Game.h"
#include "AIPlayer.h"
#include "AlphaBetaStrategy.h"
#include "Evaluator.h"

int main() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    std::cout << "=== Puissance 4 3D — IA vs IA ===\n\n";

    Player* joueur0 = new AIPlayer(BLANC, "IA_Blanc",
        new AlphaBetaStrategy(new EvaluatorAvance(), new EvaluatorMoyen(), 10, 7400));
    Player* joueur1 = new AIPlayer(NOIR, "IA_Noir",
        new AlphaBetaStrategy(new EvaluatorAvance(), new EvaluatorMoyen(), 10, 7400));

    Game partie(joueur0, joueur1);
    partie.lancer();

    return 0;
}
