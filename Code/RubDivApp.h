// RubDivApp.h

#pragma once

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

private:

	RubDivPuzzle* puzzle;

	RubDivFrame* frame;
};

wxDECLARE_APP( RubDivApp );

// RubDivApp.h