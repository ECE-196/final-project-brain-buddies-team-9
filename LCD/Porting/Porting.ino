/**
 * # SquareLine Porting Example
 *
 * The example demonstrates how to port SquareLine (v1.3.x) project. And for RGB LCD, it can enable the avoid tearing function.
 *
 * ## How to Use
 *
 * To use this example, please firstly install the following dependent libraries:
 *
 * - lvgl (>= v8.3.9, < v9)
 *
 * Follow the steps below to configure:
 *
 * 1. For **ESP32_Display_Panel**:
 *
 *     - Follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-drivers) to configure drivers if needed.
 *     - If using a supported development board, follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#using-supported-development-boards) to configure it.
 *     - If using a custom board, follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#using-custom-development-boards) to configure it.
 *
 * 2. For **lvgl**:
 *
 *     - Follow the [steps](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-lvgl) to add *lv_conf.h* file and change the configurations.
 *     - Modify the macros in the [lvgl_port_v8.h](./lvgl_port_v8.h) file to configure the LVGL porting parameters.
 *
 * 3. Navigate to the `Tools` menu in the Arduino IDE to choose a ESP board and configure its parameters. For supported
 *    boards, please refter to [Configuring Supported Development Boards](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/How_To_Use.md#configuring-supported-development-boards)
 * 4. Verify and upload the example to your ESP board.
 *
 * ## Serial Output
 *
 * ```bash
 * ...
 * Squareline porting example start
 * Initialize panel device
 * Initialize LVGL
 * Create UI
 * Squareline porting example end
 * IDLE loop
 * IDLE loop
 * IDLE loop
 * ...
 * ```
 *
 * ## Troubleshooting
 *
 * Please first check [FAQ](https://github.com/esp-arduino-libs/ESP32_Display_Panel/blob/master/docs/FAQ.md) for troubleshooting. If you still cannot solve the problem, please create a [Github issue](https://github.com/esp-arduino-libs/ESP32_Display_Panel/issues). We will get back to you as soon as possible.
 *
 */


/*  Pin numbers for buttons
  dpad:
    UP: pin16
    DOWN: pin15
    RIGHT: pin14
    LEFT: pin17

  TOP: pin1
  SELECT: pin2
  BOTTOM: pin3
*/

#include <Arduino.h>
#include <ESP_Panel_Library.h>
#include <lvgl.h>
#include "screens/ui_Focus_Page.c"
#include "screens/ui_Task_Page.c"
#include "screens/ui_Home_Page.c"
#include "images/ui_img_grey_cat_png.c"
#include "images/ui_img_raconnn_png.c"

#include <ui.h>
#include "lvgl_port_v8.h"

// SimpleTime Example
#include <WiFi.h>
#include "time.h"
#include "esp_sntp.h"

const char *ssid = "VWLife";
const char *password = "Wolfman1!";

const char *ntpServer1 = "pool.ntp.org";
const char *ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;
const char *time_zone = "PST8PDT,M3.2.0,M11.1.0";  // TimeZone rule for Europe/Rome including daylight adjustment rules (optional)

char currentTimeString[9] = {};
const int asciiOffset = 48;
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }


  if(9 < timeinfo.tm_hour && 13 > timeinfo.tm_hour || 21 < timeinfo.tm_hour && 24 > timeinfo.tm_hour){
    currentTimeString[0] = '1';
  }
  else {
    currentTimeString[0] = ' ';
  }
  if(13 > timeinfo.tm_hour){
      currentTimeString[1] = (timeinfo.tm_hour % 10) + asciiOffset;
  }
  else{
      currentTimeString[1] = ((timeinfo.tm_hour - 12) % 10) + asciiOffset;
  }
  currentTimeString[2] = ':';
  currentTimeString[3] = (timeinfo.tm_min / 10) + asciiOffset;
  currentTimeString[4] = (timeinfo.tm_min % 10) + asciiOffset;
  currentTimeString[5] = ' ';
  if(12 < timeinfo.tm_hour){
    currentTimeString[6] = 'P';
    currentTimeString[7] = 'M';
  }
  else{
    currentTimeString[6] = 'A';
    currentTimeString[7] = 'M';
  }
  currentTimeString[8] = '\0';
  lv_label_set_text(ui_clock, currentTimeString);
}// ascii: 0 = 48, 9 = 57;

/*
struct tm
{
  int	tm_sec;
  int	tm_min;
  int	tm_hour;
  int	tm_mday;
  int	tm_mon;
  int	tm_year;
  int	tm_wday;
  int	tm_yday;
  int	tm_isdst;
*/

// Callback function (gets called when time adjusts via NTP)
void timeavailable(struct timeval *t) {
  Serial.println("Got time adjustment from NTP!");
  printLocalTime();
  WiFi.disconnect();
}
// END: SimpleTime Example

//Declare custom functions
void switchScreen();
void changeUpState();
void changeDownState();
void changeRightState();
void changeLeftState();

void updateTasksTextFromHome();
void bottomFromContainer();
void selectfromContainer();
void topfromContainer();

// struct for task
struct task{
  uint64_t time24hr;   //military time with miliseconds
	char time[10];
	char name[100];
	int skippedAmount;
	int completedAmount;
};

uint64_t init_time; // here accessable everywhere, const since only set once.
uint64_t currentMicros;
uint64_t previousMicros;

uint64_t million = 1*1000*1000;
//set up struct array
task taskListTwo[10]= {
  {10 * million, "8:00am", "Wake up", 0, 0},
  {20* million,"9:00am", "Eat breakfast", 0, 0},
  {3*10* million,"10:00am", "Go to school", 0, 0},
  {4*10* million,"11:00am", "Attend classes", 0, 0},
  {4*60* million,"3:00pm", "Eat lunch", 0, 0},
  {5*60* million,"5:00pm", "Leave school", 0, 0},
  {6*60* million,"6:00pm", "Go to gym", 0, 0},
  {7*60* million,"7:00pm", "Eat dinner", 0, 0},
  {8*60* million,"8:00pm", "Shower", 0, 0},
  {9*60* million,"10:00pm", "Go to sleep", 0, 0},
};

int taskCounter2 = 0;
int taskListLength2 = 10;

//Declarations for button up
char taskList[10][10] = {"8am", "9am", "10am", "11am", "12pm", "1pm", "2pm", "3pm", "4pm", "5pm"};
int taskListLength = 10;
int taskCounter = 9;

//set up pins for buttons 
//      UP Button
const byte interruptUpPin = 16;
volatile byte upPinState = LOW;
volatile byte upState = LOW;

//      DOWN Button
const byte interruptDownPin = 15;
volatile byte downPinState = LOW;
volatile byte downState = LOW;

//      RIGHT Button
const byte interruptRightPin = 14;
volatile byte rightPinState = LOW;
volatile byte rightState = LOW;

//      LEFT Button
const byte interruptLeftPin = 17;
volatile byte leftPinState = LOW;
volatile byte leftState = LOW;

//      TOP Button
const byte interruptTopPin = 1;
volatile byte topPinState = LOW;
volatile byte topState = LOW;

//      Select Button
const byte interruptSelectPin = 2;
volatile byte selectPinState = LOW;
volatile byte selectState = LOW;

//      Bottom Button
const byte interruptBottomPin = 3;
volatile byte bottomPinState = LOW;
volatile byte bottomState = LOW;
volatile byte containerVisible = false;

// get root widget
lv_obj_t* root_widget = lv_obj_get_parent(lv_scr_act());

void setup(){
  Serial.begin(115200);
// SimpleTime begin
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  esp_sntp_servermode_dhcp(1);
  int WifiAttemptCount = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");

  // set notification call-back function
  sntp_set_time_sync_notification_cb(timeavailable);
  // #define TZ_America_Los_Angeles	PSTR("PST8PDT,M3.2.0,M11.1.0")
  configTzTime(PSTR("PST8PDT,M3.2.0,M11.1.0"), ntpServer1, ntpServer2);    
// Simple time end    
  

    Serial.println("Squareline porting example start");

    // Configure dpad buttons
    pinMode(16, INPUT_PULLDOWN);  // Configure PCB buttons, using PULLDOWN since button volt diff is initially 0 so need to pull to ground
    pinMode(17, INPUT_PULLDOWN); 
    pinMode(15, INPUT_PULLDOWN); 
    pinMode(14, INPUT_PULLDOWN); 

    // Configure decision buttons
    pinMode(1, INPUT_PULLDOWN);
    pinMode(2, INPUT_PULLDOWN);
    pinMode(3, INPUT_PULLDOWN);

    // Interupts for Dpad
    attachInterrupt(digitalPinToInterrupt(interruptUpPin), changeUpState, RISING); 
    attachInterrupt(digitalPinToInterrupt(interruptDownPin), changeDownState, RISING); 
    attachInterrupt(digitalPinToInterrupt(interruptRightPin), changeRightState, RISING); 
    attachInterrupt(digitalPinToInterrupt(interruptLeftPin), changeLeftState, RISING); 

    // Interupts for Decision
    attachInterrupt(digitalPinToInterrupt(interruptTopPin), changeTopState , RISING); 
    attachInterrupt(digitalPinToInterrupt(interruptSelectPin), changeSelectState, RISING); 
    attachInterrupt(digitalPinToInterrupt(interruptBottomPin), changeBottomState, RISING); 

    Serial.println("Initialize panel device");
    ESP_Panel *panel = new ESP_Panel();
    panel->init();

#if LVGL_PORT_AVOID_TEAR
    // When avoid tearing function is enabled, configure the RGB bus according to the LVGL configuration
    ESP_PanelBus_RGB *rgb_bus = static_cast<ESP_PanelBus_RGB *>(panel->getLcd()->getBus());
    rgb_bus->configRgbFrameBufferNumber(LVGL_PORT_DISP_BUFFER_NUM);
    rgb_bus->configRgbBounceBufferSize(LVGL_PORT_RGB_BOUNCE_BUFFER_SIZE);
#endif

    panel->begin();

    // Initialize LVGL
    Serial.println("Initialize LVGL");
    lvgl_port_init(panel->getLcd(), panel->getTouch());

    // Initialize the UI created by SquareLine Studio
    Serial.println("Create UI");

    /* Lock the mutex due to the LVGL APIs are not thread-safe */
    lvgl_port_lock(-1);

    /* Initialize LVGL UI generated by Squareline */
    ui_init();
    lv_disp_t *display1 = lv_disp_get_default();
    lv_disp_set_rotation(display1, LV_DISP_ROT_90);

    /* Release the mutex */
    lvgl_port_unlock();

    Serial.println("Squareline porting example end");
    init_time = esp_timer_get_time();


  //   static lv_img_dsc_t my_racon_dsc = {
  //     .header.always_zero = 0;
  //     .header.w
  //   }

  //   LV_IMG_DECLARE(my_racon_dsc);
  //   lv_obj_t * icon = lv_img_create(lv_scr_act(),NULL)
  //   lv_img_set_src(icon, "S:racon.c");
  }

void loop(){

  previousMicros = currentMicros;
  currentMicros = esp_timer_get_time();   
  lv_obj_t* current_screen = lv_scr_act();

  //      set up for UP Button
  upPinState = digitalRead(interruptUpPin);
  if(HIGH == upState && LOW == upPinState){
    upFromTask();
    upState = LOW;
  }
  
  //     set up for Down Button
  downPinState = digitalRead(interruptDownPin);
  if(HIGH == downState && LOW == downPinState){
    downFromTask();
    downState = LOW;
  }

  //     set up for Right Button
  rightPinState = digitalRead(interruptRightPin);
  if(HIGH == rightState && LOW == rightPinState){
    if(current_screen == ui_Home_Page){
      taskFromHome();
    }
    if(current_screen == ui_Focus_Page){
      homeFromFocus();
    }
    rightState = LOW;
  }

  //      set up for LEFT Button
  leftPinState = digitalRead(interruptLeftPin);
  if(HIGH == leftState && LOW == leftPinState){
    if(current_screen == ui_Home_Page){
      focusFromHome();
    }
    if(current_screen == ui_Task_Page){
      homeFromTask();
    }
    leftState = LOW;
  }

  //      set up for SELECT Button when in HOMEPAGE
  selectPinState = digitalRead(interruptSelectPin);

  //      set up for TOP & BOTTOM Button when in POPUP
  topPinState = digitalRead(interruptTopPin);
  bottomPinState = digitalRead(interruptBottomPin);

  if(HIGH == selectState && LOW == selectPinState && current_screen == ui_Home_Page && containerVisible == false){
    selectFromHome();
    selectState = LOW;
  }
  if(HIGH == bottomState && LOW == bottomPinState && current_screen == ui_Home_Page && containerVisible == true){
    bottomFromContainer();
    bottomState = LOW;
  }
  else if(HIGH == selectState && LOW == selectPinState && current_screen == ui_Home_Page && containerVisible == true){
    selectfromContainer();
    selectState = LOW;
  }
  else if(HIGH == topState && LOW == topPinState && current_screen == ui_Home_Page && containerVisible == true){
    topfromContainer();
    topState = LOW;
  }


  // Havng a notification POPUP
  //if (task_time_in_microseconds + init_time < currentMicrosec){
	//	do the popup/notif
  //time24hr

  if(taskListTwo[taskCounter2].time24hr + init_time < currentMicros){
    lv_obj_clear_flag(ui_Container1, LV_OBJ_FLAG_HIDDEN);
    containerVisible = true;
  }
  /*
There are some attributes which can be enabled/disabled by lv_obj_add/clear_flag(obj, LV_OBJ_FLAG_...):

LV_OBJ_FLAG_HIDDEN Make the object hidden. (Like it wasn't there at all)
  */

  // Handle LVGL tasks
  lv_task_handler();
  delay(10);
}


                //      SET UP STATES OF BUTTONS
//   Dpad 
void changeUpState(){
  //  update UP button
  upState = HIGH;
}
void changeDownState(){
  //  update DOWN button
  downState = HIGH;
}
void changeRightState(){
  //  update right button
  rightState = HIGH;
}
void changeLeftState(){
  //  update left button
  leftState = HIGH;
}

//   Decision
void changeTopState(){
  //  update TOP button
  topState = HIGH;
}
void changeSelectState(){
  //  update SELECT button
  selectState = HIGH;
}

void changeBottomState(){
  //  update bottom button
  bottomState = HIGH;
}


              //      CUSTOM FUNCTIONS FOR PAGE SWITCHING

//    current is Focus page:
void homeFromFocus(){   //  press RIGHT Button
  //current is focus page:
  // go to home
  lv_event_send(ui_homeFromFocus, LV_EVENT_CLICKED,NULL);
}

//    current is home page:
void taskFromHome(){    //  press RIGHT Button
  // go to task page
  lv_event_send(ui_taskFromHome, LV_EVENT_CLICKED, NULL);
}
void focusFromHome(){   //  press LEFT Button
  // go to focus page
  lv_event_send(ui_focusFromHome, LV_EVENT_CLICKED, NULL);
}

//    current is Task page:
void homeFromTask(){    //  press LEFT Button
  // go to home page
  lv_event_send(ui_homeFromTask, LV_EVENT_CLICKED, NULL );
}


                      //      CUSTOM FUNCTIONS FOR NAVEGATING IN PAGES 

//      Using UP button on TASK page
void upFromTask(){
  taskCounter--;
  if(taskCounter == -1){
    taskCounter = 9;
  }
  //    Special Cases
  if(taskCounter == 0){
    updateTaskText(taskList[taskListLength - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  }
  else if(taskCounter == 9){
    updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[0]);
  }
  else{
    updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  }
}

//    Using DOWN bottom on TASK page
void downFromTask(){
  taskCounter++;
  if(taskCounter == 10){
    taskCounter = 0;
  }
  //    Special Cases
  if(taskCounter == 0){
    updateTaskText(taskList[taskListLength - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  }
  else if(taskCounter == 9){
    updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[0]);
  }
  else{
    updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  }
}

//    Function to assign tasks when using up or down buttons
void updateTaskText(char* top, char* center, char* bottom){
  lv_label_set_text(ui_topTask, top);
  lv_label_set_text(ui_centerTask, center);
  lv_label_set_text(ui_bottomTask, bottom);
}


//      CUSTOM FUNCTIONS FOR NAVEGATING IN HOME PAGE
void selectFromHome(){
  lv_event_send(ui_currentTask, LV_EVENT_CLICKED, NULL);
  containerVisible = true;
}

void bottomFromContainer(){
  lv_event_send(ui_bottomButton, LV_EVENT_CLICKED, NULL);
  containerVisible = false;
}

void selectfromContainer(){
  lv_event_send(ui_selectButton, LV_EVENT_CLICKED, NULL);
  updateTasksTextFromHome("DONE");
  containerVisible = false;
}

void topfromContainer(){
  lv_event_send(ui_topButton, LV_EVENT_CLICKED, NULL);
  updateTasksTextFromHome("SKIP");
  containerVisible = false;
}

//    Function to assign tasks when using decision buttons
void updateTasksTextFromHome(const char* action) {

    if (strcmp(action, "SKIP") == 0) {
        taskListTwo[taskCounter2].skippedAmount ++;
        taskCounter2++;
        if(taskCounter2 == 8) { 
            taskCounter2 = 0; 
        }
        lv_label_set_text(ui_currentTaskLable, taskListTwo[taskCounter2].name);
        lv_label_set_text(ui_nextTasklabel, taskListTwo[taskCounter2 + 1].name);
    } 

    else if (strcmp(action, "DONE") == 0) {
        taskListTwo[taskCounter2].completedAmount ++;
        taskCounter2++;
        if (taskCounter2 == 8) { 
            taskCounter2 = 0; 
        }
        lv_label_set_text(ui_currentTaskLable, taskListTwo[taskCounter2].name);
        lv_label_set_text(ui_nextTasklabel, taskListTwo[taskCounter2 + 1].name);
    } 
}

