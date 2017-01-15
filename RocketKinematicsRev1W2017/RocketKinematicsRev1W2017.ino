/* 
 PH131 Winter 2017
 Arduino program to record rocket kinematics
 based on SD card datalogger
 by Tom Igoe`
 This example code is in the public domain.
 parker 15 jan 17
 */
 
const long ms_per_flight = 60000;  // adjust
const long ms_per_point = 10;   // adjust: 10 ms seems feasible

const int chipSelect = 4;  // for SD
const int ledPin = 19; // pin 19 is marked A5: turns on green LED, low true 
 
#include <SD.h>
#include <Wire.h>
#include <LIS331.h>   // custom .h and .cpp by parker in the library
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// Sparkfun LIS331
LIS331 lis = LIS331();
// Adafruit BMP280
Adafruit_BMP280 bme;

File dirRoot;  // we seem to need this extra "root" to list files...
File dataFile;
int nextFileNr;
String fileName = "";
int fileNr = 0;
int maxFileNr = 0;
long start_time;
bool first_loop;

void setup() {

  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);  // low true: turns off LED
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  
  Serial.println("First step: Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }  

  // we'll use the initialization code from the utility libraries
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    Serial.println("initialization failed. Things to check:");
    Serial.println("* is a card inserted?");
    Serial.println("* is your wiring correct?");
    return;
  } else {
    Serial.println("Wiring is correct and a card is present.");
    Serial.println("card initialized.");
  }
  // print the type of card
  Serial.println("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      Serial.println("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      Serial.println("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      Serial.println("SDHC");
      break;
    default:
      Serial.println("Unknown");
  }
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    Serial.println("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    return;
  }  
  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  Serial.print("Volume type is FAT ");
  Serial.println(volume.fatType());
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  Serial.print("Volume size (bytes): ");
  Serial.println(volumesize);
  volumesize /= 1024;
  Serial.print("Volume size (Kbytes): ");
  Serial.println(volumesize);
  volumesize /= 1024;
  Serial.print("Volume size (Mbytes): ");
  Serial.println(volumesize);
  Serial.println("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  
  dirRoot = SD.open("/");
  nextFileNr = 1 + findMaxFileNr(dirRoot); 
  Serial.println("nextFileNr = "+String(nextFileNr));
  dirRoot.close();

  fileName = String(nextFileNr)+"_data.txt";  
  dataFile = SD.open(fileName, FILE_WRITE);
  Serial.println("fileName = "+fileName);

  if (! dataFile) {
    Serial.println("error opening "+fileName);
    // don't do anything more:
    return;
  }
   
  if (!lis.begin(0x18)) {   // change this to 0x19 for alternative LIS331 i2c address
    dataFile.println("Could not find a valid LIS331 sensor");
    Serial.println("Could not find a valid LIS331 sensor");
    // don't do anything more:
    return;
  }  
  dataFile.println("LIS331 found!");
  Serial.println("LIS331 found!");

  if (!bme.begin()) {  // BMP280 driver has default I2C address provided
    dataFile.println("Could not find a valid BMP280 sensor");
    Serial.println("Could not find a valid BMP280 sensor");
    // don't do anything more:
    return;
  }  
  dataFile.println("BMP280 found!");
  Serial.println("BMP280 found!");  
  
  first_loop = true;  
}

void loop() {
    // make a string for assembling the data to log:
    String dataString = "";
    static long previous_elapsed_time;
    long elapsed_time;    
    
    if ( first_loop == true ) {
      dataFile.println("Waiting 5 secs for accelerometer to settle");
      Serial.println("Waiting 5 secs for accelerometer to settle");
      start_time = millis();
      do {
        ;
      }  while (millis() - start_time < 5000); 
      digitalWrite(ledPin, LOW);  // indicate that loop has started
      first_loop = false;
      start_time = millis();
    }
            
    do {
      elapsed_time = millis() - start_time;
    } while ( elapsed_time - previous_elapsed_time < ms_per_point );    
    previous_elapsed_time = elapsed_time;

    // Note: Adafruit bmp280test waits 2000 ms between readings... 

    /* get a new LIS331 sensor event, normalized */ 
    sensors_event_t event; 
    lis.getEvent(&event);    
    dataString += "ms: ";
    dataString += elapsed_time;
    dataString += " \tX: "; 
    dataString += event.acceleration.x;
    dataString += " \tY: "; 
    dataString += event.acceleration.y;
    dataString += " \tZ: "; 
    dataString += event.acceleration.z;
    dataString += " m/s^2 ";

    /* get BMP280 datqa */
    dataString += " \tTemp: ";
    dataString += bme.readTemperature();
    dataString += " C "; 
    dataString += " \tPressure: ";
    dataString += bme.readPressure();
    dataString += " Pa ";    
    dataString += " \tApprox altitude: ";
    dataString += bme.readAltitude(1013.25);  // this should be adjusted to your local forecast
    dataString += " m ";       
    
    dataFile.println(dataString);
    // if debugging, print to the serial port too:
    Serial.println(dataString);  ////////////////////////////////////////////
  
    if ( elapsed_time > ms_per_flight ) {  
      dataFile.println("End of data capture, closing file");
      Serial.println("End of data capture, closing file");
      dataFile.close();
      digitalWrite(ledPin, HIGH);  // turn off the LED
      while (true ) {
        ; // trap
      }
    }  
}

int findMaxFileNr(File dir) {
  while (true) {
    File entry =  dir.openNextFile();
    if (! entry) {
      // no more files
      break;
    }
    fileName = String(entry.name());
    fileNr = fileName.toInt();
    Serial.print(entry.name()); 
    Serial.print(": "); 
    Serial.println(String(fileNr)); 

    if (fileNr > maxFileNr){
      maxFileNr = fileNr;
    } 
    entry.close();     
  }
  return maxFileNr;
}









