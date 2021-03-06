// RubDivCanvas.h

#pragma once

#include "RubDivPuzzle.h"

#include <wx/glcanvas.h>

class RubDivCanvas : public wxGLCanvas
{
public:

	RubDivCanvas( wxWindow* parent );
	virtual ~RubDivCanvas( void );

	bool Animate( void );

private:

	void BindContext( void );
	void Render( GLenum mode, const wxPoint* mousePos = 0, RubDivPuzzle::Pick* pick = 0 );
	void RotateWithMouse( RubDivPuzzle* puzzle, const wxPoint& newMousePos );

	void OnPaint( wxPaintEvent& event );
	void OnSize( wxSizeEvent& event );
	void OnMouseLeftDown( wxMouseEvent& event );
	void OnMouseLeftUp( wxMouseEvent& event );
	void OnMouseMotion( wxMouseEvent& event );
	void OnMouseCaptureLost( wxMouseCaptureLostEvent& event );

	wxGLContext* context;
	wxPoint mousePos;

	static int attributeList[];

	RubDivPuzzle::RenderData renderData;
	RubDivPuzzle::Pick pick;
};

// RubDivCanvas.h