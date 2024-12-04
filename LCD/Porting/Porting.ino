/*
FOR PORTING INSTRUCTIONS SEE EXAMPLE IN: File > Examples > ESP32_Display_Panel > SquareLine > v8 > Porting
 */

/*  Pin numbers for buttons
  dpad:
    UP: pin16
    DOWN: pin15
    RIGHT: pin14
    LEFT: pin17

  selection:
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

#include <ui.h>
#include "lvgl_port_v8.h"

#include "pinDefs.h"
#include "wifiTimeDefs.h"

// struct for task
struct task{
  uint64_t time24hr;   //military time with miliseconds
	char time[11];
	char name[100];
	int skippedAmount;
	int completedAmount;
};

struct taskStructure{
  task* prev;
  task* current;
  task* next;
};

const size_t taskIncrement = sizeof(task);

//Declare custom functions
void switchScreen();
void changeUpState();
void changeDownState();
void changeRightState();
void changeLeftState();
void allButtonLow();

void selectFromInfo(taskStructure& aTaskStruct);
void sBackfromContainer();

void updateTasksTextFromHome();
void bottomFromContainer(taskStructure& aTaskStruct);
void selectfromContainer(taskStructure& aTaskStruct);
void topfromContainer(taskStructure& aTaskStruct);

void initializeTaskStructure(taskStructure& aTaskStruct);

void updateFocusTasks(taskStructure& aTaskStruct);
void updateHomeTasks(taskStructure& aTaskStruct);
void updateTaskPageTasks(taskStructure& aTaskStruct);

void prevTask(taskStructure& aTaskStruct);
void nextTask(taskStructure& aTaskStruct);

void updateTaskInPopUp(task* t);
void updateTimeInPopUp(task* t);

enum action { SKIP, DONE, BACK };

uint64_t init_time; // here accessable everywhere, const since only set once.
uint64_t currentMicros;
uint64_t previousMicros;
uint64_t lastClockUpdateMicros;

volatile byte containerVisible = false;
volatile byte infoVisible = false;

uint64_t seconds = 1*1000*1000; // 23 * seconds -> 23 seconds in microseconds
uint64_t minutes = 60*seconds;  // 45 * minutes -> 45 minutes in microseconds
uint64_t hours   = 60*minutes;  // 3  * hours   -> 3  hours   in microseconds
const int taskListLength = 10;
//set up struct array
task taskListTwo[taskListLength]= { // these must be in numerical order for 24hrTime
  // {800 ,    "8:00AM",   "Wake up", 0, 0},
  // {900 ,    "9:00AM",   "Eat breakfast", 0, 0},
  // {1000,    "10:00AM",  "Go to school", 0, 0},
  // {1100,    "11:00AM",  "Attend classes", 0, 0},
  // {1500,    "3:00PM",   "Eat lunch", 0, 0},
  {1700,    "5:00 PM -",   "Leave school", 0, 0},
  {1800,    "6:00 PM -",   "Go to gym", 0, 0},
  {1855,    "6:55 PM -",  "Become Enlightened :)", 0, 0},
  {1858,    "6:58 PM -",  "level up code :)", 0, 0},
  {1900,    "7:00 PM -",   "Eat dinner", 0, 0},
  {2000,    "8:00 PM -",   "Shower", 0, 0},
  {2200,   "10:00 PM -",  "Go to sleep", 0, 0}
};

task demoTaskList[3] = {
  {1600, "4:00PM", "DEMO DAY", 0, 0},
  {1603, "4:03PM", "Present", 0,0},
  {1608, "4:08PM", "5min Passed",0,0}
};

// initialize a taskStructure with data {current task, prev task, next task} all poitners to the task array
taskStructure taskStructHomeAndFocus = { //
  &taskListTwo[taskListLength - 1],
  &taskListTwo[0],
  &taskListTwo[1]
};
taskStructure taskStructTask = { //S_taskPage
  &taskListTwo[taskListLength - 1],
  &taskListTwo[0],
  &taskListTwo[1]
};
task *taskListBegin = &taskListTwo[0];
task *taskListEnd = &taskListTwo[taskListLength - 1];

// get root widget
lv_obj_t* root_widget = lv_obj_get_parent(lv_scr_act());

void setup(){
  Serial.begin(115200);
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

  setupClockTime(ssid, password);
  lastClockUpdateMicros = esp_timer_get_time();
  //Serial.println("Squareline porting example start");
  //Serial.println("Initialize panel device");
  ESP_Panel *panel = new ESP_Panel();
  panel->init();
  panel->begin();
  // Initialize LVGL
  //Serial.println("Initialize LVGL");
  lvgl_port_init(panel->getLcd(), panel->getTouch());
  // Initialize the UI created by SquareLine Studio
  // Serial.println("Create UI");
  /* Lock the mutex due to the LVGL APIs are not thread-safe */
  lvgl_port_lock(-1);
  /* Initialize LVGL UI generated by Squareline */
  ui_init();
  lv_disp_t *display1 = lv_disp_get_default();
  lv_disp_set_rotation(display1, LV_DISP_ROT_90);
  /* Release the mutex */
  lvgl_port_unlock();
  //Serial.println("Squareline porting example end");
  init_time = esp_timer_get_time();

  while(!current24hrTime){
    Serial.println("time not ready! the current time is:");
    Serial.println(current24hrTime);
    delay(1);
  };
  initializeTaskStructure(taskStructHomeAndFocus);
  initializeTaskStructure(taskStructTask);

  // update popUp
  updateTimeInPopUp(taskStructHomeAndFocus.current);
  updateTaskInPopUp(taskStructHomeAndFocus.current);

  Serial.println("end setup");
  delay (2000);
}

void loop(){

  previousMicros = currentMicros;
  currentMicros = esp_timer_get_time();
  if(lastClockUpdateMicros + 1 * minutes < currentMicros){
    lastClockUpdateMicros = currentMicros;
    printLocalTime();
  }
  lv_obj_t* current_screen = lv_scr_act();

  //      setup for UP Button
  upPinState = digitalRead(interruptUpPin);
  if(HIGH == upState && LOW == upPinState){
    allButtonLow();
    upFromTask(taskStructTask);
  }

  //     setup for Down Button
  downPinState = digitalRead(interruptDownPin);
  if(HIGH == downState && LOW == downPinState){
    allButtonLow();
    downFromTask(taskStructTask);
  }

  //     setup for Right Button
  rightPinState = digitalRead(interruptRightPin);
  if(HIGH == rightState && LOW == rightPinState){
    allButtonLow();
    if(current_screen == ui_Home_Page){
      taskFromHome();
    }
    if(current_screen == ui_Focus_Page){
      homeFromFocus();
    }
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
    allButtonLow();
  }

  //      set up for SELECT Button when in HOMEPAGE
  selectPinState = digitalRead(interruptSelectPin);

  //      set up for TOP & BOTTOM Button when in POPUP
  topPinState = digitalRead(interruptTopPin);
  bottomPinState = digitalRead(interruptBottomPin);


////// Select from task page & info container
  if(HIGH == selectState && LOW == selectPinState && current_screen == ui_Task_Page && infoVisible == false){
    selectFromInfo(taskStructTask);
    allButtonLow();
  }
  if(HIGH == bottomState && LOW == bottomPinState && current_screen == ui_Task_Page && infoVisible == true){
    sBackfromContainer();
    allButtonLow();
  }

////// Bottons in HOME PAGE & container
  if(HIGH == selectState && LOW == selectPinState && current_screen == ui_Home_Page && containerVisible == false){
    Serial.println("selectFromHome");
    selectFromHome();
    allButtonLow();
    // create a function that when called it sets all states to low.
  }
  if(HIGH == bottomState && LOW == bottomPinState && current_screen == ui_Home_Page && containerVisible == true){
    Serial.println("bottomFromContainer");
    selectfromContainer(taskStructHomeAndFocus);
    allButtonLow();
  }
  else if(HIGH == selectState && LOW == selectPinState && current_screen == ui_Home_Page && containerVisible == true){
    Serial.println("selectFromContainer");
    bottomFromContainer();
    allButtonLow();
  }
  else if(HIGH == topState && LOW == topPinState && current_screen == ui_Home_Page && containerVisible == true){
    Serial.println("topFromContainer");
    topfromContainer(taskStructHomeAndFocus);
    allButtonLow();
  }

  if(taskStructHomeAndFocus.next->time24hr <= current24hrTime){
    Serial.println("next24hour if statement activated");
    nextTask(taskStructHomeAndFocus);
    updateFocusTasks(taskStructHomeAndFocus);
    updateHomeTasks(taskStructHomeAndFocus);
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


 //////////////////////  SET UP STATES OF BUTTONS //////////////////////

// set all button states to low
void allButtonLow(){
  upState = LOW;
  downState = LOW;
  rightState = LOW;
  leftState = LOW;
  topState = LOW;
  selectState = LOW;
  bottomState = LOW;
}
//  Dpad
//  update UP button
void changeUpState(){upState = HIGH;}
//  update DOWN button
void changeDownState(){downState = HIGH;}
//  update right button
void changeRightState(){rightState = HIGH;}
//  update left button
void changeLeftState(){leftState = HIGH;}

//  Decision
//  update TOP button
void changeTopState(){topState = HIGH;}
//  update SELECT button
void changeSelectState(){selectState = HIGH;}
//  update bottom button
void changeBottomState(){bottomState = HIGH;}


//  CUSTOM FUNCTIONS FOR PAGE SWITCHING
//  current is Focus page:
//  press RIGHT Button
void homeFromFocus(){lv_event_send(ui_homeFromFocus, LV_EVENT_CLICKED,NULL);}   // go to home
//  current is Home page:
//  press RIGHT Button
void taskFromHome(){lv_event_send(ui_taskFromHome, LV_EVENT_CLICKED, NULL);}    // go to task page
//  press LEFT Button
void focusFromHome(){lv_event_send(ui_focusFromHome, LV_EVENT_CLICKED, NULL);}  // go to focus page
//  current is Task page:
//  press LEFT Button
void homeFromTask(){lv_event_send(ui_homeFromTask, LV_EVENT_CLICKED, NULL );}   // go to home page


// CUSTOM FUNCTIONS FOR NAVEGATING IN PAGES
//Using UP button on TASK page
void upFromTask(taskStructure& aTaskStruct){
  prevTask(aTaskStruct);
  updateTaskPageTasks(aTaskStruct);
}

//    Using DOWN bottom on TASK page
void downFromTask(taskStructure& aTaskStruct){
  nextTask(aTaskStruct);
  updateTaskPageTasks(aTaskStruct);
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

void selectfromContainer(taskStructure& aTaskStruct){
  lv_event_send(ui_selectButton, LV_EVENT_CLICKED, NULL);
  updateTasksTextFromHome(DONE, aTaskStruct);
   // update popUp
  updateTimeInPopUp(aTaskStruct.current);
  updateTaskInPopUp(aTaskStruct.current);
  containerVisible = false;
}



void topfromContainer(taskStructure& aTaskStruct){
  lv_event_send(ui_topButton, LV_EVENT_CLICKED, NULL);
  updateTasksTextFromHome(SKIP, aTaskStruct);
   // update popUp
  updateTimeInPopUp(aTaskStruct.current);
  updateTaskInPopUp(aTaskStruct.current);
  containerVisible = false;
}


//    Function to assign tasks when using decision buttons
////      CUSTOM FUNCTIONS FOR NAVEGATING IN TASK PAGE
void selectFromInfo(taskStructure& aTaskStruct){
  lv_event_send(ui_curseBox, LV_EVENT_CLICKED, NULL);
  infoVisible = true;
  lv_label_set_text_fmt(ui_selectedTask1, "%s", aTaskStruct.current->name);
  lv_label_set_text_fmt(ui_timeOfTask, "%s", aTaskStruct.current->time);
  lv_label_set_text_fmt(ui_skippedData, "%d", aTaskStruct.current->skippedAmount);
  lv_label_set_text_fmt(ui_completedData, "%d", aTaskStruct.current->completedAmount);
}
void sBackfromContainer(){
  lv_event_send(ui_closeTaskInfo, LV_EVENT_CLICKED, NULL);
  infoVisible = false;
}

void updateTasksTextFromHome(action anAction, taskStructure& aTaskStruct) {
  if (SKIP == anAction) {
    aTaskStruct.current->skippedAmount++;
    nextTask(aTaskStruct);
    updateHomeTasks(aTaskStruct);
  }
  else{
    aTaskStruct.current->completedAmount++;
    nextTask(aTaskStruct);
    updateHomeTasks(aTaskStruct);
  }
}

// GOATED LVGL CREATOR: lv_label_set_text_fmt(label, "Value: %d", 15) for printf formating
// functions to inilialize tasks
void updateFocusTasks(taskStructure& aTaskStruct){
  Serial.println("Update Focus task");
  // uodate current task
  lv_label_set_text_fmt(ui_actualTask, "%s %s", aTaskStruct.current->time, aTaskStruct.current->name);
}
void updateHomeTasks(taskStructure& aTaskStruct){
  Serial.println("Update home task");
  // updated current & next task
  lv_label_set_text_fmt(ui_currentTaskLable, "%s %s", aTaskStruct.current->time, aTaskStruct.current->name);
  lv_label_set_text_fmt(ui_nextTasklabel, "%s %s", aTaskStruct.next->time, aTaskStruct.next->name);
}
void updateTaskPageTasks(taskStructure& aTaskStruct){
  Serial.println("Update task page task");
  // update all tasks current, prev, next
  lv_label_set_text_fmt(ui_centerTask, "%s %s", aTaskStruct.current->time, aTaskStruct.current->name);
  lv_label_set_text_fmt(ui_bottomTask, "%s %s", aTaskStruct.next->time, aTaskStruct.next->name);
  lv_label_set_text_fmt(ui_topTask, "%s %s", aTaskStruct.prev->time, aTaskStruct.prev->name);
}

//////// Functions to inilialize taks in POPUP ///////
void updateTaskInPopUp(task* t){
  lv_label_set_text_fmt(ui_selectedTask, "%s", t->name);
}
void updateTimeInPopUp(task* t){
  lv_label_set_text_fmt(ui_time, "%s", t->time);
}

// initialize the taskStructure so that the current task matches the time.
void initializeTaskStructure(taskStructure& aTaskStruct){
  Serial.println("Initialoze task struct");
  while(aTaskStruct.next->time24hr < current24hrTime){
    nextTask(aTaskStruct);
  }
  updateFocusTasks(aTaskStruct);
  updateHomeTasks(aTaskStruct);
  updateTaskPageTasks(aTaskStruct);
}

// Iterates the prev,current,and next tasks in a task structure to the next task
void nextTask(taskStructure& aTaskStruct){
  aTaskStruct.prev = aTaskStruct.current;
  aTaskStruct.current = aTaskStruct.next;
  if(taskListEnd == aTaskStruct.current){
    aTaskStruct.next = taskListBegin;
  }
  else{
    aTaskStruct.next ++;//= taskIncrement;
  }
  Serial.println("nextTask Diagnostic:");
  Serial.println((long)aTaskStruct.prev); Serial.println((long)aTaskStruct.current); Serial.println((long)aTaskStruct.next);
}

// Iterates the prev,current,and next tasks in a task structure to the previous task
void prevTask(taskStructure& aTaskStruct
){
  aTaskStruct.next = aTaskStruct.current;
  aTaskStruct.current = aTaskStruct.prev;
  if(taskListBegin == aTaskStruct.current){
    aTaskStruct.prev = taskListEnd;
  }
  else{
    aTaskStruct.prev --;// taskIncrement;
  }
}

