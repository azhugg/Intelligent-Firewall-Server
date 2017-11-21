#if !defined(AFX_SELECTADAPTOR_H__42F1E6BB_2D8E_402A_AEB0_AD63080CE9EB__INCLUDED_)
#define AFX_SELECTADAPTOR_H__42F1E6BB_2D8E_402A_AEB0_AD63080CE9EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SelectAdaptor.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSelectAdaptor dialog
#include "LineColl.h"

class CSelectAdaptor : public CDialog
{
// Construction
public:
	
	CImageList m_ctlImage;
	CString m_Cmd;
//	CStatic m_CAdapter;
	CString m_Adapter;
	CSelectAdaptor(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectAdaptor)
	enum { IDD = IDD_DIALOG_SELECT_ADAPTORS };
	CStatic	m_CAdapter;
	CListCtrl m_ListCtrl;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectAdaptor)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Update(LineCollection &lc);
	void SelectItem(int i);
	void OnSelectItem(NMHDR* pNMHDR, LRESULT* pResult);
	BOOL OnInitDialog();
	void Ondblclickitem(NMHDR* pNMHDR, LRESULT* pResult);
	void AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex);

	// Generated message map functions
	//{{AFX_MSG(CSelectAdaptor)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTADAPTOR_H__42F1E6BB_2D8E_402A_AEB0_AD63080CE9EB__INCLUDED_)
