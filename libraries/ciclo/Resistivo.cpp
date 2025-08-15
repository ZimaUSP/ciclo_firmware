// IMPLEMENTAÇÃO

#include "Resistivo.hpp"



/*
TRECHO DO MAIN CICLO

//Implementation resistivo mode
void executarLogicaResistivo() {

    //if (done)
    //  return;'
    Serial.println("executar lógica");
    //website_data();
    pwm_motor = def_pwm_motor();

    delay(500);
    lcd_timer.setInterval(duration() * 60000);
    delay(500);
    lcd.clear();


    if (verificationResistivo() < 1) {
        delay(500);
        return;
    }


    resistivo();
    print_torque_results();
    delay(500);
    //website_data(); // AQUI NAO FUNCIONA
    //endpoints(); // AQUI NAO FUNCIONA
    Serial.println("logica executada");
    //done = true;
}

int def_pwm_motor() {
    int pwm_motor = 50;  // Inicialize o valor do PWM
    lcd.setCursor(0, 0);
    lcd.print("              ");
    
    while (!btn->getPress()) {
        //Serial.println("Botão não apertado");
        if (joy->middle()) {
            // Serial.println("Middle");
            joystick_check = true;
        }
        if (joy->left() && joystick_check) {
            // Serial.println("Left");
            if (pwm_motor >= 10) {
                pwm_motor -= 10;
                joystick_check = false;
            }
        } else if (joy->right()  && joystick_check) {
            // Serial.println("Right");
            if (pwm_motor < 100) {
                pwm_motor += 10;
                joystick_check = false;
            }
        }
        lcd.setCursor(0, 0);
        lcd.print("PWM: ");
        sprintf(t, "%02d", pwm_motor);
        lcd.print(t);
        delay(10);  // Evitar sobrecarga do loop
    }
    return pwm_motor;
}

void resistivo() {
  delay(200);
  contador = 0;
    // Resetar o array de torques
    for (int i = 0; i < MAX_SAMPLES; i++) {
        lista_values[i] = 3.0;
        tempo[i]=3;
    }
    lcd_timer.reset();
    motorController->Set_L(pwm_motor);

    while (!lcd_timer.isReady() ) {
      acs = cur->get_current();
      torque = cur->get_torque(acs);
      if (torque_Time.getTimePassed() > sample_t * 50) {
          if (contador < MAX_SAMPLES) {
              //Serial.println("ok");
              //Serial.print(torque);
              //Serial.print(", ");
              //Serial.println(lcd_timer.getTimePassed());
              lista_values[contador] = torque;
              tempo[contador]=contador*sample_t * 50;
              contador++;
              torque_Time.reset();
          }
      }
      lcd.setCursor(1, 1);
      lcd.print("TORQUE: ");
      sprintf(t, "%02f", torque);
      lcd.print(t);
      printTime();
    }

    saved->push_resistivo(tempo, lista_values, MAX_SAMPLES);
    delay(500);
    if (contador > 0) {  // Evitar divisão por zero
        torque_max = lista_values[0];
        torque_min = lista_values[0];
        double soma = 0.0;

        for (int i = 0; i < contador; i++) {
            if (lista_values[i] > torque_max) {
                torque_max = lista_values[i];
            }
            if (lista_values[i] < torque_min) {
                torque_min = lista_values[i];
            }
            soma += lista_values[i];
        }
        // Calcular a média
        torque_med = soma / contador;
    } else {
        torque_max = 0;
        torque_min = 0;
        torque_med = 0;
    }

    delay(100);
    motorController->Set_L(0);
}

void print_torque_results() {
    lcd.clear();
    delay(10);
    lcd.setCursor(0, 0);
    lcd.print("Max:");
    sprintf(t, "%.2f", torque_max);  // Exibe com 2 casas decimais
    lcd.print(t);
    lcd.print("Min:");
    sprintf(t, "%.2f", torque_min);
    lcd.print(t);
    lcd.setCursor(0, 1);
    lcd.print("Med:");
    sprintf(t, "%.2f", torque_med);
    lcd.print(t);
    lcd.clear();
    delay(10);
    lcd.setCursor(0, 0);
    lcd.print("Fim");
    delay(1000);
    lcd.clear();
}


*/

resistivo::resistivo(LiquidCrystal_I2C *lcd, SimpleTimer lcd_timer, SimpleTimer rpmTime, Encoder *encoder, Memory *saved, PID *PID_vel, H_bridge_controller *motorController)
{
}
