#include <iostream>

int main() {
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;

  std::cout << "Logs from your program will appear here!\n";
  std::cout<<"$";
  std::string input;
  std::getline(std::cin,input);
  
}
