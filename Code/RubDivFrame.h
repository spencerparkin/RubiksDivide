// RubDivFrame.h

#pragma once

#include <wx/frame.h>
#include <wx/glcanvas.h>

class RubDivCanvas;

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
		ID_Exit,
		ID_About,
		ID_OrientVertical,
		ID_OrientHorizontal,
	};

	void OnNewPuzzle( wxCommandEvent& event );
	void OnLoadPuzzle( wxCommandEvent& event );
	void OnSavePuzzle( wxCommandEvent& event );
	void OnExit( wxCommandEvent& event );
	void OnAbout( wxCommandEvent& event );
	void OnOrientVertical( wxCommandEvent& event );
	void OnOrientHorizontal( wxCommandEvent& event );
	void OnUpdateMenuItemUI( wxUpdateUIEvent& event );

	RubDivCanvas* canvas;
};

// RubDivFrame.h