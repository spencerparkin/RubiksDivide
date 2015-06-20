// RubDivCanvas.h

#pragma once

#include <wx/glcanvas.h>

class RubDivCanvas : public wxGLCanvas
{
public:

	RubDivCanvas( wxWindow* parent );
	virtual ~RubDivCanvas( void );

private:

	void BindContext( void );

	void OnPaint( wxPaintEvent& event );
	void OnSize( wxSizeEvent& event );
	void OnMouseLeftDown( wxMouseEvent& event );
	void OnMouseMotion( wxMouseEvent& event );
	void OnMouseCaptureLost( wxMouseCaptureLostEvent& event );

	wxGLContext* context;

	static int attributeList[];
};

// RubDivCanvas.h