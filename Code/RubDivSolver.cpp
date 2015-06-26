// RubDivSolver.cpp

#include "RubDivSolver.h"
#include "RubDivPuzzle.h"

RubDivSolver::RubDivSolver( void )
{
	puzzleClone = 0;
}

RubDivSolver::~RubDivSolver( void )
{
}

bool RubDivSolver::Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList )
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

void RubDivSolver::ConcatinateMoveList( RubDivPuzzle::MoveList& moveListDest, const RubDivPuzzle::MoveList& moveListSource )
{
	RubDivPuzzle::MoveList::const_iterator iter = moveListSource.begin();
	while( iter != moveListSource.end() )
	{
		const RubDivPuzzle::Move& move = *iter;
		moveListDest.push_back( move );
		iter++;
	}
}

// Note that this could generalize even further by solving multiple rows at the same time and using multiple preservation columns.
// As long as there's no collision between the columns we solve in the rows and the preservation columns, it works out.
bool RubDivSolver::FindAlgorithm( RubDivPuzzle::MoveList& algorithm )
{
	int size = puzzleClone->GetSize();

	for( int i = 0; i < size; i++ )
	{
		RubDivPuzzle::MoveList algorithmCCW, algorithmCW;

		RubDivPuzzle::Move rotationMove;
		rotationMove.squareOffset = 1;
		rotationMove.rowOrColumn = -1;
		rotationMove.shiftDir = RubDivPuzzle::SHIFT_NONE;

		rotationMove.rotateCCWCount = 1;
		bool foundCCW = FindAlgorithmForRow( i, rotationMove, algorithmCCW );
			
		rotationMove.rotateCCWCount = 3;
		bool foundCW = FindAlgorithmForRow( i, rotationMove, algorithmCW );
		
		if( foundCCW && foundCW )
		{
			if( algorithmCCW.size() > algorithmCW.size() )
				ConcatinateMoveList( algorithm, algorithmCCW );
			else
				ConcatinateMoveList( algorithm, algorithmCW );
		}
		else if( foundCCW )
			ConcatinateMoveList( algorithm, algorithmCCW );
		else if( foundCW )
			ConcatinateMoveList( algorithm, algorithmCW );

		if( algorithm.size() > 0 )
			return true;
	}

	return false;
}

// Notice that for puzzles with an odd size, the centers cannot move.
// So to avoid this special case, we always restore color A to the top
// square, and color B to the bottom square.
bool RubDivSolver::FindAlgorithmForRow( int row, const RubDivPuzzle::Move& rotationMove, RubDivPuzzle::MoveList& algorithm )
{
	int size = puzzleClone->GetSize();

	int preservationColumn;
	if( rotationMove.rotateCCWCount == 1 )
		preservationColumn = row;
	else if( rotationMove.rotateCCWCount == 3 )
		preservationColumn = size - 1 - row;
	else
		return false;

	RubDivPuzzle::MoveList shiftMoveList;
	RubDivPuzzle::MoveList inverseShiftMoveList;

	for( int j = 0; j < size; j++ )
	{
		if( j == preservationColumn )
			continue;

		if( puzzleClone->GetColor( 1, row, j ) == RubDivPuzzle::Element::COLOR_B && puzzleClone->GetColor( 2, row, j ) == RubDivPuzzle::Element::COLOR_A )
		{
			RubDivPuzzle::Move shiftMove;
			shiftMove.squareOffset = -1;
			shiftMove.rotateCCWCount = 0;
			shiftMove.rowOrColumn = j;
			shiftMove.shiftDir = RubDivPuzzle::SHIFT_BACKWARD;
			shiftMoveList.push_back( shiftMove );

			RubDivPuzzle::Move inverseShiftMove;
			shiftMove.Invert( inverseShiftMove );
			inverseShiftMoveList.push_back( inverseShiftMove );
		}
	}

	if( shiftMoveList.size() == 0 )
		return false;

	ConcatinateMoveList( algorithm, shiftMoveList );

	algorithm.push_back( rotationMove );

	RubDivPuzzle::Move preservationShiftMove;
	preservationShiftMove.squareOffset = -1;
	preservationShiftMove.rotateCCWCount = 0;
	preservationShiftMove.rowOrColumn = preservationColumn;
	preservationShiftMove.shiftDir = RubDivPuzzle::SHIFT_BACKWARD;
	algorithm.push_back( preservationShiftMove );

	RubDivPuzzle::Move inverseRotationMove;
	rotationMove.Invert( inverseRotationMove );
	algorithm.push_back( inverseRotationMove );

	ConcatinateMoveList( algorithm, inverseShiftMoveList );

	algorithm.push_back( rotationMove );

	RubDivPuzzle::Move inversePreservationShiftMove;
	preservationShiftMove.Invert( inversePreservationShiftMove );
	algorithm.push_back( inversePreservationShiftMove );

	algorithm.push_back( inverseRotationMove );

	return true;
}

void RubDivSolver::CompressMoveList( RubDivPuzzle::MoveList& moveList )
{
	//...
}

// RubDivSolver.cpp