// RubDivSolverV1.h

#pragma once

#include <list>

#include "RubDivPuzzle.h"
#include "RubDivSolver.h"

class RubDivSolverV1 : public RubDivSolver
{
public:

	RubDivSolverV1( void );
	virtual ~RubDivSolverV1( void );

	virtual bool Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList ) override;

private:

	bool FindAlgorithm( RubDivPuzzle::MoveList& algorithm );
	bool FindAlgorithmForRow( int row, const RubDivPuzzle::Move& rotationMove, RubDivPuzzle::MoveList& algorithm );

	RubDivPuzzle* puzzleClone;
};

// RubDivSolverV1.h