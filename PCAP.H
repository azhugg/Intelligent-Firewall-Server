#ifndef lib_pcap_h
#define lib_pcap_h

#include <sys/types.h>
#ifdef WIN32
#include <sys/timeb.h>
#include <time.h>
#else
#include <sys/time.h>
#endif

#include "bpf.h"
#include <stdio.h>
#ifdef __cplusplus
extern "C" {
#endif

#define PCAP_VERSION_MAJOR 2
#define PCAP_VERSION_MINOR 4

#define PCAP_ERRBUF_SIZE 256

#if BPF_RELEASE - 0 < 199406
typedef	int bpf_int32;
typedef	u_int bpf_u_int32;
#endif

typedef struct pcap pcap_t;
typedef struct pcap_dumper pcap_dumper_t;


struct pcap_file_header {
	bpf_u_int32 magic;
	u_short version_major;
	u_short version_minor;
	bpf_int32 thiszone;
	bpf_u_int32 sigfigs;	
	bpf_u_int32 snaplen;	
	bpf_u_int32 linktype;	
};

struct pcap_pkthdr {
	struct timeval ts;	
	bpf_u_int32 caplen;	
	bpf_u_int32 len;	
};

struct pcap_stat {
	u_int ps_recv;
	u_int ps_drop;
	u_int ps_ifdrop;
};
typedef void (*pcap_handler)(u_char *, const struct pcap_pkthdr *,
			     const u_char *);
char	*pcap_lookupdev(char *);
int	pcap_lookupnet(char *, bpf_u_int32 *, bpf_u_int32 *, char *);
pcap_t	*pcap_open_live(char *, int, int, int, char *);
pcap_t	*pcap_open_dead(int, int);
pcap_t	*pcap_open_offline(const char *, char *);
void	pcap_close(pcap_t *);
int	pcap_loop(pcap_t *, int, pcap_handler, u_char *);
int	pcap_dispatch(pcap_t *, int, pcap_handler, u_char *);
const u_char*
pcap_next(pcap_t *, struct pcap_pkthdr *);
int	pcap_stats(pcap_t *, struct pcap_stat *);
int	pcap_setfilter(pcap_t *, struct bpf_program *);
void	pcap_perror(pcap_t *, char *);
char	*pcap_strerror(int);
char	*pcap_geterr(pcap_t *);
int	pcap_compile(pcap_t *, struct bpf_program *, char *, int,bpf_u_int32);
int	pcap_compile_nopcap(int, int, struct bpf_program *,
char *, int, bpf_u_int32);
void	pcap_freecode(struct bpf_program *);
int	pcap_datalink(pcap_t *);
int	pcap_snapshot(pcap_t *);
int	pcap_is_swapped(pcap_t *);
int	pcap_major_version(pcap_t *);
int	pcap_minor_version(pcap_t *);
FILE	*pcap_file(pcap_t *);
int	pcap_fileno(pcap_t *);
pcap_dumper_t *pcap_dump_open(pcap_t *, const char *);
void	pcap_dump_close(pcap_dumper_t *);
void	pcap_dump(u_char *, const struct pcap_pkthdr *, const u_char *);
u_int	bpf_filter(struct bpf_insn *, u_char *, u_int, u_int);
int	bpf_validate(struct bpf_insn *f, int len);
char	*bpf_image(struct bpf_insn *, int);
void	bpf_dump(struct bpf_program *, int);
#ifdef WIN32
int pcap_setbuff(pcap_t *p, int dim);
int pcap_setmode(pcap_t *p, int mode);
int pcap_sendpacket(pcap_t *p, u_char *buf, int size);
int pcap_setmintocopy(pcap_t *p, int size);
HANDLE pcap_getevent(pcap_t *p);
#define MODE_CAPT 0
#define MODE_STAT 1
#endif 
#ifdef __cplusplus
}
#endif

#endif
