
float hum;    // Stores humidity value in percent
float temp;   // Stores temperature value in Celcius
float pres;

unsigned pmsAto10;
unsigned pmsAto2_5;
unsigned pmsAto1;
float pmsForm;
int aqi;
char aqiString[10];

const int max_length = 40;
char msg[max_length];   // initialise storage buffer (i haven't tested to this capacity.)

