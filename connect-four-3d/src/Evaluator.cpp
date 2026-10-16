/**
 * @file Evaluator.cpp
 * @brief Implémentation des évaluateurs avec asymétrie défensive renforcée.
 */
#include "Evaluator.h"

// ─── popcount128 ─────────────────────────────────────────────────────────────

int Evaluator::popcount128(u128 x) {
    return __builtin_popcountll((uint64_t)x) +
           __builtin_popcountll((uint64_t)(x >> 64));
}

// ─── scoreAlignementsMasques ─────────────────────────────────────────────────

int Evaluator::scoreAlignementsMasques(const BitBoard& bb, Couleur coulIA, bool joueurSecond) {
    static const int bonusCol [4] = {0, 3, 3, 0};
    static const int bonusProf[5] = {0, 2, 3, 2, 0};
    static const int bonusNiv [4] = {1, 3, 3, 1};

    int score = 0;
    Couleur coulAdv = (coulIA == NOIR) ? BLANC : NOIR;

    for (int c = 0; c < 4; c++)
    for (int p = 0; p < 5; p++)
    for (int n = 0; n < 4; n++) {
        int idx   = c * 20 + p * 4 + n;
        int bonus = bonusCol[c] + bonusProf[p] + bonusNiv[n];
        if (bb.getBit(idx, coulIA))  score += bonus;
        if (bb.getBit(idx, coulAdv)) score -= bonus;
    }

    u128 bitsIA  = (coulIA == NOIR) ? bb.getNoir()  : bb.getBlanc();
    u128 bitsAdv = (coulIA == NOIR) ? bb.getBlanc() : bb.getNoir();

    // Malus adverses renforcés quand NOIR pour détecter tôt les menaces
    int malus2 = joueurSecond ? 50  : 30;
    int malus3 = joueurSecond ? 700 : 500;

    int nb = BitBoard::nbMasques();
    for (int m = 0; m < nb; m++) {
        u128 masque    = BitBoard::getMasque(m);
        u128 ia_match  = bitsIA  & masque;
        u128 adv_match = bitsAdv & masque;

        if (ia_match && adv_match) continue;

        if (ia_match) {
            int cnt = popcount128(ia_match);
            if      (cnt == 3) score += 500;
            else if (cnt == 2) score +=  30;
            else if (cnt == 1) score +=   1;
        }
        if (adv_match) {
            int cnt = popcount128(adv_match);
            if      (cnt == 3) score -= malus3;
            else if (cnt == 2) score -= malus2;
            else if (cnt == 1) score -=   1;
        }
    }

    return score;
}

// ─── scoreFourchettes ────────────────────────────────────────────────────────

int Evaluator::scoreFourchettes(const BitBoard& bb, Couleur coulIA, bool joueurSecond) {
    int scoreNet = 0;

    u128 bitsIA   = (coulIA == NOIR) ? bb.getNoir()  : bb.getBlanc();
    u128 bitsAdv  = (coulIA == NOIR) ? bb.getBlanc() : bb.getNoir();
    u128 occupees = bitsIA | bitsAdv;

    static const int OFF_3   = 4000;
    static const int OFF_2_3 = 2000;
    static const int OFF_2_2 =  800;

    int DEF_3, DEF_2_3, DEF_2_2;
    if (joueurSecond) {
        DEF_3   = 5600;
        DEF_2_3 = 4200;
        DEF_2_2 = 2000;
    } else {
        DEF_3   = 4000;
        DEF_2_3 = 2000;
        DEF_2_2 =  800;
    }

    for (int c = 0; c < 4; c++)
    for (int p = 0; p < 5; p++) {
        int niv = bb.getNiveauLibre(c, p);
        if (niv >= 4) continue;

        int idx = BitBoard::indice(c, p, niv);
        if ((occupees >> idx) & 1) continue;

        int menaces3_IA  = 0, menaces2_IA  = 0;
        int menaces3_Adv = 0, menaces2_Adv = 0;

        for (int m : BitBoard::getMasquesPourCase(idx)) {
            u128 masque    = BitBoard::getMasque(m);
            u128 ia_match  = bitsIA  & masque;
            u128 adv_match = bitsAdv & masque;

            if (!adv_match) {
                int cnt = popcount128(ia_match);
                if      (cnt == 3) menaces3_IA++;
                else if (cnt == 2) menaces2_IA++;
            }
            if (!ia_match) {
                int cnt = popcount128(adv_match);
                if      (cnt == 3) menaces3_Adv++;
                else if (cnt == 2) menaces2_Adv++;
            }
        }

        if      (menaces3_IA >= 1)  scoreNet += OFF_3;
        else if (menaces2_IA >= 3)  scoreNet += OFF_2_3;
        else if (menaces2_IA >= 2)  scoreNet += OFF_2_2;

        if      (menaces3_Adv >= 1)  scoreNet -= DEF_3;
        else if (menaces2_Adv >= 3)  scoreNet -= DEF_2_3;
        else if (menaces2_Adv >= 2)  scoreNet -= DEF_2_2;
    }

    return scoreNet;
}

// ─── Forme canonique Evaluator ───────────────────────────────────────────────

Evaluator::Evaluator() {}
Evaluator::Evaluator(const Evaluator&) {}
Evaluator& Evaluator::operator=(const Evaluator&) { return *this; }
Evaluator::~Evaluator() {}

// ─── EvaluatorAvance ─────────────────────────────────────────────────────────

EvaluatorAvance::EvaluatorAvance() {}
EvaluatorAvance::EvaluatorAvance(const EvaluatorAvance& other) : Evaluator(other) {}
EvaluatorAvance& EvaluatorAvance::operator=(const EvaluatorAvance& other) {
    Evaluator::operator=(other);
    return *this;
}
EvaluatorAvance::~EvaluatorAvance() {}

int EvaluatorAvance::evaluer(const BitBoard& bb, Couleur coulIA, bool joueurSecond) const {
    return scoreAlignementsMasques(bb, coulIA, joueurSecond)
         + scoreFourchettes(bb, coulIA, joueurSecond);
}

// ─── EvaluatorMoyen ──────────────────────────────────────────────────────────

EvaluatorMoyen::EvaluatorMoyen() {}
EvaluatorMoyen::EvaluatorMoyen(const EvaluatorMoyen& other) : Evaluator(other) {}
EvaluatorMoyen& EvaluatorMoyen::operator=(const EvaluatorMoyen& other) {
    Evaluator::operator=(other);
    return *this;
}
EvaluatorMoyen::~EvaluatorMoyen() {}

int EvaluatorMoyen::evaluer(const BitBoard& bb, Couleur coulIA, bool joueurSecond) const {
    (void)joueurSecond;
    return scoreAlignementsMasques(bb, coulIA, false);
}
