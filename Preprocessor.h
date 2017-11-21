// Preprocessor.h: interface for the CPreprocessor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PREPROCESSOR_H__C2FB6E79_4D1C_4D7D_8B21_821B409B22A5__INCLUDED_)
#define AFX_PREPROCESSOR_H__C2FB6E79_4D1C_4D7D_8B21_821B409B22A5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPreprocessor  
{
public:
	void InitOutputPlugins();
	PluginSignalFuncNode* AddFuncToSignalList(void (CManipulate::*func)(int, void*), void *arg, PluginSignalFuncNode *list);
	void AddFuncToCleanExitList(void (CManipulate::*func)(int, void*), void *arg);
	void AddFuncToRestartList(void (CManipulate::*func)(int, void*), void *arg);
	void RegisterOutputPlugin(char *keyword, int type, void (CManipulate::*func)(u_char *));
	void InitPlugIns();
	void RegisterPlugin(char *keyword, void (CManipulate::*func)(char *, OptTreeNode *, int));
	int PacketIsIP(Packet *p);
	int CheckNet(struct in_addr *compare, struct in_addr *compare2);
	int IsTcpSessionTraffic(Packet *p);
	int SourceIpIsHomenet(Packet *p);
	int DestinationIpIsHomenet(Packet *p);
	int PacketIsICMP(Packet *p);
	int PacketIsUDP(Packet *p);
	int PacketIsTCP(Packet *p);
	void RegisterPreprocessor(char* keyword, void (CManipulate::*func)(u_char *));
	void InitPreprocessors();
	CPreprocessor();
	virtual ~CPreprocessor();

};

#endif // !defined(AFX_PREPROCESSOR_H__C2FB6E79_4D1C_4D7D_8B21_821B409B22A5__INCLUDED_)
