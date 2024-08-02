#ifndef HANDLER_H
#define HANDLER_H

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctime>
#include <string>
#include <unistd.h>
#include <arpa/inet.h>
#include <regex>
#include <mariadb/conncpp.hpp>
#include <nlohmann/json.hpp>
#include "DATA.h"
using json = nlohmann::json;


class DB
{
public:
    DB();
    ~DB();
    sql::Connection* ConnectDB();
    void DisconnectDB(sql::Connection* conn);
};


class Handler
{
public:

    Handler(int sock);
    ~Handler();
    void API(Info& info, int sock);
    void Calculate(Info& info, int sock);
    void InsertDB_Money(const Info & info);
    void Withdraw_Money(int* count);
    void Deposit_Money(const Info & info, int sock);

private:

    int iSock;
    static size_t write_callback(char *ptr, size_t size, size_t nmemb, std::string *data);
    std::string url_encode(const std::string &value);
    std::string RemoveTag(const std::string &html);

};

#endif