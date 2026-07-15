#include <iostream>
#include <memory>
#include "world.h"
#include <thread> // Для слипов
#include <chrono> // Для миллисекунд

int main() {
    std::cout << "\033[8;100;330t";
    std::cout << "\033[2J"; 
    std::cout << "\033[?25l" << std::flush;

    World world(330, 100);
    world.spawnCell(25, 30);
    world.spawnCell(1, 1);
    world.spawnCell(179, 49);
    world.spawnCell(1, 5);
    world.spawnCell(10, 6);
    world.spawnCell(51, 50);
    world.spawnCell(50, 51);
    world.spawnCell(49, 50);
    world.spawnCell(50, 49);
    world.spawnFood(50, 50);

    while (true) {
        world.draw();
        world.update();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    

    return 0;
}