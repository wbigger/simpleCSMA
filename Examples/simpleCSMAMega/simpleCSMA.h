//#include <SoftwareSerial.h>
#ifndef __myprotoCSMA__
#define __myprotoCSMA__
#include <inttypes.h>
#include "Arduino.h"
#define MAX_BUFFER 		64
#define STOP_BIT  		5  
#define MSG  			1 	
#define ACK  			129 //(100000001)  il primo (MSB) bit è un ack bit
#define TBASE			20
#define MAXATTEMPTS  	5
#define WNDW    		20
// SlotTime = CCATime + RxTxTurnaroundTime + AirPropagationTime+ MACProcessingDelay 
// SIFS < DIFS < EIFS
#define DIFS 			5	//DIFS =  SIFS  + (2 * Slot time) 
#define SIFS 			1
#define TXTIMEOUT 		2000
#define DEBUG  			1

#if (DEBUG)
	#define DEBUG_PRINT(x)   	Serial.print (x)
	#define DEBUG_PRINTLN(x)  	Serial.println (x)
#else
	#define DEBUG_PRINT(x)
	#define DEBUG_PRINTLN(x) 
#endif		
//SoftwareSerial mySerial(3, 4); // RX, TX
enum PROTO_STATE
{
    WAITSTATE             	  	= 1,
    ACKSTATE                  	= 2,
	BACKOFF_STARTED				= 3,
	TRANSMIT_INTERRUPTED		= 4
};

enum ERR_LIST
{
    ERR_BUFF_OVERFLOW             = -1,
    ERR_BAD_CRC                   = -2,
    ERR_EXCEPTION                 = -3
};

enum MESSAGE
{
    DA                           = 0, //!< ID field
	SA,
    GROUP, 		//!< Function code position
    SI, 		//!< Service identifier
    BYTE_CNT,  	//!< byte counter
	PAYLOAD 	//!<  start of data
};

typedef struct
{
    uint8_t u8da;          /*!< Slave address between 1 and 247. 0 means broadcast */ 
	uint8_t u8sa;          /*!< Slave address between 1 and 247. 0 means broadcast */
    uint8_t u8group;         /*!< Function code: 1, 2, 3, 4, 5, 6, 15 or 16 */
    uint8_t u8si;    /*!< Address of the first register to access at slave/s */
    uint8_t *data;     /*!< Pointer to memory image in master */
	uint8_t msglen;
} modbus_t;

uint8_t getMySA();

uint8_t getMyGroup();

void init(Stream *, uint8_t , uint8_t , uint8_t , uint32_t);

extern void rcvEventCallback(modbus_t* );

inline long getBackoff();

inline void parallelToSerial(const modbus_t *);
//--------------------------------------------------------------------------------------------------------------
// Converte il messaggio dal formato parallelo (struct) a quello seriale (array di char)
bool sendMsg(modbus_t *);

void resendMsg(const modbus_t *);

int8_t poll(modbus_t *, uint8_t *);
//----------------------------------------------------------------------------------------------------------
void sendTxBuffer(uint8_t);

int8_t getRxBuffer();

void rcvEvent(modbus_t* , uint8_t );
//--------------------------------------------------------------------------------------------------------------
//lo calcola dal primo byte del messaggio (header compreso)
uint16_t calcCRC(uint8_t);
#endif //myprotoCSMA