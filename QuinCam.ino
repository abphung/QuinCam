//This example code is in the Public Domain (or CC0 licensed, at your option.)
//By Evandro Copercini - 2018
//
//This example creates a bridge between Serial and Classical Bluetooth (SPP)
//and also demonstrate that SerialBT have the same functionalities of a normal Serial

#include "BluetoothSerial.h"
#include <esp_camera.h>
#include <EEPROM.h>
#include <list>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int address = 0;

void InitializeBluetooth() {
  Serial.println("Initializing Bluetooth");
  SerialBT.begin("QuinCam2");
  Serial.println("Finished Initializing Bluetooth");
}

void InitializeCamera() {
  Serial.println("Initializing Camera");
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 17;
  config.pin_d1 = 35;
  config.pin_d2 = 34;
  config.pin_d3 = 5;
  config.pin_d4 = 39;
  config.pin_d5 = 18;
  config.pin_d6 = 36;
  config.pin_d7 = 19;
  config.pin_xclk = 27;
  config.pin_pclk = 21;
  config.pin_vsync = 22;
  config.pin_href = 26;
  config.pin_sscb_sda = 25;
  config.pin_sscb_scl = 23;
  config.pin_pwdn = -1;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    config.frame_size = FRAMESIZE_XGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_QVGA;//FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    Serial.println("Trying again");
    return;
  }
  
  sensor_t * s = esp_camera_sensor_get();
  //s->set_hmirror(s, 1);
  
  Serial.println("Finish Initializing Camera");
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  InitializeBluetooth();
  InitializeCamera();
  
  /*
  char bytesToRead = 100;
  char* arr[1] = {&bytesToRead};
  TestBluetooth(arr);
  */
}

void GetTime(char** argv) {
  int64_t t = esp_timer_get_time();
  SerialBT.write((uint8_t*)(&t), 8);
  Serial.printf("Time was: %d\n", esp_timer_get_time());
}

void take_picture(char** argv) {
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.printf("Camera Capture Failed");
    ESP_LOGE(TAG, "Camera Capture Failed");
  }
  SerialBT.write((uint8_t*)(&fb->len), 4);
  size_t bytes_wrote = SerialBT.write(fb->buf, fb->len);
  Serial.printf("Needed to write %d bytes to bluetooth serial\n", fb->len);
  Serial.printf("Wrote %d bytes to bluetooth serial\n", bytes_wrote);
  esp_camera_fb_return(fb);
}

void TestBluetooth(char** argv) {
  uint8_t* rand_pointer;
  Serial.printf("Writing %d bytes to bluetooth", *argv[0]);
  SerialBT.write(rand_pointer, *argv[0]);
}

void ParseArguementFlags(){
  
}

void UpdateConfigs(char** argv) {
  int val = analogRead(0) / 4;
  EEPROM.write(address, val);
  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }
}

uint8_t* GetBatteryLevel() {

}

void loop() {
  
  if (SerialBT.available()) {
    Serial.println("\nCharacter available to read");
    std::list<int> l;
    l.push_back(SerialBT.read());
    Serial.printf("Command: ");
    while (l.back() != 0) {
      l.push_back(SerialBT.read());
      Serial.printf("%c", l.back());
    }

    char rxValueCStr[l.size()];
    std::copy(l.begin(), l.end(), rxValueCStr);
    const char* argcStr = strtok (rxValueCStr, " ");
    int argc = *argcStr;
    Serial.printf("\nNumber of arguements: %d\n", argc);

    char* argv[argc];
    for (int i = 0; i < argc; i += 1)
    {
      argv[i] = strtok (NULL, " ");
      Serial.printf("Arguement at %d is: %s\n", i, argv[i]);
    }
    //call api
    if (argv[0] == "GetTime")
    {
      Serial.printf("Getting Time\n");
      GetTime(argv);
      return;
    }
    if (!strcmp(argv[0], "TakePicture"))
    {
      Serial.printf("Taking picture\n");
      TakePicture(argv);
      return;
    }
    if (argv[0] == "UpdateConfigs")
    {
      UpdateConfigs(argv);
    }
    if (argv[0] == "GetBatteryLevel")
    {
      GetBatteryLevel();
    }
    Serial.printf("Command not recognized: %s", argv[0]);
  }
  delay(20);
}
