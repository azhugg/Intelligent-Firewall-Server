// Respond.cpp: implementation of the CRespond class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Respond.h"
#include "Rules.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRespond::CRespond()
{

}

CRespond::~CRespond()
{

}

void CRespond::Respond(Packet *p)
{
/*	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	
	if (ptrMainApp->otn_tmp && ptrMainApp->otn_tmp->response_flag)
    {
        if (ptrMainApp->otn_tmp->response_flag & (RESP_RST_SND | RESP_RST_RCV))
        {
            if (p->iph->ip_proto == IPPROTO_TCP)
            {
                int i;

                for (i = 0; i < 5; i++)
                {
                    if (ptrMainApp->otn_tmp->response_flag & RESP_RST_SND)
                        SendTCPRST(p->iph->ip_dst.s_addr, p->iph->ip_src.s_addr,
                                   p->tcph->th_dport, p->tcph->th_sport,
                                   p->tcph->th_ack, p->tcph->th_seq + p->dsize + i);

                    if (ptrMainApp->otn_tmp->response_flag & RESP_RST_RCV)
                        SendTCPRST(p->iph->ip_src.s_addr, p->iph->ip_dst.s_addr,
                                   p->tcph->th_sport, p->tcph->th_dport,
                                   p->tcph->th_seq, p->tcph->th_ack + p->dsize + i);
                }
            }
        }

/*        if (ptrMainApp->otn_tmp->response_flag & RESP_BAD_NET)
            SendICMP_UNREACH(ICMP_UNREACH_NET, p->iph->ip_dst.s_addr,
                             p->iph->ip_src.s_addr, p);

        if (ptrMainApp->otn_tmp->response_flag & RESP_BAD_HOST)
            SendICMP_UNREACH(ICMP_UNREACH_HOST, p->iph->ip_dst.s_addr,
                             p->iph->ip_src.s_addr, p);

        if (ptrMainApp->otn_tmp->response_flag & RESP_BAD_PORT)
            SendICMP_UNREACH(ICMP_UNREACH_PORT, p->iph->ip_dst.s_addr,
                             p->iph->ip_src.s_addr, p);*//*
    }*/
    return;
}

int CRespond::SendTCPRST(u_long saddr, u_long daddr, u_short sport, u_short dport, int seq, int ack)
{
/*	u_char *buf;
    int sz = IP_H + TCP_H;


    if ((buf = malloc(sz)) == NULL)
    {
        perror("SendTCPRST: malloc");
        return -1;
    }

    memset(buf, 0, sz);


    libnet_do_checksum(buf, IPPROTO_TCP, sz - IP_H);
    if (libnet_write_ip(nd, buf, sz) < sz)
    {
        libnet_error(LIBNET_ERR_CRITICAL, "SendTCPRST: libnet_write_ip");
        return -1;
    }

    libnet_destroy_packet(&buf);*/
    return 0;
}

int CRespond::SendICMP_UNREACH(int type, u_long saddr, u_long daddr, Packet *p)
{
/*	u_char *buf;
    int data_sz, addt_sz, sz;

    if (! p)
        return(-1);

    data_sz = (p->dsize > 8 ? 8 : p->dsize);
    addt_sz = ntohs(p->iph->ip_len) + data_sz;
    sz = IP_H + ICMP_UNREACH_H + addt_sz;

    if (libnet_init_packet(sz, &buf) < 0)
    {
        libnet_error(LIBNET_ERR_CRITICAL, "SendICMP_UNREACH: libnet_init_packet");
        return -1;
    }

    memset(buf, 0, sz);


    libnet_build_icmp_unreach(ICMP_UNREACH, type,
                              p->iph->ip_len, p->iph->ip_tos, p->iph->ip_id,
                              p->iph->ip_off, p->iph->ip_ttl, p->iph->ip_proto,
                              p->iph->ip_src.s_addr, p->iph->ip_dst.s_addr,
                              p->data, data_sz,
                              buf + IP_H);

    libnet_do_checksum(buf, IPPROTO_ICMP, sz - IP_H);

    if (libnet_write_ip(nd, buf, sz) < sz)
    {
        libnet_error(LIBNET_ERR_CRITICAL, "SendICMP_UNREACH: libnet_write_ip");
        return -1;
    }*/

    return 0;
}
