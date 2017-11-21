// IFServerSubDlg.h: interface for the CIFServerSubDlg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFSERVERSUBDLG_H__DF396593_6097_4078_967A_0C7AF6523FC5__INCLUDED_)
#define AFX_IFSERVERSUBDLG_H__DF396593_6097_4078_967A_0C7AF6523FC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIFServerSubDlg : public CDialog  
{
public:
	DECLARE_DYNCREATE(CIFServerSubDlg)

	CIFServerSubDlg();
	CIFServerSubDlg(UINT nID, CWnd *pParent = NULL);
	virtual ~CIFServerSubDlg();
	
	UINT GetID()      {return m_id;}

public:
	virtual void OnOK();
	virtual void OnCancel();
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIFServerSubDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	//}}AFX_VIRTUAL

protected:
	UINT     m_id;

	// Generated message map functions
	//{{AFX_MSG(CIFServerSubDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_IFSERVERSUBDLG_H__DF396593_6097_4078_967A_0C7AF6523FC5__INCLUDED_)
