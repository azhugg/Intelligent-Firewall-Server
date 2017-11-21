// IpIdCheck.h: interface for the CIpIdCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPIDCHECK_H__13A0B17C_12B1_4ED6_B3B8_92BB9F2CBD25__INCLUDED_)
#define AFX_IPIDCHECK_H__13A0B17C_12B1_4ED6_B3B8_92BB9F2CBD25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_IP_ID_CHECK  8

class CIpIdCheck : public CManipulate  
{
public:
	void SetupIpIdCheck();
	void IpIdCheckInit(char *data, OptTreeNode *otn, int protocol);
	void ParseIpId(char *data, OptTreeNode *otn);
	int IpIdCheckEq(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	CIpIdCheck();
	virtual ~CIpIdCheck();

};

#endif // !defined(AFX_IPIDCHECK_H__13A0B17C_12B1_4ED6_B3B8_92BB9F2CBD25__INCLUDED_)
