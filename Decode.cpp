// Decode.cpp: implementation of the CDecode class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Decode.h"
#include "Log.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#ifndef ETHERNET_HEADER_LEN
#define ETHERNET_HEADER_LEN             14
#define MINIMAL_TOKENRING_HEADER_LEN    22
#define TR_HLEN                         MINIMAL_TOKENRING_HEADER_LEN
#define TOKENRING_LLC_LEN                8 
#define SLIP_HEADER_LEN                 16
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDecode::CDecode()
{

}

CDecode::~CDecode()
{

}

void CDecode::DecodeEthPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
	int pkt_len;
	int cap_len;

	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;

	bzero((char*)p,sizeof(Packet)); 

	p->pkth = pkthdr;
    p->pkt = pkt;

	pkt_len = pkthdr->len;
	cap_len = pkthdr->caplen;

	p->caplen = cap_len;
	u_int uiSnaplen;
	
	uiSnaplen = (u_int)ptrMainApp->snaplen;
	uiSnaplen = SNAPLEN;

	if(uiSnaplen < (unsigned int)pkt_len)
		pkt_len = cap_len;
	

	p->eh = (EtherHdr*)pkt;
	
	switch(ntohs(p->eh->ether_type))
	{
		case ETHERNET_TYPE_IP:
			DecodeIP(p->pkt+ETHERNET_HEADER_LEN,cap_len-ETHERNET_HEADER_LEN,p);
			return;
		case ETHERNET_TYPE_ARP:
		case ETHERNET_TYPE_REVARP:
			ptrMainApp->pc.arp++;
			DecodeARP(p->pkt+ETHERNET_HEADER_LEN, cap_len-ETHERNET_HEADER_LEN, p);
			return;
		case ETHERNET_TYPE_IPV6:
			ptrMainApp->pc.ipv6++;
			if(ptrMainApp->pv.showipv6_flag)
				DecodeIPV6(p->pkt+ETHERNET_HEADER_LEN, (cap_len-ETHERNET_HEADER_LEN));
			return;
		case ETHERNET_TYPE_IPX:
			ptrMainApp->pc.ipx++;
			if(ptrMainApp->pv.showipx_flag)
				DecodeIPX(p->pkt+ETHERNET_HEADER_LEN, (cap_len-ETHERNET_HEADER_LEN));
			return;

        default:
            ptrMainApp->pc.other++;
            return;
	}
	return;
}

void CDecode::DecodeIP(u_char *pkt, const int len, Packet *p)
{
	u_int ip_len;	
	u_int hlen;		

	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;

	p->iph = (IPHdr *)pkt;

		
	if (len < IP_HEADER_LEN)
    {
    	p->iph = NULL;
		return;
	}
	

	if(p->iph->ip_ver !=4)
	{
		p->iph = NULL;
		return;
	}

	ip_len = ntohs(p->iph->ip_len);

	hlen = p->iph->ip_hlen << 2;

	if(p->iph->ip_hlen > 5)
	{
		DecodeIPOptions((pkt + IP_HEADER_LEN),hlen - IP_HEADER_LEN,p);
	}
	else
	{
		p->ip_option_count = 0;
	}

	ip_len -= hlen;

	p->sip = p->iph->ip_src.s_addr; 
	p->dip = p->iph->ip_dst.s_addr;	

	p->frag_flag = ntohs(p->iph->ip_off);

	p->df = (p->frag_offset & 0x4000) >> 14;
	p->mf = (p->frag_offset & 0x2000) >> 13;

    p->frag_offset &= 0x1FFF;
	
	if (p->frag_offset || p->mf)
    {
        p->frag_flag = 1;
    }

	if (!(p->frag_flag))
    {
        p->frag_flag = 0;

	CLog lBuff;

	switch (p->iph->ip_proto)
        {
            case IPPROTO_TCP:
                ptrMainApp->pc.tcp++;
                DecodeTCP(pkt + hlen, ip_len, p);
                lBuff.ClearDumpBuf();
                return;

            case IPPROTO_UDP:
                ptrMainApp->pc.udp++;
                DecodeUDP(pkt + hlen, ip_len, p);
                lBuff.ClearDumpBuf();
                return;

            case IPPROTO_ICMP:
                ptrMainApp->pc.icmp++;
                DecodeICMP(pkt + hlen, ip_len, p);
                lBuff.ClearDumpBuf();
                return;

            default:
                ptrMainApp->pc.other++;
                p->data = pkt + hlen;
                p->dsize = ip_len;
                lBuff.ClearDumpBuf();
                return;
			}
        }
		else 
		{
				ptrMainApp->pc.frags++;

			switch (p->iph->ip_proto)
			{
				case IPPROTO_TCP:
					ptrMainApp->pc.tcp++;
					break;

				case IPPROTO_UDP:
					ptrMainApp->pc.udp++;
					break;

				case IPPROTO_ICMP:
					ptrMainApp->pc.icmp++;
					break;

				default:
					ptrMainApp->pc.other++;
					break;
			  }
			p->data = pkt + hlen;
			p->dsize = ip_len;
    }

}

void CDecode::DecodeTCP(u_char *pkt, const int len, Packet *p)
{
	int hlen;
	CMessage msg;	
	CString uMsg;	

	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;		

    p->tcph = (TCPHdr *) pkt;

    hlen = p->tcph->th_off << 2;

    if(hlen > 20)
    {
		DecodeTCPOptions((u_char *)(pkt+20), (hlen - 20), p);;
	}
	else if(hlen < 20)
    {
	   p->tcph = NULL;
       return;
    }
	else
		p->tcp_option_count = 0;

	p->sp = ntohs(p->tcph->th_sport);
	p->dp = ntohs(p->tcph->th_dport);

    p->data = (u_char *)(pkt + hlen);

	if (hlen < len)
    {
        p->dsize = len - hlen;
    }
    else
    {
        p->dsize = 0;
    }
}

void CDecode::DecodeTCPOptions(u_char *o_list, int o_len, Packet *p)
{
	u_char *option_ptr;
    int bytes_processed;
    int current_option;

	option_ptr = o_list;
    bytes_processed = 0;
    current_option = 0;

	while((bytes_processed < o_len) && (current_option < 40))
	{
		p->tcp_options[current_option].code = *option_ptr;
		
		switch(*option_ptr)
		{
			case TCPOPT_NOP:
			
			case TCPOPT_EOL:
				p->tcp_options[current_option].len =0;
				p->tcp_options[current_option].data = NULL;
				bytes_processed++;
				current_option++;
				option_ptr++;
				break;
			
			case TCPOPT_SACKOK:
                p->tcp_options[current_option].len = 0;
                p->tcp_options[current_option].data = NULL;
                bytes_processed+=2;
                option_ptr+=2;
                current_option++;
                break;
            case TCPOPT_WSCALE:
                p->tcp_options[current_option].len = 3;
                p->tcp_options[current_option].data = option_ptr+2;
                option_ptr+=3;
                bytes_processed+=3;
                current_option++;
                break;

			default:
				p->tcp_options[current_option].len = *(option_ptr+1);
                if (p->tcp_options[current_option].len > 40)
                {
                    p->tcp_options[current_option].len = 40;
                }
                p->tcp_options[current_option].data = option_ptr+2;
                option_ptr+= p->tcp_options[current_option].len;
                bytes_processed+= p->tcp_options[current_option].len;
                current_option++;
				break;
		}
	}

	if (bytes_processed > o_len)
    {
        p->tcp_options[current_option].len = p->tcp_options[current_option].len - (bytes_processed - o_len);

        if (p->tcp_options[current_option].len < 0)
	           current_option--;
    }
	p->tcp_option_count = current_option;
}

void CDecode::DecodeIPOptions(u_char *o_list, int o_len, Packet *p)
{
	u_char *option_ptr;
    int bytes_processed;
    int current_option;

    option_ptr = o_list;
    bytes_processed = 0;
    current_option = 0;

    while ((bytes_processed < o_len) && (current_option < 40)){
        p->ip_options[current_option].code = *option_ptr;   

        switch (*option_ptr){
            case IPOPT_NOP:
            case IPOPT_EOL:
                p->ip_options[current_option].len = 0;
                p->ip_options[current_option].data = NULL;
                bytes_processed++;
                current_option++;
                option_ptr++;

                break;

            default:
                p->ip_options[current_option].len = *(option_ptr+1);

                if (p->ip_options[current_option].len > 40){
                    p->ip_options[current_option].len = 40;
                }

                p->ip_options[current_option].data = option_ptr+2;
                option_ptr+= p->ip_options[current_option].len;
                bytes_processed+= p->ip_options[current_option].len;
                current_option++;
                break;
        }
    }

    if (bytes_processed > o_len){
        p->ip_options[current_option].len = p->ip_options[current_option].len - (bytes_processed - o_len);
        if (p->ip_options[current_option].len < 0) current_option--;
    }
    p->ip_option_count = current_option;
}


void CDecode::DecodeUDP(u_char *pkt, const int len, Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;		


	CMessage msg;	
	CString uMsg;	
	if(len < 8)
    {
       p->udph = NULL;
       return;
    }
	
    p->udph = (UDPHdr *) pkt;

    p->sp = ntohs(p->udph->uh_sport);
    p->dp = ntohs(p->udph->uh_dport);

	p->data = (u_char *)(pkt + UDP_HEADER_LEN);

	if ((len - UDP_HEADER_LEN) > 0)
    {
        p->dsize = len - UDP_HEADER_LEN;
    }
    else
    {
        p->dsize = 0;
    }
}

void CDecode::DecodeICMP(u_char *pkt, const int len, Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;		
	CMessage msg;	
	CString uMsg;	
    p->icmph = (ICMPHdr *) pkt;
	if(len < 4){
       p->icmph = NULL;
       return;
    }
	p->dsize = len - ICMP_HEADER_LEN;
    p->data = pkt + ICMP_HEADER_LEN;
	switch (p->icmph->type)
    {
		case ICMP_ECHOREPLY:
            p->dsize -= 4;
            p->data += 4;
            break;
        case ICMP_ECHO:
            p->dsize -= 4;
            p->data += 4;
            break;
    }
    return;
}

void CDecode::DecodeARP(u_char *pkt, int len, Packet *p)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;		
	CMessage msg;	
	p->ah = (EtherARP *) pkt;
	if (len < sizeof(EtherARP)){
  		return;
    }
    return;
}

void CDecode::DecodeIPV6(u_char *pkt, int len)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;		
	
	if (ptrMainApp->pv.verbose_flag)
    {
        puts("IPV6 packet");
    }
    return;
}


void CDecode::DecodeIPX(u_char *pkt, int len)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;			
	
	if (ptrMainApp->pv.verbose_flag)
    {
        puts("IPX packet");
    }

    return;
}


void CDecode::DecodeI4LCiscoIPPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
    bzero((char *)p, sizeof(Packet));
    p->pkth = pkthdr;
    p->pkt = pkt;
    DecodeIP(pkt + 4, p->pkth->len - 4, p);
    return;
}

void CDecode::DecodeI4LRawIPPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
    bzero((char *)p, sizeof(Packet));
    p->pkth = pkthdr;
    p->pkt = pkt;
    DecodeIP(pkt + 2, p->pkth->len - 2, p);
    return;
}

void CDecode::DecodeFDDIPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;		

	CMessage msg;	
	CString uMsg;	

	int pkt_len; 
    int cap_len; 
    int dataoff; 
    bzero((char *)p, sizeof(Packet));
    p->pkth = pkthdr;
    p->pkt = pkt;
    pkt_len = pkthdr->len;
    cap_len = pkthdr->caplen;
    if (ptrMainApp->snaplen < (unsigned int)pkt_len){
        pkt_len = cap_len;
    }
	
    if (p->pkth->caplen < FDDI_MIN_HLEN){
         	        return;
    }
    
    p->fddihdr = (Fddi_hdr *) pkt;
    p->fddisaps = (Fddi_llc_saps *)(pkt + sizeof(Fddi_hdr));
    if ((p->fddisaps->dsap==FDDI_DSAP_IP) && (p->fddisaps->ssap==FDDI_SSAP_IP)){
        p->fddiiparp = (Fddi_llc_iparp *)(pkt + sizeof(Fddi_hdr) + sizeof(Fddi_llc_saps));
        dataoff = sizeof(Fddi_hdr) + sizeof(Fddi_llc_saps) + sizeof(Fddi_llc_iparp);
    }
    else if ((p->fddisaps->dsap==FDDI_DSAP_SNA) && (p->fddisaps->ssap==FDDI_SSAP_SNA)){
        p->fddisna = (Fddi_llc_sna *)(pkt + sizeof(Fddi_hdr) + sizeof(Fddi_llc_saps));
        dataoff = sizeof(Fddi_hdr) + sizeof(Fddi_llc_saps) + sizeof(Fddi_llc_sna);
    }
    else{
        p->fddiother = (Fddi_llc_other *)(pkt + sizeof(Fddi_hdr) + sizeof(Fddi_llc_other));
        dataoff = sizeof(Fddi_hdr) + sizeof(Fddi_llc_saps) + sizeof(Fddi_llc_other);
    }
    if ((p->fddisaps->dsap!=FDDI_DSAP_IP) && (p->fddisaps->ssap!=FDDI_SSAP_IP))
    {
		    return;
    }

    pkt_len -= dataoff;
    cap_len -= dataoff;

    switch (htons(p->fddiiparp->ethertype)){
		 case ETHERNET_TYPE_IP:
			 DecodeIP(p->pkt+dataoff, cap_len, p);
	         return;
         case ETHERNET_TYPE_ARP:
         case ETHERNET_TYPE_REVARP:
			 ptrMainApp->pc.arp++;
             return;
        default:
			ptrMainApp->pc.other++;
            return;
    }
    return;
}



void CDecode::DecodeNullPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
    u_int len;
    u_int cap_len;
    bzero((char *)p, sizeof(Packet));
    p->pkth = pkthdr;
    p->pkt = pkt;
    len = pkthdr->len;
    cap_len = pkthdr->caplen;
    if (cap_len < NULL_HDRLEN){
        return;
    }
    DecodeIP(p->pkt + NULL_HDRLEN, cap_len - NULL_HDRLEN, p);
}

void CDecode::DecodePppPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
	u_int len;
    u_int cap_len;
    bzero((char *)p, sizeof(Packet));
    p->pkth = pkthdr;
    p->pkt = pkt;
    len = pkthdr->len;
    cap_len = pkthdr->caplen;
    if (cap_len < PPP_HDRLEN){
    	return;
    }

    DecodeIP(p->pkt + PPP_HDRLEN, cap_len - PPP_HDRLEN, p);
}

void CDecode::DecodeRawPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
    bzero((char *)p, sizeof(Packet));

    p->pkth = pkthdr;
    p->pkt = pkt;
    
	DecodeIP(pkt, p->pkth->caplen, p);
    return;
}

void CDecode::DecodeSlipPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
	u_int len;
    u_int cap_len;
    bzero((char *)p, sizeof(Packet));
    p->pkth = pkthdr;
    p->pkt = pkt;
    len = pkthdr->len;
    cap_len = pkthdr->caplen;
	if (cap_len < SLIP_HEADER_LEN){   
        return;
    }
    DecodeIP(p->pkt + SLIP_HEADER_LEN, cap_len - SLIP_HEADER_LEN, p);     
}

void CDecode::DecodeTRPkt(Packet *p, pcap_pkthdr *pkthdr, u_char *pkt)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;		
	CMessage msg;
	CString uMsg;
	int pkt_len;  
    int cap_len;  
    int dataoff; 
    bzero((char *)p, sizeof(Packet));
    p->pkth = pkthdr;
    p->pkt = pkt;
    pkt_len = pkthdr->len;       
    cap_len = pkthdr->caplen;    
    if (ptrMainApp->snaplen < (unsigned int)pkt_len)
        pkt_len = cap_len;
    if (p->pkth->caplen < TR_HLEN){
		return;
		}
    p->trh = (Trh_hdr *) pkt;  
    p->trhllc=(Trh_llc *)(pkt + sizeof(Trh_hdr));
    if (p->trhllc->dsap!=IPARP_SAP && p->trhllc->ssap!=IPARP_SAP){
        p->trhmr=(Trh_mr *)(pkt + sizeof(Trh_hdr));
        p->trhllc=(Trh_llc *)(pkt + sizeof(Trh_hdr) + p->trhmr->len);
        dataoff=sizeof(Trh_hdr) + p->trhmr->len + sizeof(Trh_llc);
    }
    else{
        p->trhllc=(Trh_llc *)(pkt + sizeof(Trh_hdr));
        dataoff=sizeof(Trh_hdr) + sizeof(Trh_llc);
    }
	if (p->trhllc->dsap!=IPARP_SAP && p->trhllc->ssap!=IPARP_SAP){	
	    p->trhllc = NULL;
	    return;
    }
    pkt_len -= dataoff;
    cap_len -= dataoff;
    switch (htons(p->trhllc->ethertype))
    {
        case ETHERNET_TYPE_IP:
            DecodeIP(p->pkt+dataoff, cap_len, p);
            return;
        case ETHERNET_TYPE_ARP:
        case ETHERNET_TYPE_REVARP:
            ptrMainApp->pc.arp++;
            return;
        default:
			ptrMainApp->pc.other++;
            return;
    }
    return;
}