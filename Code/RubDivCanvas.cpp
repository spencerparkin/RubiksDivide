// RubDivCanvas.cpp

#include "RubDivCanvas.h"
#include "RubDivApp.h"

#include <gl/gl.h>
#include <gl/glu.h>

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

void RubDivCanvas::Render( GLenum mode )
{
	BindContext();

	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );

	float width = float( viewport[2] );
	float height = float( viewport[3] );

	renderData.xMin = 0.f;
	renderData.xMax = width;
	renderData.yMin = 0.f;
	renderData.yMax = height;

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluOrtho2D( renderData.xMin, renderData.xMax, renderData.yMin, renderData.yMax );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();
	if( puzzle )
		puzzle->Render( mode, renderData );

	glFlush();

	SwapBuffers();
}

void RubDivCanvas::OnPaint( wxPaintEvent& event )
{
	Render( GL_RENDER );
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