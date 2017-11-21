// IFServer.h : main header file for the IFSERVER application
//

#if !defined(AFX_IFSERVER_H__F40031B9_7C3B_4D5C_A63A_F9E12C46CB4A__INCLUDED_)
#define AFX_IFSERVER_H__F40031B9_7C3B_4D5C_A63A_F9E12C46CB4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include <io.h>
#include "pcap.h"

#define STD_BUF  256

#ifdef WIN32
#define DEFAULT_LOG_DIR   "log"
#define DEFAULT_DAEMON_ALERT_FILE  "log/ifs.ids"
#define UNSOCK_FILE       "ifs_alert"
#else
#define DEFAULT_LOG_DIR   "/var/log/ifs"
#define DEFAULT_DAEMON_ALERT_FILE  "ifs.alert"
#define UNSOCK_FILE       "/dev/ifs_alert"
#endif
#define W_OK            0x02    
#define S_ISDIR(x)      (((x) & 0170000) == 0040000)    
#define S_IRWXU         0000700                 
#define S_IRWXG         0000070                 
#define S_IROTH         0000004                 
#define S_IXOTH         0000001                 
#define _PATH_VARRUN "./"
#define ALERT_FULL     0x01
#define ALERT_FAST     0x02
#define ALERT_NONE     0x03
#define ALERT_UNSOCK   0x04
#define getpid _getpid
#define strncasecmp strnicmp
#define strcasecmp stricmp
#define bcopy(x, y, z) memcpy((void *)x, (const void *)y, (size_t) z)
#define bzero(x, y) memset((x), 0, (y))
#define mkdir(x, y) _mkdir(x)
#define NextChar(string)  (char)(*(string+1))
#define index strchr
#define IXDR_GET_LONG(buf) ((long)ntohl((u_long)*(buf)++))
#define IXDR_GET_ENUM(buf, t) ((t)IXDR_GET_LONG(buf))
#define RPC_MSG_VERSION ((u_long) 2)
#define IDP_FAILED_GET_DIRECTORY "Failed To Get Directory"
#define FILE_OPEN		1;
#define FILE_NULL		0;
#define VERSION "IFS - 1.0.0"
#define SIGKILL  9       
#define SIGQUIT  3       
#define SIGHUP   1       
typedef unsigned char           u_int8_t;
typedef unsigned short          u_int16_t;
typedef unsigned int            u_int32_t;
typedef int						pid_t;
typedef unsigned long u_long;
#ifdef WIN32
#ifndef SNAPLEN
#ifndef IRIX
#define SNAPLEN      1514
#else
#define SNAPLEN      1500
#endif
#endif
#define TR_ALEN      6    
#define IPARP_SAP 0xaa
#define AC           0x10
#define LLC_FRAME    0x40
#define TRMTU                      2000   
#define TR_RII                     0x80
#define TR_RCF_DIR_BIT             0x80
#define TR_RCF_LEN_MASK            0x1f00
#define TR_RCF_BROADCAST           0x8000 
#define TR_RCF_LIMITED_BROADCAST   0xC000 
#define TR_RCF_FRAME2K             0x20
#define TR_RCF_BROADCAST_MASK      0xC000
#endif 
#define FDDI_ALLC_LEN                   13
#define FDDI_ALEN                       6
#define FDDI_MIN_HLEN                   FDDI_ALLC_LEN + 3
#define FDDI_DSAP_SNA                   0x08  
#define FDDI_SSAP_SNA                   0x00  
#define FDDI_DSAP_STP                   0x42  
#define FDDI_SSAP_STP                   0x42  
#define FDDI_DSAP_IP                    0xaa  
#define FDDI_SSAP_IP                    0xaa  
#define FDDI_ORG_CODE_ETHR              0x000000 
#define FDDI_ORG_CODE_CDP               0x00000c 
#define ETHERNET_TYPE_CDP               0x2000   
#define ARPOP_REQUEST   1               
#define ARPOP_REPLY     2               
#define ARPOP_RREQUEST  3               
#define ARPOP_RREPLY    4               
#define ICMP_ECHOREPLY          0       
#define ICMP_DEST_UNREACH       3       
#define ICMP_SOURCE_QUENCH      4       
#define ICMP_REDIRECT           5       
#define ICMP_ECHO               8       
#define ICMP_TIME_EXCEEDED      11      
#define ICMP_PARAMETERPROB      12      
#define ICMP_TIMESTAMP          13      
#define ICMP_TIMESTAMPREPLY     14      
#define ICMP_INFO_REQUEST       15      
#define ICMP_INFO_REPLY         16      
#define ICMP_ADDRESS            17      
#define ICMP_ADDRESSREPLY       18      
#define NR_ICMP_TYPES           18
#define ICMP_NET_UNREACH        0       
#define ICMP_HOST_UNREACH       1       
#define ICMP_PROT_UNREACH       2       
#define ICMP_PORT_UNREACH       3       
#define ICMP_FRAG_NEEDED        4       
#define ICMP_SR_FAILED          5       
#define ICMP_NET_UNKNOWN        6
#define ICMP_HOST_UNKNOWN       7
#define ICMP_HOST_ISOLATED      8
#define ICMP_NET_ANO            9
#define ICMP_HOST_ANO           10
#define ICMP_NET_UNR_TOS        11
#define ICMP_HOST_UNR_TOS       12
#define ICMP_PKT_FILTERED       13      
#define ICMP_PREC_VIOLATION     14      
#define ICMP_PREC_CUTOFF        15      
#define NR_ICMP_UNREACH         15      

#ifndef IPOPT_EOL
#define IPOPT_EOL       0x00
#endif
#ifndef IPOPT_NOP
#define IPOPT_NOP       0x01
#endif
#ifndef IPOPT_RR
#define IPOPT_RR        0x07
#endif
#ifndef IPOPT_TS
#define IPOPT_TS        0x44
#endif
#ifndef IPOPT_SECURITY
#define IPOPT_SECURITY  0x82
#endif
#ifndef IPOPT_LSRR
#define IPOPT_LSRR      0x83
#endif
#ifndef IPOPT_LSRR_E
#define IPOPT_LSRR_E    0x84
#endif
#ifndef IPOPT_SATID
#define IPOPT_SATID     0x88
#endif
#ifndef IPOPT_SSRR
#define IPOPT_SSRR      0x89
#endif
#define TOPT_EOL        0x00
#define TOPT_NOP        0x01
#define TOPT_MSS        0x02
#define TOPT_WS         0x03
#define TOPT_TS         0x08
#ifndef TCPOPT_WSCALE
#define TCPOPT_WSCALE   3   
#endif
#ifndef TCPOPT_SACKOK
#define	TCPOPT_SACKOK	4	
#endif
#ifndef TCPOPT_SACK
#define	TCPOPT_SACK		5	
#endif
#ifndef TCPOPT_ECHO
#define TCPOPT_ECHO     6   
#endif
#ifndef TCPOPT_ECHOREPLY
#define TCPOPT_ECHOREPLY  7 
#endif
#ifndef TCPOPT_TIMESTAMP
#define TCPOPT_TIMESTAMP  8 
#endif
#ifndef TCPOPT_CC
#define TCPOPT_CC		  11
#endif
#ifndef TCPOPT_CCNEW
#define TCPOPT_CCNEW	  12
#endif
#ifndef TCPOPT_CCECHO
#define TCPOPT_CCECHO	  13	
#endif
#define EXTRACT_16BITS(p) ((u_short) ntohs (*(u_short *)(p)))
#ifdef WORDS_MUSTALIGN
#define EXTRACT_32BITS(p)  ({ u_long __tmp; memmove(&__tmp, (p), sizeof(u_long)); (u_long) ntohl(__tmp);})
#else
#define EXTRACT_32BITS(p) ((u_long) ntohl (*(u_long *)(p)))
#endif 
#define MD_FIREWALL				2
#define MD_IDS_ONLY				1
#define MD_SNIFFER				0					
#define ETHERNET_MTU            1500
#define ETHERNET_TYPE_IP        0x0800
#define ETHERNET_TYPE_ARP       0x0806
#define ETHERNET_TYPE_REVARP    0x8035
#define ETHERNET_TYPE_IPV6      0x86dd
#define ETHERNET_TYPE_IPX       0x8137

#define ETHERNET_HEADER_LEN             14
#define MINIMAL_TOKENRING_HEADER_LEN    22
#define TR_HLEN                         MINIMAL_TOKENRING_HEADER_LEN
#define TOKENRING_LLC_LEN                8 
#define SLIP_HEADER_LEN                 16
#ifndef PPP_HDRLEN
#define PPP_HDRLEN                       4
#endif
#ifndef PPP_MTU
#define PPP_MTU                 1500
#endif
#define NULL_HDRLEN             4
#define IP_HEADER_LEN           20
#define TCP_HEADER_LEN          20
#define UDP_HEADER_LEN          8
#define ICMP_HEADER_LEN         4
#define TH_FIN  0x01
#define TH_SYN  0x02
#define TH_RST  0x04
#define TH_PUSH 0x08
#define TH_ACK  0x10
#define TH_URG  0x20
#define TH_RES2 0x40
#define TH_RES1 0x80
#define	TCPOPT_EOL		0
#define	TCPOPT_NOP		1
#define	TCPOPT_MAXSEG		2
#define TCPOLEN_MAXSEG		4
#define TCPOPT_WINDOW		3
#define TCPOLEN_WINDOW		3
#define TCPOPT_SACK_PERMITTED	4		
#define TCPOLEN_SACK_PERMITTED	2
#define TCPOPT_SACK		5		
#define TCPOPT_TIMESTAMP	8
#define TCPOLEN_TIMESTAMP	10
#define TCPOLEN_TSTAMP_APPA	(TCPOLEN_TIMESTAMP+2) 
#define TCPOPT_TSTAMP_HDR	\
    (TCPOPT_NOP<<24|TCPOPT_NOP<<16|TCPOPT_TIMESTAMP<<8|TCPOLEN_TIMESTAMP)
#define	TCP_MSS	512
#define	TCP_MAXWIN	65535	
#define TCP_MAX_WINSHIFT	14	
#ifndef TCP_NODELAY
	#define	TCP_NODELAY	0x01	
#endif
#define	TCP_MAXSEG	0x02	
#define SOL_TCP		6	
#define L2TP_PORT 1701
#define DHCP_CLIENT_PORT 68
#define DHCP_SERVER_PORT 67
typedef struct _Trh_llc{
	 u_int8_t	dsap;
	 u_int8_t	ssap;
	 u_int8_t	protid[3];
	 u_int16_t	ethertype;
} Trh_llc;
typedef struct _Trh_mr{
#if defined WORD(WORDS_BIGENDIAN)
	u_int16_t bcast:3; 
	u_int16_t len:5;
	u_int16_t dir:1;
	u_int16_t lf:3;
	u_int16_t res:4;
#else
	u_int16_t len:5;
	u_int16_t bcast:3;  
	u_int16_t res:3;	
	u_int16_t lf:3;		
	u_int16_t dir:1;	
#endif
	u_int16_t rseg[8];	
} Trh_mr;

typedef struct _Trh_hdr{
	u_int8_t ac;		
	u_int8_t fc;		
	u_int8_t daddr[TR_ALEN];
	u_int8_t saddr[TR_ALEN];
} Trh_hdr;
typedef struct _Fddi_hdr{
	u_int8_t fc;
	u_int8_t daddr[FDDI_ALEN];	
	u_int8_t saddr[FDDI_ALEN];	
}Fddi_hdr;
typedef struct _Fddi_llc_saps{
	u_int8_t dsap;	
	u_int8_t ssap;	
}Fddi_llc_saps;
typedef struct _Fddi_llc_sna{
	u_int8_t ctrl_fld[2];
}Fddi_llc_sna;
typedef struct _Fddi_llc_other{
	u_int8_t ctrl_fld[1];
}Fddi_llc_other;
typedef struct _Fddi_llc_iparp{
	u_int8_t	ctrl_fld;
	u_int8_t	protid[3];
	u_int16_t	ethertype;
}Fddi_llc_iparp;

typedef struct _EtherHdr{
	u_int8_t	ether_dst[6];
	u_int8_t	ether_src[6];
	u_int16_t	ether_type;
}EtherHdr;
typedef struct _IPHdr{
#if defined(WORDS_BIGENDIAN)
	u_int8_t	ip_ver:4;	
	u_int8_t	ip_hlen:4;	
#else
	u_int8_t	ip_hlen:4;	
	u_int8_t	ip_ver:4;	
#endif
	u_int8_t	ip_tos;		
	u_int16_t	ip_len;		
	u_int16_t	ip_id;		
	u_int16_t	ip_off;		
	u_int8_t	ip_ttl;		
	u_int8_t	ip_proto;	
	u_int16_t	ip_csum;	
	struct in_addr ip_src;	
	struct in_addr ip_dst;	
}IPHdr;

typedef struct _TCPHdr{
	u_int16_t	th_sport;
	u_int16_t	th_dport;
	u_int32_t	th_seq;	
	u_int32_t	th_ack;	
#ifdef WORDS_BIGENDIAN
	u_int8_t	th_off:4;	
	u_int8_t	th_x2:4;	
#else
	u_int8_t	th_x2:4;		
	u_int8_t	th_off:4;
#endif
	u_int8_t	th_flags;	
	u_int16_t	th_win;		
	u_int16_t	th_sum;		
	u_int16_t	th_urp;		
} TCPHdr;

typedef struct _UDPHdr{
	u_int16_t	uh_sport;	
	u_int16_t	uh_dport;	
	u_int16_t	uh_len;		
	u_int16_t	uh_chk;		
}UDPHdr;

typedef struct _ICMPHdr{
	u_int8_t	type;
	u_int8_t	code;
	u_int16_t	csum;
	union{	
		u_char s_ih_pptr;	
		struct in_addr	ih_gwaddr;
		struct s_ih_idseq{
			u_int16_t icd_id;	
			u_int16_t icd_seq;		
		}s_ih_idseq;
		int s_ih_void;
		struct s_ih_pmtu{
			u_int16_t ipm_void;
			u_int16_t ipm_nextmtu;
		}s_ih_pmtu;
		struct s_ih_rtradv{
			u_char		irt_num_addrs;
			u_char		irt_wpa;
			u_int16_t	irt_lifetime;
		}s_ih_rtradv;
	}s_icmp_hun;
#define ih_pptr       s_icmp_hun.s_ih_pptr
#define ih_gwaddr     s_icmp_hun.s_ih_gwaddr
#define ih_id         s_icmp_hun.s_ih_idseq.icd_id
#define ih_seq        s_icmp_hun.s_ih_idseq.icd_seq
#define ih_void       s_icmp_hun.s_ih_void
#define ih_pmvoid     s_icmp_hun.s_ih_pmtu.ipm_void
#define ih_nextmtu    s_icmp_hun.s_ih_pmtu.ipm_nextmtu
#define ih_num_addrs  s_icmp_hun.s_ih_rtradv.irt_num_addrs
#define ih_wpa        s_icmp_hun.s_ih_rtradv.irt_wpa
#define ih_lifetime   s_icmp_hun.s_ih_rtradv.irt_lifetime
        union {
                struct s_id_ts {
                        u_int32_t its_otime;
                        u_int32_t its_rtime;
                        u_int32_t its_ttime;
                } s_id_ts;
                struct s_id_ip  {
                        struct _IPHdr idi_ip;
                } s_id_ip;
                u_int32_t s_id_mask;
                char    s_id_data[1];
        } s_icmp_dun;
#define ih_otime      s_icmp_dun.s_id_ts.its_otime
#define ih_rtime      s_icmp_dun.s_id_ts.its_rtime
#define ih_ttime      s_icmp_dun.s_id_ts.its_ttime
#define ih_ip         s_icmp_dun.s_id_ip.idi_ip
#define ih_mask       s_icmp_dun.s_id_mask
#define ih_data       s_icmp_dun.s_id_data
}ICMPHdr;
typedef struct _ARPHdr{
	u_int16_t ar_hrd;
	u_int16_t ar_pro;
	u_int8_t  ar_hln;
	u_int8_t  ar_pln;
	u_int16_t ar_op;
}ARPHdr;
typedef struct _EtherARP{
	ARPHdr ea_hdr;		
	u_int8_t arp_sha[6];
	u_int8_t arp_spa[4];
	u_int8_t arp_tha[6];
	u_int8_t arp_tpa[4];
}EtherARP;
typedef struct _Options{
	u_int8_t code;
	int len;
	u_char* data;
}Options;

typedef struct _Packet{
	struct pcap_pkthdr* pkth;	
	u_char* pkt;				
	Fddi_hdr* fddihdr;			
    Fddi_llc_saps* fddisaps;
    Fddi_llc_sna*	fddisna;
    Fddi_llc_iparp*	fddiiparp;
    Fddi_llc_other*	fddiother;
	Trh_hdr* trh;				
	Trh_llc* trhllc;
	Trh_mr* trhmr;
	EtherHdr* eh;				
	EtherARP* ah;
	IPHdr*	iph;
	TCPHdr* tcph;
	UDPHdr* udph;
	ICMPHdr* icmph;
	u_char*    data;			
	u_int16_t  dsize;			
	u_int8_t  frag_flag;		
	u_int16_t frag_offset;		
	u_char mf;					
	u_char df;					
	u_int16_t sp;				
	u_int16_t dp;				
	u_int32_t sip;				
	u_int32_t dip;				
	int caplen;
	Options ip_options[40];		
	int ip_option_count;		
	Options tcp_options[40];	
	int tcp_option_count;		
}Packet;
typedef struct _Alertpkt{
    u_char alertmsg[256];			 
    struct pcap_pkthdr pkth;
    long dlthdr;					 
    long nethdr;					 
    long transhdr;					 
    long data;
    unsigned long val;				 
#define	NOPACKET_STRUCT 0x1		     
#define NO_TRANSHDR 0x2
	u_char pkt[SNAPLEN];
} Alertpkt;


#ifndef _PreprocessKeywordNode
#include "structure.h"
#endif
typedef struct _progvars{
    int obfuscation_flag;
    int log_cmd_override;
    int alert_cmd_override;
    int char_data_flag;
    int data_flag;
    int verbose_flag;
    int showarp_flag;
    int showipv6_flag;
    int showipx_flag;
    int readmode_flag;
    int logbin_flag;
    int log_flag;
    int nolog_flag;
    int show2hdr_flag;
    int syslog_flag;
    int promisc_flag;
    int rules_order_flag;
    int smbmsg_flag;
    int track_flag;
    int daemon_flag;
    int quiet_flag;
    int pkt_cnt;
    int pkt_snaplen;
    u_long homenet;
    u_long netmask;
    int use_rules;
    int alert_mode;
	int critical_sensitivity;
    int log_plugin_active;
    int alert_plugin_active;
    char pid_filename[STD_BUF];
    char config_file[STD_BUF];
    char log_dir[STD_BUF];
    char readfile[STD_BUF];
    char smbmsg_dir[STD_BUF];
    char pid_path[STD_BUF];
    char *chroot_dir;
    char *user_name;
    char *group_name;
    u_long userid;
    u_long groupid;
    struct passwd *pw;
    struct group *gr;
    char *interfaces;
    char *pcap_cmd;
	int restart;
	int mode;
	int alert_file_flag;
	int log_file_flag;
	char *main_rules_file;
	char *password;
	char *checkpass;
	char *datalink;
	char *deny_list_file;
	int flush_failure;
	char *rules_directory;
}PV;
typedef struct _PacketCount
{
    u_long other;
    u_long tcp;
    u_long udp;
    u_long icmp;
    u_long arp;
    u_long ipv6;
    u_long ipx;
    u_long frags;
} PacketCount;


typedef struct _PortList
{
	int ports[32];
	int num_entries;
}PortList;

typedef struct _MinFragData
{
	int minfrag;
	int home_net_only;
}MinFragData;

struct Timezone {
        int     tz_minuteswest; 
        int     tz_dsttime;     
};

struct  sockaddr_un {
        u_char  sun_len;        
        u_char  sun_family;     
        char    sun_path[104];  
};

enum msg_type {
        CALL=0,
        REPLY=1
};

class CDecode;

class CIFServerApp : public CWinApp
{
public:
	void CreatePidFile(char* intf);
	int OpenInterface(char* intf);
	void GoDaemon(void);
	BOOL LoadUserConfiguration();
	void SanityChecks();
	void PrintDeviceList(const char* device, CListBox *lbInterfaces);
	void ShowInterfaces();
	BOOL LoadDefaultConfig();
	int gmt2local(time_t t);
	void InitNetMasks();
	int InitWinsock(void);
	int StartSniff(void);

	CIFServerApp();

public:
	int AlertStatus;
	CString BrowseForFolder(HWND hWnd, LPCSTR lpszTitle, UINT nFlags);
	void StopEngine();
	char *rules_file;
	CString m_iComboSetting;
	UINT nResult;
	CWinThread *ptrBlockingProcessor;
	void FreeAllMemoryBlocks();
	CWinThread *ptrCapThread;
	void PrintFmtError(char *fmt, ...);
	char* CopyArgv(char **argv);
	char* ReadInFile(char *fname);
	int GetOpt(int argc, char *argv[], char *opstring);
	void GenHomenet(char *netdata);
	int ParseCmdLine(int argc, char *argv[]);
	int Strip(char *data);
	int InitInterface(char *intf);
	char* GetAdapterFromList(void *device,int index);
	void StripQuotes(char *data);
	void ExitOrExec(int stat, int sig);
	int GetTimeOfDay(struct timeval *tv,struct Timezone *tz);
	void CleanExit(int sig);
	void Ts_Print(register const struct timeval *tvp, char *timebuf);
	void ProcessPacket(char *user, struct pcap_pkthdr *pkthdr, u_char *pkt);
	int do_detect;
	CString ptrDecodeType;
	int SetPktProcessor();
	PacketCount pc;	
	u_int snaplen;
	PV pv;	
	pcap_t *pd;  
	char *progname;        
	char **progargs;
	PreprocessKeywordList *PreprocessKeywords;
	PreprocessFuncNode *PreprocessList;
	PluginSignalFuncNode *PluginCleanExitList;
	PluginSignalFuncNode *PluginRestartList;
	u_long *netmasks;
	char *protocol_names[256];
	pcap_dumper_t *dumpd;  
	char *file_name;
	int file_line;
	int list_file_line;
	FILE *log_ptr;         
	FILE *alert;           
	FILE *binlog_ptr;      
	PortList HttpDecodePorts;
	MinFragData FragData;
	OutputFuncNode *AlertList;
	OutputFuncNode *LogList;
	KeywordXlateList *KeywordList;
	OutputKeywordList *OutputKeywords;
	OptTreeNode *otn_tmp; 
	struct sockaddr_un alertaddr; 
	int alertsd;           
	void (CManipulate::*AlertFunc)(Packet*,char*);
	void (CManipulate::*LogFunc)(Packet*,char*);
	void (CDecode::*grinder)(Packet *, struct pcap_pkthdr *, u_char *);
	char *optarg;    
	int   optind;    
	int   opterr;       
	int datalink;       
	ListHead *RuleAlert;
	ListHead *RulePass;
	ListHead *RuleLog;
	RuleTreeNode *rtn_tmp;
	int rule_count;     
	int head_count;     
	int opt_count;      
	struct VarEntry *VarHead;
	SyslogData *syslog_alert_data; 
	LogTcpdumpData *TcpdumpLogInfo;
	SpoAlertSmbData *smb_alert_data;
	SpoAlertFastData *alert_fast_data;
	SpoAlertFullData *alert_full_data;
	char *data_dump_buffer;			
	AccessList *accesslist;
	CRITICAL_SECTION Crit;
	CCriticalSection m_CriticalSection;
	TrafficControlStatus *trafficControl;
	CString m_DenyList;
	CString m_DenyStatus;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIFServerApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CIFServerApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IFSERVER_H__F40031B9_7C3B_4D5C_A63A_F9E12C46CB4A__INCLUDED_)
