// Server2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define WIN32_LEAN_AND_MEAN
#define DIV 1024

#include <iostream>
#include <conio.h>
#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <strstream>
#include <tlhelp32.h>
#include <iomanip>
#include <ctime>

#pragma warning(disable : 4996)

using namespace std;


SOCKET Connections[10];
int Counter = 0;
HWND g_HWND = NULL;
HANDLE mutexServer2 = CreateMutex(NULL, FALSE, (LPCSTR)"server2");


string getTime() {
    char buffer[80];
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    strftime(buffer, 80, "%I:%M:%S", timeinfo);
    return buffer;
}

string ServerTime() {
    unsigned int end_time = clock();
    char time[256];
    sprintf(time, "%d seconds", end_time / 1000);
    return time;
}

string ScreenSize() {
    int screenW = GetSystemMetrics(SM_CXSCREEN);
    int screenH = GetSystemMetrics(SM_CYSCREEN);

    char screen[256];
    sprintf(screen, "%d X %d", screenW, screenH);
    return screen;
}

int stopCycle;

void Cycle() {

    char msg[256];
    while (true) {
        recv(*Connections, msg, sizeof(msg), NULL);
        stopCycle = 1;
        break;
    }
}

void ClientHandler(int index) {
    char msg[256];
    int res;
    int iSendResult;
    while (true) {
        res = recv(Connections[index], msg, sizeof(msg), NULL);

        if (res == -1) {
            std::cout << "Client Disconnected!\n";
            break;
        }
        else if (res == 0) {
            break;
        }

        else {
            string message = std::string(msg);
            std::cout << "Message from a client: " << message << std::endl;

            if (message.compare("task1") == 0) {
                string pagefile = ServerTime();
                char msg[256];
                strcpy(msg, pagefile.c_str());

                //вывод принятого и отправленного задания
                cout << "Recieved task - " << message << " in " << getTime() << endl;
                cout << "send data " << ServerTime() << endl;

                iSendResult = send(Connections[index], msg, sizeof(msg), NULL);
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Cycle, NULL, NULL, NULL);
                while (stopCycle != 1) {
                    Sleep(1000);
                    cout << "edit data " << ServerTime() << endl;
                    pagefile = ServerTime();
                    strcpy(msg, pagefile.c_str());
                    iSendResult = send(Connections[index], msg, sizeof(msg), NULL);
                    Sleep(4000);
                }
                stopCycle = 0;

                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(Connections[index]);
                    WSACleanup();
                    break;
                }
            }
            else if (message.compare("task2") == 0) {
                string pagefile = ScreenSize();
                char msg[256];
                strcpy(msg, pagefile.c_str());

                //вывод принятого и отправленного задания
                cout << "Recieved task - " << message << " in " << getTime() << endl;
                cout << "send data " << ScreenSize() << endl;

                iSendResult = send(Connections[index], msg, sizeof(msg), NULL);
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Cycle, NULL, NULL, NULL);
                while (stopCycle != 1) {
                    Sleep(1000);
                    if (pagefile == ScreenSize()) {
                        cout << "data not changed" << endl;
                    }
                    else if (pagefile != ScreenSize()) {
                        pagefile = ScreenSize();
                        strcpy(msg, pagefile.c_str());
                        iSendResult = send(Connections[index], msg, sizeof(msg), NULL);
                    }
                    Sleep(4000);
                }
                stopCycle = 0;

                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(Connections[index]);
                    WSACleanup();
                    break;
                }
            }
            else {
                char msg[256] = "Unknown command!";
                iSendResult = send(Connections[index], msg, sizeof(msg), NULL);
                if (iSendResult == SOCKET_ERROR) {
                    printf("send failed with error: %d\n", WSAGetLastError());
                    closesocket(Connections[index]);
                    WSACleanup();
                    break;
                }
            }
        }
    }
}

int main(int argc, char* argv[])
{
    DWORD state;
    //getWindowRect();
    state = WaitForSingleObject(mutexServer2, 0);
    if (state != WAIT_OBJECT_0) {
        cout << "Server 2 is running" << endl;
        cout << endl;
    }
    while (1) {
        state = WaitForSingleObject(mutexServer2, 0);
        if (state == WAIT_OBJECT_0) break;

    }

    //WSAStartup
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }
    cout << "Server 2 started" << endl;
    
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    addr.sin_port = htons(1111);
    addr.sin_family = AF_INET;

    SOCKET sListen = socket(AF_INET, SOCK_STREAM, NULL);
    bind(sListen, (SOCKADDR*)&addr, sizeof(addr));
    listen(sListen, SOMAXCONN);

    SOCKET newConnection;
    for (int i = 0; i < 10; i++) {
        newConnection = accept(sListen, (SOCKADDR*)&addr, &sizeofaddr);

        if (newConnection == 0) {
            std::cout << "Error #2\n";
        }
        else {
            std::cout << "Client Connected!\n";
            char msg[256] = "Connected";
            send(newConnection, msg, sizeof(msg), NULL);

            Connections[i] = newConnection;
            Counter++;
            CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, (LPVOID)(i), NULL, NULL);
        }
    }


    CloseHandle(mutexServer2);
    return 0;
}
