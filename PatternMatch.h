// PatternMatch.h: interface for the CPatternMatch class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PATTERNMATCH_H__CB7B8C80_03F5_4324_8D2B_B857CC26A386__INCLUDED_)
#define AFX_PATTERNMATCH_H__CB7B8C80_03F5_4324_8D2B_B857CC26A386__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define PLUGIN_PATTERN_MATCH   1

class CPatternMatch : public CManipulate  
{
public:
	void PayloadSearchNocase(char *data, OptTreeNode *otn, int protocol);
	void PayloadSearchDepth(char *data, OptTreeNode *otn, int protocol);
	void PayloadSearchOffset(char *data, OptTreeNode *otn, int protocol);
	void SetupPatternMatch();
	void NewNode(OptTreeNode *otn);
	void ParsePattern(char *rule, OptTreeNode *otn);
	int CheckPatternMatch(Packet *p, struct _OptTreeNode *otn_idx, OptFpList *fp_list);
	void PayloadSearchInit(char *data, OptTreeNode *otn, int protocol);
	CPatternMatch();
	virtual ~CPatternMatch();

};

#endif // !defined(AFX_PATTERNMATCH_H__CB7B8C80_03F5_4324_8D2B_B857CC26A386__INCLUDED_)
