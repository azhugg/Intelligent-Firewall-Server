// PortScan.cpp: implementation of the CPortScan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "PortScan.h"
#include "Rules.h"
#include "Log.h"
#include "StringMan.h"
#include "Preprocessor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPortScan::CPortScan()
{

}

CPortScan::~CPortScan()
{

}

void CPortScan::PortscanPreprocFunction(Packet *p)
{
	SourceInfo *currentSource;
	ScanType scanType;
	struct spp_timeval currTime;
	char logMessage[180];
	int numPorts;
	CRules objRule;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	void (CManipulate::*ptrFunAlert)(Packet*,char*);
	ptrFunAlert = ptrMainApp->AlertFunc;
	CManipulate objManip;
    if (p->iph == NULL){
       return;
    }

    switch (p->iph->ip_proto)
    {
        case IPPROTO_TCP:
            if (p->tcph == NULL) 
            {
                return;
            }
            scanType = CheckTCPFlags(p->tcph->th_flags);
            break;

        case IPPROTO_UDP:   
            scanType = sUDP;
            break;

        default:
            return;   
            break;
    }

    if (!scanType) return;
    if (!objRule.CheckAddrPort(homeNet, homeNetMask, 0, 0, p, ANY_DST_PORT, CHECK_DST)) return;
    if (IsServer(p) && !(scanType & sRESERVEDBITS & scansToWatch))
					scanType =(ScanType)(scanType & ~(sSYN | sUDP));

    if (scanType & scansToWatch)  
    {
        currTime.tv_sec = p->pkth->ts.tv_sec;
        currTime.tv_usec = p->pkth->ts.tv_usec;
        ExpireConnections(scanList, maxTime, currTime);
        numPorts = NewScan(scanList, p, scanType);
        scanList->lastSource->lastPacketTime = currTime;

        if ((numPorts > maxPorts) || (scanType & ~(sSYN | sUDP)))
        {
            if (scanType & ~(sSYN | sUDP))
            {
               scanList->lastSource->stealthScanUsed = 1;
               scanList->lastSource->reportStealth = 1;
            }

            if (!scanList->lastSource->scanDetected)
            {
                if (scanList->lastSource->stealthScanUsed)
                {
                    sprintf(logMessage, MODNAME ": PORTSCAN DETECTED from %s (STEALTH)", inet_ntoa(scanList->lastSource->saddr)); 
                }
                else
                {
                    sprintf(logMessage, MODNAME ": PORTSCAN DETECTED from %s (THRESHOLD %ld connections exceeded in %ld seconds)", 
                            inet_ntoa(scanList->lastSource->saddr), maxPorts, 
                            (long int)(currTime.tv_sec - scanList->lastSource->firstPacketTime.tv_sec));
                }
	
				(objManip.*ptrFunAlert)(NULL, logMessage);;
                scanList->lastSource->scanDetected = 1;
                scanList->lastSource->reportTime = currTime;
            }
        }
        currentSource = scanList->listHead;
        while (currentSource){
            if (currentSource->scanDetected){
                if (currentSource->reportTime.tv_sec + maxTime.tv_sec < currTime.tv_sec){
                    if (currentSource->numberOfConnections == 0){
                        sprintf(logMessage, MODNAME ": End of portscan from %s: TOTAL time(%lds) hosts(%d) TCP(%d) UDP(%d)%s", 
                                inet_ntoa(currentSource->saddr), 
                                (long int)(currentSource->lastPacketTime.tv_sec - currentSource->firstPacketTime.tv_sec),
                                currentSource->totalNumberOfDestinations,
                                currentSource->totalNumberOfTCPConnections,
                                currentSource->totalNumberOfUDPConnections,
                                (currentSource->reportStealth) ? " STEALTH" : "");
                        (objManip.*ptrFunAlert)(NULL, logMessage);;
                        currentSource->scanDetected = 0;
                    }
                    else
                    {
                        if (logLevel & lFILE) LogScanInfoToSeparateFile(currentSource);
                        if (logLevel & lEXTENDED) AlertIntermediateInfo(currentSource); 
                        currentSource->totalNumberOfTCPConnections += currentSource->numberOfTCPConnections;
                        currentSource->totalNumberOfUDPConnections += currentSource->numberOfUDPConnections;
                        ClearConnectionInfoFromSource(currentSource);
                        currentSource->stealthScanUsed = 0;
                        currentSource->reportTime = currTime;
                    }
                }
            }
            currentSource = currentSource->nextNode;
        }
    }
}

ScanType CPortScan::CheckTCPFlags(u_char th_flags)
{
	u_char th_flags_cleaned;
	ScanType scan = sNONE;
	th_flags_cleaned = th_flags & ~(R_RES1 | R_RES2);

	if(th_flags != th_flags_cleaned)
	{
		scan = sRESERVEDBITS;
	}

	if(th_flags_cleaned & R_ACK)
	{
		switch(th_flags_cleaned)
		{
			case	(R_ACK):
			case	(R_SYN | R_ACK):
			case	(R_FIN | R_ACK):
			case	(R_RST | R_ACK):
			case	(R_ACK | R_PSH):
			case	(R_ACK | R_URG):
			case	(R_ACK | R_URG | R_PSH):
			case	(R_FIN | R_ACK | R_PSH):
			case	(R_FIN | R_ACK | R_URG):
			case	(R_FIN | R_ACK | R_URG | R_PSH):
			case	(R_RST | R_ACK | R_PSH):
				break;
			case	(R_SYN | R_RST | R_ACK | R_FIN | R_PSH | R_URG):
					scan =(ScanType) (scan | sFULLXMAS);
			
			case (R_SYN | R_PSH | R_ACK | R_URG):
                scan = (ScanType) (scan | sSPAU);
                break;

            default:
                scan = (ScanType) (scan | sINVALIDACK);
                break; 
		}
	}
	else
    {
        switch (th_flags_cleaned)
        {
            case R_SYN:
                scan = (ScanType)(scan | sSYN);
                break;

            case R_RST:
                break;

            case R_FIN:
                scan = (ScanType)(scan | sFIN);
                break;

            case (R_SYN | R_FIN):
                scan = (ScanType)(scan | sSYNFIN);
                break;

            case 0:
                scan = (ScanType)(scan | sNULL);
                break;

            case (R_FIN | R_PSH | R_URG):
                scan = (ScanType)(scan | sXMAS);
                break;

            case R_URG:
            case R_PSH:
            case (R_URG | R_FIN):
            case (R_PSH | R_FIN):
            case (R_URG | R_PSH):
                scan = (ScanType)(scan | sVECNA);
                break;

            case (R_SYN | R_FIN | R_PSH | R_URG):
                scan = (ScanType)(scan | sNMAPID);
                break;

            default:
                scan = (ScanType)(scan | sNOACK);
                break;
        }
    }

    return(scan);
}

int CPortScan::IsServer(Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	ServerNode *currentServer = serverList;
	CRules objRule;

#ifdef DEBUG
    char sourceIP[16], ruleIP[16], ruleNetMask[16];
#endif
   
    while (currentServer){
        if (objRule.CheckAddrPort(currentServer->address, currentServer->netmask,0, 0, p, ANY_SRC_PORT, CHECK_SRC)){
			return(1);
        }
        currentServer = currentServer->nextNode;
    }
    return(0);
}

void CPortScan::ExpireConnections(ScanList *scanList, spp_timeval watchPeriod, spp_timeval currentTime)
{
    SourceInfo *currentSource = scanList->listHead, *tmpSource;
    DestinationInfo *currentDestination, *tmpDestination;
    ConnectionInfo *currentConnection, *tmpConnection;
    if (!scanList->listHead) return;
    while (currentSource){
        if (currentSource->scanDetected){
            currentSource = currentSource->nextNode;
            continue;
        }
        currentDestination = currentSource->destinationsList;
        while (currentDestination){
            currentConnection = currentDestination->connectionsList;
            while (currentConnection){
                if (currentConnection->timestamp.tv_sec + watchPeriod.tv_sec < currentTime.tv_sec){
                   tmpConnection = currentConnection;
                    currentConnection = currentConnection->nextNode;
                    if (tmpConnection->prevNode == NULL){
                        currentDestination->connectionsList = tmpConnection->nextNode;
                    }
                    if (tmpConnection->scanType == sUDP){
                        currentSource->numberOfUDPConnections--;
                    }else{
                        currentSource->numberOfTCPConnections--;
                    }
					RemoveConnection(tmpConnection);
                    currentSource->numberOfConnections--;
                    currentDestination->numberOfConnections--;
                }else{
                    currentConnection = currentConnection->nextNode;
                }
            }
            tmpDestination = currentDestination;
            currentDestination = currentDestination->nextNode;
            if (tmpDestination->numberOfConnections == 0){
                if (tmpDestination->prevNode == NULL){
                    currentSource->destinationsList = tmpDestination->nextNode;
                }
                RemoveDestination(tmpDestination);         
                currentSource->numberOfDestinations--;
            }
        }
        tmpSource = currentSource;
        currentSource = currentSource->nextNode;
        if (tmpSource->numberOfDestinations == 0){
            if (tmpSource->prevNode == NULL){
                scanList->listHead = tmpSource->nextNode;
            }
            RemoveSource(tmpSource);
            scanList->numberOfSources--;
        }
    }     
    if (scanList->numberOfSources == 0){ 
        scanList->listHead = NULL; 
    }
}

int CPortScan::NewScan(ScanList *scanList, Packet *p, ScanType scanType)
{
	SourceInfo *currentSource = scanList->listHead;
    DestinationInfo *currentDestination;
    ConnectionInfo *currentConnection;
    int matchFound = 0;
    struct in_addr saddr;
    struct in_addr daddr;
    u_short sport;
    u_short dport;
    if (!scanList->listHead){
        scanList->listHead = NewSource(p, scanType);
        scanList->numberOfSources = 1;
        scanList->lastSource = scanList->listHead;
        return(scanList->listHead->numberOfConnections);
    }
	ExtractHeaderInfo(p, &saddr, &daddr, &sport, &dport);
    while (!matchFound){
        if (currentSource->saddr.s_addr == saddr.s_addr){
            currentDestination = currentSource->destinationsList;
            if (currentSource->destinationsList == NULL){
                currentSource->destinationsList = NewDestination(p, scanType);
                currentSource->numberOfConnections++;
                if (scanType == sUDP){
                    currentSource->numberOfUDPConnections++;
                }else{
                    currentSource->numberOfTCPConnections++;
                }
                currentSource->numberOfDestinations++;
                matchFound = 1;
            }
            currentDestination = currentSource->destinationsList;

            while (!matchFound){
                if (currentDestination->daddr.s_addr == daddr.s_addr){
                    currentConnection = currentDestination->connectionsList;
                    while (!matchFound){
                        if (currentConnection == NULL) 
//                            FatalError(MODNAME": currentConnection is NULL!!!??\n"); 
                        if ((currentConnection->dport == dport) && (currentConnection->scanType == scanType)){
                            /* If the same exact connection already exists, just update the timestamp. */
                            currentConnection->timestamp.tv_sec = p->pkth->ts.tv_sec;
                            currentConnection->timestamp.tv_usec = p->pkth->ts.tv_usec;
                            currentConnection->sport = sport;
                            matchFound = 1;
                        }else{
                            if (!currentConnection->nextNode){
                                currentConnection = AddConnection(currentConnection, p, scanType);
                                currentSource->numberOfConnections++;
                                if (scanType == sUDP){
                                    currentSource->numberOfUDPConnections++;
                                }else{
                                    currentSource->numberOfTCPConnections++;
                                }

                                currentDestination->numberOfConnections++;
                                matchFound = 1;
                            }else currentConnection = currentConnection->nextNode;
                        }                     
                    }
                }else{
                    if (!currentDestination->nextNode){  
                        currentDestination = AddDestination(currentDestination, p, scanType);
                        currentSource->numberOfConnections++;
                        if (scanType == sUDP){ 
                            currentSource->numberOfUDPConnections++;
                        }else{
                            currentSource->numberOfTCPConnections++;
                        }
                        currentSource->numberOfDestinations++;
                        currentSource->totalNumberOfDestinations++;
                        matchFound = 1;
                    }else currentDestination = currentDestination->nextNode;
                }
            }
        }else{
            if (!currentSource->nextNode){
                currentSource = AddSource(currentSource, p, scanType);
                currentSource->numberOfConnections = 1;
                if (scanType == sUDP){
                    currentSource->numberOfUDPConnections = 1;
                }else{
                    currentSource->numberOfTCPConnections = 1;
                }
                scanList->numberOfSources++;
                matchFound = 1;
            }else currentSource = currentSource->nextNode;
        }
    }
    scanList->lastSource = currentSource;
    return(currentSource->numberOfConnections);
}

void CPortScan::LogScanInfoToSeparateFile(SourceInfo *currentSource)
{
	DestinationInfo *currentDestination;
    ConnectionInfo *currentConnection;
    char *scanType;
    char *reservedBits;
    char *month;
    struct tm *time;
    char sourceAddress[16], destinationAddress[16]; 
    char outBuf[160];     
    memset(sourceAddress, '\0', 16);
    memset(destinationAddress, '\0', 16);
    strncpy(sourceAddress, inet_ntoa(currentSource->saddr), 15);
    for (currentDestination = currentSource->destinationsList; currentDestination; 
            currentDestination = currentDestination->nextNode){
        strncpy(destinationAddress, inet_ntoa(currentDestination->daddr), 15);
        for (currentConnection = currentDestination->connectionsList; currentConnection;
                currentConnection = currentConnection->nextNode){
            time = (timeFormat == tLOCAL) ? localtime(&currentConnection->timestamp.tv_sec)
                    : gmtime(&currentConnection->timestamp.tv_sec);
            switch (time->tm_mon){
                case 0:
                    month = "Jan";
                    break;
                case 1:
                    month = "Feb";
                    break;
                case 2:
                    month = "Mar";
                    break;
                case 3:
                    month = "Apr";
                    break;
                case 4:
                    month = "May";
                    break;
                case 5:
                    month = "Jun";
                    break;
                case 6:
                    month = "Jul";
                    break;
                case 7:
                    month = "Aug";
                    break;
                case 8:
                    month = "Sep";
                    break;
                case 9:
                    month = "Oct";
                    break;
                case 10:
                    month = "Nov";
                    break;
                case 11:
                    month = "Dec";
                    break;
                default:
                    month = "MONTH IS INVALID!!";
                    break;
            }

            reservedBits = (currentConnection->scanType & sRESERVEDBITS) ? "RESERVEDBITS" : "";
            switch (currentConnection->scanType & ~sRESERVEDBITS){
                case sUDP:
                    scanType = "UDP";
                    break;
                case sSYN:
                    scanType = "SYN";
                    break;
                case sFIN:
                    scanType = "FIN";
                    break;            
                case sSYNFIN:
                    scanType = "SYNFIN";
                    break;
                case sNULL:
                    scanType = "NULL";
                    break;
                case sXMAS:
                    scanType = "XMAS";
                    break;
                case sFULLXMAS:
                    scanType = "FULLXMAS";
                    break;
                case sVECNA:
                    scanType = "VECNA";
                    break;
                case sNOACK:
                    scanType = "NOACK";
                    break;
                case sNMAPID:
                    scanType = "NMAPID";
                    break;
                case sSPAU:
                    scanType = "SPAU";
                    break;
                case sINVALIDACK:
                    scanType = "INVALIDACK";
                    break;
                default:
                    scanType = "UNKNOWN";
                    break;
            }
            sprintf(outBuf, "%s %2d %.2d:%.2d:%.2d %s:%d -> %s:%d %s %s %s\n", month, time->tm_mday, 
                    time->tm_hour, time->tm_min, time->tm_sec,
                    sourceAddress, currentConnection->sport, destinationAddress,
                    currentConnection->dport, scanType, currentConnection->tcpFlags, reservedBits);
            fwrite(outBuf, strlen(outBuf), 1, logFile);
        }
    }
     fflush(logFile);
}


void CPortScan::AlertIntermediateInfo(SourceInfo *currentSource)
{
	char logMessage[160];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	void (CManipulate::*ptrFunAlert)(Packet*,char*);
	ptrFunAlert = ptrMainApp->AlertFunc;
	CManipulate objManip;
	sprintf(logMessage, MODNAME ": portscan status from %s: %d connections across %d hosts: TCP(%d), UDP(%d)%s",
            inet_ntoa(currentSource->saddr), currentSource->numberOfConnections, currentSource->numberOfDestinations,
            currentSource->numberOfTCPConnections, currentSource->numberOfUDPConnections, 
            (currentSource->stealthScanUsed) ? " STEALTH" : "");
	(objManip.*ptrFunAlert)(NULL, logMessage);
}


void CPortScan::ClearConnectionInfoFromSource(SourceInfo *currentSource)
{
	DestinationInfo *currentDestination, *tmpDestination;
    ConnectionInfo *currentConnection, *tmpConnection;
    currentDestination = currentSource->destinationsList;
    while (currentDestination){
        currentConnection = currentDestination->connectionsList;
        while (currentConnection){
            tmpConnection = currentConnection;
            currentConnection = currentConnection->nextNode;
            if (tmpConnection->scanType == sUDP){
                currentSource->numberOfUDPConnections--;
            }else{
                currentSource->numberOfTCPConnections--;
            }
            RemoveConnection(tmpConnection);
            currentDestination->numberOfConnections--;
            currentSource->numberOfConnections--;
        }
        tmpDestination = currentDestination;
        currentDestination = currentDestination->nextNode;
        RemoveDestination(tmpDestination);
        currentSource->numberOfDestinations--;
    }
    currentSource->destinationsList = NULL;
}

void CPortScan::RemoveConnection(ConnectionInfo *delConnection)
{
    if (delConnection->prevNode || delConnection->nextNode){
        if (delConnection->prevNode){
            delConnection->prevNode->nextNode = delConnection->nextNode;
        }else if (delConnection->nextNode){
            delConnection->nextNode->prevNode = NULL;
        }
        if (delConnection->nextNode){
            delConnection->nextNode->prevNode = delConnection->prevNode;
        }else if (delConnection->prevNode){
            delConnection->prevNode->nextNode = NULL;
        }
    }
    free(delConnection);
}


void CPortScan::RemoveDestination(DestinationInfo *delDestination)
{
    if (delDestination->prevNode || delDestination->nextNode){
        if (delDestination->prevNode){
            delDestination->prevNode->nextNode = delDestination->nextNode;
        }else if (delDestination->nextNode){
            delDestination->nextNode->prevNode = NULL;
        }if (delDestination->nextNode){
            delDestination->nextNode->prevNode = delDestination->prevNode;
        }else if (delDestination->prevNode){
            delDestination->prevNode->nextNode = NULL;
        }
    }
    free(delDestination);
}

void CPortScan::RemoveSource(SourceInfo *delSource)
{
    if (delSource->prevNode || delSource->nextNode){
        if (delSource->prevNode){
            delSource->prevNode->nextNode = delSource->nextNode;
        }else if (delSource->nextNode){
            delSource->nextNode->prevNode = NULL;
        }
        if (delSource->nextNode){
            delSource->nextNode->prevNode = delSource->prevNode;
        }else if (delSource->prevNode){
            delSource->prevNode->nextNode = NULL;
        }
    }
    free(delSource);
}

SourceInfo* CPortScan::NewSource(Packet *p, ScanType scanType)
{
	SourceInfo *newSource = (SourceInfo*)malloc(sizeof(SourceInfo));
    newSource->prevNode = NULL;
    newSource->nextNode = NULL;
    newSource->saddr = p->iph->ip_src;  
    newSource->numberOfConnections = 1;
    newSource->firstPacketTime.tv_sec = p->pkth->ts.tv_sec;
    newSource->firstPacketTime.tv_usec = p->pkth->ts.tv_usec;
    newSource->lastPacketTime.tv_sec =p->pkth->ts.tv_sec;
    newSource->lastPacketTime.tv_usec =p->pkth->ts.tv_usec;
    if (scanType == sUDP){
        newSource->numberOfUDPConnections = 1;
        newSource->numberOfTCPConnections = 0;
    }else{
        newSource->numberOfTCPConnections = 1;
        newSource->numberOfUDPConnections = 0;
    }
    newSource->totalNumberOfTCPConnections = 0; 
    newSource->totalNumberOfUDPConnections = 0; 
    newSource->stealthScanUsed = 0;  
    newSource->scanDetected = 0;
    newSource->destinationsList = NewDestination(p, scanType);
    newSource->numberOfDestinations = 1;
    newSource->totalNumberOfDestinations = 1;
    newSource->reportStealth = 0;    
    return(newSource);
}

void CPortScan::ExtractHeaderInfo(Packet *p, in_addr *saddr, in_addr *daddr, u_short *sport, u_short *dport)
{
    *sport = p->sp;
    *dport = p->dp;
    *saddr = p->iph->ip_src;
    *daddr = p->iph->ip_dst;
}

DestinationInfo* CPortScan::NewDestination(Packet *p, ScanType scanType)
{
	DestinationInfo *newDestination = (DestinationInfo*)malloc(sizeof(DestinationInfo));
    newDestination->prevNode = NULL;
    newDestination->nextNode = NULL;
    newDestination->daddr = p->iph->ip_dst;
    newDestination->connectionsList = NewConnection(p, scanType);
    newDestination->numberOfConnections = 1;
    return(newDestination);
}

DestinationInfo* CPortScan::AddDestination(DestinationInfo *currentDestination, Packet *p, ScanType scanType)
{
	if (currentDestination->nextNode) 
//		FatalError(MODNAME":  AddDestination():  Not at end of destination list!");
   
    currentDestination->nextNode = NewDestination(p, scanType);
    currentDestination->nextNode->prevNode = currentDestination;
    return(currentDestination->nextNode);
}


ConnectionInfo* CPortScan::AddConnection(ConnectionInfo *currentConnection, Packet *p, ScanType scanType)
{
    if (currentConnection->nextNode) 
        //FatalError(MODNAME":  AddConnection():  Not at end of connection list!"); 
  
    currentConnection->nextNode = NewConnection(p, scanType);
    currentConnection->nextNode->prevNode = currentConnection;   

    return(currentConnection->nextNode);
}


SourceInfo* CPortScan::AddSource(SourceInfo *currentSource, Packet *p, ScanType scanType)
{
	if (currentSource->nextNode) 
//        FatalError(MODNAME":  AddSource():  Not at end of source list!");
   
    currentSource->nextNode = NewSource(p, scanType);
    currentSource->nextNode->prevNode = currentSource;
   
    return(currentSource->nextNode);
}

ConnectionInfo* CPortScan::NewConnection(Packet *p, ScanType scanType)
{
	ConnectionInfo *newConnection = (ConnectionInfo*)malloc(sizeof(ConnectionInfo));
	CLog objLog;
    newConnection->prevNode = NULL;
    newConnection->nextNode = NULL;
    newConnection->scanType = scanType;
    newConnection->timestamp.tv_sec = p->pkth->ts.tv_sec; 
    newConnection->timestamp.tv_usec = p->pkth->ts.tv_usec; 
    newConnection->sport = p->sp;
    newConnection->dport = p->dp;
    switch (p->iph->ip_proto){
        case IPPROTO_TCP:
            objLog.CreateTCPFlagString(p, newConnection->tcpFlags);
            if (logLevel & lPACKET);
	           break;
        case IPPROTO_UDP:   
            strncpy(newConnection->tcpFlags, "\0", 1);
            if (logLevel & lPACKET);
            break;
        default:
            break;
    }
     return(newConnection);
}

void CPortScan::ParsePortscanArgs(u_char *args)
{
	char **toks;
    int numToks;
    char *logFileName;
    logLevel = lNONE;
    if (!args){
//        FatalError(MODNAME": ERROR: %s (%d) => portscan configuration format:  address/mask ports seconds [logfile]\n", file_name, file_line);
    }

	CStringMan objStrman;
    toks = objStrman.mSplit((char*)args, " ", 4, &numToks, '\\');  /*ZDNOTE What does the '\\' do? */
    if ((numToks < 3) || (numToks > 4)){
//        FatalError(MODNAME": ERROR: %s (%d) => portscan configuration format:  address/mask ports seconds [logfile]\n", file_name, file_line);
    }
    maxPorts = atoi(toks[1]);
    maxTime.tv_sec = atoi(toks[2]);
    maxTime.tv_usec = 0;
	CRules objRule;
	objRule.ParseIP(toks[0], &homeNet, &homeNetMask);
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    if (numToks == 4){
        char *logdir;
		logdir = new char[STD_BUF];
		logdir = ptrMainApp->pv.log_dir;
        if (logdir && (*toks[3] != '/')){
            if (*(logdir + strlen(logdir) - 1) != '/'){
				logFileName = (char*)malloc(strlen(logdir) + strlen(toks[3]) + 1 + 1);
				strncpy(logFileName, logdir, strlen(logdir));
                strncat(logFileName, "/", 1);
                strncat(logFileName, toks[3], strlen(toks[3]));
            }else{
                logFileName = (char*)malloc(strlen(logdir) + strlen(toks[3]) + 1);
                strncpy(logFileName, logdir, strlen(logdir));
                strncat(logFileName, toks[3], strlen(toks[3]));
            }
        }else{
            logFileName = (char*)malloc(strlen(toks[3]) + 1);
            strncpy(logFileName, toks[3], strlen(toks[3]));
        }
        logFile = fopen(logFileName, "a");
        if (!logFile);
			//FatalError(MODNAME": logfile open error (%s)\n", logFileName);
		logLevel = (LogLevel)(logLevel | lFILE);
    }
    if (maxPorts == 0 || maxTime.tv_sec == 0){
//        FatalError(MODNAME": ERROR: %s (%d) => portscan configuration format:  address/mask ports seconds [logfile]\n", file_name, file_line);
    }
	scansToWatch = (ScanType) -1;   
	logLevel = (LogLevel)(logLevel | lEXTENDED); 
    packetLogSize = 100;
    timeFormat = tLOCAL;  
}

ScanList* CPortScan::CreateScanList()
{
	ScanList *newList = (ScanList*)malloc(sizeof(ScanList));
    newList->listHead = NULL;
    newList->lastSource = NULL;
    newList->numberOfSources = 0;
    return(newList);
}

void CPortScan::PortscanInit(u_char *args)
{
	ParsePortscanArgs(args); 
    scanList = CreateScanList();
	CRules objRule;
	serverList = NULL;
    void (CManipulate::*ptrfun)(Packet*);
	ptrfun = &CManipulate::PortscanPreprocFunction;
	objRule.AddFuncToPreprocList(ptrfun);
}


void CPortScan::SetupPortscan()
{
	CPreprocessor objPrep;
	void (CManipulate::*ptrfun)(u_char*);
	ptrfun = &CManipulate::PortscanInit;
	objPrep.RegisterPreprocessor("portscan",ptrfun);
}

void CPortScan::CreateServerList(u_char *servers)
{
	char **toks;
    int num_toks;
    int num_servers = 0;
    ServerNode *currentServer;
    currentServer = NULL;
    serverList = NULL;
    if (servers == NULL){
  //      FatalError(MODNAME": ERROR: %s (%d)=> No arguments to portscan-ignorehosts preprocessor!\n", file_name, file_line);
    }
	CStringMan objString;
	toks = objString.mSplit((char*)servers, " ", 31, &num_toks, '\\');
    for (num_servers = 0; num_servers < num_toks; num_servers++){
        if (currentServer != NULL){
            currentServer->nextNode = (ServerNode*)malloc(sizeof(ServerNode));
            currentServer = currentServer->nextNode;
        }else{
            currentServer = (ServerNode*)malloc(sizeof(ServerNode));
            serverList = currentServer;
        }
		CRules objRule;
        objRule.ParseIP(toks[num_servers], &currentServer->address, &currentServer->netmask);
        currentServer->nextNode = NULL;
    }
    free(toks);  
}

void CPortScan::PortscanIgnoreHostsInit(u_char *args)
{
	CreateServerList(args);
}


void CPortScan::SetupPortscanIgnoreHosts()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char*);
	ptrfun = &CManipulate::PortscanIgnoreHostsInit;
	objPrep->RegisterPreprocessor("portscan-ignorehosts",ptrfun);		
}
