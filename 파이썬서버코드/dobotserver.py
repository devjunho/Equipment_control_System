#devType: ML
#isUsingRail: False
dType.SetQueuedCmdClear(api)
dType.SetQueuedCmdStopExec(api)
dType.SetQueuedCmdStartExec(api)
dType.SetQueuedCmdClear(api)
dType.SetQueuedCmdStopExec(api)
dType.SetQueuedCmdStartExec(api)
dType.SetQueuedCmdClear(api)
dType.SetQueuedCmdStopExec(api)
dType.SetQueuedCmdStartExec(api)
dType.SetQueuedCmdClear(api)
dType.SetQueuedCmdStopExec(api)
dType.SetQueuedCmdStartExec(api)
dType.SetQueuedCmdClear(api)
dType.SetQueuedCmdStopExec(api)
dType.SetQueuedCmdStartExec(api)
dType.SetQueuedCmdClear(api)
dType.SetQueuedCmdStopExec(api)
dType.SetQueuedCmdStartExec(api)
import socket, threading
import json
import math

def paper10000(turn):
    for i in range(turn):
        dType.SetArmSpeedRatio(api, 0, 80, 0)
        dType.SetPTPCmd(api, 1, -2.2892, -178.332, 45.085, -143.3099, 5)  # 시작점
        dType.SetPTPCmd(api, 1, 108.0607, -318.9091, -3.9639, -123.8558, 1)
        dType.SetPTPCmd(api, 1, 112.0746, -318.1369, -54.4581, -123.1679, 1)  # 지폐 접촉
        dType.SetEndEffectorSuctionCup(api, 1, 1, 1)  # 흡착컵 활성화
        dType.SetWAITCmd(api, 2000, 1)
        dType.SetPTPCmd(api, 1, 84.7841, -330.9062, 81.1153, -100, 1)
        dType.SetPTPCmd(api, 1, 228.0368, -8.7699, 177.5908, -100, 1)
        dType.SetPTPCmd(api, 1, 209.5209, -2.7142, 68.2081, -100, 1)
        dType.SetPTPCmd(api, 1, 189.4998, -2.4734, -5.2022, -100, 1)  # 지폐 놓기
        dType.SetEndEffectorSuctionCup(api, 0, 0, 1)  # 흡착컵의 비활성화
    return 10000

def paper5000(turn):
    for i in range(turn):
        dType.SetArmSpeedRatio(api, 0, 80, 0)
        dType.SetPTPCmd(api, 1, -2.2892, -178.332, 45.085, -143.3099, 1)  # 시작점
        dType.SetPTPCmd(api, 1, 20.0604, -322.665, 6.6853, -185.6946, 1)
        dType.SetPTPCmd(api, 1, 24.168, -325.1773, -54.4581, -185.0016, 1)
        dType.SetEndEffectorSuctionCup(api, 1, 1, 1)  # 흡착컵의 상태를 나타냄 1은 흡착컵 활성화
        dType.SetWAITCmd(api, 2000, 1)
        dType.SetPTPCmd(api, 1, 26.8018, -306.7728, 89.504, -184.2591, 1)
        dType.SetPTPCmd(api, 1, 198.3829, -17.7565, 58.863, -18, 1)
        dType.SetPTPCmd(api, 1, 191.8747, -7.3642, 16.0621, -18, 1)  # 지폐 놓기
        dType.SetEndEffectorSuctionCup(api, 0, 0, 1)  # 흡착컵의 비활성화
    return 5000

def paper1000(turn):
    for i in range(turn):
        dType.SetArmSpeedRatio(api, 0, 80, 0)
        dType.SetPTPCmd(api, 1, -2.2892, -178.332, 45.085, -143.3099, 1)  # 시작점
        dType.SetPTPCmd(api, 1, -48.4398, -316.0831, 21.1069, -114.5148, 1)
        dType.SetPTPCmd(api, 1, -57.0458, -319.094, -54.4581, -115.938, 1)
        dType.SetEndEffectorSuctionCup(api, 1, 1, 1)  # 흡착컵의 상태를 나타냄 1은 흡착컵 활성화
        dType.SetWAITCmd(api, 2000, 1)
        dType.SetPTPCmd(api, 1, -47.7735, -311.7352, 49.4154, -114.5148, 1)
        dType.SetPTPCmd(api, 1, 186.9654, -16.5989, 57.7294, 67, 1)
        dType.SetPTPCmd(api, 1, 191.8747, -7.3642, 16.0621, 67, 1)  # 지폐 놓기
        dType.SetEndEffectorSuctionCup(api, 0, 0, 1)  # 흡착컵의 비활성화
    return 1000

def coin500(turn, rest):
    width = 0
    index = rest % 5
    for i in range(turn):
        height = index * -1.8
        dType.SetArmSpeedRatio(api, 0, 80, 0)
        dType.SetPTPCmd(api, 1, 4.4391, -180.2133, 47.5148, 88.5889, 1)  # 시작점1
        dType.SetPTPCmd(api, 1, -34.3275, -224.5556, -27.7551, -98.6914, 1)
        dType.SetPTPCmd(api, 1, -35.0486 + width, -229.0015, -45.6662 + height, -98.7016, 1)  # pick1
        dType.SetEndEffectorSuctionCup(api, 1, 1, 1)  # 흡착컵의 상태를 나타냄 1은 흡착컵 활성화
        dType.SetWAITCmd(api, 2000, 1)
        dType.SetPTPCmd(api, 1, -13.266, -208.5874, 42.8377, -93.6391, 1)
        dType.SetPTPCmd(api, 1, 176.5406, -17.8594, 38.1539, -5.7766, 1)
        #dType.SetPTPCmd(api, 1, 217.2784, 35.8320, -14.0411, -43.2104, 1)
        dType.SetEndEffectorSuctionCup(api, 0, 0, 1)  # 흡착컵의 비활성화
        rest = rest - 1
        if rest % 5 == 0:
            width = width + 30
        if index == 0:
            index = 4
        else:
            index = index - 1
    return 500

def coin100(turn, rest):
    width = 0
    index = rest % 5
    for i in range(turn):
        height = index * -3
        dType.SetArmSpeedRatio(api, 0, 80, 0)
        dType.SetPTPCmd(api, 1, 4.4391, -180.2133, 47.5148, 88.5889, 1)  # 시작점1
        dType.SetPTPCmd(api, 1, -69.3668, -232.9172, -7.9205, -106.5844, 1)
        dType.SetPTPCmd(api, 1, -72.0081 + width, -231.107, -50 + height, -107.3059, 1)  # pick1
        dType.SetEndEffectorSuctionCup(api, 1, 1, 1)  # 흡착컵의 상태를 나타냄 1은 흡착컵 활성화
        dType.SetWAITCmd(api, 2000, 1)
        dType.SetPTPCmd(api, 1, -13.9851, -184.1808, 38.5014, -94.3422, 1)
        dType.SetPTPCmd(api, 1, 178.8862, -11.303, 33.8616, -3.6154, 1)
        #dType.SetPTPCmd(api, 1, 205.8645, 25.8984, 1.7901, -45.4041, 1)
        dType.SetEndEffectorSuctionCup(api, 0, 0, 1)  # 흡착컵의 비활성화
        rest = rest - 1
        if rest % 5 == 0:
            width = width + 25
        if index == 0:
            index = 4
        else:
            index = index - 1
    return 100

def binder(client_socket, addr):
    print('Connected by', addr)
    try:
        data = client_socket.recv(1024)

        if not data:
            print("클라이언트 연결 끊김: ", addr)
        else:
            msg = data.decode('utf-8')
            print('받은 메세지 : ', msg)

            jsonmsg = json.loads(msg)
            print('제이슨 메세지 : ', jsonmsg)

            turn = [jsonmsg["Result10000"][0]["Count"], jsonmsg["Result5000"][0]["Count"],
                    jsonmsg["Result1000"][0]["Count"],
                    jsonmsg["Result500"][0]["Count"], jsonmsg["Result100"][0]["Count"]]
            rest = [jsonmsg["Result10000"][0]["Rest"], jsonmsg["Result5000"][0]["Rest"], jsonmsg["Result1000"][0]["Rest"],
                    jsonmsg["Result500"][0]["Rest"], jsonmsg["Result100"][0]["Rest"]]

            print("turn", turn)
            print("rest", rest)

            sendmsg = {"Type": 50}

            jsonsend = json.dumps(sendmsg)  # Python 객체를 JSON 문자열로 변환
            print("보낸 메세지 :", jsonsend)
            client_socket.sendall(jsonsend.encode())

            # 로봇 팔 움직이기
            result = paper10000(turn[0])
            print(turn[0])

            result = paper5000(turn[1])
            print(turn[1])

            result = paper1000(turn[2])
            print(turn[2])

            result = coin500(turn[3], rest[3])
            print(turn[3], rest[3])

            result = coin100(turn[4], rest[4])
            print(turn[4], rest[4])
    except json.JSONDecodeError:
        print("JSON 디코딩 오류 발생")

    except ConnectionResetError:
        print("클라이언트 연결 끊김: ", addr)

    except Exception as e:
        print("예기치 않은 오류 발생: ", e)
    finally:
        client_socket.close()
        print("클라가 연결 끊음")

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_socket.bind(('', 5001))

server_socket.listen()

print("서버 열림!")

try:
    while True:
        client_socket, addr = server_socket.accept()
        thr = threading.Thread(target=binder, args=(client_socket, addr))
        thr.start()

except Exception as e:
    print("Server exception:", e)
finally:
    server_socket.close()
dType.RestartMagicBox(api)
dType.RestartMagicBox(api)
dType.RestartMagicBox(api)
dType.RestartMagicBox(api)
dType.RestartMagicBox(api)
dType.RestartMagicBox(api)