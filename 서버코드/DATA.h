#ifndef DATA_H
#define DATA_H
#include <string>

struct Info
{
int Type;
std::string Keyword;

std::string TITLE;          // 책 제목
std::string PRE_PRICE;      // 책 원가
std::string ISBN;           // 책 바코드

std::string RECEIVED;       // 받은 돈
std::string BASKET;         // 장바구니 금액
std::string BALANCE;        // 잔돈

std::string PAPER_10000;    // 만원
std::string PAPER_5000;     // 오천원
std::string PAPER_1000;     // 천원
std::string COIN_500;       // 500원
std::string COIN_100;       // 100원
};

enum TYPE
{
    
    // 0번
    CONNECT_FAIL = 0,

    // 10번
    INQUIRY_BOOK = 10,      // 책 조회

    // 20번
    CALCULATE_MONEY = 20,

    // 30번
    MANAGE_MONEY = 30,        // 시재 관리

    // 40번
    CALCULATE_COUNT = 40,

    // 50번
    SUCCEED = 50,

    // 60번
    FAIL = 60,

    // 70번
    EMPTY = 70,

    // 80번
    DEPOSIT = 80
};

#endif