// IcmpCodeCheck.h: interface for the CIcmpCodeCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICMPCODECHECK_H__4541F7E9_614A_4E4E_BBC0_9EB5055C6A34__INCLUDED_)
#define AFX_ICMPCODECHECK_H__4541F7E9_614A_4E4E_BBC0_9EB5055C6A34__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_ICMP_CODE  4


class CIcmpCodeCheck : public CManipulate  
{
public:
	void SetupIcmpCodeCheck();
	void IcmpCodeCheckInit(char *data, OptTreeNode *otn, int protocol);
	void ParseIcmpCode(char *data, OptTreeNode *otn);
	int IcmpCodeCheck(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	CIcmpCodeCheck();
	virtual ~CIcmpCodeCheck();

};

#endif // !defined(AFX_ICMPCODECHECK_H__4541F7E9_614A_4E4E_BBC0_9EB5055C6A34__INCLUDED_)
