# ─── Makefile — Puissance 4 3D ────────────────────────────────────────────────
# Faire "source /opt/intel/oneapi/setvars.sh" avant de compiler sur X2go

CC       = mpicxx
CXX      = g++
CXXFLAGS = -std=c++17 -O2 -Wall

MOTEUR = Board.cpp BitBoard.cpp Cell.cpp Move.cpp \
         Player.cpp AIPlayer.cpp \
         Strategy.cpp \
         AlphaBetaStrategy.cpp Evaluator.cpp

SRC_MPI   = TestJeu.cpp position.cpp arbitre.cpp joueur.cpp $(MOTEUR)
SRC_LOCAL = main.cpp $(MOTEUR) Game.cpp

# ── Tournoi MPI ───────────────────────────────────────────────────────────────
exe: exe1 exe2 exe3

exe1: $(SRC_MPI)
	$(CC) $(CXXFLAGS) -o exe1 $(SRC_MPI)

exe2: $(SRC_MPI)
	$(CC) $(CXXFLAGS) -o exe2 $(SRC_MPI)

exe3: $(SRC_MPI)
	$(CC) $(CXXFLAGS) -o exe3 $(SRC_MPI)

run: exe
	mpirun -n 1 ./exe1 : -n 1 ./exe2 : -n 1 ./exe3

run-local: exe
	mpirun --allow-run-as-root --oversubscribe -n 1 ./exe1 : -n 1 ./exe2 : -n 1 ./exe3

# ── Mode local humain vs IA ───────────────────────────────────────────────────
p4: $(SRC_LOCAL)
	$(CXX) $(CXXFLAGS) -o p4 $(SRC_LOCAL)

clean:
	rm -f *.o exe1 exe2 exe3 p4

.PHONY: exe run run-local p4 clean
