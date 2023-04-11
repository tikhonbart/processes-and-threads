// Server1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
#include <chrono>
#include <ctime>
#include <Setupapi.h>
#include <winsock2.h>
#include <string> 

#pragma warning(disable : 4996)

using namespace std;

SOCKET Connections[10];
int Counter = 0;
HWND g_HWND = NULL;
HANDLE mutexServer1 = CreateMutex(NULL, FALSE, (LPCSTR)"server1");


string getTime() {
    char buffer[80];
    time_t seconds = time(NULL);
    tm* timeinfo = localtime(&seconds);
    strftime(buffer, 80, "%I:%M:%S", timeinfo);
    return buffer;
}

string TotalPageFile() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    int totalPageFile = statex.ullTotalPageFile / DIV;
    char pagefile[256];
    sprintf(pagefile, "%d", totalPageFile);
    return pagefile;
}

string AvailblePageFile() {
    MEMORYSTATUSEX statex;
    statex.dwLength = sizeof(statex);
    GlobalMemoryStatusEx(&statex);
    int AvailPageFile = statex.ullAvailPageFile / DIV;
    char pagefile[256];
    sprintf(pagefile, "%d", AvailPageFile);
    return pagefile;
}

string edit;

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
        else if (res == 3) {
            break;
        }

        else {
            //std::cout << msg << std::endl;
            string message = std::string(msg);
            std::cout << "Message from a client: " << message << std::endl;

            if (message.compare("task1") == 0) {
                string pagefile = TotalPageFile();
                char msg[256];
                strcpy(msg, pagefile.c_str());
                
                //вывод принятого и отправленного задания
                cout << "Recieved task - " << message << " in " << getTime() << endl;
                cout << "send data " << TotalPageFile() << endl;
                
                iSendResult = send(Connections[index], msg, sizeof(msg), NULL);
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Cycle, NULL, NULL, NULL);
                while (stopCycle != 1) {
                    Sleep(1000);
                    if (pagefile == TotalPageFile()) {
                        cout << "data not changed" << endl;
                    }
                    else if (pagefile != TotalPageFile()) {
                        pagefile = AvailblePageFile();
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
            else if (message.compare("task2") == 0) {
                string pagefile = AvailblePageFile();
                char msg[256];
                strcpy(msg, pagefile.c_str());

                //вывод принятого и отправленного задания
                cout << "Recieved task - " << message << " in " << getTime() << endl;
                cout << "send data " << AvailblePageFile() << endl;

                iSendResult = send(Connections[index], msg, sizeof(msg), NULL);
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)Cycle, NULL, NULL, NULL);
                while (stopCycle != 1) {
                    Sleep(1000);
                    cout << "edit data " << AvailblePageFile() << endl;
                    pagefile = AvailblePageFile();
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
    state = WaitForSingleObject(mutexServer1, 0);
    if (state != WAIT_OBJECT_0) {
        cout << "Server 1 is running" << endl;
        cout << endl;
    }
    while (1) {
        state = WaitForSingleObject(mutexServer1, 0);
        if (state == WAIT_OBJECT_0) break;

    }

    //WSAStartup
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }
    cout << "Server 1 started" << endl;
    SOCKADDR_IN addr;
    int sizeofaddr = sizeof(addr);
    addr.sin_addr.s_addr = inet_addr("127.1.1.1");
    addr.sin_port = htons(1112);
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

    CloseHandle(mutexServer1);
    return 0;
}
