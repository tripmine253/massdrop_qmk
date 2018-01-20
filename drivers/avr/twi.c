#include <avr/io.h>
#include <util/twi.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h>
#include "twi.h"

#ifndef F_SCL
    #define F_SCL 100000UL // SCL frequency
#endif
#ifndef PRESCALER
    #define PRESCALER 1
#endif

// Limits the amount of we wait for any one i2c transaction.
// Since were running SCL line 100kHz (=> 10μs/bit), and each transactions is
// 9 bits, a single transaction will take around 90μs to complete.
//
// (F_CPU/SCL_CLOCK)  =>  # of μC cycles to transfer a bit
// poll loop takes at least 8 clock cycles to execute

#ifdef TWI_TIMEOUT
    #ifndef TWI_TX_SIZE
        #define TWI_TX_SIZE 9
    #endif
#endif

// Wait for an twi operation to finish
inline static
void twi_wait(uint8_t status) {
    #ifdef TWI_TIMEOUT
        uint16_t lim = 0;
        while ( !(TWCR & (_BV(status))) && lim < (TWI_TX_SIZE+1)*(F_CPU/F_SCL)/8)
            lim++;
    #else 
        while ( !(TWCR & (_BV(status))) );
    #endif
}

void twi_master_init(void) {
    TWBR = (uint8_t) ((((F_CPU / F_SCL) / PRESCALER) - 16 ) / 2);
}

void twi_slave_init(uint8_t address) {
    TWAR = address << 0; // slave i2c address
    TWCR = _BV(TWIE) | _BV(TWEA) | _BV(TWINT) | _BV(TWEN);
}

uint8_t twi_master_start(uint8_t address) {
    // reset TWI control register and transmit start
    TWCR = 0;
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTA);

    twi_wait(TWINT);
    
    // check if the start condition was successfully transmitted
    if (TW_STATUS != TW_START)
        return 1;
    
    // set slave address and transmit
    TWDR = address;
    TWCR = _BV(TWINT) | _BV(TWEN);

    twi_wait(TWINT);

    // check if the device has acknowledged the READ / WRITE mode
    if ( (TW_STATUS != TW_MT_SLA_ACK) && (TW_STATUS != TW_MR_SLA_ACK) ) 
        return 1;
    
    return 0;
}

void twi_master_stop(void) {
    // transmit STOP condition
    TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWSTO);

    twi_wait(TWSTO);
}

uint8_t twi_write(uint8_t data) {
    // load data into data register and start transmission of data
    TWDR = data;
    TWCR = _BV(TWINT) | _BV(TWEN);

    twi_wait(TWINT);
    
    if ( TW_STATUS != TW_MT_DATA_ACK )
        return 1;
    
    return 0;
}

uint8_t twi_read(bool ack) {
    // start TWI module and acknowledge data after reception
    if (ack)
        TWCR = _BV(TWINT) | _BV(TWEN) | _BV(TWEA); 
    else
        TWCR = _BV(TWINT) | _BV(TWEN);
    
    twi_wait(TWINT);

    return TWDR;
}

uint8_t twi_transmit(uint8_t address, uint8_t* data, uint16_t length) {
    if (twi_start(address | TW_WRITE)) 
        return 1;
    
    for (uint16_t i = 0; i < length; i++) {
        if (twi_write(data[i])) 
            return 1;
    }
    
    twi_stop();
    
    return 0;
}

uint8_t twi_receive(uint8_t address, uint8_t* data, uint16_t length) {
    if (twi_start(address | TW_READ)) 
        return 1;
    
    for (uint16_t i = 0; i < (length-1); i++) {
        data[i] = twi_read(true);
    }
    data[(length-1)] = twi_read(false);
    
    twi_stop();
    
    return 0;
}

uint8_t twi_writeReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length) {
    if (twi_start(devaddr | TW_WRITE)) 
        return 1;

    twi_write(regaddr);

    for (uint16_t i = 0; i < length; i++) {
        if (twi_write(data[i])) 
            return 1;
    }

    twi_stop();

    return 0;
}

uint8_t twi_readReg(uint8_t devaddr, uint8_t regaddr, uint8_t* data, uint16_t length)
{
    if (twi_start(devaddr)) 
        return 1;

    twi_write(regaddr);

    if (twi_start(devaddr | TW_READ)) 
        return 1;

    for (uint16_t i = 0; i < (length-1); i++) {
        data[i] = twi_read(true);
    }
    data[(length-1)] = twi_read(false);

    twi_stop();

    return 0;
}

uint8_t buffer_address = 0;

ISR(TWI_vect) {
    uint8_t data;
    switch (TW_STATUS) {
        case TW_SR_SLA_ACK:
            buffer_address = 0xff;
            TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
            break;
        case TW_SR_DATA_ACK:
            data = TWDR;
            if (buffer_address == 0xff) {
                // store address to read from later
                buffer_address = data;
                TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
            } else {
                // store data from address and increment
                rxbuffer[buffer_address] = data;
                buffer_address++;

                if (buffer_address < 0xFF) {
                    // ack
                    TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
                } else {
                    // nack
                    TWCR &= ~_BV(TWEA);
                    TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEN);
                }

            }
            break;
        case TW_ST_SLA_ACK:
        case TW_ST_DATA_ACK:
            data = TWDR;

            if (buffer_address == 0xFF) {
                buffer_address = data;
            }

            TWDR = txbuffer[buffer_address];
            buffer_address++;


            if (buffer_address < 0xFF) {
                // ack
                TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEA) | _BV(TWEN);
            } else {
                // nack
                TWCR &= ~_BV(TWEA);
                TWCR |= _BV(TWIE) | _BV(TWINT) | _BV(TWEN);
            }
            break;
        case TW_BUS_ERROR:
            TWCR = 0;
            break;
        default:
            TWCR |= _BV(TWIE) | _BV(TWEA) | _BV(TWEN);
            break;
    }
}