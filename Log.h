// Log.h: interface for the CLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LOG_H__BAD49988_3CD6_44C9_983D_621F2252B2D6__INCLUDED_)
#define AFX_LOG_H__BAD49988_3CD6_44C9_983D_621F2252B2D6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#ifndef LOG_AUTH
#define	LOG_AUTH	(4<<3)	
#define	LOG_KERN	(0<<3)	
#define	LOG_USER	(1<<3)	
#define	LOG_MAIL	(2<<3)	
#define	LOG_DAEMON	(3<<3)	
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
#define FRAME_SIZE        66
#define C_OFFSET          49
#define DUMP              1
#define BOGUS             2
#define NON_IP            3
#define ARP               4

class CLog  
{
public:
	void ClearDumpBuf();
	CLog();
	virtual ~CLog();

public:
	void AddToCriticalClients(Packet *p, char *msg, char *TStamp, int protocol);
	void SmbAlert(Packet *p, char *msg, void *args);
	void AlertFast(Packet *p, char *msg, FILE *file);
	void FastAlert(Packet *p, char *msg, void *arg);
	void FullAlert(Packet *p, char *msg, void *arg);
	void SyslogAlert(Packet *p, char *msg, void *arg);
	void UnixSockAlert(Packet *p, char *msg, void *arg);
	void OpenAlertSock();
	void AlertFull(Packet *p, char *msg, FILE *file);
	FILE* OpenAlertFile(char *filearg);
	void PrintArpHeader(FILE *fp, Packet *p);
	int OpenLogFile(int mode, Packet *p);
	void PrintCharData(FILE *fp, u_char *data, int data_len);
	void PrintNetData(FILE *fp, u_char *start, const u_int len);
	void NoAlert(Packet *p, char *msg, void *arg);
	void LogPkt(Packet *p, char *msg, void *arg);
	void NoLog(Packet *p, char *msg, void *arg);
	void LogArpPkt(Packet *p);
	void InitBinLogFile();
	char* IcmpFileName(Packet *p);
	void LogBin(Packet *p, char *msg, void *arg);
	void PrintTcpOptions(FILE *fp, Packet *p);
	void PrintIpOptions(FILE *fp, Packet *p);
	void PrintICMPHeader(FILE *fp, Packet *p);
	void PrintUDPHeader(FILE *fp, Packet *p);
	void PrintTCPHeader(FILE *fp, Packet *p);
	void PrintIPHeader(FILE *fp, Packet *p);
	void PrintTrHeader(FILE *fp, Packet *p);
	void PrintEthHeader(FILE *fp, Packet *p);
	void Print2ndHeader(FILE *fp, Packet *p);
	void PrintIPPkt(FILE *fp, int type, Packet *p);
	void CreateTCPFlagString(Packet *p, char *flagBuffer);
	unsigned char ezero[6]; 
	int dump_ready;          
	int dump_size;
public:
	void CreatePidFile(char* intf);
	CIFServerApp* ptrMainApp;
};

#endif // !defined(AFX_LOG_H__BAD49988_3CD6_44C9_983D_621F2252B2D6__INCLUDED_)
