// RubDivSolver.h

#pragma once

#include <list>

#include "RubDivPuzzle.h"

// With what I've learned writing the solver derivatives, I can see an easy way of solving all faces of the NxNxN Rubik's cube.
class RubDivSolver
{
public:

	RubDivSolver( void );
	virtual ~RubDivSolver( void );

	// Return a list of moves that solves the given puzzle.
	virtual bool Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList ) = 0;

protected:

	static void ConcatinateMoveList( RubDivPuzzle::MoveList& moveListDest, const RubDivPuzzle::MoveList& moveListSource );
	static void CompressMoveList( RubDivPuzzle::MoveList& moveList );
};

// RubDivSolver.h