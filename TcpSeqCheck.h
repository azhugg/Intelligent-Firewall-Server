// TcpSeqCheck.h: interface for the CTcpSeqCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPSEQCHECK_H__F8D210F2_72BE_453A_910C_586555F88C4E__INCLUDED_)
#define AFX_TCPSEQCHECK_H__F8D210F2_72BE_453A_910C_586555F88C4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_TCP_SEQ_CHECK  10

class CTcpSeqCheck : public CManipulate  
{
public:
	void SetupTcpSeqCheck();
	void TcpSeqCheckInit(char *data, OptTreeNode *otn, int protocol);
	int CheckTcpSeqEq(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	void ParseTcpSeq(char *data, OptTreeNode *otn);
	CTcpSeqCheck();
	virtual ~CTcpSeqCheck();

};

#endif // !defined(AFX_TCPSEQCHECK_H__F8D210F2_72BE_453A_910C_586555F88C4E__INCLUDED_)
