// RubDivPuzzle.h

#pragma once

#include "c3ga/c3ga.h"
#include <wx/glcanvas.h>
#include <gl/gl.h>

class RubDivPuzzle
{
public:

	enum Orientation
	{
		VERTICAL,
		HORIZONTAL,
	};

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

	// This describtes how we render the puzzle with user-manipulation applied.
	struct RenderData
	{
		RenderData( void );

		// This is our view into the XY-plane.
		float yMin, yMax;
		float xMin, xMax;

		// Draw the indicated square matrix rotated by the given amount.
		float rotationAngle;
		int squareOffset;

		// Offset the indiciate row/column of each square matrix.
		float translation;
		int rowOrColumn;
	};

	void Render( GLenum mode, const RenderData& renderData ) const;

	struct Pick
	{
		Pick( void );

		int squareOffset;
		int row, col;

		void PushNameStackData( void );
		void PopNameStackData( void );

		void ReadHitRecord( unsigned int* hitRecord );
	};

	void ProcessHitBuffer( unsigned int* hitBuffer, int hitBufferSize, int hitCount, Pick& pick );

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

		void Render( GLenum mode, const RenderData& renderData, const c3ga::vectorE2GA& squareCenter, float squareSize, int squareOffset, const RubDivPuzzle* puzzle ) const;
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

	Orientation GetOrientation( void ) const;
	void SetOrientation( Orientation orientation );

	const c3ga::vectorE3GA& GetColorA( void ) const;
	const c3ga::vectorE3GA& GetColorB( void ) const;

	void SetColorA( c3ga::vectorE3GA& color );
	void SetColorB( c3ga::vectorE3GA& color );

	const c3ga::vectorE3GA* TranslateColor( Element::Color color ) const;

private:

	void SwapRowsOrColumns( int rowOrColumn, int matrixOffsetA, int matrixOffsetB );

	SquareMatrix** squareMatrixArray;

	Orientation orientation;

	c3ga::vectorE3GA colorA;
	c3ga::vectorE3GA colorB;
};

// RubDivPuzzle.h