
#if !defined(AFX_SELECTADAPTER_H__D41A3004_2B3D_11D0_9528_0020AF2A4474__INCLUDED_)
#define AFX_SELECTADAPTER_H__D41A3004_2B3D_11D0_9528_0020AF2A4474__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// SelectAdapter.h : header file
//

#include "LineColl.h"
#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CSelectAdapter dialog

int ExecuteApp(CString & s);

class CSelectAdapter : public CDialog
{
// Construction
public:
	CSelectAdapter(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelectAdapter)
	enum { IDD = IDD_ADAPTER };
	CStatic	m_CAdapter;
	CListCtrl	m_ListCtrl;
	//}}AFX_DATA
	CString m_Adapter;
	CString m_Cmd;
    CImageList m_ctlImage;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectAdapter)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void Update(LineCollection &lc);
	void AddItem(int nItem,int nSubItem,LPCTSTR strItem,int nImageIndex=-1);
	void SelectItem(int i);

	// Generated message map functions
	//{{AFX_MSG(CSelectAdapter)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnSelectItem(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void Ondblclickitem(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTADAPTER_H__D41A3004_2B3D_11D0_9528_0020AF2A4474__INCLUDED_)
