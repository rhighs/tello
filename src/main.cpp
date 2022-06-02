#include "tello.hpp"

int main(void)
{
    Tello tello;
    tello.connect();
    tello.takeoff();
    tello.land();

    return 0;
}

