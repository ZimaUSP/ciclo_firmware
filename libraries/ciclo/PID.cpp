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
#include "config.hpp"

/*****************************************
 * Class Methods Bodies Definitions
 *****************************************/

PID::PID(double k_p,double k_i,double k_d,int i_sat) {
  this->k_p = k_p;
  this->k_i = k_i;
  this->k_d = k_d; 
  this->wind_up_limit = i_sat;
  this->previous_input = 0;
  this->integrative_term = 0;
  this->error = 0;
  this->previous_time = 0;
  this->previous_error = 0;
  this->input_index = 0;
  this->input_count = 0;
  this->previous_input_avg = 0.0;
  this->proportional = 0;
  this->integrative = 0;
  this->derivative = 0;
  for (int i = 0; i < INPUT_AVG_BUFFER_SIZE; ++i) this->previous_inputs[i] = 0.0;
}

double PID::computePID(double input,float setpoint,float tolerancia) {
  if (this->previous_time == 0) {
    // first call: initialize time and buffers to avoid spikes
    this->previous_time = millis();
    for (int i = 0; i < INPUT_AVG_BUFFER_SIZE; ++i) this->previous_inputs[i] = input;
    this->input_count = INPUT_AVG_BUFFER_SIZE;
    this->input_index = 0;
    this->previous_input_avg = input;
    this->previous_input = input;
    return 0;
  }

  this->current_time = millis(); // get current time
  this->delta_time = (double)(this->current_time - this->previous_time); // time since previous
  double dt_seconds = this->delta_time / 1000.0;

  this->error = setpoint - input; // determine error

  // push new input into circular buffer of recent inputs
  this->previous_inputs[this->input_index] = input;
  this->input_index = (this->input_index + 1) % INPUT_AVG_BUFFER_SIZE;
  if (this->input_count < INPUT_AVG_BUFFER_SIZE) this->input_count++;

  // compute current average of available inputs(functions as a low-pass filter to reduce noise in derivative)
  double sum = 0.0;
  for (int i = 0; i < this->input_count; ++i) sum += this->previous_inputs[i];
  double current_avg = (this->input_count > 0) ? (sum / this->input_count) : 0.0;

  // derivative = difference between current average and previous average
  double avg_derivative = 0.0;
  if (dt_seconds > 0) {
    avg_derivative = (current_avg - this->previous_input_avg) / dt_seconds;
  }
  if (abs(this->error) < tolerancia) {
    avg_derivative = 0; // no derivative contribution if error within tolerance
  }

  this->proportional = this->k_p * this->error;
  this->derivative = -this->k_d * avg_derivative; // negative sign because derivative acts against the change

  // Stop storing integral error when the controller is already saturated in the
  // same direction as the current error. This avoids a large overspeed after a stall.
  double unclamped_output = this->proportional + (this->k_i * this->integrative_term) + this->derivative;
  bool saturating_high = unclamped_output >= passive_pid_output_limit && this->error > 0;
  bool saturating_low = unclamped_output <= -passive_pid_output_limit && this->error < 0;

  if (dt_seconds > 0 && !saturating_high && !saturating_low) {
    this->integrative_term += this->error * dt_seconds;
    if (this->integrative_term > this->wind_up_limit) {
      this->integrative_term = this->wind_up_limit;
    }
    else if (this->integrative_term < -this->wind_up_limit) {
      this->integrative_term = -this->wind_up_limit;
    }
  }

  this->integrative = this->k_i * integrative_term;

  // PID output uses the average-based derivative (difference of averages)
  double out = proportional + integrative + derivative;

  if (out < -passive_pid_output_limit) { // Safety measure
    out = -passive_pid_output_limit;
  } else if (out > passive_pid_output_limit) {
    out = passive_pid_output_limit;
  }

  this->previous_error = this->error;                 // remember current error
  this->previous_time = this->current_time;           // remember current time
  this->previous_input = input;                       // remember current input
  this->previous_input_avg = current_avg;             // remember current average for next derivative
  return out;
}

void PID::reset() {
  this->error = 0;
  this->integrative_term = 0;
  this->d_error = 0;
  this->input_error = 0;
  this->previous_time = 0;
  this->previous_error = 0;                              //remember current time
  this->previous_input = 0;
  this->previous_input_avg = 0.0;
  this->input_index = 0;
  this->input_count = 0;
  for (int i = 0; i < INPUT_AVG_BUFFER_SIZE; ++i) this->previous_inputs[i] = 0.0;
}

void PID::imprimir() {
  Serial.print("Proporcional:");
  Serial.print(this->proportional);
  Serial.print(",");
  Serial.print("; Integrativo:");
  Serial.print(this->integrative);
  Serial.print(",");
  Serial.print("; Derivativo:"); 
  Serial.println(this->derivative); 
}
