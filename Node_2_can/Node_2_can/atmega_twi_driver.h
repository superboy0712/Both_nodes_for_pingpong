/* This file has been prepared for Doxygen automatic documentation generation.*/
/*! \file *********************************************************************
 *
 * \brief  MEGA TWI master driver header file.
 *
 *      This file contains the function prototypes and enumerator definitions
 *      for various configuration parameters for the MEGA TWI master driver.
 *
 *      The driver is not intended for size and/or speed critical code, since
 *      most functions are just a few lines of code, and the function call
 *      overhead would decrease code performance. The driver is intended for
 *      rapid prototyping and documentation purposes for getting started with
 *      the MEGA TWI master module.
 *
 *      For size and/or speed critical code, it is recommended to copy the
 *      function contents directly into your application instead of making
 *      a function call.
 *
 * 
 * $Date: 2011-10-10 13:03:43 $  \n
 *
 * Copyright (c) 2008, Atmel Corporation All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 *
 * 3. The name of ATMEL may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE EXPRESSLY AND
 * SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *****************************************************************************/

#ifndef AT_MEGA_TWI_DRIVER
#define AT_MEGA_TWI_DRIVER 
//*********************************MEGAAVR TWI PARAMETERS************************************************************************

/*! \brief Largest message size that will be sent/received excluding address byte and register address. When using array write/read functions, the count parameter can not exceed NUM_BYTES*/
#define NUM_BYTES 16

//BAUD RATE PARAMETERS
//The SCL frequency is calculated as (CPU_CLOCK FREQ)/(16 + 2*TWBR*(4^TWPS))
//User must select the follwing values carefully depeding on CPU clock and required frequency.
//SCL frequency should be less than or equal to 100KHz for most users

#define TWI_TWBR          0x40    //!< TWI Bit rate Register setting. for 12MHz CPU clock, 75 KHz SCL. See Application note for detailed information on setting this value.
#define TWI_TWPS          0x00      //!<  This driver presumes prescaler = 00

//***************************************************************************************************************************************

/*! \brief Status register definitions */
union TWI_statusReg                       // Status byte holding flags.
{
    unsigned char all;                 /*!< All bits of this variable*/
    struct
    {
        unsigned char lastTransOK:1;   /*!< Only use one bit, 1 if last TWI transfer was OK, else 0 */    
        unsigned char unusedBits:7;    /*!< 7 Unused bits */
    };
};
extern union TWI_statusReg TWI_statusReg;


void twi_master_initialise( void );
unsigned char twi_transceiver_busy( void );
unsigned char twi_get_state_info( void );
void twi_start_transceiver_with_data( unsigned char * , unsigned char, unsigned char, unsigned char );
void twi_start_transceiver( void );
unsigned char twi_get_data_from_transceiver( unsigned char *, unsigned char );


void atmel_led_drvr_init();
unsigned char atmel_led_drvr_writeregister(unsigned char slave_address, unsigned char REG_ADDR, unsigned char REG_DATA);
unsigned char atmel_led_drvr_readregister(unsigned char slave_address, unsigned char REG_ADDR, unsigned char* receivedData);
unsigned char atmel_led_drvr_writearray(unsigned char slave_address, unsigned char REG_ADDR, unsigned char* Data, unsigned char count);
unsigned char atmel_led_drvr_readarray(unsigned char slave_address, unsigned char REG_ADDR, unsigned char* Data, unsigned char count);


/*! \brief Bit position for R/W bit in "address byte". */
#define TWI_READ_BIT  0     

/*! \brief Bit position for LSB of the slave address bits in the init byte. */
#define TWI_ADR_BITS  1       

/*Boolean defintions */
#define TRUE          1 //!< TRUE = 1 
#define FALSE         0 //!< FALSE = 0


/*General TWI Master status codes  */                    
#define TWI_START                  0x08  //!< START has been transmitted  
#define TWI_REP_START              0x10  //!< Repeated START has been transmitted
#define TWI_ARB_LOST               0x38  //!< Arbitration lost

/*TWI Master Transmitter status codes     */                 
#define TWI_MTX_ADR_ACK            0x18  //!< SLA+W has been tramsmitted and ACK received
#define TWI_MTX_ADR_NACK           0x20  //!< SLA+W has been tramsmitted and NACK received 
#define TWI_MTX_DATA_ACK           0x28  //!< Data byte has been tramsmitted and ACK received
#define TWI_MTX_DATA_NACK          0x30  //!< Data byte has been tramsmitted and NACK received 

/*TWI Master Receiver status */  
#define TWI_MRX_ADR_ACK            0x40  //!< SLA+R has been tramsmitted and ACK received
#define TWI_MRX_ADR_NACK           0x48  //!< SLA+R has been tramsmitted and NACK received
#define TWI_MRX_DATA_ACK           0x50  //!< Data byte has been received and ACK tramsmitted
#define TWI_MRX_DATA_NACK          0x58  //!< Data byte has been received and NACK tramsmitted

/*TWI Slave Transmitter status codes */
#define TWI_STX_ADR_ACK            0xA8  //!< Own SLA+R has been received; ACK has been returned
#define TWI_STX_ADR_ACK_M_ARB_LOST 0xB0  //!< Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been returned
#define TWI_STX_DATA_ACK           0xB8  //!< Data byte in TWDR has been transmitted; ACK has been received
#define TWI_STX_DATA_NACK          0xC0  //!< Data byte in TWDR has been transmitted; NOT ACK has been received
#define TWI_STX_DATA_ACK_LAST_BYTE 0xC8  //!< Last data byte in TWDR has been transmitted (TWEA = �0�); ACK has been received

/*TWI Slave Receiver status codes */
#define TWI_SRX_ADR_ACK            0x60  //!< Own SLA+W has been received ACK has been returned
#define TWI_SRX_ADR_ACK_M_ARB_LOST 0x68  //!< Arbitration lost in SLA+R/W as Master; own SLA+W has been received; ACK has been returned
#define TWI_SRX_GEN_ACK            0x70  //!< General call address has been received; ACK has been returned
#define TWI_SRX_GEN_ACK_M_ARB_LOST 0x78  //!< Arbitration lost in SLA+R/W as Master; General call address has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_ACK       0x80  //!< Previously addressed with own SLA+W; data has been received; ACK has been returned
#define TWI_SRX_ADR_DATA_NACK      0x88  //!< Previously addressed with own SLA+W; data has been received; NOT ACK has been returned
#define TWI_SRX_GEN_DATA_ACK       0x90  //!< Previously addressed with general call; data has been received; ACK has been returned
#define TWI_SRX_GEN_DATA_NACK      0x98  //!< Previously addressed with general call; data has been received; NOT ACK has been returned
#define TWI_SRX_STOP_RESTART       0xA0  //!< A STOP condition or repeated START condition has been received while still addressed as Slave

/*TWI Miscellaneous status codes */
#define TWI_NO_STATE               0xF8  //!< No relevant state information available; TWINT = �0�
#define TWI_BUS_ERROR              0x00  //!< Bus error due to an illegal START or STOP condition

#endif
