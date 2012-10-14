
#include <inttypes.h>

#define DHT22_ERROR_VALUE -995

#define DHT_ERROR_NONE          0
#define DHT_BUS_HUNG            1
#define DHT_ERROR_NOT_PRESENT   2
#define DHT_ERROR_ACK_TOO_LONG  3
#define DHT_ERROR_SYNC_TIMEOUT  4
#define DHT_ERROR_DATA_TIMEOUT  5
#define DHT_ERROR_CHECKSUM      9
#define DHT_ERROR_TOOQUICK      7

typedef struct{
    short int error;
//    unsigned long lastReadTime;
    short int lastHumidity;
    short int lastTemperature;
} dht22_result;

void dht22_readData();
dht22_result result;
uint8_t bitTimes[41];
