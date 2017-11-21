#ifndef __STRUCTURE_H__
#define __STRUCTURE_H__

#ifndef CManipulate
#include "Manipulate.h"
#endif

#ifndef CStringMan
#include "Stringman.h"
#endif
typedef struct _PreprocessKeywordNode{
	char* keyword;
	void (CManipulate::*func)(u_char*);
} PreprocessKeywordNode;
typedef struct _PreprocessKeywordList{
	PreprocessKeywordNode entry;
	struct _PreprocessKeywordList *next;
}PreprocessKeywordList;
typedef struct _PreprocessFuncNode{
    void (CManipulate::*func)(Packet *);
    struct _PreprocessFuncNode *next;

} PreprocessFuncNode;
typedef struct _PluginSignalFuncNode{
    void (CManipulate::*func)(int, void*);
       void *arg;
    struct _PluginSignalFuncNode *next;

} PluginSignalFuncNode;
struct spp_timeval{
	time_t tv_sec;
	time_t tv_usec;
};
typedef enum _scanType{
	sNONE=0, sUDP=1, sSYN=2, sSYNFIN=4, sFIN=8, sNULL=16,
    sXMAS=32, sFULLXMAS=64, sRESERVEDBITS=128, sVECNA=256, sNOACK=512, sNMAPID=1024,
    sSPAU=2048, sINVALIDACK=4096 
}ScanType;
typedef enum _logLevel{ 
    lNONE=0, lFILE=1, lEXTENDED=2, lPACKET=4 
} LogLevel;
typedef struct _connectionInfo{
    ScanType scanType;
    u_short sport;
    u_short dport;
    struct spp_timeval timestamp;
    char tcpFlags[9]; 
    u_char *packetData;
    struct _connectionInfo *prevNode;
    struct _connectionInfo *nextNode;
}  ConnectionInfo;
typedef struct _destinationInfo{
    struct in_addr daddr;
    int numberOfConnections;
    ConnectionInfo *connectionsList;
    struct _destinationInfo *prevNode;
    struct _destinationInfo *nextNode;
}  DestinationInfo;
typedef struct _sourceInfo{
    struct in_addr saddr;
    int numberOfConnections;
    int numberOfDestinations; 
    int numberOfTCPConnections;
    int numberOfUDPConnections;
    int totalNumberOfTCPConnections;
    int totalNumberOfUDPConnections;
    int totalNumberOfDestinations; 
    struct spp_timeval firstPacketTime;
    struct spp_timeval lastPacketTime;
    int reportStealth;
    int stealthScanUsed;
    int scanDetected;
    struct spp_timeval reportTime;           
    DestinationInfo *destinationsList;
    struct _sourceInfo *prevNode;
    struct _sourceInfo *nextNode;
}SourceInfo;
typedef struct _scanList{
    SourceInfo *listHead;
    SourceInfo *lastSource;
    long numberOfSources;        
}  ScanList;
typedef struct _serverNode{
    u_long address;
    u_long netmask;
    struct _serverNode *nextNode;
}  ServerNode;
struct VarEntry{
    char *name;
    char *value;
    struct VarEntry *prev;
    struct VarEntry *next;
};
typedef struct _timestruct{
        u_int32_t tv_sec;
        u_int32_t tv_usec;
} time_struct;
typedef Packet *frag;
typedef struct tree_node Tree;	
struct tree_node{
		Tree * left, * right;
		frag key;
		int size;
	};
struct _OptTreeNode;      
struct _RuleTreeNode;     
typedef struct _RuleFpList{
    int (CManipulate::*RuleHeadFunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
    struct _RuleFpList *next;
} RuleFpList;
typedef struct _OptFpList{
    int (CManipulate::*OptTestFunc)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
    struct _OptFpList *next;
} OptFpList;
typedef struct _OptTreeNode{
    OptFpList *opt_func;
    void *ds_list[512];   
    int chain_node_number;
    int type;            
    int proto;           
    int session_flag;    
    char *logto;         
    char *message;
    int response_flag;   
    struct _OptTreeNode *next;
} OptTreeNode;
typedef struct _KeywordXlate{
    char *keyword;
    void (CManipulate::*func)(char *, OptTreeNode *, int);
} KeywordXlate;
typedef struct _KeywordXlateList{
    KeywordXlate entry;
    struct _KeywordXlateList *next;
} KeywordXlateList;
typedef struct _RuleTreeNode{
    RuleFpList *rule_func;
    int head_node_number;
    int type;
    u_long sip;          
    u_long smask;        
    u_long dip;          
    u_long dmask;        
    int not_sp_flag;     
    u_short hsp;         
    u_short lsp;         
    int not_dp_flag;     
    u_short hdp;         
    u_short ldp;         
    u_char flags;        
    struct _RuleTreeNode *right;
    OptTreeNode *down;   
} RuleTreeNode;
typedef struct _ListHead{
    RuleTreeNode *TcpList;
    RuleTreeNode *UdpList;
    RuleTreeNode *IcmpList;
} ListHead; 
typedef struct _OutputKeywordNode{
    char *keyword;
    char node_type;
    void (CManipulate::*func)(u_char *);
} OutputKeywordNode;
typedef struct _OutputKeywordList{
    OutputKeywordNode entry;
    struct _OutputKeywordList *next;
} OutputKeywordList;
typedef struct _OutputFuncNode{
    void (CManipulate::*func)(Packet *, char *, void *);
    void *arg;
    struct _OutputFuncNode *next;
} OutputFuncNode;
typedef struct _PatternMatchData{
    int offset;             
    int depth;              
    u_int pattern_size;     
    char *pattern_buf;      
    int (CManipulate::*search)(char *, int, char *, int, int *, int *); 
    int *skip_stride; 
    int *shift_stride; 
    struct _PatternMatchData *next; 
} PatternMatchData;
typedef struct _TCPFlagCheckData{
    u_char tcp_flags; 
} TCPFlagCheckData;
typedef struct _IcmpTypeCheckData{
    int icmp_type;
} IcmpTypeCheckData;
typedef struct _IcmpCodeCheckData{
    int icmp_code;
} IcmpCodeCheckData;
typedef struct _TtlCheckData{
    int ttl;
} TtlCheckData;
typedef struct _IpIdData{
    u_long ip_id;
} IpIdData;
typedef struct _TcpAckCheckData{
    u_long tcp_ack;
} TcpAckCheckData;
typedef struct _TcpSeqCheckData{
    u_long tcp_seq;
} TcpSeqCheckData;
typedef struct _DsizeCheckData{
    int dsize;
} DsizeCheckData;
typedef struct _IpOptionData{
    u_char ip_option;
} IpOptionData;
typedef struct _RpcCheckData{
    u_long program; 
    u_long vers; 
    u_long proc; 
    int flags; 
} RpcCheckData;
typedef struct _IcmpIdData{
    u_short icmpid;
} IcmpIdData;
typedef struct _IcmpSeqData{
    unsigned short icmpseq;
} IcmpSeqData;
typedef struct _SessionData{
    int session_flag;
} SessionData;
typedef struct _SyslogData{
    int facility;
    int priority;
    int options;
} SyslogData;
typedef struct _LogTcpdumpData{
    char *filename;
    pcap_dumper_t *dumpd;
} LogTcpdumpData;
typedef struct _SpoAlertFastData{
    FILE *file;
} SpoAlertFastData;
typedef struct _SpoAlertFullData{
	FILE *file;
} SpoAlertFullData;
typedef struct _SpoAlertSmbData{
    char *filename;
} SpoAlertSmbData;
typedef struct _AlertNode{
	Packet *Pkt;
	char   *AlertType;
	char   *TimeStamp;
	u_int  Weight;
	_AlertNode *next;
}AlertNode;
typedef struct _CriticalClientList{
	AlertNode *ClientQueue[256];
}CriticalClientList;
typedef struct _AccessList{
	CriticalClientList *TCPList;
	CriticalClientList *UDPList;
	CriticalClientList *ICMPList;
}AccessList;
typedef struct _TrafficControlStatus{
	char* RulesOnInterface;
	char* StatisticsOnInterface;
}TrafficControlStatus;

#endif /*__STRUCTURE_H__*/