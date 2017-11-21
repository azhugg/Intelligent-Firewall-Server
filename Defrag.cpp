// Defrag.cpp: implementation of the CDefrag class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Defrag.h"
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

CDefrag::CDefrag()
{

}

CDefrag::~CDefrag()
{

}

void CDefrag::PreprocDefrag(Packet *p)
{
	Packet *newp;
	time_struct timecheck;
	Tree *found;
	frag freetemp;
	int overhead;
    int cap;
    u_char* tmp;


	if(!p || !p->pkt || !p->pkt || !p->iph)
	{
//		ErrorMessage("%s\n","Garbage Packet with Null Pointer discarded!");
		return;
	}
	if (p->frag_flag) {  
		overhead = 2 + (char*)p->pkt - (char*)p->pkth; 
		if( overhead < 1 || overhead > sizeof( struct pcap_pkthdr) + 4)
			overhead = sizeof(struct pcap_pkthdr) + 2;
		newp = (Packet*) malloc(sizeof(Packet));
		if(newp == NULL){
//			ErrorMessage("Cannot allocate fragment buffer(usage 0x%X)\n",fragmemuse);
			return;
		}
		fragmemuse += sizeof(Packet);
		memcpy(newp, p, sizeof(Packet));
                cap = p->caplen + overhead;
		tmp = (u_char*) calloc(cap, sizeof(char));
                newp->pkth = (struct pcap_pkthdr *) tmp;
		newp->pkt = (u_char*)newp->pkth + overhead;
		newp->iph = (IPHdr*)((u_char*)newp->pkt + ((u_char*)p->iph - (u_char*)p->pkt));
		if(newp == NULL){
//			ErrorMessage("Cannot allocate fragment buffer(usage %X)\n",fragmemuse);
			return;
		}
		fragmemuse += p->pkth->caplen + overhead;
		memcpy(newp->pkth, p->pkth, overhead);
		memcpy(newp->pkt, p->pkt, p->pkth->caplen);
		froot = FragInsert(newp, froot); 
		if(!MF(p))
			froot = ReassembleIP(froot);
		if(froot){
			if(++fragsweep > node_size(froot)+1)
				fragsweep = 1;
			found = FragFind_Rank(fragsweep,froot);
			if(found != NULL){
				froot = FragsPlay(found->key, froot);
                            AddTime((time_struct *)&(froot->key->pkth->ts), &fragtimeout, &timecheck);
                                if(TIME_LT(timecheck. , (unsigned int)p->pkth->ts.)){
					fragmemuse -= froot->key->pkth->caplen + sizeof(Packet);
					freetemp = froot->key;
					froot = FragDelete(froot->key, froot);
					free(freetemp->pkth);  
					free(freetemp);
					fragsweep--;
				}
			}

			if(froot && fragmemuse > FASTSWEEPLIM){
				if(++fragsweep > node_size(froot))
					fragsweep = 0;
				found = FragFind_Rank(fragsweep,froot);
				if(found){
 					froot = FragsPlay(found->key, froot);
					AddTime((time_struct *)&(froot->key->pkth->ts), &fragtimeout, &timecheck);
					if(TIME_LT(timecheck. , (unsigned int)p->pkth->ts. )){
						fragmemuse -= froot->key->pkth->caplen + sizeof(Packet);
						freetemp = froot->key;
						froot = FragDelete(froot->key, froot);
						free(freetemp->pkth);  
						free(freetemp);
						fragsweep--;
					}
				}
			}
		}
	}
	return;
}

Tree * CDefrag::FragInsert(frag i, Tree *t)
{
    Tree * newp;
    if (t != NULL){
		t = FragsPlay(i,t);
		if (FragCompare(i, t->key)==0){
			return t;
		}
    }
    newp = (Tree *) malloc (sizeof (Tree));
    if (newp == NULL){ 
//		ErrorMessage("Ran out of space\n"); 
		return(t);
	}
    if (t == NULL){
		newp->left = newp->right = NULL;
    }
	else if (FragCompare(i, t->key) < 0){
		newp->left = t->left;
		newp->right = t;
		t->left = NULL;
		t->size = 1+node_size(t->right);
    }else{
		newp->right = t->right;
		newp->left = t;
		t->right = NULL;
		t->size = 1+node_size(t->left);
    }
    newp->key = i;
    newp->size = 1 + node_size(newp->left) + node_size(newp->right);
    return newp;
}

Tree* CDefrag::ReassembleIP(Tree *froot)
{
	Packet *p;
	int writecount = 0;
	char *np;
    u_char *tmp;
	u_int psize;
	int fragrank;
	int overhead, moreoverhead;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	void (CManipulate::*ptrFunAlert)(Packet*,char*);
	ptrFunAlert = ptrMainApp->AlertFunc;
	CManipulate objManip;
	psize = (froot->key)->dsize + FOFF(froot->key); 
	moreoverhead = (char*)froot->key->iph - (char*)froot->key->pkt;
	overhead = 2 + (char*)froot->key->pkt - (char*)froot->key->pkth; 
	if( overhead < 1 || overhead > sizeof( struct pcap_pkthdr) + 4)
		overhead = sizeof(struct pcap_pkthdr) + 2;
	p = (Packet *)malloc(sizeof(Packet));
	if (p == NULL) return NULL;
        memcpy(p, froot->key, sizeof(Packet));

	p->pkth = (struct pcap_pkthdr *)malloc(psize + overhead + moreoverhead);
	if (p->pkth == NULL){
		free(p);return NULL;
	}
	p->iph = (IPHdr *)((u_char*)p->pkth + overhead + moreoverhead);
	p->pkt = (u_char*)p->iph - moreoverhead;
	memcpy(p->pkth, froot->key->pkth, overhead); 
	memcpy((char*)p->pkth + overhead, froot->key->pkt, moreoverhead); 
        tmp = (u_char *) froot->key->iph;
	memcpy(p->iph, tmp, sizeof(IPHdr)); 
	p->pkth->caplen = psize + overhead + moreoverhead + sizeof(IPHdr);
	p->pkth->len = p->pkth->caplen;
	p->iph->ip_off ^= IP_MF;
	p->iph->ip_len = htons((unsigned short)psize);
	p->iph->ip_off = 0;
        p->frag_flag = 0;
	fragrank = FragGet_Rank(froot);
	while(fragrank > 0 && froot && FragAddrMatch(p,froot->key)){
		if((unsigned int)(FOFF(froot->key) + froot->key->dsize) <= psize){
			memcpy(DATA(p)+FOFF(froot->key), DATA(froot->key), froot->key->dsize);
			writecount += froot->key->dsize;
		}else (objManip.*ptrFunAlert)(p, "Fragmentation Overflow Attack");
		fragmemuse -= froot->key->pkth->caplen + sizeof(Packet);
		np = (char *)froot->key->pkth;  
		fragrank = FragGet_Rank(froot);
		if(froot) froot = FragDelete(froot->key, froot);
		if(np) free(np);
	}
	if(psize > 8192){
		if(writecount > (int)psize/2){	
			if (p != NULL) ptrMainApp->ProcessPacket(NULL, p->pkth, p->pkt);
		}else{
			(objManip.*ptrFunAlert)(p, "Mostly Empty Fragmented Packet Discarded!");
		}
	} 
	else{
		if(p) ptrMainApp->ProcessPacket(NULL, p->pkth, p->pkt);
	}
	free(p->pkth);
	free(p);
	return(froot);
}

Tree* CDefrag::FragFind_Rank(int r, Tree *t)
{
    int lsize;
    if ((r < 0) || (r >= node_size(t))) return NULL;
    for (; t != 0 ;){
	lsize = node_size(t->left);
	if (r < lsize) {
	    t = t->left;
	}else if(r > lsize) {
	    r = r - lsize -1;
	    t = t->right;
	}else {
	    return t;
	}}return NULL;
}

Tree* CDefrag::FragsPlay(frag i, Tree *t)
{
    Tree N, *l, *r, *y;
    int comp, root_size, l_size, r_size;
    if (t == NULL) return t;
    N.left = N.right = NULL;
    l = r = &N;
    root_size = node_size(t);
    l_size = r_size = 0;
    for (;;){
        comp = FragCompare(i, t->key);
        if (comp < 0){
            if (t->left == NULL) break;
            if (FragCompare(i, t->left->key) < 0){
                y = t->left;                     
                t->left = y->right;
                y->right = t;
                t->size = node_size(t->left) + node_size(t->right) + 1;
                t = y;
                if (t->left == NULL) break;
            }
            r->left = t;                         
            r = t;
            t = t->left;
            r_size += 1+node_size(r->right);
        } else if (comp > 0) {
            if (t->right == NULL) break;
            if (FragCompare(i, t->right->key) > 0) {
                y = t->right;                    
                t->right = y->left;
                y->left = t;
		t->size = node_size(t->left) + node_size(t->right) + 1;
                t = y;
                if (t->right == NULL) break;
            }
            l->right = t;                              
            l = t;
            t = t->right;
            l_size += 1+node_size(l->left);
        } else {
            break;
        }
    }
    l_size += node_size(t->left);  
    r_size += node_size(t->right); 
    t->size = l_size + r_size + 1;
    l->right = r->left = NULL;
    for (y = N.right; y != NULL; y = y->right){
        y->size = l_size;
        l_size -= 1+node_size(y->left);
    }
    for (y = N.left; y != NULL; y = y->left){
        y->size = r_size;
        r_size -= 1+node_size(y->right);
    }
    l->right = t->left;                                
    r->left = t->right;
    t->left = N.right;
    t->right = N.left;
    return t;
}

void CDefrag::AddTime(time_struct *op1, time_struct *op2, time_struct *result)
{
	result->tv_usec = op1->tv_usec+op2->tv_usec;
    if (result->tv_usec > 999999){
        result->tv_usec -= 1000000;
        op1->tv_sec++;
	}
	result->tv_sec = op1->tv_sec+op2->tv_sec;
}

Tree* CDefrag::FragDelete(frag i, Tree *t)
{
    Tree * x;
    int tsize;
    if (t == NULL) return NULL;
    tsize = t->size;
    t = FragsPlay(i,t);
    if( FragCompare(i, t->key) == 0){               
	if (t->left == NULL) {
	    x = t->right;
	} else {
	    x = FragsPlay(i, t->left);
	    x->right = t->right;
	}
	free(t);
	if (x != NULL){
	    x->size = tsize-1;
	}
		return x;
    } 
	else { return t;                         
    }
}

int CDefrag::FragCompare(frag i, frag j)
{
	if(j == NULL){
		if(i == NULL)
			return(0);
		else
			return(1);
	}else{
		if(i == NULL)
			return(-1);
	}
             if( SADDR(i) > SADDR(j)){
					return (1); 
				}
	else if( SADDR(i) < SADDR(j))
		 { return (-1);}
	else if( DADDR(i) > DADDR(j))
		 { return (1);}
	else if( DADDR(i) < DADDR(j))
		 { return (-1);}
	else if( PROTO(i) > PROTO(j))
		 { return (1);}
	else if( PROTO(i) < PROTO(j))
		 { return (-1);}
	else if( ID(i) > ID(j))
		 { return (1);}
	else if( ID(i) < ID(j))
		 { return (-1);}
	else if( FOFF(i) > FOFF(j))
		 { return (1);}
	else if( FOFF(i) < FOFF(j))
		 { return (-1);}
	else if( i->dsize > j->dsize)
		 { return (1);}
	else if( i->dsize < j->dsize)
		 { return (-1);}
	return (0);
}

int CDefrag::FragGet_Rank(Tree *t)
{
	if(t == NULL) return(0);
	return(node_size(t->left)+1);
}

int CDefrag::FragAddrMatch(frag i, frag j)
{
	if((SADDR(i) == SADDR(j)) && (DADDR(i) == DADDR(j))	&& (ID(i) == ID(j)) && (PROTO(i) == PROTO(j)))
			return (1);
	return (0);
}

void CDefrag::DefragInit(u_char *args)
{
	void (CManipulate::*ptrfun)(Packet*);
	ptrfun = &CManipulate::PreprocDefrag;
	CRules objRule;
	objRule.AddFuncToPreprocList(ptrfun);
	froot = NULL;  
	fragmemuse = 0;	
	fragtimeout.tv_sec = FRAGTIMEOUTSEC;
	fragtimeout.tv_usec = FRAGTIMEOUTUSEC;
	fragsweep=0;
}

void CDefrag::SetupDefrag()
{
	CPreprocessor objPrep;
	void (CManipulate::*ptrfun)(u_char*);
	ptrfun = &CManipulate::DefragInit;
	objPrep.RegisterPreprocessor("defrag",ptrfun);
}
