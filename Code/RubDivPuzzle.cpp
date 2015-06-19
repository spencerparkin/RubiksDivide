// RubDivPuzzle.cpp

#include "RubDivPuzzle.h"

RubDivPuzzle::RubDivPuzzle( int size )
{
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

void RubDivPuzzle::Render( GLenum mode, const RenderData& renderData )
{
	//...
	// If rendering in selection mode, embed 3-dimensional coordinates in stack for each element.
}

RubDivPuzzle::Orientation RubDivPuzzle::GetOrientation( void ) const
{
	return orientation;
}

void RubDivPuzzle::SetOrientation( Orientation orientation )
{
	this->orientation = orientation;
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

// RubDivPuzzle.cpp