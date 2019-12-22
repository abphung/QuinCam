#include "BluetoothSerial.h"
#include <esp_camera.h>
#include <EEPROM.h>
#include <list>
#include <map> 

// Select camera model
//#define CAMERA_MODEL_WROVER_KIT
//#define CAMERA_MODEL_ESP_EYE
//#define CAMERA_MODEL_M5STACK_PSRAM
#define CAMERA_MODEL_M5STACK_WIDE//this works, but psram not found :(
//#define CAMERA_MODEL_AI_THINKER
//#define MY_CAMERA

#include "camera_pins.h"

/** the current address in the EEPROM (i.e. which byte we're going to write to next) **/
int address = 0;
BluetoothSerial SerialBT;

//////////////////
//Setup functions
//////////////////
void InitializeBluetooth() {
  Serial.println("Initializing Bluetooth");
  SerialBT.begin("QuinCam");
  Serial.println("Finished Initializing Bluetooth");
}

void InitializeCamera() {
  Serial.println("Initializing Camera");
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  /*
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
  */
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  //init with high specs to pre-allocate larger buffers
  if (psramFound()) {
    Serial.println("psram found");
    config.frame_size = FRAMESIZE_XGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    Serial.println("psram not found");
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

////////////////
//Loop functions
////////////////
void get_sensor_id(){
  sensor_t* sensor = esp_camera_sensor_get();
  sensor_id_t* id = &sensor->id;
  SerialBT.write(&id->MIDH, 1);
  SerialBT.write(&id->MIDL, 1);
  SerialBT.write(&id->PID, 1);
  SerialBT.write(&id->VER, 1);
}

void get_slv_addr(){
  //can sensor be set as global?
  sensor_t* sensor = esp_camera_sensor_get();
  SerialBT.write(&sensor->slv_addr, 1);
}

void get_pixformat(){
  sensor_t* sensor = esp_camera_sensor_get();
  static const char* pixformat_enum_strings[] = {
    "2BPP/RGB565",
    "2BPP/YUV422",
    "1BPP/GRAYSCALE",
    "JPEG/COMPRESSED",
    "3BPP/RGB888",
    "RAW",
    "3BP2P/RGB444",
    "3BP2P/RGB555",
  };
  const char* pixformat_string = pixformat_enum_strings[sensor->pixformat];
  SerialBT.write((uint8_t*)pixformat_string, strlen(pixformat_string));
}

void get_framesize(){
  sensor_t* sensor = esp_camera_sensor_get();
  static const char* framesize_enum_strings[] = {
    "160x120",
    "128x160",
    "176x144",
    "240x176",
    "320x240",
    "400x296",
    "640x480",
    "800x600",
    "1024x768",
    "1280x1024",
    "1600x1200",
    "2048*1536",
  };
  camera_status_t* status = &sensor->status;
  const char* framesize_string = framesize_enum_strings[status->framesize];
  SerialBT.write((uint8_t*)framesize_string, strlen(framesize_string));
}

void get_camera_status(){
  sensor_t* sensor = esp_camera_sensor_get();
  camera_status_t* status = &sensor->status;
  //if everything is stored next to each other can I just write SerialBT.write(&status->quality, 25)?
  SerialBT.write(&status->quality, 1);
  SerialBT.write((uint8_t*)&status->brightness, 1);
  SerialBT.write((uint8_t*)&status->contrast, 1);
  SerialBT.write((uint8_t*)&status->saturation, 1);
  //SerialBT.write((uint8_t*)&status->sharpness, 1);
  //SerialBT.write(&status->denoise, 1);
  SerialBT.write(&status->special_effect, 1);
  SerialBT.write(&status->wb_mode, 1);
  SerialBT.write(&status->awb, 1);
  SerialBT.write(&status->awb_gain, 1);
  SerialBT.write(&status->aec, 1);
  SerialBT.write(&status->aec2, 1);
  SerialBT.write((uint8_t*)&status->ae_level, 1);
  SerialBT.write((uint8_t*)&status->aec_value, 2);
  SerialBT.write(&status->agc, 1);
  SerialBT.write(&status->agc_gain, 1);
  SerialBT.write(&status->gainceiling, 1);
  SerialBT.write(&status->bpc, 1);
  SerialBT.write(&status->wpc, 1);
  SerialBT.write(&status->raw_gma, 1);
  SerialBT.write(&status->lenc, 1);
  SerialBT.write(&status->hmirror, 1);
}

void take_picture() {
  //can fb be set as global?
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.printf("Camera Capture Failed");
    ESP_LOGE(TAG, "Camera Capture Failed");
  }
  size_t bytes_wrote;
  bytes_wrote = SerialBT.write((uint8_t*)(&fb->len), 4);
  bytes_wrote = SerialBT.write(fb->buf, fb->len);
  Serial.printf("Needed to write %d bytes to bluetooth serial\n", fb->len);
  Serial.printf("Wrote %d bytes to bluetooth serial\n", bytes_wrote);
  esp_camera_fb_return(fb);
}

void TestBluetooth(char** argv) {
  uint8_t* rand_pointer;
  Serial.printf("Writing %d bytes to bluetooth", *argv[0]);
  SerialBT.write(rand_pointer, *argv[0]);
}

void UpdateConfigs(char** argv) {
  int val = analogRead(0) / 4;
  EEPROM.write(address, val);
  address = address + 1;
  if (address == EEPROM.length()) {
    address = 0;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println();
  InitializeBluetooth();
  InitializeCamera();
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

    sensor_t* sensor = esp_camera_sensor_get();
    std::map<std::string, int (*)(_sensor*, int)> string_function_map; 
    /*
      {"init_status", sensor->init_status},
      {"reset", sensor->reset},
      {"set_pixformat", sensor->set_pixformat},
      {"set_framesize", sensor->set_framesize},
      {"set_contrast", sensor->set_contrast},
      {"set_brightness", sensor->set_brightness},
      {"set_saturation", sensor->set_saturation},
      //{"set_sharpness", sensor->set_sharpness}
    };*/
    
    if (!strcmp(argv[0], "init_status")){
      sensor->init_status(sensor);
    }
    else if (!strcmp(argv[0], "reset")){
      sensor->reset(sensor);
    }
    else if (!strcmp(argv[0], "set_pixformat")){
      //(int)argv[1]
      sensor->set_pixformat(sensor, static_cast<pixformat_t>(atoi(argv[1])));
    }
    else if (!strcmp(argv[0], "set_framesize")){
      Serial.printf("Setting new framesize");
      sensor->set_framesize(sensor, static_cast<framesize_t>(atoi(argv[1])));
      //get_framesize()
    }
    else if (!strcmp(argv[0], "set_gainceiling")){
      sensor->set_gainceiling(sensor, static_cast<gainceiling_t>(atoi(argv[1])));
    }
    else if (!strcmp(argv[0], "take_picture"))
    {
      take_picture();
    }
    else{
      string_function_map[argv[0]](sensor, (int)argv[1]);
    }
  }
  delay(20);
}
