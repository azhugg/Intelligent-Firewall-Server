// TtlCheck.h: interface for the CTtlCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TTLCHECK_H__4DA68D1E_8663_40B7_B400_78527AF0F1F8__INCLUDED_)
#define AFX_TTLCHECK_H__4DA68D1E_8663_40B7_B400_78527AF0F1F8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_TTL_CHECK  5

class CTtlCheck:public CManipulate
{
public:
	void SetupTtlCheck();
	void TtlCheckInit(char *data, OptTreeNode *otn, int protocol);
	int CheckTtlEq(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	void ParseTtl(char *data, OptTreeNode *otn);
	CTtlCheck();
	virtual ~CTtlCheck();

};

#endif // !defined(AFX_TTLCHECK_H__4DA68D1E_8663_40B7_B400_78527AF0F1F8__INCLUDED_)
