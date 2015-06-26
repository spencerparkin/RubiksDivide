// RubDivSolverV2.cpp

#include "RubDivSolverV2.h"

RubDivSolverV2::RubDivSolverV2( void )
{
	puzzleClone = 0;
}

/*virtual*/ RubDivSolverV2::~RubDivSolverV2( void )
{
	wxASSERT( puzzleClone == 0 );
}

/*virtual*/ bool RubDivSolverV2::Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList )
{
	wxASSERT( puzzleClone == 0 );

	// We only deal with the vertical case, which is really no different than the horizontal case.
	if( puzzle->GetOrientation() != RubDivPuzzle::VERTICAL )
		return false;

	moveList.clear();
	puzzleClone = puzzle->Clone();
	while( !puzzleClone->IsSolved() )
	{
		RubDivPuzzle::MoveList algorithm;
		if( FindAlgorithm( algorithm ) )
		{
			bool manipulated = puzzleClone->ManipulatePuzzle( algorithm );
			wxASSERT( manipulated );
			ConcatinateMoveList( moveList, algorithm );
		}
		else
		{
			RubDivPuzzle::Move rotateMove;
			rotateMove.squareOffset = 2;
			rotateMove.rotateCCWCount = 1;
			rotateMove.rowOrColumn = -1;
			rotateMove.shiftDir = RubDivPuzzle::SHIFT_NONE;

			bool manipulated = puzzleClone->ManipulatePuzzle( rotateMove );
			wxASSERT( manipulated );
			moveList.push_back( rotateMove );
		}
	}

	delete puzzleClone;
	puzzleClone = 0;

	CompressMoveList( moveList );
	
	return true;
}

bool RubDivSolverV2::FindAlgorithm( RubDivPuzzle::MoveList& algorithm )
{
	//...
	// TODO: A shift generates one or more preservation shifts based on the rotation we're going to consider.
	//       We can keep adding shifts and preservation shifts as long as their are no collisions between any of these.
	//       Also, we may be able to do some simple column exchanges before-hand based on counts, but this is optional.
	return false;
}

// RubDivSolverV2.cpp