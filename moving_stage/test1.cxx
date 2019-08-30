#include <iostream>
#include <unistd.h>
#include <string>
#include <cstdlib>
#include <cstdio>

int originStage() {
  std::cout << "Move stage to origin+" << std::endl;
  std::string cmd = "python stage.py -o -";
  system(cmd.c_str() );
  sleep(10);
  return 0;
}

int moveStage(int n) {
  std::cout << "Move stage for " << n << " pulses" << std::endl;
  char cmd[200];
  std::sprintf(cmd, "python stage.py -ml %d", n);
  system(cmd);
  sleep(3);
  return 0;
}

int takeData() {
  std::cout << "Taking data for 5 seconds" << std::endl;
  system("echo 'sleeping'; sleep 5");
}

int main(int argc, char* argv[]) {
  originStage();

  std::cout << "Move stage: " << std::endl;
  moveStage(10000);

  std::cout << "Take data: " << std::endl;
  takeData();

  std::cout << "End of the program" << std::endl;
  return 0;
}
