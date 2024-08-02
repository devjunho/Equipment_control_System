#include "isbn_handler.h"


DB::DB() { }

DB::~DB() { }

sql::Connection* DB::ConnectDB()                        // 완료
{
    try
    {
        sql::Driver* driver = sql::mariadb::get_driver_instance();
        sql::SQLString url = "jdbc:mariadb://10.10.21.114:3306/CALCULATE";
        sql::Properties properties({{"user", "OPERATOR"}, {"password", "1234"}});
        std::cout << "DB 접속 성공" << std::endl;

        return driver->connect(url, properties);   
     }
    catch(sql::SQLException& e)
    {
        std::cerr << "DB 접속 실패: " << e.what() << std::endl;
        exit(1);
    }
}

void DB::DisconnectDB(sql::Connection* conn)            // 완료
{
    if (!conn->isClosed())
    {
        conn->close();
        std::cout << "DB 접속 해제" << std::endl;
    }
}

Handler::Handler(int sock)                              // 완료
{
    int iSock = sock;
}

Handler::~Handler() { }

void Handler::API(Info &info, int sock)                 // 완료
{
    CURL *curl;
    CURLcode res;

    std::string sendData;
    std::string chunk_;

    json js;
    int bytesSent = 0;

    curl_global_init(CURL_GLOBAL_ALL); // libcurl 초기화

    curl = curl_easy_init();
    if (curl)
    {
        std::string url = "https://www.nl.go.kr/seoji/SearchApi.do?cert_key=ff90ea5dcecab88ca34dbf42d134285cf8c65b19e93a91d394d75827c44b41e9&result_style=json&page_no=1&page_size=1";
        url += "&isbn=" + url_encode(info.ISBN);

        std::cout << info.ISBN << std::endl;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str()); // std::string을 C 스타일 문자열로 변환하여 사용
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &chunk_);

        res = curl_easy_perform(curl); // HTTP 요청 실행
        if (res != CURLE_OK)
        {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();

    try
    {
        // 요청 성공 시 JSON 파싱
        json result = json::parse(chunk_);

        if (result.contains("docs"))
        {
            js = {
                {"Type", SUCCEED},
                {"BasketList", json::array()}};                   // 책에 대한 전체내용

            auto items = result["docs"];
            // 필요한 정보 추출
            for (const auto &item : items)
            {
                info.TITLE = item.value("TITLE", "");               // 책 제목
                info.PRE_PRICE = item.value("PRE_PRICE", "");       // 책 원가

                // HTML 태그 제거
                std::string title_ = RemoveTag(info.TITLE);
                std::string price_ = RemoveTag(info.PRE_PRICE);
                info.TITLE = title_;
                info.PRE_PRICE = price_;

                js["BasketList"].push_back({{"TITLE", info.TITLE},
                        {"PRE_PRICE", info.PRE_PRICE}
                        });

            }

            sendData = js.dump();
            std::cout << sendData << std::endl;

            bytesSent = write(sock, sendData.c_str(), sendData.length());   // 서버에게 json 형식으로 보내기
            std::cout << bytesSent << std::endl;

            return;
        }
        else
        {
            std::cout << "ELSE_FAIL" << std::endl;

            js = json{{"Type", FAIL}};
            sendData = js.dump();
            bytesSent = write(sock, sendData.c_str(), sendData.length());

            std::cout << bytesSent << std::endl;

            return;
        }
    }
    catch (json::parse_error &e)
    {
        std::cerr << "JSON 파싱 에러 : FAIL" << e.what();

        js = json{{"Type", FAIL}};
        sendData = js.dump();
        bytesSent = write(sock, sendData.c_str(), sendData.length());

        std::cout << bytesSent << std::endl;

    }
    catch (json::type_error &e)
    {
        std::cerr << "JSON 타입 에러 : FAIL" << e.what();
        
        js = json{{"Type", FAIL}};
        sendData = js.dump();
        bytesSent = write(sock, sendData.c_str(), sendData.length());

        std::cout << bytesSent << std::endl;
    }
}

size_t Handler::write_callback(char *ptr, size_t size, size_t nmemb, std::string *data)     // 완료
{
    data->append(ptr, size * nmemb);
    return size * nmemb;
}

std::string Handler::url_encode(const std::string &value)                                   // 완료
{
    std::ostringstream escaped;
    escaped << std::hex << std::uppercase;
    for (char c : value)
    {
        if (isalnum(c) || c == '-' || c == '_' || c == '.' || c == '~')
        {
            escaped << c;
        }
        else
        {
            escaped << '%' << std::setw(2) << int((unsigned char)c);
        }
    }
    return escaped.str();
}

std::string Handler::RemoveTag(const std::string &html)                                     // 완료
{
    std::regex regex("<.*?>");
    return std::regex_replace(html, regex, "");
}

// 잔돈 계산 및 잔돈에 따른 지폐, 동전 횟수 계산
void Handler::Calculate(Info & info, int sock)                                              // 로봇 팔로 좌표 값 알아본 후, 함수 내용 추가 예정
{
    std::string sendData;

    json js;
    int bytesSent = 0;

    try
    {
        DB db;
        
        std::cout << "처음" << std::endl;

        std::string Inmoney[5] = {"0", "0", "0", "0", "0"};
        std::string Outmoney[5] = {"0", "0", "0", "0", "0"};

        int count[5] ={0, 0, 0, 0, 0};  // 횟수 담기 위한 배열
        int rest;   // temp에서 빼고 나머지 금액
        // int quotient;   // 몫
        // int remainder;  // 나머지

        // '잔돈 계산' 완료
        int temp = std::stoi(info.RECEIVED) - std::stoi(info.BASKET);
        info.BALANCE = std::to_string(temp);

        std::cout << "잔돈계산" << std::endl;


        // 관리자가 넣은 시재 수량
        try
        {
            sql::Connection*con = db.ConnectDB();
            sql::PreparedStatement*InManageInfo 
            = con->prepareStatement("SELECT SUM(PAPER_10000),SUM(PAPER_5000),SUM(PAPER_1000),SUM(COIN_500),SUM(COIN_100) FROM MANAGE_MONEY WHERE STATE = 0");
            sql::ResultSet*Log = InManageInfo->executeQuery();
            while (Log->next())
            {
                Inmoney[0] = Log->getString(1); // PAPER_10000
                Inmoney[1] = Log->getString(2); // PAPER_5000
                Inmoney[2] = Log->getString(3); // PAPER_1000
                Inmoney[3] = Log->getString(4); // COIN_500
                Inmoney[4] = Log->getString(5); // COIN_100
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        // 잔돈으로 인해 빼낸 시재 수량
        try
        {
            sql::Connection*con = db.ConnectDB();
            sql::PreparedStatement*OutManageInfo 
            = con->prepareStatement("SELECT SUM(PAPER_10000),SUM(PAPER_5000),SUM(PAPER_1000),SUM(COIN_500),SUM(COIN_100) FROM MANAGE_MONEY WHERE STATE = 1");
            sql::ResultSet*Log = OutManageInfo->executeQuery();
            while (Log->next())
            {
                Outmoney[0] = Log->getString(1);  // PAPER_10000
                Outmoney[1] = Log->getString(2);  // PAPER_5000
                Outmoney[2] = Log->getString(3);  // PAPER_1000
                Outmoney[3] = Log->getString(4);  // COIN_500
                Outmoney[4] = Log->getString(5);  // COIN_100
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
        
        int cal[5];

        cal[0] = std::stoi(Inmoney[0]) - std::stoi(Outmoney[0]);
        cal[1] = std::stoi(Inmoney[1]) - std::stoi(Outmoney[1]);
        cal[2] = std::stoi(Inmoney[2]) - std::stoi(Outmoney[2]);
        cal[3] = std::stoi(Inmoney[3]) - std::stoi(Outmoney[3]);
        cal[4] = std::stoi(Inmoney[4]) - std::stoi(Outmoney[4]);

        if(temp >= 10000) // 잔돈이 10000원 이상일 경우
        {
            count[0] = temp / 10000;    // 10000원 지폐 횟수
            rest = temp - (count[0] * 10000);

            count[1] = rest / 5000;     // 5000원 지폐 횟수
            temp = rest - (count[1] * 5000);

            count[2] = temp / 1000;     // 1000원 지폐 횟수
            rest = temp - (count[2] * 1000);

            count[3] = rest / 500;      // 500원 동전 횟수
            temp = rest - (count[3] * 500);

            count[4] = temp / 100;      // 100원 동전 횟수
        }
        else if(temp >= 5000)   // 잔돈 금액이 10000원 이상일 경우
        {
            count[1] = temp / 5000;     // 5000원 지폐 횟수
            rest = temp - (count[1] * 5000);

            count[2] = rest / 1000;     // 1000원 지폐 횟수
            temp = rest - (count[2] * 1000);

            count[3] = temp / 500;      // 500원 동전 횟수
            rest = temp - (count[3] * 500);

            count[4] = rest / 100;      // 100원 동전 횟수
        }
        else
        {
            count[2] = temp / 1000;     // 1000원 지폐 횟수
            rest = temp - (count[2] * 1000);

            count[3] = rest / 500;      // 500원 동전 횟수
            temp = rest - (count[3] * 500);

            count[4] = temp / 100;
        }

        for(int i=0; i<5; i++)
        {
            if(count[i] > cal[i])
            {
                throw std::runtime_error("잔돈이 부족합니다.");
            }
        }

        js = {
            {"Type", SUCCEED},
            {"BALANCE", info.BALANCE},
            {"Result10000", json::array()},
            {"Result5000", json::array()},
            {"Result1000", json::array()},
            {"Result500", json::array()},
            {"Result100", json::array()},
            };

        js["Result10000"].push_back({{"Count", count[0]},{"Rest", cal[0]}});

        js["Result5000"].push_back({{"Count", count[1]}, {"Rest", cal[1]}});

        js["Result1000"].push_back({{"Count", count[2]}, {"Rest", cal[2]}});

        js["Result500"].push_back({{"Count", count[3]}, {"Rest", cal[3]}});

        js["Result100"].push_back({{"Count", count[4]}, {"Rest", cal[4]}});



        sendData = js.dump();
        std::cout << sendData << std::endl;

        bytesSent = write(sock, sendData.c_str(), sendData.length());
        std::cout << bytesSent << std::endl;

        InsertDB_Money(info);       // '받은 돈, 장바구니 금액, 잔돈 DB에 넣어주기' 완료
        Withdraw_Money(count);

        return;
    }
    catch (json::parse_error &e)
    {
        std::cerr << "JSON 파싱 에러 : FAIL" << e.what();

        js = json{{"Type", FAIL}};
        sendData = js.dump();
        bytesSent = write(sock, sendData.c_str(), sendData.length());

        std::cout << bytesSent << std::endl;

    }
    catch (json::type_error &e)
    {
        std::cerr << "JSON 타입 에러 : FAIL" << e.what();
        
        js = json{{"Type", FAIL}};
        sendData = js.dump();
        bytesSent = write(sock, sendData.c_str(), sendData.length());

        std::cout << bytesSent << std::endl;
    }
}

void Handler::InsertDB_Money(const Info & info)                                             // 돈 구역에서 맨 마지막에 넣어주기
{
    try
    {
        DB db;
        sql::Connection*con = db.ConnectDB();
        sql::PreparedStatement*Money
        = con->prepareStatement("INSERT INTO MONEY (RECEIVED, BASKET, BALANCE) VALUES (?, ?, ?)");

        Money->setString(1, info.RECEIVED);
        Money->setString(2, info.BASKET);
        Money->setString(3, info.BALANCE);

        Money->executeQuery();

        std::cout << "DB 기록 저장" << std::endl;

        sql::PreparedStatement*clearNO1 = con->prepareStatement("ALTER TABLE MONEY AUTO_INCREMENT=1");           // NO를 AUTO_INCREMENT로 지정하였지만, 혹시 몰라 경고하는데 너 잔들어~
        clearNO1->executeQuery();
        sql::PreparedStatement*clearNO2 = con->prepareStatement("SET @COUNT = 0");
        clearNO2->executeQuery();
        sql::PreparedStatement*clearNO3 = con->prepareStatement("UPDATE MONEY SET NO = @COUNT:=@COUNT+1");
        clearNO3->executeQuery();
    
        db.DisconnectDB(con);
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "DB 기록 저장 실패 : " << e.what() << std::endl;
    }
}

void Handler::Withdraw_Money(int* count)                                             // 돈 구역에서 맨 마지막에 넣어주기
{
    try
    {
        DB db;
        sql::Connection*con = db.ConnectDB();
        sql::PreparedStatement*Mange = con->prepareStatement("INSERT INTO MANAGE_MONEY VALUES (DEFAULT, ?, ?, ?, ?, ?, 1)");

        Mange->setString(1, std::to_string(count[0]));
        Mange->setString(2, std::to_string(count[1]));
        Mange->setString(3, std::to_string(count[2]));
        Mange->setString(4, std::to_string(count[3]));
        Mange->setString(5, std::to_string(count[4]));

        Mange->executeQuery();

        std::cout << "DB 기록 저장" << std::endl;

        db.DisconnectDB(con);
    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "DB 기록 저장 실패 : " << e.what() << std::endl;
    }
}

void Handler::Deposit_Money(const Info & info, int sock)
{
    std::string sendData;
    json js;
    int bytesSent = 0;

    try
    {
        DB db;
        sql::Connection*con = db.ConnectDB();
        sql::PreparedStatement*Mange = con->prepareStatement("INSERT INTO MANAGE_MONEY VALUES (DEFAULT, ?, ?, ?, ?, ?, 0)");

        Mange->setInt(1, std::stoi(info.PAPER_10000));
        Mange->setInt(2, std::stoi(info.PAPER_5000));
        Mange->setInt(3, std::stoi(info.PAPER_1000));
        Mange->setInt(4, std::stoi(info.COIN_500));
        Mange->setInt(5, std::stoi(info.COIN_100));

        Mange->executeQuery();

        std::cout << "DB 기록 저장" << std::endl;


        js = json{{"Type", SUCCEED}};

        sendData = js.dump();
        bytesSent = write(sock, sendData.c_str(), sendData.length());

        std::cout << sendData << std::endl;
        std::cout << bytesSent << std::endl;

        db.DisconnectDB(con);

    }
    catch(const sql::SQLException& e)
    {
        std::cerr << "DB 기록 저장 실패 : " << e.what() << std::endl;

        js = json{{"Type", FAIL}};

        sendData = js.dump();
        bytesSent = write(sock, sendData.c_str(), sendData.length());

        std::cout << sendData << std::endl;
        std::cout << bytesSent << std::endl;
    }
}
