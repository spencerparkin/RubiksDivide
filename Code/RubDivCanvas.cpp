// RubDivCanvas.cpp

#include "RubDivCanvas.h"

int RubDivCanvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

RubDivCanvas::RubDivCanvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	context = 0;

	Bind( wxEVT_PAINT, &RubDivCanvas::OnPaint, this );
	Bind( wxEVT_SIZE, &RubDivCanvas::OnSize, this );
	Bind( wxEVT_LEFT_DOWN, &RubDivCanvas::OnMouseLeftDown, this );
	Bind( wxEVT_MOTION, &RubDivCanvas::OnMouseMotion, this );
	Bind( wxEVT_MOUSE_CAPTURE_LOST, &RubDivCanvas::OnMouseCaptureLost, this );
}

/*virtual*/ RubDivCanvas::~RubDivCanvas( void )
{
	delete context;
}

void RubDivCanvas::BindContext( void )
{
	if( !context )
		context = new wxGLContext( this );

	SetCurrent( *context );
}

void RubDivCanvas::OnPaint( wxPaintEvent& event )
{
	BindContext();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glFlush();

	SwapBuffers();
}

void RubDivCanvas::OnSize( wxSizeEvent& event )
{
	BindContext();

	wxSize size = event.GetSize();
	glViewport( 0, 0, size.GetWidth(), size.GetHeight() );

	Refresh();
}

void RubDivCanvas::OnMouseLeftDown( wxMouseEvent& event )
{
}

void RubDivCanvas::OnMouseMotion( wxMouseEvent& event )
{
}

void RubDivCanvas::OnMouseCaptureLost( wxMouseCaptureLostEvent& event )
{
}

// RubDivCanvas.cpp