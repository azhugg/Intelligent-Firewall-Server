// MinFrag.h: interface for the CMinFrag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINFRAG_H__3820095E_A809_42EC_9318_303665222F46__INCLUDED_)
#define AFX_MINFRAG_H__3820095E_A809_42EC_9318_303665222F46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MINFRAG_ALERT_MESSAGE "Tiny Fragments - Possible Hostile Activity"

class CMinFrag : public CManipulate  
{
public:
	void SetupMinfrag();
	void MinfragInit(u_char *args);
	void CheckMinfrag(Packet *p);
	void ProcessMinfragArgs(u_char *args);
	CMinFrag();
	virtual ~CMinFrag();

};

#endif // !defined(AFX_MINFRAG_H__3820095E_A809_42EC_9318_303665222F46__INCLUDED_)
