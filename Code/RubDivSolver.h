// RubDivSolver.h

#pragma once

#include <list>

#include "RubDivPuzzle.h"

class RubDivSolver
{
public:

	RubDivSolver( void );
	virtual ~RubDivSolver( void );

	// Return a list of moves that solves the given puzzle.
	virtual bool Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList ) = 0;

protected:

	void ConcatinateMoveList( RubDivPuzzle::MoveList& moveListDest, const RubDivPuzzle::MoveList& moveListSource );
	void CompressMoveList( RubDivPuzzle::MoveList& moveList );
};

// RubDivSolver.h