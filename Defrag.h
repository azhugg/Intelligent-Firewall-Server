// Defrag.h: interface for the CDefrag class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DEFRAG_H__EF4702CB_A014_4680_8DC6_08623F09E87D__INCLUDED_)
#define AFX_DEFRAG_H__EF4702CB_A014_4680_8DC6_08623F09E87D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define ID(x)		*((short*)((char*)x->iph+4))
#define PROTO(x)	*((char*)x->iph+9)
#define	SADDR(x)	x->sip
#define	DADDR(x)	x->dip
#define	DATA(x)		((char*)x->iph+20)
#define FOFF(x)		((x->frag_offset)<<3)
#define DF(x)		(x->df)
#define MF(x)		(x->mf)

#define BPFHDRLEN	sizeof(struct pcap_pkthdr)+2
#define TIME_LT(x,y) (x tv_sec<y tv_sec||(x tv_sec==y tv_sec&&x tv_usec<y tv_usec))

#define node_size(x) (((x)==NULL) ? 0 : ((x)->size))
#define FRAGTIMEOUTSEC	10	
#define FRAGTIMEOUTUSEC	0	
#define FASTSWEEPLIM	16000000	
#define IP_OFFMASK 0x1fff
#define IP_MF 0x2000
#define IP_DF 0x4000
#define IP_RF 0x8000

class CDefrag : public CManipulate  
{
public:
	void PreprocDefrag(Packet *p);
	CDefrag();
	virtual ~CDefrag();
	
public:
	void SetupDefrag();
	void DefragInit(u_char *args);
	int FragAddrMatch(frag i,frag j);
	int FragGet_Rank(Tree *t);
	int FragCompare(frag i,frag j);
	Tree* FragDelete(frag i, Tree *t);
	void AddTime(time_struct * op1,time_struct *  op2,time_struct * result);
	Tree* FragsPlay(frag i, Tree *t);
	Tree* FragFind_Rank(int r, Tree *t);
	Tree* ReassembleIP(Tree *froot);
	Tree * FragInsert(frag i, Tree * t);
	Tree *froot;
	long int fragmemuse;
	int fragsweep;  
	time_struct fragtimeout;
};

#endif // !defined(AFX_DEFRAG_H__EF4702CB_A014_4680_8DC6_08623F09E87D__INCLUDED_)
