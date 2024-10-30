#include "CSV.hpp"

float dado1[5] = {1,2,3,4,5};
float dado2[5] = {6,7,8,9,10};

void setup() {
  Serial.begin(9600);
  CSV *csv = new CSV();
  String data = csv->to_csv("torque", dado1, "tempo", dado2, 5);
  Serial.println(data);
}

void loop() {
  // put your main code here, to run repeatedly:

}
