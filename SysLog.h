// SysLog.h: interface for the CSysLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSLOG_H__1B03D362_7789_49A2_88E4_022BB73E09E7__INCLUDED_)
#define AFX_SYSLOG_H__1B03D362_7789_49A2_88E4_022BB73E09E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define TBUF_LEN        2048
#define FMT_LEN         1024


class CSysLog  
{
public:
	void vSysLog(int pri, char *fmt, va_list ap);
	void SysLog(int pri, char *fmt, ...);
	CSysLog();
	virtual ~CSysLog();
public:
	void CloseLog();
	void AddEventSource(char *ident);
	void OpenLog(char *ident, int logstat, int logfac);
	HANDLE	hEventLog;			
	int opened;                 
};

#endif // !defined(AFX_SYSLOG_H__1B03D362_7789_49A2_88E4_022BB73E09E7__INCLUDED_)
