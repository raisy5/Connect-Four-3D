#pragma once
#include <climits>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <algorithm>
#include <vector>
#include <cstring>
#include "Strategy.h"
#include "Evaluator.h"

/**
 * @file AlphaBetaStrategy.h
 * @brief Alpha-Bêta + Iterative Deepening + Move Ordering
 *        + Table de Transposition (Zobrist + Trait) + Killer Moves
 *        + Asymétrie défensive Blancs/Noirs.
 *
 * ENCAPSULATION : tous les membres et méthodes internes sont privés.
 * Seul meilleurCoup() est public.
 * FORME CANONIQUE complète.
 */

struct EntreeTransposition {
    int  score;
    int  profondeur;
    enum Type { EXACT, LOWER_BOUND, UPPER_BOUND } type;
};

class AlphaBetaStrategy : public Strategy {
public:
    AlphaBetaStrategy(Evaluator* evalComplet, Evaluator* evalMoyen,
                      int profMax = 10, int tempsMs = 7500);
    AlphaBetaStrategy(const AlphaBetaStrategy& other);
    AlphaBetaStrategy& operator=(const AlphaBetaStrategy& other);
    ~AlphaBetaStrategy();

    Move meilleurCoup(const BitBoard& bb, Couleur coulIA) override;

private:
    Evaluator* evaluateurComplet;
    Evaluator* evaluateurMoyen;
    int        profondeurMax;
    int        tempsLimiteMs;
    bool       timeOut;
    bool       joueurSecond;

    using Horloge    = std::chrono::steady_clock;
    using PointTemps = std::chrono::time_point<Horloge>;
    PointTemps debutDuCoup;

    std::unordered_map<u64, EntreeTransposition> transpositionTable;
    Move killers[20][2];

    bool              tempsDepasse()                                               const;
    void              enregistrerKiller(const Move& coup, int profondeur, int score);
    std::vector<Move> trierCoups(BitBoard& bb, const std::vector<Move>& coups,
                                 Couleur coulIA, int profondeur);
    Move              chercherAuNiveau(BitBoard& bb, int profondeur, Couleur coulIA);
    int               alphaBeta(BitBoard& bb, int profondeur, int alpha, int beta,
                                bool maximise, Couleur coulIA);
};
