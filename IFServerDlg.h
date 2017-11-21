// IFServerDlg.h : header file
//

#if !defined(AFX_IFSERVERDLG_H__606F5293_783B_48BD_8EF9_10E8A00BEFA3__INCLUDED_)
#define AFX_IFSERVERDLG_H__606F5293_783B_48BD_8EF9_10E8A00BEFA3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFServerSubDlg.h"
#include "IFServerStatic.h"

#include <afxtempl.h>
#include "IFServer.h"

class pageStruct
{
   public:
   CIFServerSubDlg *pDlg;
   UINT id;
   CIFServerSubDlg *pDlgParent;
   CString csCaption;
};

#define WM_CHANGE_PAGE (WM_APP + 100)
#define WM_SET_FOCUS_WND (WM_APP + 101)

/////////////////////////////////////////////////////////////////////////////
// CIFServerDlg dialog

class CIFServerDlg : public CDialog
{
// Construction
public:
	CString m_temp;
	CIFServerApp *m_ptrMainApp;
	CString Adapter;
	CIFServerDlg(CWnd* pParent = NULL);	// standard constructor
	~CIFServerDlg();

// Dialog Data
	//{{AFX_DATA(CIFServerDlg)
	enum { IDD = IDD_IFSERVER_DIALOG };
	CStatic	m_SelAdaptor;
	CStatic	m_boundingFrame;
	//}}AFX_DATA

	void SetTitle(CString t)   {m_csTitle = t;}
	void SetConstantText(CString t)   {m_csConstantText = t;}
	void SetValidateBeforeChangingPages(bool b)	{m_bValidateBeforeChangingPage = b;}
	bool AddPage(CIFServerSubDlg &page, const char *pCaption, CIFServerSubDlg *pDlgParent = NULL);
	bool ShowPage(int iPage);
	bool ShowPage(CIFServerSubDlg * pPage);
	void EndSpecial(UINT res, bool bOk = true);
	void SetStartPage(CIFServerSubDlg *pPage = NULL) {m_pStartPage = pPage;}

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIFServerDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	
	bool EndOK();

	CIFServerStatic	   m_captionBar;
	CTreeCtrl	      m_pageTree;
	HTREEITEM FindHTREEItemForDlg(CIFServerSubDlg *pParent);
	// Generated message map functions
	//{{AFX_MSG(CIFServerDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonStartengine();
	afx_msg void OnButtonStopengine();
	afx_msg void OnButtonSelectadaptor();
	afx_msg void OnSelchangedPageTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfoPageTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnPhelp();
	afx_msg void OnSelchangingPageTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
	afx_msg long OnChangePage(UINT, LONG);
	afx_msg long OnSetFocusWnd(UINT, LONG);
	DECLARE_MESSAGE_MAP()
	CPtrArray   m_pages;
	int         m_iCurPage;
	CRect       m_frameRect;
	CString     m_csTitle, m_csConstantText;
	bool m_bValidateBeforeChangingPage;
	CIFServerSubDlg	*m_pStartPage;
	CMap< CIFServerSubDlg *, CIFServerSubDlg *, DWORD, DWORD&  > m_dlgMap;

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IFSERVERDLG_H__606F5293_783B_48BD_8EF9_10E8A00BEFA3__INCLUDED_)
