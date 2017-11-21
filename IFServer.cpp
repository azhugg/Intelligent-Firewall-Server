// IFServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "IFServer.h"
#include "IFServerDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//////////////////////////////////////////////////////////////////////////////
//Inclusions
//////////////////////////////////////////////////////////////////////////////

#include "Preprocessor.h"
#include "Log.h"
#include "Rules.h"
#include "Message.h"
#include "Syslog.h"
#include "fcntl.h"
#include "pcap-namedb.h"
#include "Libcap.h"
#include <process.h>
#include "FileManagement.h"
#include "TrafficControl.h"
#include "ConfigA.h"
#include "ConfigB.h"
#include "ConfigC.h"
#include "ConfigD.h"
#include "ConfigE.h"

#include <signal.h>
#include <winsock.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include "decode.h"

#ifndef LOG_PID
	#define	LOG_PID		0x01	
	#define	LOG_CONS	0x02	
	#define	LOG_ODELAY	0x04	
	#define	LOG_NDELAY	0x08	
	#define	LOG_NOWAIT	0x10	
	#define	LOG_PERROR	0x20	
#endif

#ifndef LOG_AUTH
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
#endif

#ifndef	LOG_DEBUG
	#define	LOG_EMERG	0	
	#define	LOG_ALERT	1	
	#define	LOG_CRIT	2	
	#define	LOG_ERR		3	
	#define	LOG_WARNING	4	
	#define	LOG_NOTICE	5	
	#define	LOG_INFO	6	
	#define	LOG_DEBUG	7	
#endif

void ProcessPkt(char *user, struct pcap_pkthdr *pkthdr, u_char *pkt);
UINT CapThreadProc( LPVOID pParam );

int thiszone;         
char err[PCAP_ERRBUF_SIZE]; 


BEGIN_MESSAGE_MAP(CIFServerApp, CWinApp)
	//{{AFX_MSG_MAP(CIFServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIFServerApp construction

CIFServerApp::CIFServerApp()
{
	// TODO: add construction code here,
	
	optarg = NULL;
	optind = 1;       
	opterr = 1;    
	pv.restart = NULL;
	PreprocessKeywords = new PreprocessKeywordList;
	PreprocessList = (PreprocessFuncNode *) calloc(sizeof(PreprocessFuncNode), sizeof(char)); PreprocessFuncNode;
	PreprocessList = NULL;
	PluginCleanExitList = (PluginSignalFuncNode *) calloc(sizeof(PluginSignalFuncNode), sizeof(char));
	PluginCleanExitList = NULL;
	PluginRestartList = (PluginSignalFuncNode *) calloc(sizeof(PluginSignalFuncNode), sizeof(char));
	PluginRestartList = NULL;
	AlertList = (OutputFuncNode *) calloc(sizeof(OutputFuncNode), sizeof(char));
	AlertList = NULL;
	LogList = (OutputFuncNode *) calloc(sizeof(OutputFuncNode), sizeof(char));
	LogList = NULL;
	KeywordList = (KeywordXlateList*) calloc(sizeof( KeywordXlateList), sizeof(char));
	OutputKeywords = (OutputKeywordList *) calloc(sizeof(OutputKeywordList), sizeof(char));
	RuleLog = (ListHead *) calloc(sizeof(ListHead), sizeof(char));
	RuleAlert = (ListHead *) calloc(sizeof(ListHead), sizeof(char));
	RulePass = (ListHead *) calloc(sizeof(ListHead), sizeof(char));
	head_count = NULL;
	rule_count = NULL;
	opt_count = NULL;
	rtn_tmp = (RuleTreeNode *) calloc(sizeof(RuleTreeNode), sizeof(char));
	VarHead = (VarEntry *)calloc(sizeof(VarEntry),sizeof(char));
	VarHead = NULL;
	syslog_alert_data = NULL;
	TcpdumpLogInfo = NULL;
	smb_alert_data = NULL; 
	alert_fast_data = NULL;
	alert_full_data = NULL;
	data_dump_buffer = NULL;
	accesslist = (AccessList *)calloc(sizeof(AccessList),sizeof(char));
	accesslist->ICMPList = (CriticalClientList *)calloc(sizeof(CriticalClientList),sizeof(char));
	accesslist->TCPList =  (CriticalClientList *)calloc(sizeof(CriticalClientList),sizeof(char));
	accesslist->UDPList =  (CriticalClientList *)calloc(sizeof(CriticalClientList),sizeof(char));
	pv.critical_sensitivity = 5; 
	pv.mode = MD_FIREWALL;
	pv.alert_file_flag = NULL;
	pv.log_file_flag = NULL;
	pv.password = new char[30];
	pv.password = "Open";
	pv.user_name = new char[30];
	pv.main_rules_file = new char[STD_BUF];
	pv.deny_list_file = new char[STD_BUF];
	pv.rules_directory = new char[STD_BUF];
}

CIFServerApp theApp;

BOOL CIFServerApp::InitInstance()
{
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif
	m_iComboSetting = "Administrator";
	
	CIFServerDlg dlg;
	
	CConfigA dlgConA;
	CConfigB dlgConB;
	CConfigC dlgConC;
	CConfigD dlgConD;
	CConfigE dlgConE;

	dlg.AddPage(dlgConA, "Welcome"); 
	dlg.AddPage(dlgConC, "Control Panel");
	dlg.AddPage(dlgConD, "Rules and Bans");
	dlg.AddPage(dlgConB, "Configuration",&dlgConC);
	dlg.AddPage(dlgConE, "Alerts and Dumps");
	dlg.SetTitle("Intelligent Firewall Server");
	dlg.SetConstantText("Intelligent Firewall Server");
	dlg.SetValidateBeforeChangingPages(true);
	dlgConA.m_iComboSelection  = m_iComboSetting;
	AlertStatus = 0;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		m_iComboSetting = dlgConA.m_iComboSelection;
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

int CIFServerApp::StartSniff()
{
    umask(077);

	if(pv.restart)
	{
		delete(RuleAlert);
		delete(RuleLog);
		delete(RulePass);
		RuleAlert = (ListHead *) calloc(sizeof(ListHead), sizeof(char));
		RuleLog = (ListHead *) calloc(sizeof(ListHead), sizeof(char));
		RulePass = (ListHead *) calloc(sizeof(ListHead), sizeof(char));
		head_count = NULL;
		rule_count = NULL;
		opt_count = NULL;

		
	}
    pv.promisc_flag = 1;
	char *optdir;
	optdir = "e:/temp";
	CRules *objRule;
	objRule = new CRules;
	CSysLog *objLog;
	objLog = new CSysLog;
	CLog *objLogs;
	objLogs = new CLog;
	CLibCap *objLib;
	objLib = new CLibCap;
	char* te;
	te = new char[STD_BUF];
	objLib->SetAdapt(te);
	strncpy(pv.log_dir, optdir, STD_BUF-1);
	sprintf(pv.pid_path,"%s/", pv.log_dir);
	objLib->CreatePidFile();
if(!pv.restart)
{
#ifdef WIN32
	if (!InitWinsock())
		AfxMessageBox("[!] ERROR: Could not Initialize Winsock!",MB_OK,0);
#endif
    InitNetMasks();
	objLib->InitProtoNames();
}
    pv.pkt_cnt = -1;
    pv.alert_mode = ALERT_FULL;
    thiszone = gmt2local(0);
   	pv.log_flag = 1;
    pv.showarp_flag = 1;
	pv.logbin_flag = 1;
    pv.log_cmd_override = 1;
	pv.show2hdr_flag = 1;
	pv.alert_cmd_override = 1;
	pv.syslog_flag = 0;
	pv.daemon_flag =0;
	pv.quiet_flag = 0;
	if(pv.mode == MD_FIREWALL || pv.mode == MD_IDS_ONLY)
	{
			pv.data_flag = 1;
			pv.use_rules = 1;
	}
	else 
	{
			pv.data_flag = 0;
			pv.use_rules = 0;
	}
    if(!pv.log_flag)
    {
        strncpy(pv.log_dir,DEFAULT_LOG_DIR,strlen(DEFAULT_LOG_DIR)+1);  
    }

    if (pv.use_rules && pv.rules_order_flag)
    {
		AfxMessageBox("Rule application order changed to Pass->Alert->Log",MB_OK,0);
    }
    if (!pv.use_rules && !pv.verbose_flag && !pv.log_flag)
    {
		AfxMessageBox("Suggested Quit",MB_OK,0);
    }
    SetPktProcessor();
    if (pv.use_rules)
    {
		if(!pv.alert_file_flag)
		{
			CPreprocessor *objPrep;
			objPrep = new CPreprocessor;
			objPrep->InitPreprocessors();
			objPrep->InitPlugIns();
			objPrep->InitOutputPlugins();
		}
		objRule->ParseRulesFile("e:/temp/rules/rules.sample", 0);
    }
	void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
 	AlertList = NULL;	
	LogList = NULL;		
	if (pv.alert_cmd_override){
        if (pv.syslog_flag && !pv.restart){
			void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
			ptrfun = &CManipulate::SyslogAlert;
			objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_ALERT, NULL);
            objLog->OpenLog("ifs", LOG_PID|LOG_NDELAY|LOG_NOWAIT, LOG_AUTH);
        }else if (pv.smbmsg_flag){
#ifdef ENABLE_SMB_ALERTS
            AddFuncToOutputList(SmbAlert, NT_OUTPUT_ALERT, NULL);
#endif
        }
        else
        {
			CFileManagement *objFile;
			objFile = new CFileManagement;

            switch (pv.alert_mode && !pv.alert_file_flag && pv.mode != MD_SNIFFER)
            {
                case ALERT_FAST:
                    alert = objFile->OpenAlertFile();
					ptrfun = &CManipulate::FastAlert;
					objRule->AddFuncToOutputList(ptrfun , NT_OUTPUT_ALERT, NULL);
                    break;

                case ALERT_FULL:
                    alert = objFile->OpenAlertFile();
					ptrfun = &CManipulate::FullAlert;
					objRule->AddFuncToOutputList(ptrfun , NT_OUTPUT_ALERT, NULL);
                    break;

                case ALERT_NONE:
                    ptrfun = &CManipulate::NoAlert;
					objRule->SetOutputList(ptrfun, NT_OUTPUT_ALERT, NULL);
                    break;

                case ALERT_UNSOCK:
					ptrfun = &CManipulate::UnixSockAlert;
					objRule->AddFuncToOutputList(ptrfun , NT_OUTPUT_ALERT, NULL);
	                objLogs->OpenAlertSock();
                    break;
            }
        }
    }
    if (AlertList == NULL && pv.use_rules && !pv.restart){
        alert = objLogs->OpenAlertFile(NULL);
		ptrfun = &CManipulate::FullAlert;
		objRule->AddFuncToOutputList(ptrfun , NT_OUTPUT_ALERT, NULL);
    }
    AlertFunc = &CManipulate::CallAlertPlugins;
	LogFunc = &CManipulate::CallLogPlugins;
	void(CManipulate::*ptrOL)(Packet*, char*, void*);
    if (pv.log_cmd_override && !pv.restart){
        if (pv.logbin_flag){
            objLogs->InitBinLogFile();
			ptrOL = &CManipulate::LogBin;
			if(!pv.restart)
				objRule->AddFuncToOutputList(ptrOL, NT_OUTPUT_LOG, NULL);
        }else{ 
            if (!pv.nolog_flag){
				ptrOL = &CManipulate::LogPkt;
				objRule->AddFuncToOutputList(ptrOL, NT_OUTPUT_LOG, NULL);
			}else{
				ptrOL = &CManipulate::NoLog;
                objRule->SetOutputList(ptrOL, NT_OUTPUT_LOG, NULL);
			}
        }
    }

    if (LogList == NULL){
		ptrOL = &CManipulate::LogPkt;
        objRule->AddFuncToOutputList(ptrOL, NT_OUTPUT_LOG, NULL);
	}
#ifdef ENABLE_RESPONSE
    if ((nd = libnet_open_raw_sock(IPPROTO_RAW)) < 0)
    {
//        FatalError("ERROR: cannot open raw socket for libnet, exiting...\n");
    }
#endif
	InitializeCriticalSection(&Crit);
	ptrCapThread = AfxBeginThread(CapThreadProc,0);
	if(pv.mode == MD_FIREWALL){
		CTrafficControl *ptrTraffic;
		ptrTraffic = new CTrafficControl;
		ptrTraffic->ActivateBlockingProcessor();
	}
    return 0;
}

int CIFServerApp::gmt2local(time_t t)
{
    register int dt, dir;
    register struct tm *gmt, *loc;
    struct tm sgmt;
    if (t == 0)
        t = time(NULL);
    gmt = &sgmt;
    *gmt = *gmtime(&t);
    loc = localtime(&t);
    dt = (loc->tm_hour - gmt->tm_hour) * 60 * 60 + 
         (loc->tm_min - gmt->tm_min) * 60;
    dir = loc->tm_year - gmt->tm_year;
    if (dir == 0)
        dir = loc->tm_yday - gmt->tm_yday;
    dt += dir * 24 * 60 * 60;
    return(dt);
}

void CIFServerApp::PrintDeviceList(const char *device, CListBox *lbInterfaces)
{
		DWORD dwVersion;
		DWORD dwWindowsMajorVersion;
		const WCHAR* t;
		const char* t95;
		int i=0;
		int DescPos=0;
		char *Desc;
		int n=1;
		dwVersion = GetVersion();
		dwWindowsMajorVersion = (DWORD)(LOBYTE(LOWORD(dwVersion)));
		if(dwVersion >= 0x80000000 && dwWindowsMajorVersion >= 4){
			t95 = (char*)device;
			while(*(t95+DescPos)!=0 || *(t95+DescPos-1)!=0){
                        DescPos++;
			}
			Desc=(char*)t95+DescPos+1;
			while (!(t95[i]==0 && t95[i-1]==0)){
                        if (t95[i]==0){
                                putchar(' ');
                                putchar('(');
                                while(*Desc!=0){
                                        putchar(*Desc);
                                        Desc++;
                                }
                                Desc++;
                                putchar(')');
                        }
                        else putchar(t95[i]);
                        if((t95[i]==0) && (t95[i+1]!=0))
							lbInterfaces->AddString((CString)t95);
                        i++;
                }
		}
        else{
                t=(WCHAR*)device;
                while(*(t+DescPos)!=0 || *(t+DescPos-1)!=0){
                        DescPos++;
                }
                DescPos<<=1;
                Desc=(char*)t+DescPos+2;
                while (!(t[i]==0 && t[i-1]==0)){
                        if (t[i]==0){
                                putchar(' ');
                                putchar('(');
                                while(*Desc!=0){
                                        putchar(*Desc);
                                        Desc++;
                                }
                                Desc++;
                                putchar(')');
                        }
                        else putchar(t[i]);

                        if(t[i]==0 && t[i+1]!=0)
							lbInterfaces->AddString((CString)t);
                        i++;
                }
        }
}

int CIFServerApp::SetPktProcessor()
{
	pv.datalink = new char[STD_BUF];
	switch(datalink)
	{
		case DLT_EN10MB: 
			if(!pv.readmode_flag){
				pv.datalink = "Decoding Ethernet 10/100x Full Duplex";
			}
			else
				MessageBox(NULL,"Entering Readback Mode.........","IFS Information Message",MB_OK);
			grinder = &CDecode::DecodeEthPkt;
			break;
		case DLT_IEEE802:
			if(!pv.readmode_flag){
				pv.datalink = "Decoding Token Ring";
			}else
					MessageBox(NULL,"Entering Readback Mode","IFS Information Message",MB_OK);
			grinder = &CDecode::DecodeTRPkt;
			break;
		case DLT_FDDI:	
			if(!pv.readmode_flag){
				 pv.datalink = "Decoding Fiber DDI";
			}
			else
					MessageBox(NULL,"Entering Readback Mode","IFS Information Message",MB_OK);

			grinder = &CDecode::DecodeFDDIPkt;
			break;
		case DLT_SLIP:
			if (pv.show2hdr_flag == 1)
            {
                MessageBox(NULL,"Second layer header parsing for this datalink isn't implemented yet","IFS Information Message", MB_OK);
				pv.show2hdr_flag = 0;
            }
			grinder = &CDecode::DecodeSlipPkt;
			break;
		case DLT_PPP:
			if(!pv.readmode_flag){
				pv.datalink = "Decoding Point To Point Protocol";
			}else
				MessageBox(NULL,"Entering Readback Mode","IFS Information Message",MB_OK);

			if (pv.show2hdr_flag == 1){
                MessageBox(NULL,"Second layer header parsing for this datalink isn't implemented yet","IFS Information Message", MB_OK);
				pv.show2hdr_flag = 0;
            }
			grinder = &CDecode::DecodePppPkt;
			break;

		case DLT_NULL:		   
			if (pv.show2hdr_flag == 1)
            {
                MessageBox(NULL,"Second layer header parsing for this datalink isn't implemented yet","IFS Information Message", MB_OK);
				pv.show2hdr_flag = 0;
            }
			grinder = &CDecode::DecodeNullPkt;
			break;

#ifdef DLT_RAW 
		case DLT_RAW:	
			if (pv.show2hdr_flag == 1)
            {
                MessageBox(NULL,"There's no second layer header available for this datalink","IFS Information Message",MB_OK);
				pv.show2hdr_flag = 0;
            }
			grinder = &CDecode::DecodeRawPkt;
			break;
#endif	
	
			
#ifdef DLT_I4L_RAWIP	
        case DLT_I4L_RAWIP:
				grinder = &CDecode::DecodeI4LRawIPPkt;
				break;
#endif

#ifdef DLT_I4L_IP		
        case DLT_I4L_IP:
				grinder = &CDecode::DecodeEthPkt;
				break;
#endif

#ifdef DLT_I4L_CISCOHDLC	
        case DLT_I4L_CISCOHDLC:
				grinder = &CDecode::DecodeI4LCiscoIPPkt;
				break;
#endif
		default:			
			CleanExit(SIGQUIT);
	}
	return 0;
}

void CIFServerApp::Ts_Print(const timeval *tvp, char *timebuf)
{
	register int s;  
    struct timeval tv;
    struct Timezone tz;
    struct tm *lt;
    if (!tvp){
        bzero((char *)&tz,sizeof(tz));
        GetTimeOfDay(&tv,&tz);
        tvp=&tv;
    }
    lt = localtime((time_t *)&tvp->tv_sec);
    s = (tvp->tv_sec + thiszone) % 86400;
    (void)sprintf(timebuf, "%02d/%02d-%02d:%02d:%02d.%06u ", lt->tm_mon+1, 
                  lt->tm_mday, s / 3600, (s % 3600) / 60, s % 60, 
                  (u_int)tvp->tv_usec);
}

void CIFServerApp::CleanExit(int sig)
{
	struct pcap_stat ps;
    float drop;
    float recv;
    PluginSignalFuncNode *idx;
    fflush(stdout);
    if (pv.logbin_flag)
    {
        pcap_dump_close(dumpd);
    }

    unlink(pv.pid_filename);
    pv.pid_filename[0] = 0;

	fflush(alert);
    if (pv.alert_mode == ALERT_FAST || pv.alert_mode == ALERT_FULL)
    {
        if (alert != NULL)
            fclose(alert);
    }
    if(sig != SIGHUP) 
    {
        idx = PluginCleanExitList;
    } 
    else 
    {
        idx = PluginRestartList;
    }

    while(idx != NULL) 
    {
		 void (CManipulate::*ptrfun)(int,void*);
		 ptrfun = idx->func;
		 CManipulate objManip;
		 (objManip.*ptrfun)(sig, idx->arg);
         idx = idx->next;
    }

    if (pv.readmode_flag)
    {
        puts("\n\n===============================================================================");

        recv = pc.tcp+pc.udp+pc.icmp+pc.arp+pc.ipx+pc.ipv6+pc.other;
        puts("\n\n===============================================================================");

        ExitOrExec(0, sig);
		FreeAllMemoryBlocks();
        exit(0);
    }

    if (pd == NULL)
        exit(1);

    if (pcap_stats(pd, &ps))
    {
        pcap_perror(pd, "pcap_stats");
    }
    else
    {
        recv = ps.ps_recv;
        drop = ps.ps_drop;

        puts("\n\n===============================================================================");

        puts("Breakdown by protocol:");
        puts("===============================================================================");
    }
	
	HANDLE thread = ptrCapThread->m_hThread;

	if (thread != NULL){
		BOOL res = TerminateThread(thread,0);
		if(res==false){
			AfxMessageBox("Fatal Error: cannot terminate the capture thread"); 
			exit(0);
		}
	}
    pcap_close(pd);
	WaitForSingleObject(thread,INFINITE);
	FreeAllMemoryBlocks();
    ExitOrExec(0, sig);
}

int CIFServerApp::GetTimeOfDay(timeval *tv,Timezone *tz)
{
	struct _timeb tb;
	if(tv==NULL) return -1;
	_ftime(&tb);
	tv->tv_sec = tb.time;
	tv->tv_usec = ((int) tb.millitm) * 1000;
	return 0;
}

void CIFServerApp::ExitOrExec(int stat, int sig)
{
	if ( sig != SIGHUP ){
        exit(stat);
    }
}

void CIFServerApp::StripQuotes(char *data)
{
	if(index(data, '"') != NULL)
    {
        *(index(data,'"')) = ' ';
    }

    if(strrchr(data,'"') != NULL)
    {
        *(strrchr(data,'"')) = ' ';
    }
    return;
}

char* CIFServerApp::GetAdapterFromList(void *device, int index)
{
		DWORD dwVersion;
        DWORD dwWindowsMajorVersion;
        char *Adapter95;
        WCHAR *Adapter;
        int i;

        dwVersion = GetVersion();
        dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
        if (dwVersion >= 0x80000000 && dwWindowsMajorVersion >= 4)
        {
                Adapter95 =(char*) device;
                for(i = 0;i < index - 1; i++){
                        while(*Adapter95++ != 0);
                        if(*Adapter95 == 0)
							return NULL;
                }
                return  Adapter95;
        }
        else{
                Adapter=(WCHAR *)device;
                for(i = 0;i < index - 1; i++)
				{
                        while(*Adapter++ != 0);
                        if(*Adapter == 0)return NULL;
                }
                return  (char*)Adapter;
        }
}

int CIFServerApp::Strip(char *data)
{
	int size;
    char *end;
    char *idx;

    idx = data;
    end = data + strlen(data);
    size = end - idx;

    while (idx != end)
    {
        if ((*idx == '\n') ||
            (*idx == '\r'))
        {
            *idx = 0;
            size--;
        }

        if (*idx == '\t')
        {
            *idx = ' ';
        }

        idx++;
    }

    return size;
}

void CIFServerApp::GenHomenet(char *netdata)
{
	struct in_addr net;           
    char **toks;                  
    int num_toks;                 
    int nmask;                    
	CStringMan *objString;
	objString = new CStringMan;
    toks = objString->mSplit(optarg,"/",2,&num_toks,0);
    if (num_toks > 1)
    {
        nmask = atoi(toks[1]);
        if ((nmask > 0) && (nmask < 33))
        {
            pv.netmask = netmasks[nmask];
        }
        else
        {
//            FatalError("ERROR: Bad CIDR block [%s:%d], 1 to 32 please!\n",
//                       toks[1],nmask);
        }
    }
    else
    {
//        FatalError("ERROR: No netmask specified for home network!\n");
    }

    if ((net.s_addr = inet_addr(toks[0])) ==-1)
    {
//        FatalError("ERROR: Homenet (%s) didn't x-late, WTF?\n",
//                   toks[0]);
    }
    else
    {
        pv.homenet = ((u_long)net.s_addr & pv.netmask);
    }

    free(toks);
}

char* CIFServerApp::ReadInFile(char *fname)
{
    register int fd, cc;
    register char *cp;
    struct stat buf;
    fd = open(fname, O_RDONLY);
    if (fd < 0);
	     PrintFmtError("can't open %s: %s", fname, pcap_strerror(errno));
	if (fstat(fd, &buf) < 0);
	      PrintFmtError("can't stat %s: %s", fname, pcap_strerror(errno));
    cp = (char*) malloc((u_int)buf.st_size + 1);
    cc = read(fd, cp, (int)buf.st_size);
    if (cc < 0)
	      PrintFmtError("read %s: %s", fname, pcap_strerror(errno));
    if (cc != buf.st_size)
	      PrintFmtError("short read %s (%d != %d)", fname, cc, (int)buf.st_size);
    cp[(int)buf.st_size] = '\0';
    return(cp);
}

char* CIFServerApp::CopyArgv(char **argv)
{
	char **p;
    u_int len = 0;
    char *buf;
    char *src, *dst;
    void ftlerr(char *, ...);
    p = argv;
    if (*p == 0) return 0;
    while (*p)
        len += strlen(*p++) + 1;
    buf = (char *) malloc (len);
    if (buf == NULL)
    {
//        FatalError("malloc() failed: %s\n", strerror(errno));
    }
    p = argv;
    dst = buf;
    while ((src = *p++) != NULL)
    {
        while ((*dst++ = *src++) != '\0');
        dst[-1] = ' ';
    }
    dst[-1] = '\0';
    return buf;
}

void CIFServerApp::PrintFmtError(char *fmt,...)
{
	CSysLog *objSysLog;
	objSysLog = new CSysLog;
	char buf[STD_BUF+1];
    va_list ap;
    va_start(ap,fmt);
    vsprintf(buf,fmt,ap);
    if (pv.daemon_flag)
        objSysLog->SysLog(LOG_CONS|LOG_DAEMON|LOG_ERR,"%s",buf);
}

void ProcessPkt(char *user, struct pcap_pkthdr *pkthdr, u_char *pkt)
{
	Packet p;
	CLog objLog;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CDecode objDecode;
	objDecode.DecodeEthPkt(&p,pkthdr,pkt);
    if (ptrMainApp->pv.verbose_flag){
        if (p.iph != NULL){
            objLog.PrintIPPkt(stdout, p.iph->ip_proto, &p);
        }
        else if (p.ah != NULL && ptrMainApp->pv.showarp_flag){
            objLog.PrintArpHeader(stdout, &p);
            if (ptrMainApp->pv.log_flag){
                objLog.LogArpPkt(&p);
            }
        }
    }
    if (!ptrMainApp->pv.use_rules){
        if (ptrMainApp->pv.log_flag){
            if (ptrMainApp->pv.logbin_flag){
                objLog.LogBin(&p, NULL, NULL);
            }
            else{
                if (p.iph != NULL)
                    objLog.LogPkt(&p, NULL, NULL);
                if (p.ah != NULL){
                    if (!ptrMainApp->pv.nolog_flag){
                        objLog.OpenLogFile(ARP, &p);
                        objLog.PrintArpHeader(ptrMainApp->log_ptr, &p);
                        fclose(ptrMainApp->log_ptr);
                    }
                }
            }
        }
    }
    else{
		CRules objRule;
		objRule.Preprocess(&p);
    }

    objLog.ClearDumpBuf();
}

UINT CapThreadProc( LPVOID pParam )
{
	CIFServerApp* objMainApp;
	objMainApp = PTRAPP;
    if (pcap_loop(objMainApp->pd, objMainApp->pv.pkt_cnt, (pcap_handler)ProcessPkt, NULL) < 0);
	return 0;
}

void CIFServerApp::InitNetMasks()
{
			netmasks = new u_long[33];
			netmasks[0] =	0x0;
			netmasks[1] =	0x80000000;
			netmasks[2] = 	0xC0000000;
			netmasks[3] = 	0xE0000000;
			netmasks[4] = 	0xF0000000;
			netmasks[5] = 	0xF8000000;
			netmasks[6] = 	0xFC000000;
			netmasks[7] = 	0xFE000000;
			netmasks[8] =	0xFF000000;
			netmasks[9] =	0xFF800000;
			netmasks[10] =	0xFFC00000;
			netmasks[11] =	0xFFE00000;
			netmasks[12] =	0xFFF00000;
			netmasks[13] =	0xFFF80000;
			netmasks[14] =	0xFFFC0000;
			netmasks[15] =	0xFFFE0000;
			netmasks[16] =	0xFFFF0000;
			netmasks[17] =	0xFFFF8000;
			netmasks[18] =	0xFFFFC000;
			netmasks[19] =	0xFFFFE000;
			netmasks[20] =	0xFFFFF000;
			netmasks[21] =	0xFFFFF800;
			netmasks[22] =	0xFFFFFC00;
			netmasks[23] =	0xFFFFFE00;
			netmasks[24] =	0xFFFFFF00;
			netmasks[25] =	0xFFFFFF80;
			netmasks[26] =	0xFFFFFFC0;
			netmasks[27] =	0xFFFFFFE0;
			netmasks[28] =	0xFFFFFFF0;
			netmasks[29] =	0xFFFFFFF8;
			netmasks[30] =	0xFFFFFFFC;
			netmasks[31] =	0xFFFFFFFE;
			netmasks[32] =	0xFFFFFFFF;
}

int CIFServerApp::InitWinsock()
{
	WORD wVersionRequested = MAKEWORD(2,2);
	WSADATA wsaData;

	if(WSAStartup(wVersionRequested,&wsaData)){
		MessageBox(NULL,"[!] ERROR: Unable to find a usable Winsock","IFS Error Message",MB_OK);
		return 0;
	}
	
	if((LOBYTE(wsaData.wVersion) < 1) || (HIBYTE(wsaData.wVersion)<1)){
		MessageBox(NULL,"[!] ERROR: Unable to find Winsock version 1.1 or greater. You have version " ,"IFS Error Message",MB_OK);
		WSACleanup();
		return 0;
	}
	return 1;
}

void CIFServerApp::ProcessPacket(char *user, pcap_pkthdr *pkthdr, u_char *pkt)
{
	Packet p;
	CLog objLog;
	CDecode objDecode;
	objDecode.DecodeEthPkt(&p,pkthdr,pkt);
    if (pv.verbose_flag){
        if (p.iph != NULL){
            objLog.PrintIPPkt(stdout, p.iph->ip_proto, &p);
        }
        else if (p.ah != NULL && pv.showarp_flag){
            objLog.PrintArpHeader(stdout, &p);
            if (pv.log_flag){
                objLog.LogArpPkt(&p);
            }
        }
    }
    if (!pv.use_rules){
        if (pv.log_flag){
            if (pv.logbin_flag){
                objLog.LogBin(&p, NULL, NULL);
            }else{
                if (p.iph != NULL)
                    objLog.LogPkt(&p, NULL, NULL);

                if (p.ah != NULL){
                    if (!pv.nolog_flag){
                        objLog.OpenLogFile(ARP, &p);

                        objLog.PrintArpHeader(log_ptr, &p);

                        fclose(log_ptr);
                    }
                }
            }
        }
    }else{
		CRules objRule;
		objRule.Preprocess(&p);
    }
    objLog.ClearDumpBuf();
}

void CIFServerApp::FreeAllMemoryBlocks()
{
	delete(progname);        
	delete(progargs);
	delete(PreprocessKeywords);
	delete(PreprocessList);
	delete(PluginCleanExitList);
	delete(PluginRestartList);
	delete(netmasks);
	delete(file_name);
	delete(log_ptr);         
	delete(binlog_ptr);      
	delete(AlertList);
	delete(LogList);
	delete(KeywordList);
	delete(OutputKeywords);
	delete(otn_tmp); 
	delete(optarg);    
	delete(RuleAlert);
	delete(RulePass);
	delete(RuleLog);
	delete(rtn_tmp);
	delete(VarHead);
}

int CIFServerApp::ExitInstance() 
{
	// TODO: Add your specialized code here and/or call the base class
	FreeAllMemoryBlocks();
	return CWinApp::ExitInstance();
}

void CIFServerApp::StopEngine()
{
	HANDLE thread = ptrCapThread->m_hThread;
	if (thread != NULL){
		BOOL res = TerminateThread(thread,0);
		if(res==false){
			AfxMessageBox("Fatal Error: cannot terminate the capture thread"); 
			exit(0);
		}
	}
	pcap_close(pd);
	pv.restart = 1;
	WaitForSingleObject(thread,INFINITE);
}

CString CIFServerApp::BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags)
{
	CString strResult = "";
	LPMALLOC lpMalloc;  
	LPITEMIDLIST lpItemIDList;
	char szDisplayName[_MAX_PATH];
	if (::SHGetMalloc(&lpMalloc) != NOERROR)
		 return strResult; 
	
	char szBuffer[_MAX_PATH];
	CoInitialize(NULL);
	BROWSEINFO browseInfo;
	  browseInfo.hwndOwner = hWnd;
	  browseInfo.pidlRoot = NULL; 
	  browseInfo.pszDisplayName = szDisplayName;
	  browseInfo.lpszTitle = lpszTitle;   
	  browseInfo.ulFlags = nFlags;   
	  browseInfo.lpfn = NULL;      
	  browseInfo.lParam = 0;      
	  if ((lpItemIDList = ::SHBrowseForFolder(&browseInfo))!= NULL){
		 if (::SHGetPathFromIDList(lpItemIDList, szBuffer)){
			if (szBuffer[0] == '\0'){
			   AfxMessageBox(IDP_FAILED_GET_DIRECTORY,MB_ICONSTOP|MB_OK);
			   return strResult;
			}
			strResult = szBuffer;
			return strResult;
		 }else {
			AfxMessageBox(IDP_FAILED_GET_DIRECTORY,MB_ICONSTOP|MB_OK);
			return strResult; 
		 }
			 lpMalloc->Free(lpItemIDList);
			 lpMalloc->Release();      
	}
	return strResult;
}

