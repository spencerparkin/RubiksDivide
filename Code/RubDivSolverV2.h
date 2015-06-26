// RubDivSolverV2.h

#pragma once

#include <list>
#include <map>

#include "RubDivPuzzle.h"
#include "RubDivSolver.h"

class RubDivSolverV2 : public RubDivSolver
{
public:

	RubDivSolverV2( void );
	virtual ~RubDivSolverV2( void );

	virtual bool Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList ) override;

private:

	bool FindAlgorithm( RubDivPuzzle::MoveList& algorithm );

	RubDivPuzzle* puzzleClone;
};

// RubDivSolverV2.h