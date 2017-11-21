// SelectAdaptor.cpp : implementation file
//

#include "stdafx.h"
#include "IFServer.h"
#include "SelectAdaptor.h"


#include "CapDll.h"
#include "LineColl.h"
#include "pcap.h"

#define SA_ERR "Unable to choose Adaptors"
#define SA_ADS "Network Cards"
#define MFR_TCPDUMPERR "Unable to Capture on this operating system"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectAdaptor dialog


CSelectAdaptor::CSelectAdaptor(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectAdaptor::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectAdaptor)
	//}}AFX_DATA_INIT
}


void CSelectAdaptor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectAdaptor)
	DDX_Control(pDX, IDC_ADAPTER, m_CAdapter);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectAdaptor, CDialog)
	//{{AFX_MSG_MAP(CSelectAdaptor)
	ON_NOTIFY(HDN_ITEMCLICK, IDC_LIST1, OnSelectItem)
	ON_NOTIFY(HDN_ITEMDBLCLICK, IDC_LIST1, Ondblclickitem)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectAdaptor message handlers

void CSelectAdaptor::AddItem(int nItem, int nSubItem, LPCTSTR strItem, int nImageIndex)
{
	LV_ITEM lvItem;
	lvItem.mask = LVIF_TEXT;
	lvItem.iItem = nItem;
	lvItem.iSubItem = nSubItem;
	lvItem.pszText = (LPTSTR) strItem;
	if(nImageIndex != -1){
		lvItem.mask |= LVIF_IMAGE;
		lvItem.iImage = nImageIndex;
	}
	if(nSubItem == 0)
		m_ListCtrl.InsertItem(&lvItem);
	else m_ListCtrl.SetItem(&lvItem);
}

void CSelectAdaptor::Ondblclickitem(NMHDR *pNMHDR, LRESULT *pResult)
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

	AfxMessageBox("Error setting the filter");

	int i,n=m_ListCtrl.GetItemCount();
	for(i=0;i<n;i++)
	{
		if (m_ListCtrl.GetItemState(i,LVIS_SELECTED))
		{
			m_Adapter=m_ListCtrl.GetItemText(i,0);
			break;
		}
	}
	if(i==n) 
	{
		if (n!=0) m_Adapter=m_ListCtrl.GetItemText(0,0);
		else m_Adapter="";
	}

	*pResult = 0;
	SendMessage(WM_CLOSE,0,0);
}

BOOL CSelectAdaptor::OnInitDialog()
{
	char ebuf[PCAP_ERRBUF_SIZE];
	int i,n;

	CDialog::OnInitDialog();
	
	DWORD dwVersion=GetVersion();		
	DWORD dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));

	if (dwVersion >= 0x80000000 && dwWindowsMajorVersion >= 4)
	{
		char *device,*device1;
		device=(char*)pcap_lookupdev(ebuf);
		i=0;
		n=0;
		while(device[i]!=0||device[i+1]!=0){
			if(device[i]==0)n++;
			i++;
		}
		n++;
		device1=new char[i+n+2];
		i=0;
		n=0;
		while(device[i]!=0||device[i+1]!=0){
			device1[n]=device[i];
			if(device[i]==0){
				device1[n]=13;
				device1[n+1]=10;
				n++;
			}
			i++;
			n++;
		}
		device1[n]=13;
		device1[n+1]=10;
		device1[n+2]=0;
		m_Cmd=device1;
		delete [] device1;
	}else{
		WCHAR *device,*device1;
		device=(WCHAR*)pcap_lookupdev(ebuf);
		i=0;
		n=0;
		while(device[i]!=0||device[i+1]!=0){
			if(device[i]==0)n++;
			i++;
		}
		n++;
		device1=new WCHAR[i+n+2];
		i=0;
		n=0;
		while(device[i]!=0||device[i+1]!=0){
			device1[n]=device[i];
			if(device[i]==0){
				device1[n]=13;
				device1[n+1]=10;
				n++;
			}
			i++;
			n++;
		}
		device1[n]=13;
		device1[n+1]=10;
		device1[n+2]=0;
		m_Cmd=device1;
		delete [] device1;
	}
	LineCollection lc(&m_Cmd);	
    m_ListCtrl.InsertColumn(0,SA_ADS , LVCFMT_LEFT,200);
	m_ctlImage.Create(IDB_CAP_WIZ,16,0,RGB(255,0,255));
	m_ListCtrl.SetImageList(&m_ctlImage,LVSIL_SMALL);
	Update(lc);
	m_ListCtrl.SetFocus();
	m_CAdapter.SetWindowText(m_Adapter);
	return FALSE;  
}

void CSelectAdaptor::OnOK() 
{
	int i,n=m_ListCtrl.GetItemCount();
	for(i=0;i<n;i++){
		if (m_ListCtrl.GetItemState(i,LVIS_SELECTED)){
			m_Adapter=m_ListCtrl.GetItemText(i,0);
			if(m_Adapter.Find("Wan",0) != -1){
				AfxMessageBox("Interface Not Supported", MB_OK,0);
				return;
			}
			break;
		}
	}
	if(i==n){
		if (n!=0) m_Adapter=m_ListCtrl.GetItemText(0,0);
		else m_Adapter="";
	}
	CDialog::OnOK();
}

void CSelectAdaptor::OnSelectItem(NMHDR *pNMHDR, LRESULT *pResult)
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	int i,n=m_ListCtrl.GetItemCount();
	for(i=0;i<n;i++){
		if (m_ListCtrl.GetItemState(i,LVIS_SELECTED)){
			m_Adapter=m_ListCtrl.GetItemText(i,0);
			break;
		}
	}
	if(i==n){
		if (n!=0) m_Adapter=m_ListCtrl.GetItemText(0,0);
		else m_Adapter="";
	}
	m_CAdapter.SetWindowText(m_Adapter);
	*pResult = 0;
}

void CSelectAdaptor::SelectItem(int i)
{
	LV_FINDINFO it;
	it.flags=LVFI_PARAM;
	it.lParam=i;
	i=m_ListCtrl.FindItem(&it);
	m_ListCtrl.SetItemState(i,0xFFFFFFFF,LVIS_SELECTED);
}

void CSelectAdaptor::Update(LineCollection &lc)
{
	m_ListCtrl.DeleteAllItems();
	    int n=lc.getLineCount();
		CString t;
        for(n--;n>=0;n--){
			t=lc.line(n);
			t.TrimLeft();
			t.TrimRight();
			if (strlen(t)>0)
	        AddItem(0,0,t,3);
         }
		m_ListCtrl.UpdateWindow();
		n=m_ListCtrl.GetItemCount();
		for (n--;n>=0;n--){
			if (m_Adapter.CompareNoCase(m_ListCtrl.GetItemText(n,0))==0){
					SelectItem(n);
					return;
				}
		}
		m_Adapter=m_ListCtrl.GetItemText(0,0);
}
