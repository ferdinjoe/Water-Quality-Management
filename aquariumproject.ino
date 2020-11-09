/*
Aquarium Monitoring System Using IoT
Ferdin Joe John Joseph
Faculty of Information Technology
Thai-Nichi Institute of Technology

The Original of this source code is downloadable from http://github.com/ferdinjoe
*/
#include <SoftwareSerial.h>
SoftwareSerial pmsSerial(2, 3);
#include <dht.h>
#include <DallasTemperature.h>
#include <OneWire.h>
// Example By ArduinoAll
# define  ONE_WIRE_BUS  5  // specifies that the sensor pin 18B20 is connected to pin 2
OneWire oneWire (ONE_WIRE_BUS);
DallasTemperature sensors (& oneWire);
# define  ONE_WIRE_BUS  5  // specifies that the sensor pin 18B20 is connected to pin 2
dht DHT;
int buf[10],temp;
float calibration = 21.34; //change this value to calibrate
unsigned long int avgValue;

#define DHT11_PIN 4
 // Digital pin 8 will be called 'pin8'
int pin8 = 8;
// Analog pin 0 will be called 'sensor'
int sensor = A0;
// Set the initial sensorValue to 0
int cosensorValue = 0;
void setup() {
  // our debugging output
  Serial.begin(115200);
  sensors. begin ();
  //Serial.println("Initializing.... Please Wait!");
 
  // sensor baud rate is 9600
  // Initialize the digital pin 8 as an output
  pinMode(pin8, OUTPUT);
  pmsSerial.begin(9600);
}
 
struct pms5003data {
  uint16_t framelen;
  uint16_t pm10_standard, pm25_standard, pm100_standard;
  uint16_t pm10_env, pm25_env, pm100_env;
  uint16_t particles_03um, particles_05um, particles_10um, particles_25um, particles_50um, particles_100um;
  uint16_t unused;
  uint16_t checksum;
};
 
struct pms5003data data;
    
void loop() {
  // Read the input on analog pin 0 (named 'sensor')
  
  if (readPMSdata(&pmsSerial)) {
    // reading data was successful!
    delay(3000);
    //Serial.println();
   // Serial.println("---------------------------------------");
    //Serial.println("Concentration Units (standard)");
    //Serial.print("PM 1.0: "); Serial.print(data.pm10_standard);
    //Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_standard);
    //Serial.print("\t\tPM 10: "); Serial.println(data.pm100_standard);
    //Serial.println("---------------------------------------");
    //Serial.println("Concentration Units (environmental)");
    //Serial.print("PM 1.0: "); Serial.print(data.pm10_env);
    //Serial.print("\t\tPM 2.5: "); Serial.print(data.pm25_env);
   // Serial.print("\t\tPM 10: "); Serial.println(data.pm100_env);
    //Serial.println("---------------------------------------");
    //Serial.print("Particles > 0.3um / 0.1L air:"); Serial.println(data.particles_03um);
    //Serial.print("Particles > 0.5um / 0.1L air:"); Serial.println(data.particles_05um);
   // Serial.print("Particles > 1.0um / 0.1L air:"); Serial.println(data.particles_10um);
    //Serial.print("Particles > 2.5um / 0.1L air:"); Serial.println(data.particles_25um);
    //Serial.print("Particles > 5.0um / 0.1L air:"); Serial.println(data.particles_50um);
    //Serial.print("Particles > 10.0 um / 0.1L air:"); Serial.println(data.particles_100um);
    //Serial.println("---------------------------------------");
    cosensorValue = analogRead(sensor);
    sensors.requestTemperatures (); // add to read temperature

    float wtemp=sensors.getTempCByIndex ( 0 );
    for(int i=0;i<10;i++) 
 { 
 buf[i]=analogRead(A3);
 delay(30);
 }
 for(int i=0;i<9;i++)
 {
 for(int j=i+1;j<10;j++)
 {
 if(buf[i]>buf[j])
 {
 temp=buf[i];
 buf[i]=buf[j];
 buf[j]=temp;
 }
 }
 }
 avgValue=0;
 for(int i=2;i<8;i++)
 avgValue+=buf[i];
 float pHVol=(float)avgValue*5.0/1024/6;
 float phValue = (-5.70 * pHVol) + 11;
 //Serial.print(pHVol);
 //Serial.println(phValue);
    //Serial.print (wtemp); // Show temperat
  // Print out the value you read
  //Serial.print("Carbon Monoxide:");
  //Serial.print(cosensorValue, DEC);
  //Serial.println(" ppm");
  // If sensorValue is greater than 500
  if (cosensorValue > 500) {
    // Activate digital output pin 8 - the LED will light up
    digitalWrite(pin8, HIGH);
  }
  else {
    // Deactivate digital output pin 8 - the LED will not light up
    digitalWrite(pin8, LOW);
  }
  int ldr_sensor_value = analogRead(A1);
  //Serial.print("Light Sensor:");
  ldr_sensor_value=100-ldr_sensor_value;
  //Serial.println(ldr_sensor_value);
  int chk = DHT.read11(DHT11_PIN);
  //Serial.print("Temperature = ");
  //Serial.print(DHT.temperature);
  //Serial.println(" Celsius");
  //Serial.print("Humidity = ");
  //Serial.print(DHT.humidity);
  //Serial.println("%");
  //Serial.println("---------------------------------------");
  float n[ 6 ] = { data.pm10_standard, data.pm25_standard, data.pm100_standard, cosensorValue, ldr_sensor_value, DHT.temperature, DHT.humidity,wtemp, phValue } ;
  String p[9]={"PM1.0: ","PM2.5: ", "PM10: ", "CO: ","Light Intensity: ","Temperature: ", "Relative Humidity: ", "Water Temperature: ", "PH Value"};
  String stringOne = "";
  for (int ar=0;ar<9;ar++)
  {
  //Serial.print(n[ar]);
  stringOne+=n[ar];
  if(ar<8)
  {
  stringOne+=",";
  }
  else
  {
  //stringOne+=";";
  }
  //Serial.print(" ");
  
  }
  Serial.println(stringOne);
  }
}
 
boolean readPMSdata(Stream *s) {
  if (! s->available()) {
    return false;
  }
  
  // Read a byte at a time until we get to the special '0x42' start-byte
  if (s->peek() != 0x42) {
    s->read();
    return false;
  }
 
  // Now read all 32 bytes
  if (s->available() < 32) {
    return false;
  }
    
  uint8_t buffer[32];    
  uint16_t sum = 0;
  s->readBytes(buffer, 32);
 
  // get checksum ready
  for (uint8_t i=0; i<30; i++) {
    sum += buffer[i];
  }
 
  /* debugging
  for (uint8_t i=2; i<32; i++) {
    Serial.print("0x"); Serial.print(buffer[i], HEX); Serial.print(", ");
  }
  Serial.println();
  */
  
  // The data comes in endian'd, this solves it so it works on all platforms
  uint16_t buffer_u16[15];
  for (uint8_t i=0; i<15; i++) {
    buffer_u16[i] = buffer[2 + i*2 + 1];
    buffer_u16[i] += (buffer[2 + i*2] << 8);
  }
 
  // put it into a nice struct :)
  memcpy((void *)&data, (void *)buffer_u16, 30);
 
  if (sum != data.checksum) {
    //Serial.println("Checksum failure");
    return false;
  }
  // success!
  return true;
}
