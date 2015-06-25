// RubDivPuzzle.h

#pragma once

#include "c3ga/c3ga.h"
#include <wx/glcanvas.h>
#include <gl/gl.h>
#include <list>

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

	RubDivPuzzle* Clone( void ) const;

	int GetSize( void ) const { return squareMatrixArray[0]->size; }

	void Scramble( void );
	void Reset( void );

	bool IsSolved( void ) const;

	bool RotateSquareMatrixCW( int squareOffset );
	bool RotateSquareMatrixCCW( int squareOffset );
	bool ShiftRowOrColumnForward( int rowOrColumn );
	bool ShiftRowOrColumnBackward( int rowOrColumn );

	enum ShiftDir
	{
		SHIFT_NONE,
		SHIFT_FORWARD,
		SHIFT_BACKWARD,
	};

	struct Move
	{
		int squareOffset;
		int rotateCCWCount;
		int rowOrColumn;
		ShiftDir shiftDir;

		bool IsValid( void ) const;
		void Invert( Move& inverseMove ) const;
	};

	typedef std::list< Move > MoveList;

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

	bool TranslateMove( const RenderData& renderData, Move& move );
	bool ManipulatePuzzle( const Move& move, RenderData* renderData = 0 );
	bool ManipulatePuzzle( const MoveList& moveList );

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

	Element::Color GetColor( int squareOffset, int i, int j ) const;

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
	bool SetOrientation( Orientation orientation );

	const c3ga::vectorE3GA& GetColorA( void ) const;
	const c3ga::vectorE3GA& GetColorB( void ) const;

	void SetColorA( c3ga::vectorE3GA& color );
	void SetColorB( c3ga::vectorE3GA& color );

	const c3ga::vectorE3GA* TranslateColor( Element::Color color ) const;

	static int RandomInteger( int min, int max );
	static void ShuffleArray( int* array, int arraySize );
	static void NormalizeAngle( float& angle );

	c3ga::vectorE2GA CalculateSquareCenter( const RenderData& renderData, int squareOffset ) const;

private:

	void SwapRowsOrColumns( int rowOrColumn, int matrixOffsetA, int matrixOffsetB );
	float CalculateSquareSize( const RenderData& renderData, float& width, float& height ) const;

	SquareMatrix** squareMatrixArray;

	Orientation orientation;

	c3ga::vectorE3GA colorA;
	c3ga::vectorE3GA colorB;
};

// RubDivPuzzle.h