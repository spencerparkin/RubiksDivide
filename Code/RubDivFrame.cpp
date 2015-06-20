// RubDivFrame.cpp

#include "RubDivFrame.h"
#include "RubDivApp.h"
#include "RubDivPuzzle.h"
#include "RubDivCanvas.h"

#include <wx/menu.h>
#include <wx/numdlg.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>

RubDivFrame::RubDivFrame( wxWindow* parent, const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/ ) : wxFrame( parent, wxID_ANY, "Rubik's Divide", pos, size )
{
	wxMenu* puzzleMenu = new wxMenu();
	wxMenuItem* newPuzzleMenuItem = new wxMenuItem( puzzleMenu, ID_NewPuzzle, "New Puzzle", "Start a new puzzle." );
	wxMenuItem* loadPuzzleMenuItem = new wxMenuItem( puzzleMenu, ID_LoadPuzzle, "Load Puzzle", "Load an old puzzle." );
	wxMenuItem* savePuzzleMenuItem = new wxMenuItem( puzzleMenu, ID_SavePuzzle, "Save Puzzle", "Save your puzzle." );
	wxMenuItem* exitMenuItem = new wxMenuItem( puzzleMenu, ID_Exit, "Exit", "Exit this program." );
	puzzleMenu->Append( newPuzzleMenuItem );
	puzzleMenu->AppendSeparator();
	puzzleMenu->Append( loadPuzzleMenuItem );
	puzzleMenu->Append( savePuzzleMenuItem );
	puzzleMenu->AppendSeparator();
	puzzleMenu->Append( exitMenuItem );

	wxMenu* helpMenu = new wxMenu();
	wxMenuItem* aboutMenuItem = new wxMenuItem( helpMenu, ID_About, "About", "How about box." );
	helpMenu->Append( aboutMenuItem );

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append( puzzleMenu, "Puzzle" );
	menuBar->Append( helpMenu, "Help" );
	SetMenuBar( menuBar );

	//wxStatusBar* statusBar = new wxStatusBar( this );
	//SetStatusBar( statusBar );

	canvas = new RubDivCanvas( this );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( canvas, 1, wxALL | wxGROW, 0 );
	SetSizer( boxSizer );

	Bind( wxEVT_MENU, &RubDivFrame::OnNewPuzzle, this, ID_NewPuzzle );
	Bind( wxEVT_MENU, &RubDivFrame::OnLoadPuzzle, this, ID_LoadPuzzle );
	Bind( wxEVT_MENU, &RubDivFrame::OnSavePuzzle, this, ID_SavePuzzle );
	Bind( wxEVT_MENU, &RubDivFrame::OnExit, this, ID_Exit );
	Bind( wxEVT_MENU, &RubDivFrame::OnAbout, this, ID_About );
}

/*virtual*/ RubDivFrame::~RubDivFrame( void )
{
}

void RubDivFrame::OnNewPuzzle( wxCommandEvent& event )
{
	int size = ( int )wxGetNumberFromUser( "What size of puzzle?", "Size:", "Puzzle Size", 4, 2, 32, this );
	if( size == -1 )
		return;

	RubDivPuzzle* puzzle = new RubDivPuzzle( size );
	wxGetApp().SetPuzzle( puzzle );
}

void RubDivFrame::OnLoadPuzzle( wxCommandEvent& event )
{
}

void RubDivFrame::OnSavePuzzle( wxCommandEvent& event )
{
}

void RubDivFrame::OnExit( wxCommandEvent& event )
{
	Close( true );
}

void RubDivFrame::OnAbout( wxCommandEvent& event )
{
}

// RubDivFrame.cpp