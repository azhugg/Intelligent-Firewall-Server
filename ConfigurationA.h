#if !defined(AFX_CONFIGURATIONA_H__6738DCA5_2D85_4591_A208_526510BCE183__INCLUDED_)
#define AFX_CONFIGURATIONA_H__6738DCA5_2D85_4591_A208_526510BCE183__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfigurationA.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConfigurationA dialog

class CConfigurationA : public CDialog
{
// Construction
public:
	CConfigurationA(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CConfigurationA)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConfigurationA)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConfigurationA)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONFIGURATIONA_H__6738DCA5_2D85_4591_A208_526510BCE183__INCLUDED_)
