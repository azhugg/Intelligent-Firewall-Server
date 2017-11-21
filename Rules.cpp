// Rules.cpp: implementation of the CRules class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "IFServer.h"
#include "Rules.h"
#include "StringMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CRules::CRules()
{
}

CRules::~CRules()
{

}

void CRules::AddFuncToPreprocList(void (CManipulate::*func)(Packet *))
{
	PreprocessFuncNode *idx;
	static void (CManipulate::*ptrfun)(Packet *);
	ptrfun = func;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	idx = ptrMainApp->PreprocessList;
	if(idx == NULL){
		ptrMainApp->PreprocessList  = (PreprocessFuncNode *) calloc(sizeof(PreprocessFuncNode),sizeof(char));
		ptrMainApp->PreprocessList->func = ptrfun;
	}else{
        while (idx->next != NULL)
            idx = idx->next;
        idx->next = (PreprocessFuncNode *) calloc(sizeof(PreprocessFuncNode), sizeof(char));
        idx = idx->next;
        idx->func = ptrfun;
    }
    return;
}

void CRules::CallAlertPlugins(Packet *p, char *message)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	OutputFuncNode *idx;
	CManipulate objManip;
    idx = ptrMainApp->AlertList;
    void (CManipulate::*ptrfun)(Packet*, char*, void*);
	while (idx != NULL){
        ptrfun = idx->func;
		(objManip.*ptrfun)(p, message, idx->arg);
        idx = idx->next;
    } 
    return;
}

void CRules::CallLogPlugins(Packet *p, char *message)
{
	OutputFuncNode *idx;
	CManipulate objManip;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	void (CManipulate::*ptrfun)(Packet*, char*, void*);
    idx = ptrMainApp->LogList;
    while (idx != NULL){
		ptrfun = idx->func;
		(objManip.*ptrfun)(p, message, idx->arg);
        idx = idx->next;
    } 
    return;
}

int CRules::CheckAddrPort(u_long addr, u_long mask, u_short hi_port, u_short lo_port, Packet *p, char flags, int mode)
{
    u_long  pkt_addr;
    u_short pkt_port;
    int     any_port_flag = 0;
    int     except_addr_flag = 0;
    int     except_port_flag = 0;

    if ((mode & CHECK_SRC)== CHECK_SRC){
        pkt_addr = p->iph->ip_src.s_addr;
        pkt_port = p->sp;
        if ((mode & INVERSE)==INVERSE){
            if (flags & EXCEPT_DST_IP){
                except_addr_flag = 1;
            }
            if (flags & ANY_DST_PORT){
                any_port_flag = 1;
            }
            if (flags & EXCEPT_DST_PORT){
                except_port_flag = 1;
            }
        }else{
            if (flags & EXCEPT_SRC_IP){
                except_addr_flag = 1;
            }
            if (flags & ANY_SRC_PORT){
                any_port_flag = 1;
            }
            if (flags & EXCEPT_SRC_PORT){
                except_port_flag = 1;
            }
        }
    }else{
        pkt_addr = p->iph->ip_dst.s_addr;
        pkt_port = p->dp;
        if ((mode & INVERSE)==INVERSE){
            if (flags & EXCEPT_SRC_IP){
                except_addr_flag = 1;
            }
            if (flags & ANY_SRC_PORT){
                any_port_flag = 1;
            }
            if (flags & EXCEPT_SRC_PORT){
                except_port_flag = 1;
            }
        }else{
            if (flags & EXCEPT_DST_IP){
                except_addr_flag = 1;
            }
            if (flags & ANY_DST_PORT){
                any_port_flag = 1;
            }
            if (flags & EXCEPT_DST_PORT){
                except_port_flag = 1;
            }
        }
    }
    if (!((addr == (pkt_addr & mask))^ (except_addr_flag))){
        return 0;
    }
    if (any_port_flag)
        return 1;
    if ((pkt_port > hi_port) || (pkt_port < lo_port)){
        if (!except_port_flag){
            return 0;
        }
    }else{
        if (except_port_flag){
            return 0;
        }
    }
    return 1;
}


int CRules::ParseIP(char *paddr, u_long *ip_addr, u_long *netmask)
{
	char **toks;      
    int num_toks;     
    int nmask;        
    char *addr;       
    struct hostent *host_info;  
    struct sockaddr_in sin;     
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	addr = new char[STD_BUF];
	host_info = new hostent;
    if (! strncmp(paddr, "$", 1)){
        if ((addr = VarGet(paddr + 1)) == NULL){
//          FatalError( "ERROR %s (%d) => Undefined variable %s\n", file_name, file_line, paddr);
        }
    }else
        addr = paddr;
    if (!strncasecmp(addr, "any", 3)){
        *ip_addr = 0;
        *netmask = 0;
        return 1;
    }
    CStringMan objstring;
	toks = objstring.mSplit(addr,"/",2,&num_toks,0);
    switch (num_toks){
        case 1:
            *netmask = ptrMainApp->netmasks[32];
            break;
        case 2:
            nmask = atoi(toks[1]);
            if (!isdigit((int)toks[1][0])) nmask=-1;
            if ((nmask > -1)&&(nmask < 33)){
                *netmask = ptrMainApp->netmasks[nmask];
            }else{
//                FatalError("ERROR %s (%d) => Invalid CIDR block for IP addr %s\n", file_name, file_line, addr);
            }
            break;

        default:
//            FatalError("ERROR %s (%d) => No netmask specified for IP address %s\n", file_name, file_line, addr);
            break;
    }

#ifndef WORDS_BIGENDIAN
    *netmask = htonl(*netmask);
#endif
    if (isalpha((int)toks[0][0])){
        if ((host_info = gethostbyname(toks[0]))){
            bcopy(host_info->h_addr, (char *)&sin.sin_addr, host_info->h_length);
        }else if ((sin.sin_addr.s_addr = inet_addr(toks[0])) == INADDR_NONE){
//            FatalError("ERROR %s (%d) => Couldn't resolve hostname %s\n",file_name, file_line, toks[0]);
        }
       *ip_addr = ((u_long)(sin.sin_addr.s_addr) & (*netmask));
        return 1;
    }
    if (! strncmp (toks[0], "255.255.255.255", 15)){
        *ip_addr = INADDR_BROADCAST;
    }else if ((*ip_addr = inet_addr(toks[0])) == -1){
//        FatalError( "ERROR %s (%d) => Rule IP addr (%s) didn't x-late, WTF?\n", file_name, file_line, toks[0]);
    }else{
        *ip_addr = ((u_long)(*ip_addr) & (*netmask));
    }
	free(toks);
    return 0;
}

char* CRules::VarGet(char *name)
{
	struct VarEntry *p;
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
    if (!ptrMainApp->VarHead)
        return(NULL);
    p = ptrMainApp->VarHead;
    do{
        if (strcasecmp(p->name, name) == 0)
            return(p->value);
        p = p->next;
    } while (p != ptrMainApp->VarHead);
    return(NULL);
}

void CRules::Preprocess(Packet *p)
{	
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	PreprocessFuncNode *idx;
    ptrMainApp->do_detect = 1;
    idx = ptrMainApp->PreprocessList;
	void (CManipulate::*ptrfun)(Packet*);
	CManipulate objManip;
    while (idx != NULL){
        ptrfun = idx->func;
		(objManip.*ptrfun)(p);
        idx = idx->next;
    }
    if (!p->frag_flag && ptrMainApp->do_detect){
        Detect(p);
    }
}

int CRules::Detect(Packet *p)
{

	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    if (!ptrMainApp->pv.rules_order_flag){
        if (!EvalPacket(ptrMainApp->RuleAlert, RULE_ALERT, p)){
            if (!EvalPacket(ptrMainApp->RulePass, RULE_PASS, p)){
                if (EvalPacket(ptrMainApp->RuleLog, RULE_LOG, p))
                    return 1;
            }else{
                return 0;
            }
        }else{
            return 1;
        }
    }else{
		if (!EvalPacket(ptrMainApp->RulePass, RULE_PASS, p)){
            if (!EvalPacket(ptrMainApp->RuleAlert, RULE_ALERT, p)){
                if (EvalPacket(ptrMainApp->RuleLog, RULE_LOG, p))
                    return 1;
            }else{
                return 1;
            }
        }
    }
    return 0;
}

int CRules::EvalPacket(ListHead *List, int mode, Packet *p)
{
	RuleTreeNode *rtn_idx;
    if (p->iph == NULL){
        return 0;
    }
    switch (p->iph->ip_proto){
        case IPPROTO_TCP:
            rtn_idx = List->TcpList;
            break;
        case IPPROTO_UDP:
            rtn_idx = List->UdpList;
            break;
        case IPPROTO_ICMP:
            rtn_idx = List->IcmpList;
            break;
        default: 
            rtn_idx = NULL;
            break;
    }
    return EvalHeader(rtn_idx, p);
}

int CRules::EvalHeader(RuleTreeNode *rtn_idx, Packet *p)
{
	CManipulate objManip;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	void (CManipulate::*ptrfun)(Packet*, char*);
	int rule_match = 0;
    if (rtn_idx == NULL){
        return 0;
    }
	int (CManipulate::*ptrfun2)(Packet *, struct _RuleTreeNode *,struct _RuleFpList *);
	ptrfun2 = rtn_idx->rule_func->RuleHeadFunc;
    if (!(objManip.*ptrfun2)(p, rtn_idx, rtn_idx->rule_func)){
        return EvalHeader(rtn_idx->right, p);
    }else{
        rule_match = EvalOpts(rtn_idx->down, p);
        if (rule_match){
            switch (rtn_idx->type){
                case RULE_PASS: 
                    return 1;
                case RULE_ALERT: 
					ptrfun = ptrMainApp->LogFunc;
			        (objManip.*ptrfun)(p, ptrMainApp->otn_tmp->message);
#ifdef ENABLE_RESPONSE
					CRespond objRespond;
                    objRespond.Respond(p);
#endif
                    return 1;
                case RULE_LOG: 
					ptrfun = ptrMainApp->LogFunc;
			       (objManip.*ptrfun)(p, ptrMainApp->otn_tmp->message);
                    return 1;
            }
        }
        if (!rule_match){
            return EvalHeader(rtn_idx->right, p);
        }
    }
    return 0;
}

int CRules::EvalOpts(OptTreeNode *List, Packet *p)
{
	CManipulate objManip;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	void (CManipulate::*ptrfun)(Packet*, char*);
	if (List == NULL)
        return 0;
    if (List->opt_func == NULL){
//        FatalError( "List->opt_func was NULL on option #%d!\n", List->chain_node_number);
    }
	int (CManipulate::*ptrfun2)(Packet *, struct _OptTreeNode *, struct _OptFpList *);
	ptrfun2 = List->opt_func->OptTestFunc;
    if (!(objManip.*ptrfun2)(p, List, List->opt_func)){
        if (EvalOpts(List->next, p))
            return 1;
        else
            return 0;
    }else{
        switch (List->type){
            case RULE_PASS: 
                return 1;
            case RULE_ALERT: 
                ptrMainApp->otn_tmp = List;
				ptrfun = ptrMainApp->AlertFunc;
			    (objManip.*ptrfun)(p, List->message);
                return 1;
            case RULE_LOG: 
                ptrMainApp->otn_tmp = List;
				ptrfun = ptrMainApp->LogFunc;
				(objManip.*ptrfun)(p, List->message);
                return 1;
        }
    }
   return  0;
}

void CRules::AddOptFuncToList(int (CManipulate::*func)(Packet *,struct _OptTreeNode *,struct _OptFpList *), OptTreeNode *otn)
{
	OptFpList *idx;
    idx = otn->opt_func;
    if (idx == NULL){
        otn->opt_func = (OptFpList *) calloc(sizeof(OptFpList), sizeof(char));
        if (otn->opt_func == NULL){
//            FatalError( "ERROR => AddOptFuncToList new node calloc failed: %s\n", strerror(errno));
        }
        otn->opt_func->OptTestFunc = func; 
    }else{
        while (idx->next != NULL){
            idx = idx->next;
        }
        idx->next = (OptFpList *) calloc(sizeof(OptFpList), sizeof(char));
        if (idx->next == NULL){
//            FatalError( "ERROR => AddOptFuncToList new node calloc failed: %s\n", strerror(errno));
        }
        idx = idx->next;
        idx->OptTestFunc = func;
    }
}

void CRules::AddFuncToOutputList(void (CManipulate::*func)(Packet *,char *,void *), char node_type, void *arg)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	OutputFuncNode *idx; 
	idx = (OutputFuncNode*)calloc(sizeof(OutputFuncNode),sizeof(char));
	idx = NULL;
    switch (node_type){
        case NT_OUTPUT_ALERT:
            if (ptrMainApp->AlertList == NULL){
                ptrMainApp->AlertList = (OutputFuncNode *) calloc(sizeof(OutputFuncNode), sizeof(char));
                ptrMainApp->AlertList->func = func;
                return;
            }else{
                idx = ptrMainApp->AlertList;
            }
            break;
        case NT_OUTPUT_LOG:
            if (ptrMainApp->LogList == NULL){
                ptrMainApp->LogList = (OutputFuncNode *) calloc(sizeof(OutputFuncNode), sizeof(char));
                ptrMainApp->LogList->func = func;
                return;
            }else{
                idx = ptrMainApp->LogList;
            }
            break;
        default:
			char temp[12] = "Avoid Error";
    }

    while (idx->next != NULL){
        idx = idx->next;
    }
    idx->next = (OutputFuncNode *) calloc(sizeof(OutputFuncNode), sizeof(char));
    idx = idx->next;
    idx->func = func;
    return;
}

char* CRules::ProcessFileOption(char *filespec)
{
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	   char *filename;
       char buffer[STD_BUF];
       if(strstr(filespec, "..") != NULL){
//               FatalError( "ERROR: file definition contains \"..\".  Do not do that!\n");
       }
       if(filespec[0] == '/'){
               filename = strdup(filespec);
       }else{
               if(ptrMainApp->pv.log_flag) {
                       strncpy(buffer, ptrMainApp->pv.log_dir, STD_BUF-1);
               }else{
                       strncpy(buffer, "/var/log", STD_BUF-1);
               }
               strncat(buffer, "/", STD_BUF-strlen(buffer)-1);
               strncat(buffer, filespec, STD_BUF-strlen(buffer)-1);
               filename = strdup(buffer);
          }
       return filename;
}

void CRules::ParseRulesFile(char *file, int inclevel)
{
	FILE *thefp;       
    char *buf; 
    char *index;       
    CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	char *stored_file_name = ptrMainApp->file_name;
    int  stored_file_line = ptrMainApp->file_line;
	char *Index;
	buf = new char[STD_BUF];
	Index = new char[STD_BUF];
    if (inclevel == 0){
        ptrMainApp->file_name = strdup(file);
    }
    if ((thefp = fopen(file,"r")) == NULL){
//        FatalError( "ERROR: Unable to open rules file: %s\n", file);
    }
    bzero((char *)buf, STD_BUF);
    stored_file_line = ptrMainApp->file_line;
    stored_file_name = ptrMainApp->file_name;
    ptrMainApp->file_name = strdup(file);
    ptrMainApp->file_line = 0;
    while ((fgets(buf, STD_BUF, thefp)) != NULL){
        ptrMainApp->file_line++;
        Index = buf;
        if ((*Index != '#') && (*Index!= 0x0a) && (*Index!= ';') && (Index != NULL)){
            while (isspace((int) *Index)) Index++;
            ParseRule(Index, inclevel);
        }
        bzero((char *)buf, STD_BUF);
    }
    if (ptrMainApp->file_name)
        free(ptrMainApp->file_name);
    ptrMainApp->file_name = stored_file_name;
    ptrMainApp->file_line = stored_file_line;
      fclose(thefp);
    return;
}

void CRules::IntegrityCheck(RuleTreeNode *rtn_head, char *listname)
{
	RuleTreeNode *rtn_idx = NULL;
    OptTreeNode *otn_idx;
    OptFpList   *ofl_idx;
    int opt_func_count;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    if ( !ptrMainApp->pv.quiet_flag )
    if (rtn_head == NULL){
        if ( !ptrMainApp->pv.quiet_flag )
        return;
    }
    rtn_idx = rtn_head;
    while (rtn_idx != NULL){
        otn_idx = rtn_idx->down;
        while (otn_idx != NULL){
            ofl_idx = otn_idx->opt_func;
            opt_func_count = 0;
            while (ofl_idx != NULL){
                opt_func_count++;
                ofl_idx = ofl_idx->next;
            }
			if (opt_func_count == 0)
            {
                exit(1);
            }
            otn_idx = otn_idx->next;
        }

        rtn_idx = rtn_idx->right;
    }

    if ( !ptrMainApp->pv.quiet_flag );
}

void CRules::DumpChain(RuleTreeNode *rtn_head, char *name)
{
	RuleTreeNode *rtn_idx;
    OptTreeNode *otn_idx;
    rtn_idx = rtn_head;
    if (rtn_idx == NULL)

    while (rtn_idx != NULL){
       otn_idx = rtn_idx->down;
        while (otn_idx != NULL){
            otn_idx = otn_idx->next;
        }
        rtn_idx = rtn_idx->right;
    }
}

void CRules::ParseRule(char *prule, int inclevel)
{
	char **toks;          
    int num_toks;         
    int rule_type;        
    char rule[1024];
    int protocol;
    RuleTreeNode proto_node;
	CStringMan *objString;
	objString = new CStringMan;
	CRules *objRule;
	objRule = new CRules;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    bzero((char *)&proto_node, sizeof(RuleTreeNode));
    ptrMainApp->Strip(prule);
    strcpy(rule, ExpandVars(prule));
    toks = objString->mSplit(rule, " ", 10, &num_toks,0);
    rule_type = RuleType(toks[0]);
    proto_node.type = rule_type;
    switch (rule_type){
        case RULE_PASS:
            break;
        case RULE_LOG:
            break;
        case RULE_ALERT:
            break;
        case RULE_INCLUDE:
            ParseRulesFile(toks[1], inclevel + 1);
            return;
        case RULE_VAR:
            VarDefine(toks[1], toks[2]);
            return;
        case RULE_PREPROCESS:
            ParsePreprocessor(rule);
            return;
        case RULE_OUTPUT:
            ParseOutputPlugin(rule);
            return;
    }
    protocol = WhichProto(toks[1]);
    if (*toks[2]=='!'){
        proto_node.flags =(proto_node.flags | EXCEPT_SRC_IP);
        ParseIP(&toks[2][1], (u_long *) &proto_node.sip,
                (u_long *) &proto_node.smask);
    }else{
        ParseIP(toks[2], (u_long *) &proto_node.sip,
                (u_long *) &proto_node.smask);
    }
    if (ParsePort(toks[3], (u_short *) &proto_node.hsp, 
                  (u_short *) &proto_node.lsp, toks[1], 
                  (int *) &proto_node.not_sp_flag))
    {
          proto_node.flags = (proto_node.flags | ANY_SRC_PORT);
    }

    if (proto_node.not_sp_flag)
        proto_node.flags = (proto_node.flags | EXCEPT_SRC_PORT);
    if (!strncmp("<>", toks[4], 2))
    {
        proto_node.flags = (proto_node.flags | BIDIRECTIONAL);
    }
    if (*toks[5]=='!')  
    {
        proto_node.flags = (proto_node.flags | EXCEPT_DST_IP);
        ParseIP(&toks[5][1], (u_long *) &proto_node.dip,
                (u_long *) &proto_node.dmask);
    }else
        ParseIP(toks[5], (u_long *) &proto_node.dip,
                (u_long *) &proto_node.dmask);

    if (ParsePort(toks[6], (u_short *) &proto_node.hdp, 
                  (u_short *) &proto_node.ldp, toks[1], 
                  (int *) &proto_node.not_dp_flag))
    {
        proto_node.flags =(proto_node.flags | ANY_DST_PORT);
    }

    if (proto_node.not_dp_flag)
        proto_node.flags =(proto_node.flags | EXCEPT_DST_PORT);

    switch (rule_type){
        case RULE_ALERT:
            ProcessHeadNode(&proto_node, ptrMainApp->RuleAlert, protocol); 
            break;
        case RULE_LOG:
            ProcessHeadNode(&proto_node, ptrMainApp->RuleLog, protocol); 
            break;
        case RULE_PASS:
            ProcessHeadNode(&proto_node, ptrMainApp->RulePass, protocol); 
            break;
    }
    ptrMainApp->rule_count++;
    objRule->ParseRuleOptions(rule, rule_type, protocol);
    free(toks);
    return;
}

int CRules::RuleType(char *func)
{
	if (func == NULL){
//        FatalError("ERROR line %s (%d) => Unknown rule type (%s)\n", file_name, file_line, func);
    }
    if (!strncasecmp(func, "log",3))
        return RULE_LOG;
    if (!strncasecmp(func, "alert",5))
        return RULE_ALERT;
    if (!strncasecmp(func, "pass",4))
        return RULE_PASS;
    if (!strncasecmp(func, "var",3))
        return RULE_VAR;
    if (!strncasecmp(func, "include",7))
        return RULE_INCLUDE;
    if (!strncasecmp(func, "preprocessor",12))
        return RULE_PREPROCESS;
    if (!strncasecmp(func, "output",6))
        return RULE_OUTPUT;
    //FatalError("ERROR line %s (%d) => Unknown rule type: %s\n", file_name, file_line, func);
    return 0;
}

char* CRules::ExpandVars(char *string)
{
	static char estring[1024];
    char rawvarname[128],
    varname[128],
    varaux[128],
    varbuffer[128],
    varmodifier,
    *varcontents;
    int varname_completed,
    c, i, j, iv, jv,
    l_string,
    name_only;
    if (!string || ! *string || ! strchr(string, '$'))
        return(string);
    bzero((char *)estring, sizeof(estring));
    i = j = 0;
    l_string = strlen(string);
    while (i < l_string && j < sizeof(estring) - 1){
        c = string[i++];
        if (c == '$'){
            bzero((char *)rawvarname, sizeof(rawvarname));
            varname_completed = 0;
            name_only = 1;
            iv = i;
            jv = 0;
            if (string[i] == '('){
                name_only = 0;
                iv = i + 1;
            }
			while (!varname_completed 
                   && iv < l_string 
                   && jv < sizeof(rawvarname) - 1)
            {
                c = string[iv++];

                if ((name_only && !(isalnum(c) || c == '_')) 
                    || (! name_only && c == ')'))
                {
                    varname_completed = 1;

                    if (name_only)
                        iv--;
                }
                else
                {
                    rawvarname[jv++] = c;
                }
            }

            if (varname_completed){
                char *p;
                i = iv;
                varcontents = NULL;
                bzero((char *)varname, sizeof(varname));
                bzero((char *)varaux, sizeof(varaux));
                varmodifier = ' ';
                if ((p = strchr(rawvarname, ':')))
                {
                    strncpy(varname, rawvarname, p - rawvarname);
                    if (strlen(p) >= 2){
                        varmodifier = *(p + 1);
                        strcpy(varaux, p + 2);
                    }
                }else
                    strcpy(varname, rawvarname);

                bzero((char *)varbuffer, sizeof(varbuffer));
                varcontents = VarGet(varname);
                switch (varmodifier){
                    case '-':
                        if (! varcontents || ! strlen(varcontents))
                            varcontents = varaux;
                        break;

                    case '?':
                        break;
                }
                if (varcontents)
                {
                    int l_varcontents = strlen(varcontents);
                    iv = 0;
                    while (iv < l_varcontents && j < sizeof(estring) - 1)
                        estring[j++] = varcontents[iv++];
                }
            }
            else
                estring[j++] = '$';
        }
        else
            estring[j++] = c;
    }
    return(estring);
}

void CRules::VarDefine(char *name, char *value)
{
	struct VarEntry *p;
    int found = 0;
	CIFServerApp *ptrMainApp;
	ptrMainApp = PTRAPP;
    if (! ptrMainApp->VarHead)
    {
        p = VarAlloc();
        p->name = strdup(name);
        p->value = strdup(value);
        p->prev = p;
        p->next = p;
        ptrMainApp->VarHead = p;
        return;
    }
    p = ptrMainApp->VarHead;

    do{
        if (strcasecmp(p->name, name) == 0){
            found = 1;
            break;
        }
        p = p->next;
    } while (p != ptrMainApp->VarHead);

    if (found){
        if (p->value)
            free(p->value);

        p->value = strdup(value);
    }else{
        p = VarAlloc();
        p->name = strdup(name);
        p->value = strdup(value);
        p->prev = ptrMainApp->VarHead;
        p->next = ptrMainApp->VarHead->next;
        p->next->prev = p;
        ptrMainApp->VarHead->next = p;
    }    
}

void CRules::ParsePreprocessor(char *rule)
{
	char **toks;          
    char **pp_head;       
    char *funcname;       
	char *pp_args = NULL; 
    int num_toks;         
    int found = 0;        
    PreprocessKeywordList *pl_idx;
	CStringMan *objString;
	objString = new CStringMan;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CManipulate objManip;
	pp_args = new char[1024];
	funcname = new char[2];
    toks = objString->mSplit(rule, ":", 2, &num_toks,'\\');
    if (num_toks >= 1){
        pp_args = toks[1];
    }
    pp_head = objString->mSplit(toks[0], " ", 2, &num_toks, '\\');
    funcname = pp_head[1];
    pl_idx = ptrMainApp->PreprocessKeywords;
    while (pl_idx != NULL){
        if (!strcasecmp(funcname, pl_idx->entry.keyword)){
			void (CManipulate::*func)(u_char *);            
			func = pl_idx->entry.func;
			(objManip.*func)((u_char *)pp_args);
            found = 1;
        }
        if (!found){
            pl_idx = pl_idx->next;
        }else
            break;
    }
}

void CRules::ParseOutputPlugin(char *rule)
{
	char **toks;
    char **pp_head;
    char *funcname;
    char *pp_args = NULL;
    int num_toks;
    int found = 0;
    OutputKeywordList *pl_idx;
	CStringMan *objString;
	objString = new CStringMan;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	CRules *objRule;
	objRule = new CRules;
	CManipulate objManip;
	void (CManipulate::*func)(u_char *);            
    toks = objString->mSplit(rule, ":", 2, &num_toks,'\\');
    if (num_toks >= 1){
        pp_args = toks[1];
    }
    pp_head = objString->mSplit(toks[0], " ", 2, &num_toks, '\\');
    funcname = pp_head[1];
    pl_idx = ptrMainApp->OutputKeywords;
    while (pl_idx != NULL){
        if (!strcasecmp(funcname, pl_idx->entry.keyword)){
            switch (pl_idx->entry.node_type){
                case NT_OUTPUT_ALERT:
                if (ptrMainApp->AlertFunc == NULL){
						ptrMainApp->AlertFunc = &CManipulate::CallAlertPlugins;
                    }
                    func = pl_idx->entry.func;
					(objManip.*func)((u_char*)pp_args);
                    break;
                case NT_OUTPUT_LOG:
				if (ptrMainApp->LogFunc == NULL){
						ptrMainApp->LogFunc = &CManipulate::CallLogPlugins;
                    }
					func = pl_idx->entry.func;
					(objManip.*func)((u_char*)pp_args);
                    break;
            }

            found = 1;
        }
        if (!found){
            pl_idx = pl_idx->next;
        }else
            break;
    }
}

int CRules::WhichProto(char *proto_str)
{
	if (!strncasecmp(proto_str, "tcp", 3))
        return IPPROTO_TCP;
    if (!strncasecmp(proto_str, "udp", 3))
        return IPPROTO_UDP;
    if (!strncasecmp(proto_str, "icmp", 4))
        return IPPROTO_ICMP;
    return 0;
}

int CRules::ParsePort(char *prule_port, u_short *hi_port, u_short *lo_port, char *proto, int *not_flag)
{
    char **toks;         
    int num_toks;        
    char *rule_port;     
	CStringMan *objString;
	objString = new CStringMan;
	rule_port = new char[STD_BUF];
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    *not_flag = 0;
    if (! strncmp(prule_port, "$", 1)){
        if ((rule_port = VarGet(prule_port + 1)) == NULL){
//            FatalError( "ERROR %s (%d) => Undefined variable %s\n", file_name, file_line, prule_port);
        }
    }
    else
        rule_port = prule_port;
    if (!strncasecmp(rule_port, "any", 3)){
        *hi_port = 0;
        *lo_port = 0;
        return 1;
    }
    if (rule_port[0] == '!'){
        *not_flag = 1;
        rule_port++;
    }
    if (rule_port[0] == ':'){
        *lo_port = 0;
    }
    toks = objString->mSplit(rule_port, ":", 2, &num_toks,0);
    switch (num_toks){
        case 1:
            *hi_port = ConvPort(toks[0], proto);
            if (rule_port[0] == ':'){
                *lo_port = 0;
            }else{
                *lo_port = *hi_port;
                if (index(rule_port, ':') != NULL){
                    *hi_port = 65535;
                }
            }
            return 0;
        case 2:
            *lo_port = ConvPort(toks[0], proto);
            if (toks[1][0] == 0)
                *hi_port = 65535;
            else
                *hi_port = ConvPort(toks[1], proto);
            return 0;
        default:
			int i=1;
    }             
    return 0;
}

int CRules::ConvPort(char *port, char *proto)
{
	int conv;  
    struct servent *service_info;
    if (isalpha((int)port[0]) != 0){
        service_info = getservbyname(port, proto);
        if (service_info != NULL){
            conv = ntohs(service_info->s_port);
            return conv; 
        }
    }
    if (!isdigit((int)port[0])){
/*        FatalError( "ERROR %s (%d) => Invalid port: %s\n", file_name, 
                    file_line, port);*/
    }
    conv = atoi(port);
	if ((conv >= 0) && (conv < 65536)){
        return conv;
    }else{
/*        FatalError( "ERROR %s (%d) => bad port number: %s", file_name, 
                    file_line, port);*/
    }

    return 0;
}

void CRules::ProcessHeadNode(RuleTreeNode *test_node, ListHead *list, int protocol)
{
	int match = 0;
    RuleTreeNode *rtn_idx;
	rtn_idx = (RuleTreeNode *) calloc(sizeof(RuleTreeNode), sizeof(char));
	rtn_idx = NULL;
    int count = 0;
	CIFServerApp* ptrMainApp;
	ptrMainApp = PTRAPP;
	ptrMainApp->rtn_tmp = NULL;
    switch (protocol){
        case IPPROTO_TCP:
            rtn_idx =  list->TcpList; 
            break;
        case IPPROTO_UDP:
            rtn_idx =  list->UdpList; 
            break;
        case IPPROTO_ICMP:
            rtn_idx =  list->IcmpList; 
            break;
        default: rtn_idx = NULL;
            break;
    }
    if (rtn_idx == NULL){
        ptrMainApp->head_count++;
        switch (protocol){
            case IPPROTO_TCP:
                list->TcpList = (RuleTreeNode *) calloc(sizeof(RuleTreeNode), sizeof(char)); 
                ptrMainApp->rtn_tmp = list->TcpList;
                break;
            case IPPROTO_UDP:
                list->UdpList = (RuleTreeNode *) calloc(sizeof(RuleTreeNode), sizeof(char)); 
                ptrMainApp->rtn_tmp = list->UdpList;
                break;
            case IPPROTO_ICMP:
                list->IcmpList = (RuleTreeNode *) calloc(sizeof(RuleTreeNode), sizeof(char)); 
                ptrMainApp->rtn_tmp = list->IcmpList;
                break;
        }
        XferHeader(test_node, ptrMainApp->rtn_tmp);
        ptrMainApp->rtn_tmp->head_node_number = ptrMainApp->head_count; 
        ptrMainApp->rtn_tmp->down = NULL;
        SetupRTNFuncList(ptrMainApp->rtn_tmp);
        return;
    }
    match = TestHeader(rtn_idx,test_node);
    while ((rtn_idx->right != NULL) && !match){
        count++;
        match = TestHeader(rtn_idx,test_node);
        if (!match)
            rtn_idx = rtn_idx->right;
        else
            break;
    }
    match = TestHeader(rtn_idx,test_node);
    if (!match){
        ptrMainApp->head_count++;
        rtn_idx->right = (RuleTreeNode *) calloc(sizeof(RuleTreeNode), sizeof(char));
        ptrMainApp->rtn_tmp = rtn_idx->right;
        if (ptrMainApp->rtn_tmp == NULL){
//            FatalError( "ERROR: Unable to allocate Rule Head Node!!\n");
        }
        XferHeader(test_node, ptrMainApp->rtn_tmp);
        ptrMainApp->rtn_tmp->head_node_number = ptrMainApp->head_count; 
        ptrMainApp->rtn_tmp->down = NULL;
        SetupRTNFuncList(ptrMainApp->rtn_tmp);
    }else{
        ptrMainApp->rtn_tmp = rtn_idx;
    }
}

void CRules::SetupRTNFuncList(RuleTreeNode *rtn)
{
	int(CManipulate::*func)(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
	if (rtn->flags & BIDIRECTIONAL){
		func = &CManipulate::CheckBidirectional;
		AddRuleFuncToList(func, rtn);
    }else{
        AddrToFunc(rtn, rtn->sip, rtn->smask, (rtn->flags & EXCEPT_SRC_IP ? 1 : 0), SRC);
        AddrToFunc(rtn, rtn->dip, rtn->dmask, (rtn->flags & EXCEPT_DST_IP ? 1 : 0), DST);
        PortToFunc(rtn, (rtn->flags & ANY_SRC_PORT ? 1 : 0), 
                   (rtn->flags & EXCEPT_SRC_PORT ? 1: 0), SRC);   
        PortToFunc(rtn, (rtn->flags & ANY_DST_PORT ? 1 : 0), 
                   (rtn->flags & EXCEPT_DST_PORT ? 1: 0), DST);   
    }
	func = &CManipulate::RuleListEnd;
    AddRuleFuncToList(func, rtn);
}

void CRules::AddrToFunc(RuleTreeNode *rtn, u_long ip, u_long mask, int exception_flag, int mode)
{
    if ((ip == 0) && (mask == 0))
        return;
	int (CManipulate::*ptrfunc)(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
    if (exception_flag){
        switch (mode){
            case SRC:
				ptrfunc = &CManipulate::CheckSrcIPNotEq;
				AddRuleFuncToList(ptrfunc, rtn);
                break;
            case DST:
				ptrfunc = &CManipulate::CheckDstIPNotEq;
                AddRuleFuncToList(ptrfunc, rtn);
                break;
        }
        return;
    }
    switch (mode){
        case SRC:
			ptrfunc = &CManipulate::CheckSrcIPEqual;
            AddRuleFuncToList(ptrfunc, rtn);
            break;
        case DST:
			ptrfunc = &CManipulate::CheckDstIPEqual;
            AddRuleFuncToList(ptrfunc, rtn);
            break;
    }
}

void CRules::PortToFunc(RuleTreeNode *rtn, int any_flag, int except_flag, int mode)
{
    if (any_flag)
        return;
	int (CManipulate::*ptrfunc)(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list);
    if (except_flag){
        switch (mode){
            case SRC:
				ptrfunc = &CManipulate::CheckSrcPortNotEq;
                AddRuleFuncToList(ptrfunc, rtn);
                break;
            case DST:
				ptrfunc = &CManipulate::CheckDstPortNotEq;
                AddRuleFuncToList(ptrfunc, rtn);
                break;
        }
        return;
    }
    switch (mode){
        case SRC:
			ptrfunc = &CManipulate::CheckSrcPortEqual;
            AddRuleFuncToList(ptrfunc, rtn);
            break;
        case DST:
            ptrfunc = CManipulate::CheckDstPortEqual;
			AddRuleFuncToList(ptrfunc, rtn);
            break;
    }
    return;
}

void CRules::XferHeader(RuleTreeNode *rule, RuleTreeNode *rtn)
{
	rtn->type = rule->type;
    rtn->sip = rule->sip;
    rtn->dip = rule->dip;
    rtn->smask = rule->smask;
    rtn->dmask = rule->dmask;
    rtn->hsp = rule->hsp;
    rtn->lsp = rule->lsp;
    rtn->hdp = rule->hdp;
    rtn->ldp = rule->ldp;
    rtn->flags = rule->flags;
}

int CRules::TestHeader(RuleTreeNode *rule, RuleTreeNode *rtn)
{
	if (rtn->sip == rule->sip){
        if (rtn->dip == rule->dip){
            if (rtn->dmask == rule->dmask){
                if (rtn->smask == rule->smask){
                    if (rtn->hsp == rule->hsp){
                        if (rtn->lsp == rule->lsp){
                            if (rtn->hdp == rule->hdp){
                                if (rtn->ldp == rule->ldp){
                                    if (rtn->flags == rule->flags){
                                        return 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return 0;
}

struct VarEntry* CRules::VarAlloc()
{
	struct VarEntry *New;

    if ((New = (struct VarEntry *)calloc(sizeof(struct VarEntry), sizeof(char))) == NULL){
//        FatalError( "ERROR: cannot allocate memory for VarEntry.");
    }

    New->name = NULL;
    New->value = NULL;
    New->prev = NULL;
    New->next = NULL;

    return(New);
}

void CRules::SetOutputList(void (CManipulate::*func)(Packet *,char *,void *), char node_type, void *arg)
{

   	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	OutputFuncNode *idx;
    OutputFuncNode *prev;
    switch (node_type){
        case NT_OUTPUT_ALERT:
            prev = ptrMainApp->AlertList;
            break;
        case NT_OUTPUT_LOG:
            prev = ptrMainApp->LogList;
            break;
        default:
            return;
    }
    while(prev != NULL) {
        idx = prev->next;
        free(prev);
        prev = idx;
    }
    AddFuncToOutputList(func, node_type, arg);
    return;
}

void CRules::ParseRuleOptions(char *rule, int rule_type, int protocol)
{
	char **toks = NULL;
    char **opts;
    char *idx;
    char *aux;
    int num_toks;
    int i;
    int num_opts;
    int found = 0;
    int list = 0;
    OptTreeNode *otn_idx;
    KeywordXlateList *kw_idx;
	kw_idx = new KeywordXlateList;
	kw_idx = NULL;
	CStringMan *objString;
	objString = new CStringMan;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
	idx = new char[256];
	int(CManipulate::*ptrfunc)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
    otn_idx = ptrMainApp->rtn_tmp->down;
    if (otn_idx != NULL)
    {
        while (otn_idx->next != NULL){
            otn_idx = otn_idx->next;
        }
        otn_idx->next = (OptTreeNode *) calloc(sizeof(OptTreeNode), sizeof(char));
        ptrMainApp->otn_tmp = otn_idx->next;
        if (ptrMainApp->otn_tmp == NULL){
//            FatalError("ERROR: Unable to alloc OTN: %s", strerror(errno));
        }
        ptrMainApp->otn_tmp->next = NULL;
        ptrMainApp->opt_count++;
    }else{
        otn_idx = (OptTreeNode *) calloc(sizeof(OptTreeNode), sizeof(char));
        bzero((char *)otn_idx, sizeof(OptTreeNode));
        ptrMainApp->otn_tmp = otn_idx;
        if (ptrMainApp->otn_tmp == NULL){
//            FatalError( "ERROR: Unable to alloc OTN!\n");
        }
        ptrMainApp->otn_tmp->next = NULL;
        ptrMainApp->rtn_tmp->down = ptrMainApp->otn_tmp;
        ptrMainApp->opt_count++;
    }

    ptrMainApp->otn_tmp->chain_node_number = ptrMainApp->opt_count;
    ptrMainApp->otn_tmp->type = rule_type;
    idx = index(rule, '(');
    i = 0;
    if (idx != NULL){
        idx++;
        aux = strrchr(idx,')');
        *aux = 0;
        toks = objString->mSplit(idx, ";", 10, &num_toks,'\\');
        num_toks--;
        while (num_toks){
            opts = objString->mSplit(toks[i], ":", 4, &num_opts,'\\');
            while (isspace((int)*opts[0])) opts[0]++;
            if (!strcasecmp(opts[0], "content_list")){
				int (CManipulate::*ptrfunc)(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list);
				ptrfunc = &CManipulate::OptListEnd;
				AddOptFuncToList(ptrfunc, ptrMainApp->otn_tmp);
                while (isspace((int)*opts[1])) opts[1]++;
                ParseListFile(rule, opts[1]);
                list = 1;
            }else if (!strcasecmp(opts[0], "msg")){
                ParseMessage(opts[1]);
            }else if (!strcasecmp(opts[0], "logto")){
                ParseLogto(opts[1]); 
            }
#ifdef ENABLE_RESPONSE
            else if (!strncasecmp(opts[0], "resp", 4))
            {
                otn_tmp->response_flag = RESP_RST_SND;

                ParseResponse(opts[1]);
            }
#endif
            else{

                kw_idx = ptrMainApp->KeywordList;
                found = 0;
                while (kw_idx != NULL){
                    if (!strcasecmp(opts[0], kw_idx->entry.keyword)){
						CManipulate objManip;
						void (CManipulate::*ptrfunc)(char*,_OptTreeNode*,int);
						ptrfunc = kw_idx->entry.func;
                        (objManip.*ptrfunc)(opts[1], ptrMainApp->otn_tmp, protocol);
                        found = 1;
                    }
                    if (!found){
                        kw_idx = kw_idx->next;
                    }else
                        break;
                }
                if (!found){
                    if (!strcasecmp(opts[0], "minfrag")){
//                        FatalError( "\nERROR: %s (%d) => Minfrag is no longer a rule option, it is a\npreprocessor (please remove it from your rules file).  See RULES.SAMPLE or\nifs-lib for examples of using the new preprocessors!\n", file_name, file_line);
                    }else{
//                        FatalError( "\nERROR: %s (%d) => Unknown keyword \"%s\" in rule!\n", file_name, file_line, opts[0]);
                    }
                }
            }
            free(opts);
            --num_toks;
            i++;
        }
		ptrfunc = &CManipulate::OptListEnd;
		AddOptFuncToList(ptrfunc, ptrMainApp->otn_tmp);
    }else{
		ptrfunc = &CManipulate::OptListEnd;
        AddOptFuncToList(ptrfunc, ptrMainApp->otn_tmp);
    }
    free(toks);
}

void CRules::AddRuleFuncToList(int (CManipulate::*func)(Packet *,_RuleTreeNode *,_RuleFpList *), RuleTreeNode *rtn)
{
    RuleFpList *idx;
    idx = rtn->rule_func;
    if (idx == NULL){
        rtn->rule_func = (RuleFpList *) calloc(sizeof(RuleFpList), sizeof(char));
        rtn->rule_func->RuleHeadFunc = func; 
    }else{
        while (idx->next != NULL)
            idx = idx->next;
        idx->next = (RuleFpList *) calloc(sizeof(RuleFpList), sizeof(char));
        idx = idx->next;
        idx->RuleHeadFunc = func;
    }
}

int CRules::CheckBidirectional(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	int test_result = 0;
    test_result = CheckAddrPort(rtn_idx->sip, rtn_idx->smask, rtn_idx->hsp, rtn_idx->lsp, p, rtn_idx->flags, CHECK_SRC);
    if (test_result){
        test_result = CheckAddrPort(rtn_idx->dip, rtn_idx->dmask, rtn_idx->hdp,rtn_idx->ldp, p, rtn_idx->flags, CHECK_DST);
        if (!test_result){
            test_result = CheckAddrPort(rtn_idx->dip, rtn_idx->dmask, rtn_idx->hdp,rtn_idx->ldp, p, rtn_idx->flags, (CHECK_SRC|INVERSE));
            if (test_result){
                test_result = CheckAddrPort(rtn_idx->sip, rtn_idx->smask, rtn_idx->hsp, rtn_idx->lsp, p, rtn_idx->flags, (CHECK_DST|INVERSE));
                if (!test_result){
                    return 0;
                }
            }else{
                return 0;
            }
        }
    }else{
        test_result = CheckAddrPort(rtn_idx->dip, rtn_idx->dmask, rtn_idx->hdp,rtn_idx->ldp, p, rtn_idx->flags, CHECK_SRC|INVERSE);
        if (test_result){
            test_result = CheckAddrPort(rtn_idx->sip, rtn_idx->smask, rtn_idx->hsp, rtn_idx->lsp, p, rtn_idx->flags, CHECK_DST|INVERSE);
            if (!test_result){
                return 0;
            }
        }else{
            return 0;
        }
    }
    return 1;
}

int CRules::RuleListEnd(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	return 1;
}

int CRules::CheckSrcIPNotEq(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
    if (rtn_idx->sip != (p->iph->ip_src.s_addr & rtn_idx->smask))
    {
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::CheckDstIPNotEq(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
    if (rtn_idx->dip != (p->iph->ip_dst.s_addr & rtn_idx->dmask))
    {
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::CheckSrcIPEqual(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
    if (rtn_idx->sip == (p->iph->ip_src.s_addr & rtn_idx->smask)){
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::CheckDstIPEqual(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
    if (rtn_idx->dip == (p->iph->ip_dst.s_addr & rtn_idx->dmask)){
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::CheckSrcPortNotEq(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
    if ((p->sp > rtn_idx->hsp) || (p->sp < rtn_idx->lsp)){
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::CheckDstPortNotEq(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
	if ((p->dp > rtn_idx->hdp) || (p->dp < rtn_idx->ldp)){
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::CheckSrcPortEqual(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
	if ((p->sp <= rtn_idx->hsp) && (p->sp >= rtn_idx->lsp)){
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::CheckDstPortEqual(Packet *p, _RuleTreeNode *rtn_idx, RuleFpList *fp_list)
{
	CManipulate objManip;
    if ((p->dp <= rtn_idx->hdp) && (p->dp >= rtn_idx->ldp)){
		int (CManipulate::*ptrfunc)(Packet *, struct _RuleTreeNode *, struct _RuleFpList *);
		ptrfunc = fp_list->next->RuleHeadFunc;
        return (objManip.*ptrfunc)(p, rtn_idx, fp_list->next);
    }
    return 0;
}

int CRules::OptListEnd(Packet *p, _OptTreeNode *otn_idx, OptFpList *fp_list)
{
	return 1;
}

void CRules::ParseListFile(char *rule, char *file)
{
	FILE *thefp;            
    char *tmp;
    char buf[STD_BUF];      
    char rule_buf[STD_BUF]; 
    int frazes_count;       
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    if ((thefp = fopen(file,"r")) == NULL){
//        FatalError("Unable to open list file: %s\n", file);
    }
    bzero((char *)buf, STD_BUF);
    bzero((char *)rule_buf, STD_BUF);
    frazes_count = 0;
    while ((fgets(buf, STD_BUF, thefp)) != NULL){
        list_file_line++;
        if ((buf[0] != '#') && (buf[0] != 0x0a) && (buf[0] != ';')){
            frazes_count++;
            ptrMainApp->Strip(buf);
            tmp = CreateRule(rule, buf, &rule_buf[0]);
            ParseRule(rule_buf, 0);
        }
        bzero((char *)buf, STD_BUF);
        bzero((char *)rule_buf, STD_BUF);
    }
    fclose(thefp);
    return;
}

void CRules::ParseMessage(char *msg)
{
	char *ptr;
    char *end;
    int size;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    ptr = index(msg,'"');
    if (ptr == NULL){
        ptr = msg;
    }else
        ptr++;
    end = index(ptr,'"');
    if (end != NULL)
        *end = 0;
    while (isspace((int) *ptr)) ptr++;
    size = strlen(msg)+1;
    if (size > 0){
        ptrMainApp->otn_tmp->message = (char *)calloc((sizeof(char)*size), sizeof(char));
        strncpy(ptrMainApp->otn_tmp->message, ptr, size);
        ptrMainApp->otn_tmp->message[size-1] = 0;
    }
}

void CRules::ParseLogto(char *filename)
{
	char *sptr;
    char *eptr;
	CIFServerApp* ptrMainApp;
	ptrMainApp =PTRAPP;
    sptr = index(filename, '"');
    eptr = strrchr(filename, '"');
    if (sptr != NULL && eptr != NULL){
        sptr++;
        *eptr = 0;
    }else{
        sptr = filename;
    }
    ptrMainApp->otn_tmp->logto = (char *) calloc(strlen(sptr) + 1, sizeof(char));
    bzero((char *)ptrMainApp->otn_tmp->logto, strlen(sptr)+1);
    strncpy(ptrMainApp->otn_tmp->logto, sptr, strlen(sptr));
}

char* CRules::CreateRule(char *rule, char *buf, char *rule_buf)
{
    char *idx;           
    char con[] = "content: \"";  
    char oux[] = ")";
    bzero((char *)rule_buf, STD_BUF);
    memccpy(rule_buf, rule, (int)'(', STD_BUF);  
    idx = strcat(rule_buf, con);             
    idx = strcat(rule_buf, buf);         
    idx = strcat(rule_buf, &con[9]);     
    idx = strcat(rule_buf, strchr(rule, ';'));       
    idx = strcat(rule_buf, oux);             
    return(rule_buf);
}
