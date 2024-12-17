/****************************************************************
* PLAY MUSIC STORED ON SD CARD THROUGH AMPLIFIER
* ==============================================
*
* In this program MP3 compatible music file is stored on microSD
* card. The program opens the file and plays the music on the
* speaker through the MAX98357A amplifier
*
* Author : Dogan Ibrahim
* Program: SDMusic
* Date : April, 2023
**************************************************************/
#include "Arduino.h"
#include "Audio.h"
#include "SD.h"
#include "FS.h"
//
// microSD card to ESP32 connections
//
#define CARD_MOSI 13
#define CARD_MISO 12
#define CARD_SCK 14
#define CARD_CS 15
//
// Define MAX98357A amplifier connections
//
#define I2S_BCLK 5
#define I2S_LRC 4
#define I2S_DOUT 6
Audio audio; // Audio object
void setup()
{
// CS is output
pinMode(CARD_CS, OUTPUT); digitalWrite(CARD_CS, HIGH); // CS HIGH
SPI.begin(CARD_SCK,CARD_MISO,CARD_MOSI); // Init SPI bus
Serial.begin(115200); // Serial Monitor
if(!SD.begin(CARD_CS)) // Start SD card
{
Serial.println(F("Error in microSD card")); while(true);
// card error

//Practical Audio DSP Projects with the ESP32
}
Serial.println(F("microSD card accessed correctly"));
audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT); // Setup I2S
audio.setVolume(10); // Set volume
audio.connecttoFS(SD,"TRACK0.mp3"); // Open file
}
void loop()
{
audio.loop();
}