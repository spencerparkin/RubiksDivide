// RubDivPuzzle.cpp

#include "RubDivPuzzle.h"

RubDivPuzzle::RubDivPuzzle( int size )
{
	colorA.set( c3ga::vectorE3GA::coord_e1_e2_e3, 1.f, 0.f, 0.f );
	colorB.set( c3ga::vectorE3GA::coord_e1_e2_e3, 1.f, 1.f, 1.f );

	squareMatrixArray = new SquareMatrix*[ SQUARE_MATRIX_COUNT ];

	for( int i = 0; i < SQUARE_MATRIX_COUNT; i++ )
		squareMatrixArray[i] = new SquareMatrix( size );

	orientation = VERTICAL;

	Reset();
}

RubDivPuzzle::~RubDivPuzzle( void )
{
	for( int i = 0; i < SQUARE_MATRIX_COUNT; i++ )
		delete squareMatrixArray[i];

	delete[] squareMatrixArray;
}

bool RubDivPuzzle::IsSolved( void ) const
{
	if( !squareMatrixArray[0]->IsHomogeneousOfColor( Element::COLOR_NONE ) )
		return false;

	if( !squareMatrixArray[3]->IsHomogeneousOfColor( Element::COLOR_NONE ) )
		return false;

	if( squareMatrixArray[1]->IsHomogeneousOfColor( Element::COLOR_A ) )
	{
		wxASSERT( squareMatrixArray[2]->IsHomogeneousOfColor( Element::COLOR_B ) );
		return true;
	}
	
	if( squareMatrixArray[1]->IsHomogeneousOfColor( Element::COLOR_B ) )
	{
		wxASSERT( squareMatrixArray[2]->IsHomogeneousOfColor( Element::COLOR_A ) );
		return true;
	}

	return false;
}

// Note that to ensure solvability, we could start with a puzzle
// in the solved state, then randomly apply valid moves to the
// puzzle, but a more interesting approach here is to randomly
// populate the elements of the puzzle with the idea or hunch, if
// you will, that any such population is reachable from the solved state.
void RubDivPuzzle::Scramble( void )
{
	squareMatrixArray[0]->MakeHomogeneousOfColor( Element::COLOR_NONE );
	squareMatrixArray[3]->MakeHomogeneousOfColor( Element::COLOR_NONE );

	int size = squareMatrixArray[0]->size;

	int colorACount = size * size;
	int colorBCount = size * size;

	srand( unsigned( time( NULL ) ) );

	for( int i = 0; i < size; i++ )
	{
		for( int j = 0; j < size; j++ )
		{
			for( int k = 1; k < 3; k++ )
			{
				Element::Color color;
				if( rand() > RAND_MAX / 2 )
				{
					color = Element::COLOR_A;
					if( colorACount == 0 )
						color = Element::COLOR_B;
				}
				else
				{
					color = Element::COLOR_B;
					if( colorBCount == 0 )
						color = Element::COLOR_A;
				}

				squareMatrixArray[k]->matrix[i][j]->color = color;

				if( color == Element::COLOR_A )
					colorACount--;
				else if( color == Element::COLOR_B )
					colorBCount--;
			}
		}
	}
}

void RubDivPuzzle::Reset( void )
{
	squareMatrixArray[0]->MakeHomogeneousOfColor( Element::COLOR_NONE );
	squareMatrixArray[1]->MakeHomogeneousOfColor( Element::COLOR_A );
	squareMatrixArray[2]->MakeHomogeneousOfColor( Element::COLOR_B );
	squareMatrixArray[3]->MakeHomogeneousOfColor( Element::COLOR_NONE );
}

bool RubDivPuzzle::RotateSquareMatrixCW( int squareOffset )
{
	for( int i = 0; i < 3; i++ )
		if( !RotateSquareMatrixCCW( squareOffset ) )
			return false;

	return true;
}

bool RubDivPuzzle::RotateSquareMatrixCCW( int squareOffset )
{
	if( squareOffset < 0 || squareOffset >= SQUARE_MATRIX_COUNT )
		return false;

	// Only the middle two of the four square matrices can be rotated.
	if( squareOffset == 0 || squareOffset == 3 )
		return false;

	SquareMatrix* squareMatrix = squareMatrixArray[ squareOffset ];

	// The square can only be rotated if it does not contain any elements of COLOR_NONE.
	if( squareMatrix->CountColor( Element::COLOR_NONE ) > 0 )
		return false;

	squareMatrix->FlipDiagonal();
	squareMatrix->FlipHorizontal();
	return true;
}

bool RubDivPuzzle::ShiftRowOrColumnForward( int rowOrColumn )
{
	if( rowOrColumn < 0 || rowOrColumn >= squareMatrixArray[0]->size )
		return false;

	switch( orientation )
	{
		case HORIZONTAL:
		{
			if( squareMatrixArray[3]->matrix[ rowOrColumn ][0]->color != Element::COLOR_NONE )
				return false;
			break;
		}
		case VERTICAL:
		{
			if( squareMatrixArray[3]->matrix[0][ rowOrColumn ]->color != Element::COLOR_NONE )
				return false;
			break;
		}
	}

	SwapRowsOrColumns( rowOrColumn, 2, 3 );
	SwapRowsOrColumns( rowOrColumn, 1, 2 );
	SwapRowsOrColumns( rowOrColumn, 0, 1 );
	return true;
}

bool RubDivPuzzle::ShiftRowOrColumnBackward( int rowOrColumn )
{
	if( rowOrColumn < 0 || rowOrColumn >= squareMatrixArray[0]->size )
		return false;

	switch( orientation )
	{
		case HORIZONTAL:
		{
			if( squareMatrixArray[0]->matrix[ rowOrColumn ][0]->color != Element::COLOR_NONE )
				return false;
			break;
		}
		case VERTICAL:
		{
			if( squareMatrixArray[0]->matrix[0][ rowOrColumn ]->color != Element::COLOR_NONE )
				return false;
			break;
		}
	}

	SwapRowsOrColumns( rowOrColumn, 0, 1 );
	SwapRowsOrColumns( rowOrColumn, 1, 2 );
	SwapRowsOrColumns( rowOrColumn, 2, 3 );
	return true;
}

void RubDivPuzzle::SwapRowsOrColumns( int rowOrColumn, int matrixOffsetA, int matrixOffsetB )
{
	int size = squareMatrixArray[0]->size;

	SquareMatrix* squareMatrixA = squareMatrixArray[ matrixOffsetA ];
	SquareMatrix* squareMatrixB = squareMatrixArray[ matrixOffsetB ];

	switch( orientation )
	{
		case HORIZONTAL:
		{
			for( int i = 0; i < size; i++ )
				squareMatrixA->matrix[ rowOrColumn ][i]->SwapColorWith( squareMatrixB->matrix[ rowOrColumn ][i] );

			break;
		}
		case VERTICAL:
		{
			for( int i = 0; i < size; i++ )
				squareMatrixA->matrix[i][ rowOrColumn ]->SwapColorWith( squareMatrixB->matrix[i][ rowOrColumn ] );

			break;
		}
	}
}

void RubDivPuzzle::Render( GLenum mode, const RenderData& renderData ) const
{
	float width = renderData.xMax - renderData.xMin;
	float height = renderData.yMax - renderData.yMin;

	float puzzleExtent = 0.f;
	if( orientation == VERTICAL )
		puzzleExtent = height;
	else if( orientation == HORIZONTAL )
		puzzleExtent = width;

	float squareSize = puzzleExtent / ( 5.f / 2.f * ( sqrt( 2.f ) - 1.f ) + 4.f );

	c3ga::vectorE2GA squareCenter;
	if( orientation == VERTICAL )
		squareCenter.set( c3ga::vectorE2GA::coord_e1_e2, renderData.xMin + width / 2.f, renderData.yMax - squareSize * sqrt( 2.f ) / 2.f );
	else if( orientation == HORIZONTAL )
		squareCenter.set( c3ga::vectorE2GA::coord_e1_e2, renderData.xMin + squareSize * sqrt( 2.f ) / 2.f, renderData.yMin + height / 2.f );

	for( int i = 0; i < SQUARE_MATRIX_COUNT; i++ )
	{
		squareMatrixArray[i]->Render( mode, renderData, squareCenter, squareSize, i, this );

		if( orientation == VERTICAL )
			squareCenter.m_e2 -= squareSize * ( sqrt( 2.f ) + 1.f ) / 2.f;
		else if( orientation == HORIZONTAL )
			squareCenter.m_e1 += squareSize * ( sqrt( 2.f ) + 1.f ) / 2.f;
	}
}

RubDivPuzzle::Orientation RubDivPuzzle::GetOrientation( void ) const
{
	return orientation;
}

void RubDivPuzzle::SetOrientation( Orientation orientation )
{
	this->orientation = orientation;
}

const c3ga::vectorE3GA& RubDivPuzzle::GetColorA( void ) const
{
	return colorA;
}

const c3ga::vectorE3GA& RubDivPuzzle::GetColorB( void ) const
{
	return colorB;
}

void RubDivPuzzle::SetColorA( c3ga::vectorE3GA& color )
{
	colorA = color;
}

void RubDivPuzzle::SetColorB( c3ga::vectorE3GA& color )
{
	colorB = color;
}

const c3ga::vectorE3GA* RubDivPuzzle::TranslateColor( Element::Color color ) const
{
	switch( color )
	{
		case Element::COLOR_A: return &colorA;
		case Element::COLOR_B: return &colorB;
	}

	return 0;
}

RubDivPuzzle::SquareMatrix::SquareMatrix( int size )
{
	this->size = size;

	matrix = new Element**[ size ];
	for( int i = 0; i < size; i++ )
	{
		matrix[i] = new Element*[ size ];
		for( int j = 0; j < size; j++ )
		{
			Element* element = new Element;
			element->color = Element::COLOR_NONE;

			matrix[i][j] = element;
		}
	}
}

RubDivPuzzle::SquareMatrix::~SquareMatrix( void )
{
	for( int i = 0; i < size; i++ )
	{
		for( int j = 0; j < size; j++ )
			delete matrix[i][j];

		delete[] matrix[i];
	}

	delete[] matrix;
}

void RubDivPuzzle::SquareMatrix::Render( GLenum mode, const RenderData& renderData, const c3ga::vectorE2GA& squareCenter, float squareSize, int squareOffset, const RubDivPuzzle* puzzle ) const
{
	float elementSize = squareSize / float( size );

	for( int i = 0; i < size; i++ )
	{
		for( int j = 0; j < size; j++ )
		{
			Element* element = matrix[i][j];
			if( element->color == Element::COLOR_NONE )
				continue;

			c3ga::vectorE2GA elementCenter;
			float t = float(i) / float( size - 1 );
			elementCenter.m_e2 = ( ( squareSize - elementSize ) * ( 1.f - t ) + ( elementSize - squareSize ) * t ) / 2.f;
			t = float(j) / float( size - 1 );
			elementCenter.m_e1 = ( ( elementSize - squareSize ) * ( 1.f - t ) + ( squareSize - elementSize ) * t ) / 2.f;

			c3ga::vectorE2GA vertex[5];
			int vertexCount = sizeof( vertex ) / sizeof( c3ga::vectorE2GA );

			vertex[0].set( c3ga::vectorE2GA::coord_e1_e2, elementCenter.m_e1, elementCenter.m_e2 );
			vertex[1].set( c3ga::vectorE2GA::coord_e1_e2, elementCenter.m_e1 - elementSize / 2.f, elementCenter.m_e2 - elementSize / 2.f );
			vertex[2].set( c3ga::vectorE2GA::coord_e1_e2, elementCenter.m_e1 + elementSize / 2.f, elementCenter.m_e2 - elementSize / 2.f );
			vertex[3].set( c3ga::vectorE2GA::coord_e1_e2, elementCenter.m_e1 + elementSize / 2.f, elementCenter.m_e2 + elementSize / 2.f );
			vertex[4].set( c3ga::vectorE2GA::coord_e1_e2, elementCenter.m_e1 - elementSize / 2.f, elementCenter.m_e2 + elementSize / 2.f );

			if( renderData.squareOffset == squareOffset )
			{
				c3ga::rotorE2GA rotor;
				float halfAngle = renderData.rotationAngle / 2.f;
				rotor.m_scalar = cos( halfAngle );
				rotor.m_e1_e2 = sin( halfAngle );
				for( int k = 0; k < vertexCount; k++ )
					vertex[k] = c3ga::applyUnitVersor( rotor, vertex[k] );
			}
			else if( puzzle->GetOrientation() == VERTICAL && renderData.rowOrColumn == j )
			{
				for( int k = 0; k < vertexCount; k++ )
					vertex[k].m_e2 += renderData.translation;
			}
			else if( puzzle->GetOrientation() == HORIZONTAL && renderData.rowOrColumn == i )
			{
				for( int k = 0; k < vertexCount; k++ )
					vertex[k].m_e1 += renderData.translation;
			}

			for( int k = 0; k < vertexCount; k++ )
				vertex[k] = vertex[k] + squareCenter;

			Pick pick;
			pick.squareOffset = squareOffset;
			pick.row = i;
			pick.col = j;

			if( mode == GL_SELECT )
				pick.PushNameStackData();

			glBegin( GL_TRIANGLE_FAN );

			const c3ga::vectorE3GA* color = puzzle->TranslateColor( element->color );
			glColor3f( color->m_e1, color->m_e2, color->m_e3 );
			glVertex2f( vertex[0].m_e1, vertex[0].m_e2 );

			c3ga::vectorE3GA borderColor = c3ga::gp( *color, 0.5f );
			glColor3f( borderColor.m_e1, borderColor.m_e2, borderColor.m_e3 );

			glVertex2f( vertex[1].m_e1, vertex[1].m_e2 );
			glVertex2f( vertex[2].m_e1, vertex[2].m_e2 );
			glVertex2f( vertex[3].m_e1, vertex[3].m_e2 );
			glVertex2f( vertex[4].m_e1, vertex[4].m_e2 );
			glVertex2f( vertex[1].m_e1, vertex[1].m_e2 );

			glEnd();

			if( mode == GL_SELECT )
				pick.PopNameStackData();
		}
	}
}

void RubDivPuzzle::ProcessHitBuffer( unsigned int* hitBuffer, int hitBufferSize, int hitCount, Pick& pick )
{
	pick.squareOffset = -1;
	pick.row = 0;
	pick.col = 0;

	unsigned int* hitRecord = hitBuffer;
	float smallestZ = 9999.f;
	for( int i = 0; i < hitCount; i++ )
	{
		unsigned int nameCount = hitRecord[0];
		wxASSERT( nameCount == 3 );
		if( nameCount == 3 )
		{
			float minZ = float( hitRecord[1] ) / float( 0x7FFFFFFF );
			if( minZ < smallestZ )
			{
				smallestZ = minZ;
				pick.ReadHitRecord( hitRecord );
			}
		}

		hitRecord += 3 + nameCount;
	}
}

bool RubDivPuzzle::SquareMatrix::IsHomogeneousOfColor( Element::Color color ) const
{
	return( size * size == CountColor( color ) ) ? true : false;
}

int RubDivPuzzle::SquareMatrix::CountColor( Element::Color color ) const
{
	int count = 0;

	for( int i = 0; i < size; i++ )
		for( int j = 0; j < size; j++ )
			if( matrix[i][j]->color == color )
				count++;

	return count;
}

void RubDivPuzzle::SquareMatrix::MakeHomogeneousOfColor( Element::Color color )
{
	for( int i = 0; i < size; i++ )
		for( int j = 0; j < size; j++ )
			matrix[i][j]->color = color;
}

void RubDivPuzzle::SquareMatrix::FlipDiagonal( void )
{
	for( int i = 0; i < size; i++ )
		for( int j = 0; j < size; j++ )
			matrix[i][j]->SwapColorWith( matrix[j][i] );
}

void RubDivPuzzle::SquareMatrix::FlipHorizontal( void )
{
	for( int j = 0; j < size; j++ )
		for( int i = 0; i < size / 2; i++ )
			matrix[i][j]->SwapColorWith( matrix[ size - i - 1 ][i] );
}

void RubDivPuzzle::SquareMatrix::FlipVertical( void )
{
	for( int i = 0; i < size; i++ )
		for( int j = 0; j < size / 2; j++ )
			matrix[i][j]->SwapColorWith( matrix[i][ size - j - 1 ] );
}

void RubDivPuzzle::Element::SwapColorWith( Element* element )
{
	Color tempColor = this->color;
	this->color = element->color;
	element->color = tempColor;
}

RubDivPuzzle::RenderData::RenderData( void )
{
	xMin = 0.f;
	xMax = 0.f;
	yMin = 0.f;
	yMax = 0.f;

	rotationAngle = 0.f;
	squareOffset = -1;

	translation = 0.f;
	rowOrColumn = -1;
}

RubDivPuzzle::Pick::Pick( void )
{
	squareOffset = -1;
	row = 0;
	col = 0;
}

void RubDivPuzzle::Pick::PushNameStackData( void )
{
	glPushName( squareOffset );
	glPushName( row );
	glPushName( col );
}

void RubDivPuzzle::Pick::PopNameStackData( void )
{
	glPopName();
	glPopName();
	glPopName();
}

void RubDivPuzzle::Pick::ReadHitRecord( unsigned int* hitRecord )
{
	squareOffset = hitRecord[3];
	row = hitRecord[4];
	col = hitRecord[5];
}

// RubDivPuzzle.cpp