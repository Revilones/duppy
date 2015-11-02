
#include <kSeries.h>
#include <DHT.h>
#include <duppyTypes.h>

int readHumidity(DHT *dht, float *Humidity);
int readTemperature(DHT *dht, bool isMetric, float *Temperature);
int readCo2(kSeries k_30, double *Co2);
