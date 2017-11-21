#ifndef _BALLOON_HELP_H_INCLUDED_
#define _BALLOON_HELP_H_INCLUDED_

#include <afxmt.h>

class CBalloonHelp : public CWnd
{
public:
	CBalloonHelp();
	virtual ~CBalloonHelp();
   static const unsigned int unCLOSE_ON_LBUTTON_UP;   
   static const unsigned int unCLOSE_ON_RBUTTON_UP;   
   static const unsigned int unCLOSE_ON_MOUSE_MOVE;   
   static const unsigned int unCLOSE_ON_KEYPRESS;     
   static const unsigned int unDELETE_THIS_ON_CLOSE;  
   static const unsigned int unSHOW_CLOSE_BUTTON;     
   static const unsigned int unSHOW_INNER_SHADOW;     
   static const unsigned int unSHOW_TOPMOST;          
   static const unsigned int unDISABLE_FADEIN;        
   static const unsigned int unDISABLE_FADEOUT;       
   static const unsigned int unDISABLE_FADE;          

   BOOL Create(const CString& strTitle,         
               const CString& strContent,       
               const CPoint& ptAnchor,          
               unsigned int unOptions,          
               CWnd* pParentWnd = NULL,         
               const CString strURL = "",       
               unsigned int unTimeout = 0,      
               HICON hIcon = NULL);             
  
   static void LaunchBalloon(const CString& strTitle, const CString& strContent, 
               const CPoint& ptAnchor, 
               LPCTSTR szIcon = IDI_EXCLAMATION,
               unsigned int unOptions = unSHOW_CLOSE_BUTTON,
               CWnd* pParentWnd = NULL, 
               const CString strURL = "",
               unsigned int unTimeout = 10000);
   void SetTitleFont(CFont* pFont);
   void SetContentFont(CFont* pFont);
   void SetIcon(HICON hIcon);
   void SetIcon(HBITMAP hBitmap, COLORREF crMask);
   void SetIcon(HBITMAP hBitmap, HBITMAP hMask);
   void SetIcon(CImageList* pImageList, int nIconIndex);
   void SetURL(const CString& strURL);
   void SetTimeout(unsigned int unTimeout);
   void SetMouseMoveTolerance(int nTolerance);
   void SetAnchorPoint(CPoint ptAnchor);
   void SetTitle(const CString& strTitle);
   void SetContent(const CString& strContent);
   void SetForegroundColor(COLORREF crForeground);
   void SetBackgroundColor(COLORREF crBackground);
protected:
   static const int nTIP_TAIL;
   static const int nTIP_MARGIN;
   static const int nMOUSE_MOVE_TRIGGER;
   enum BALLOON_QUADRANT { BQ_TOPRIGHT, BQ_TOPLEFT, BQ_BOTTOMRIGHT, BQ_BOTTOMLEFT };
   BALLOON_QUADRANT GetBalloonQuadrant();
   virtual CSize DrawHeader(CDC* pDC, bool bDraw = TRUE);
   virtual CSize DrawContent(CDC* pDC, int nTop, bool bDraw = TRUE);
   CSize CalcHeaderSize(CDC* pDC) { return DrawHeader(pDC, FALSE); }
   CSize CalcContentSize(CDC* pDC) { return DrawContent(pDC, 0, FALSE); }
   CSize CalcWindowSize();
   CSize CalcClientSize();
   void PositionWindow();
   void ShowBalloon(void);
   void HideBalloon(void);
   static ATOM GetClassAtom();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnNcPaint();
	afx_msg void OnLButtonDown(UINT, CPoint point);
	afx_msg void OnLButtonUp(UINT, CPoint point);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);
	afx_msg void OnTimer(UINT nIDEvent);
   afx_msg void OnMouseMove(UINT, CPoint point);
   afx_msg void OnDestroy();
   afx_msg void OnRButtonUp(UINT, CPoint);
   afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
private:
   static LRESULT CALLBACK KeyboardProc( int code, WPARAM wParam, LPARAM lParam);
   static HHOOK            s_hKeyboardHook;
   static CPtrArray        s_apKeyboardCloseWnds;
   static CCriticalSection s_KeyboardHookSection;
   void SetKeyboardHook();
   void RemoveKeyboardHook();
   BOOL SetLayeredWindowAttributes(COLORREF crKey, int nAlpha, DWORD dwFlags);
   typedef BOOL (WINAPI* FN_SET_LAYERED_WINDOW_ATTRIBUTES)(HWND,COLORREF,BYTE,DWORD);
   FN_SET_LAYERED_WINDOW_ATTRIBUTES m_fnSetLayeredWindowAttributes;
   unsigned int   m_unOptions;
   unsigned int   m_unTimeout;      
   CString        m_strContent;     
   CString        m_strURL;         
   CPoint         m_ptAnchor;       
   CImageList     m_ilIcon;         

   CFont*         m_pTitleFont;     
   CFont*         m_pContentFont;   
   
   COLORREF       m_crBackground;   
   COLORREF       m_crForeground;   
   
   CRgn           m_rgnComplete;    
   int            m_nAlpha;         
   CPoint         m_ptMouseOrig;    
   UINT           m_uCloseState;    
   int            m_nMouseMoveTolerance;  

   static ATOM    m_ClassAtom;      
protected:
   virtual void PostNcDestroy();
};

#endif // _BALLOON_HELP_H_INCLUDED_