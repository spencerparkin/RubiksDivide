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

	// TODO: We might consider doing some simple column exchanges before-hand based on counts, but this would
	//       only be an optimization and would not be necessary to the correctness of our algorithm here.

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
	AlgorithmComponents algorithmComponentsCCW( true ), algorithmComponentsCW( false );

	int size = puzzleClone->GetSize();

	// Since we scan in this order, we probably don't necessarily find the best algorithm.
	for( int j = 0; j < size; j++ )
	{
		RubDivPuzzle::Move shiftMove;
		shiftMove.squareOffset = -1;
		shiftMove.rotateCCWCount = 0;
		shiftMove.rowOrColumn = j;
		shiftMove.shiftDir = RubDivPuzzle::SHIFT_BACKWARD;

		algorithmComponentsCCW.AddShiftIfPossible( shiftMove, puzzleClone );
		algorithmComponentsCW.AddShiftIfPossible( shiftMove, puzzleClone );
	}

	RubDivPuzzle::MoveList algorithmCCW, algorithmCW;

	bool constructedCCW = algorithmComponentsCCW.ConstructAlgorithm( algorithmCCW );
	bool constructedCW = algorithmComponentsCW.ConstructAlgorithm( algorithmCW );

	if( constructedCCW && constructedCW )
	{
		if( algorithmComponentsCCW.ColorMoveCount() > algorithmComponentsCW.ColorMoveCount() )
			ConcatinateMoveList( algorithm, algorithmCCW );
		else
			ConcatinateMoveList( algorithm, algorithmCW );
	}
	else if( constructedCCW )
		ConcatinateMoveList( algorithm, algorithmCCW );
	else if( constructedCW )
		ConcatinateMoveList( algorithm, algorithmCW );

	return( algorithm.size() > 0 ) ? true : false;
}

RubDivSolverV2::AlgorithmComponents::AlgorithmComponents( bool ccw )
{
	rotationMove.squareOffset = 1;
	rotationMove.rotateCCWCount = ccw ? 1 : 3;
	rotationMove.rowOrColumn = -1;
	rotationMove.shiftDir = RubDivPuzzle::SHIFT_NONE;

	rotationMove.Invert( inverseRotationMove );

	colorMoveCount = 0;
}

RubDivSolverV2::AlgorithmComponents::~AlgorithmComponents( void )
{
}

bool RubDivSolverV2::AlgorithmComponents::ConstructAlgorithm( RubDivPuzzle::MoveList& algorithm )
{
	if( shiftMoveList.size() == 0 )
		return false;

	if( shiftMoveList.size() > 1 && preservationMoveList.size() > 1 )
	{
		int b = 0;
	}

	algorithm.clear();
	ConcatinateMoveList( algorithm, shiftMoveList );
	algorithm.push_back( rotationMove );
	ConcatinateMoveList( algorithm, preservationMoveList );
	algorithm.push_back( inverseRotationMove );
	ConcatinateMoveList( algorithm, inverseShiftMoveList );
	algorithm.push_back( rotationMove );
	ConcatinateMoveList( algorithm, inversePreservationMoveList );
	algorithm.push_back( inverseRotationMove );
	return true;
}

bool RubDivSolverV2::AlgorithmComponents::ShiftHelpsToCompleteRow( const RubDivPuzzle::Move& shiftMove, int row, const RubDivPuzzle* puzzle )
{
	wxASSERT( shiftMove.shiftDir == RubDivPuzzle::SHIFT_BACKWARD );
	if( puzzle->GetColor( 1, row, shiftMove.rowOrColumn ) == RubDivPuzzle::Element::COLOR_B && puzzle->GetColor( 2, row, shiftMove.rowOrColumn ) == RubDivPuzzle::Element::COLOR_A )
		return true;
	return false;
}

void RubDivSolverV2::AlgorithmComponents::AddShiftIfPossible( const RubDivPuzzle::Move& shiftMove, const RubDivPuzzle* puzzle )
{
	// If the column is already in play, then we must reject the move.
	ColumnMap::iterator iter = columnMap.find( shiftMove.rowOrColumn );
	if( iter != columnMap.end() )
		return;

	int size = puzzle->GetSize();

	// Look at everything we're already preserving and make sure we're still preserving it with the new shift.
	RubDivPuzzle::MoveList::iterator moveIter = preservationMoveList.begin();
	while( moveIter != preservationMoveList.end() )
	{
		const RubDivPuzzle::Move& preservationMove = *moveIter;
		int row = ( rotationMove.rotateCCWCount == 1 ) ? preservationMove.rowOrColumn : ( size - 1 - preservationMove.rowOrColumn );
		if( !ShiftHelpsToCompleteRow( shiftMove, row, puzzle ) )
			break;
		moveIter++;
	}
	if( moveIter != preservationMoveList.end() )
		return;

	std::list< int > preservationColumns;
	for( int i = 0; i < size; i++ )
	{
		if( !ShiftHelpsToCompleteRow( shiftMove, i, puzzle ) )
			continue;
		
		int j = ( rotationMove.rotateCCWCount == 1 ) ? i : ( size - 1 - i );
		iter = columnMap.find( j );
		if( iter != columnMap.end() || j == shiftMove.rowOrColumn )
			continue;

		preservationColumns.push_back( j );
	}

	if( preservationColumns.size() == 0 )
		return;

	// We can add the shift.  Do it.
	columnMap[ shiftMove.rowOrColumn ] = 0;
	shiftMoveList.push_back( shiftMove );
	RubDivPuzzle::Move inverseShiftMove;
	shiftMove.Invert( inverseShiftMove );
	inverseShiftMoveList.push_back( inverseShiftMove );

	for( std::list< int >::iterator iter = preservationColumns.begin(); iter != preservationColumns.end(); iter++ )
	{
		int j = *iter;
		columnMap[j] = 0;

		RubDivPuzzle::Move preservationMove;
		preservationMove.squareOffset = -1;
		preservationMove.rotateCCWCount = 0;
		preservationMove.rowOrColumn = j;
		preservationMove.shiftDir = RubDivPuzzle::SHIFT_BACKWARD;
		preservationMoveList.push_back( preservationMove );

		RubDivPuzzle::Move inversePreservationMove;
		preservationMove.Invert( inversePreservationMove );
		inversePreservationMoveList.push_back( inversePreservationMove );
	}

	colorMoveCount += preservationColumns.size();
}

// RubDivSolverV2.cpp