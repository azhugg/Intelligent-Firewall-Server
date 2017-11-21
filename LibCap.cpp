// LibCap.cpp: implementation of the CLibCap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "LibCap.h"
#include "pcap.h"
#include "winsock2.h"
#include "Log.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


#ifndef IRIX
#define SNAPLEN      1514
#else
#define SNAPLEN      1500
#endif

#define MIN_SNAPLEN  68
#define PROMISC      1
#define READ_TIMEOUT 500


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CLibCap::CLibCap()
{

}

CLibCap::~CLibCap()
{

}

char* CLibCap::LookupDev(char err[256])
{
	char* devicev;
	devicev = new char[STD_BUF];
	devicev = pcap_lookupdev(err);
	if(devicev == NULL)
		AfxMessageBox(err,MB_OK,0);
	return devicev;
}

char* CLibCap::GetAdapterFromList(char *device, int index)
{
		DWORD dwVersion;
        DWORD dwWindowsMajorVersion;
        char *Adapter95;
        WCHAR *Adapter;
        int i;
        dwVersion = GetVersion();
        dwWindowsMajorVersion =  (DWORD)(LOBYTE(LOWORD(dwVersion)));
        if (dwVersion >= 0x80000000 && dwWindowsMajorVersion >= 4){
                Adapter95 =(char*) device;
                for(i = 0;i < index - 1; i++){
                        while(*Adapter95++ != 0);
                        if(*Adapter95 == 0)
							return NULL;
                }
                return  Adapter95;
        }else{
                Adapter=(WCHAR *)device;
                for(i = 0;i < index - 1; i++){
                        while(*Adapter++ != 0);
                        if(*Adapter == 0)return NULL;
                }
                return  (char*)Adapter;
        }	
}

void CLibCap::SetAdapt(char *ad)
{
	bpf_u_int32 localnet, netmask;    
    struct bpf_program fcode;         
	bpf_u_int32 defaultnet = 0xFFFFFF00;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	char *devi, *adapt, *err;
	int snapval = SNAPLEN;
	err = new char[STD_BUF];
	pcap_t *ptrlive;
	devi = LookupDev(ad);
	adapt = GetAdapterFromList(devi,1);
	if (!ptrMainApp->pv.readmode_flag){
        if (ptrMainApp->pv.pkt_snaplen){
            if (ptrMainApp->pv.pkt_snaplen < MIN_SNAPLEN){
                ptrMainApp->snaplen = MIN_SNAPLEN;
            }else{
                ptrMainApp->snaplen = ptrMainApp->pv.pkt_snaplen;
            }
        }else{
            ptrMainApp->snaplen = SNAPLEN; 
        }
       ptrlive= pcap_open_live(adapt, snapval,ptrMainApp->pv.promisc_flag ? PROMISC : 0, READ_TIMEOUT, err);
	}else{
        ptrlive = pcap_open_offline(adapt, err);
        ptrMainApp->snaplen = pcap_snapshot(ptrlive);
    }
	ptrMainApp->pd = ptrlive;
	int check;
	check =	pcap_lookupnet(pcap_lookupdev(err), &localnet, &netmask,err);
    if (check < 0){
        netmask = htonl(defaultnet);
    }
    if (pcap_compile(ptrlive, &fcode, ptrMainApp->pv.pcap_cmd, 1, netmask) < 0);
    if (pcap_setfilter(ptrlive, &fcode) < 0)
    {
//        FatalError("ERROR: OpenPcap() setfilter: \n\t%s\n", pcap_geterr(pd));
    }
    ptrMainApp->datalink = pcap_datalink(ptrlive);
    if (ptrMainApp->datalink < 0)
    {
//        FatalError("ERROR: OpenPcap() datalink grab: \n\t%s\n", pcap_geterr(pd));
    }
}

void CLibCap::InitProtoNames()
{
    int i;
    struct protoent *pt;
    unsigned char *tmp;
    u_char protoname[11];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    for(i = 0; i < 256; i++){
        pt = getprotobynumber(i);
        if(pt){
            ptrMainApp->protocol_names[i] = strdup(pt->p_name);
            tmp = (unsigned char*)ptrMainApp->protocol_names[i];
            for(tmp = (unsigned char*)ptrMainApp->protocol_names[i]; *tmp != 0; tmp++)
                *tmp = (unsigned char) toupper(*tmp);
        }else{_snprintf((char*)protoname, 10, "PROTO%03d", i);
            ptrMainApp->protocol_names[i] = strdup((char*)protoname);
        }
    }
}

int CLibCap::InitInterface(char *device)
{
	bpf_u_int32 localnet, netmask;    
    struct bpf_program fcode;         
    char errorbuf[PCAP_ERRBUF_SIZE];  
    bpf_u_int32 defaultnet = 0xFFFFFF00;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    if (ptrMainApp->pv.interfaces == NULL){
        ptrMainApp->pv.interfaces = pcap_lookupdev(errorbuf);
        if (ptrMainApp->pv.interfaces == NULL){
			//  FatalError("ERROR: OpenPcap() interface lookup: \n\t%s\n", errorbuf);
        }
    }
    if (pcap_lookupnet(ptrMainApp->pv.interfaces, &localnet, &netmask, errorbuf) < 0){
        netmask = htonl(defaultnet);
    }
    if (pcap_compile(ptrMainApp->pd, &fcode, ptrMainApp->pv.pcap_cmd, 1, netmask) < 0){
			//		msg.FatalErrorMessage(msg.uMsg);
    }
    if (pcap_setfilter(ptrMainApp->pd, &fcode) < 0){
//        FatalError("ERROR: OpenPcap() setfilter: \n\t%s\n", pcap_geterr(pd));
    }
    ptrMainApp->datalink = pcap_datalink(ptrMainApp->pd);
    if (ptrMainApp->datalink < 0)
    {
//        FatalError("ERROR: OpenPcap() datalink grab: \n\t%s\n", pcap_geterr(pd));
    }
    return 0;
}

void CLibCap::CreatePidFile()
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CLog *objLog;
	objLog = new CLog;
	objLog->CreatePidFile(ptrMainApp->pv.interfaces);
}
