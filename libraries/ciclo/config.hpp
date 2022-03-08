#ifndef __CONFIG_HPP__
#define __CONFIG_HPP__


/**
 * @file  config.hpp
 *
 * @brief firmware configuration
 * 
 * @copyright MIT License
 */

//spi config
#define SPI_MODE SPI_MODE0 //SPI_MODE0 or SPI_MODE1 or SPI_MODE2 or SPI_MODE3
#define SPI_MASTER_FREQ SPI_MASTER_FREQ_8M
#define DMA_CHANNEL 1
#define QUEUE_SIZE 1
#define SPI_TYPE HSPI //HSPI or VSPI 
#define TARGET_SIZE 16
#define HEADER_SIZE 2
#define BUFFER_SIZE TARGET_SIZE + 4
#define FOOTER_SIZE HEADER_SIZE

//i2c configuration
    // set the max number of bytes the slave will send.
    // if the slave send more bytes, they will still be read
    // but the WireSlaveRequest will perform more reads
    // until a whole packet is read
#define max_i2c_slave_response 32


// State
#define STAND_BY 0
#define ACTIVE 1
#define PASSIVE 2



//  Encoder

// Comum used
#define Nominal_pulses 360
#define pitch_pulley 125
#define Mode 4

#define A_pin 2 // Green cable
#define B_pin 3 // White cable


// BTS
#define MAX_PWM 50
#define R_pin 6 // L Bts
#define L_pin 5 // R Bts


// PID
#define kp  2
#define ki  0
#define kd  0.01

#endif 