// Log.cpp: implementation of the CLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Log.h"
#include <Direct.h>
#include "SysLog.h"
#include <Process.h>
#include <stdio.h>
#include <malloc.h>
#include "TrafficControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define FIRST_CLIENT	0
#define FIRST_ATTEMPT	1
#define MAX_CLIENTS		25


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLog::CLog()
{
	ptrMainApp =PTRAPP;
}

CLog::~CLog()
{

}

void CLog::ClearDumpBuf()
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	if (ptrMainApp->data_dump_buffer != NULL && dump_ready);
	ptrMainApp->data_dump_buffer = NULL;
	dump_ready = 0;
}

void CLog::CreateTCPFlagString(Packet *p, char *flagBuffer)
{
	memset(flagBuffer, '\0', 9);
    *flagBuffer++ = (p->tcph->th_flags & TH_RES2) ? '2' : '*';
    *flagBuffer++ = (p->tcph->th_flags & TH_RES1) ? '1' : '*';
    *flagBuffer++ = (p->tcph->th_flags & TH_SYN)  ? 'S' : '*';
    *flagBuffer++ = (p->tcph->th_flags & TH_FIN)  ? 'F' : '*';
	*flagBuffer++ = (p->tcph->th_flags & TH_RST)  ? 'R' : '*';
    *flagBuffer++ = (p->tcph->th_flags & TH_PUSH) ? 'P' : '*';
    *flagBuffer++ = (p->tcph->th_flags & TH_ACK)  ? 'A' : '*';
    *flagBuffer++ = (p->tcph->th_flags & TH_URG)  ? 'U' : '*'; 
}

void CLog::PrintIPPkt(FILE *fp, int type, Packet *p)
{
	char timestamp[23];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    bzero((char *)timestamp, 23);
    ptrMainApp->Ts_Print((struct timeval*)&p->pkth->ts, timestamp);
    fwrite(timestamp, 22, 1, fp);
    if (ptrMainApp->pv.show2hdr_flag){
        Print2ndHeader(fp, p);
    }
    PrintIPHeader(fp, p);
    if (!p->frag_flag){
        switch (p->iph->ip_proto)
        {
            case IPPROTO_TCP:
                if(p->tcph != NULL)
                    PrintTCPHeader(fp, p);
                else
                    PrintNetData(fp, p->pkt, p->pkth->caplen); 
                    
                break;

            case IPPROTO_UDP:
                if(p->udph != NULL)
                    PrintUDPHeader(fp, p);
                else
                    PrintNetData(fp, p->pkt, p->pkth->caplen); 
                    
                break;

            case IPPROTO_ICMP:
                if(p->icmph != NULL)
                    PrintICMPHeader(fp, p);
                else
                    PrintNetData(fp, p->pkt, p->pkth->caplen); 

                break;

            default: 
                
                break;
        }
    }

    if (ptrMainApp->pv.data_flag)
    {
        if (ptrMainApp->pv.char_data_flag)
            PrintCharData(fp, p->data, p->dsize);
        else
            PrintNetData(fp, p->data, p->dsize); 
    }

    fprintf(fp, "=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+\n");
}

void CLog::Print2ndHeader(FILE *fp, Packet *p)
{
	if (p->eh)
        PrintEthHeader(fp, p);
    if (p->trh)
        PrintTrHeader(fp, p);
}

void CLog::PrintEthHeader(FILE *fp, Packet *p)
{
    fprintf(fp, "%X:%X:%X:%X:%X:%X -> ", p->eh->ether_src[0],
            p->eh->ether_src[1], p->eh->ether_src[2], p->eh->ether_src[3],
            p->eh->ether_src[4], p->eh->ether_src[5]);
    fprintf(fp, "%X:%X:%X:%X:%X:%X ", p->eh->ether_dst[0],
            p->eh->ether_dst[1], p->eh->ether_dst[2], p->eh->ether_dst[3],
            p->eh->ether_dst[4], p->eh->ether_dst[5]);
    fprintf(fp, "type:0x%X len:0x%X\n", ntohs(p->eh->ether_type), p->pkth->len);
}


void CLog::PrintTrHeader(FILE *fp, Packet *p)
{
	fprintf(fp, "%X:%X:%X:%X:%X:%X -> ", p->trh->saddr[0],
            p->trh->saddr[1], p->trh->saddr[2], p->trh->saddr[3],
            p->trh->saddr[4], p->trh->saddr[5]);
    fprintf(fp, "%X:%X:%X:%X:%X:%X\n", p->trh->daddr[0],
            p->trh->daddr[1], p->trh->daddr[2], p->trh->daddr[3],
            p->trh->daddr[4], p->trh->daddr[5]);
    fprintf(fp, "access control:0x%X frame control:0x%X\n", p->trh->ac,
            p->trh->fc);
    if (!p->trhllc)
        return;
    fprintf(fp, "DSAP: 0x%X SSAP 0x%X protoID: %X%X%X Ethertype: %X\n",
            p->trhllc->dsap, p->trhllc->ssap, p->trhllc->protid[0],
            p->trhllc->protid[1], p->trhllc->protid[2], p->trhllc->ethertype);
    if (p->trhmr)
    {
        fprintf(fp, "RIF structure is present:\n");
        fprintf(fp, "bcast: 0x%X length: 0x%X direction: 0x%X largest"
                "fr. size: 0x%X res: 0x%X\n",
                p->trhmr->bcast, p->trhmr->len, p->trhmr->dir, p->trhmr->lf,
                p->trhmr->res);
        fprintf(fp, "rseg -> %X:%X:%X:%X:%X:%X:%X:%X\n",
                p->trhmr->rseg[0], p->trhmr->rseg[1], p->trhmr->rseg[2],
                p->trhmr->rseg[3], p->trhmr->rseg[4], p->trhmr->rseg[5],
                p->trhmr->rseg[6], p->trhmr->rseg[7]);
    }
}

void CLog::PrintIPHeader(FILE *fp, Packet *p)
{

	CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
	if (p->frag_flag || (p->iph->ip_proto == IPPROTO_ICMP)){
        if(!ptrMainApp->pv.obfuscation_flag)
        {
            fputs(inet_ntoa(p->iph->ip_src), fp);
            fwrite(" -> ", 4, 1, fp);
            fputs(inet_ntoa(p->iph->ip_dst), fp);
        }else{
            if(ptrMainApp->pv.homenet != 0){
               if((p->iph->ip_dst.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
                   fputs(inet_ntoa(p->iph->ip_src), fp);
                   fwrite(" -> xxx.xxx.xxx.xxx", 19, 1, fp);
               }else if((p->iph->ip_src.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
                   fwrite("xxx.xxx.xxx.xxx ->", 18, 1, fp);
                   fputs(inet_ntoa(p->iph->ip_dst), fp);
               }else{
                   fputs("xxx.xxx.xxx.xxx -> xxx.xxx.xxx.xxx", fp);
               }
            }else{
                fputs("xxx.xxx.xxx.xxx -> xxx.xxx.xxx.xxx", fp);
            }
        }
    }else{ if(!ptrMainApp->pv.obfuscation_flag)
			{
	            fputs(inet_ntoa(p->iph->ip_src), fp);
				fprintf(fp, ":%d -> ", p->sp);
				fputs(inet_ntoa(p->iph->ip_dst), fp);
				fprintf(fp, ":%d", p->dp);
			}else{
            if(ptrMainApp->pv.homenet != 0){
               if((p->iph->ip_dst.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
                   fputs(inet_ntoa(p->iph->ip_src), fp);
                   fprintf(fp, ":%d -> xxx.xxx.xxx.xxx:%d", p->sp, p->dp);
               }
               else if((p->iph->ip_src.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
                   fprintf(fp, "xxx.xxx.xxx.xxx:%d -> %s:%d",p->sp, inet_ntoa(p->iph->ip_dst), p->dp);
               }else{
                   fprintf(fp, "xxx.xxx.xxx.xxx:%d -> xxx.xxx.xxx.xxx:%d", p->sp, p->dp);
               }
            }else{
                fprintf(fp, "xxx.xxx.xxx.xxx:%d -> xxx.xxx.xxx.xxx:%d", p->sp, p->dp);
            }
         }
    }
    if (!ptrMainApp->pv.show2hdr_flag){
        fputc('\n', fp);
    }else{
        fputc(' ', fp);
    } 

    if(ptrMainApp->protocol_names[p->iph->ip_proto][0] != 0){
        fprintf(fp, "%s ", ptrMainApp->protocol_names[p->iph->ip_proto]);
    }else{
        fprintf(fp, "Proto: %d ", p->iph->ip_proto);
    }
    fprintf(fp, "TTL:%d TOS:0x%X ID:%d ", p->iph->ip_ttl, p->iph->ip_tos, 
            ntohs(p->iph->ip_id));
    if (p->df)
        fprintf(fp, " DF");
    if (p->mf)
        fprintf(fp, " MF");
    fputc('\n', fp);
    if (p->ip_option_count != 0){
        PrintIpOptions(fp, p);
    }
    if (p->frag_flag){
        fprintf(fp, "Frag Offset: 0x%X   Frag Size: 0x%X", 
                (p->frag_offset & 0xFFFF), p->dsize);
        fputc('\n', fp);
    }
}

void CLog::PrintTCPHeader(FILE *fp, Packet *p)
{
	char tcpFlags[9];
    if(p->tcph == NULL){
        return;
    }
    CreateTCPFlagString(p, tcpFlags);
    fwrite(tcpFlags, 8, 1, fp);
    fprintf(fp, " Seq: 0x%lX   Ack: 0x%lX   Win: 0x%X\n",
            (u_long)ntohl(p->tcph->th_seq),
            (u_long)ntohl(p->tcph->th_ack), 
            ntohs(p->tcph->th_win));
    if (p->tcp_option_count != 0){
        PrintTcpOptions(fp, p); 
    }
}

void CLog::PrintUDPHeader(FILE *fp, Packet *p)
{
	if(p->udph == NULL){
        return;
    }
    fprintf(fp, "Len: %d\n", ntohs(p->udph->uh_len));
}


void CLog::PrintICMPHeader(FILE *fp, Packet *p)
{
	if(p->icmph == NULL){
        return;
    }
    switch (p->icmph->type){
        case ICMP_ECHOREPLY:
            fprintf(fp, "ID:%d   Seq:%d  ", p->icmph->ih_id, p->icmph->ih_seq);
            fwrite("ECHO REPLY\n", 10, 1, fp);
            break;
        case ICMP_DEST_UNREACH:
            fwrite("DESTINATION UNREACHABLE: ", 25, 1, fp); 
            switch (p->icmph->code){
                case ICMP_NET_UNREACH:
                    fwrite("NET UNREACHABLE", 15, 1, fp);
                    break;
                case ICMP_HOST_UNREACH:
                    fwrite("HOST UNREACHABLE", 16, 1, fp);
                    break;
                case ICMP_PROT_UNREACH:
                    fwrite("PROTOCOL UNREACHABLE", 20, 1, fp);
                    break;
                case ICMP_PORT_UNREACH:
                    fwrite("PORT UNREACHABLE", 16, 1, fp);
                    break;
                case ICMP_FRAG_NEEDED:
                    fwrite("FRAGMENTATION NEEDED", 20, 1, fp);
                    break;
                case ICMP_SR_FAILED:
                    fwrite("SOURCE ROUTE FAILED", 19, 1, fp);
                    break;
                case ICMP_NET_UNKNOWN:
                    fwrite("NET UNKNOWN", 11, 1, fp);
                    break;
                case ICMP_HOST_UNKNOWN:
                    fwrite("HOST UNKNOWN", 12, 1, fp);
                    break;
                case ICMP_HOST_ISOLATED:
                    fwrite("HOST ISOLATED", 13, 1, fp);
                    break;
                case ICMP_NET_ANO:
                    fwrite("NET ANO", 7, 1, fp);
                    break;
                case ICMP_HOST_ANO:
                    fwrite("HOST ANO", 8, 1, fp);
                    break;
                case ICMP_NET_UNR_TOS:
                    fwrite("NET UNREACHABLE TOS", 19, 1, fp);
                    break;
                case ICMP_HOST_UNR_TOS:
                    fwrite("HOST UNREACHABLE TOS", 20, 1, fp);
                    break;
                case ICMP_PKT_FILTERED:
                    fwrite("PACKET FILTERED", 15, 1, fp);
                    break;
                case ICMP_PREC_VIOLATION:
                    fwrite("PREC VIOLATION", 14, 1, fp);
                    break;
                case ICMP_PREC_CUTOFF:
                    fwrite("PREC CUTOFF", 12, 1, fp);
                    break;
                default:
                    fwrite("UNKNOWN", 7, 1, fp);
                    break;
            }
            break;
        case ICMP_SOURCE_QUENCH:
            fwrite("SOURCE QUENCH", 13, 1, fp);
            break;
        case ICMP_REDIRECT:
            fwrite("REDIRECT", 8, 1, fp);
            break;
        case ICMP_ECHO:
            fprintf(fp, "ID:%d   Seq:%d  ", p->icmph->ih_id, p->icmph->ih_seq);
            fwrite("ECHO\n", 4, 1, fp);
            break;
        case ICMP_TIME_EXCEEDED:
            fwrite("TTL EXCEEDED", 12, 1, fp);
            break;
        case ICMP_PARAMETERPROB:
            fwrite("PARAMETER PROBLEM", 17, 1, fp);
            break;
        case ICMP_TIMESTAMP:
            fwrite("TIMESTAMP REQUEST", 17, 1, fp);
            break;
        case ICMP_TIMESTAMPREPLY:
            fwrite("TIMESTAMP REPLY", 15, 1, fp);
            break;
        case ICMP_INFO_REQUEST:
            fwrite("INFO REQUEST", 12, 1, fp);
            break;
        case ICMP_INFO_REPLY:
            fwrite("INFO REPLY", 10, 1, fp);
            break;
        case ICMP_ADDRESS:
            fwrite("ADDRESS REQUEST", 15, 1, fp);
            break;
        case ICMP_ADDRESSREPLY:
            fwrite("ADDRESS REPLY", 13, 1, fp);
            break;
        default:
            fwrite("UNKNOWN", 7, 1, fp);
            break;
    }
    putc('\n', fp);
}

void CLog::PrintIpOptions(FILE *fp, Packet *p)
{
	int i;
    int j;
	int lastopt = IPOPT_NOP;
    if (!p->ip_option_count)
        return;
    fwrite("IP Options => ", 15, 1, fp);
    for (i = 0; i < p->ip_option_count; i++){
        if(lastopt == p->ip_options[i].code && lastopt != IPOPT_NOP){
            return;
        }
        switch (p->ip_options[i].code){
            case IPOPT_RR:
                fwrite("RR ", 3, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            case IPOPT_EOL:
                fwrite("EOL ", 4, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            case IPOPT_NOP:
                fwrite("NOP ", 5, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            case IPOPT_TS:
                fwrite("TS ", 3, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            case IPOPT_SECURITY:
                fwrite("SEC ", 4, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            case IPOPT_LSRR:
            case IPOPT_LSRR_E:
                fwrite("LSRR ", 5, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            case IPOPT_SATID:
                fwrite("SID ", 4, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            case IPOPT_SSRR:
                fwrite("SSRR ", 5, 1, fp);
				lastopt = p->ip_options[i].code;
                break;
            default:
                fprintf(fp, "Opt %d: ", p->ip_options[i].code);
				lastopt = p->ip_options[i].code;
                if (p->ip_options[i].len){
                    for (j = 0; j < p->ip_options[i].len-2; j+=2){
                        fprintf(fp, "%02X%02X ", p->ip_options[i].data[j], p->ip_options[i].data[j+1]);
                    }
                }
                break;
        }
    }
    fwrite("\n", 1, 1, fp);
}

void CLog::PrintTcpOptions(FILE *fp, Packet *p)
{
	int i;
    int j;
    u_char temp[5];
	int lastopt = TCPOPT_NOP;
    fwrite("TCP Options => ", 15, 1, fp); 
    for (i = 0; i < p->tcp_option_count; i++)
    {
        if(lastopt == p->tcp_options[i].code && lastopt != TCPOPT_NOP){
            return;
        }
        switch (p->tcp_options[i].code){
            case TCPOPT_MAXSEG:
                bzero((char *)temp, 5);
                fwrite("MSS: ", 5, 1, fp);
				u_char *tdata;
				tdata = (p->tcp_options[i].data);
                _mbsncpy(temp,tdata, 2); 
                fprintf(fp, "%u ", EXTRACT_16BITS(temp));
                break;
            case TCPOPT_EOL:
                fwrite("EOL ", 4, 1, fp);
                break;
            case TCPOPT_NOP:
                fwrite("NOP ", 4, 1, fp);
                break;
            case TCPOPT_WSCALE:
                fprintf(fp, "WS: %u ", p->tcp_options[i].data[0]);
                break;
            case TCPOPT_SACK:
                bzero((char *)temp, 5);
                memcpy(temp, p->tcp_options[i].data, 2);
                fprintf(fp, "Sack: %u@", EXTRACT_16BITS(temp));
                bzero((char *)temp, 5);
                memcpy(temp, (p->tcp_options[i].data)+2, 2);
                fprintf(fp, "%u ", EXTRACT_16BITS(temp));
                break;
            case TCPOPT_SACKOK:
                fwrite("SackOK ", 7, 1, fp);
                break;
            case TCPOPT_ECHO:
                bzero((char *)temp, 5);
                memcpy(temp, p->tcp_options[i].data, 4);
                fprintf(fp,"Echo: %lu ", EXTRACT_32BITS(temp));
                break;
            case TCPOPT_ECHOREPLY:
                bzero((char *)temp, 5);
                memcpy(temp, p->tcp_options[i].data, 4);
                fprintf(fp, "Echo Rep: %lu ", EXTRACT_32BITS(temp));
                break;
            case TCPOPT_TIMESTAMP:
                bzero((char *)temp, 5);
                memcpy(temp, p->tcp_options[i].data, 4);
                fprintf(fp, "TS: %lu ", EXTRACT_32BITS(temp));
                bzero((char *)temp, 5);
                memcpy(temp, (p->tcp_options[i].data)+4, 4);
                fprintf(fp, "%lu ", EXTRACT_32BITS(temp));
                break;
            case TCPOPT_CC:
                bzero((char *)temp, 5);
                memcpy(temp, p->tcp_options[i].data, 4);
                fprintf(fp, "CC %lu ", EXTRACT_32BITS(temp));
                break;
            case TCPOPT_CCNEW:
                bzero((char *)temp, 5);
                memcpy(temp, p->tcp_options[i].data, 4);
                fprintf(fp, "CCNEW: %lu ", EXTRACT_32BITS(temp));
                break;
            case TCPOPT_CCECHO:
                bzero((char *)temp, 5);
                memcpy(temp, p->tcp_options[i].data, 4);
                fprintf(fp, "CCECHO: %lu ", EXTRACT_32BITS(temp));
                break;
            default:
                if (p->tcp_options[i].len > 2){
                    fprintf(fp, "Opt %d (%d): ", p->tcp_options[i].code, 
                            p->tcp_options[i].len);
                    for (j = 0; j < p->tcp_options[i].len-2; j+=2){
                        fprintf(fp, "%02X%02X ", p->tcp_options[i].data[j], p->tcp_options[i].data[j+1]);
                    }
                }else{
                    fprintf(fp, "Opt %d ", p->tcp_options[i].code);
                }
                break;
        }
		lastopt = p->tcp_options[i].code;
    }

    fwrite("\n", 1, 1, fp);
}

void CLog::LogBin(Packet *p, char *msg, void *arg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
	pcap_dump((u_char *)ptrMainApp->dumpd,p->pkth,p->pkt);
	FILE* ptrfile;
	ptrfile = (FILE*)ptrMainApp->dumpd;
	_commit(_fileno(ptrfile));
}

char* CLog::IcmpFileName(Packet *p)
{
	if(p->icmph == NULL)
        return "BAD_PACKET";
    switch (p->icmph->type){
        case ICMP_ECHOREPLY:
            return "ECHO_REPLY";
        case ICMP_DEST_UNREACH:
            switch (p->icmph->code){
                case ICMP_NET_UNREACH:
                    return "NET_UNRCH";
                case ICMP_HOST_UNREACH:
                    return "HST_UNRCH";
                case ICMP_PROT_UNREACH:
                    return "PROTO_UNRCH";
                case ICMP_PORT_UNREACH:
                    return "PORT_UNRCH";
                case ICMP_FRAG_NEEDED:
                    return "UNRCH_FRAG_NEEDED";
                case ICMP_SR_FAILED:
                    return "UNRCH_SOURCE_ROUTE_FAILED";
                case ICMP_NET_UNKNOWN:
                    return "UNRCH_NETWORK_UNKNOWN";
                case ICMP_HOST_UNKNOWN:
                    return "UNRCH_HOST_UNKNOWN";
                case ICMP_HOST_ISOLATED:
                    return "UNRCH_HOST_ISOLATED";
                case ICMP_NET_ANO:
                    return "UNRCH_NET_ANO";
                case ICMP_HOST_ANO:
                    return "UNRCH_HOST_ANO";
                case ICMP_NET_UNR_TOS:
                    return "UNRCH_NET_UNR_TOS";
                case ICMP_HOST_UNR_TOS:
                    return "UNRCH_HOST_UNR_TOS";
                case ICMP_PKT_FILTERED:
                    return "UNRCH_PACKET_FILT";
                case ICMP_PREC_VIOLATION:
                    return "UNRCH_PREC_VIOL";
                case ICMP_PREC_CUTOFF:
                    return "UNRCH_PREC_CUTOFF";
                default:
                    return "UNKNOWN";
            }
        case ICMP_SOURCE_QUENCH:
            return "SRC_QUENCH";
        case ICMP_REDIRECT:
            return "REDIRECT";
        case ICMP_ECHO:
            return "ECHO";
        case ICMP_TIME_EXCEEDED:
            return "TTL_EXCEED";
        case ICMP_PARAMETERPROB:
            return "PARAM_PROB";
        case ICMP_TIMESTAMP:
            return "TIMESTAMP";
        case ICMP_TIMESTAMPREPLY:
            return "TIMESTAMP_RPL";
        case ICMP_INFO_REQUEST:
            return "INFO_REQ";
        case ICMP_INFO_REPLY:
            return "INFO_RPL";
        case ICMP_ADDRESS:
            return "ADDR";
        case ICMP_ADDRESSREPLY:
            return "ADDR_RPL";
        default:
            return "UNKNOWN";
    }
}

void CLog::InitBinLogFile()
{
	time_t curr_time;      
    struct tm *loc_time;   
    char timebuf[10];
    char logdir[STD_BUF];
	CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
    bzero((char *)timebuf, 10);
    curr_time = time(NULL);
    loc_time = localtime(&curr_time);
    strftime(timebuf,91,"%m%d@%H%M",loc_time);
    bzero((char *)logdir, STD_BUF);
    if (strlen(ptrMainApp->pv.log_dir)+strlen(timebuf)+12 < STD_BUF)
        sprintf(logdir,"%s/ifs-%s.log", ptrMainApp->pv.log_dir, timebuf);

	ptrMainApp->dumpd = NULL;
    if ((ptrMainApp->dumpd=pcap_dump_open(ptrMainApp->pd,logdir)) == NULL)
    {
//        FatalError("ERROR => InitBinFrag() pcap_dump_open: %s\n", strerror(errno));
    }
	ptrMainApp->pv.log_file_flag = FILE_OPEN;
    return;
}

void CLog::LogArpPkt(Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
	if (!ptrMainApp->pv.nolog_flag){
        OpenLogFile(ARP, p);
        PrintArpHeader(ptrMainApp->log_ptr, p);
        fclose(ptrMainApp->log_ptr);
    }
}

void CLog::NoLog(Packet *p, char *msg, void *arg)
{
	return;
}


void CLog::LogPkt(Packet *p, char *msg, void *arg)
{
    CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
	if(p->iph != NULL){
        OpenLogFile(0, p);
        if (msg != NULL){
            fwrite("[**] ", 5, 1, ptrMainApp->log_ptr);
            fwrite(msg, strlen(msg), 1, ptrMainApp->log_ptr);
            fwrite(" [**]\n", 6, 1, ptrMainApp->log_ptr);
        }
        PrintIPPkt(ptrMainApp->log_ptr, p->iph->ip_proto, p);
        fclose(ptrMainApp->log_ptr);
    }else{
        OpenLogFile(NON_IP, p);
        PrintNetData(ptrMainApp->log_ptr, p->pkt, p->pkth->caplen);
        fclose(ptrMainApp->log_ptr);
    }
}

void CLog::NoAlert(Packet *p, char *msg, void *arg)
{
	return;
}

void CLog::PrintNetData(FILE *fp, u_char *start, const u_int len)
{
	u_char *end;            
    int i;                  
    int j;                  
    int dbuf_size;          
    int done;               
    u_char *data;           
    char *frame_ptr;        
    char *d_ptr;            
    char *c_ptr;            
    char conv[] = "0123456789ABCDEF"; 
    CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
    done = 0;
    if (start == NULL){
        return;
    }
    if (!len){
        fputc('\n', fp);
        return;
    }
    if (dump_ready){
        fwrite(ptrMainApp->data_dump_buffer, dump_size, 1, fp);
        fflush(fp);
        return;
    }
    end = start + (len-1); 
    if (len > ETHERNET_MTU){
        dbuf_size = 66 + 67;
        end = start + 32;
    }else{
        dbuf_size = ((len/16) * 66) + 67;
    }
    ptrMainApp->data_dump_buffer = (char *) malloc(dbuf_size);   
    if (ptrMainApp->data_dump_buffer == NULL){
        perror("PrintNetData()");
        ptrMainApp->CleanExit(SIGQUIT);
    }
    memset(ptrMainApp->data_dump_buffer, 0x20, dbuf_size);
    data = start;
    frame_ptr = ptrMainApp->data_dump_buffer;
    while (!done){
        i = 0;
        j = 0;
        d_ptr = frame_ptr;
        c_ptr = (frame_ptr + C_OFFSET);
        for (i=0; i<16; i++){
            *d_ptr = conv[((*data&0xFF) >> 4)];
            d_ptr++;
            *d_ptr = conv[((*data&0xFF)&0x0F)];
            d_ptr++;
            *d_ptr = 0x20;
            d_ptr++;
            if (*data > 0x1F && *data < 0x7F)
                *c_ptr = (*data&0xFF);
            else
                *c_ptr = 0x2E;
            c_ptr++;
            if (data < end)
                data++;
            else{
                done = 1;
                dump_ready = 1;
                *c_ptr='\n';
                c_ptr++;
                *c_ptr='\n';
                c_ptr++;
                *c_ptr = 0;
                dump_size = (int) (c_ptr - ptrMainApp->data_dump_buffer);
                fwrite(ptrMainApp->data_dump_buffer, dump_size, 1, fp);
                return;
            }
        }

        *c_ptr = '\n';
        frame_ptr += FRAME_SIZE;
    }
}

void CLog::PrintCharData(FILE *fp, u_char *data, int data_len)
{
	int bytes_processed;
    int linecount = 0;
    u_char *index;
    char *ddb_ptr;
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
    if (data == NULL){
        return;
    }
    if (dump_ready){
        fwrite(ptrMainApp->data_dump_buffer, dump_size, 1, fp);
        fflush(fp);
        return;
    }
    bytes_processed = data_len;
    index = data;
    ptrMainApp->data_dump_buffer = (char *) calloc(data_len + (data_len >> 6) + 2, sizeof(char));
    ddb_ptr = ptrMainApp->data_dump_buffer;
    while (bytes_processed){
        if (*index > 0x1F && *index < 0x7F){
            *ddb_ptr = *index;
        }else{
            *ddb_ptr = '.';      
        }
        if (++linecount == 64){
            ddb_ptr++;
            *ddb_ptr = '\n';
            linecount = 0;
        }
        ddb_ptr++;
        index++;
        bytes_processed--;
    }
    ddb_ptr++;
    *ddb_ptr = '\n';
    ddb_ptr++;
    dump_ready = 1;
    dump_size = (int) (ddb_ptr - ptrMainApp->data_dump_buffer);   
    fwrite(ptrMainApp->data_dump_buffer, dump_size, 1, fp);
}

int CLog::OpenLogFile(int mode, Packet *p)
{
    CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	char log_path[STD_BUF]; 
    char log_file[STD_BUF]; 
    char proto[5];          
    bzero((char *)log_path, STD_BUF);
    bzero((char *)log_file, STD_BUF);
    bzero((char *)proto, 5);
    if(mode == NON_IP){
        if(p->eh != NULL){
            sprintf(log_file, "%s/NET_PROTO%d", ptrMainApp->pv.log_dir, ntohs(p->eh->ether_type));
        }else if(p->trhllc != NULL){
            sprintf(log_file, "%s/NET_PROTO%d", ptrMainApp->pv.log_dir, ntohs(p->trhllc->ethertype));
        }else if(p->fddiiparp != NULL){
            sprintf(log_file, "%s/NET_PROTO%d", ptrMainApp->pv.log_dir, ntohs(p->fddiiparp->ethertype));
        }
        if ((ptrMainApp->log_ptr = fopen(log_file, "a")) == NULL){
//            FatalError("ERROR: OpenLogFile() => fopen(%s) log file: %s\n",
//                       log_file, strerror(errno));
        }
        return 0;
    }
    if (mode == ARP){
        sprintf(log_file, "%s/ARP", ptrMainApp->pv.log_dir);
        if ((ptrMainApp->log_ptr = fopen(log_file, "a")) == NULL){
//            FatalError("ERROR: OpenLogFile() => fopen(%s) log file: %s\n",
//                       log_file, strerror(errno));
        }
        return 0;
    }
    if (ptrMainApp->otn_tmp != NULL){
        if (ptrMainApp->otn_tmp->logto != NULL){
            sprintf(log_file, "%s/%s", ptrMainApp->pv.log_dir, ptrMainApp->otn_tmp->logto);
            if ((ptrMainApp->log_ptr = fopen(log_file, "a")) == NULL){
//                FatalError("ERROR: OpenLogFile() => fopen(%s) log file: %s\n", log_file, strerror(errno));
            }
            return 0;
        }
    }
    if ((p->iph->ip_dst.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
        if ((p->iph->ip_src.s_addr & ptrMainApp->pv.netmask) != ptrMainApp->pv.homenet){
            sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_src));
        }else{if ( p->sp >= p->dp ){
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_src));
            }else{
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_dst));
            }
        }
    }else{if ((p->iph->ip_src.s_addr & ptrMainApp->pv.netmask) == ptrMainApp->pv.homenet){
            sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_dst));
        }else{if (p->sp >= p->dp){
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_src));
            }else{
                sprintf(log_path, "%s/%s", ptrMainApp->pv.log_dir, inet_ntoa(p->iph->ip_dst));
            }
        }
    }
    if (mkdir(log_path,S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH)){
    }
    if(!p->frag_flag){
        switch(p->iph->ip_proto){
           case IPPROTO_TCP:
           case IPPROTO_UDP:
               if (p->sp >= p->dp){
#ifdef WIN32 
	               sprintf(log_file, "%s/%s_%d-%d.ids", log_path, 
#else
                   sprintf(log_file, "%s/%s:%d-%d", log_path, 
#endif
                           ptrMainApp->protocol_names[p->iph->ip_proto], p->sp, p->dp);

               }else{
#ifdef WIN32 
				   sprintf(log_file, "%s/%s_%d-%d.ids", log_path,
#else
                   sprintf(log_file, "%s/%s:%d-%d", log_path,
#endif
                           ptrMainApp->protocol_names[p->iph->ip_proto], p->dp, p->sp);

               }
                break;

          case IPPROTO_ICMP:
#ifdef WIN32
               sprintf(log_file, "%s/%s_%s.ids", log_path, "ICMP", IcmpFileName(p));
#else
               sprintf(log_file, "%s/%s_%s", log_path, "ICMP", IcmpFileName(p));
#endif
               break;
          default:
#ifdef WIN32
			   sprintf(log_file, "%s/XPORT_PROTO%d.ids", log_path, p->iph->ip_proto);
#else
               sprintf(log_file, "%s/XPORT_PROTO%d", log_path, p->iph->ip_proto);
#endif
               break;
       }
    }else{ 
#ifdef WIN32
	   sprintf(log_file, "%s/IP_FRAG.ids", log_path);
#else
       sprintf(log_file, "%s/IP_FRAG", log_path);
#endif
    }
    if ((ptrMainApp->log_ptr = fopen(log_file, "a")) == NULL){
//        FatalError("ERROR: OpenLogFile() => fopen(%s) log file: %s\n",
//                   log_file, strerror(errno));
    }
    return 0;
}

void CLog::PrintArpHeader(FILE *fp, Packet *p)
{
	struct in_addr *ip_addr;
    char timestamp[23];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    if(p->ah == NULL){
//        ErrorMessage("[!] WARNING: PrintArpHeader got NULL p->ah pointer\n");
        return;
    }
    bzero((char *)timestamp, 23);
    ptrMainApp->Ts_Print((struct timeval *)&p->pkth->ts, timestamp);
    fwrite(timestamp, 22, 1, fp);
    if (ntohs(p->ah->ea_hdr.ar_pro) != ETHERNET_TYPE_IP){
        fprintf(fp, "ARP #%d for protocol #%.4X (%d) hardware #%d (%d)\n", 
                ntohs(p->ah->ea_hdr.ar_op), ntohs(p->ah->ea_hdr.ar_pro), 
                p->ah->ea_hdr.ar_pln, ntohs(p->ah->ea_hdr.ar_hrd), 
                p->ah->ea_hdr.ar_hln);
        return;
    }
    switch (ntohs(p->ah->ea_hdr.ar_op)){
        case ARPOP_REQUEST: 
            ip_addr = (struct in_addr *)p->ah->arp_tpa;
            fprintf(fp, "ARP who-has %s", inet_ntoa(*ip_addr));
            if (memcmp((char *)ezero, (char *)p->ah->arp_tha, 6) != 0){
                fprintf(fp, " (%X:%X:%X:%X:%X:%X)", p->ah->arp_tha[0], 
                        p->ah->arp_tha[1], p->ah->arp_tha[2], p->ah->arp_tha[3],
                        p->ah->arp_tha[4], p->ah->arp_tha[5]); 
            }
            ip_addr = (struct in_addr *)p->ah->arp_spa;
            fprintf(fp, " tell %s", inet_ntoa(*ip_addr));    
            if (memcmp((char *)p->eh->ether_src, (char *)p->ah->arp_sha, 6) != 0){
                fprintf(fp, " (%X:%X:%X:%X:%X:%X)", p->ah->arp_sha[0], 
                        p->ah->arp_sha[1], p->ah->arp_sha[2], p->ah->arp_sha[3],
                        p->ah->arp_sha[4], p->ah->arp_sha[5]); 
            }

            break;

        case ARPOP_REPLY: 
            ip_addr = (struct in_addr *)p->ah->arp_spa;
            fprintf(fp, "ARP reply %s", inet_ntoa(*ip_addr));
            if (memcmp((char *)p->eh->ether_src, (char *)p->ah->arp_sha, 6) != 0){
                fprintf(fp, " (%X:%X:%X:%X:%X:%X)", p->ah->arp_sha[0], 
                        p->ah->arp_sha[1], p->ah->arp_sha[2], p->ah->arp_sha[3],
                        p->ah->arp_sha[4], p->ah->arp_sha[5]); 
            }
            fprintf(fp, " is-at %X:%X:%X:%X:%X:%X", p->ah->arp_sha[0], 
                    p->ah->arp_sha[1], p->ah->arp_sha[2], p->ah->arp_sha[3],
                    p->ah->arp_sha[4], p->ah->arp_sha[5] );    

            if (memcmp((char *)p->eh->ether_dst, (char *)p->ah->arp_tha, 6) != 0){
                fprintf(fp, " (%X:%X:%X:%X:%X:%X)", p->ah->arp_tha[0], 
                        p->ah->arp_tha[1], p->ah->arp_tha[2], p->ah->arp_tha[3],
                        p->ah->arp_tha[4], p->ah->arp_tha[5]); 
            }

            break;

        case ARPOP_RREQUEST:
            fprintf(fp, "RARP who-is %X:%X:%X:%X:%X:%X tell %X:%X:%X:%X:%X:%X", 
                    p->ah->arp_tha[0], p->ah->arp_tha[1], p->ah->arp_tha[2], 
                    p->ah->arp_tha[3], p->ah->arp_tha[4], p->ah->arp_tha[5], 
                    p->ah->arp_sha[0], p->ah->arp_sha[1], p->ah->arp_sha[2], 
                    p->ah->arp_sha[3], p->ah->arp_sha[4], p->ah->arp_sha[5]);

            break;

        case ARPOP_RREPLY:
            ip_addr = (struct in_addr *)p->ah->arp_tpa;
            fprintf(fp, "RARP reply %X:%X:%X:%X:%X:%X at %s", 
                    p->ah->arp_tha[0], p->ah->arp_tha[1], p->ah->arp_tha[2], 
                    p->ah->arp_tha[3], p->ah->arp_tha[4], p->ah->arp_tha[5],
                    inet_ntoa(*ip_addr));

            break;

        default:
            fprintf(fp, "Unknown operation: %d", ntohs(p->ah->ea_hdr.ar_op));
            break;
    }

    fprintf(fp, "\n\n");

}

FILE* CLog::OpenAlertFile(char *filearg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	
	char filename[STD_BUF];
    FILE *file;
 
    if(filearg == NULL) 
    {
        if(!ptrMainApp->pv.daemon_flag)
        {
#ifdef WIN32
		sprintf(filename, "%s/alert.ids", ptrMainApp->pv.log_dir);
#else
        sprintf(filename, "%s/alert", ptrMainApp->pv.log_dir);
#endif
        }
        else
        {
            sprintf(filename, "%s/%s", ptrMainApp->pv.log_dir, DEFAULT_DAEMON_ALERT_FILE);
        }
    } 
    else 
    {
        strncpy(filename, filearg, STD_BUF-1);
    }
    

	file = fopen(filename, "a");
#ifdef WIN32
	setvbuf(file,(char *)NULL,_IONBF,(size_t)0);
#else
    setvbuf(file,(char *)NULL,_IOLBF,(size_t)0);
#endif
	ptrMainApp->pv.alert_file_flag = FILE_OPEN;
    return file;
}

void CLog::AlertFull(Packet *p, char *msg, FILE *file)
{
	char timestamp[23];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;


    if (msg != NULL)
    {
        fwrite("[**] ", 5, 1, file);
        fwrite(msg, strlen(msg), 1, file);
        fwrite(" [**]\n", 6, 1, file);
    }
    else
    {
        fwrite("[**] IFS Alert! [**]", 22, 1, file);
    }
    bzero((char *)timestamp, 23);
    ptrMainApp->Ts_Print((struct timeval*)(p==NULL?NULL:&p->pkth->ts), timestamp);
    fwrite(timestamp, 22, 1, file);
    if (p){
        if (ptrMainApp->pv.show2hdr_flag)
        {
            Print2ndHeader(file, p);
        }
        PrintIPHeader(file, p);
        if (!p->frag_flag)
        {
            switch (p->iph->ip_proto)
            {
                case IPPROTO_TCP:
                    PrintTCPHeader(file, p);
                    break;

                case IPPROTO_UDP:
                    PrintUDPHeader(file, p);
                    break;

                case IPPROTO_ICMP:
                    PrintICMPHeader(file, p);
                    break;

                default: break;
            }
        }
    }
    fputc('\n', file);
	AddToCriticalClients(p,msg,timestamp,p->iph->ip_proto);	
    return;
}

void CLog::OpenAlertSock()
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	char *srv=UNSOCK_FILE;
    if (access(srv,W_OK))
//        ErrorMessage("WARNING: %s file doesn't exist or isn't writable!\n",srv);

    bzero((char *)&ptrMainApp->alertaddr,sizeof(ptrMainApp->alertaddr));
    bcopy((const void *)srv,(void *)ptrMainApp->alertaddr.sun_path,strlen(srv)); /* we trust what we define */
    ptrMainApp->alertaddr.sun_family=AF_UNIX;
    if ((ptrMainApp->alertsd=socket(AF_UNIX,SOCK_DGRAM,0))<0)
    {
//        FatalError("socket() call failed: %s", strerror(errno));
    }
}

void CLog::UnixSockAlert(Packet *p, char *msg, void *arg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	
	static Alertpkt alertpkt;
	bzero((char *)&alertpkt,sizeof(alertpkt));
    if (p){
        bcopy((const void *)p->pkth,(void *)&alertpkt.pkth,sizeof(struct pcap_pkthdr));
        bcopy((const void *)p->pkt,alertpkt.pkt,
             alertpkt.pkth.caplen > SNAPLEN? SNAPLEN : alertpkt.pkth.caplen);
    }
    else
        alertpkt.val = (alertpkt.val| NOPACKET_STRUCT);
    bcopy((const void *)msg,(void *)alertpkt.alertmsg,strlen(msg) > 255 ? 256: strlen(msg)+1);
    if (!(alertpkt.val & NOPACKET_STRUCT))
    {
        alertpkt.dlthdr=(char *)p->eh-(char *)p->pkt;
        alertpkt.nethdr=(char *)p->iph-(char *)p->pkt;

        switch (p->iph->ip_proto)
        {
            case IPPROTO_TCP:
                alertpkt.transhdr=(char *)p->tcph-(char *)p->pkt;
                break;

            case IPPROTO_UDP:
                alertpkt.transhdr=(char *)p->udph-(char *)p->pkt;
                break;

            case IPPROTO_ICMP:
                alertpkt.transhdr=(char *)p->icmph-(char *)p->pkt;
                break;

            default:
                alertpkt.val = (alertpkt.val| NO_TRANSHDR);
        }
        alertpkt.data=p->data - p->pkt;
    }


    if (sendto(ptrMainApp->alertsd,(const char *)&alertpkt,sizeof(Alertpkt),
               0,(struct sockaddr *)&ptrMainApp->alertaddr,sizeof(ptrMainApp->alertaddr))==-1)
    {
    }

    return;
}

void CLog::SyslogAlert(Packet *p, char *msg, void *arg)
{
	char sip[16];
    char dip[16];
	CSysLog *objSyslog;
	objSyslog = new CSysLog;
    if (p){
        strncpy(sip, inet_ntoa(p->iph->ip_src), 16);
        strncpy(dip, inet_ntoa(p->iph->ip_dst), 16);
        if ((p->iph->ip_proto != IPPROTO_TCP && p->iph->ip_proto != IPPROTO_UDP) || p->frag_flag){
            if (msg != NULL){
                objSyslog->SysLog(LOG_AUTHPRIV|LOG_ALERT, "%s: %s -> %s", msg,sip, dip);
            }else{
                objSyslog->SysLog(LOG_AUTHPRIV|LOG_ALERT, "ALERT: %s -> %s",sip, dip);
            }
        }else{if (msg != NULL){
                objSyslog->SysLog(LOG_AUTHPRIV|LOG_ALERT, "%s: %s:%i -> %s:%i", msg,sip, p->sp, dip, p->dp);
            }else{
                objSyslog->SysLog(LOG_AUTHPRIV|LOG_ALERT, "ALERT: %s:%i -> %s:%i",sip, p->sp, dip, p->dp);
            }
        }
    }else 
		objSyslog->SysLog(LOG_AUTHPRIV|LOG_ALERT,"%s",msg==NULL?"ALERT!":msg);
    return;
}

void CLog::FullAlert(Packet *p, char *msg, void *arg)
{
	AlertFull(p, msg, ptrMainApp->alert); return; 
}

void CLog::FastAlert(Packet *p, char *msg, void *arg)
{
	AlertFast(p, msg, ptrMainApp->alert); return;
}


void CLog::AlertFast(Packet *p, char *msg, FILE *file)
{
	char timestamp[23];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    bzero((char *)timestamp, 23);
    ptrMainApp->Ts_Print((struct timeval*)(p==NULL?NULL:&p->pkth->ts), timestamp);
    fwrite(timestamp, 22, 1, file);
    if (msg != NULL){
        fwrite(" [**] ", 6, 1, file);
        fwrite(msg, strlen(msg), 1, file);
        fwrite(" [**] ", 6, 1, file);
    }
    if (p){
        if (p->frag_flag){
            fputs(inet_ntoa(p->iph->ip_src), file);
            fwrite(" -> ", 4, 1, file);
            fputs(inet_ntoa(p->iph->ip_dst), file);
        }else{
            switch (p->iph->ip_proto){
		case IPPROTO_UDP:
		case IPPROTO_TCP:
                    fputs(inet_ntoa(p->iph->ip_src), file);
                    fprintf(file, ":%d -> ", p->sp);
                    fputs(inet_ntoa(p->iph->ip_dst), file);
                    fprintf(file, ":%d", p->dp);
		    break;
		case IPPROTO_ICMP:
		default:
                    fputs(inet_ntoa(p->iph->ip_src), file);
                    fwrite(" -> ", 4, 1, file);
                    fputs(inet_ntoa(p->iph->ip_dst), file);
            }
        }
    } 
    fputc('\n', file);
    return;
}

void CLog::SmbAlert(Packet *p, char *msg, void *args)
{
	char command_line[2048];
    FILE *output;
    FILE *workstations;
    char *tempmsg;
    char workfile[STD_BUF];
    char tempwork[STD_BUF];
    char timestamp[23];
    int msg_str_size;
	int str_size;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    bzero((char *)timestamp, 23);
    ptrMainApp->Ts_Print((struct timeval *)(p==NULL?NULL:&p->pkth->ts), timestamp);
    sprintf(workfile, "%s", ptrMainApp->pv.smbmsg_dir);
    msg_str_size = strlen(msg) + 32 + 10 + 150;

    if((tempmsg = (char *) calloc(msg_str_size, sizeof(char))) == NULL){
//        FatalError("[!] ERROR: SmbAlert() unable to allocate space for tempmsg: %s\n", strerror(errno));
    }
    if((workstations = fopen(workfile,"r")) != NULL){
        bzero((char *)workfile, STD_BUF);
        if(p != NULL){
            if(p->frag_flag || (p->iph->ip_proto != IPPROTO_TCP && p->iph->ip_proto != IPPROTO_UDP)){
                sprintf(tempmsg, "IFS ALERT - Possible Network Attack or Probe:\n [**] %s [**]\n%s %s->", msg, timestamp, inet_ntoa(p->iph->ip_src));
                str_size = strlen(tempmsg);
                sprintf(tempmsg+str_size, "%s", inet_ntoa(p->iph->ip_dst));
            }else{
                sprintf(tempmsg, "IFS ALERT - Possible Network Attack or Probe:\n [**] %s [**]\n%s:%d ->  ", msg, timestamp, inet_ntoa(p->iph->ip_src), p->sp);
                str_size = strlen(tempmsg);
                sprintf(tempmsg+str_size, "%s:%d", inet_ntoa(p->iph->ip_dst), p->dp);
            }
        }else{
            sprintf(tempmsg, "IFS ALERT - Possible Network Attack or Probe:\n [**] %s [**]\n", msg);
        }
        bzero((char *)tempwork, STD_BUF);
        bzero((char *)command_line, 2048);
        while ((fgets(tempwork, STD_BUF-1, workstations)) != NULL){
            if (tempwork[0] != 0x0A){
                ptrMainApp->Strip(tempwork);
                if(strlen(tempmsg) + strlen(tempwork) + 50 < 2048){
                   sprintf(command_line, "echo \"%s\" | smbclient -U IFS -M %s", tempmsg, tempwork);
                   output = _popen(command_line,"r");
                   _pclose(output);
                }else{
//                   ErrorMessage("[!] WARNING: Unable to send alert to %s, command buffer size exceeded!\n", tempwork);
                }
                bzero((char *)tempwork, STD_BUF);
                bzero((char *)command_line, 2048);
            }
        }
        fclose(workstations);
    }

    free(tempmsg);
}

void CLog::CreatePidFile(char *intf)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	FILE *pid_file;
#ifdef WIN32
	sprintf(ptrMainApp->pv.pid_filename, "%sifs.pid", ptrMainApp->pv.pid_path);
#else
    sprintf(ptrMainApp->pv.pid_filename, "%sifs_%s.pid", ptrMainApp->pv.pid_path, intf);
#endif
    
	char *filet;
	filet = new char[STD_BUF];
	filet = ptrMainApp->pv.pid_filename;
	pid_file = fopen(filet, "w");
    if (pid_file)
    {
        fprintf(pid_file, "%d\n", (int) getpid());
        fclose(pid_file);
    }
    else
    {
		ptrMainApp->pv.pid_filename[0] = 0;
    }
}

void CLog::AddToCriticalClients(Packet *p, char *msg, char *TStamp, int protocol)
{
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	CSingleLock objCrit(&ptrMainApp->m_CriticalSection);
	int ClientCount = NULL;
	int NullCount = NULL;
	char *ipadd;
	CTrafficControl *ptrTraffic;
	ptrTraffic = new CTrafficControl;
	
	CriticalClientList *idx;   
	AlertNode *alert_idx;
	AlertNode *next_idx;

    switch(protocol)
	{
		case IPPROTO_TCP:
			idx = ptrMainApp->accesslist->TCPList;
			break;
		case IPPROTO_UDP:
			idx = ptrMainApp->accesslist->UDPList;
			break;
		case IPPROTO_ICMP:
			idx = ptrMainApp->accesslist->ICMPList;
			break;
		default:
			return;
	}

	objCrit.Lock();
	alert_idx = idx->ClientQueue[FIRST_CLIENT];

	if(alert_idx == NULL)
	{
		 idx->ClientQueue[FIRST_CLIENT] = (AlertNode *)calloc(sizeof(AlertNode),sizeof(char));
		 alert_idx = idx->ClientQueue[FIRST_CLIENT];
			alert_idx->AlertType = msg; 
			alert_idx->Pkt = (Packet*)calloc(sizeof(Packet),sizeof(char));
			alert_idx->Pkt->eh = (EtherHdr *)calloc(sizeof(EtherHdr),sizeof(char));
			alert_idx->Pkt->iph = (IPHdr *)calloc(sizeof(IPHdr),sizeof(char));
			alert_idx->TimeStamp = new char[23];
			for(int loop = 0;loop <= 6; loop++)
			{
				alert_idx->Pkt->eh->ether_dst[loop] = p->eh->ether_dst[loop];
				
				alert_idx->Pkt->eh->ether_src[loop] = p->eh->ether_src[loop];
			}
			alert_idx->Pkt->eh->ether_type = p->eh->ether_type;
			alert_idx->Pkt->iph->ip_src = p->iph->ip_src;
			alert_idx->Pkt->iph->ip_dst = p->iph->ip_dst;
			alert_idx->Pkt->iph->ip_proto = p->iph->ip_proto;
			alert_idx->Pkt->dp = p->dp;
			alert_idx->Pkt->sp = p->sp;
			sprintf(alert_idx->TimeStamp,"%s",TStamp);
			alert_idx->Weight = FIRST_ATTEMPT;
			free(ptrTraffic);
			return;
	}
	else
	{
		do
		{
			if (ClientCount == MAX_CLIENTS || idx->ClientQueue[ClientCount] == NULL)
			{
					alert_idx = NULL;
					break;
			}
			else
			{
				if(inet_ntoa(idx->ClientQueue[ClientCount]->Pkt->iph->ip_src) == inet_ntoa(p->iph->ip_src))
				{
					if(strncmp(idx->ClientQueue[ClientCount]->TimeStamp,TStamp,10))
					{
						objCrit.Unlock();
						ptrTraffic->FreeNode(idx->ClientQueue[ClientCount],ClientCount,protocol);
						return;
					}
					alert_idx = NULL; 
					break;
				}
			}
				alert_idx = idx->ClientQueue[ClientCount]; ClientCount++;
				ipadd = inet_ntoa(idx->ClientQueue[ClientCount]->Pkt->iph->ip_src);
		}while(ipadd == inet_ntoa(p->iph->ip_src));
	}
	
    
    if (alert_idx == NULL)
    {
        alert_idx = (AlertNode *) calloc(sizeof(AlertNode), sizeof(char));
		alert_idx->Pkt = (Packet*)calloc(sizeof(Packet),sizeof(char));
		alert_idx->Pkt->eh = (EtherHdr *)calloc(sizeof(EtherHdr),sizeof(char));
		alert_idx->Pkt->iph = (IPHdr *)calloc(sizeof(IPHdr),sizeof(char));
        alert_idx->AlertType = msg; 
		for(int loop = 0;loop <= 6; loop++){
			alert_idx->Pkt->eh->ether_dst[loop] = p->eh->ether_dst[loop];
			alert_idx->Pkt->eh->ether_src[loop] = p->eh->ether_src[loop];
		}
		alert_idx->Pkt->eh->ether_type = p->eh->ether_type;
		alert_idx->Pkt->iph->ip_src = p->iph->ip_src;
		alert_idx->Pkt->iph->ip_dst = p->iph->ip_dst;
		alert_idx->Pkt->iph->ip_proto = p->iph->ip_proto;
		alert_idx->Pkt->dp = p->dp;
		alert_idx->Pkt->sp = p->sp;
		alert_idx->TimeStamp = new char[23];
		sprintf(alert_idx->TimeStamp,"%s",TStamp);
		next_idx = idx->ClientQueue[ClientCount];
		while(next_idx->next != NULL){
			if(next_idx->next != NULL)
				next_idx = next_idx->next;
			else
				break;
		}
		alert_idx->Weight = next_idx->Weight + 1;
		next_idx->next = alert_idx;
    }
	objCrit.Unlock();
	free(ptrTraffic);
}
