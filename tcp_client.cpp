#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <windows.h>  // Để dùng GetAsyncKeyState
#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET clientSocket;
    sockaddr_in serverAddr{};

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(54000);
    inet_pton(AF_INET, "10.2.1.47", &serverAddr.sin_addr); // IP máy server

    bool connected = false;

    while (!connected) {
        clientSocket = socket(AF_INET, SOCK_STREAM, 0);

        if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            cout << "❌ Khong the ket noi toi server. Nhan F5 de thu lai...\n";
            closesocket(clientSocket);

            // Đợi người dùng nhấn F5
            while (!(GetAsyncKeyState(VK_F5) & 0x8000)) {
                Sleep(100);  // giảm CPU
            }

            system("cls"); // xóa màn hình cho gọn (tùy chọn)
        }
        else {
            connected = true;
        }
    }

    cout << "✅ Da ket noi voi server!\n";

    const char* message = "Xin chao server!";
    send(clientSocket, message, strlen(message), 0);

    char buffer[1024];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        cout << "Nhan tu server: " << buffer << endl;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
