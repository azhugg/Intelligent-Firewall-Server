// HttpDecode.h: interface for the CHttpDecode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_HTTPDECODE_H__8C3994E8_1EA6_4A68_A440_C71B5983481F__INCLUDED_)
#define AFX_HTTPDECODE_H__8C3994E8_1EA6_4A68_A440_C71B5983481F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CHttpDecode:public CManipulate
{
public:
	void SetupHttpDecode();
	int Nibble(char what);
	void PreprocUrlDecode(Packet *p);
	void HttpDecodeInit(u_char *args);
	void SetPorts(char *portlist);
	CHttpDecode();
	virtual ~CHttpDecode();

};

#endif // !defined(AFX_HTTPDECODE_H__8C3994E8_1EA6_4A68_A440_C71B5983481F__INCLUDED_)
