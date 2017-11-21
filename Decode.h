// Decode.h: interface for the CDecode class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECODE_H__F2C2D506_31E8_4ADE_8343_B584DF05D73F__INCLUDED_)
#define AFX_DECODE_H__F2C2D506_31E8_4ADE_8343_B584DF05D73F__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "IFServer.h"

#ifndef IRIX
#define SNAPLEN      1514
#else
#define SNAPLEN      1500
#endif

#define MIN_SNAPLEN  68
#define PROMISC      1
#define READ_TIMEOUT 500


class CDecode  
{
public:
	void DecodeTRPkt(Packet *p, struct pcap_pkthdr *pkthdr, u_char *pkt);
	void DecodeSlipPkt(Packet *p, struct pcap_pkthdr *pkthdr, u_char *pkt);
	void DecodeRawPkt(Packet *p, struct pcap_pkthdr *pkthdr, u_char *pkt);
	void DecodePppPkt(Packet *p, struct pcap_pkthdr *pkthdr, u_char *pkt);
	void DecodeNullPkt(Packet* p, struct pcap_pkthdr* pkthdr, u_char* pkt);
	void DecodeFDDIPkt(Packet *p, struct pcap_pkthdr *pkthdr, u_char *pkt);
	void DecodeI4LRawIPPkt(Packet *p, struct pcap_pkthdr *pkthdr, u_char *pkt);
	void DecodeI4LCiscoIPPkt(Packet *p, struct pcap_pkthdr *pkthdr, u_char *pkt);
	void DecodeIPX(u_char *pkt, int len);
	void DecodeIPV6(u_char *pkt, int len);
	void DecodeARP(u_char *pkt, int len, Packet *p);
	void DecodeICMP(u_char *pkt, const int len, Packet *p);
	void DecodeUDP(u_char *pkt, const int len, Packet *p);
	void DecodeIPOptions(u_char *o_list, int o_len, Packet *p);
	void DecodeTCPOptions(u_char* o_list, int o_len, Packet* p);
	void DecodeTCP(u_char *pkt, const int len, Packet *p);
	void DecodeIP(u_char *pkt, const int len, Packet *p);
	void DecodeEthPkt(Packet *p, struct pcap_pkthdr* pkthdr, u_char* pkt);
	CDecode();
	virtual ~CDecode();

};

#endif // !defined(AFX_DECODE_H__F2C2D506_31E8_4ADE_8343_B584DF05D73F__INCLUDED_)
