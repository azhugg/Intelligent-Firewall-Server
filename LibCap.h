// LibCap.h: interface for the CLibCap class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_LIBCAP_H__ACCA438B_030A_4D1B_AF03_E5F50E434189__INCLUDED_)
#define AFX_LIBCAP_H__ACCA438B_030A_4D1B_AF03_E5F50E434189__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CLibCap  
{
public:
	void CreatePidFile();
	int InitInterface(char *device);
	void InitProtoNames();
	void SetAdapt(char* ad);
	char* GetAdapterFromList(char *device, int index);
	char* LookupDev(char err[256]);
	CLibCap();
	virtual ~CLibCap();

};

#endif // !defined(AFX_LIBCAP_H__ACCA438B_030A_4D1B_AF03_E5F50E434189__INCLUDED_)
