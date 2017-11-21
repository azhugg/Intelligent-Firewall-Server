// SysLog.cpp: implementation of the CSysLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "SysLog.h"
#include <stdarg.h>
#include <winbase.h>
#include "name.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

static int LogStat = 0;            
static char* LogTag = NULL;	  	   


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSysLog::CSysLog()
{
}

CSysLog::~CSysLog()
{

}

void CSysLog::SysLog(int pri, char *fmt, ...)
{
	 va_list ap;
     va_start(ap, fmt);
     vSysLog(pri, fmt, ap);
     va_end(ap);
}

void CSysLog::vSysLog(int pri, char *fmt, va_list ap)
{
	char ch, *p, *t;
	int tbuf_left, fmt_left, prlen, saved_errno;
	char tbuf[TBUF_LEN], fmt_cpy[FMT_LEN];
	p = tbuf;
	tbuf_left = TBUF_LEN;
	saved_errno = errno;
        for (t = fmt_cpy, fmt_left = FMT_LEN; (ch = *fmt); ++fmt) {
                if (ch == '%' && fmt[1] == 'm') {
                        ++fmt;
                        prlen = _snprintf(t, fmt_left, "%s",
                            strerror(saved_errno));
                        if (prlen >= fmt_left)
                                prlen = fmt_left - 1;
                        t += prlen;
                        fmt_left -= prlen;
                } else {
                        if (fmt_left > 1) {
                                *t++ = ch;
                                fmt_left--;
                        }
                }
        }
        *t = '\0';
	_vsnprintf(p, tbuf_left, fmt_cpy, ap);
	if (!opened)
		OpenLog(LogTag, LogStat, 0);
	if (!ReportEvent(hEventLog, EVENTLOG_INFORMATION_TYPE, 0, MSG, NULL, 1, 0, (const char **)&p, NULL))
         return;
}


void CSysLog::OpenLog(char *ident, int logstat, int logfac)
{
	if(ident != NULL){
		LogTag = ident;
        LogStat = logstat;
	  AddEventSource(ident);
	  hEventLog = RegisterEventSource(NULL, ident);
      if (hEventLog == NULL)
		 return;
	}
	opened = 1;
}

void CSysLog::AddEventSource(char *ident)
{
	HKEY hk; 
    DWORD dwData; 
    char szFilePath[_MAX_PATH];
	char key[_MAX_PATH];
	
    _snprintf(key, sizeof(key), "SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\%s", ident);

    if (RegCreateKey(HKEY_LOCAL_MACHINE, key, &hk)) {
		exit(-1);
	}
	GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
 
    if (RegSetValueEx(hk,             
            "EventMessageFile",       
            0,                        
            REG_EXPAND_SZ,            
            (LPBYTE) szFilePath,      
            strlen(szFilePath) + 1)) {       
	}
 
    dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | 
        EVENTLOG_INFORMATION_TYPE | EVENTLOG_AUDIT_SUCCESS | EVENTLOG_AUDIT_FAILURE; 
 
    if (RegSetValueEx(hk,      
            "TypesSupported",  
            0,                 
            REG_DWORD,         
            (LPBYTE) &dwData,  
            sizeof(DWORD))){   
		exit(-1);
	}
 
    RegCloseKey(hk); 
}

void CSysLog::CloseLog()
{
	DeregisterEventSource(hEventLog);
}
