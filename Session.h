// Session.h: interface for the CSession class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SESSION_H__471AD410_CCE6_4D39_B865_F259059C49E4__INCLUDED_)
#define AFX_SESSION_H__471AD410_CCE6_4D39_B865_F259059C49E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_SESSION     15
#define SESSION_PRINTABLE  1
#define SESSION_ALL        2

class CSession : public CManipulate  
{
public:
	void OpenSessionFile(Packet *p);
	int LogSessionData(Packet *p, struct _OptTreeNode *otn, OptFpList *fp_list);
	void ParseSession(char *data, OptTreeNode *otn);
	CSession();
	virtual ~CSession();
public:
	void SetupSession();
	void SessionInit(char *data, OptTreeNode *otn, int protocol);
	void DumpSessionData(FILE *fp, Packet *p, struct _OptTreeNode *otn);
	FILE *session;
};

#endif // !defined(AFX_SESSION_H__471AD410_CCE6_4D39_B865_F259059C49E4__INCLUDED_)
