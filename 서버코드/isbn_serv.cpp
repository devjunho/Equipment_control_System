#include "isbn_handler.cpp"

using json = nlohmann::json;

#define PORT_NUM 5001

struct ClientInfo // 클라이언트 구조체
{
    int clnt_sock;        // 클라이언트 소켓
    sockaddr_in clnt_adr; // 클라이언트 소켓 주소
};

pthread_mutex_t mutx;                 // 뮤텍스 선언
std::vector<ClientInfo*> clnt_socks; // 클라이언트 소켓 관리 벡터

void error_handling(const char *msg); // 에러 관리
void *handle_clnt(void *arg);

int main()
{
    int serv_sock;
    struct sockaddr_in serv_adr;

    pthread_mutex_init(&mutx, nullptr); // 뮤텍스 준비

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);

    if (serv_sock == -1)
    // 소켓 함수의 반환값이 -1이면 에러 발생 상황
    {
        error_handling("서버 소켓 생성 실패");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));

    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = INADDR_ANY;
    serv_adr.sin_port = htons(PORT_NUM);


    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) < 0)
    {
        error_handling("서버 소켓 바인딩 실패");
    }

    if (listen(serv_sock, 5) == -1)
    {
        error_handling("서버 소켓 리슨 실패");
    }

    /** accept 시작 **/

    while (1)
    {
        int clnt_sock;
        struct sockaddr_in clnt_adr;
        socklen_t clnt_adr_sz = sizeof(clnt_adr_sz);

        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);

        if (clnt_sock == -1)
        {
            std::cerr << "클라이언트 연결 수락 실패";
            close(clnt_sock);
        }

        pthread_mutex_lock(&mutx);

        ClientInfo *clientData = new ClientInfo;
        clientData->clnt_sock = clnt_sock;
        clientData->clnt_adr = clnt_adr;
        clnt_socks.push_back(clientData);

        pthread_mutex_unlock(&mutx);
        pthread_t clnt_thread;
        pthread_create(&clnt_thread, nullptr, handle_clnt, (void *)clientData);
        pthread_detach(clnt_thread);
    }


    close(serv_sock);

    return 0;
}

void *handle_clnt(void *arg)
{
    ClientInfo *clientData = static_cast<ClientInfo *>(arg);
    int clnt_sock = clientData->clnt_sock;
    sockaddr_in clnt_adr = clientData->clnt_adr;
    char buffer[1024] = {0};

    try
    {
        // 데이터를 받는다
        int read = recv(clnt_sock, buffer, 1024, 0);

        // 역직렬화
        std::string temp(buffer, read);
        json recvJson = json::parse(temp);

        std::cout << recvJson << std::endl;

        Info recvData;
        recvJson.at("Type").get_to(recvData.Type);

        std::cout << recvData.Type << std::endl;


        // 데이터 확인 ( 타입 )
        std::cout << "sock: " << clnt_sock << " Type: " << recvData.Type << "\n";

        // 핸들러 객체 생성
        Handler clnt(clnt_sock);


        switch (recvData.Type)
        {
            case INQUIRY_BOOK:
                std::cout << "INQUIRY_BOOK" << std::endl;
   
                recvJson.at("ISBN").get_to(recvData.ISBN);

                clnt.API(recvData, clnt_sock);

                break;

            case CALCULATE_COUNT:
                std::cout << "CALCULATE_COUNT" << std::endl;

                recvJson.at("RECEIVED").get_to(recvData.RECEIVED);
                recvJson.at("BASKET").get_to(recvData.BASKET);

                clnt.Calculate(recvData, clnt_sock);

                break;

            case DEPOSIT:
                std::cout << "DEPOSIT" << std::endl;

                recvJson.at("PAPER_10000").get_to(recvData.PAPER_10000);
                recvJson.at("PAPER_5000").get_to(recvData.PAPER_5000);
                recvJson.at("PAPER_1000").get_to(recvData.PAPER_1000);
                recvJson.at("COIN_500").get_to(recvData.COIN_500);
                recvJson.at("COIN_100").get_to(recvData.COIN_100);

                clnt.Deposit_Money(recvData, clnt_sock);

                break;

            default:
                break;
        }
    }
    
    catch(const std::exception& e)
    // 데이터 받음에 문제 발생
    {
        // 문제 확인
        std::cerr << e.what() << '\n';
        try
        // clnt에 전송 시도
        {
            Info sendData;
            sendData.Type = CONNECT_FAIL;
            json js;
            js = json{{"Type", sendData.Type}};
            std::string serializedData = js.dump();
            send(clnt_sock, serializedData.c_str(), serializedData.size(), 0);
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
        }
    }
    

    pthread_mutex_lock(&mutx);                    
    //
    pthread_mutex_unlock(&mutx);

    std::cout << clnt_sock << "접속종료\n";
    delete clientData;
    close(clnt_sock);
    return nullptr;
}

void error_handling(const char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}
