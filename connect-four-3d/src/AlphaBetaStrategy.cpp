/**
 * @file AlphaBetaStrategy.cpp
 * @brief Alpha-Bêta + Iterative Deepening + Move Ordering
 *        + Table de Transposition + Killer Moves
 *        + Asymétrie Blancs/Noirs + Trait Zobrist.
 *
 * CORRECTION TRAIT ZOBRIST :
 *   Le hash Zobrist de base ne distingue pas "même plateau, tour différent".
 *   Exemple : position P avec les Blancs qui jouent → hash H
 *             position P avec les Noirs qui jouent  → hash H aussi (FAUX)
 *
 *   La TT stocke un score pour H. Quand l'IA Noirs consulte H, elle récupère
 *   un score calculé du point de vue des Blancs → mauvaise décision.
 *
 *   SOLUTION : dans alphaBeta(), les nœuds MIN (tour de l'adversaire) XORent
 *   leur hash avec ZOBRIST_TRAIT avant de consulter/écrire la TT.
 *   Résultat : position P tour Blancs → hash H
 *              position P tour Noirs  → hash H ^ TRAIT  (différent)
 *   La TT ne confond plus jamais les deux.
 */
#include "AlphaBetaStrategy.h"

// ─── Constructeur / Destructeur ───────────────────────────────────────────────

AlphaBetaStrategy::AlphaBetaStrategy(Evaluator* evalComplet, Evaluator* evalMoyen,
                                     int profMax, int tempsMs)
    : evaluateurComplet(evalComplet),
      evaluateurMoyen(evalMoyen),
      profondeurMax(profMax),
      tempsLimiteMs(tempsMs - 50),
      timeOut(false),
      joueurSecond(false)
{
    memset(killers, 0, sizeof(killers));
}

AlphaBetaStrategy::~AlphaBetaStrategy() {
    delete evaluateurComplet;
    delete evaluateurMoyen;
}

AlphaBetaStrategy::AlphaBetaStrategy(const AlphaBetaStrategy& other)
    : evaluateurComplet(other.evaluateurComplet),
      evaluateurMoyen(other.evaluateurMoyen),
      profondeurMax(other.profondeurMax),
      tempsLimiteMs(other.tempsLimiteMs),
      timeOut(other.timeOut),
      joueurSecond(other.joueurSecond)
{
    memset(killers, 0, sizeof(killers));
}

AlphaBetaStrategy& AlphaBetaStrategy::operator=(const AlphaBetaStrategy& other) {
    if (this != &other) {
        evaluateurComplet = other.evaluateurComplet;
        evaluateurMoyen   = other.evaluateurMoyen;
        profondeurMax     = other.profondeurMax;
        tempsLimiteMs     = other.tempsLimiteMs;
        timeOut           = other.timeOut;
        joueurSecond      = other.joueurSecond;
        memset(killers, 0, sizeof(killers));
    }
    return *this;
}


// ─── meilleurCoup ─────────────────────────────────────────────────────────────

Move AlphaBetaStrategy::meilleurCoup(const BitBoard& bb, Couleur coulIA) {
    debutDuCoup = Horloge::now();
    timeOut     = false;
    transpositionTable.clear();
    memset(killers, 0, sizeof(killers));

    // Blancs = toujours premier joueur (règle officielle)
    // Noirs  = toujours second → mode défensif activé
    joueurSecond = (coulIA == NOIR);

    std::cout << "  [" << (joueurSecond ? "Noirs - defensif" : "Blancs - offensif") << "]\n";

    std::vector<Move> coupsPossibles = bb.getCoupsPossibles();
    if (coupsPossibles.size() == 1) return coupsPossibles[0];

    Move meilleur = coupsPossibles[0];
    BitBoard plateau = bb;

    for (int profondeur = 1; profondeur <= profondeurMax; profondeur++) {
        Move candidat = chercherAuNiveau(plateau, profondeur, coulIA);
        if (!timeOut) {
            meilleur = candidat;
            std::cout << "  profondeur " << profondeur << " OK\n";
        } else {
            std::cout << "  (arrete a profondeur " << profondeur << ")\n";
            break;
        }
    }
    return meilleur;
}

// ─── tempsDepasse ────────────────────────────────────────────────────────────

bool AlphaBetaStrategy::tempsDepasse() const {
    auto duree = std::chrono::duration_cast<std::chrono::milliseconds>(
        Horloge::now() - debutDuCoup).count();
    return duree >= tempsLimiteMs;
}

// ─── enregistrerKiller ───────────────────────────────────────────────────────

void AlphaBetaStrategy::enregistrerKiller(const Move& coup, int profondeur, int score) {
    if (profondeur >= 20) return;
    if (score >= 90000)   return;
    if (!(coup == killers[profondeur][0])) {
        killers[profondeur][1] = killers[profondeur][0];
        killers[profondeur][0] = coup;
    }
}

// ─── trierCoups ──────────────────────────────────────────────────────────────

std::vector<Move> AlphaBetaStrategy::trierCoups(BitBoard& bb,
                                                  const std::vector<Move>& coups,
                                                  Couleur coulIA,
                                                  int profondeur)
{
    if (profondeur <= 1) return coups;

    Couleur coulAdv = (coulIA == NOIR) ? BLANC : NOIR;
    Move k1 = (profondeur < 20) ? killers[profondeur][0] : Move();
    Move k2 = (profondeur < 20) ? killers[profondeur][1] : Move();

    std::vector<std::pair<int, Move>> scores;
    scores.reserve(coups.size());

    for (const Move& coup : coups) {
        int col  = coup.getColonne();
        int prof = coup.getProfondeur();
        int priorite = 0;

        // 1. Victoire immédiate IA
        bb.poser(col, prof, coulIA);
        if (bb.estGagne(col, prof, coulIA)) {
            bb.annuler(col, prof, coulIA);
            scores.push_back({200000, coup});
            continue;
        }
        bb.annuler(col, prof, coulIA);

        // 2. Blocage victoire adverse
        bb.poser(col, prof, coulAdv);
        if (bb.estGagne(col, prof, coulAdv)) {
            bb.annuler(col, prof, coulAdv);
            scores.push_back({150000, coup});
            continue;
        }
        bb.annuler(col, prof, coulAdv);

        // 3. Killer moves
        if (coup == k1) { scores.push_back({140000, coup}); continue; }
        if (coup == k2) { scores.push_back({130000, coup}); continue; }

        // 4. TT ou EvaluatorMoyen
        bb.poser(col, prof, coulIA);
        auto it = transpositionTable.find(bb.getHash());
        if (it != transpositionTable.end()) {
            priorite = it->second.score;
        } else {
            priorite = evaluateurMoyen->evaluer(bb, coulIA, joueurSecond);
        }
        bb.annuler(col, prof, coulIA);

        scores.push_back({priorite, coup});
    }

    std::sort(scores.begin(), scores.end(),
              [](const auto& a, const auto& b) { return a.first > b.first; });

    std::vector<Move> coupsTries;
    coupsTries.reserve(scores.size());
    for (auto& paire : scores)
        coupsTries.push_back(paire.second);

    return coupsTries;
}

// ─── chercherAuNiveau ────────────────────────────────────────────────────────

Move AlphaBetaStrategy::chercherAuNiveau(BitBoard& bb, int profondeur, Couleur coulIA) {
    int  meilleurScore = INT_MIN;
    Move meilleur;
    int  alpha = INT_MIN, beta = INT_MAX;

    std::vector<Move> coups = trierCoups(bb, bb.getCoupsPossibles(), coulIA, profondeur);

    for (const Move& coup : coups) {
        if (tempsDepasse()) { timeOut = true; return meilleur; }

        int col  = coup.getColonne();
        int prof = coup.getProfondeur();

        bb.poser(col, prof, coulIA);
        if (bb.estGagne(col, prof, coulIA)) {
            bb.annuler(col, prof, coulIA);
            return coup;
        }

        int score = alphaBeta(bb, profondeur - 1, alpha, beta, false, coulIA);
        bb.annuler(col, prof, coulIA);

        if (score > meilleurScore) {
            meilleurScore = score;
            meilleur      = coup;
        }
        alpha = std::max(alpha, meilleurScore);
    }
    return meilleur;
}

// ─── alphaBeta ───────────────────────────────────────────────────────────────

int AlphaBetaStrategy::alphaBeta(BitBoard& bb, int profondeur,
                                  int alpha, int beta, bool maximise,
                                  Couleur coulIA)
{
    if (tempsDepasse()) { timeOut = true; return 0; }
    if (bb.plateauPlein()) return 0;

    // ── Hash avec trait ───────────────────────────────────────────────────────
    // Les nœuds MAX (tour IA)  → hash normal
    // Les nœuds MIN (tour adv) → hash ^ ZOBRIST_TRAIT
    // Ainsi deux positions identiques mais avec des joueurs différents
    // ont des hashs différents → la TT ne les confond plus.
    u64 hash = bb.getHash();
    if (!maximise) hash ^= BitBoard::getZobristTrait();

    // ── Consultation TT ───────────────────────────────────────────────────────
    auto it = transpositionTable.find(hash);
    if (it != transpositionTable.end()) {
        const EntreeTransposition& e = it->second;
        if (e.profondeur >= profondeur) {
            if (e.type == EntreeTransposition::EXACT)       return e.score;
            if (e.type == EntreeTransposition::LOWER_BOUND) alpha = std::max(alpha, e.score);
            if (e.type == EntreeTransposition::UPPER_BOUND) beta  = std::min(beta,  e.score);
            if (beta <= alpha) return e.score;
        }
    }

    // ── Feuille ───────────────────────────────────────────────────────────────
    if (profondeur == 0)
        return evaluateurComplet->evaluer(bb, coulIA, joueurSecond);

    Couleur coulAdv      = (coulIA == NOIR) ? BLANC : NOIR;
    int     alphaInitial = alpha;

    std::vector<Move> coups = trierCoups(bb, bb.getCoupsPossibles(), coulIA, profondeur);

    if (maximise) {
        // ── Nœud MAX — tour de l'IA ───────────────────────────────────────────
        int  valeur = INT_MIN;
        Move meilleurCoupLocal;

        for (const Move& coup : coups) {
            int col  = coup.getColonne();
            int prof = coup.getProfondeur();

            bb.poser(col, prof, coulIA);
            if (bb.estGagne(col, prof, coulIA)) {
                bb.annuler(col, prof, coulIA);
                return 100000 + profondeur;
            }

            int v = alphaBeta(bb, profondeur - 1, alpha, beta, false, coulIA);
            bb.annuler(col, prof, coulIA);

            if (v > valeur) { valeur = v; meilleurCoupLocal = coup; }
            alpha = std::max(alpha, valeur);

            if (beta <= alpha) {
                enregistrerKiller(coup, profondeur, valeur);
                break;
            }
        }

        // Stockage TT avec hash trait MAX
        EntreeTransposition e;
        e.score      = valeur;
        e.profondeur = profondeur;
        if      (valeur >= beta)         e.type = EntreeTransposition::LOWER_BOUND;
        else if (valeur <= alphaInitial) e.type = EntreeTransposition::UPPER_BOUND;
        else                             e.type = EntreeTransposition::EXACT;
        transpositionTable[hash] = e;
        return valeur;

    } else {
        // ── Nœud MIN — tour de l'adversaire ──────────────────────────────────
        int valeur = INT_MAX;

        for (const Move& coup : coups) {
            int col  = coup.getColonne();
            int prof = coup.getProfondeur();

            bb.poser(col, prof, coulAdv);
            if (bb.estGagne(col, prof, coulAdv)) {
                bb.annuler(col, prof, coulAdv);
                return -(100000 + profondeur);
            }

            int v = alphaBeta(bb, profondeur - 1, alpha, beta, true, coulIA);
            bb.annuler(col, prof, coulAdv);

            if (v < valeur) valeur = v;
            beta = std::min(beta, valeur);
            if (beta <= alpha) break;
        }

        // Stockage TT avec hash trait MIN (hash déjà XORé en haut de la fonction)
        EntreeTransposition e;
        e.score      = valeur;
        e.profondeur = profondeur;
        if      (valeur <= alphaInitial) e.type = EntreeTransposition::UPPER_BOUND;
        else if (valeur >= beta)         e.type = EntreeTransposition::LOWER_BOUND;
        else                             e.type = EntreeTransposition::EXACT;
        transpositionTable[hash] = e;
        return valeur;
    }
}
