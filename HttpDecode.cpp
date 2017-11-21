// HttpDecode.cpp: implementation of the CHttpDecode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "HttpDecode.h"
#include "StringMan.h"
#include "Preprocessor.h"
#include "Rules.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHttpDecode::CHttpDecode()
{

}

CHttpDecode::~CHttpDecode()
{

}

void CHttpDecode::SetPorts(char *portlist)
{
	char **toks;
    int num_toks;
    int num_ports = 0;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CStringMan strMan;
	toks = strMan.mSplit(portlist, " ", 31, &num_toks, '\\');
    for (num_ports = 0; num_ports < num_toks; num_ports++){
        ptrMainApp->HttpDecodePorts.ports[num_ports] = atoi(toks[num_ports]);
    }   
    ptrMainApp->HttpDecodePorts.num_entries = num_ports;
}

void CHttpDecode::HttpDecodeInit(u_char *args)
{
	CRules objRule;
	static void (CManipulate::*ptrfun)(Packet *);
	ptrfun = &CManipulate::PreprocUrlDecode;
	SetPorts((char *)args);
	objRule.AddFuncToPreprocList(ptrfun);
}

void CHttpDecode::PreprocUrlDecode(Packet *p)
{
	char *url;     
    u_char *index; 
    u_char *end;   
    u_short psize; 
    int i;         
	u_char *Index;
	u_char *End;
	url = new char[2048];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CPreprocessor objPre;
    if (!objPre.PacketIsTCP(p)){
		return;
    }

	if (!objPre.IsTcpSessionTraffic(p)){

        return;
    }
    for (i = 0; i < ptrMainApp->HttpDecodePorts.num_entries; i++){
        if (ptrMainApp->HttpDecodePorts.ports[i] == p->dp){
            url = (char *) p->data;
            Index = p->data;
            End =  p->data + p->dsize;
            psize = (u_short) (p->dsize);
            while (Index < End){
                if ((*Index == '%') && (Index < End - 2)){
                    if (isxdigit((int)*(Index+1)) && isxdigit((int)*(Index+2))){
                        *url = (Nibble(*(Index+1)) << 4) | Nibble(*(Index+2));
                        Index += 3; 
                        url++;
                        psize -= 2;
                    }else{
                        *url = *Index;
                        url++;
                        Index++;
                    }        
                }else{
                    *url = *Index;
                    url++;
                    Index++;
                }
            }
            p->dsize = psize;
			return;
        }
    }
}

int CHttpDecode::Nibble(char what)
{
	if (isdigit((int)what)) return what - '0';

    if (isxdigit((int)what)) return toupper((int)what) - 'A' + 10;

    return -1;
}

void CHttpDecode::SetupHttpDecode()
{
	static void (CManipulate::*ptrfun)(u_char *);
	ptrfun = &CManipulate::HttpDecodeInit;
	PreprocessKeywordList *PrepKeywords;
	PrepKeywords = new PreprocessKeywordList;
	PrepKeywords->entry.keyword  = NULL;
	PrepKeywords->next = NULL;
	CPreprocessor objPre;
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	ptrMainApp->PreprocessKeywords = NULL;
	objPre.RegisterPreprocessor("http_decode",ptrfun);
}
