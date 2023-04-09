#include <stdint.h>

typedef struct 
{
    float temp;
    float humidity;
    uint8_t state;
} htuvalues;

enum htustates 
{
    error,
    ok
};

void init_htu(htuvalues *htval);
void read_htu(htuvalues *htval);
//void update_htu();

