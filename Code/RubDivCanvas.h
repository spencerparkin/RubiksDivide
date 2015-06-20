// RubDivCanvas.h

#pragma once

#include "RubDivPuzzle.h"

#include <wx/glcanvas.h>

class RubDivCanvas : public wxGLCanvas
{
public:

	RubDivCanvas( wxWindow* parent );
	virtual ~RubDivCanvas( void );

private:

	void BindContext( void );
	void Render( GLenum mode );

	void OnPaint( wxPaintEvent& event );
	void OnSize( wxSizeEvent& event );
	void OnMouseLeftDown( wxMouseEvent& event );
	void OnMouseMotion( wxMouseEvent& event );
	void OnMouseCaptureLost( wxMouseCaptureLostEvent& event );

	wxGLContext* context;

	static int attributeList[];

	RubDivPuzzle::RenderData renderData;
};

// RubDivCanvas.h