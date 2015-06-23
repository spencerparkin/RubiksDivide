// RubDivApp.h

#pragma once

#include "RubDivPuzzle.h"

#include <wx/app.h>

class RubDivPuzzle;
class RubDivFrame;

class RubDivApp : public wxApp
{
public:

	RubDivApp( void );
	virtual ~RubDivApp( void );

	virtual bool OnInit( void ) override;
	virtual int OnExit( void ) override;

	RubDivPuzzle* GetPuzzle( void );
	void SetPuzzle( RubDivPuzzle* puzzle );

	RubDivFrame* GetFrame( void ) { return frame; }

	void EnqueueMove( const RubDivPuzzle::Move& move );
	bool ProcessMoveQueue( RubDivPuzzle::RenderData& renderData );
	void AddHistory( const RubDivPuzzle::Move& move );
	bool GetMoveForUndo( RubDivPuzzle::Move& move );
	bool GetMoveForRedo( RubDivPuzzle::Move& move );
	bool CanUndo( void );
	bool CanRedo( void );

private:

	RubDivPuzzle* puzzle;

	RubDivFrame* frame;

	RubDivPuzzle::MoveList moveQueue;
	RubDivPuzzle::MoveList moveHistory;
	RubDivPuzzle::MoveList::iterator historyIter;
};

wxDECLARE_APP( RubDivApp );

// RubDivApp.h