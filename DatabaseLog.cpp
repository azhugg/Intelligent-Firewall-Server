// DatabaseLog.cpp: implementation of the CDatabaseLog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "DatabaseLog.h"
#include "Rules.h"
#include "Preprocessor.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDatabaseLog::CDatabaseLog()
{

}

CDatabaseLog::~CDatabaseLog()
{

}

void CDatabaseLog::ParseDatabaseArgs(char *args)
{
	char * dbarg;
    char * a1;

    if (args == NULL)
    {
//        FatalError("log_database: Must supply arguments for database plugin\n");
    }

    dbtype = strtok(args, ", ");

    if (dbtype == NULL) 
    {
//        FatalError("log_database: Must enter database type in configuration file\n");
    }


    dbarg = strtok(NULL, " =");
    while(dbarg != NULL)
    {   
        a1 = NULL;
        a1 = strtok(NULL, " ");
        if(!strncasecmp(dbarg,"host",4))
        {
            host = a1;
        }
        if(!strncasecmp(dbarg,"port",4))
        {
            port = a1;
        }
        if(!strncasecmp(dbarg,"user",4))
        {
            user = a1;
        }
        if(!strncasecmp(dbarg,"password",8))
        {
            password = a1;
        }
        if(!strncasecmp(dbarg,"dbname",6))
        {
            dbname = a1;
        }
        dbarg = strtok(NULL, "=");
    } 

    if (dbname == NULL) 
    {
        exit(-1);
    }
}

void CDatabaseLog::LogDatabase(Packet *p, char *msg, void *arg)
{
	char *i0;
    char *i1;
    char *i2;
    char sip[16];
    char dip[16];
    char *s0,*s1,*s2,*s3,*d0,*d1,*d2,*d3;

    if (p == NULL)
    {
        return;
    }

    if (!p->frag_flag)
    {
        i0 = (char *)malloc(MAX_QUERY_LENGTH);
        i1 = (char *)malloc(MAX_QUERY_LENGTH);
        i2 = (char *)malloc(MAX_QUERY_LENGTH);

        strncpy(sip, inet_ntoa(p->iph->ip_src), 16);
        strncpy(dip, inet_ntoa(p->iph->ip_dst), 16);
        s0 = strtok(sip,".");
        s1 = strtok(NULL,".");
        s2 = strtok(NULL,".");
        s3 = strtok(NULL,".");
        d0 = strtok(dip,".");
        d1 = strtok(NULL,".");
        d2 = strtok(NULL,".");
        d3 = strtok(NULL,".");

        if (p->iph->ip_proto == IPPROTO_ICMP)
        {
            sprintf(i2, 
            "INSERT INTO icmphdr (sid,cid,type,code) VALUES ('%i','%i','%i','%i')",
            sid, cid, p->icmph->type, p->icmph->code);
        }
        else if (p->iph->ip_proto == IPPROTO_TCP)
        {
            sprintf(i2, 
            "INSERT INTO tcphdr (sid,cid,th_sport,th_dport,th_flags,th_win,th_urp) VALUES ('%i','%i','%i','%i','%i','%i','%i')",
             sid, cid, ntohs(p->tcph->th_sport), ntohs(p->tcph->th_dport), 
             p->tcph->th_flags, ntohs(p->tcph->th_win), p->tcph->th_urp);
        }
        else if (p->iph->ip_proto == IPPROTO_UDP)
        {
            sprintf(i2, 
            "INSERT INTO udphdr (sid,cid,uh_sport,uh_dport,uh_len) VALUES ('%i','%i','%i','%i','%i')",
            sid, cid, ntohs(p->udph->uh_sport), ntohs(p->udph->uh_dport), 
            ntohs(p->udph->uh_len));
        }

        if (msg == NULL)
        {
            msg = "NULL MESSAGE";
        }

        sprintf(i0, "INSERT INTO event (sid,cid,signature,timestamp) VALUES ('%i','%i','%s',now());",sid,cid,msg);
        sprintf(i1, "INSERT INTO iphdr (sid,cid,ip_proto,ip_src0,ip_src1,ip_src2,ip_src3,ip_dst0,ip_dst1,ip_dst2,ip_dst3,ip_tos,ip_ttl,ip_id,ip_off,ip_len) VALUES ('%i','%i','%i','%s','%s','%s','%s','%s','%s','%s','%s','%i','%i','%i','%i','%i');",sid,cid,p->iph->ip_proto,s0,s1,s2,s3,d0,d1,d2,d3,p->iph->ip_tos,p->iph->ip_ttl,ntohs(p->iph->ip_id),ntohs(p->frag_offset),ntohs(p->iph->ip_len)); 

        Insert(i0); free(i0);
        Insert(i1); free(i1);
        Insert(i2); free(i2);

        cid++;

    }
}

int CDatabaseLog::Insert(char *query)
{
	int result = 0;

#ifdef ENABLE_MYSQL
    if(!strcasecmp(dbtype,MYSQL))
    {
        if(!(mysql_query(m_sock,query)))
        {
            result = 1;
        }
        if(!result) 
        {
//            ErrorMessage("Error: %s\n", mysql_error(m_sock));
        }
    }
#endif
    return result;
}

int CDatabaseLog::Select(char *query)
{
	int result = 0;

#ifdef ENABLE_MYSQL
    if(!strcasecmp(dbtype,MYSQL))
    {
        if(mysql_query(m_sock,query))
        {
            result = 0;
        }
        else
        {
            if(!(m_result = mysql_use_result(m_sock)))
            {
                result = 0;
            }
            else
            {
                if((m_row = mysql_fetch_row(m_result)))
                {
                    if(m_row[0] != NULL)
                    {
                        result = atoi(m_row[0]);
                    }
                }
            }
        }
        mysql_free_result(m_result);
        if(!result) 
        {
//            ErrorMessage("Error: %s\n", mysql_error(m_sock));
        }
    }
#endif
    return result;
}

void CDatabaseLog::Connect()
{
#ifdef ENABLE_MYSQL
    int x; 
    if(!strcasecmp(dbtype,MYSQL))
    {
        m_sock = mysql_init(NULL);
        if(m_sock == NULL)
        {
//            FatalError("Connection to database '%s' failed\n", dbname);
        } 
        if (port != NULL)
        {
            x = atoi(port);
        }
        else
        {
            x = 0;
        }
        if (mysql_real_connect(m_sock, host, user, password, dbname,x, NULL, 0) == 0)
        {
//            FatalError("Error: %s\n", mysql_error(m_sock));
        }
    }
#endif
}

void CDatabaseLog::LogDatabaseInit(u_char *args)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	
	char * select0;
	char * select1;
	char * insert0;
	
	ptrMainApp->pv.log_plugin_active = 1;

	ParseDatabaseArgs((char *)args);

	select0 = (char *)malloc(MAX_QUERY_LENGTH);
	select1 = (char *)malloc(MAX_QUERY_LENGTH);
	insert0 = (char *)malloc(MAX_QUERY_LENGTH);

	if (ptrMainApp->pv.pcap_cmd == NULL)
	{
		sprintf(insert0, "INSERT INTO sensor (hostname, interface) VALUES ('%s','%s')", getenv("HOSTNAME"), ptrMainApp->pv.interfaces);
		sprintf(select0, "SELECT sid FROM sensor WHERE hostname = '%s' AND interface = '%s' AND filter IS NULL", getenv("HOSTNAME"), ptrMainApp->pv.interfaces);
	}
	else
	{
		sprintf(select0,"SELECT sid FROM sensor WHERE hostname = '%s' and interface = '%s' and filter ='%s';", getenv("HOSTNAME"), ptrMainApp->pv.interfaces, ptrMainApp->pv.pcap_cmd);
		sprintf(insert0,"INSERT INTO sensor (hostname, interface, filter) VALUES ('%s','%s','%s');", getenv("HOSTNAME"), ptrMainApp->pv.interfaces, ptrMainApp->pv.pcap_cmd);
	}

	Connect();

	sid = Select(select0);
	if (sid == 0)
	{
	    Insert(insert0);
	    sid = Select(select0);
	    if (sid == 0)
		{
//      FatalError("Problem obtaining SENSOR ID (sid) from %s->%s->event\n",dbtype,dbname);
		}
	}

	sprintf(select1,"SELECT max(cid) FROM event WHERE sid = '%i'", sid);

	cid = Select(select1);
	cid++;

	  free(select0);
	  free(select1);
	  free(insert0);

	  CRules *objRule;
	  objRule = new CRules;
	  void (CManipulate::*ptrfun)(Packet *p, char *msg, void *arg);
	  ptrfun = &CManipulate::LogDatabase;
	  objRule->AddFuncToOutputList(ptrfun, NT_OUTPUT_LOG, NULL);
}


void CDatabaseLog::SetupLogDatabase()
{
	CPreprocessor *objPrep;	
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(u_char *args);
	ptrfun = &CManipulate::LogDatabaseInit;
	objPrep->RegisterOutputPlugin("log_database",NT_OUTPUT_LOG,ptrfun);
}
