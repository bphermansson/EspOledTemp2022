
#include <Arduino.h>
#include "Adafruit_HTU21DF.h"
#include "htu21d.h"
Adafruit_HTU21DF htu = Adafruit_HTU21DF();

void init_htu(htuvalues *htval)
{
  if (!htu.begin()) 
  {  
    htval->state = error;
  }
  else 
  {
    htval->state = ok;
  }
}

void read_htu(htuvalues *htval)
{
  if(htval->state != error)
  {
    htval->temp = htu.readTemperature();
    htval->humidity = htu.readHumidity();
  }   
}
