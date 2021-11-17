#include <iostream>

enum Result { correct, higher, lower };

class Manager {
public:
  Manager(){}
  virtual ~Manager(){}
  virtual Result SendGuess(int guess) = 0;
};

class UI {
public:
  UI(){}
  virtual ~UI(){}
  virtual int Run() = 0;
};

class Network {
public:
  Network(){}
  virtual ~Network(){}
  virtual Result SendGuess(int guess) = 0;
  virtual int Connect() = 0;
};

class CommandLineUI : public UI {
private:
  Manager* mgmt;
public:
  CommandLineUI(Manager* m){
    mgmt = m;
  }
  
  int Run() {
    while (true) {
      std::cout << "Please enter a number: ";

      int guess;
      std::cin >> guess;
      std::cout << std::endl;

      Result r = mgmt->SendGuess(guess);
      
      switch (r) {
      case correct:
	std::cout << "Correct number!" << std::endl;
	break;

      case higher:
	std::cout << "Too low" << std::endl;
	break;

      case lower:
	std::cout << "Too high" << std::endl;
	break;
      }
    }
    return 0;
  }
};

class TestManager: public Manager {
private:
  int answer;
  
public:
  TestManager(int a) {
    answer = a;
  }
  
  Result SendGuess(int guess) {
    Result r;
    if (guess == answer) {
      r = correct;
    } else if (guess < answer) {
      r = higher;
    } else {
      r = lower;
    }

    return r;
  }
};


int main(int argc, char* argv[]) {
  TestManager* mgmt = new TestManager(50);

  CommandLineUI* ui = new CommandLineUI(mgmt);

  ui->Run();

  return 0;
}
