// RubDivSolver.h

#pragma once

#include <list>

class RubDivPuzzle;

class RubDivSolver
{
public:

	RubDivSolver( void );
	~RubDivSolver( void );

	struct Move
	{
		int squareOffset;
		int rotateCCWCount;
		int columnOffset;
		int shiftDirection;
	};

	typedef std::list< Move > MoveList;

	bool Solve( const RubDivPuzzle* puzzle, MoveList& moveList );
};

// RubDivSolver.h