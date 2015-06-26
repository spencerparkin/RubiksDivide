// RubDivSolver.cpp

#include "RubDivSolver.h"
#include "RubDivPuzzle.h"

RubDivSolver::RubDivSolver( void )
{
}

/*virtual*/ RubDivSolver::~RubDivSolver( void )
{
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

void RubDivSolver::CompressMoveList( RubDivPuzzle::MoveList& moveList )
{
	//...
}

// RubDivSolver.cpp