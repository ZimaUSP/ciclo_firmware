#include "current_sensor.hpp"
#include "config.hpp"
#include "rele.hpp"
rele *passive_active_rele; // normaly open ->active normaly // normaly close passive
rele *stand_by_active_rele; // normaly open-> stand_by // normaly close -> active
current_sensor *acs;
#define pin A2
void setup() {
  Serial.begin(9600);
  acs= new current_sensor(A2,20);
  passive_active_rele= new rele(passive_active,true); // Este rele em especifico tem logica reversa
  passive_active_rele->init();

  stand_by_active_rele= new rele(stand_by_active,true);
  stand_by_active_rele->init();

}


void loop() {
  active();
  Serial.println(acs->get_current());
}

void active(){
  // activate reles in the desired way
  passive_active_rele->turn_off();
  stand_by_active_rele->turn_on();
  return;
}