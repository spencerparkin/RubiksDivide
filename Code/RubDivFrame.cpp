// RubDivFrame.cpp

#include "RubDivFrame.h"
#include "RubDivApp.h"
#include "RubDivPuzzle.h"
#include "RubDivCanvas.h"

#include <wx/menu.h>
#include <wx/numdlg.h>
#include <wx/sizer.h>
#include <wx/statusbr.h>
#include <wx/msgdlg.h>

RubDivFrame::RubDivFrame( wxWindow* parent, const wxPoint& pos /*= wxDefaultPosition*/, const wxSize& size /*= wxDefaultSize*/ ) : wxFrame( parent, wxID_ANY, "Rubik's Divide", pos, size ), timer( this, ID_Timer )
{
	wxMenu* orientMenu = new wxMenu();
	wxMenuItem* orientVerticalMenuItem = new wxMenuItem( orientMenu, ID_OrientVertical, "Vertical", "Orient the puzzle vertically", wxITEM_CHECK );
	wxMenuItem* orientHorizontalMenuItem = new wxMenuItem( orientMenu, ID_OrientHorizontal, "Horizontal", "Orient the puzzle horizontally", wxITEM_CHECK );
	orientMenu->Append( orientVerticalMenuItem );
	orientMenu->Append( orientHorizontalMenuItem );

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
	menuBar->Append( orientMenu, "Orient" );
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
	Bind( wxEVT_MENU, &RubDivFrame::OnOrientVertical, this, ID_OrientVertical );
	Bind( wxEVT_MENU, &RubDivFrame::OnOrientHorizontal, this, ID_OrientHorizontal );
	Bind( wxEVT_UPDATE_UI, &RubDivFrame::OnUpdateMenuItemUI, this, ID_OrientVertical );
	Bind( wxEVT_UPDATE_UI, &RubDivFrame::OnUpdateMenuItemUI, this, ID_OrientHorizontal );
	Bind( wxEVT_TIMER, &RubDivFrame::OnTimer, this, ID_Timer );

	timer.Start(1);
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

	puzzle->Scramble();

	canvas->Refresh();
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

void RubDivFrame::OnTimer( wxTimerEvent& event )
{
	if( canvas->Animate() )
		canvas->Refresh();
}

void RubDivFrame::OnOrientVertical( wxCommandEvent& event )
{
	RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();
	if( puzzle )
	{
		if( puzzle->SetOrientation( RubDivPuzzle::VERTICAL ) )
			canvas->Refresh();
		else
			wxMessageBox( "Center the puzzle before changing orientation.", "Oops", wxICON_EXCLAMATION );
	}
}

void RubDivFrame::OnOrientHorizontal( wxCommandEvent& event )
{
	RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();
	if( puzzle )
	{
		if( puzzle->SetOrientation( RubDivPuzzle::HORIZONTAL ) )
			canvas->Refresh();
		else
			wxMessageBox( "Center the puzzle before changing orientation.", "Oops", wxICON_EXCLAMATION );
	}
}

void RubDivFrame::OnUpdateMenuItemUI( wxUpdateUIEvent& event )
{
	RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();

	switch( event.GetId() )
	{
		case ID_OrientVertical:
		{
			if( !puzzle )
				event.Enable( false );
			else
			{
				event.Enable( true );
				if( puzzle->GetOrientation() == RubDivPuzzle::VERTICAL )
					event.Check( true );
				else
					event.Check( false );
			}
			break;
		}
		case ID_OrientHorizontal:
		{
			if( !puzzle )
				event.Enable( false );
			else
			{
				event.Enable( true );
				if( puzzle->GetOrientation() == RubDivPuzzle::HORIZONTAL )
					event.Check( true );
				else
					event.Check( false );
			}
			break;
		}
	}
}

// RubDivFrame.cpp