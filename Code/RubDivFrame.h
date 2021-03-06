// RubDivFrame.h

#pragma once

#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <wx/timer.h>

class RubDivCanvas;

// TODO: Expose A/B colors.

class RubDivFrame : public wxFrame
{
public:

	RubDivFrame( wxWindow* parent, const wxPoint& pos = wxDefaultPosition, const wxSize& size = wxDefaultSize );
	virtual ~RubDivFrame( void );

private:

	enum
	{
		ID_NewPuzzle = wxID_HIGHEST,
		ID_LoadPuzzle,
		ID_SavePuzzle,
		ID_ScramblePuzzle,
		ID_SolvePuzzle,
		ID_Exit,
		ID_About,
		ID_OrientVertical,
		ID_OrientHorizontal,
		ID_Timer,
		ID_Undo,
		ID_Redo,
	};

	void OnNewPuzzle( wxCommandEvent& event );
	void OnLoadPuzzle( wxCommandEvent& event );
	void OnSavePuzzle( wxCommandEvent& event );
	void OnScramblePuzzle( wxCommandEvent& event );
	void OnSolvePuzzle( wxCommandEvent& event );
	void OnUndo( wxCommandEvent& event );
	void OnRedo( wxCommandEvent& event );
	void OnExit( wxCommandEvent& event );
	void OnAbout( wxCommandEvent& event );
	void OnOrientVertical( wxCommandEvent& event );
	void OnOrientHorizontal( wxCommandEvent& event );
	void OnUpdateMenuItemUI( wxUpdateUIEvent& event );
	void OnTimer( wxTimerEvent& event );

	RubDivCanvas* canvas;

	wxTimer timer;
};

// RubDivFrame.h