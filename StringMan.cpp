// StringMan.cpp: implementation of the CStringMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "StringMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CStringMan::CStringMan()
{

}

CStringMan::~CStringMan()
{

}

char** CStringMan::mSplit(char *str, char *sep, int max_strs, int *toks, char meta)
{
	char** retstr;		
	char* idx;			
	char* end;			
	char* sep_end;		
	char* sep_idx;		
	int len = 0;		
	int curr_str = 0;	
	char last_char = 0xFF;
		sep_end = sep + strlen(sep);
		end = str + strlen(str);
    while(isspace((int)*(end-1)) && ((end-1) >= str)) *(--end) = '\0'; /* -1 because of NULL */
    sep_idx = sep;
    idx = str;
    retstr = (char **) malloc((sizeof(char **) * max_strs));
    max_strs--;
    while (idx < end){
        while (sep_idx < sep_end){
            if ((*idx == *sep_idx) && (last_char != meta)){
                if (len > 0){
                    if(curr_str <= max_strs){
                        retstr[curr_str] = (char *) malloc((sizeof(char) * len) + 1);
                        if (retstr[curr_str] == NULL){
                            fprintf(stderr, "msplit() got NULL substring malloc!\n");
                            exit(1);
                        }
                        memcpy(retstr[curr_str], (idx - len), len);
                        retstr[curr_str][len] = 0; 
                        len = 0;
                        curr_str++;
						last_char = *idx;
                        idx++;
                    }
                    if (curr_str >= max_strs){
                       while(isspace((int)*idx)) idx++;
                       len = end - idx;
                       retstr[curr_str] = (char *) malloc((sizeof(char) * len) + 1);
                       if (retstr[curr_str] == NULL)
                          printf("Got NULL back from substr malloc\n");
                       memcpy(retstr[curr_str], idx, len);
                       retstr[curr_str][len] = 0; 
					   *toks = curr_str + 1;
					   return retstr;
                    }
                }else{
                    last_char = *idx;
                    idx++;
                    sep_idx = sep;
                    len = 0;
                }
            }else{
                sep_idx++;
            }
        }

        sep_idx = sep;
        len++;
        last_char = *idx;
        idx++;
    }
    if (len > 0){
		retstr[curr_str] = (char *) malloc((sizeof(char) * len) + 1);
        if (retstr[curr_str] == NULL)
            printf("Got NULL back from substr malloc\n");
        memcpy(retstr[curr_str], (idx - len), len);
        retstr[curr_str][len] = 0; 
		*toks = curr_str + 1;
    }return retstr;
}

int CStringMan::mSearch(char *buf, int blen, char *ptrn, int plen, int *skip, int *shift)
{
    int b_idx = plen;
    if (plen == 0) return 1;
    while (b_idx <= blen){
        int p_idx = plen, skip_stride, shift_stride;
        while (buf[--b_idx] == ptrn[--p_idx]){
            if (p_idx == 0){
                return 1;
            }
        }
        skip_stride = skip[(unsigned char)buf[b_idx]];
        shift_stride = shift[p_idx];
        b_idx += (skip_stride > shift_stride) ? skip_stride : shift_stride;
    }
    return 0;
}

int* CStringMan::MakeSkip(char *ptrn, int plen)
{
	int *skip = (int *)malloc(256 * sizeof(int));
    int *sptr= &skip[256];
    while (sptr-- != skip) *sptr = plen + 1;
    while (plen != 0) skip[(unsigned char)*ptrn++] = plen--;
    return skip;
}

int* CStringMan::MakeShift(char *ptrn, int plen)
{
	int *shift = (int *)malloc(plen * sizeof(int));
    int *sptr= shift + plen - 1;
    char *pptr = ptrn + plen - 1;
    char c = ptrn[plen - 1];
    *sptr = 1;
    while (sptr-- != shift){
        char *p1 = ptrn + plen - 2, *p2, *p3;
		do{
            while (p1 >= ptrn && *p1-- != c);
			p2 = ptrn + plen - 2;
            p3 = p1;
            while (p3 >= ptrn && *p3-- == *p2-- && p2 >= pptr);
        }
        while (p3 >= ptrn && p2 >= pptr);
        *sptr = shift + plen - sptr + p2 - p3;
        pptr--;
    }
    return shift;
}

int CStringMan::mSearchCI(char *buf, int blen, char *ptrn, int plen, int *skip, int *shift)
{
	    int b_idx = plen;

    if (plen == 0) return 1;
    while (b_idx <= blen){
        int p_idx = plen, skip_stride, shift_stride;
        while ((unsigned char)ptrn[--p_idx] == toupper((unsigned char)buf[--b_idx])){
            if (p_idx == 0){
                return 1;
            }
        }
        skip_stride = skip[toupper((unsigned char)buf[b_idx])];
        shift_stride = shift[p_idx];
        b_idx += (skip_stride > shift_stride) ? skip_stride : shift_stride;
    }
    return 0;
}
