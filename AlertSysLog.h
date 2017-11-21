// AlertSysLog.h: interface for the CAlertSysLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ALERTSYSLOG_H__B2CCD3FE_CEBF_4F0C_A7AC_3C297CFF6725__INCLUDED_)
#define AFX_ALERTSYSLOG_H__B2CCD3FE_CEBF_4F0C_A7AC_3C297CFF6725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef LOG_PID
#define	LOG_PID		0x01	
#define	LOG_CONS	0x02	
#define	LOG_ODELAY	0x04	
#define	LOG_NDELAY	0x08	
#define	LOG_NOWAIT	0x10	
#define	LOG_PERROR	0x20	


#define	LOG_KERN	(0<<3)	
#define	LOG_USER	(1<<3)	
#define	LOG_MAIL	(2<<3)	
#define	LOG_DAEMON	(3<<3)	
#define	LOG_AUTH	(4<<3)	
#define	LOG_SYSLOG	(5<<3)	
#define	LOG_LPR		(6<<3)	
#define	LOG_NEWS	(7<<3)	
#define	LOG_UUCP	(8<<3)	
#define	LOG_CRON	(9<<3)	
#define	LOG_AUTHPRIV	(10<<3)
#define	LOG_FTP		(11<<3)	

#define	LOG_EMERG	0	
#define	LOG_ALERT	1	
#define	LOG_CRIT	2	
#define	LOG_ERR		3	
#define	LOG_WARNING	4	
#define	LOG_NOTICE	5	
#define	LOG_INFO	6	
#define	LOG_DEBUG	7	
#endif

#ifndef NT_OUTPUT_ALERT
	#define NT_OUTPUT_ALERT  0   
	#define NT_OUTPUT_LOG    1   
#endif

class CAlertSysLog : public CManipulate  
{
public:
	SyslogData* ParseSyslogArgs(char *args);
	CAlertSysLog();
	virtual ~CAlertSysLog();
public:
	void SetupAlertSyslog();
	void AlertSyslogRestartFunc(int signal, void *arg);
	void AlertSyslogCleanExitFunc(int signal, void *arg);
	void AlertSyslogInit(u_char *args);
	void AlertSyslog(Packet *p, char *msg, void *arg);
};

#endif // !defined(AFX_ALERTSYSLOG_H__B2CCD3FE_CEBF_4F0C_A7AC_3C297CFF6725__INCLUDED_)
