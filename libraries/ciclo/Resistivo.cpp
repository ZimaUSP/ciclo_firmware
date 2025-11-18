// IMPLEMENTAÇÃO

#include "Resistivo.hpp"

// Construtor
resistivo::resistivo(LiquidCrystal_I2C* lcd, SimpleTimer* lcd_timer, SimpleTimer* rpmTime, SimpleTimer* torque_Time,
    Encoder* encoder, Memory* saved, PID* PID_vel, H_bridge_controller* motorController,
    Button* btn, Joystick* joy, current_sensor* cur) 
{
    // Armazenando os ponteiros para os objetos externos
    this->lcd = lcd;
    this->lcd_timer = lcd_timer;
    this->rpmTime = rpmTime;
    this->torque_Time = torque_Time;
    this->encoder = encoder;
    this->saved = saved;
    this->PID_vel = PID_vel;
    this->motorController = motorController;
    this->btn = btn;
    this->joy = joy;
    this->cur = cur;

    // Inicializando variáveis de estado
    this->joystick_check = false;
    this->contador = 0;
    this->pwm_motor = 50; 
    this->verif = 1; 
}



void resistivo::executarLogicaResistivo() {
    Serial.println("executar lógica");
    pwm_motor = def_pwm_motor(); 

    delay(500);
    lcd_timer->setInterval(duration() * 60000); 
    delay(500);
    lcd->clear();

    if (verificationResistivo() < 1) { 
        delay(500);
        return;
    }

    resis(); 
    print_torque_results(); 
    delay(500);
    
    Serial.println("logica executada");
}

int resistivo::def_pwm_motor() {
    int pwm_motor_local = this->pwm_motor; // Inicia com o valor já salvo na classe
    lcd->setCursor(0, 0);
    lcd->print("                "); 

    while (!btn->getPress()) {
        if (joy->middle()) {
            joystick_check = true; 
        }
        if (joy->left() && joystick_check) {
            if (pwm_motor_local >= 10) {
                pwm_motor_local -= 10;
                joystick_check = false; 
            }
        } else if (joy->right() && joystick_check) {
            if (pwm_motor_local < 100) {
                pwm_motor_local += 10;
                joystick_check = false; 
            }
        }
        lcd->setCursor(0, 0);
        lcd->print("PWM: ");
        sprintf(t, "%02d", pwm_motor_local); 
        lcd->print(t);
        delay(10); 
    }
    return pwm_motor_local;
}

void resistivo::resis() {
    delay(200);
    contador = 0; 
    
    for (int i = 0; i < MAX_SAMPLES; i++) {
        lista_values[i] = 3.0; 
        tempo[i] = 3;          
    }
    lcd_timer->reset(); 
    motorController->Set_L(pwm_motor); 

    while (!lcd_timer->isReady()) { 
        acs = cur->get_current();      
        torque = cur->get_torque(acs); 
        
        if (torque_Time->getTimePassed() > sample_t * 50) { 
            if (contador < MAX_SAMPLES) {
                lista_values[contador] = torque;
                tempo[contador] = contador * sample_t * 50;
                contador++;
                torque_Time->reset(); 
            }
        }
        lcd->setCursor(1, 1);
        lcd->print("TORQUE: ");
        sprintf(t, "%02f", torque);
        lcd->print(t);
        printTime(); // Chama método da classe
    }

    saved->push_resistivo(tempo, lista_values, MAX_SAMPLES); 
    delay(500);
    
    if (contador > 0) {
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
        torque_med = soma / contador;
    } else {
        torque_max = 0;
        torque_min = 0;
        torque_med = 0;
    }

    delay(100);
    motorController->Set_L(0); 
}

void resistivo::print_torque_results() {
    lcd->clear();
    delay(10);
    lcd->setCursor(0, 0);
    lcd->print("Max:");
    sprintf(t, "%.2f", torque_max); 
    lcd->print(t);
    lcd->print("Min:");
    sprintf(t, "%.2f", torque_min); 
    lcd->print(t);
    lcd->setCursor(0, 1);
    lcd->print("Med:");
    sprintf(t, "%.2f", torque_med); 
    lcd->print(t);
    
    delay(3000); // Delay para mostrar os resultados
    
    lcd->clear();
    delay(10);
    lcd->setCursor(0, 0);
    lcd->print("Fim");
    delay(1000);
    lcd->clear();
}


int resistivo::verificationResistivo() {
    lcd->clear();
    while (!btn->getPress()) {
        lcd->setCursor(0, 0);
        lcd->print("PWM: ");
        sprintf(t, "%02d", pwm_motor); // Usa membro da classe
        lcd->print(t);
        lcd->print("|");
        lcd->print("Time:");
        sprintf(t, "%02d", lcd_timer->getInterval() / 60000); // Usa membro da classe
        lcd->print(t);
        lcd->setCursor(0, 1);
        if (verif == 0) { // Usa membro da classe
            lcd->print("Nao            ");
            lcd->setCursor(0, 1);
            lcd->noBacklight();
        }
        if (verif == 1) { // Usa membro da classe
            lcd->print("Sim            ");
            lcd->setCursor(0, 1);
            lcd->noBacklight();  //mudar(?)
        }
    }
    lcd->clear();
    return verif; // Usa membro da classe
}


int resistivo::duration() {

    //mudar talvez
    int t_Duration = 1; 

    lcd->setCursor(0, 0);
    lcd->print("                ");
    while (!btn->getPress()){
        if (joy->middle())
        {
            joystick_check = true; // Usa membro da classe
        }
        if (joy->left() && joystick_check)
        {
            if(t_Duration != 0 ){
                t_Duration--;
                joystick_check = false; // Usa membro da classe
            }
        }
        else if (joy->right() && joystick_check)
        {
            if(t_Duration != 10){
                t_Duration++;
                joystick_check = false; // Usa membro da classe
            }
        }   
        lcd->setCursor(0, 0); 
        lcd->print("Duration: ");
        sprintf(t, "%02d", t_Duration); // Usa membro da classe
        lcd->print(t);
        lcd->print(":00  ");
    }
    return t_Duration;
}


void resistivo::printTime() {
    lcd->setCursor(0, 0);
    lcd->print("Duration: ");
    sprintf(t, "%02d", lcd_timer->getMinutes()); // Usa membro da classe
    lcd->print(t);
    lcd->print(":");
    sprintf(t, "%02d", lcd_timer->getSeconds()); // Usa membro da classe
    lcd->print(t);
}