// RubDivApp.h

#pragma once

#include <wx/app.h>

class RubDivApp : public wxApp
{
public:

	RubDivApp( void );
	virtual ~RubDivApp( void );

	virtual bool OnInit( void ) override;
	virtual int OnExit( void ) override;
};

wxDECLARE_APP( RubDivApp );

// RubDivApp.h