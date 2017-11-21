// TcpAckCheck.h: interface for the CTcpAckCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPACKCHECK_H__BB29B42F_F6DD_4072_8313_A102D80EFD4B__INCLUDED_)
#define AFX_TCPACKCHECK_H__BB29B42F_F6DD_4072_8313_A102D80EFD4B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_TCP_ACK_CHECK  9

class CTcpAckCheck : public CManipulate  
{
public:
	void SetupTcpAckCheck();
	void TcpAckCheckInit(char *data, OptTreeNode *otn, int protocol);
	int CheckTcpAckEq(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	void ParseTcpAck(char *data, OptTreeNode *otn);
	CTcpAckCheck();
	virtual ~CTcpAckCheck();

};

#endif // !defined(AFX_TCPACKCHECK_H__BB29B42F_F6DD_4072_8313_A102D80EFD4B__INCLUDED_)
