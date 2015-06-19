// RubDivPuzzle.h

#pragma once

#include "c3ga/c3ga.h"
#include <wx/glcanvas.h>
#include <gl/gl.h>

class RubDivPuzzle
{
public:

	RubDivPuzzle( int size );
	~RubDivPuzzle( void );

	void Scramble( void );
	void Reset( void );

	bool IsSolved( void ) const;

	// These are used to manipulate the puzzle.
	bool RotateSquareMatrixCW( int squareOffset );
	bool RotateSquareMatrixCCW( int squareOffset );
	bool ShiftRowOrColumnForward( int rowOrColumn );
	bool ShiftRowOrColumnBackward( int rowOrColumn );

	// This describtes how we render the puzzle.
	struct RenderData
	{
		// This is the amount of area an element should take up in the XY-plane.
		float elementArea;

		// Draw the indicated square matrix rotated by the given amount.
		c3ga::rotorE3GA rotor;
		int squareOffset;

		// Offset the indiciate row/column of each square matrix.
		c3ga::evenVersor motor;
		int rowOrColumn;
	};

	void Render( GLenum mode, const RenderData& renderData );

	struct Element
	{
		enum Color
		{
			COLOR_NONE,
			COLOR_A,
			COLOR_B,
		};

		void SwapColorWith( Element* element );

		Color color;
	};

	class SquareMatrix
	{
		friend class RubDivPuzzle;

	public:

		SquareMatrix( int size );
		~SquareMatrix( void );

		bool IsHomogeneousOfColor( Element::Color color ) const;
		int CountColor( Element::Color color ) const;
		void MakeHomogeneousOfColor( Element::Color color );
		void FlipDiagonal( void );
		void FlipHorizontal( void );
		void FlipVertical( void );

	private:

		Element*** matrix;
		int size;
	};

	enum
	{
		SQUARE_MATRIX_COUNT = 4,
	};

	enum Orientation
	{
		VERTICAL,
		HORIZONTAL,
	};

	Orientation GetOrientation( void ) const;
	void SetOrientation( Orientation orientation );

private:

	void SwapRowsOrColumns( int rowOrColumn, int matrixOffsetA, int matrixOffsetB );

	SquareMatrix** squareMatrixArray;

	Orientation orientation;
};

// RubDivPuzzle.h