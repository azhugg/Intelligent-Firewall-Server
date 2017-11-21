// IFServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "IFServerDlg.h"
#include "SelectAdaptor.h"
#include "resource.h"
#include "FileManagement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CIFServerDlg dialog

CIFServerDlg::CIFServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIFServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIFServerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	CFileManagement objFile;
	objFile.LoadConfigurationFile();
	Adapter="";

	m_iCurPage = -1;
	m_pages.RemoveAll();
	
	m_pStartPage = NULL;
	
	m_csTitle = "Untitled";

	m_bValidateBeforeChangingPage = false;

	m_ptrMainApp = PTRAPP;

	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

}

CIFServerDlg::~CIFServerDlg()
{
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		if (pPS)
			delete pPS;
	}
}


void CIFServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIFServerDlg)
	DDX_Control(pDX, IDC_PAGE_TREE, m_pageTree);
	DDX_Control(pDX, IDC_DLG_FRAME, m_boundingFrame);
	DDX_Control(pDX, IDC_CAPTION_BAR, m_captionBar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CIFServerDlg, CDialog)

	//{{AFX_MSG_MAP(CIFServerDlg)
	ON_WM_CREATE()
	ON_BN_CLICKED(IDC_BUTTON_STARTENGINE, OnButtonStartengine)
	ON_BN_CLICKED(IDC_BUTTON_STOPENGINE, OnButtonStopengine)
	ON_BN_CLICKED(IDC_BUTTON_SELECTADAPTOR, OnButtonSelectadaptor)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PAGE_TREE, OnSelchangedPageTree)
	ON_NOTIFY(TVN_GETDISPINFO, IDC_PAGE_TREE, OnGetdispinfoPageTree)
	ON_BN_CLICKED(IDC_PHELP, OnPhelp)
	ON_NOTIFY(TVN_SELCHANGING, IDC_PAGE_TREE, OnSelchangingPageTree)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CHANGE_PAGE, OnChangePage)
	ON_MESSAGE(WM_SET_FOCUS_WND, OnSetFocusWnd)

END_MESSAGE_MAP()


void CIFServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); 

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		dc.DrawIcon(x, y, m_hIcon);
	}else{
		CDialog::OnPaint();
	}
}

HCURSOR CIFServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CIFServerDlg::OnButtonStartengine() 
{
	// TODO: Add your control notification handler code here
	CFileManagement objFileM;
	objFileM.SaveConfigurationFile();
}

void CIFServerDlg::OnButtonStopengine() 
{
	// TODO: Add your control notification handler code here
}

void CIFServerDlg::OnButtonSelectadaptor() 
{
	// TODO: Add your control notification handler code here
	CSelectAdaptor dlg;
	dlg.m_Adapter=Adapter;
	if (dlg.DoModal()==IDOK) Adapter=dlg.m_Adapter;
	CWnd *ptrDlg = this;
	CString titlestr;
	CString tmpstr;
	tmpstr=Adapter.Right(100);
	if(tmpstr.GetLength()==Adapter.GetLength())
	 titlestr.Format("IFServer-%s",Adapter);
	else
	 titlestr.Format("IFServer-...%s",tmpstr);
	m_SelAdaptor.SetWindowText(titlestr);
}

BOOL CIFServerDlg::PreTranslateMessage(MSG* pMsg) 
{
	ASSERT(pMsg != NULL);
	ASSERT_VALID(this);
	ASSERT(m_hWnd != NULL);
	
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return TRUE;
	}

	if (CWnd::PreTranslateMessage(pMsg))
		return TRUE;
	
	CFrameWnd* pFrameWnd = GetTopLevelFrame();
	if (pFrameWnd != NULL && pFrameWnd->m_bHelpMode)
		return FALSE;
	
	pFrameWnd = GetParentFrame();
	while (pFrameWnd != NULL){
		if (pFrameWnd->PreTranslateMessage(pMsg))
			return TRUE;
		pFrameWnd = pFrameWnd->GetParentFrame();
	}
	return PreTranslateInput(pMsg);
}

/////////////////////////////////////////////////////////////////////////////

int CIFServerDlg::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
BOOL CIFServerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	long l = GetWindowLong(m_pageTree.m_hWnd, GWL_STYLE);
#if (_MSC_VER > 1100)
	l = l | TVS_TRACKSELECT ;
#else
	l = l | 0x0200;
#endif
	SetWindowLong(m_pageTree.m_hWnd, GWL_STYLE, l);
	m_boundingFrame.GetWindowRect(m_frameRect);
	ScreenToClient(m_frameRect);
	m_frameRect.DeflateRect(2,2);
	
	SetWindowText(m_csTitle);
	m_captionBar.m_textClr     = ::GetSysColor(COLOR_3DFACE);
	m_captionBar.m_fontWeight  = FW_BOLD;
	m_captionBar.m_fontSize    = 14;
	m_captionBar.m_csFontName  = "Verdana";
	m_captionBar.SetConstantText(m_csConstantText);
	
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		ASSERT(pPS->pDlg);
		if (pPS)
		{
			TV_INSERTSTRUCT tvi;
			
			tvi.hParent = FindHTREEItemForDlg(pPS->pDlgParent);
			
			tvi.hInsertAfter = TVI_LAST;
			tvi.item.cchTextMax = 0;
			tvi.item.pszText = LPSTR_TEXTCALLBACK;
			tvi.item.lParam = (long)pPS;
			tvi.item.mask = TVIF_PARAM | TVIF_TEXT;
			
			HTREEITEM hTree = m_pageTree.InsertItem(&tvi);
			
			if (hTree)
			{
				DWORD dwTree = (DWORD)hTree;
				m_dlgMap.SetAt(pPS->pDlg, dwTree);
			}
		}
	}
	
	if (m_pStartPage==NULL)
	{
		if (ShowPage(0))
		{
			m_iCurPage = 0;   
		}
	}
	else
	{
		for (int i=0;i<m_pages.GetSize();i++)
		{
			pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
			ASSERT(pPS);
			if (pPS)
			{
				ASSERT(pPS->pDlg);
				if (pPS->pDlg == m_pStartPage)
				{
					ShowPage(i);
					m_iCurPage = i;
					break;
				}
			}
		}
	}
	
	return TRUE;  
}

/////////////////////////////////////////////////////////////////////////////

HTREEITEM CIFServerDlg::FindHTREEItemForDlg(CIFServerSubDlg *pParent)
{
	if (pParent==NULL)
	{
		return TVI_ROOT;
	}
	else
	{
		DWORD dwHTree;
		if (m_dlgMap.Lookup(pParent, dwHTree))
		{
			return (HTREEITEM)dwHTree;
		}
		else
		{
			ASSERT(FALSE);
			return TVI_ROOT;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

bool CIFServerDlg::AddPage(CIFServerSubDlg &dlg, const char *pCaption, CIFServerSubDlg* pDlgParent /*=NULL*/)
{
	if (m_hWnd)
	{
		ASSERT(0);
		return false;
	}
	
	pageStruct *pPS = new pageStruct;
	pPS->pDlg = &dlg;
	pPS->id = dlg.GetID();
	pPS->csCaption = pCaption;
	pPS->pDlgParent = pDlgParent;
	
	m_pages.Add(pPS);
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////

BOOL CIFServerDlg::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;
	
	cs.lpszClass = AfxRegisterWndClass(CS_DBLCLKS, NULL, NULL, NULL);
	cs.style |= WS_CLIPCHILDREN;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

bool CIFServerDlg::ShowPage(CIFServerSubDlg * pPage)
{
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg == pPage)
			{
				ShowPage(i);
				m_iCurPage = i;
				return true;
			}
		}
	}
	return false;
}

/////////////////////////////////////////////////////////////////////////////

bool CIFServerDlg::ShowPage(int iPage)
{
	if(iPage != NULL)
	{
		m_temp = m_ptrMainApp->pv.user_name;
		m_temp.MakeLower();
		if(m_temp == "administrator")
		{
			m_temp = m_ptrMainApp->pv.password;
			m_temp.MakeLower();

			if(m_temp != m_ptrMainApp->pv.checkpass)
			{
				AfxMessageBox("Wrong Password",MB_OK,0);
				return false;
			}
		}else{
			AfxMessageBox("No Such User",MB_OK,0);
			return false;
		}
	}
		m_captionBar.SetWindowText("");
	
	if ((m_iCurPage >= 0) && (m_iCurPage < m_pages.GetSize()))
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(m_iCurPage);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->ShowWindow(SW_HIDE);
				}
			}
		}
		else
		{
			return false;
		}
	}
	
	if ((iPage >= 0) && (iPage < m_pages.GetSize()))
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(iPage);
		ASSERT(pPS);
		
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				m_captionBar.SetWindowText(pPS->csCaption);
				if (!::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->Create(pPS->pDlg->GetID(), this);
				}
			
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->MoveWindow(m_frameRect.left, m_frameRect.top, m_frameRect.Width(), m_frameRect.Height());
					pPS->pDlg->ShowWindow(SW_SHOW);
					pPS->pDlg->SetFocus();
				}
				
				HTREEITEM hItem = FindHTREEItemForDlg(pPS->pDlg);
				if (hItem)
				{
					m_pageTree.SelectItem(hItem);
				}
				return true;
			}
		}
	}
	

	return false;
}

void CIFServerDlg::OnOK() 
{
	if (EndOK())
	{
		CDialog::OnOK();
	}
}

bool CIFServerDlg::EndOK()
{
	bool bOK = true;
	
	CIFServerSubDlg * pPage = NULL;

	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					if (!pPS->pDlg->UpdateData(TRUE))
					{
						bOK = false;
						pPage = pPS->pDlg;
						break;
					}
				}
			}
		}
	}
	
	if ((!bOK) && (pPage!=NULL))
	{
		ShowPage(pPage);
		return false;
	}
	
	for (i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->OnOK();
				}
			}
		}
	}
	
	return true;
}

/////////////////////////////////////////////////////////////////////////////

void CIFServerDlg::OnCancel() 
{
	for (int i=0;i<m_pages.GetSize();i++)
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(i);
		ASSERT(pPS);
		
		if (pPS)
		{
			ASSERT(pPS->pDlg);
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					pPS->pDlg->OnCancel();
				}
			}
		}
	}
	
	CDialog::OnCancel();
}

/////////////////////////////////////////////////////////////////////////////

void CIFServerDlg::EndSpecial(UINT res, bool bOk)
{
	if (bOk)
	{
		EndOK();
	}
	
	EndDialog(res);
}
/////////////////////////////////////////////////////////////////////////////

void CIFServerDlg::OnSelchangingPageTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	if (m_bValidateBeforeChangingPage)
	{
		if ((m_iCurPage >= 0) && (m_iCurPage < m_pages.GetSize()))
		{
			pageStruct *pPS = (pageStruct *)m_pages.GetAt(m_iCurPage);
			if (pPS)
			{
				ASSERT(pPS->pDlg);
				if (pPS->pDlg)
				{
					if (::IsWindow(pPS->pDlg->m_hWnd))
					{
						if (!pPS->pDlg->UpdateData())
						{  
							CWnd *pLost = pPS->pDlg->GetFocus();
							if (pLost)
							{
								PostMessage(WM_SET_FOCUS_WND, (UINT)pLost->m_hWnd);
							}
							*pResult = 1;
							return;
						}
					}
				}
			}
		}
	}
	*pResult = 0;
}
/////////////////////////////////////////////////////////////////////////////

void CIFServerDlg::OnSelchangedPageTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	
	if (pNMTreeView->itemNew.lParam)
	{
		int iIdx = -1;
		for (int i=0;i<m_pages.GetSize();i++)
		{
			if (m_pages.GetAt(i)==(pageStruct *)pNMTreeView->itemNew.lParam)
			{
				iIdx = i;
				break;
			}
		}
		if ((iIdx >= 0) && (iIdx < m_pages.GetSize()))
		{
			pageStruct *pPS = (pageStruct *)m_pages.GetAt(iIdx);
			if (m_iCurPage!=iIdx)
			{
				PostMessage(WM_CHANGE_PAGE, iIdx);
			}
		}
	}
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////

void CIFServerDlg::OnGetdispinfoPageTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	
	if (pTVDispInfo->item.lParam)
	{
		if (pTVDispInfo->item.mask & TVIF_TEXT)
		{
			pageStruct *pPS = (pageStruct *)pTVDispInfo->item.lParam;
			strcpy(pTVDispInfo->item.pszText, pPS->csCaption);
		}
	}
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////

void CIFServerDlg::OnPhelp() 
{
	if ((m_iCurPage >= 0) && (m_iCurPage < m_pages.GetSize()))
	{
		pageStruct *pPS = (pageStruct *)m_pages.GetAt(m_iCurPage);
		ASSERT(pPS);
		ASSERT(pPS->pDlg);
		if (pPS)
		{
			if (pPS->pDlg)
			{
				if (::IsWindow(pPS->pDlg->m_hWnd))
				{
					NMHDR nm;
					nm.code=PSN_HELP;
					nm.hwndFrom=m_hWnd;
					nm.idFrom=CIFServerDlg::IDD;
					pPS->pDlg->SendMessage(WM_NOTIFY, 0, (long)&nm);
				}
			}
		}
	}
}

/////////////////////////////////////////////////////////////////////////////

LONG CIFServerDlg::OnChangePage(UINT u, LONG l)
{
	if (ShowPage(u))
	{
		m_iCurPage = u;   
	}
	
	return 0L;
}

/////////////////////////////////////////////////////////////////////////////

LONG CIFServerDlg::OnSetFocusWnd(UINT u, LONG l)
{
	if (::IsWindow((HWND)u))
	{
		::SetFocus((HWND)u);
	}

	return 0L;
}

void CIFServerDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialog::OnShowWindow(bShow, nStatus);
	m_ptrMainApp->AlertStatus = 0;	
}
