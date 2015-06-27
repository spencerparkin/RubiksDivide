// RubDivSolverV2.h

#pragma once

#include <list>
#include <map>

#include "RubDivPuzzle.h"
#include "RubDivSolver.h"

class RubDivSolverV2 : public RubDivSolver
{
public:

	RubDivSolverV2( void );
	virtual ~RubDivSolverV2( void );

	virtual bool Solve( const RubDivPuzzle* puzzle, RubDivPuzzle::MoveList& moveList ) override;

private:

	bool FindAlgorithm( RubDivPuzzle::MoveList& algorithm );
	
	class AlgorithmComponents
	{
	public:

		AlgorithmComponents( bool ccw );
		~AlgorithmComponents( void );

		bool ConstructAlgorithm( RubDivPuzzle::MoveList& algorithm );
		void AddShiftIfPossible( const RubDivPuzzle::Move& shiftMove, const RubDivPuzzle* puzzle );

		int ColorMoveCount( void ) { return colorMoveCount; }

	private:

		bool ShiftHelpsToCompleteRow( const RubDivPuzzle::Move& shiftMove, int row, const RubDivPuzzle* puzzle );

		RubDivPuzzle::Move rotationMove;
		RubDivPuzzle::Move inverseRotationMove;
		RubDivPuzzle::MoveList shiftMoveList;
		RubDivPuzzle::MoveList inverseShiftMoveList;
		RubDivPuzzle::MoveList preservationMoveList;
		RubDivPuzzle::MoveList inversePreservationMoveList;

		typedef std::map< int, int > ColumnMap;
		ColumnMap columnMap;

		int colorMoveCount;
	};

	RubDivPuzzle* puzzleClone;
};

// RubDivSolverV2.h