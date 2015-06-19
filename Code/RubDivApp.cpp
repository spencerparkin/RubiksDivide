// RubDivApp.cpp

#include "RubDivApp.h"
#include "RubDivPuzzle.h"
#include "RubDivFrame.h"

wxIMPLEMENT_APP( RubDivApp );

RubDivApp::RubDivApp( void )
{
	puzzle = 0;
	frame = 0;
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
}

// RubDivApp.cpp