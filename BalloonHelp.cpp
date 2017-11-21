#include "stdafx.h"
#include "BalloonHelp.h"
#include <atlbase.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#ifndef DFCS_HOT
#define DFCS_HOT 0x1000
#endif

#ifndef WS_EX_LAYERED
#define WS_EX_LAYERED   0x00080000
#define LWA_COLORKEY    0x00000001
#define LWA_ALPHA       0x00000002
#endif

#ifndef CS_DROPSHADOW
#define CS_DROPSHADOW   0x00020000
#endif

#ifndef SPI_GETDROPSHADOW
#define SPI_GETDROPSHADOW  0x1024
#endif

#ifndef SPI_GETTOOLTIPANIMATION
#define SPI_GETTOOLTIPANIMATION 0x1016
#endif

#ifndef SPI_GETTOOLTIPFADE
#define SPI_GETTOOLTIPFADE 0x1018
#endif

#define ID_TIMER_SHOW      3333
#define ID_TIMER_HIDE      3334
#define ID_TIMER_CLOSE     3335
#define ID_TIMER_HOTTRACK  3336

const unsigned int   CBalloonHelp::unCLOSE_ON_LBUTTON_UP = 0x001;
const unsigned int   CBalloonHelp::unCLOSE_ON_RBUTTON_UP = 0x002;
const unsigned int   CBalloonHelp::unCLOSE_ON_MOUSE_MOVE = 0x004;
const unsigned int   CBalloonHelp::unCLOSE_ON_KEYPRESS   = 0x008;
const unsigned int   CBalloonHelp::unDELETE_THIS_ON_CLOSE= 0x010;
const unsigned int   CBalloonHelp::unSHOW_CLOSE_BUTTON   = 0x020;
const unsigned int   CBalloonHelp::unSHOW_INNER_SHADOW   = 0x040;
const unsigned int   CBalloonHelp::unSHOW_TOPMOST        = 0x080;
const unsigned int   CBalloonHelp::unDISABLE_FADEIN      = 0x100;
const unsigned int   CBalloonHelp::unDISABLE_FADEOUT     = 0x200;
const unsigned int   CBalloonHelp::unDISABLE_FADE        = CBalloonHelp::unDISABLE_FADEIN|CBalloonHelp::unDISABLE_FADEOUT;


const int            CBalloonHelp::nTIP_TAIL             = 20;
const int            CBalloonHelp::nTIP_MARGIN           = 8;

ATOM                 CBalloonHelp::m_ClassAtom           = NULL;

CCriticalSection     CBalloonHelp::s_KeyboardHookSection;

CPtrArray            CBalloonHelp::s_apKeyboardCloseWnds;

HHOOK                CBalloonHelp::s_hKeyboardHook       = NULL;

CBalloonHelp::CBalloonHelp()
:  m_fnSetLayeredWindowAttributes(NULL),
   m_unOptions(0),
   m_unTimeout(0),
   m_strURL(""),
   m_ptAnchor(0,0),
   m_strContent(""),
   m_nAlpha(255),
   m_nMouseMoveTolerance(3),
   m_ptMouseOrig(0,0),
   m_uCloseState(0),
   m_pTitleFont(NULL),
   m_pContentFont(NULL),
   m_crForeground(::GetSysColor(COLOR_INFOTEXT)),
   m_crBackground(::GetSysColor(COLOR_INFOBK))
{

   HMODULE hUser32 = GetModuleHandle(_T("USER32.DLL"));
   m_fnSetLayeredWindowAttributes = (FN_SET_LAYERED_WINDOW_ATTRIBUTES)GetProcAddress(hUser32, _T("SetLayeredWindowAttributes"));
}

CBalloonHelp::~CBalloonHelp()
{
   if ( NULL != m_pTitleFont )
      delete m_pTitleFont;
   m_pTitleFont = NULL;
   if ( NULL != m_pContentFont )
      delete m_pContentFont;
   m_pContentFont = NULL;
}


void CBalloonHelp::LaunchBalloon(const CString& strTitle, const CString& strContent, 
               const CPoint& ptAnchor, 
               LPCTSTR szIcon ,
               unsigned int unOptions ,
               CWnd* pParentWnd ,
               const CString strURL ,
               unsigned int unTimeout)
{
   CBalloonHelp* pbh = new CBalloonHelp;
   if ( NULL != szIcon )
   {
      HICON hIcon = (HICON)::LoadImage(NULL, szIcon, IMAGE_ICON, 16,16, LR_SHARED);
      if (NULL != hIcon)
      {
         
         CDC dc;
         CDC dcTmp1;
         CDC dcTmp2;
         CBitmap bmpIcon;
         CBitmap bmpIconSmall;
         dc.Attach(::GetDC(NULL));
         dcTmp1.CreateCompatibleDC(&dc);
         dcTmp2.CreateCompatibleDC(&dc);
         bmpIcon.CreateCompatibleBitmap(&dc, 32,32);
         bmpIconSmall.CreateCompatibleBitmap(&dc, 16,16);
         ::ReleaseDC(NULL, dc.Detach());
         CBitmap* pbmpOld1 = dcTmp1.SelectObject(&bmpIcon);
         CBitmap* pbmpOld2 = dcTmp2.SelectObject(&bmpIconSmall);
         dcTmp1.FillSolidRect(0,0,32,32, pbh->m_crBackground);
         ::DrawIconEx(dcTmp1, 0,0,hIcon,32,32,0,NULL,DI_NORMAL);
         dcTmp2.SetStretchBltMode(HALFTONE);
         dcTmp2.StretchBlt(0,0,16,16,&dcTmp1, 0,0,32,32,SRCCOPY);
         dcTmp1.SelectObject(pbmpOld1);
         dcTmp2.SelectObject(pbmpOld2);

         pbh->SetIcon(bmpIconSmall, pbh->m_crBackground);
      }
   }

   pbh->Create(strTitle, strContent, ptAnchor, unOptions|unDELETE_THIS_ON_CLOSE, 
               pParentWnd, strURL, unTimeout, NULL);
}


void CBalloonHelp::SetTitleFont(CFont* pFont)
{
   if ( NULL != m_pTitleFont )
      delete m_pTitleFont;
   m_pTitleFont = pFont;
   if ( NULL != m_hWnd )
      PositionWindow();
}

void CBalloonHelp::SetContentFont(CFont* pFont)
{
   if ( NULL != m_pContentFont )
      delete m_pContentFont;
   m_pContentFont = pFont;
   if ( NULL != m_hWnd )
      PositionWindow();
}

void CBalloonHelp::SetIcon(HICON hIcon)
{
   if ( NULL != m_ilIcon.m_hImageList )
      m_ilIcon.DeleteImageList();
   ICONINFO iconinfo;
   if ( NULL != hIcon && ::GetIconInfo(hIcon, &iconinfo) )
   {
      SetIcon(iconinfo.hbmColor, iconinfo.hbmMask);
      ::DeleteObject(iconinfo.hbmColor);
      ::DeleteObject(iconinfo.hbmMask);
   }
   if ( NULL != m_hWnd )
      PositionWindow();
}

void CBalloonHelp::SetIcon(HBITMAP hBitmap, COLORREF crMask)
{
   if ( NULL != m_ilIcon.m_hImageList )
      m_ilIcon.DeleteImageList();

   if ( NULL != hBitmap )
   {
      BITMAP bm;
      if (::GetObject(hBitmap, sizeof(bm),(LPVOID)&bm))
      {
         m_ilIcon.Create(bm.bmWidth, bm.bmHeight, ILC_COLOR24|ILC_MASK,1,0);
         m_ilIcon.Add(CBitmap::FromHandle(hBitmap), crMask);
      }
   }
   if ( NULL != m_hWnd )
      PositionWindow();
}


void CBalloonHelp::SetIcon(HBITMAP hBitmap, HBITMAP hMask)
{
   if ( NULL != m_ilIcon.m_hImageList )
      m_ilIcon.DeleteImageList();

   ASSERT(NULL != hBitmap);
   ASSERT(NULL != hMask);

   BITMAP bm;
   if (::GetObject(hBitmap, sizeof(bm),(LPVOID)&bm))
   {
      m_ilIcon.Create(bm.bmWidth, bm.bmHeight, ILC_COLOR24|ILC_MASK,1,0);
      m_ilIcon.Add(CBitmap::FromHandle(hBitmap), CBitmap::FromHandle(hMask));
   }

   if ( NULL != m_hWnd )
      PositionWindow();
}


void CBalloonHelp::SetIcon(CImageList* pImageList, int nIconIndex)
{

   ASSERT_VALID(pImageList);
   ASSERT(nIconIndex >= 0 && nIconIndex < pImageList->GetImageCount() );

   HICON hIcon = NULL;
   if ( NULL != pImageList && nIconIndex >= 0 && nIconIndex < pImageList->GetImageCount() )
      hIcon = pImageList->ExtractIcon(nIconIndex);
   SetIcon(hIcon);
   if ( NULL != hIcon )
      ::DestroyIcon(hIcon);

   if ( NULL != m_hWnd )
      PositionWindow();
}

void CBalloonHelp::SetURL(const CString& strURL)
{
   m_strURL = strURL;
}


void CBalloonHelp::SetTimeout(unsigned int unTimeout)
{
   m_unTimeout = unTimeout;

   if ( NULL != m_hWnd )
   {
      if ( m_unTimeout > 0 )
         SetTimer(ID_TIMER_CLOSE, m_unTimeout, NULL);
      else
         KillTimer(ID_TIMER_CLOSE);
   }
}

void CBalloonHelp::SetAnchorPoint(CPoint ptAnchor)
{
   m_ptAnchor = ptAnchor;

   if ( NULL != m_hWnd )
   {
      PositionWindow();
   }
}

void CBalloonHelp::SetTitle(const CString& strTitle)
{
   SetWindowText(strTitle);
   if ( NULL != m_hWnd )
      PositionWindow();
}

void CBalloonHelp::SetContent(const CString& strContent)
{
   m_strContent = strContent;
   if ( NULL != m_hWnd )
      PositionWindow();
}

void CBalloonHelp::SetForegroundColor(COLORREF crForeground)
{
   m_crForeground = crForeground;
   if ( NULL != m_hWnd )
      Invalidate(FALSE);
}

void CBalloonHelp::SetBackgroundColor(COLORREF crBackground)
{
   m_crBackground = crBackground;

   if ( NULL != m_hWnd )
      Invalidate(FALSE);
}

void CBalloonHelp::SetMouseMoveTolerance(int nTolerance)
{
   m_nMouseMoveTolerance = nTolerance;
}

BOOL CBalloonHelp::Create(const CString& strTitle, const CString& strContent, 
               const CPoint& ptAnchor, unsigned int unOptions,
               CWnd* pParentWnd,
               const CString strURL ,
               unsigned int unTimeout,
               HICON hIcon)
{
   m_strContent   = strContent;
   m_ptAnchor     = ptAnchor;
   m_unOptions    = unOptions;
   m_strURL       = strURL;
   m_unTimeout    = unTimeout;

   if ( NULL != hIcon )
      SetIcon(hIcon);
   
   if (NULL == pParentWnd)
      pParentWnd = AfxGetMainWnd();
   if (NULL == pParentWnd)
      return FALSE;

   if ( NULL == m_pContentFont )
   {
      m_pContentFont = new CFont;
      if ( !m_pContentFont->CreateStockObject(DEFAULT_GUI_FONT) )
         return FALSE;
   }

   if ( NULL == m_pTitleFont )
   {
      m_pTitleFont = new CFont;
      LOGFONT LogFont;
      m_pContentFont->GetLogFont(&LogFont);
      LogFont.lfWeight = FW_BOLD;
      if ( !m_pTitleFont->CreateFontIndirect(&LogFont) )
         return FALSE;
   }

   if ( !GetClassAtom() )  
      return FALSE;

   BOOL bFade = FALSE;
   ::SystemParametersInfo(SPI_GETTOOLTIPANIMATION, 0, &bFade, 0);
   if (bFade)
      ::SystemParametersInfo(SPI_GETTOOLTIPFADE, 0, &bFade, 0);
   if (!bFade || NULL == m_fnSetLayeredWindowAttributes)
      m_unOptions |= unDISABLE_FADE;

   DWORD dwExStyle = WS_EX_TOOLWINDOW|WS_EX_LAYERED;
   if ( m_unOptions&unSHOW_TOPMOST )      // make topmost, if requested
      dwExStyle |= WS_EX_TOPMOST;
   if ( !CreateEx(dwExStyle, _T("BalloonHelpClass"), strTitle, WS_POPUP, CRect(0,0,10,10), pParentWnd, 0, NULL) )
      return FALSE;
   PositionWindow();

   ShowBalloon();

   if ( m_unOptions & unCLOSE_ON_MOUSE_MOVE )
   {
      ::GetCursorPos(&m_ptMouseOrig);
      ScreenToClient(&m_ptMouseOrig);
      SetTimer(ID_TIMER_HOTTRACK, 100, NULL);
   }

   if ( (m_unOptions & unCLOSE_ON_LBUTTON_UP) || (m_unOptions & unCLOSE_ON_RBUTTON_UP) )
      SetCapture();

   if ( m_unOptions&unCLOSE_ON_KEYPRESS )
      SetKeyboardHook();

   ShowWindow(SW_SHOWNOACTIVATE);

   return TRUE;
}

BOOL CBalloonHelp::SetLayeredWindowAttributes(COLORREF crKey, int nAlpha, DWORD dwFlags)
{
   if ( NULL != m_fnSetLayeredWindowAttributes )
      return (*m_fnSetLayeredWindowAttributes)(m_hWnd, crKey, (BYTE)nAlpha, dwFlags);
   return FALSE;
}

CBalloonHelp::BALLOON_QUADRANT CBalloonHelp::GetBalloonQuadrant()
{
   CRect rectDesktop;
   ::GetWindowRect(::GetDesktopWindow(), &rectDesktop);
   
   if ( m_ptAnchor.y < rectDesktop.Height()/2 )
   {
      if ( m_ptAnchor.x < rectDesktop.Width()/2 )
      {
         return BQ_TOPLEFT;
      }
      else
      {
         return BQ_TOPRIGHT;
      }
   }
   else
   {
      if ( m_ptAnchor.x < rectDesktop.Width()/2 )
      {
         return BQ_BOTTOMLEFT;
      }
      else
      {
         return BQ_BOTTOMRIGHT;
      }
   }

}

CSize CBalloonHelp::DrawHeader(CDC* pDC, bool bDraw)
{
   CSize sizeHdr(0,0);
   CRect rectClient;
   GetClientRect(&rectClient);   
                                 
   if ( NULL != m_ilIcon.m_hImageList )
   {
      int x = 0;
      int y = 0;
      ImageList_GetIconSize(m_ilIcon, &x, &y);
      sizeHdr.cx += x;
      sizeHdr.cy = max(sizeHdr.cy, y);
      m_ilIcon.SetBkColor(m_crBackground);
      if (bDraw)
         m_ilIcon.Draw(pDC, 0, CPoint(0,0), ILD_NORMAL);
      rectClient.left += x;
   }

   if ( m_unOptions & unSHOW_CLOSE_BUTTON )
   {
      if ( sizeHdr.cx > 0 )
         sizeHdr.cx += nTIP_MARGIN;
      sizeHdr.cx += 16;
      sizeHdr.cy = max(sizeHdr.cy, 16);
      if (bDraw)
         pDC->DrawFrameControl(CRect(rectClient.right-16,0,rectClient.right,16), DFC_CAPTION, DFCS_CAPTIONCLOSE|DFCS_FLAT);
      rectClient.right -= 16;
   }

   CString strTitle;
   GetWindowText(strTitle);
   if ( !strTitle.IsEmpty() )
   {
      CFont* pOldFont = pDC->SelectObject(m_pTitleFont);

      if ( sizeHdr.cx > 0 ) 
         sizeHdr.cx += nTIP_MARGIN;
      CRect rectTitle(0,0,0,0);
      pDC->DrawText(strTitle, &rectTitle, DT_CALCRECT | DT_NOPREFIX | DT_EXPANDTABS | DT_SINGLELINE);
      sizeHdr.cx += rectTitle.Width();
      sizeHdr.cy = max(sizeHdr.cy, rectTitle.Height());

     if ( bDraw )
	 {
         pDC->SetBkMode(TRANSPARENT);
         pDC->SetTextColor(m_crForeground);
         pDC->DrawText(strTitle, &rectClient, DT_CENTER | DT_NOPREFIX  | DT_EXPANDTABS | DT_SINGLELINE);
     }

      pDC->SelectObject(pOldFont);
   }

   return sizeHdr;
}

CSize CBalloonHelp::DrawContent(CDC* pDC, int nTop, bool bDraw)
{
   CRect rectContent;
   ::GetClientRect(::GetDesktopWindow(), &rectContent);

   rectContent.top = nTop;

   rectContent.right -= rectContent.Width()/2;

   CFont* pOldFont = pDC->SelectObject(m_pContentFont);
   if ( !m_strContent.IsEmpty() )
      pDC->DrawText(m_strContent, &rectContent, DT_CALCRECT | DT_LEFT | DT_NOPREFIX | DT_EXPANDTABS | DT_WORDBREAK);
   else
      rectContent.SetRectEmpty();   
   
   
   if (bDraw)
   {
      pDC->SetBkMode(TRANSPARENT);
      pDC->SetTextColor(m_crForeground);
      pDC->DrawText(m_strContent, &rectContent, DT_LEFT | DT_NOPREFIX | DT_WORDBREAK | DT_EXPANDTABS);
   }
   
   pDC->SelectObject(pOldFont);

   return rectContent.Size();
}

CSize CBalloonHelp::CalcClientSize()
{
   ASSERT(NULL != m_hWnd);
   CWindowDC dc(this);

   CSize sizeHeader = CalcHeaderSize(&dc);
   CSize sizeContent = CalcContentSize(&dc);

   return CSize(max(sizeHeader.cx,sizeContent.cx), sizeHeader.cy + nTIP_MARGIN + sizeContent.cy);
}

CSize CBalloonHelp::CalcWindowSize()
{
   CSize size = CalcClientSize();
   size.cx += nTIP_MARGIN*2;
   size.cy += nTIP_TAIL+nTIP_MARGIN*2;
   return size;
}

void CBalloonHelp::PositionWindow()
{
   CSize sizeWnd = CalcWindowSize();

   CPoint ptTail[3];
   CPoint ptTopLeft(0,0);
   CPoint ptBottomRight(sizeWnd.cx, sizeWnd.cy);

   switch (GetBalloonQuadrant())
   {
   case BQ_TOPLEFT:
      ptTopLeft.y = nTIP_TAIL;
      ptTail[0].x = (sizeWnd.cx-nTIP_TAIL)/4 + nTIP_TAIL;
      ptTail[0].y = nTIP_TAIL+1;
      ptTail[2].x = (sizeWnd.cx-nTIP_TAIL)/4;
      ptTail[2].y = ptTail[0].y;
      ptTail[1].x = ptTail[2].x;
      ptTail[1].y = 1;
      break;
   case BQ_TOPRIGHT:
      ptTopLeft.y = nTIP_TAIL;
      ptTail[0].x = (sizeWnd.cx-nTIP_TAIL)/4*3;
      ptTail[0].y = nTIP_TAIL+1;
      ptTail[2].x = (sizeWnd.cx-nTIP_TAIL)/4*3 + nTIP_TAIL;
      ptTail[2].y = ptTail[0].y;
      ptTail[1].x = ptTail[2].x;
      ptTail[1].y = 1;
      break;
   case BQ_BOTTOMLEFT:
      ptBottomRight.y = sizeWnd.cy-nTIP_TAIL;
      ptTail[0].x = (sizeWnd.cx-nTIP_TAIL)/4 + nTIP_TAIL;
      ptTail[0].y = sizeWnd.cy-nTIP_TAIL-2;
      ptTail[2].x = (sizeWnd.cx-nTIP_TAIL)/4;
      ptTail[2].y = ptTail[0].y;
      ptTail[1].x = ptTail[2].x;
      ptTail[1].y = sizeWnd.cy-2;
      break;
   case BQ_BOTTOMRIGHT:
      ptBottomRight.y = sizeWnd.cy-nTIP_TAIL;
      ptTail[0].x = (sizeWnd.cx-nTIP_TAIL)/4*3;
      ptTail[0].y = sizeWnd.cy-nTIP_TAIL-2;
      ptTail[2].x = (sizeWnd.cx-nTIP_TAIL)/4*3 + nTIP_TAIL;
      ptTail[2].y = ptTail[0].y;
      ptTail[1].x = ptTail[2].x;
      ptTail[1].y = sizeWnd.cy-2;
      break;
   }

   if ( ptTail[0].x < nTIP_MARGIN )
      ptTail[0].x = nTIP_MARGIN;
   if ( ptTail[0].x > sizeWnd.cx - nTIP_MARGIN )
      ptTail[0].x = sizeWnd.cx - nTIP_MARGIN;
   if ( ptTail[1].x < nTIP_MARGIN )
      ptTail[1].x = nTIP_MARGIN;
   if ( ptTail[1].x > sizeWnd.cx - nTIP_MARGIN )
      ptTail[1].x = sizeWnd.cx - nTIP_MARGIN;
   if ( ptTail[2].x < nTIP_MARGIN )
      ptTail[2].x = nTIP_MARGIN;
   if ( ptTail[2].x > sizeWnd.cx - nTIP_MARGIN )
      ptTail[2].x = sizeWnd.cx - nTIP_MARGIN;

   CPoint ptOffs(m_ptAnchor.x - ptTail[1].x, m_ptAnchor.y - ptTail[1].y);

   CRect rectScreen;
   ::GetWindowRect(::GetDesktopWindow(), &rectScreen);
   int nAdjustX = 0;
   int nAdjustY = 0;
   if ( ptOffs.x < 0 )
      nAdjustX = -ptOffs.x;
   else if ( ptOffs.x + sizeWnd.cx >= rectScreen.right )
      nAdjustX = rectScreen.right - (ptOffs.x + sizeWnd.cx);
   if ( ptOffs.y < 0 )
      nAdjustY = -ptOffs.y;
   else if ( ptOffs.y + sizeWnd.cy >= rectScreen.bottom )
      nAdjustY = rectScreen.bottom - (ptOffs.y + sizeWnd.cy);

   ptTail[1].x -= nAdjustX;
   ptOffs.x    += nAdjustX;
   ptOffs.y    += nAdjustY;

   MoveWindow(ptOffs.x, ptOffs.y, sizeWnd.cx, sizeWnd.cy, TRUE);

   BOOL bRegionChanged = TRUE;
   CRgn region;
   CRgn regionRound;
   CRgn regionComplete;
   region.CreatePolygonRgn(&ptTail[0], 3, ALTERNATE);
   regionRound.CreateRoundRectRgn(ptTopLeft.x,ptTopLeft.y,ptBottomRight.x,ptBottomRight.y,nTIP_MARGIN*3,nTIP_MARGIN*3);
   regionComplete.CreateRectRgn(0,0,1,1);
   regionComplete.CombineRgn(&region, &regionRound, RGN_OR);
   if ( NULL == m_rgnComplete.m_hObject )
      m_rgnComplete.CreateRectRgn(0,0,1,1);
   else if ( m_rgnComplete.EqualRgn(&regionComplete) )
      bRegionChanged = FALSE;
   m_rgnComplete.CopyRgn(&regionComplete);
   
   SetWindowRgn((HRGN)regionComplete.Detach(), TRUE);

   if ( bRegionChanged )
      RedrawWindow(NULL, NULL, RDW_UPDATENOW| RDW_ERASE | RDW_INVALIDATE | RDW_FRAME | RDW_ALLCHILDREN);
}


ATOM CBalloonHelp::GetClassAtom()
{
   if ( NULL == m_ClassAtom )
   {
      WNDCLASSEX wcx; 

      if ( NULL == afxCurrentInstanceHandle && NULL != _pModule )
      {
         afxCurrentInstanceHandle = _pModule->GetModuleInstance();
      }

      wcx.cbSize = sizeof(wcx);                 
      wcx.style = CS_DBLCLKS|CS_SAVEBITS
         |CS_DROPSHADOW;                        
      wcx.lpfnWndProc = AfxWndProc;             
      wcx.cbClsExtra = 0;                       
      wcx.cbWndExtra = 0;                       
      wcx.hInstance = AfxGetInstanceHandle();   
      wcx.hIcon = NULL;                         
      wcx.hCursor = LoadCursor(NULL,IDC_ARROW); 
      wcx.hbrBackground = ::GetSysColorBrush(COLOR_WINDOW);                 
      wcx.lpszMenuName =  NULL;                 
      wcx.lpszClassName = "BalloonHelpClass";   
      wcx.hIconSm = NULL;                       

      
      m_ClassAtom = RegisterClassEx(&wcx);
            
      if ( NULL == m_ClassAtom )
      {
         wcx.style &= ~CS_DROPSHADOW;
         m_ClassAtom = RegisterClassEx(&wcx);
      }
   }

   return m_ClassAtom;
}

void CBalloonHelp::ShowBalloon(void)
{
   m_nAlpha = 5;
   SetLayeredWindowAttributes(0, m_nAlpha, LWA_ALPHA);
   SetTimer(ID_TIMER_SHOW, 25, NULL);
}


void CBalloonHelp::HideBalloon(void)
{
   m_nAlpha = 250;
   SetTimer(ID_TIMER_HIDE, 25, NULL);
}

void CBalloonHelp::SetKeyboardHook()
{
   CSingleLock singleLock(&s_KeyboardHookSection);
   singleLock.Lock();
   if ( NULL == s_hKeyboardHook )
   {
      s_hKeyboardHook = ::SetWindowsHookEx(WH_KEYBOARD, (HOOKPROC)KeyboardProc, NULL, ::GetCurrentThreadId());
   }
   s_apKeyboardCloseWnds.Add(m_hWnd);
}

void CBalloonHelp::RemoveKeyboardHook()
{
   CSingleLock singleLock(&s_KeyboardHookSection);
   singleLock.Lock();
   INT_PTR i;
   for (i=0; i<s_apKeyboardCloseWnds.GetSize(); ++i)
   {
      if ( s_apKeyboardCloseWnds.GetAt(i) == m_hWnd )
         s_apKeyboardCloseWnds.RemoveAt(i--);
   }

   if ( s_apKeyboardCloseWnds.GetSize() == 0 && NULL != s_hKeyboardHook )
   {
      ::UnhookWindowsHookEx(s_hKeyboardHook);
      s_hKeyboardHook = NULL;
   }
}


BEGIN_MESSAGE_MAP(CBalloonHelp, CWnd)
   ON_WM_ERASEBKGND()
   ON_WM_PAINT()
   ON_WM_NCPAINT()
   ON_WM_LBUTTONDOWN()
   ON_WM_LBUTTONUP()
   ON_WM_NCCALCSIZE()
   ON_WM_TIMER()
   ON_WM_MOUSEMOVE()
   ON_WM_DESTROY()
   ON_WM_RBUTTONUP()
   ON_WM_CLOSE()
END_MESSAGE_MAP()

BOOL CBalloonHelp::OnEraseBkgnd(CDC* pDC) 
{
   CRect rect;
   GetClientRect(&rect);
   pDC->FillSolidRect(&rect, m_crBackground);
   return TRUE;
}

void CBalloonHelp::OnPaint() 
{
	CPaintDC dc(this); 
   CSize sizeHeader = DrawHeader(&dc);
   DrawContent(&dc, sizeHeader.cy+nTIP_MARGIN);
}

void CBalloonHelp::OnNcPaint() 
{
   CWindowDC dc(this);
   CRect rect;
   GetWindowRect(&rect);
   ScreenToClient(&rect);
   CRect rectClient;
   GetClientRect(&rectClient);
   rectClient.OffsetRect(-rect.left, -rect.top);
   rect.OffsetRect(-rect.left, -rect.top);
   dc.ExcludeClipRect(&rectClient);
   dc.FillSolidRect(&rect, m_crBackground);

   ASSERT(NULL != m_rgnComplete.m_hObject);
   CBrush   brushFg;
   brushFg.CreateSolidBrush(m_crForeground);
   if ( m_unOptions & unSHOW_INNER_SHADOW )
   {
      CBrush   brushHL;
      int red = 170 + GetRValue(m_crBackground)/3;
      int green = 170 + GetGValue(m_crBackground)/3;
      int blue = 170 + GetBValue(m_crBackground)/3;
      brushHL.CreateSolidBrush(RGB(red,green,blue));
      m_rgnComplete.OffsetRgn(1,1);
      dc.FrameRgn(&m_rgnComplete, &brushHL, 2, 2);
      red = GetRValue(m_crForeground)/3 + GetRValue(m_crBackground)/3*2;
      green = GetGValue(m_crForeground)/3 + GetGValue(m_crBackground)/3*2;
      blue = GetBValue(m_crForeground)/3 + GetBValue(m_crBackground)/3*2;
      brushHL.DeleteObject();
      m_rgnComplete.OffsetRgn(-2,-2);
      brushHL.CreateSolidBrush(RGB(red,green,blue));
      dc.FrameRgn(&m_rgnComplete, &brushHL, 2, 2);
      m_rgnComplete.OffsetRgn(1,1);
   }

   dc.FrameRgn(&m_rgnComplete, &brushFg, 1, 1);
}

void CBalloonHelp::OnLButtonDown(UINT, CPoint point) 
{
   if (m_unOptions & unSHOW_CLOSE_BUTTON)
   {
      CRect rect;
      GetClientRect(&rect);
      rect.left = rect.right-16;
      rect.bottom = rect.top+16;
      if ( rect.PtInRect(point) )
      {
         m_uCloseState |= DFCS_PUSHED;
         SetCapture();
         OnMouseMove(0, point);
      }
   }
}

void CBalloonHelp::OnLButtonUp(UINT, CPoint point) 
{
   if ( (m_unOptions & unSHOW_CLOSE_BUTTON) && (m_uCloseState & DFCS_PUSHED) )
   {
      ReleaseCapture();
      m_uCloseState &= ~DFCS_PUSHED;
      CRect rect;
      GetClientRect(&rect);
      rect.left = rect.right-16;
      rect.bottom = rect.top+16;
      if ( rect.PtInRect(point) )
         HideBalloon();
   }
   else if ( m_unOptions & unCLOSE_ON_LBUTTON_UP )
   {
      ReleaseCapture();
      HideBalloon();
   }
   else if ( !m_strURL.IsEmpty() )
   {
      HideBalloon();
      ::ShellExecute(NULL, NULL, m_strURL, NULL, NULL, SW_SHOWNORMAL);
   }
}

void CBalloonHelp::OnRButtonUp(UINT, CPoint)
{
   if ( m_unOptions & unCLOSE_ON_RBUTTON_UP )
   {
      ReleaseCapture();
      HideBalloon();
   }
}

void CBalloonHelp::OnMouseMove(UINT, CPoint point)
{
   if (m_unOptions & unSHOW_CLOSE_BUTTON)
   {
      CRect rect;
      GetClientRect(&rect);
      rect.left = rect.right-16;
      rect.bottom = rect.top+16;
      CClientDC dc(this);
      UINT uState = DFCS_CAPTIONCLOSE;
      BOOL bPushed = m_uCloseState&DFCS_PUSHED;
      m_uCloseState &= ~DFCS_PUSHED;
      if ( rect.PtInRect(point) )
      {
         uState |= DFCS_HOT;
         if ( bPushed )
            uState |= DFCS_PUSHED;
         SetTimer(ID_TIMER_HOTTRACK, 500, NULL);
      }
      else
      {
         uState |= DFCS_FLAT;
      }
      if ( uState != m_uCloseState )
      {
         dc.DrawFrameControl(&rect, DFC_CAPTION, uState);
         m_uCloseState = uState;
      }
      if ( bPushed )
         m_uCloseState |= DFCS_PUSHED;
   }

   if ( m_unOptions & unCLOSE_ON_MOUSE_MOVE )
   {
      if ( m_nAlpha == 255 && (abs(point.x-m_ptMouseOrig.x) > m_nMouseMoveTolerance || abs(point.y-m_ptMouseOrig.y) > m_nMouseMoveTolerance) )
         HideBalloon();
      else
         SetTimer(ID_TIMER_HOTTRACK, 100, NULL);
   }
}

void CBalloonHelp::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp) 
{
   ::InflateRect(&lpncsp->rgrc[0], -nTIP_MARGIN,-nTIP_MARGIN);

   switch ( GetBalloonQuadrant() )
   {
   case BQ_TOPRIGHT:
   case BQ_TOPLEFT:
      lpncsp->rgrc[0].top += nTIP_TAIL;
      break;
   case BQ_BOTTOMRIGHT:
   case BQ_BOTTOMLEFT:
      lpncsp->rgrc[0].bottom -= nTIP_TAIL;
      break;
   }

   if ( lpncsp->rgrc[0].right < lpncsp->rgrc[0].left )
      lpncsp->rgrc[0].right = lpncsp->rgrc[0].left;
   if ( lpncsp->rgrc[0].bottom < lpncsp->rgrc[0].top )
      lpncsp->rgrc[0].bottom = lpncsp->rgrc[0].top;

   if ( bCalcValidRects )
   {
      if ( !::EqualRect(&lpncsp->rgrc[0], &lpncsp->rgrc[2]) )
      {
         ::SetRectEmpty(&lpncsp->rgrc[2]);
      }
   }
}

void CBalloonHelp::OnTimer(UINT nIDEvent) 
{
   switch (nIDEvent)
   {
   case ID_TIMER_SHOW:
      m_nAlpha += 25;
      if ( m_nAlpha > 255 || m_unOptions&unDISABLE_FADEIN)
      {
         m_nAlpha = 255;
         KillTimer(ID_TIMER_SHOW);
         SetTimeout(m_unTimeout);     
      }
         
      SetLayeredWindowAttributes(0, m_nAlpha, LWA_ALPHA);
      break;
   case ID_TIMER_HIDE:
      
      KillTimer(ID_TIMER_SHOW);

      m_nAlpha -= 25;
      if ( m_nAlpha < 0 || m_unOptions&unDISABLE_FADEOUT )
      {
         m_nAlpha = 0;
         KillTimer(ID_TIMER_HIDE);
         DestroyWindow();
      }
      else
      {
         SetLayeredWindowAttributes(0, m_nAlpha, LWA_ALPHA);
      }
      break;
   case ID_TIMER_CLOSE:
      KillTimer(ID_TIMER_CLOSE);
      HideBalloon();
      break;
   case ID_TIMER_HOTTRACK:
      {
         CPoint point;
         KillTimer(ID_TIMER_HOTTRACK);
         ::GetCursorPos(&point);
         ScreenToClient(&point);
         OnMouseMove(0, point);
         break;
      }
   default:
      CWnd::OnTimer(nIDEvent);
      break;
   }
}


void CBalloonHelp::OnDestroy()
{
   RemoveKeyboardHook();
   CWnd::OnDestroy();
}

void CBalloonHelp::OnClose()
{
   HideBalloon();
}

LRESULT CALLBACK CBalloonHelp::KeyboardProc( int code, WPARAM wParam, LPARAM lParam)
{

   if (code >= 0 && !(lParam & 0x80000000))
   {
      CSingleLock singleLock(&CBalloonHelp::s_KeyboardHookSection);
      singleLock.Lock();
      INT_PTR nNumWnds = CBalloonHelp::s_apKeyboardCloseWnds.GetSize();
      INT_PTR i;
      for (i=0; i<nNumWnds; ++i)
         ::PostMessage((HWND)CBalloonHelp::s_apKeyboardCloseWnds.GetAt(i), WM_CLOSE, 0, 0);
      CBalloonHelp::s_apKeyboardCloseWnds.RemoveAll();
   }
   return ::CallNextHookEx(CBalloonHelp::s_hKeyboardHook, code, wParam, lParam);
}

void CBalloonHelp::PostNcDestroy()
{
   CWnd::PostNcDestroy();
   if ( m_unOptions & unDELETE_THIS_ON_CLOSE )
      delete this;
}
