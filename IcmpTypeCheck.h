// IcmpTypeCheck.h: interface for the CIcmpTypeCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICMPTYPECHECK_H__D8247BCB_E9B1_418A_8001_64F8BF691DBD__INCLUDED_)
#define AFX_ICMPTYPECHECK_H__D8247BCB_E9B1_418A_8001_64F8BF691DBD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_ICMP_TYPE  3

class CIcmpTypeCheck : public CManipulate  
{
public:
	void SetupIcmpTypeCheck();
	void IcmpTypeCheckInit(char *data, OptTreeNode *otn, int protocol);
	void ParseIcmpType(char *data, OptTreeNode *otn);
	int IcmpTypeCheck(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	CIcmpTypeCheck();
	virtual ~CIcmpTypeCheck();

};

#endif // !defined(AFX_ICMPTYPECHECK_H__D8247BCB_E9B1_418A_8001_64F8BF691DBD__INCLUDED_)
