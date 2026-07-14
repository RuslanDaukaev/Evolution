#include <iostream>
#include <memory>
#include "world.h"
#include <thread> // Для слипов
#include <chrono> // Для миллисекунд

int main() {
    std::cout << "\033[8;50;180t";
    std::cout << "\033[2J"; 
    std::cout << "\033[?25l" << std::flush;

    World world(180, 50);
    world.spawnCell(25, 30);
    world.spawnCell(1, 1);
    world.spawnCell(25, 40);
    world.spawnCell(80, 10);
    world.spawnCell(50, 25);
    world.spawnCell(170, 20);
    world.spawnCell(2, 2);
    world.spawnCell(40, 40);
    world.spawnCell(75, 49);
    world.spawnCell(66, 14);

    while (true) {
        world.draw();
        world.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    

    return 0;
}