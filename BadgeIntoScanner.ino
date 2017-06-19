#include <ESP8266WiFi.h>

#include "SH1106.h"
#include "Adafruit_MCP23008.h"
#include "ESP.h"
extern "C" {
  #include "user_interface.h"
} 
/* 
 *  Skydogcon 6 presented all participants with an AMAZING badge
 *  built with the ESP8266MOD WiFi Chip
 *  Let's use that badge for some FUN
 */

ADC_MODE(ADC_VCC);

SH1106 display(0x3c, 5,4);
Adafruit_MCP23008 mcp;

int loopMe = 0;

  int bu = 0;
  int bd = 0;
  int bl = 0;
  int br = 0;
  int bp = 0;
  int bb = 0;
  
byte packetBuffer[2000];

void setup() {
  // put your setup code here, to run once:
    mcp.begin();
  mcp.pinMode(0, INPUT);
  mcp.pullUp(0, LOW);
  mcp.pinMode(1, INPUT);
  mcp.pullUp(1, LOW);
  mcp.pinMode(2, INPUT);
  mcp.pullUp(2, LOW);
  mcp.pinMode(3, INPUT);
  mcp.pullUp(3, LOW);
  mcp.pinMode(4, INPUT);
  mcp.pullUp(4, LOW);

  mcp.pinMode(5, OUTPUT);
  mcp.pinMode(6, OUTPUT); 
  mcp.pinMode(7, OUTPUT);

  mcp.digitalWrite(5, HIGH);
  mcp.digitalWrite(6, HIGH);
  mcp.digitalWrite(7, HIGH);

  pinMode(16, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(12, OUTPUT);
  
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  
  Serial.begin(115200);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.drawString(0,0,"$ cu -l /dev/ttyS0 -s 28800");
  display.drawString(0,13,"AT#MFR?");
  display.display();

  int id = ESP.getChipId();
  char message[50];
  sprintf(message, "ATDT9,1555%d", id);
  display.drawString(0,23, "@ruff_tr <tcruff@tcruff.com>");
  display.drawString(0,33, "OK");
  display.display();
  delay(500);
  display.drawString(0,43, message);
  display.display();
  display.drawString(0,53, "CONNECT 28800 V42bis");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
    float volt = 0.00f;
    if ((digitalRead(0) == 1) && (bb == 0)) {

    sendPacket("BD");
    bb = 1;
  }

  if ((digitalRead(0) == 0) && (bb == 1))
  
  {

    sendPacket("BU");
    bb = 0;
  }

  if ((mcp.digitalRead(0) == 1) && (bp == 0)) {    

      sendPacket("PD");
      bp = 1;
  }
  if ((mcp.digitalRead(0) == 0) && (bp == 1)) {    

      sendPacket("PU");
      bp = 0;
  }
  if ((mcp.digitalRead(1) == 1) && (bu == 0)) {    

      sendPacket("UD");
      bu = 1;
  }
  if ((mcp.digitalRead(1) == 0) && (bu == 1)) {    

      sendPacket("UU");
      bu = 0;
  }
  if ((mcp.digitalRead(3) == 1) && (bd == 0)) {    

      sendPacket("DD");
      bd = 1;
  }
  if ((mcp.digitalRead(3) == 0) && (bd == 1)) {    

      sendPacket("DU");
      bd = 0;
  }
  if ((mcp.digitalRead(2) == 1) && (bl == 0)) {    

      sendPacket("LD");
      bl = 1;
  }
  if ((mcp.digitalRead(2) == 0) && (bl == 1)) {    

      sendPacket("LU");
      bl = 0;
  }
  if ((mcp.digitalRead(4) == 1) && (br == 0)) {    

      sendPacket("RD");
      br = 1;
  }
  if ((mcp.digitalRead(4) == 0) && (br == 1)) {    

      sendPacket("RU");
      br = 0;
  }

}

void sendPacket(const char *data) {
  if (data[1] == 'D')
    return;
  display.clear();
  int n = WiFi.scanNetworks();
  Serial.println("scan done");
  if (n == 0)
    display.drawString(0,0,"no networks found");
  else
  {
    char message[50];
    int j = 0;
    sprintf(message, "%02d%s", n, " networks found");
    display.drawString(0,0, message);
    for (int i = 0; i < n; ++i)
    {
      j = 13 + (i * 10);
      // Print SSID and RSSI for each network found
      sprintf(message, "%i%s", i, ": " );
      display.drawString(0,j, message + WiFi.SSID(i) + " " + WiFi.RSSI(i));
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE)?" ":"*");
      delay(10);
    }
  }
  display.display();
  delay(500);
}


