// IcmpIdCheck.h: interface for the CIcmpIdCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICMPIDCHECK_H__4E07BE68_2488_404D_A47A_235626A150EE__INCLUDED_)
#define AFX_ICMPIDCHECK_H__4E07BE68_2488_404D_A47A_235626A150EE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_ICMP_ID_CHECK  12

class CIcmpIdCheck : public CManipulate  
{
public:
	void SetupIcmpIdCheck();
	void IcmpIdCheckInit(char *data, OptTreeNode *otn, int protocol);
	int IcmpIdCheck(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	void ParseIcmpId(char *data, OptTreeNode *otn);
	CIcmpIdCheck();
	virtual ~CIcmpIdCheck();

};

#endif // !defined(AFX_ICMPIDCHECK_H__4E07BE68_2488_404D_A47A_235626A150EE__INCLUDED_)
