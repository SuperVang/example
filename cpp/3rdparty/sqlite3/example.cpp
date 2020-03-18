#include <iostream>
#include <algorithm>
#include <functional>
#include <sqlite3.h>

#include <gtest/gtest.h>

TEST(SQLITE3,connect)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    rc = sqlite3_open(NULL, &db);

    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stderr, "Open database successfully\n");
    }
    sqlite3_close(db);
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName)
{
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}

TEST(SQLITE3,create)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stdout, "Opened database successfully\n");
    }

    /* Create SQL statement */
    const char *sql = "CREATE TABLE COMPANY("
          "ID INT PRIMARY KEY     NOT NULL,"
          "NAME           TEXT    NOT NULL,"
          "AGE            INT     NOT NULL,"
          "ADDRESS        CHAR(50),"
          "SALARY         REAL );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Table created successfully\n");
    }
    sqlite3_close(db);
}

TEST(SQLITE3,insert)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;

    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    const char *sql ="INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
          "VALUES (1, 'Paul', 32, 'California', 20000.00 ); "
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY) "
          "VALUES (2, 'Allen', 25, 'Texas', 15000.00 ); "
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
          "VALUES (3, 'Teddy', 23, 'Norway', 20000.00 );"
          "INSERT INTO COMPANY (ID,NAME,AGE,ADDRESS,SALARY)"
          "VALUES (4, 'Mark', 25, 'Rich-Mond ', 65000.00 );";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, 0, 0, 0);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Records created successfully\n");
    }
    sqlite3_close(db);
}

TEST(SQLITE3,select)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create SQL statement */
    const char *sql = "SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}

TEST(SQLITE3,update)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    const char *sql = "UPDATE COMPANY set SALARY = 25000.00 where ID=1; SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}

TEST(SQLITE3,delete)
{
    sqlite3 *db;
    char *zErrMsg = 0;
    int rc;
    const char *data = "Callback function called";

    /* Open database */
    rc = sqlite3_open("test.db", &db);
    if (rc)
    {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        exit(0);
    }
    else
    {
        fprintf(stderr, "Opened database successfully\n");
    }

    /* Create merged SQL statement */
    const char *sql = "DELETE from COMPANY where ID=2; SELECT * from COMPANY";

    /* Execute SQL statement */
    rc = sqlite3_exec(db, sql, callback, (void *)data, &zErrMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }
    else
    {
        fprintf(stdout, "Operation done successfully\n");
    }
    sqlite3_close(db);
}
