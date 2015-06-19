// RubDivApp.cpp

#include "RubDivApp.h"

wxIMPLEMENT_APP( RubDivApp );

RubDivApp::RubDivApp( void )
{
}

/*virtual*/ RubDivApp::~RubDivApp( void )
{
}

/*virtual*/ bool RubDivApp::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	return true;
}

/*virtual*/ int RubDivApp::OnExit( void )
{
	return wxApp::OnExit();
}

// RubDivApp.cpp