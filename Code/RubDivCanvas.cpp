// RubDivCanvas.cpp

#include "RubDivCanvas.h"
#include "RubDivApp.h"
#include "RubDivFrame.h"

#include <gl/gl.h>
#include <gl/glu.h>
#include <wx/msgdlg.h>

int RubDivCanvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, 0 };

RubDivCanvas::RubDivCanvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	context = 0;

	Bind( wxEVT_PAINT, &RubDivCanvas::OnPaint, this );
	Bind( wxEVT_SIZE, &RubDivCanvas::OnSize, this );
	Bind( wxEVT_LEFT_DOWN, &RubDivCanvas::OnMouseLeftDown, this );
	Bind( wxEVT_LEFT_UP, &RubDivCanvas::OnMouseLeftUp, this );
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

void RubDivCanvas::Render( GLenum mode, const wxPoint* mousePos /*= 0*/, RubDivPuzzle::Pick* pick /*= 0*/ )
{
	BindContext();

	int hitBufferSize = 0;
	unsigned int* hitBuffer = 0;
	if( mode == GL_SELECT )
	{
		hitBufferSize = 512;		// We only have at most one pick-able thing on the name stack at any given time.
		hitBuffer = new unsigned int[ hitBufferSize ];
		glSelectBuffer( hitBufferSize, hitBuffer );
		glRenderMode( GL_SELECT );
		glInitNames();
	}

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
	if( mode == GL_SELECT )
		gluPickMatrix( GLdouble( mousePos->x ), GLdouble( viewport[3] - mousePos->y - 1 ), 1.0, 1.0, viewport );
	gluOrtho2D( renderData.xMin, renderData.xMax, renderData.yMin, renderData.yMax );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();
	if( puzzle )
		puzzle->Render( mode, renderData );

	glFlush();

	if( mode == GL_SELECT )
	{
		int hitCount = glRenderMode( GL_RENDER );
		if( puzzle )
			puzzle->ProcessHitBuffer( hitBuffer, hitBufferSize, hitCount, *pick );

		delete[] hitBuffer;
	}

	if( mode == GL_RENDER )
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
	RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();
	if( puzzle )
	{
		renderData.rowOrColumn = -1;
		renderData.squareOffset = -1;

		mousePos = event.GetPosition();
		Render( GL_SELECT, &mousePos, &pick );
		if( pick.squareOffset != -1 )
			CaptureMouse();
	}
}

void RubDivCanvas::OnMouseLeftUp( wxMouseEvent& event )
{
	if( pick.squareOffset != -1 )
	{
		ReleaseMouse();

		RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();
		if( puzzle )
		{
			RubDivPuzzle::Move move;
			if( puzzle->TranslateMove( renderData, move ) )
			{
				if( puzzle->ManipulatePuzzle( move, &renderData ) )
				{
					wxGetApp().AddHistory( move );
					
					if( puzzle->IsSolved() )
						wxMessageBox( "You rock!", "Solved!", wxICON_EXCLAMATION );
				}
			}
		}

		pick.squareOffset = -1;
	}
}

void RubDivCanvas::RotateWithMouse( RubDivPuzzle* puzzle, const wxPoint& newMousePos )
{
	c3ga::vectorE2GA squareCenter = puzzle->CalculateSquareCenter( renderData, pick.squareOffset );
	c3ga::vectorE2GA originalMousePos( c3ga::vectorE2GA::coord_e1_e2, mousePos.x, renderData.yMax - mousePos.y );
	c3ga::vectorE2GA currentMousePos( c3ga::vectorE2GA::coord_e1_e2, newMousePos.x, renderData.yMax - newMousePos.y );

	c3ga::vectorE2GA originalMouseVec = originalMousePos - squareCenter;
	c3ga::vectorE2GA currentMouseVec = currentMousePos - squareCenter;
	float angle = acos( c3ga::lc( c3ga::unit( originalMouseVec ), c3ga::unit( currentMouseVec ) ) );
	
	c3ga::bivectorE2GA bivec = c3ga::op( originalMouseVec, currentMouseVec );
	if( bivec.get_e1_e2() > 0.f )
		renderData.rotationAngle = angle;
	else
		renderData.rotationAngle = -angle;
}

void RubDivCanvas::OnMouseMotion( wxMouseEvent& event )
{
	RubDivPuzzle* puzzle = wxGetApp().GetPuzzle();
	if( !puzzle )
		return;

	if( event.LeftIsDown() && pick.squareOffset != -1 )
	{
		wxPoint mouseDelta = event.GetPosition() - mousePos;
		if( mouseDelta.x * mouseDelta.x + mouseDelta.y * mouseDelta.y > 4 )
		{
			switch( puzzle->GetOrientation() )
			{
				case RubDivPuzzle::VERTICAL:
				{
					if( renderData.rowOrColumn == -1 && renderData.squareOffset == -1 )
					{
						if( abs( mouseDelta.x ) < abs( mouseDelta.y ) )
							renderData.rowOrColumn = pick.col;
						else
							renderData.squareOffset = pick.squareOffset;
					}

					if( renderData.rowOrColumn != -1 )
						renderData.translation = -float( mouseDelta.y );
					else if( renderData.squareOffset != -1 )
						RotateWithMouse( puzzle, event.GetPosition() );

					break;
				}
				case RubDivPuzzle::HORIZONTAL:
				{
					if( renderData.rowOrColumn == -1 && renderData.squareOffset == -1 )
					{
						if( abs( mouseDelta.y ) < abs( mouseDelta.x ) )
							renderData.rowOrColumn = pick.row;
						else
							renderData.squareOffset = pick.squareOffset;
					}

					if( renderData.rowOrColumn != -1 )
						renderData.translation = float( mouseDelta.x );
					else if( renderData.squareOffset != -1 )
						RotateWithMouse( puzzle, event.GetPosition() );

					break;
				}
			}

			Refresh();
		}
	}
}

void RubDivCanvas::OnMouseCaptureLost( wxMouseCaptureLostEvent& event )
{
	pick.squareOffset = -1;
}

// TODO: We really should be animating in a frame-rate independent way here.
bool RubDivCanvas::Animate( void )
{
	const float eps = 1e-2f;
	const float lerp = 0.5f;

	if( pick.squareOffset == -1 )
	{
		if( renderData.squareOffset != -1 )
		{
			RubDivPuzzle::NormalizeAngle( renderData.rotationAngle );

			if( renderData.rotationAngle == 0.f || renderData.rotationAngle == 2.f * float( M_PI ) )
				renderData.squareOffset = -1;
			else
			{
				if( fabs( renderData.rotationAngle ) < fabs( renderData.rotationAngle - 2.f * float( M_PI ) ) )
				{
					if( fabs( renderData.rotationAngle ) < eps )
						renderData.rotationAngle = 0.f;
					else
						renderData.rotationAngle *= lerp;
				}
				else
				{
					if( fabs( renderData.rotationAngle - 2.f * float( M_PI ) ) < eps )
						renderData.rotationAngle = 2.f * float( M_PI );
					else
						renderData.rotationAngle = renderData.rotationAngle + ( 1.f - lerp ) * ( 2.f * float( M_PI ) - renderData.rotationAngle );
				}
			}
			return true;
		}
		else if( renderData.rowOrColumn != -1 )
		{
			if( renderData.translation == 0.f )
				renderData.rowOrColumn = -1;
			else if( fabs( renderData.translation ) >= eps )
				renderData.translation *= lerp;
			else
				renderData.translation = 0.f;
			return true;
		}
		else
		{
			if( wxGetApp().ProcessMoveQueue( renderData ) )
				return true;
		}
	}

	wxString text = wxString::Format( "rotAgl: %1.2f degs; trans: %1.2f units", renderData.rotationAngle * 180.f / float( M_PI ), renderData.translation );
	wxGetApp().GetFrame()->GetStatusBar()->SetStatusText( text );

	return false;
}

// RubDivCanvas.cpp