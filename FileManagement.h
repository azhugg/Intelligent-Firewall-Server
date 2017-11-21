// FileManagement.h: interface for the CFileManagement class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEMANAGEMENT_H__B26DA22A_E4FF_47C8_ADAB_27EA5C35CAD8__INCLUDED_)
#define AFX_FILEMANAGEMENT_H__B26DA22A_E4FF_47C8_ADAB_27EA5C35CAD8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFServerSubDlg.h"
#include "Rules.h"	// Added by ClassView

class CFileManagement  
{
public:
	BOOL SaveConfigurationFile();
	BOOL LoadConfigurationFile();
	void LoadAlertFile(char *fp, CIFServerSubDlg *m_subDlg);
	void OpenIncludeFileToVeiw(char *file, CIFServerSubDlg *ptrDlg);
	void OpenIDSRulesFile(char *file, CIFServerSubDlg *m_subDlg);
	FILE* OpenAlertFile();
	CFileManagement();
	virtual ~CFileManagement();
};

#endif // !defined(AFX_FILEMANAGEMENT_H__B26DA22A_E4FF_47C8_ADAB_27EA5C35CAD8__INCLUDED_)
