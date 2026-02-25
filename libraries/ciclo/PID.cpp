/**
 * @file  PID.cpp
 *
 * @brief PID class
 *
 * @author Thomaz Akira Furukawa <thomazakira@usp.br>
 *
 * @date 10/2021
 * 
 * @copyright MIT License
 */


#include "PID.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

PID::PID(double k_p,double k_i,double k_d,int i_sat) {
    this->k_p = k_p;
    this->k_i = k_i;
    this->k_d = k_d; 
    this->i_sat = i_sat;
}

double PID::computePID(double input,float setpoint,float tolerancia) {
  

  this->current_time= 400;                                              //get current time: this->current_time= millis();
  this->delta_time = 400;        //compute time elapsed from previous computation: (double)(this->current_time - this->previus_time);


  this->error = setpoint - input;                                      // determine error
  this->i_error +=  this->error *  this->delta_time;
  if(this->i_error > this->i_sat){
    this->i_error = this->i_sat;

    }else if(this->i_error < -this->i_sat){
      this->i_error=  -this->i_sat;

    }
  
                             // compute integral
  
  this->d_error = (this->error - this->previus_error) / this->delta_time;             // compute derivative
  
  this->proportional = this->k_p * this->error;
  this->integrative = this->k_i * this->i_error;
  this ->derivative = this->k_d * this->d_error;
  double out = proportional + integrative + derivative;  //PID output
  
  this->previus_error = this->error;                                         //remember current error
  this->previus_time =  this->current_time;                                //remember current time
  return out;                                                 //have function return the PID output
}

void PID::reset() {
    this->error = 0;
    this->i_error = 0;
    this->d_error = 0;
    this->previus_time = millis();
}

void PID::imprimir() {
  Serial.print("Proporcional: ");
  Serial.print(this->proportional);
  Serial.print("; Integrativo: ");
  Serial.print(this->integrative);
  Serial.print("; Derivativo: "); 
  Serial.println(this->derivative); 
}