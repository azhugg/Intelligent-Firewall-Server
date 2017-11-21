// Respond.h: interface for the CRespond class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RESPOND_H__9B9A66F8_521A_404D_A23D_58494FF0F901__INCLUDED_)
#define AFX_RESPOND_H__9B9A66F8_521A_404D_A23D_58494FF0F901__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CRespond  
{
public:
	int SendICMP_UNREACH(int type, u_long saddr, u_long daddr, Packet *p);
	int SendTCPRST(u_long saddr, u_long daddr, u_short sport, u_short dport, int seq, int ack);
	void Respond(Packet *p);
	CRespond();
	virtual ~CRespond();

};

#endif // !defined(AFX_RESPOND_H__9B9A66F8_521A_404D_A23D_58494FF0F901__INCLUDED_)
