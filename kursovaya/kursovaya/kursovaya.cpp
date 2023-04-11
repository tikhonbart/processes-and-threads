// Client.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#define WIN32_LEAN_AND_MEAN

#include <iostream>
#include <windows.h>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#pragma warning(disable: 4996)

using namespace std;

SOCKET Connection;
int res;

void ClientHandler() {
    res;
    char msg[256];
    while (true) {
        res = recv(Connection, msg, sizeof(msg), NULL);

        if (res == -1) {
            std::cout << "Disconnection\n";
            break;
        }
        else if (res == 0) {
            break;
        }
        else {
            std::cout << "Recieved data: " << msg << std::endl;
        }
    }
    
}

int main(int argc, char* argv[]) {
    SOCKADDR_IN addr;
    int sizeofaddr;
    WSAData wsaData;
    WORD DLLVersion = MAKEWORD(2, 1);
    if (WSAStartup(DLLVersion, &wsaData) != 0) {
        std::cout << "Error" << std::endl;
        exit(1);
    }

    string server;
    cout << "which server connect?" << endl;
    while (true) {
        std::cin >> server;
        if (server.compare("1") == 0) {

            sizeofaddr = sizeof(addr);
            addr.sin_addr.s_addr = inet_addr("127.1.1.1");
            addr.sin_port = htons(1112);
            addr.sin_family = AF_INET;

            Connection = socket(AF_INET, SOCK_STREAM, NULL);
            if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
                std::cout << "Failed to connect to server 1.\n";
                continue;
            }
            else {
                std::cout << "Connected to server 1.\n";
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
                cout << "which task to complete? or 3 to exit" << endl;
                char msg1[256];
                while (true) {
                    
                    std::cin >> msg1;
                    if (std::string(msg1).compare("3") == 0 || res == -1) {
                        std::cout << msg1 << endl;
                        closesocket(Connection);
                        break;
                    }
                    send(Connection, msg1, sizeof(msg1), NULL);
                }
            }
        }

        if (server.compare("2") == 0) {
            sizeofaddr = sizeof(addr);
            addr.sin_addr.s_addr = inet_addr("127.0.0.1");
            addr.sin_port = htons(1111);
            addr.sin_family = AF_INET;

            Connection = socket(AF_INET, SOCK_STREAM, NULL);
            if (connect(Connection, (SOCKADDR*)&addr, sizeof(addr)) != 0) {
                std::cout << "Failed to connect to server 2.\n";
                continue;
            }
            else {
                std::cout << "Connected to server 2.\n";
                CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)ClientHandler, NULL, NULL, NULL);
                cout << "which task to complete? or 3 to exit" << endl;
                char msg1[256];
                while (true) {
                    std::cin >> msg1;
                    if (std::string(msg1).compare("3") == 0 || res == -1) {
                        closesocket(Connection);
                        break;
                    }
                    send(Connection, msg1, sizeof(msg1), NULL);
                }
            }
        }

        if (server.compare("0") == 0) {
            break;
        }

    }

}

