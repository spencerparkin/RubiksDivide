// RubDivApp.cpp

#include "RubDivApp.h"
#include "RubDivPuzzle.h"
#include "RubDivFrame.h"

wxIMPLEMENT_APP( RubDivApp );

RubDivApp::RubDivApp( void )
{
	puzzle = 0;
	frame = 0;

	historyIter = moveHistory.end();
}

/*virtual*/ RubDivApp::~RubDivApp( void )
{
	SetPuzzle(0);
}

/*virtual*/ bool RubDivApp::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	frame = new RubDivFrame(0);
	frame->Show();

	return true;
}

/*virtual*/ int RubDivApp::OnExit( void )
{
	return wxApp::OnExit();
}

RubDivPuzzle* RubDivApp::GetPuzzle( void )
{
	return puzzle;
}

void RubDivApp::SetPuzzle( RubDivPuzzle* puzzle )
{
	if( this->puzzle )
		delete this->puzzle;

	this->puzzle = puzzle;

	moveHistory.clear();
	moveQueue.clear();
	historyIter = moveHistory.end();
}

void RubDivApp::EnqueueMove( const RubDivPuzzle::Move& move )
{
	moveQueue.push_back( move );
}

void RubDivApp::EnqueueMoveList( const RubDivPuzzle::MoveList& moveList )
{
	RubDivPuzzle::MoveList::const_iterator iter = moveList.begin();
	while( iter != moveList.end() )
	{
		EnqueueMove( *iter );
		iter++;
	}
}

bool RubDivApp::ProcessMoveQueue( RubDivPuzzle::RenderData& renderData )
{
	if( !puzzle )
		return false;
			
	RubDivPuzzle::MoveList::iterator iter = moveQueue.begin();
	if( iter != moveQueue.end() )
	{
		RubDivPuzzle::Move move = *iter;
		if( puzzle->ManipulatePuzzle( move, &renderData ) )
		{
			moveQueue.erase( iter );
			return true;
		}
	}

	return false;
}

void RubDivApp::AddHistory( const RubDivPuzzle::Move& move )
{
	if( moveHistory.size() > 0 && historyIter == moveHistory.end() )
		moveHistory.clear();
	else
	{
		while( true )
		{
			RubDivPuzzle::MoveList::iterator iter = historyIter;
			if( iter == moveHistory.end() )
				break;
			
			iter++;
			if( iter == moveHistory.end() )
				break;

			moveHistory.erase( iter );
		}
	}
	
	moveHistory.push_back( move );

	if( historyIter == moveHistory.end() )
		historyIter = moveHistory.begin();
	else
		historyIter++;

	RubDivPuzzle::MoveList::iterator iter = historyIter;
	iter++;
	wxASSERT( iter == moveHistory.end() );
}

bool RubDivApp::GetMoveForUndo( RubDivPuzzle::Move& move )
{
	if( !CanUndo() )
		return false;

	RubDivPuzzle::Move inverseMove = *historyIter;
	inverseMove.Invert( move );
	if( historyIter == moveHistory.begin() )
		historyIter = moveHistory.end();
	else
		historyIter--;
	return true;
}

bool RubDivApp::GetMoveForRedo( RubDivPuzzle::Move& move )
{
	if( !CanRedo() )
		return false;

	if( historyIter == moveHistory.end() )
		historyIter = moveHistory.begin();
	else
		historyIter++;
	move = *historyIter;
	return true;
}

bool RubDivApp::CanUndo( void )
{
	return( historyIter == moveHistory.end() ) ? false : true;
}

bool RubDivApp::CanRedo( void )
{
	RubDivPuzzle::MoveList::iterator iter = historyIter;
	if( iter == moveHistory.end() )
		iter = moveHistory.begin();
	else
		iter++;

	return( iter == moveHistory.end() ) ? false : true;
}

// RubDivApp.cpp