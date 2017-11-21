// IcmpSeqCheck.h: interface for the CIcmpSeqCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ICMPSEQCHECK_H__0FC47D91_0215_444E_8341_AEF10E081E12__INCLUDED_)
#define AFX_ICMPSEQCHECK_H__0FC47D91_0215_444E_8341_AEF10E081E12__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_ICMP_SEQ_CHECK  13


class CIcmpSeqCheck : public CManipulate  
{
public:
	void SetupIcmpSeqCheck();
	void IcmpSeqCheckInit(char *data, OptTreeNode *otn, int protocol);
	int IcmpSeqCheck(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	void ParseIcmpSeq(char *data, OptTreeNode *otn);
	CIcmpSeqCheck();
	virtual ~CIcmpSeqCheck();

};

#endif // !defined(AFX_ICMPSEQCHECK_H__0FC47D91_0215_444E_8341_AEF10E081E12__INCLUDED_)
