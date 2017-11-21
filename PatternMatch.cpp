// PatternMatch.cpp: implementation of the CPatternMatch class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "PatternMatch.h"
#include "Preprocessor.h"
#include "Rules.h"
#include "StringMan.h"
#include "Manipulate.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPatternMatch::CPatternMatch()
{

}

CPatternMatch::~CPatternMatch()
{

}

void CPatternMatch::PayloadSearchInit(char *data, OptTreeNode *otn, int protocol)
{
    NewNode(otn);
    ParsePattern(data, otn);
	int (CManipulate::*ptrfun)(Packet *p,struct _OptTreeNode *otn_idx,struct _OptFpList *fp_list);
	ptrfun = &CManipulate::CheckPatternMatch;
	CRules objRule;
	objRule.AddOptFuncToList(ptrfun, otn);
}

int CPatternMatch::CheckPatternMatch(Packet *p,struct _OptTreeNode *otn_idx, OptFpList *fp_list)
{
    int sub_depth = NULL;
    int found = 0;
    PatternMatchData *idx;
    int (CManipulate::*ptrfun)(char *, int, char *, int, int *, int *);
	CManipulate objManip;
    idx =(PatternMatchData*) otn_idx->ds_list[PLUGIN_PATTERN_MATCH];
	while (idx != NULL){
        if (idx->offset > p->dsize){
            return 0;
        }else{
            if ((idx->depth + idx->offset) > p->dsize){
                sub_depth = p->dsize - idx->offset;
                if ((unsigned int)sub_depth >= idx->pattern_size){
					ptrfun = idx->search; 
					found = (objManip.*ptrfun)((char *)(p->data+idx->offset), sub_depth,idx->pattern_buf, idx->pattern_size, idx->skip_stride, idx->shift_stride);
                    if (!found){
                        return 0;
                    }
                }
            }else{
                if (idx->depth){
					ptrfun = idx->search; 
					found = (objManip.*ptrfun)((char *)(p->data+idx->offset), idx->depth,idx->pattern_buf, idx->pattern_size, idx->skip_stride, idx->shift_stride);
                }else{
					ptrfun = idx->search; 
					found = (objManip.*ptrfun)((char *)(p->data+idx->offset), p->dsize,idx->pattern_buf, idx->pattern_size, idx->skip_stride, idx->shift_stride);
                }
                if (!found){
                    return 0;
                }
            }
        }
        idx = idx->next;
    }
    if (found){
		CManipulate objManip;
		int (CManipulate::*ptrfun)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
		ptrfun = fp_list->next->OptTestFunc;
        return (objManip.*ptrfun)(p, otn_idx, fp_list->next);
    }
	return 0;
}

void CPatternMatch::ParsePattern(char *rule, OptTreeNode *otn)
{
	char tmp_buf[2048];
    char *start_ptr;
    char *end_ptr;
    char *idx;
    char *dummy_idx;
    char *dummy_end;
    char hex_buf[9];
    u_int dummy_size = 0;
    u_int size;
    int hexmode = 0;
    int hexsize = 0;
    int pending = 0;
    int cnt = 0;
    int literal = 0;
    PatternMatchData *ds_idx;
    bzero(tmp_buf, 2048);
    start_ptr = index(rule,'"');

    if (start_ptr == NULL){
//        FatalError( "ERROR Line %d => Content data needs to be enclosed in quotation marks (\")!\n", file_line);
    }
    start_ptr++;
    end_ptr = strrchr(start_ptr, '"');
    if (end_ptr == NULL){
//        FatalError( "ERROR Line %d => Content data needs to be enclosed in quotation marks (\")!\n", file_line);
    }
    *end_ptr = 0;
    size = end_ptr - start_ptr;
    if (size <= 0){
//        FatalError( "ERROR Line %d => Bad pattern length!\n", file_line);
    }    idx = start_ptr;

    dummy_idx = tmp_buf;
    dummy_end = (dummy_idx + size);
	bzero(hex_buf, 9);
    memset(hex_buf, '0', 8);

    while (idx < end_ptr){
        switch (*idx){
            case '|':
                if (!literal){
                    if (!hexmode){
                        hexmode = 1;
                    }else{
                        hexmode = 0;
                    }
                   if (hexmode)
                        hexsize = 0;
                }else{
                    literal = 0;
                    tmp_buf[dummy_size] = start_ptr[cnt];
                    dummy_size++;
                }

                break;

            case '\\':
                if (!literal){
                    literal = 1;
                }else{
					tmp_buf[dummy_size] = start_ptr[cnt];
                    literal = 0;
                    dummy_size++;
                }

                break;

            default:
                if (hexmode){
                    if (isxdigit((int)*idx)){
                        hexsize++;
                        if (!pending){
                            hex_buf[7] = *idx;
                            pending++;
                        }else{
                            hex_buf[8] = *idx;
                            pending--;

                            if (dummy_idx < dummy_end){
                                tmp_buf[dummy_size] = (u_long) strtol(hex_buf, (char **)NULL, 16);

                                dummy_size++;
                                bzero(hex_buf, 9);
                                memset(hex_buf, '0', 8);
                            }else{
//                                FatalError( "ERROR => ParsePattern() dummy buffer overflow, make a smaller pattern please! (Max size = 2048)\n");
                            }
                        }
                    }else{
                        if (*idx != ' '){
//                            FatalError( "ERROR Line %d => What is this \"%c\"(0x%X) doing in your binary buffer?  Valid hex values only please! (0x0 - 0xF) Position: %d\n", file_line, (char) *idx, (char) *idx, cnt);
                        }
                    }
                }else{
                    if (*idx >= 0x1F && *idx <= 0x7e){
                        if (dummy_idx < dummy_end){
                            tmp_buf[dummy_size] = start_ptr[cnt];
                            dummy_size++;
                        }else{
//                            FatalError( "ERROR Line %d=> ParsePattern() dummy buffer overflow!\n", file_line);
                        }
                        if (literal){
                            literal = 0;
                        }
                    }else{
                        if (literal){
                            tmp_buf[dummy_size] = start_ptr[cnt];
                            dummy_size++;
                            literal = 0;
                        }
                        else
                        {
//                            FatalError( "ERROR Line %d=> character value out of range, try a binary buffer dude\n", file_line);
                        }
                    }
                }

                break;
        }

        dummy_idx++;
        idx++;
        cnt++;
    }

    ds_idx = (PatternMatchData *) otn->ds_list[PLUGIN_PATTERN_MATCH];

    while (ds_idx->next != NULL)
        ds_idx = ds_idx->next;

    if ((ds_idx->pattern_buf=(char *)malloc(sizeof(char)*dummy_size))==NULL)
    {
//        FatalError( "ERROR => ParsePattern() pattern_buf malloc filed!\n");
    }

    memcpy(ds_idx->pattern_buf, tmp_buf, dummy_size);

    ds_idx->pattern_size = dummy_size;
	CStringMan objString;
	ds_idx->search = &CManipulate::mSearch;	



    ds_idx->skip_stride = objString.MakeSkip(ds_idx->pattern_buf, ds_idx->pattern_size);
    ds_idx->shift_stride = objString.MakeShift(ds_idx->pattern_buf, ds_idx->pattern_size);

    return;
}

void CPatternMatch::NewNode(OptTreeNode *otn)
{
	PatternMatchData *idx;


    idx = (PatternMatchData *) otn->ds_list[PLUGIN_PATTERN_MATCH];

    if (idx == NULL)
    {
        if ((otn->ds_list[PLUGIN_PATTERN_MATCH] = (PatternMatchData *) calloc(sizeof(PatternMatchData), sizeof(char))) == NULL)
        {
//            FatalError( "ERROR => sp_pattern_match NewNode() calloc failed!\n");
        }
    }
    else
    {
        idx = (PatternMatchData*) otn->ds_list[PLUGIN_PATTERN_MATCH];

        while (idx->next != NULL)
            idx = idx->next;

        if ((idx->next = (PatternMatchData *) calloc(sizeof(PatternMatchData), sizeof(char))) == NULL)
        {
//            FatalError( "ERROR => sp_pattern_match NewNode() calloc failed!\n");
        }
    }
}

void CPatternMatch::SetupPatternMatch()
{
	CPreprocessor *objPrep;
	objPrep = new CPreprocessor;
	void (CManipulate::*ptrfun)(char *data, OptTreeNode *otn, int protocol);
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
	ptrMainApp->KeywordList = NULL;
	ptrfun = &CManipulate::PayloadSearchInit;
	objPrep->RegisterPlugin("content",ptrfun);
	ptrfun = &CManipulate::PayloadSearchOffset;
	objPrep->RegisterPlugin("offset",ptrfun);
	ptrfun = &CManipulate::PayloadSearchDepth;
	objPrep->RegisterPlugin("depth",ptrfun);
	ptrfun = &CManipulate::PayloadSearchNocase;
	objPrep->RegisterPlugin("nocase",ptrfun);
}

void CPatternMatch::PayloadSearchOffset(char *data, OptTreeNode *otn, int protocol)
{
	    PatternMatchData *idx;
    idx =(PatternMatchData*) otn->ds_list[PLUGIN_PATTERN_MATCH];

    if (idx == NULL)
    {
//        FatalError( "ERROR Line %d => Please place \"content\" rules before depth, nocase or offset modifiers.\n", file_line);
    }

    while (idx->next != NULL)
        idx = idx->next;

    while (isspace((int)*data)) data++;
    idx->offset = atoi(data);
    return;
}

void CPatternMatch::PayloadSearchDepth(char *data, OptTreeNode *otn, int protocol)
{
	PatternMatchData *idx;

    idx = (PatternMatchData*) otn->ds_list[PLUGIN_PATTERN_MATCH];

    if (idx == NULL)
    {
//        FatalError( "ERROR Line %d => Please place \"content\" rules before depth, nocase or offset modifiers.\n", file_line);
    }

    while (idx->next != NULL)
        idx = idx->next;

    while (isspace((int)*data)) data++;
    idx->depth = atoi(data);
    return;
}

void CPatternMatch::PayloadSearchNocase(char *data, OptTreeNode *otn, int protocol)
{
	PatternMatchData *idx;
    int i;
    idx = (PatternMatchData*) otn->ds_list[PLUGIN_PATTERN_MATCH];
    if (idx == NULL)
    {
//        FatalError( "ERROR Line %d => Please place \"content\" rules before depth, nocase or offset modifiers.\n", file_line);
    }
    while (idx->next != NULL)
        idx = idx->next;

	CStringMan *objString;
	objString = new CStringMan;
	idx->search = &CManipulate::mSearchCI;
    i = idx->pattern_size;
    while (--i >= 0) idx->pattern_buf[i] = toupper((unsigned char)idx->pattern_buf[i]);
    free(idx->skip_stride);
    idx->skip_stride = objString->MakeSkip(idx->pattern_buf, idx->pattern_size);
    free(idx->shift_stride);
    idx->shift_stride = objString->MakeShift(idx->pattern_buf, idx->pattern_size);
    return;
}
