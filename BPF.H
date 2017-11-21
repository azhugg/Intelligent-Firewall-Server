

#ifndef BPF_MAJOR_VERSION
#define BPF_RELEASE 199606
#ifdef WIN32 
#include "gnuc.h"
#include <winsock2.h>
#endif

typedef	int bpf_int32;
typedef	u_int bpf_u_int32;
#ifndef __NetBSD__
#define BPF_ALIGNMENT sizeof(bpf_int32)
#else
#define BPF_ALIGNMENT sizeof(long)
#endif
#define BPF_WORDALIGN(x) (((x)+(BPF_ALIGNMENT-1))&~(BPF_ALIGNMENT-1))
#define BPF_MAXINSNS 512
#define BPF_MAXBUFSIZE 0x8000
#define BPF_MINBUFSIZE 32
struct bpf_program {
	u_int bf_len;
	struct bpf_insn *bf_insns;
};
struct bpf_stat {
	u_int bs_recv;		
	u_int bs_drop;		
};

struct bpf_version {
	u_short bv_major;
	u_short bv_minor;
};
#define BPF_MAJOR_VERSION 1
#define BPF_MINOR_VERSION 1

#if (defined(sun) || defined(ibm032)) && !defined(__GNUC__)
#define	BIOCGBLEN	_IOR(B,102, u_int)
#define	BIOCSBLEN	_IOWR(B,102, u_int)
#define	BIOCSETF	_IOW(B,103, struct bpf_program)
#define	BIOCFLUSH	_IO(B,104)
#define BIOCPROMISC	_IO(B,105)
#define	BIOCGDLT	_IOR(B,106, u_int)
#define BIOCGETIF	_IOR(B,107, struct ifreq)
#define BIOCSETIF	_IOW(B,108, struct ifreq)
#define BIOCSRTIMEOUT	_IOW(B,109, struct timeval)
#define BIOCGRTIMEOUT	_IOR(B,110, struct timeval)
#define BIOCGSTATS	_IOR(B,111, struct bpf_stat)
#define BIOCIMMEDIATE	_IOW(B,112, u_int)
#define BIOCVERSION	_IOR(B,113, struct bpf_version)
#define BIOCSTCPF	_IOW(B,114, struct bpf_program)
#define BIOCSUDPF	_IOW(B,115, struct bpf_program)
#else
#define	BIOCGBLEN	_IOR('B',102, u_int)
#define	BIOCSBLEN	_IOWR('B',102, u_int)
#define	BIOCSETF	_IOW('B',103, struct bpf_program)
#define	BIOCFLUSH	_IO('B',104)
#define BIOCPROMISC	_IO('B',105)
#define	BIOCGDLT	_IOR('B',106, u_int)
#define BIOCGETIF	_IOR('B',107, struct ifreq)
#define BIOCSETIF	_IOW('B',108, struct ifreq)
#define BIOCSRTIMEOUT	_IOW('B',109, struct timeval)
#define BIOCGRTIMEOUT	_IOR('B',110, struct timeval)
#define BIOCGSTATS	_IOR('B',111, struct bpf_stat)
#define BIOCIMMEDIATE	_IOW('B',112, u_int)
#define BIOCVERSION	_IOR('B',113, struct bpf_version)
#define BIOCSTCPF	_IOW('B',114, struct bpf_program)
#define BIOCSUDPF	_IOW('B',115, struct bpf_program)
#endif
struct bpf_hdr {
	struct timeval	bh_tstamp;	
	bpf_u_int32	bh_caplen;	
	bpf_u_int32	bh_datalen;	
	u_short		bh_hdrlen;	
};
#if defined(KERNEL) || defined(_KERNEL)
#define SIZEOF_BPF_HDR 18
#endif
#define DLT_NULL	0	
#define DLT_EN10MB	1	
#define DLT_EN3MB	2	
#define DLT_AX25	3	
#define DLT_PRONET	4	
#define DLT_CHAOS	5	
#define DLT_IEEE802	6	
#define DLT_ARCNET	7	
#define DLT_SLIP	8	
#define DLT_PPP		9	
#define DLT_FDDI	10	
#define DLT_ATM_RFC1483	11	
#define DLT_RAW		12	
#define DLT_SLIP_BSDOS	15	
#define DLT_PPP_BSDOS	16	
#define DLT_ATM_CLIP	19	
#define DLT_PPP_SERIAL	50	
#define DLT_C_HDLC	104	
#define DLT_CHDLC	DLT_C_HDLC
#define DLT_IEEE802_11	105	
#define DLT_LOOP	108
#define DLT_LINUX_SLL	113
#define BPF_CLASS(code) ((code) & 0x07)
#define		BPF_LD		0x00
#define		BPF_LDX		0x01
#define		BPF_ST		0x02
#define		BPF_STX		0x03
#define		BPF_ALU		0x04
#define		BPF_JMP		0x05
#define		BPF_RET		0x06
#define		BPF_MISC	0x07
#define BPF_SIZE(code)	((code) & 0x18)
#define		BPF_W		0x00
#define		BPF_H		0x08
#define		BPF_B		0x10
#define BPF_MODE(code)	((code) & 0xe0)
#define		BPF_IMM 	0x00
#define		BPF_ABS		0x20
#define		BPF_IND		0x40
#define		BPF_MEM		0x60
#define		BPF_LEN		0x80
#define		BPF_MSH		0xa0
#define BPF_OP(code)	((code) & 0xf0)
#define		BPF_ADD		0x00
#define		BPF_SUB		0x10
#define		BPF_MUL		0x20
#define		BPF_DIV		0x30
#define		BPF_OR		0x40
#define		BPF_AND		0x50
#define		BPF_LSH		0x60
#define		BPF_RSH		0x70
#define		BPF_NEG		0x80
#define		BPF_JA		0x00
#define		BPF_JEQ		0x10
#define		BPF_JGT		0x20
#define		BPF_JGE		0x30
#define		BPF_JSET	0x40
#define BPF_SRC(code)	((code) & 0x08)
#define		BPF_K		0x00
#define		BPF_X		0x08
#define BPF_RVAL(code)	((code) & 0x18)
#define		BPF_A		0x10
#define BPF_MISCOP(code) ((code) & 0xf8)
#define		BPF_TAX		0x00
#define		BPF_TXA		0x80
struct bpf_insn {
	u_short	code;
	u_char 	jt;
	u_char 	jf;
	bpf_int32 k;
};
#define BPF_STMT(code, k) { (u_short)(code), 0, 0, k }
#define BPF_JUMP(code, k, jt, jf) { (u_short)(code), jt, jf, k }
#if defined(BSD) && (defined(KERNEL) || defined(_KERNEL))
# if __STDC__
extern void bpf_tap(struct ifnet *, u_char *, u_int);
extern void bpf_mtap(struct ifnet *, struct mbuf *);
extern void bpfattach(struct ifnet *, u_int, u_int);
extern void bpfilterattach(int);
# else
extern void bpf_tap();
extern void bpf_mtap();
extern void bpfattach();
extern void bpfilterattach();
# endif 
#endif 
#if __STDC__
extern int bpf_validate(struct bpf_insn *, int);
extern u_int bpf_filter(struct bpf_insn *, u_char *, u_int, u_int);
#else
extern int bpf_validate();
extern u_int bpf_filter();
#endif
#define BPF_MEMWORDS 16
#endif
