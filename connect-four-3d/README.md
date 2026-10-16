# Connect Four 3D: C++ Game Engine with Alpha-Beta AI

A three-dimensional Connect Four game in modern C++ (C++17), with an AI player built on alpha-beta search and a 128-bit bitboard.

Team project, Object-Oriented Programming in C++ course, 2025–2026 (team of 4).

## The game
- The board has **4 columns × 5 depths × 4 levels**, so **80 cells** in total.
- Pieces stack vertically, as in classic Connect Four.
- The goal is to align 4 pieces along any line in 3D space.

## Highlights

**AI engine**
- **128-bit bitboard:** each position is stored as two 128-bit integers, so detecting a win takes a few bitwise tests on precomputed masks (O(1)).
- **Alpha-beta search with iterative deepening:** the AI searches depth 1, 2, … up to 10 under a time budget of about 7.5 seconds per move.
- **Zobrist transposition table:** avoids re-evaluating positions the AI has already seen.
- **Move ordering and killer moves:** the most promising moves are explored first, so alpha-beta prunes more.
- **Asymmetric evaluation:** an offensive evaluator for White and a defensive one for Black.

**Software design**
- **Strategy pattern:** the search algorithm (`Strategy`) and the scoring function (`Evaluator`) are interchangeable.
- **Clean class hierarchy:** abstract `Player` with an `AIPlayer` subclass, and `Board` / `BitBoard` / `Cell` / `Move` for the game state.
- **Rule of three, encapsulation and Doxygen documentation** for every class.

## Build and run

Requires `g++` with C++17 support.

```bash
cd src
make p4      # build the local version
./p4         # AI vs AI game in the terminal
```

The program prints the board after every move, along with the search depth the AI reached.

```
           Prof.0      Prof.1      Prof.2      Prof.3      Prof.4
 Niv 3 | . . . .  | . . . .  | . . . .  | . . . .  | . . . .  |
 ...
IA_Blanc reflechit...
  profondeur 7 OK
  (arrete a profondeur 8)
IA_Blanc joue : colonne B, profondeur 2
```

> The `exe` / `run` targets build the MPI tournament version used to pit teams' AIs against each other. They need an MPI compiler (`mpicxx`) and the tournament harness file `TestJeu.cpp`, which the course provided and which is not included here. The files `arbitre.*`, `joueur.*` and `position.*` are part of that course-provided harness.

## Project structure

```
.
├── src/
│   ├── main.cpp                    # Entry point (AI vs AI)
│   ├── Game.*                      # Game loop: display → move → play → win check
│   ├── Board.*, BitBoard.*         # Board state and 128-bit bitboard
│   ├── Cell.*, Move.*, Couleur.h   # Basic types
│   ├── Player.*, AIPlayer.*        # Player hierarchy
│   ├── Strategy.*                  # Abstract search strategy
│   ├── AlphaBetaStrategy.*         # Alpha-beta + iterative deepening + Zobrist TT
│   ├── Evaluator.*                 # Position scoring functions
│   └── Makefile
└── docs/
    ├── final-report.pdf            # Final project report (French)
    ├── midterm-report-2026-05-02.pdf
    ├── progress-update-2026-05-25.pdf
    ├── doxygen-documentation.pdf
    └── doxygen-html.zip            # HTML API documentation
```

## Team
| Member | Role |
|---|---|
| Youssef Rais | Code management |
| Moussa | Testing and AI strategy development |
| Maher | LaTeX report, co-developer |
| Mohamed | Doxygen documentation, co-developer |

Code comments and reports are in French.
