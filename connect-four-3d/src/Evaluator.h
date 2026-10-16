#pragma once
#include <cstdint>
#include "BitBoard.h"
#include "Couleur.h"

/**
 * @file Evaluator.h
 * @brief Évaluateurs de position pour l'IA.
 *
 * CONVENTION : score > 0 = bon pour coulIA, score < 0 = mauvais.
 * ASYMÉTRIE : joueurSecond=true (NOIR) → poids défensifs renforcés.
 *
 * ENCAPSULATION :
 *   popcount128, scoreAlignementsMasques, scoreFourchettes sont
 *   méthodes statiques privées de Evaluator — détails internes cachés.
 */
class Evaluator {
public:
    Evaluator();
    Evaluator(const Evaluator& other);
    Evaluator& operator=(const Evaluator& other);
    virtual ~Evaluator();

    virtual int evaluer(const BitBoard& bb, Couleur coulIA, bool joueurSecond) const = 0;

protected:
    static int scoreAlignementsMasques(const BitBoard& bb, Couleur coulIA, bool joueurSecond);
    static int scoreFourchettes(const BitBoard& bb, Couleur coulIA, bool joueurSecond);

private:
    static int popcount128(u128 x);
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Évaluateur complet — feuilles de l'arbre alpha-bêta.
 * Alignements + fourchettes asymétriques (~10 µs par appel).
 */
class EvaluatorAvance : public Evaluator {
public:
    EvaluatorAvance();
    EvaluatorAvance(const EvaluatorAvance& other);
    EvaluatorAvance& operator=(const EvaluatorAvance& other);
    ~EvaluatorAvance();

    int evaluer(const BitBoard& bb, Couleur coulIA, bool joueurSecond) const override;
};

// ─────────────────────────────────────────────────────────────────────────────

/**
 * @brief Évaluateur léger — Move Ordering uniquement.
 * Alignements sans fourchettes (~0.4 µs par appel).
 */
class EvaluatorMoyen : public Evaluator {
public:
    EvaluatorMoyen();
    EvaluatorMoyen(const EvaluatorMoyen& other);
    EvaluatorMoyen& operator=(const EvaluatorMoyen& other);
    ~EvaluatorMoyen();

    int evaluer(const BitBoard& bb, Couleur coulIA, bool joueurSecond) const override;
};
