// IpOptionCheck.h: interface for the CIpOptionCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_IPOPTIONCHECK_H__43DDC981_E548_496B_A689_DC0D91D89462__INCLUDED_)
#define AFX_IPOPTIONCHECK_H__43DDC981_E548_496B_A689_DC0D91D89462__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_IPOPTION_CHECK  11

class CIpOptionCheck : public CManipulate  
{
public:
	void SetupIpOptionCheck();
	void IpOptionInit(char *data, OptTreeNode *otn, int protocol);
	int CheckIpOptions(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	void ParseIpOptionData(char *data, OptTreeNode *otn);
	CIpOptionCheck();
	virtual ~CIpOptionCheck();

};

#endif // !defined(AFX_IPOPTIONCHECK_H__43DDC981_E548_496B_A689_DC0D91D89462__INCLUDED_)
