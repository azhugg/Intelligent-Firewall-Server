// TcpFlagCheck.h: interface for the CTcpFlagCheck class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPFLAGCHECK_H__9D6CD1F2_90A4_4603_B980_A4B3A341ED73__INCLUDED_)
#define AFX_TCPFLAGCHECK_H__9D6CD1F2_90A4_4603_B980_A4B3A341ED73__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_TCP_FLAG_CHECK   2

#define R_FIN          0x01
#define R_SYN          0x02
#define R_RST          0x04
#define R_PSH          0x08
#define R_ACK          0x10
#define R_URG          0x20
#define R_RES2         0x40
#define R_RES1         0x80


class CTcpFlagCheck : public CManipulate  
{
public:
	void SetupTCPFlagCheck();
	void TCPFlagCheckInit(char *data, OptTreeNode *otn, int protocol);
	int CheckTcpFlagsEq(Packet *p, struct _OptTreeNode *otn_idx, OptFpList *fp_list);
	void ParseTCPFlags(char *rule, OptTreeNode *otn);
	CTcpFlagCheck();
	virtual ~CTcpFlagCheck();
};

#endif // !defined(AFX_TCPFLAGCHECK_H__9D6CD1F2_90A4_4603_B980_A4B3A341ED73__INCLUDED_)
