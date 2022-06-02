#include <winsock2.h>
#include <iostream>
#include <string>
#include <algorithm>

#include <thread>

#define UDP_BUFFER_LEN 512
#define TELLO_UDP_SERVER_HOST "192.168.10.1"
#define TELLO_UDP_SERVER_PORT 8889

#define TELLO_UDP_STREAM_HOST "0.0.0.0"
#define TELLO_UDP_STREAM_PORT 11111

typedef std::int32_t int32;

struct Tello
{
    std::string host = TELLO_UDP_SERVER_HOST;
    int32 port = TELLO_UDP_SERVER_PORT;

    sockaddr_in in_socket;
    int32 socket_descriptor;
    int32 socket_len = sizeof(in_socket);

    WSADATA wsa;
    char buffer[UDP_BUFFER_LEN];
    char message[UDP_BUFFER_LEN];
 
    Tello();
    Tello(const std::string& host, int32 port);

    void connect();

    void takeoff();
    void land();
    void streamon();
    void streamoff();
    void emergency();
    void up(int32 x);
    void down(int32 x);
    void left(int32 x);
    void right(int32 x);
    void forward(int32 x);
    void back(int32 x);
    void rotate_clockwise(int32 degrees);
    void rotate_counter_clockwise(int32 degrees);
    void flip(char direction);
    void flyTo(int32 x, int32 y, int32 z, int32 speed);
    void stop();

private: 
    void _init();
    void _send_command(const std::string& command);
    void _close();
    std::string _recv();
};

Tello::Tello()
{
    _init();
}

Tello::Tello(const std::string& host, int32 port)
{
    this->host = host;
    this->port = port;
    _init();
}

void Tello::_init()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
    {
        exit(EXIT_FAILURE);
    }

    if ((socket_descriptor = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR)
    {
        exit(EXIT_FAILURE);
    }

    memset((char*) &in_socket, 0, socket_len);
    in_socket.sin_family = AF_INET;
    in_socket.sin_port = htons(port);
    in_socket.sin_addr.S_un.S_addr = inet_addr(host.c_str());
}

std::string Tello::_recv()
{
    memset(buffer, '\0', UDP_BUFFER_LEN);
    if (recvfrom(socket_descriptor,
        buffer,
        UDP_BUFFER_LEN,
        0, 
        (sockaddr*) &in_socket, &socket_len) == SOCKET_ERROR)
    {
        std::cout << "recvfrom() failed\n";
        exit(EXIT_FAILURE);
    }

    return buffer;
}

void Tello::_send_command(const std::string& command)
{
    if (sendto(socket_descriptor,
        command.c_str(),
        command.size(),
        0,
        (sockaddr*) &in_socket, socket_len) == SOCKET_ERROR)
    {
        std::cout << "sendto() failed\n";
        exit(EXIT_FAILURE);
    }
}

void Tello::_close()
{
    closesocket(socket_descriptor);
    WSACleanup();
}

void Tello::connect()
{
    _send_command("command"); // Start SDK Mode
}

void Tello::takeoff()
{
    _send_command("takeoff"); // Send takeoff command
}

void Tello::land()
{
    _send_command("land"); // Send takeoff command
}

void Tello::stop()
{
    _send_command("stop");
}

void Tello::streamon()
{
    // Receiving video data should be run on another thread
    _send_command("streamon");
}

void Tello::streamoff()
{
    _send_command("streamoff");
}

void Tello::emergency()
{
    _send_command("emergency");
}

void Tello::up(int32 x)
{
    x = std::clamp(x, 20, 500);
    _send_command("up " + std::to_string(x));
}

void Tello::down(int32 x)
{
    x = std::clamp(x, 20, 500);
    _send_command("down " + std::to_string(x));
}

void Tello::left(int32 x)
{
    x = std::clamp(x, 20, 500);
    _send_command("left " + std::to_string(x));
}

void Tello::right(int32 x)
{
    x = std::clamp(x, 20, 500);
    _send_command("right " + std::to_string(x));
}

void Tello::forward(int32 x)
{
    x = std::clamp(x, 20, 500);
    _send_command("forward " + std::to_string(x));
}

void Tello::back(int32 x)
{
    x = std::clamp(x, 20, 500);
    _send_command("back " + std::to_string(x));
}

void Tello::rotate_clockwise(int32 degrees)
{
    degrees = std::clamp(degrees, 1, 360);
    _send_command("cw " + std::to_string(degrees));
}

void Tello::rotate_counter_clockwise(int32 degrees)
{
    degrees = std::clamp(degrees, 1, 360);
    _send_command("ccw " + std::to_string(degrees));
}

void Tello::flip(char direction)
{
    switch (direction)
    {
        case 'l':
        case 'r':
        case 'f':
        case 'b':
            _send_command("flip " + direction);
            break;
        default: break;
    }
}

void Tello::flyTo(int32 x, int32 y, int32 z, int32 speed)
{
    char buffer[100];

    x = std::clamp(x, -500, 500);
    y = std::clamp(x, -500, 500);
    z = std::clamp(x, -500, 500);
    speed = std::clamp(x, 10, 100);

    _send_command("go " + std::to_string(x) + " " +
        std::to_string(y) + " " +
        std::to_string(z) + " " +
        std::to_string(speed) + " "
    );
}
