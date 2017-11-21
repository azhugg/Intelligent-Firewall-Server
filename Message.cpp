// Message.cpp: implementation of the CMessage class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Message.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMessage::CMessage()
{

}

CMessage::~CMessage()
{

}

void CMessage::ErrorMessage(CString str)
{
	MessageBox(NULL,str,"IFS Error Message",MB_OK);
}

void CMessage::FatalErrorMessage(CString str)
{
	MessageBox(NULL,str,"IFS Fatal Error Message",MB_OK);
}

void CMessage::InformationMessage(CString str)
{
	MessageBox(NULL,str,"IFS Information Message",MB_OK);	
}
