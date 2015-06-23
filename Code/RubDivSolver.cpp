// RubDivSolver.cpp

#include "RubDivSolver.h"
#include "RubDivPuzzle.h"

RubDivSolver::RubDivSolver( void )
{
	//puzzleClone = 0;
}

RubDivSolver::~RubDivSolver( void )
{
}

#if 0

bool RubDivSolver::Solve( const RubDivPuzzle* puzzle, MoveList& moveList )
{
	if( puzzle->GetOrientation() != RubDivPuzzle::VERTICAL )
		return false;

	moveList.clear();

	puzzleClone = puzzle->Clone();

	while( !puzzleClone->IsSolved() )
	{
		if( !FindCommutator( moveList ) )
		{
			// Bail for now, but there may be something we need to do here.
			break;
		}
	}

	delete puzzleClone;
	puzzleClone = 0;
	
	return true;
}

bool RubDivSolver::FindCommutator( MoveList& moveList )
{
	int size = puzzleClone->GetSize();

	for( int i = 0; i < size; i++ )
	{
		for( int j = 0; j < size; j++ )
		{
			if( puzzleClone->GetColor( 1, i, j ) == RubDivPuzzle::Element::COLOR_B && puzzleClone->GetColor( 2, i, j ) == RubDivPuzzle::Element::COLOR_A )
			{
				AppendCommutator( moveList, i, j );
				return true;
			}
		}
	}

	return false;
}

void RubDivSolver::AppendCommutator( MoveList& moveList, int i, int j )
{
	Move shiftMove, unshiftMove;
	Move rotateMove, unrotateMove;
	Move preserveMove, unpreserveMove;

	shiftMove.squareOffset = -1;
	shiftMove.rotateCCWCount = 0;
	shiftMove.columnOffset = j;
	shiftMove.shiftDir = SHIFT_FORWARD;
	moveList.push_back( shiftMove );

	if( i == j )
	{
	}
	else
	{
		rotateMove.squareOffset = 1;
		rotateMove.rotateCCWCount = 1;
		rotateMove.columnOffset = -1;
		rotateMove.shiftDir = SHIFT_NONE;
		moveList.push_back( rotateMove );
	}

	preserveMove.squareOffset = -1;
	preserveMove.rotateCCWCount = 0;
	preserveMove.columnOffset = i;
	preserveMove.shiftDir = SHIFT_FORWARD;
	moveList.push_back( preserveMove );

	shiftMove.Invert( unshiftMove );
	rotateMove.Invert( unrotateMove );
	preserveMove.Invert( unrotateMove );

	moveList.push_back( unrotateMove );
	moveList.push_back( unshiftMove );
	moveList.push_back( rotateMove );
	moveList.push_back( unpreserveMove );
	moveList.push_back( unrotateMove );
}

#endif

// RubDivSolver.cpp