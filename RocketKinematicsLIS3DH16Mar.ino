/* 
 PH131 Winter 2017
 Uppdated Dec 2019 to set DATA LED HIGH true
 NEEDS TESTING!!
 Arduino program to record rocket kinematics
 based on SD card datalogger
 by Tom Igoe`
 This example code is in the public domain.
 This version uses LIS3DH accelerometer
 and BMP280 pressure/temperature sensor
 */

//#define DEBUG  // Comment this out if USB-->PC isn't connected
 
#define baroInHg 30.00            // update this from weather bureau ////////////////////////
float mbar = baroInHg * 33.8639;  
#define ms_per_flight 60000       // adjust
#define ms_per_point 10           // adjust: 10 ms seems feasible

#ifdef DEBUG
 #define DEBUG_PRINT(x)  Serial.print(x)
 #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
 #define DEBUG_PRINT(x)
 #define DEBUG_PRINTLN(x)
#endif

#define chipSelect 4 // for SD
#define ledPin 19    // pin 19 is marked A5: turns on green LED, HIGH true

#include <SD.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_BMP280.h>

// set up variables using the SD utility library functions:
Sd2Card card;
SdVolume volume;
SdFile root;

// Sparkfun or Adafruit LIS3DH
Adafruit_LIS3DH lis;
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
  digitalWrite(ledPin, LOW);  // HIGH true: turns off LED
  
  #ifdef DEBUG 
    // Open serial communications and wait for port to open:
    Serial.begin(9600);
    while (!Serial) {
      ; // wait for serial port to connect. Needed for native USB port only    
    }  
  #endif  
  
  DEBUG_PRINTLN("First step: Initializing SD card...");
  // see if the card is present and can be initialized:
  if (!SD.begin(chipSelect)) {
    DEBUG_PRINTLN("Card failed, or not present");
    setupFailed();
  }  

  // we'll use the initialization code from the utility libraries
  if (!card.init(SPI_HALF_SPEED, chipSelect)) {
    DEBUG_PRINTLN("initialization failed. Things to check:");
    DEBUG_PRINTLN("* is a card inserted?");
    DEBUG_PRINTLN("* is your wiring correct?");
    setupFailed();
  } else {
    DEBUG_PRINTLN("Wiring is correct and a card is present.");
    DEBUG_PRINTLN("card initialized.");
  }
  // print the type of card
  DEBUG_PRINT("\nCard type: ");
  switch (card.type()) {
    case SD_CARD_TYPE_SD1:
      DEBUG_PRINTLN("SD1");
      break;
    case SD_CARD_TYPE_SD2:
      DEBUG_PRINTLN("SD2");
      break;
    case SD_CARD_TYPE_SDHC:
      DEBUG_PRINTLN("SDHC");
      break;
    default:
      DEBUG_PRINTLN("Unknown");
  }
  // Now we will try to open the 'volume'/'partition' - it should be FAT16 or FAT32
  if (!volume.init(card)) {
    DEBUG_PRINTLN("Could not find FAT16/FAT32 partition.\nMake sure you've formatted the card");
    setupFailed();
  }  
  // print the type and size of the first FAT-type volume
  uint32_t volumesize;
  DEBUG_PRINT("Volume type is FAT ");
  DEBUG_PRINTLN(volume.fatType());
  volumesize = volume.blocksPerCluster();    // clusters are collections of blocks
  volumesize *= volume.clusterCount();       // we'll have a lot of clusters
  volumesize *= 512;                            // SD card blocks are always 512 bytes
  DEBUG_PRINT("Volume size (bytes): ");
  DEBUG_PRINTLN(volumesize);
  volumesize /= 1024;
  DEBUG_PRINT("Volume size (Kbytes): ");
  DEBUG_PRINTLN(volumesize);
  volumesize /= 1024;
  DEBUG_PRINT("Volume size (Mbytes): ");
  DEBUG_PRINTLN(volumesize);
  DEBUG_PRINTLN("\nFiles found on the card (name, date and size in bytes): ");
  root.openRoot(volume);
  // list all files in the card with date and size
  root.ls(LS_R | LS_DATE | LS_SIZE);
  
  dirRoot = SD.open("/");
  nextFileNr = 1 + findMaxFileNr(dirRoot); 
  DEBUG_PRINTLN("nextFileNr = "+String(nextFileNr));
  dirRoot.close();

  fileName = String(nextFileNr)+"_data.txt";  
  dataFile = SD.open(fileName, FILE_WRITE);
  DEBUG_PRINTLN("fileName = "+fileName);

  if (! dataFile) {
    DEBUG_PRINTLN("error opening "+fileName);
    setupFailed();
  }
     
  DEBUG_PRINTLN("Trying LIS3DH sensor"); 
  if (!lis.begin(0x19)) {   // 0x18 is default LIS3DH i2c address with SDO open
    dataFile.println("Could not find a valid LIS3DH sensor");
    DEBUG_PRINTLN("Could not find a valid LIS3DH sensor");
    setupFailed();
  }  
  dataFile.println("LIS3DH found!");
  DEBUG_PRINTLN("LIS3DH found!");
  
  lis.setRange(LIS3DH_RANGE_16_G);    // 2,4,8,or 16 g  /////////////////
  dataFile.print("Range = "); dataFile.print(2 << lis.getRange());  
  dataFile.println("G");  
  DEBUG_PRINT("Range = "); DEBUG_PRINT(2 << lis.getRange());  
  DEBUG_PRINTLN("G");

  DEBUG_PRINTLN("Trying BMP280 sensor");  
  if (!bme.begin()) {  // BMP280 driver has default I2C address provided
    dataFile.println("Could not find a valid BMP280 sensor");
    DEBUG_PRINTLN("Could not find a valid BMP280 sensor");
    setupFailed();
  }  
  dataFile.println("BMP280 found!");
  DEBUG_PRINTLN("BMP280 found!");   
  first_loop = true;  
}

void loop() {
    // make a string for assembling the data to log:
    String dataString = "";
    static long previous_elapsed_time;
    long elapsed_time;
    
    if ( first_loop == true ) {
      dataFile.println("Waiting 2secs for accelerometer to settle");
      DEBUG_PRINTLN("Waiting 2 secs for accelerometer to settle");
      start_time = millis();
      do {
        ;
      }  while (millis() - start_time < 2000);
      dataString += "ms: ";
      dataString += "\tX"; 
      dataString += " \tY "; 
      dataString += " \tZ ";
      dataString += " \tTemp";
      dataString += " \tPressure ";
      dataString += " \tApprox altitude ";
      dataFile.println(dataString);
      DEBUG_PRINTLN(dataString);
      dataString = "";       
      digitalWrite(ledPin, HIGH);  // indicate that loop has started      
      first_loop = false;
      start_time = millis();
    }
            
    do {
      elapsed_time = millis() - start_time;
    } while ( elapsed_time - previous_elapsed_time < ms_per_point );    
    previous_elapsed_time = elapsed_time;

    // Note: Adafruit bmp280test waits 2000 ms between readings... 

    /* get a new LIS3DH sensor event, normalized */ 
    sensors_event_t event; 
    lis.getEvent(&event);
    dataString += elapsed_time;
    dataString += "\t";
    //dataString += " \tX: "; 
    dataString += event.acceleration.x;
    dataString += "\t";
    //dataString += " \tY: "; 
    dataString += event.acceleration.y;
    dataString += "\t";
    //dataString += " \tZ: "; 
    dataString += event.acceleration.z;
    //dataString += " m/s^2 ";
    /* get BMP280 datqa */
    //dataString += " \tTemp: ";
    dataString += "\t";
    dataString += bme.readTemperature();
    dataString += "\t";
    //dataString += " C "; 
    //dataString += " \tPressure: ";
    dataString += bme.readPressure();
    dataString += "\t";
    //dataString += " Pa ";    
    //dataString += " \tApprox altitude: ";
    dataString += "\t";
    dataString += bme.readAltitude(mbar); 
    //dataString += " m ";    
    dataFile.println(dataString);
    DEBUG_PRINTLN(dataString);
  
    if ( elapsed_time > ms_per_flight ) {  
      dataFile.println("End of data capture, closing file");
      DEBUG_PRINTLN("End of data capture, closing file");
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
    DEBUG_PRINT(entry.name()); 
    DEBUG_PRINT(": "); 
    DEBUG_PRINTLN(String(fileNr)); 

    if (fileNr > maxFileNr){
      maxFileNr = fileNr;
    } 
    entry.close();     
  }
  return maxFileNr;
}

void setupFailed() {
  while (true) {
    digitalWrite(ledPin, LOW);  // turn on LED
    delay(500);
    digitalWrite(ledPin, HIGH);  // turn off LED
    delay(500);    
  }
}









