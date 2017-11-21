// TcpDumpLog.h: interface for the CTcpDumpLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TCPDUMPLOG_H__C398C965_8D3B_4D98_9719_4DE46A2FFA68__INCLUDED_)
#define AFX_TCPDUMPLOG_H__C398C965_8D3B_4D98_9719_4DE46A2FFA68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CTcpDumpLog : public CManipulate  
{
public:
	void TcpdumpInitLogFile(LogTcpdumpData *data);
	CTcpDumpLog();
	virtual ~CTcpDumpLog();

public:
	void SetupTcpDumpLog();
	void LogTcpdumpInit(u_char *args);
	void SpoLogTcpdumpRestartFunc(int signal, void *arg);
	void SpoLogTcpdumpCleanExitFunc(int signal, void *arg);
	LogTcpdumpData* ParseTcpdumpArgs(char *args);
	void LogTcpdump(Packet *p, char *msg, void *arg);
};

#endif // !defined(AFX_TCPDUMPLOG_H__C398C965_8D3B_4D98_9719_4DE46A2FFA68__INCLUDED_)
