// IFServerStatic.h: interface for the CIFServerStatic class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IFSERVERSTATIC_H__70EE3AB5_23F6_40FD_896C_92C93935233C__INCLUDED_)
#define AFX_IFSERVERSTATIC_H__70EE3AB5_23F6_40FD_896C_92C93935233C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CIFServerStatic : public CStatic  
{
public:
	CIFServerStatic();
    CString m_csFontName;
	void SetConstantText(const char *pText)	{m_csConstantText = pText;}
	int m_fontSize, m_fontWeight;
	BOOL m_grayText;
	COLORREF m_textClr;
	virtual ~CIFServerStatic();

protected:
	CFont m_captionFont, m_nameFont;
	CBitmap m_bm;
	CString m_csConstantText;
	void MakeCaptionBitmap();
	// Generated message map functions

protected:
	//{{AFX_MSG(CIFServerStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg BOOL OnSetText (WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before t

#endif // !defined(AFX_IFSERVERSTATIC_H__70EE3AB5_23F6_40FD_896C_92C93935233C__INCLUDED_)
