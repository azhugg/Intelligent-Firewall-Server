// DatabaseLog.h: interface for the CDatabaseLog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATABASELOG_H__B3AE04EA_3E71_4BEF_BDF0_E02BE5DF02C1__INCLUDED_)
#define AFX_DATABASELOG_H__B3AE04EA_3E71_4BEF_BDF0_E02BE5DF02C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef ENABLE_MYSQL
#include <mysql.h>
MYSQL * m_sock;
MYSQL_RES * m_result;
MYSQL_ROW m_row;
#endif
#define MAX_QUERY_LENGTH 600
#define POSTGRESQL   "postgresql"
#define MYSQL        "mysql"
#define UNIXODBC     "unixodbc"
#define POSTGRESQL   "postgresql"


class CDatabaseLog : public CManipulate  
{
public:
	CDatabaseLog();
	virtual ~CDatabaseLog();
public:
	void SetupLogDatabase();
	void LogDatabaseInit(u_char *args);
	void Connect();
	int Select(char * query);
	int Insert(char * query);
	void LogDatabase(Packet *p, char *msg, void *arg);
	void ParseDatabaseArgs(char *args);
	char * dbtype;
	char * dbname;
	char * password;
	char * host;
	char * user;
	char * port;
	int  sid, cid;
};

#endif // !defined(AFX_DATABASELOG_H__B3AE04EA_3E71_4BEF_BDF0_E02BE5DF02C1__INCLUDED_)
