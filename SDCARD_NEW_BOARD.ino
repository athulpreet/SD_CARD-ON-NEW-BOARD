#include <SPI.h>
#include <SD.h>

// Pin definitions for SPI2
#define SD_CS   PB12    // CS
#define SD_SCK  PB13    // SCK
#define SD_MISO PB14    // MISO
#define SD_MOSI PB15    // MOSI

// The STM32F1 SPIClass takes a port number, not pins
// SPI1 = 1, SPI2 = 2
SPIClass SPI_2(2);  // Use SPI2

void setup() {
  // Start serial communication using Serial2 (USART2 - PA2/PA3)
  Serial2.begin(9600);
  delay(1000);
  
  Serial2.println("\nSD Card Test - SPI2 Version");

  // Configure SPI pins (must be done manually for SPI2)
  pinMode(SD_CS, OUTPUT);
  digitalWrite(SD_CS, HIGH); // Deselect SD card
  
  // Initialize SPI2 with conservative settings
  SPI_2.begin();
  SPI_2.setClockDivider(SPI_CLOCK_DIV128); // Very slow speed
  SPI_2.setDataMode(SPI_MODE0);
  SPI_2.setBitOrder(MSBFIRST);
  
  Serial2.println("\nPower-up sequence:");
  
  // Power-up sequence
  digitalWrite(SD_CS, HIGH);
  delay(100);
  
  // Send dummy clock cycles with CS high
  for(int i = 0; i < 10; i++) {
    SPI_2.transfer(0xFF);
    Serial2.print(".");
  }
  Serial2.println(" Done");
  
  delay(100);

  Serial2.println("\nInitializing SD card on SPI2...");
  Serial2.println("Using SPI2 configuration:");
  Serial2.println("CS   -> PB12");
  Serial2.println("SCK  -> PB13");
  Serial2.println("MISO -> PB14");
  Serial2.println("MOSI -> PB15");
  Serial2.println("VCC  -> 5V   (to module's VCC)");
  Serial2.println("GND  -> GND");
  
  // Use SPI2 by selecting it before initializing the SD card
  SPI.setModule(2);
  
  // Initialize SD card
  if (!SD.begin(SD_CS)) {
    Serial2.println("\nSD card initialization failed!");
    Serial2.println("\nTroubleshooting:");
    Serial2.println("1. Check physical connections to PB12-PB15");
    Serial2.println("2. Make sure level shifters are connected to these pins");
    Serial2.println("3. Verify SD card is properly seated and working");
    while(1);
  }

  Serial2.println("SD card initialization successful on SPI2!");

  // Test file operations
  File dataFile = SD.open("test.txt", FILE_WRITE);
  if (dataFile) {
    Serial2.println("\nWriting to test.txt...");
    dataFile.println("Testing SD card with SPI2");
    dataFile.println("Module is working properly on PB12-PB15!");
    dataFile.close();
    Serial2.println("Write successful!");

    // Read back the file
    dataFile = SD.open("test.txt");
    if (dataFile) {
      Serial2.println("\nReading file contents:");
      while (dataFile.available()) {
        Serial2.write(dataFile.read());
      }
      dataFile.close();
      Serial2.println("\nRead successful!");
    }
  } else {
    Serial2.println("Error opening test file!");
  }
}

void loop() {
  // Empty loop
}
