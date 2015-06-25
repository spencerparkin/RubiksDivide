// RubDivSolver.h

#pragma once

#include <list>

#include "RubDivPuzzle.h"

class RubDivSolver
{
public:

	RubDivSolver( void );
	~RubDivSolver( void );

	// Return a list of moves that solves the given puzzle.
	bool Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList );

private:

	bool FindAlgorithm( RubDivPuzzle::MoveList& algorithm );
	bool FindAlgorithmForRow( int row, const RubDivPuzzle::Move& rotationMove, RubDivPuzzle::MoveList& algorithm );
	void ConcatinateMoveList( RubDivPuzzle::MoveList& moveListDest, const RubDivPuzzle::MoveList& moveListSource );
	void CompressMoveList( RubDivPuzzle::MoveList& moveList );

	RubDivPuzzle* puzzleClone;
};

// RubDivSolver.h