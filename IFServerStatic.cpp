// IFServerStatic.cpp: implementation of the CIFServerStatic class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "IFServerStatic.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CIFServerStatic::CIFServerStatic()
{
	m_textClr = ::GetSysColor (COLOR_3DFACE);
	m_fontWeight = FW_NORMAL;
	m_fontSize = 12;

}

CIFServerStatic::~CIFServerStatic()
{
   if (m_bm.GetSafeHandle())
   {
      m_bm.DeleteObject();
   }

	if (m_captionFont.GetSafeHandle())
   {
		m_captionFont.DeleteObject();
   }

	if (m_nameFont.GetSafeHandle())
   {
		m_nameFont.DeleteObject();
   }
}

BEGIN_MESSAGE_MAP(CIFServerStatic, CStatic)
	//{{AFX_MSG_MAP(CIFServerStatic)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
   ON_MESSAGE( WM_SETTEXT, OnSetText )
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSAPrefsStatic message handlers

void CIFServerStatic::OnPaint() 
{
   CPaintDC dc(this); 
	CFont *pOldFont = NULL;

	if (m_csFontName=="")
	{
		CWnd *pWndParent = GetParent();
		if (pWndParent)
		{
			dc.SelectObject (pWndParent->GetFont());
		}
	}
	else
	{
		if (m_captionFont.GetSafeHandle()==NULL)
		{
			m_captionFont.CreateFont( m_fontSize, 
												0, 0, 0, 
												m_fontWeight,
												0, 0, 0, ANSI_CHARSET,
												OUT_DEFAULT_PRECIS,
												CLIP_DEFAULT_PRECIS,
												DEFAULT_QUALITY,
												FF_MODERN,
												m_csFontName);
		}

		if (m_captionFont.GetSafeHandle()!=NULL)
			pOldFont = dc.SelectObject(&m_captionFont);
	}

	CString strText;
	GetWindowText(strText);

   dc.SetTextColor(m_textClr);
   dc.SetBkMode(TRANSPARENT);

   CRect cr;
   GetClientRect(cr); 

	cr.left+=5;
	dc.DrawText( strText, cr, DT_SINGLELINE | DT_LEFT | DT_VCENTER);

	if (pOldFont)
		dc.SelectObject(pOldFont);
}

BOOL CIFServerStatic::OnEraseBkgnd(CDC* pDC) 
{
   if (!m_bm.GetSafeHandle())
   {
      MakeCaptionBitmap();
   }

   if (m_bm.GetSafeHandle())
   {
      CRect cr;
      GetClientRect(cr);

      CDC memDC;
      memDC.CreateCompatibleDC(pDC);

      CBitmap *pOB = memDC.SelectObject(&m_bm);

      pDC->BitBlt(0,0,cr.Width(), cr.Height(), &memDC, 0,0, SRCCOPY);

      memDC.SelectObject(pOB);

   }

	return TRUE;
}

BOOL CIFServerStatic::OnSetText(WPARAM wParam, LPARAM lParam)
{
	DefWindowProc (WM_SETTEXT, wParam, lParam);
	Invalidate(TRUE);
	return (TRUE);
}

static void PaintRect(CDC& dc, int x, int y, int w, int h, COLORREF color)
{
	CBrush brush(color);
	CBrush* pOldBrush = dc.SelectObject(&brush);
	dc.PatBlt(x, y, w, h, PATCOPY);
	dc.SelectObject(pOldBrush);
}

void CIFServerStatic::MakeCaptionBitmap()
{
	if (m_bm.m_hObject)
		return;								   

   CRect cr;
   GetClientRect(cr);
   int w = cr.Width();
   int h = cr.Height();

	CWindowDC dcWin(this);
	CDC dc;
	dc.CreateCompatibleDC(&dcWin);
	m_bm.DeleteObject();
	m_bm.CreateCompatibleBitmap(&dcWin, w, h);
	CBitmap* pOldBitmap = dc.SelectObject(&m_bm);

   COLORREF clrBG = ::GetSysColor(COLOR_3DFACE);
	int r = GetRValue(clrBG);				
	int g = GetGValue(clrBG);				
	int b = GetBValue(clrBG);				
	int x = 8*cr.right/8;					
	int w1 = x - cr.left;					

   int NCOLORSHADES = 128;		

	int xDelta= max( w / NCOLORSHADES , 1);	

	PaintRect(dc, x, 0, cr.right-x, h, clrBG);

	while (x > xDelta) 
   {										
		x -= xDelta;						
		int wmx2 = (w1-x)*(w1-x);			
		int w2  = w1*w1;					
		PaintRect(dc, x, 0, xDelta, h,	
			RGB(r-(r*wmx2)/w2, g-(g*wmx2)/w2, b-(b*wmx2)/w2));
	}

	PaintRect(dc,0,0,x,h,RGB(0,0,0));  

	if (m_nameFont.GetSafeHandle()==NULL)
	{
		m_nameFont.CreateFont( 18, 0, 0, 0, FW_BOLD,
											0, 0, 0, ANSI_CHARSET,
											OUT_DEFAULT_PRECIS,
											CLIP_DEFAULT_PRECIS,
											DEFAULT_QUALITY,
											FF_MODERN,
											m_csFontName);	
	}

	CFont * OldFont = dc.SelectObject(&m_nameFont);
	cr.right-=5;
	dc.SetBkMode(TRANSPARENT);
	dc.SetTextColor( ::GetSysColor( COLOR_3DHILIGHT));
	dc.DrawText( m_csConstantText, cr + CPoint(1,1), DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	dc.SetTextColor( ::GetSysColor( COLOR_3DSHADOW));
	dc.DrawText( m_csConstantText, cr, DT_SINGLELINE | DT_RIGHT | DT_VCENTER);
	dc.SelectObject(OldFont);
	dc.SelectObject(pOldBitmap);
}

