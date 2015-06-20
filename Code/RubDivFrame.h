// RubDivFrame.h

#pragma once

#include <wx/frame.h>
#include <wx/glcanvas.h>
#include <gl/gl.h>
#include <gl/glu.h>

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
	};

	void OnNewPuzzle( wxCommandEvent& event );
	void OnLoadPuzzle( wxCommandEvent& event );
	void OnSavePuzzle( wxCommandEvent& event );
	void OnExit( wxCommandEvent& event );
	void OnAbout( wxCommandEvent& event );

	RubDivCanvas* canvas;
};

// RubDivFrame.h