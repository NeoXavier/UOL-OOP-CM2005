#include <iostream>

class Vehicle {
public:
  Vehicle();
  float getSpeed();
  void accelerate();
  void beepHorn();

private:
  float speed;
};

Vehicle::Vehicle() { speed = 0.0f; }

float Vehicle::getSpeed() { return speed; }

void Vehicle::accelerate() { speed += 0.5f; }

void Vehicle::beepHorn() { std::cout << "Beep beep!" << std::endl; }

class Train : public Vehicle {
public:
  Train();
  void beepHorn();
};

Train::Train() {}

void Train::beepHorn() {
    Vehicle::beepHorn();
  if (getSpeed() < 0.5f) {
    std::cout << "Choo choo!" << std::endl;
  } else {
    std::cout << "CHOO CHOO!" << std::endl;
  }
}

int main() {
  Vehicle myCar;

  Train myTrain;
  myTrain.beepHorn();
  myTrain.accelerate();
  myTrain.beepHorn();

  return 0;
}
