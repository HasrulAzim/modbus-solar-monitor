/*
 *    Written by:   Danny Holstein
 *    email:        dgholstein@gmail.com
 *    Description: Library to enable Labview to access SQL database
 * 
 */
#define SQL_LV_VERSION "sql_LV-1.12"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "extcode.h"

#ifdef WIN
	#include <windows.h>
	#define HAVE_ODBC
	typedef long ulong;
#else 
	#define HAVE_MYSQL
#endif
#define SQL_APPEND sql_query+strlen(sql_query)

#ifdef HAVE_MYSQL
	#include <mysql.h>
#endif

#ifdef HAVE_ODBC
 #include <sql.h>
 #include <sqlext.h>
 int idx_find(char *idx_field, unsigned long  *db, char *table);
 void date_manipulate(char* field, unsigned long  *db);
 #define ODBC_ERROR	   SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, 1024, 0);\
	   sql_error(&debug, msg_debug, sql_query);\
	   return (-2);
 #define SQL_FIELD 1024  
#endif

#ifdef HAVE_ORACLE
    #include <oci.h>
    
    static void checkerr(OCIError *errhp, sword status, char* msg_debug);
    #define OCI_CHECKERR if (retCode != OCI_SUCCESS) {LV_str_cp(debug, msg_debug); return(-2);}
    #define SQL_FIELD 1024
    OCIBind *bndhp[SQL_FIELD];

    sword    retCode = 0;

    OCIStmt *stmthp;
    OCIDefine *defnp = (OCIDefine *) 0;
#endif
       
enum {odbc_mysql=0,
      odbc_oracle=1,
      odbc_db2=2,
      odbc_fb=3,
      odbc_access=4,
      odbc_sqlserver=5,
      odbc_postgreSQL=6};

enum {mysql_api=0, odbc=1, oracle_api=2};
#define QUERY_LEN 16384
#define DEBUG_LEN 1024

/* Typedefs */	
typedef struct {
#ifdef HAVE_MYSQL
	MYSQL mysql;
	MYSQL_RES *query_results;
#endif
#ifdef HAVE_ODBC
	HENV henv;
	HDBC hdbc;
	HSTMT hstmt;
#endif
#ifdef HAVE_ORACLE
    OCIEnv *envhp;
    OCIError *errhp;
    OCISession *authp;
    OCIServer *srvhp;
    OCISvcCtx *svchp;
#endif
	unsigned short int odbc_driver, db_type;
	} SQL_LV_REF;
    
typedef struct {
	LStrHandle db;
	LStrHandle host;
	LStrHandle user;
	LStrHandle password;
	unsigned short int db_type;
	} DB_LOGIN;

typedef struct {
	int32 dimSize;
	LStrHandle String[1];
	} TD2;
typedef TD2 **TD2Hdl;

typedef struct {
	int32 dimSizes[2];
	LStrHandle String[1];
	} TD4;
typedef TD4 **TD4Hdl;

void sql_error(LStrHandle *debug, char *db_error, char *query);
void StringAdd(TD2Hdl in_array, char *string);
void StringRowAdd(TD4Hdl in_array, char *string[], int cols);
void StringRowAdd_b(TD4Hdl in_array, char *string[], int cols, int *lengths);
void LV_str_cp(LStrHandle LV_string, char* c_str);
void LV_str_cat(LStrHandle LV_string, char* c_str);


long sql_select(unsigned long *db, CStr table, CStr clause, TD2Hdl fields,
       char *distinct, TD4Hdl values, long limit, LStrHandle debug)
{
/* open db and count the SELECT quantity. */
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN];
#ifdef HAVE_MYSQL
 MYSQL_RES *query_results; MYSQL_ROW row;
#endif
int i, num_cols;
SQL_LV_REF *db_ref;
#define SQL_APPEND sql_query+strlen(sql_query)

#ifdef HAVE_ODBC
 static char **sql_row, *cache_LV;
 static unsigned int sql_warnings=0;
 SQLLEN *k, j=0;
#else
 int k, j=0;
 static char **sql_row;
#endif
 
unsigned short int odbc_driver, db_type;

if (*db == 0L) return -1;
	
db_ref = (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
strcpy(sql_query, "");

num_cols = (**fields).dimSize;

#ifdef HAVE_ODBC
k = malloc(sizeof(SQLLEN) * num_cols);
#endif
#if defined(HAVE_ODBC) || defined(HAVE_ORACLE)
sql_row = malloc(sizeof(char*) * num_cols);
for (i=0; i< num_cols; i++) {sql_row[i] = malloc(SQL_FIELD);}
#endif
/*
 *  build SQL "SELECT" and read from DB
 */
 if (!*distinct) {strcat(sql_query, "SELECT ");}
 else {strcat(sql_query, "SELECT DISTINCT ");}
#ifdef HAVE_ODBC
 if (odbc_driver==odbc_access && db_type == odbc && limit>0) {
   sprintf(SQL_APPEND, "TOP %d ", limit);}
#endif
 
 switch (num_cols) {
    case 0:
        return (0);
    case 1:
        strncat(sql_query, (**(**fields).String[0]).str, (**(**fields).String[0]).cnt);
        break;
    default:
        strncat(sql_query, (**(**fields).String[0]).str, (**(**fields).String[0]).cnt);
        for (i=1; i<num_cols ; i++) {
        strcat(sql_query, ", ");
        strncat(sql_query, (**(**fields).String[i]).str, (**(**fields).String[i]).cnt);}
        break;
     }
 sprintf(SQL_APPEND, " FROM %s", table);
 if (strlen(clause) > 0) {
   sprintf(SQL_APPEND, " %s", clause);}
/*
 *  finished building query, now read from DB
 */
 switch (db_type) {
#ifdef HAVE_MYSQL
   case mysql_api:
        if (limit>0) {sprintf(SQL_APPEND, " LIMIT %d", limit);}
        if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
            sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query);
 #ifdef HAVE_ODBC
            for (i=0; i< num_cols; i++) {free(sql_row[i]);} free(sql_row);
 #endif
            return (-2);}
        if ((query_results = mysql_store_result(&(db_ref->mysql))) == NULL) {
            sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query);
            return (-2);}
        while ((row = mysql_fetch_row(query_results))) {
            StringRowAdd(values, row, num_cols); j++;
        }
        mysql_free_result(query_results);
        return ((long) j); 
#endif
#ifdef HAVE_ODBC
   case odbc:
        if (odbc_driver==odbc_mysql && limit>0) {sprintf(SQL_APPEND, " LIMIT %d ", limit);}
        SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
        if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {ODBC_ERROR}
        for (i=0; i<num_cols ; i++) {
        if (SQLBindCol(db_ref->hstmt, i+1, SQL_CHAR, sql_row[i], SQL_FIELD-1, &k[i]) == SQL_ERROR) {ODBC_ERROR}}
        while (SQLFetchScroll(db_ref->hstmt, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
            StringRowAdd_b(values, sql_row, num_cols, (int *)k); j++;}
        SQLFreeStmt(db_ref->hstmt, SQL_CLOSE);
        for (i=0; i< num_cols; i++) {free(sql_row[i]);} free(sql_row);
        return ((long) j);
#endif
#ifdef HAVE_ORACLE
   case oracle_api:
        checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
        checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                    (ub4) strlen(sql_query),
                                    (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
                /* define variable assignment */
        for (i=0; i<num_cols ; i++) {
            checkerr(db_ref->errhp, (retCode = OCIDefineByPos(stmthp, &defnp, db_ref->errhp, i+1,
                    (dvoid *) sql_row[i], (sword) SQL_FIELD-1, SQLT_STR ,
                    (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)), msg_debug);
            OCI_CHECKERR;
        }
                /* execute and fetch */
        checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 0, (ub4) 0,
                    (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
        while ((retCode = OCIStmtFetch2 (stmthp, db_ref->errhp, (ub4) 1, OCI_DEFAULT, (ub4) 0, OCI_DEFAULT)) == OCI_SUCCESS){
            StringRowAdd(values, sql_row, num_cols); j++;
        }
        checkerr(db_ref->errhp, retCode, msg_debug); if (retCode == OCI_ERROR) {LV_str_cp(debug, msg_debug); return(-2);};
        for (i=0; i< num_cols; i++) {free(sql_row[i]);} free(sql_row);
        return ((long) j);
#endif
   default:
     return (0);
     break;
  }
}


long sql_select_b(unsigned long *db, CStr table, CStr clause, TD2Hdl fields,
       char *distinct, TD4Hdl values, long limit, LStrHandle debug)
{
/* open db and count the SELECT quantity. */
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN];
#ifdef HAVE_MYSQL
  MYSQL_RES *query_results; MYSQL_ROW row;
#endif
int i, num_cols, *lengths;
SQL_LV_REF *db_ref;
unsigned short int odbc_driver, db_type;

#ifdef HAVE_ODBC
static unsigned int sql_warnings=0;
static char **sql_row;
SQLLEN *k, j=0;
#else
int j=0;
#endif

if (*db == 0L) return -1;

num_cols = (**fields).dimSize;

#ifdef HAVE_ODBC
k = malloc(sizeof(SQLLEN) * num_cols);
sql_row = malloc(sizeof(char*) * num_cols);
#endif

db_ref = (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
strcpy(sql_query, "");
/*
 *  build SQL "SELECT" and read from DB
 */
 if (!*distinct) {strcat(sql_query, "SELECT ");}
 else {strcat(sql_query, "SELECT DISTINCT ");}
#ifdef HAVE_ODBC
 if (odbc_driver==odbc_access && db_type == odbc && limit>0) {
   sprintf(SQL_APPEND, "TOP %d ", limit);}
#endif

 switch (num_cols) {
   case 0:
     return (0);
   case 1:
     strncat(sql_query, (**(**fields).String[0]).str, (**(**fields).String[0]).cnt);
     break;
   default:
     strncat(sql_query, (**(**fields).String[0]).str, (**(**fields).String[0]).cnt);
     for (i=1; i<num_cols ; i++) {
       strcat(sql_query, ", ");
       strncat(sql_query, (**(**fields).String[i]).str, (**(**fields).String[i]).cnt);}
     break;
     }
 sprintf(SQL_APPEND, " FROM %s", table);
 if (strlen(clause) > 0) {
   sprintf(SQL_APPEND, " %s", clause);}

 switch (db_type) {
#ifdef HAVE_MYSQL
   case mysql_api:
     if (limit>0) {sprintf(SQL_APPEND, " LIMIT %d", limit);}
     if (mysql_real_query(&(db_ref->mysql), sql_query, strlen(sql_query)) != 0) {
         sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query);
         return (-2);}
     if ((query_results = mysql_store_result(&(db_ref->mysql))) == NULL) {
        sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query);
        return (-2);}
     while ((row = mysql_fetch_row(query_results))) {
	     lengths = (int *)mysql_fetch_lengths(query_results);
	StringRowAdd_b(values, row, num_cols, lengths); j++;
     }
     mysql_free_result(query_results);
  #ifdef HAVE_ODBC
	 // we also have to free this in mysql mode
     free(k); free(sql_row);
  #endif     
     return ((long) j);
#endif
	 
#ifdef HAVE_ODBC
   case odbc:
     if (odbc_driver==odbc_mysql && limit>0) {sprintf(SQL_APPEND, " LIMIT %d ", limit);}
     SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
     lengths = malloc(sizeof(int) * num_cols);
     if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {ODBC_ERROR}
     while (SQLFetchScroll(db_ref->hstmt, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
       for (i=0; i<num_cols ; i++) {
	 switch (SQLGetData(db_ref->hstmt, i+1, SQL_BINARY, sql_row[i], 0, (SQLLEN *)&lengths[i])) {
	   case SQL_SUCCESS:
	   case SQL_SUCCESS_WITH_INFO:
	     switch (lengths[i]) {
	     	case SQL_NULL_DATA:
		  lengths[i] = 0;
		break;
 		case SQL_NO_TOTAL:
		  lengths[i] = 40;
		default:
		  sql_row[i] = malloc(lengths[i]+1);
		  if (SQLGetData(db_ref->hstmt, i+1, SQL_BINARY, sql_row[i], lengths[i]+1, NULL) == SQL_ERROR) {ODBC_ERROR}
	        break;
	        }
	     break;
	   case SQL_ERROR:
	     lengths[i] = 0;
	     if (sql_warnings++ < 20) {
		 SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, 1024, 0);
  #ifdef WIN
		 MessageBox(0, msg_debug, "SQL Warning", 0);
  #else
		 fprintf(stderr, msg_debug);
  #endif
	     }
	     break;
	   default:
	     ODBC_ERROR
	    }
       } //for
       StringRowAdd_b(values, sql_row, num_cols, lengths); j++;
       SQLFreeStmt(db_ref->hstmt, SQL_UNBIND);
       for (i=0; i< num_cols; i++) {free(sql_row[i]);}
     } // while
     SQLFreeStmt(db_ref->hstmt, SQL_CLOSE);
     free(sql_row); free(lengths); free (k);
     return ((long) j);
#endif
   default:
     return (0);
     break;
  }
}


long sql_insert(unsigned short int operation, char *return_id, unsigned long *db, char table[], TD4Hdl values, 
	TD2Hdl fields, long ids[], LStrHandle debug)
{
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN], idx_field[80], idx_value[80];
int i, j, k, l, idx_rtn;
SQL_LV_REF *db_ref;
unsigned short int odbc_driver, db_type;

if (*db == 0L) return -1;

db_ref = (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
/*
 *  build SQL "INSERT" and post to DB
 */
  k = (**values).dimSizes[0];
  l = (**values).dimSizes[1];
#ifdef HAVE_ODBC
  if (odbc_driver==odbc_oracle && db_type == odbc) {
    SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
    if (SQLExecDirect(db_ref->hstmt, "ALTER SESSION SET NLS_DATE_FORMAT = \"dd-Month-YYYY hh24:mi:ss\"", SQL_NTS) != SQL_SUCCESS) {
      ODBC_ERROR}
    SQLFreeStmt(db_ref->hstmt, SQL_DROP);}
#endif

  for (i=0; i<k ; i++) {
    if (operation == 1) strcpy(sql_query, "REPLACE INTO ");
    else strcpy(sql_query, "INSERT INTO ");
    strcat(sql_query, table);
    strcat(sql_query, " (");
#ifdef HAVE_ODBC
    if (*return_id && db_type == odbc) {
      if ((idx_rtn = idx_find(idx_field, db, table)) <= 0) return(idx_rtn);
      strcat(sql_query, idx_field); strcat(sql_query, ", ");}
#endif
    strncat(sql_query, (**(**fields).String[0]).str, (**(**fields).String[0]).cnt);
    for (j=1; j<((**fields).dimSize) ;j++) {    /* field names */
        strcat(sql_query, ", ");
        strncat(sql_query, (**(**fields).String[j]).str, (**(**fields).String[j]).cnt);}
    strcat(sql_query, ") VALUES (");

#ifdef HAVE_ODBC
     if (*return_id && db_type == odbc) {
        idx_rtn++; sprintf(idx_value, "%d, ", idx_rtn);
        strcat(sql_query, idx_value);}
#endif
    strcat(sql_query, "'");
    strncat(sql_query, (**(**values).String[i*l]).str, (**(**values).String[i*l]).cnt);
    for (j=1; j<l ; j++) {    /* field values */
        strcat(sql_query, "', '");
        strncat(sql_query, (**(**values).String[j+i*l]).str, (**(**values).String[j+i*l]).cnt);}
    strcat(sql_query, "')");
/*
 *  query built, now post to DB
 */
    switch (db_type) {
#ifdef HAVE_MYSQL
        case mysql_api:
            if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
            sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query);
            return (-2);}
            break;
#endif
#ifdef HAVE_ODBC
        case odbc:
            SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
            if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {ODBC_ERROR}
            SQLFreeStmt(db_ref->hstmt, SQL_DROP);
            break;
#endif
#ifdef HAVE_ORACLE
        case oracle_api:
            checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                    OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
            checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                            (ub4) strlen(sql_query),
                                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
            checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 1, (ub4) 0,
                        (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_COMMIT_ON_SUCCESS)), msg_debug); OCI_CHECKERR;
            break;
#endif
        default:
	break;
     }

    if (*return_id) {
      switch (db_type) {
#ifdef HAVE_MYSQL
        case mysql_api:
            ids[i] = mysql_insert_id(&(db_ref->mysql));
            break;
#endif
#ifdef HAVE_ODBC
        case odbc:
            ids[i] = idx_rtn;
            break;
#endif
#ifdef HAVE_ORACLE
        case oracle_api:    /*  oracle uses IDENTITY, not auto-increment, so a manual work-around is to use "SELECT MAX(idx)+1"   */
            ids[i] = 0;
            break;
#endif
        default:
            break;
       }
     }
   }
  return (0);
}



long sql_insert_b(unsigned short int operation, char *return_id, unsigned long *db, char table[], TD4Hdl values, 
	TD2Hdl fields, long ids[], LStrHandle debug)
{
char *sql_query, msg_debug[DEBUG_LEN], idx_field[80], idx_value[80];
int i, j, k, l, idx_rtn, m, last_m;
SQL_LV_REF *db_ref;
unsigned short int odbc_driver, db_type;

/*
 * unsigned long mysql_real_escape_string(MYSQL *mysql, char *to, const char *from, unsigned long length);
 */
if (*db == 0L) return -1;

db_ref =  (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;

sql_query = malloc(5000);
/*
 *  build SQL "INSERT" and post to DB
 */
  k = (**values).dimSizes[0];
  l = (**values).dimSizes[1];
#ifdef HAVE_ODBC
  if (odbc_driver==odbc_oracle && db_type == odbc) {
    SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
    if (SQLExecDirect(db_ref->hstmt, "ALTER SESSION SET NLS_DATE_FORMAT = \"dd-Month-YYYY hh24:mi:ss\"", SQL_NTS) != SQL_SUCCESS) {
      SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, 1024, 0);
      sql_error(&debug, msg_debug, "ALTER SESSION SET...");
      return (-2);}
    SQLFreeStmt(db_ref->hstmt, SQL_DROP);}
#endif

  last_m = 0;
  for (i=0; i<k ; i++) {
/*
 *    allocate space for big data
 */
    m=0; for (j=0; j<l; j++) {m += (**(**values).String[j+i*l]).cnt * 2;}
    if (m > last_m) {
        if(sql_query != NULL) free (sql_query);
        sql_query = malloc(5000 + m); last_m = m;
    }

    if (operation == 1) strcpy(sql_query, "REPLACE INTO ");
    else strcpy(sql_query, "INSERT INTO ");
    strcat(sql_query, table);
    strcat(sql_query, " (");
#ifdef HAVE_ODBC
    if (*return_id && db_type == odbc) {
      if ((idx_rtn = idx_find(idx_field, db, table)) <= 0) return(idx_rtn);
      strcat(sql_query, idx_field); strcat(sql_query, ", ");}
#endif
    strncat(sql_query, (**(**fields).String[0]).str, (**(**fields).String[0]).cnt);
    for (j=1; j<l ;j++) {
      strcat(sql_query, ", ");
      strncat(sql_query, (**(**fields).String[j]).str, (**(**fields).String[j]).cnt);}
    strcat(sql_query, ") VALUES (");

#ifdef HAVE_ODBC
     if (*return_id && db_type == odbc) {
       idx_rtn++; sprintf(idx_value, "%d, ", idx_rtn);
       strcat(sql_query, idx_value);}
#endif
    switch (db_type) {
#ifdef HAVE_MYSQL
      case mysql_api:
		strcat(sql_query, "'");
		mysql_real_escape_string(&(db_ref->mysql), sql_query + strlen(sql_query),
								 (**(**values).String[i*l]).str, (**(**values).String[i*l]).cnt);
		for (j=1; j<l ; j++) {
		  strcat(sql_query, "', '");
		  mysql_real_escape_string(&(db_ref->mysql), sql_query + strlen(sql_query),
								   (**(**values).String[j+i*l]).str, (**(**values).String[j+i*l]).cnt);
					   }
		strcat(sql_query, "')");
		break;
#endif
#ifdef HAVE_ODBC
      case odbc:
        strcat(sql_query, "?");
		for (j=1; j<l ; j++) {strcat(sql_query, ", ?");}
		strcat(sql_query, ")");
        break;
#endif
      default:
	break;
    }
      
    switch (db_type) {
#ifdef HAVE_MYSQL
      case mysql_api:
		if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
			if (strlen (sql_query) > 256) {sql_query[250] = '\0'; strcat(sql_query,"..."); }
		  sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query);
		  if(sql_query != NULL) free (sql_query);
		  return (-2);
		  }
		break;
#endif
#ifdef HAVE_ODBC
      case odbc:
		SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
		for (j=0; j<l ; j++) {SQLBindParameter(db_ref->hstmt, j+1, SQL_PARAM_INPUT, SQL_C_BINARY, 
												   SQL_LONGVARBINARY, (**(**values).String[j+i*l]).cnt,
							   0,(**(**values).String[j+i*l]).str, (**(**values).String[j+i*l]).cnt,
							   &((**(**values).String[j+i*l]).cnt));}
		if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {ODBC_ERROR}
		for (j=1; j<l ; j++) {
		  SQLParamData(db_ref->hstmt, 0);
		  SQLPutData(db_ref->hstmt, (**(**values).String[j+i*l]).str, (**(**values).String[j+i*l]).cnt);}
		SQLFreeStmt(db_ref->hstmt, SQL_DROP);
		break;
#endif
      default:
	break;
     }

    if (*return_id) {
      switch (db_type) {
#ifdef HAVE_MYSQL
		case mysql_api:
			ids[i] = mysql_insert_id(&(db_ref->mysql));
			break;
#endif
#ifdef HAVE_ODBC
		case odbc:
			ids[i] = idx_rtn;
			break;
#endif
		default:
			break;
       }
     }
   }
  if(sql_query != NULL) free (sql_query);	  
  return (0);
}


long sql_update(unsigned long *db, char table[], TD4Hdl values, TD2Hdl fields, 
	TD2Hdl where, LStrHandle debug, long *num_rows)
{
  char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN], debug_str[DEBUG_LEN];
  int i, j, k, l;
  unsigned short int odbc_driver, db_type;

#ifdef HAVE_ODBC
  SQLLEN m;
#endif
SQL_LV_REF *db_ref;

if (*db == 0L) return -1;

*num_rows = 0;

db_ref =  (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
/*
 *  build SQL "UPDATE" and execute to DB
 */
  k = (**values).dimSizes[0];
  if (k != (**where).dimSize) {
	sprintf(debug_str,"Silly programmer mistake: wheres=%d, rows=%d", (**where).dimSize, k);
	LV_str_cp(debug, debug_str); return (-2);}
  l = (**values).dimSizes[1];
  if (l != (**fields).dimSize) {
    sprintf(debug_str,"Silly programmer mistake: columns=%d, values=%d", (**fields).dimSize, l);
	LV_str_cp(debug, debug_str); return (-2);}
	
  for (i=0; i<k ; i++) {    /*    loop through updates array    */
    sprintf(sql_query, "UPDATE %s SET ", table);
    strncat(sql_query, (**(**fields).String[0]).str, (**(**fields).String[0]).cnt); strcat(sql_query, "='");
    strncat(sql_query, (**(**values).String[i*l]).str, (**(**values).String[i*l]).cnt); strcat(sql_query, "'");
    for (j=1; j<((**fields).dimSize) ;j++) {
        strcat(sql_query, ", ");
        strncat(sql_query, (**(**fields).String[j]).str, (**(**fields).String[j]).cnt);
        strcat(sql_query, "='");
        strncat(sql_query, (**(**values).String[j+i*l]).str, (**(**values).String[j+i*l]).cnt);
        strcat(sql_query, "'");}

    strcat(sql_query, " WHERE ");
    strncat(sql_query, (**(**where).String[i]).str, (**(**where).String[i]).cnt);

    switch (db_type) {
#ifdef HAVE_MYSQL
        case mysql_api:
            if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
                sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query);
                return (-2);}
            *num_rows += mysql_affected_rows(&(db_ref->mysql));
            break;
#endif
#ifdef HAVE_ODBC
        case odbc:
            SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
            if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {
                SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, DEBUG_LEN, 0);
                sql_error(&debug, msg_debug, sql_query);
                return (-2);}
            SQLRowCount(db_ref->hstmt, &m);
            *num_rows += m;
            SQLFreeStmt(db_ref->hstmt, SQL_CLOSE);
            break;
#endif
#ifdef HAVE_ORACLE
        case oracle_api:
            checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                    OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
            checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                            (ub4) strlen(sql_query),
                                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
            checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 1, (ub4) 0,
                        (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_COMMIT_ON_SUCCESS)), msg_debug); OCI_CHECKERR;
            break;
#endif
        default:
            break;
    }
  }
  return (0);
}


long sql_delete(unsigned long *db, char del_table[], char table[], TD2Hdl where, LStrHandle debug, long *num_rows)
{
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN];
int i, j;
unsigned short int odbc_driver, db_type;
#ifdef HAVE_ODBC
  SQLLEN k;
#endif
SQL_LV_REF *db_ref;

if (*db == 0L) return -1;

*num_rows = 0;

db_ref =  (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
/*
 *  build SQL "DELETE" and post to DB
 */
  j = (**where).dimSize;
  for (i=0; i<j ; i++) {    /*    loop through delete array    */
    switch (strlen(del_table)) {
        case 0:
        case 1:
            strcpy(sql_query, "DELETE FROM ");
        break;
            default:
            sprintf(sql_query, "DELETE %s FROM ", del_table);
            break;
    }
    strcat(sql_query, table);
    strcat(sql_query, " WHERE ");
    strncat(sql_query, (**(**where).String[i]).str, (**(**where).String[i]).cnt);

   switch (db_type) {
#ifdef HAVE_MYSQL
        case mysql_api:
            if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
                sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
            *num_rows += mysql_affected_rows(&(db_ref->mysql));
            break;
#endif
#ifdef HAVE_ODBC
        case odbc:
            SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
            if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {
                SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, 1024, 0);
                sql_error(&debug, msg_debug, sql_query); return (-2);}
            SQLRowCount(db_ref->hstmt, &k);
            *num_rows += k;
            SQLFreeStmt(db_ref->hstmt, SQL_CLOSE);
            break;
#endif
#ifdef HAVE_ORACLE
        case oracle_api:
            checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                    OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
            checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                            (ub4) strlen(sql_query),
                                            (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
            checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 1, (ub4) 0,
                        (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_COMMIT_ON_SUCCESS)), msg_debug); OCI_CHECKERR;
            break;
#endif
     default:
       break;
    }
  }
  return (0);
}


long sql_cmd(unsigned long *db, TD2Hdl sql_cmds, LStrHandle debug)
{
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN];
unsigned char chr;
int i, j, m, n;
SQL_LV_REF *db_ref;
unsigned short int odbc_driver, db_type;

if (*db == 0L) return -1;

db_ref =  (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
/*
 *  send SQL statement to DB
 */
  j = (**sql_cmds).dimSize;
  for (i=0; i<j; i++) {    /*    loop through command array    */
    n=0;
    for (m=0; m<(**(**sql_cmds).String[i]).cnt ; m++) {
        chr = *((**(**sql_cmds).String[i]).str+m);
        if (chr >0 && chr <128) *(sql_query + n++) = chr;} *(sql_query + n++) = 0;

    if (strlen(sql_query)>2) {
        switch (db_type) {
#ifdef HAVE_MYSQL
            case mysql_api:
				if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
					sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
				break;
#endif
#ifdef HAVE_ODBC
            case odbc:
                SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
                if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {
                    SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, DEBUG_LEN, 0);
                    sql_error(&debug, msg_debug, sql_query); return (-2);}
                SQLFreeStmt(db_ref->hstmt, SQL_CLOSE);
                break;
#endif
#ifdef HAVE_ORACLE
            case oracle_api:
                checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                        OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
                checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                                (ub4) strlen(sql_query),
                                                (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
                checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 1, (ub4) 0,
                            (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_COMMIT_ON_SUCCESS)), msg_debug); OCI_CHECKERR;
                break;
#endif
        default:
        break;
        }
    }
  }
  return (0);
}


long sql_cmd_resp(unsigned long *db, char sql_cmd[],
                                  TD2Hdl col_headers, TD4Hdl values, LStrHandle debug)
{
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN], col_name[256];
int i, j=0;

#ifdef HAVE_MYSQL
  MYSQL_ROW row; MYSQL_FIELD *field;
#endif

#ifdef HAVE_ODBC
 static char **sql_row, *cache_LV;
 static unsigned int sql_warnings=0;
 SQLLEN *k;
 SQLSMALLINT num_cols;
#else
 char **sql_row;
 int num_cols;
#endif
SQL_LV_REF *db_ref;
unsigned short int odbc_driver, db_type;

if (*db == 0L) return -1;

db_ref =  (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
/*
 *  send SQL statement to DB
 */
  strcpy(sql_query, sql_cmd);

  if (strlen(sql_query)>2) {
    switch (db_type) {
#ifdef HAVE_MYSQL
        case mysql_api:
            if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
                sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
            if ((db_ref->query_results = mysql_store_result(&(db_ref->mysql))) == NULL) {
                sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
            num_cols = mysql_num_fields(db_ref->query_results);
            while ((row = mysql_fetch_row(db_ref->query_results))) {StringRowAdd(values, row, num_cols); j++;}
            while((field = mysql_fetch_field(db_ref->query_results))){StringAdd(col_headers, field->name);}
            mysql_free_result(db_ref->query_results);
            break;
#endif
#ifdef HAVE_ODBC
        case odbc:
            SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
            if (SQLExecDirect(db_ref->hstmt, sql_query, SQL_NTS) != SQL_SUCCESS) {
                SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, DEBUG_LEN, 0);
                sql_error(&debug, msg_debug, sql_query); return (-2);}
            SQLNumResultCols(db_ref->hstmt, &num_cols);
            k = malloc(sizeof(SQLLEN) * num_cols); sql_row = malloc(sizeof(char*) * num_cols);
            for (i=0; i< num_cols; i++) {sql_row[i] = malloc(SQL_FIELD);}
            for (i=0; i<num_cols ; i++) {
                SQLBindCol(db_ref->hstmt, i+1, SQL_CHAR, sql_row[i], SQL_FIELD-1, &k[i]);}
            while (SQLFetchScroll(db_ref->hstmt, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
                StringRowAdd(values, sql_row, num_cols); j++;}
            for (i=0; i<num_cols ; i++) {
                SQLColAttribute(db_ref->hstmt, (SQLSMALLINT)(i + 1), SQL_DESC_NAME, 
                                col_name, 256, NULL, NULL);
                StringAdd(col_headers, col_name);}
            SQLFreeStmt(db_ref->hstmt, SQL_CLOSE);
            for (i=0; i< num_cols; i++) {free(sql_row[i]);} free(sql_row);
            break;
#endif
#ifdef HAVE_ORACLE
   case oracle_api:
            checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                    OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
            checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                        (ub4) strlen(sql_query),
                                        (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
                    /* execute */
            checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 0, (ub4) 0,
                        (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
                        
            /* define variable assignment */
            i=0;
            sql_row = malloc(sizeof(char*) * 1); sql_row[0] = malloc(SQL_FIELD);
            while((retCode = OCIDefineByPos(stmthp, &defnp, db_ref->errhp, i+1,
                        (dvoid *) sql_row[i], (sword) SQL_FIELD-1, SQLT_STR ,
                        (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)) == 0) {
                i++; num_cols = i;
                sql_row = realloc(sql_row, sizeof(char*) * num_cols+1);
                sql_row[i] = malloc(SQL_FIELD);}

                    /* fetch */
            while ((retCode = OCIStmtFetch2 (stmthp, db_ref->errhp, (ub4) 1, OCI_DEFAULT, (ub4) 0, OCI_DEFAULT)) == OCI_SUCCESS){
                StringRowAdd(values, sql_row, num_cols); j++;
            }
            checkerr(db_ref->errhp, retCode, msg_debug); if (retCode == OCI_ERROR) {LV_str_cp(debug, msg_debug); return(-2);};
            for (i=0; i< num_cols; i++) {free(sql_row[i]);} free(sql_row);
            return ((long) j);
#endif
        default:
            break;
    }
  }
  return (0);
}


long sql_tables(unsigned long *db, CStr schema, TD2Hdl values, LStrHandle debug)
{
/* open db and count the SELECT quantity. */
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN];
#ifdef HAVE_MYSQL
  MYSQL_RES *query_results; MYSQL_ROW row;
#endif
int j;
char odbc_row[DEBUG_LEN];
SQL_LV_REF *db_ref;
unsigned short int db_type;

if (*db == 0L) return -1;

db_ref =   (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type;
strcpy(sql_query, "SHOW TABLES");
 
 switch (db_type) {
#ifdef HAVE_MYSQL
    case mysql_api:
        if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
            sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
        if ((query_results = mysql_store_result(&(db_ref->mysql))) == NULL) {
            sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
        j=0;
        while ((row = mysql_fetch_row(query_results))) {
            StringAdd(values, row[0]);
            j++;}
        mysql_free_result(query_results);
        return (j);
#endif
#ifdef HAVE_ODBC
    case odbc:
        switch ((*db_ref).odbc_driver) {
            case odbc_oracle: break;
            case odbc_access: schema=NULL; break;
            default: strcpy(schema, ""); break; }
        SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt)); j=0;
        if (SQLTables(db_ref->hstmt, NULL, SQL_NTS, NULL, SQL_NTS, NULL, SQL_NTS, "TABLE", SQL_NTS) != SQL_SUCCESS) {
            SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, DEBUG_LEN, 0);
            sql_error(&debug, msg_debug, sql_query); return (-2);}
        if (SQLBindCol(db_ref->hstmt, 3, SQL_CHAR, odbc_row, DEBUG_LEN-1, NULL) != SQL_SUCCESS) {
            SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, DEBUG_LEN, 0);
            sql_error(&debug, msg_debug, sql_query); return (-2);};
        while (SQLFetchScroll(db_ref->hstmt, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
            StringAdd(values, odbc_row);
            j++;}
        SQLFreeStmt(db_ref->hstmt, SQL_DROP);
        if (j==0) /* MessageBox(0, schema, "error", 0); */
        return (j);
#endif
#ifdef HAVE_ORACLE
    case oracle_api:
        strcpy(sql_query, "SELECT table_name FROM dba_tables WHERE owner=sys_context('USERENV', 'SESSION_USER')");
        checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
        checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                    (ub4) strlen(sql_query),
                                    (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
                /* define variable assignment */
        checkerr(db_ref->errhp, (retCode = OCIDefineByPos(stmthp, &defnp, db_ref->errhp, 1,
                (dvoid *) odbc_row, (sword) DEBUG_LEN-1, SQLT_STR ,
                (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)), msg_debug);
        OCI_CHECKERR;
                /* execute and fetch */
        checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 0, (ub4) 0,
                    (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
        while ((retCode = OCIStmtFetch2 (stmthp, db_ref->errhp, (ub4) 1, OCI_DEFAULT, (ub4) 0, OCI_DEFAULT)) == OCI_SUCCESS){
            StringAdd(values, odbc_row); j++;
        }
        checkerr(db_ref->errhp, retCode, msg_debug); if (retCode == OCI_ERROR) {LV_str_cp(debug, msg_debug); return(-2);};
        return (j);
#endif
    default:
        return (0);
        break;
  }
}


long sql_cols(unsigned long *db, char table[], TD2Hdl values, LStrHandle debug)
{
/* open db and count the SELECT quantity. */
char sql_query[QUERY_LEN], msg_debug[DEBUG_LEN];
#ifdef HAVE_MYSQL
  MYSQL_RES *query_results; MYSQL_ROW row;
#endif
int j=0;
char odbc_row[1024];
SQL_LV_REF *db_ref;
unsigned short int db_type;

if (*db == 0L) return -1;

db_ref =  (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type;
sprintf(sql_query, "DESCRIBE %s", table);
 
 switch (db_type) {
#ifdef HAVE_MYSQL
    case mysql_api:
        if (mysql_query(&(db_ref->mysql), sql_query) != 0) {
            sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
        if ((query_results = mysql_store_result(&(db_ref->mysql))) == NULL) {
            sql_error(&debug, (char *) mysql_error(&(db_ref->mysql)), sql_query); return (-2);}
        while ((row = mysql_fetch_row(query_results))) {
            StringAdd(values, row[0]);
            j++;}
        mysql_free_result(query_results);
        return (j);
#endif
#ifdef HAVE_ODBC
    case odbc:
        SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
        if (SQLColumns(db_ref->hstmt, NULL, SQL_NTS, NULL, SQL_NTS, table, SQL_NTS, NULL, SQL_NTS) != SQL_SUCCESS) {
            SQLGetDiagRec(SQL_HANDLE_STMT, db_ref->hstmt, 1, 0, 0, msg_debug, DEBUG_LEN, 0);
            sql_error(&debug, msg_debug, sql_query); return (-2);}
        SQLBindCol(db_ref->hstmt, 4, SQL_CHAR, odbc_row, SQL_FIELD-1, NULL);
        while (SQLFetchScroll(db_ref->hstmt, SQL_FETCH_NEXT, 0) != SQL_NO_DATA) {
            StringAdd(values, odbc_row); j++;}
        SQLFreeStmt(db_ref->hstmt, SQL_DROP);
        return (j);
#endif
#ifdef HAVE_ORACLE
    case oracle_api:
        sprintf(sql_query, "SELECT column_name FROM user_tab_cols WHERE table_name = '%s'", table);
        checkerr(db_ref->errhp, (retCode = OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &stmthp,
                OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
        checkerr(db_ref->errhp, (retCode = OCIStmtPrepare(stmthp, db_ref->errhp, sql_query,
                                    (ub4) strlen(sql_query),
                                    (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
                /* define variable assignment */
        checkerr(db_ref->errhp, (retCode = OCIDefineByPos(stmthp, &defnp, db_ref->errhp, 1,
                (dvoid *) odbc_row, (sword) DEBUG_LEN-1, SQLT_STR ,
                (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)), msg_debug);
        OCI_CHECKERR;
                /* execute and fetch */
        checkerr(db_ref->errhp, (retCode = OCIStmtExecute(db_ref->svchp, stmthp, db_ref->errhp, (ub4) 0, (ub4) 0,
                    (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
        while ((retCode = OCIStmtFetch2 (stmthp, db_ref->errhp, (ub4) 1, OCI_DEFAULT, (ub4) 0, OCI_DEFAULT)) == OCI_SUCCESS){
            StringAdd(values, odbc_row); j++;
        }
        checkerr(db_ref->errhp, retCode, msg_debug); if (retCode == OCI_ERROR) {LV_str_cp(debug, msg_debug); return(-2);};
        return (j);
#endif
    default:
        return (0);
        break;
  }
}



long sql_info(unsigned long *db_ref, unsigned short int *driver, unsigned short int *type, 
	LStrHandle info_string, LStrHandle debug)
{
#define BUFF_SIZE 256
    char DriverName[BUFF_SIZE];
    SQL_LV_REF *db;
#ifdef HAVE_ODBC
    SQLSMALLINT     outlen;
#endif
#ifdef HAVE_ORACLE
    sword  retCode;
    char* sql_query = "SELECT Banner FROM v$version WHERE Banner LIKE 'Oracle%'";
    char ora_buff[DEBUG_LEN], msg_debug[DEBUG_LEN];
#endif

    if (*db_ref == 0L) return -1;

    db =  (SQL_LV_REF *)*db_ref;
    *type = db->db_type;

    switch (db->db_type) {
#ifdef HAVE_MYSQL
        case mysql_api:
            sprintf(DriverName, "%s\nMySQL\n%s", SQL_LV_VERSION, mysql_get_server_info(&db->mysql));
            break;
#endif
#ifdef HAVE_ODBC
        case odbc:
            sprintf(DriverName, "%s\n", SQL_LV_VERSION);
            SQLGetInfo (db->hdbc,  SQL_DBMS_NAME, DriverName + strlen(DriverName), BUFF_SIZE - strlen(DriverName), &outlen);
            strcat(DriverName, "\n");
            SQLGetInfo (db->hdbc,  SQL_DBMS_VER,  DriverName + strlen(DriverName), BUFF_SIZE - strlen(DriverName), &outlen);
            break;
#endif
#ifdef HAVE_ORACLE
        case oracle_api:
            sprintf(DriverName, "%s\n", SQL_LV_VERSION);
            checkerr(db->errhp, (retCode = OCIHandleAlloc( (dvoid *) db->envhp, (dvoid **) &stmthp,
                    OCI_HTYPE_STMT, (size_t) 0, (dvoid **) 0)), msg_debug); OCI_CHECKERR;
            checkerr(db->errhp, (retCode = OCIStmtPrepare(stmthp, db->errhp, sql_query,
                                        (ub4) strlen(sql_query),
                                        (ub4) OCI_NTV_SYNTAX, (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
                    /* define variable assignment */
            checkerr(db->errhp, (retCode = OCIDefineByPos(stmthp, &defnp, db->errhp, 1,
                    (dvoid *) ora_buff, (sword) SQL_FIELD-1, SQLT_STR ,
                    (dvoid *) 0, (ub2 *) 0, (ub2 *) 0, OCI_DEFAULT)), msg_debug);
            OCI_CHECKERR;
                    /* execute and fetch */
            checkerr(db->errhp, (retCode = OCIStmtExecute(db->svchp, stmthp, db->errhp, (ub4) 0, (ub4) 0,
                        (CONST OCISnapshot *) NULL, (OCISnapshot *) NULL, OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
            
            if ((retCode = OCIStmtFetch2 (stmthp, db->errhp, (ub4) 1, OCI_DEFAULT, (ub4) 0, OCI_DEFAULT)) == OCI_SUCCESS){
                strcat(DriverName, ora_buff);
            }
            else {checkerr(db->errhp, retCode, msg_debug); if (retCode == OCI_ERROR) {LV_str_cp(debug, msg_debug); return(-2);}}
            break;
#endif
    }
    *driver = db->odbc_driver;
 
    LV_str_cp(info_string, DriverName);
    return(0);
}


long sql_open(DB_LOGIN *login, unsigned long  *db, long *driver, LStrHandle debug)
{
int i, SQL_State;
char host[80], user[80], passwd[80], db_nm[80], buff[256];
unsigned int port;
char debug_str[1024], msg_debug[DEBUG_LEN];
char DriverName[256], ConnStr[1024], ConnStrOut[1024];
#ifdef HAVE_ODBC
SQLSMALLINT     outlen;
#endif
#ifdef HAVE_ORACLE
 sword retCode;
 char EasyConnect[1024];
#endif
SQL_LV_REF *db_ref;
 unsigned short int odbc_driver, db_type;

 buff[(**(login->host)).cnt] = 0;
 for (i=(**(login->host)).cnt; i>0; i--) {buff[i-1] = (**(login->host)).str[i-1];}
 if (sscanf(buff, "%[^:]:%d", host, &port) != 2) {port = 0; strcpy(host, buff);}
 user[(**(login->user)).cnt] = 0;
 for (i=(**(login->user)).cnt; i>0; i--) {user[i-1] = (**(login->user)).str[i-1];}
 passwd[(**(login->password)).cnt] = 0;
 for (i=(**(login->password)).cnt; i>0; i--) {passwd[i-1] = (**(login->password)).str[i-1];}
 db_nm[(**(login->db)).cnt] = 0;
 for (i=(**(login->db)).cnt; i>0; i--) {db_nm[i-1] = (**(login->db)).str[i-1];}
 db_type = (*login).db_type;

 db_ref = (SQL_LV_REF *) malloc(sizeof(SQL_LV_REF));
 *db = (unsigned long)db_ref;
 db_ref->db_type = db_type;
/*
 *  open database connection
 */
 switch (db_ref->db_type) {
#ifdef HAVE_MYSQL
    case mysql_api:
        mysql_init(&db_ref->mysql);
        if (!(mysql_real_connect(&db_ref->mysql, host, user, passwd, db_nm, port, 0, CLIENT_MULTI_STATEMENTS))) {
            sprintf(debug_str, "%s\nhost: %s\nuser: %s\npassword: %s\n", mysql_error(&db_ref->mysql), host, user, passwd);
            LV_str_cp(debug, debug_str); return(-1);}
        db_ref->odbc_driver=odbc_mysql;
        break;
#endif
#ifdef HAVE_ODBC
    case odbc:
        SQLAllocEnv(&db_ref->henv);
        SQLAllocConnect(db_ref->henv, &db_ref->hdbc);
        
        if (strlen(db_nm) > 1) {sprintf(ConnStr, "DSN=%s;UID=%s;PWD=%s;XSM=%s", host, user, passwd, db_nm);}
        else {sprintf(ConnStr, "DSN=%s;UID=%s;PWD=%s", host, user, passwd);}
        if ((SQL_State = SQLDriverConnect(db_ref->hdbc, NULL, ConnStr, strlen(ConnStr), ConnStrOut, 1024, NULL, SQL_DRIVER_NOPROMPT)) != 
            SQL_SUCCESS && SQL_State != SQL_SUCCESS_WITH_INFO) {
            SQLGetDiagRec(SQL_HANDLE_DBC, db_ref->hdbc, 1, 0, 0, msg_debug, 1024, 0);
            SQLAllocEnv(&db_ref->henv);
            SQLAllocConnect(db_ref->henv, &db_ref->hdbc);
            sprintf(ConnStr, "DRIVER=Firebird/InterBase(r) driver;UID=%s;PWD=%s; DBNAME=%s", user, passwd, db_nm);
            if (SQLDriverConnect(db_ref->hdbc, NULL, ConnStr, strlen(ConnStr), ConnStrOut, 1024, NULL, SQL_DRIVER_NOPROMPT) == SQL_SUCCESS) {
                odbc_driver=odbc_fb; break;}
            sprintf(debug_str, "%s\nhost: %s\nuser: %s\npassword: %s\n", msg_debug, host, user, passwd);
            LV_str_cp(debug, debug_str);
            return(-1);}
        SQLGetInfo (db_ref->hdbc,  SQL_DBMS_NAME, DriverName, 256, &outlen);
        strcat(DriverName, "\n");
        SQLGetInfo (db_ref->hdbc,  SQL_DBMS_VER, DriverName + strlen(DriverName), 256 - strlen(DriverName), &outlen);

        for (i=0; i<(int) (strlen(DriverName)-strlen("MySQL")+1); i++) {
        if (strncmp(DriverName+i, "MySQL", strlen("MySQL"))==0) {db_ref->odbc_driver=odbc_mysql;break;}}
        for (i=0; i<(int) (strlen(DriverName)-strlen("MS Access")+1); i++) {
        if (strncmp(DriverName+i, "MS Access", strlen("MS Access"))==0) {db_ref->odbc_driver=odbc_access;break;}}
        for (i=0; i<(int) (strlen(DriverName)-strlen("SQL Server")+1); i++) {
        if (strncmp(DriverName+i, "SQL Server", strlen("SQL Server"))==0) {db_ref->odbc_driver=odbc_sqlserver;break;}}
        for (i=0; i<(int) (strlen(DriverName)-strlen("PostgreSQL")+1); i++) {
        if (strncmp(DriverName+i, "PostgreSQL", strlen("PostgreSQL"))==0) {db_ref->odbc_driver=odbc_postgreSQL;break;}}
        break;
#endif
#ifdef HAVE_ORACLE
    case oracle_api:
        sprintf(EasyConnect, "%s:%d/%s", host, ((port==0) ? 1521: port), db_nm);
        retCode = OCIEnvCreate((OCIEnv **) &db_ref->envhp, (ub4) OCI_DEFAULT,
                (dvoid *) 0, (dvoid * (*)(dvoid *,size_t)) 0,
                (dvoid * (*)(dvoid *, dvoid *, size_t)) 0,
                (void (*)(dvoid *, dvoid *)) 0, (size_t) 0, (dvoid **) 0);

        if (retCode != 0) {
            (void) sprintf(debug_str, "OCIEnvCreate failed with retCode = %d.\n", retCode);
            LV_str_cp(debug, debug_str); return(-1);}
            
        (void) OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &db_ref->errhp, OCI_HTYPE_ERROR,
                    (size_t) 0, (dvoid **) 0);   /* server contexts */
        (void) OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &db_ref->srvhp, OCI_HTYPE_SERVER,
                        (size_t) 0, (dvoid **) 0);
        (void) OCIHandleAlloc( (dvoid *) db_ref->envhp, (dvoid **) &db_ref->svchp, OCI_HTYPE_SVCCTX,
                        (size_t) 0, (dvoid **) 0);
        retCode =  OCIServerAttach( db_ref->srvhp, db_ref->errhp, (text *) EasyConnect,    /* Connect to the target */
                                    strlen((const char *) EasyConnect), 0);
        if (-1 == retCode) {
            (void) sprintf(debug_str, "The following TNS Service Name or Easy Connect Name is invalid:\n   %s\n\n", EasyConnect);
            LV_str_cp(debug, debug_str); return(-1);}

        /* set attribute server context in the service context */
        (void) OCIAttrSet( (dvoid *) db_ref->svchp, OCI_HTYPE_SVCCTX, (dvoid *)db_ref->srvhp,
                            (ub4) 0, OCI_ATTR_SERVER, (OCIError *) db_ref->errhp);
        db_ref->authp = (OCISession *) 0;
        (void) OCIHandleAlloc((dvoid *) db_ref->envhp, (dvoid **)&db_ref->authp,
                                (ub4) OCI_HTYPE_SESSION, (size_t) 0, (dvoid **) 0);
        (void) OCIAttrSet((dvoid *) db_ref->authp, (ub4) OCI_HTYPE_SESSION,  /* Set the username */
                        (dvoid *) user, (ub4) strlen((char *)user),
                        (ub4) OCI_ATTR_USERNAME, db_ref->errhp);
        (void) OCIAttrSet((dvoid *) db_ref->authp, (ub4) OCI_HTYPE_SESSION,   /* Set the password */
                        (dvoid *) passwd, (ub4) strlen((char *)passwd),
                        (ub4) OCI_ATTR_PASSWORD, db_ref->errhp);
        /* Try to authenicate on the target connection */
        checkerr(db_ref->errhp, (retCode = OCISessionBegin ( db_ref->svchp,  db_ref->errhp, db_ref->authp, OCI_CRED_RDBMS,
                                (ub4) OCI_DEFAULT)), msg_debug); OCI_CHECKERR;
        (void) OCIAttrSet((dvoid *) db_ref->svchp, (ub4) OCI_HTYPE_SVCCTX,
                (dvoid *) db_ref->authp, (ub4) 0,
                (ub4) OCI_ATTR_SESSION, db_ref->errhp);
        db_ref->db_type = oracle_api;
        db_ref->odbc_driver = odbc_oracle;
        break;
#endif
   }
/*
 *  DB open, clean up
 */
  switch (db_ref->db_type) {
#ifdef HAVE_MYSQL
   case mysql_api:
        *driver=odbc_mysql; break;
#endif
#ifdef HAVE_ODBC
   case odbc:
     *driver=db_ref->odbc_driver;break;
#endif
#ifdef HAVE_ORACLE
   case oracle_api:
     *driver=oracle_api; break;
#endif
   default:
     *driver=0;break;
  }
 return(0);
}


void sql_close(unsigned long  *db)
{
SQL_LV_REF *db_ref;

if (*db == 0L) return;

db_ref = (SQL_LV_REF *)*db;
/*
 *  close database connection
 */
 switch ((*db_ref).db_type) {
#ifdef HAVE_MYSQL
    case mysql_api:
        mysql_close(&(db_ref->mysql));
        break;
#endif
#ifdef HAVE_ODBC
    case odbc:
        SQLDisconnect(db_ref->hdbc);            /* disconnect from the database */
        SQLFreeConnect(db_ref->hdbc);           /* free the connection handle */
        SQLFreeEnv(db_ref->henv);               /* free the environment handle */
        break;
#endif
#ifdef HAVE_ORACLE
    case oracle_api:
        retCode = OCISessionEnd(db_ref->svchp, db_ref->errhp, db_ref->authp, OCI_DEFAULT);
        retCode = OCIServerDetach(db_ref->srvhp, db_ref->errhp, OCI_DEFAULT);
        if (db_ref->envhp)
            (void) OCIHandleFree((dvoid *) db_ref->envhp, OCI_HTYPE_ENV);
        break;
#endif
    }
    free(db_ref);
}


unsigned short int sql_driver(unsigned long  *db)
{
SQL_LV_REF *db_ref;
unsigned short int odbc_driver, db_type;

if (*db == 0L) return -1;

db_ref = (SQL_LV_REF *)*db;
db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
/*
 *  read driver
 */
 return (odbc_driver);
}


void StringAdd(TD2Hdl in_array, char *string)
{
  int j;

  DSSetHandleSize((TD4Hdl) in_array, sizeof(int32) + ((*in_array)->dimSize + 1)*sizeof(LStrHandle));
  j = (*in_array)->dimSize++;
  (*in_array)->String[j] = (LStrHandle)(DSNewHandle(sizeof(int32) + (strlen(string) + 1)*sizeof(uChar)));

  (*(*in_array)->String[j])->cnt=strlen(string);
  strcpy((*(*in_array)->String[j])->str, string);
}


void StringRowAdd(TD4Hdl in_array, char *string[], int cols)
{
  int i, j, num_rows;
  TD4 * str_array;

  i = (*in_array)->dimSizes[0];
  num_rows = i + 1;
  DSSetHandleSize(in_array,sizeof(int32)*2 + num_rows*cols*sizeof(LStrHandle));

  str_array = *in_array;

  str_array->dimSizes[0] = num_rows;
  str_array->dimSizes[1] = cols;

  for(j=0; j < cols; j++)
  {
     if (string[j] == NULL) {
       str_array->String[i*cols +j] = (LStrHandle)DSNewHandle(sizeof(int32) + (strlen("")+1)*sizeof(uChar));
       (*(str_array->String[i*cols +j]))->cnt = strlen("");
       strcpy((*(str_array->String[i*cols +j]))->str,"");}
     else {
       str_array->String[i*cols +j] = (LStrHandle)DSNewHandle(sizeof(int32) + (strlen(string[j])+1)*sizeof(uChar));
       (*(str_array->String[i*cols +j]))->cnt = strlen(string[j]);
       strcpy((*(str_array->String[i*cols +j]))->str,string[j]);}
  }
}


void StringRowAdd_b(TD4Hdl in_array, char *string[], int cols, int *lengths)
{
  int i, j, k, num_rows;
  TD4 * str_array;

  i = (*in_array)->dimSizes[0];
  num_rows = i + 1;
  DSSetHandleSize(in_array,sizeof(int32)*2 + num_rows*cols*sizeof(LStrHandle));

  str_array = *in_array;

  str_array->dimSizes[0] = num_rows;
  str_array->dimSizes[1] = cols;

  for(j=0; j < cols; j++)
  {
     if (lengths[j] > 0) {
       str_array->String[i*cols +j] = (LStrHandle)DSNewHandle(sizeof(int32) + (lengths[j]+1)*sizeof(uChar));
       (*(str_array->String[i*cols +j]))->cnt = lengths[j];
       for(k=0; k < lengths[j]; k++) {*(((*(str_array->String[i*cols +j]))->str)+k) = *(string[j]+k);}}
     else {
       str_array->String[i*cols +j] = (LStrHandle)DSNewHandle(sizeof(int32) + sizeof(uChar));
       (*(str_array->String[i*cols +j]))->cnt = 0;}
  }
}


void sql_error(LStrHandle *debug, char *db_error, char *query)
{
LV_str_cp(*debug, db_error);
LV_str_cat(*debug, "\nQuery: ");
LV_str_cat(*debug, query);
/* sprintf(debug,"%s\nQuery: ", db_error);
strncat(debug, query, 512); */
}


#ifdef HAVE_ODBC
int idx_find(char *idx_field, unsigned long  *db, char *table)
{
  char idx_query[80];
  int i, j = 1;
  int condition;
  char id_str[80];
  SQLSMALLINT condition_value;
  SQL_LV_REF *db_ref;
  unsigned short int odbc_driver, db_type;
  SQLCHAR ColumnName[256];
  SQLSMALLINT DataType;
  static unsigned int sql_warnings=0;

  db_ref  = (SQL_LV_REF *)*db;
  db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;
  switch (odbc_driver) {
    case odbc_oracle:
      sprintf(idx_query, "SELECT * FROM %s WHERE rownum<2", table);
      condition = SQL_DESC_NULLABLE;
      break;
    default:
      sprintf(idx_query, "SELECT * FROM %s LIMIT 1", table);
      condition = SQL_DESC_AUTO_UNIQUE_VALUE;
      break; }
       
  SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
  SQLExecDirect(db_ref->hstmt, idx_query, SQL_NTS);

  while (SQLColAttribute (db_ref->hstmt,j, condition, NULL,
         0, NULL, &condition_value)== SQL_SUCCESS) {
    SQLDescribeCol(db_ref->hstmt, j, ColumnName, sizeof (ColumnName), NULL,
                    &DataType, NULL, NULL, NULL);
    if (condition_value == SQL_TRUE || condition_value == SQL_NO_NULLS) {
      strcpy(idx_field, ColumnName);
      sprintf(idx_query, "SELECT max(%s) FROM %s", ColumnName, table);
      SQLFreeStmt(db_ref->hstmt, SQL_CLOSE); SQLAllocStmt(db_ref->hdbc, &(db_ref->hstmt));
      SQLExecDirect(db_ref->hstmt, idx_query, SQL_NTS);
      SQLBindCol(db_ref->hstmt, 1, SQL_CHAR, id_str, 79, NULL);
      SQLFetch(db_ref->hstmt);
      i = atoi(id_str);
      SQLFreeStmt(db_ref->hstmt, SQL_CLOSE);
      break;}
    j++;
  }
  return (i);
}

void date_manipulate(char* field, unsigned long  *db) {
  char *months[] ={"jan", "feb", "mar", "apr", "may", "jun", "jul", "aug", "sep", "oct", "nov", "dec"};
  int j, mm, yyyy, dd;
  SQL_LV_REF *db_ref;
  unsigned short int odbc_driver, db_type;
  SQLCHAR ColumnName[256];
  SQLSMALLINT DataType;
  static unsigned int sql_warnings=0;

  j=1;

  db_ref  = (SQL_LV_REF *)*db;
  db_type = (*db_ref).db_type; odbc_driver=(*db_ref).odbc_driver;

  while (SQLDescribeCol(db_ref->hstmt, j, ColumnName, sizeof (ColumnName), NULL,
                    &DataType, NULL, NULL, NULL) == SQL_SUCCESS){
    if(DataType == SQL_TIMESTAMP) {
      if (sscanf(field, "%04d-%02d-%02d", &yyyy, &mm, &dd) == 3) {;
        sprintf(field, "%02d-%s-%04d", dd, months[mm-1], yyyy);} 
      }
    j++;
  }
}
#endif


void LV_str_cp(LStrHandle LV_string, char* c_str)
{
  DSSetHandleSize(LV_string, sizeof(int) + (strlen(c_str) + 1)*sizeof(char));
  (*LV_string)->cnt=strlen(c_str);
  strcpy((*LV_string)->str, c_str);
}


void LV_str_cat(LStrHandle LV_string, char* c_str)
{
  int n;
  n = (*LV_string)->cnt;
  DSSetHandleSize(LV_string, n + sizeof(int) + (strlen(c_str) + 1)*sizeof(char));
  (*LV_string)->cnt = n + strlen(c_str);
  strcpy((*LV_string)->str+n, c_str);
}


#ifdef HAVE_ORACLE
static void checkerr(OCIError *errhp, sword status, char* msg_debug)
{
  text errbuf[512];
  sb4 errcode = 0;

  switch (status)
  {
  case OCI_SUCCESS:
    break;
  case OCI_SUCCESS_WITH_INFO:
    (void) sprintf(msg_debug, "Error - OCI_SUCCESS_WITH_INFO\n");
    break;
  case OCI_NEED_DATA:
    (void) sprintf(msg_debug, "Error - OCI_NEED_DATA\n");
    break;
  case OCI_NO_DATA:
    (void) sprintf(msg_debug, "Error - OCI_NODATA\n");
    break;
  case OCI_ERROR:
    (void) OCIErrorGet((dvoid *)errhp, (ub4) 1, (text *) NULL, &errcode,
                        errbuf, (ub4) sizeof(errbuf), OCI_HTYPE_ERROR);
    (void) sprintf(msg_debug, "Error - %.*s\n", 512, (char *) errbuf);
    break;
  case OCI_INVALID_HANDLE:
    (void) sprintf(msg_debug, "Error - OCI_INVALID_HANDLE\n");
    break;
  case OCI_STILL_EXECUTING:
    (void) sprintf(msg_debug, "Error - OCI_STILL_EXECUTE\n");
    break;
  case OCI_CONTINUE:
    (void) sprintf(msg_debug, "Error - OCI_CONTINUE\n");
    break;
  default:
    break;
  }
}
#endif
