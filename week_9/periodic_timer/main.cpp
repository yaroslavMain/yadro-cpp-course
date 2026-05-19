#include <iomanip>
#include <iostream>

#include "periodic_timer.h"

void some_func() {
  const auto now = std::chrono::system_clock::now();
  const std::time_t now_c = std::chrono::system_clock::to_time_t(now);
  std::cout << "[" << std::put_time(std::localtime(&now_c), "%Y-%m-%d %H:%M:%S")
            << "]" << " Some func" << std::endl;
}

int main() {
  PeriodicTimer pt(std::chrono::milliseconds{1000}, some_func);
  std::this_thread::sleep_for(std::chrono::seconds(30));

  return 0;
}
