
#include "bespinSensor.h"

int readHumidity(DHT *dht, float *Humidity)
{
    int error = SUCCESS;
    float humidity = 0;
    
    humidity = dht->getHumidity();
    if (isnan(humidity)) {
      Serial.println("Humidity Error");
      error = ERROR_INTERNAL;
      goto error;
    } 
    
    *Humidity = humidity;
    
cleanup:
    
    return error;
    
error:

    goto cleanup;
}

int readTemperature(DHT *dht, bool isMetric, float *Temperature)
{
    int error = SUCCESS;
    float temperature = 0;
    
    temperature = dht->getTemperature();
    if (isnan(temperature)) {
        Serial.println("Temperature Error");
        error = ERROR_INTERNAL;
        goto error;
    } else if(isMetric != true) {
        temperature = dht->toFahrenheit(temperature);
    }
    
    *Temperature = temperature;

cleanup:
    
    return error;
    
error:

    goto cleanup;
}

int readCo2(kSeries k_30, double *Co2)
{
    int error = SUCCESS;
    double co2 = 0;
    
    co2 = k_30.getCO2('p');
    
    if (co2 < 0)
    {
        Serial.println("Co2 Error");
        error = ERROR_INTERNAL;
        goto error;
    }
    
    *Co2 = co2;

cleanup:

    return error;
    
error:

    goto cleanup;
}


