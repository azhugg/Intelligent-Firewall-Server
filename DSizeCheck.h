// DSizeCheck.h: interface for the CDSizeCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSIZECHECK_H__FD16DACE_77FC_4ED3_B3A2_DA6364317BE2__INCLUDED_)
#define AFX_DSIZECHECK_H__FD16DACE_77FC_4ED3_B3A2_DA6364317BE2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_DSIZE_CHECK  7

#define EQ                   0
#define GT                   1
#define LT                   2

class CDSizeCheck : public CManipulate  
{
public:
	void SetupDsizeCheck();
	void DsizeCheckInit(char *data, struct _OptTreeNode *otn, int protocol);
	void ParseDsize(char *data, OptTreeNode *otn);
	int CheckDsizeLT(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	int CheckDsizeGT(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	int CheckDsizeEq(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	CDSizeCheck();
	virtual ~CDSizeCheck();

};

#endif // !defined(AFX_DSIZECHECK_H__FD16DACE_77FC_4ED3_B3A2_DA6364317BE2__INCLUDED_)
