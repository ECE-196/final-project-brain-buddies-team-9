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
	char time[10];
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

void updateTasksTextFromHome();
void bottomFromContainer(taskStructure& s);
void selectfromContainer(taskStructure& s);
void topfromContainer(taskStructure& s);

void updateFocusTasks(taskStructure& s);
void updateHomeTasks(taskStructure& s);
void updateTaskPageTasks(taskStructure& s);

void prevTask(taskStructure& s);
void nextTask(taskStructure& s);

enum action { SKIP, DONE, BACK };

uint64_t init_time; // here accessable everywhere, const since only set once.
uint64_t currentMicros;
uint64_t previousMicros;
uint64_t lastClockUpdateMicros;

volatile byte containerVisible = false;

uint64_t seconds = 1*1000*1000; // 23 * seconds -> 23 seconds in microseconds
uint64_t minutes = 60*seconds;  // 45 * minutes -> 45 minutes in microseconds
uint64_t hours   = 60*minutes;  // 3  * hours   -> 3  hours   in microseconds
const int taskListLength = 10;
//set up struct array
task taskListTwo[taskListLength]= {
  {800 ,    "8:00am",   "Wake up", 0, 0},
  {900 ,    "9:00am",   "Eat breakfast", 0, 0},
  {1000,    "10:00am",  "Go to school", 0, 0},
  {1100,    "11:00am",  "Attend classes", 0, 0},
  {1500,    "3:00pm",   "Eat lunch", 0, 0},
  {1700,    "5:00pm",   "Leave school", 0, 0},
  {1800,    "6:00pm",   "Go to gym", 0, 0},
  {1900,    "7:00pm",   "Eat dinner", 0, 0},
  {2000,    "8:00pm",   "Shower", 0, 0},
  {2200,    "10:00pm",  "Go to sleep", 0, 0},

  // {30 * seconds,  "8:00am",   "Wake up", 0, 0},
  // {2  * minutes,  "9:00am",   "Eat breakfast", 0, 0},
  // {5  * minutes,  "10:00am",  "Go to school", 0, 0},
  // {10 * minutes,  "11:00am",  "Attend classes", 0, 0},
  // {30 * minutes,  "3:00pm",   "Eat lunch", 0, 0},
  // {1  * hours,    "5:00pm",   "Leave school", 0, 0},
  // {2  * hours,    "6:00pm",   "Go to gym", 0, 0},
  // {3  * hours,    "7:00pm",   "Eat dinner", 0, 0},
  // {4  * hours,    "8:00pm",   "Shower", 0, 0},
  // {5  * hours,    "10:00pm",  "Go to sleep", 0, 0},
};

// initialize a taskStructure with data {current task, prev task, next task} all poitners to the task array 
taskStructure S = { 
  &taskListTwo[taskListLength - 1],
  &taskListTwo[0],
  &taskListTwo[1]
};
taskStructure S_taskPage = { 
  &taskListTwo[taskListLength - 1],
  &taskListTwo[0],
  &taskListTwo[1]
};
task *taskListBegin = &taskListTwo[0];
task *taskListEnd = &taskListTwo[taskListLength - 1];

//int taskCounter2 = 0;
//int taskListLength2 = 10;

//Declarations for button up
//char taskList[10][10] = {"8am", "9am", "10am", "11am", "12pm", "1pm", "2pm", "3pm", "4pm", "5pm"};

//int taskCounter = 9;


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



  // need to initialize the tasks in task page
  updateTaskPageTasks(S);
  //initialize task in home page
  updateHomeTasks(S);
  // initialize task in focus page
  updateFocusTasks(S);
  // delay(2000);
  // Serial.print("Size of Task: "); Serial.println((long)taskIncrement);
  // Serial.print("Begin Address: "); Serial.println((long)taskListBegin);
  // Serial.print("End Address: "); Serial.println((long)taskListEnd);
  // for(int i = 0; i < taskListLength; i++){
  //   Serial.print("Address "); Serial.println(i); Serial.println((long)&taskListTwo[i]);
  // }
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
    upFromTask(S_taskPage);
    upState = LOW;
  }
  
  //     setup for Down Button
  downPinState = digitalRead(interruptDownPin);
  if(HIGH == downState && LOW == downPinState){
    downFromTask(S_taskPage);
    downState = LOW;
  }

  //     setup for Right Button
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
    Serial.println("selectFromHome");
    selectFromHome();
    selectState = LOW;
    bottomState = LOW;
    topState = LOW;
  }
  if(HIGH == bottomState && LOW == bottomPinState && current_screen == ui_Home_Page && containerVisible == true){
    Serial.println("bottomFromContainer");
    selectfromContainer(S);
    bottomState = LOW;
  }
  else if(HIGH == selectState && LOW == selectPinState && current_screen == ui_Home_Page && containerVisible == true){
    Serial.println("selectFromContainer");
    bottomFromContainer();    
    selectState = LOW;
  }
  else if(HIGH == topState && LOW == topPinState && current_screen == ui_Home_Page && containerVisible == true){
    Serial.println("topFromContainer");
    topfromContainer(S);
    topState = LOW;
  }


  // Havng a notification POPUP
  //if (task_time_in_microseconds + init_time < currentMicrosec){
	//	do the popup/notif
  //time24hr

  if(S.current->time24hr + init_time < currentMicros){
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
void upFromTask(taskStructure& s){
  prevTask(s);
  updateTaskPageTasks(s);

  // taskCounter--;
  // if(taskCounter == -1){
  //   taskCounter = 9;
  // }
  // //    Special Cases
  // if(taskCounter == 0){
  //   updateTaskText(taskList[taskListLength - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  // }
  // else if(taskCounter == 9){
  //   updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[0]);
  // }
  // else{
  //   updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  // }
}

//    Using DOWN bottom on TASK page
void downFromTask(taskStructure& s){
  nextTask(s);
  updateTaskPageTasks(s);
  // taskCounter++;
  // if(taskCounter == 10){
  //   taskCounter = 0;
  // }
  // //    Special Cases
  // if(taskCounter == 0){
  //   updateTaskText(taskList[taskListLength - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  // }
  // else if(taskCounter == 9){
  //   updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[0]);
  // }
  // else{
  //   updateTaskText(taskList[taskCounter - 1], taskList[taskCounter], taskList[taskCounter + 1]);
  // }
}

//    Function to assign tasks when using up or down buttons
// void updateTaskText(char* top, char* center, char* bottom){
//   lv_label_set_text(ui_topTask, top);
//   lv_label_set_text(ui_centerTask, center);
//   lv_label_set_text(ui_bottomTask, bottom);
// }


//      CUSTOM FUNCTIONS FOR NAVEGATING IN HOME PAGE
void selectFromHome(){
  lv_event_send(ui_currentTask, LV_EVENT_CLICKED, NULL);
  containerVisible = true;
}

void bottomFromContainer(){
  lv_event_send(ui_bottomButton, LV_EVENT_CLICKED, NULL);
  containerVisible = false;
}

void selectfromContainer(taskStructure& s){
  lv_event_send(ui_selectButton, LV_EVENT_CLICKED, NULL);
  updateTasksTextFromHome(DONE, s);
  containerVisible = false;
}

void topfromContainer(taskStructure& s){
  lv_event_send(ui_topButton, LV_EVENT_CLICKED, NULL);
  updateTasksTextFromHome(SKIP, s);
  containerVisible = false;
}

//    Function to assign tasks when using decision buttons
void updateTasksTextFromHome(action anAction, taskStructure& s) {

  if (SKIP == anAction) {
    s.current->skippedAmount++;
    nextTask(s);
    updateHomeTasks(s);
    // taskListTwo[taskCounter2].skippedAmount ++;
    // taskCounter2++;
    // if(taskCounter2 == 8) { 
    //     taskCounter2 = 0; 
    // }
    // lv_label_set_text(ui_currentTaskLable, taskListTwo[taskCounter2].name);
    // lv_label_set_text(ui_nextTasklabel, taskListTwo[taskCounter2 + 1].name);
  } 
  else{
  //else if (strcmp(action, "DONE") == 0) {
    s.current->skippedAmount++;
    nextTask(s);
    updateHomeTasks(s);
    // taskListTwo[taskCounter2].completedAmount ++;
    // taskCounter2++;
    // if (taskCounter2 == 8) { 
    //     taskCounter2 = 0; 
    // }
    // lv_label_set_text(ui_currentTaskLable, taskListTwo[taskCounter2].name);
    // lv_label_set_text(ui_nextTasklabel, taskListTwo[taskCounter2 + 1].name);
  } 
}

// GOATED LVGL CREATOR: lv_label_set_text_fmt(label, "Value: %d", 15) for printf formating
// functions to inilialize tasks
void updateFocusTasks(taskStructure& s){
  // uodate current task
  lv_label_set_text_fmt(ui_actualTask, "%s %s", s.current->time, s.current->name);
}
void updateHomeTasks(taskStructure& s){
  // updated current & next task
  lv_label_set_text_fmt(ui_currentTaskLable, "%s %s", s.current->time, s.current->name);
  lv_label_set_text_fmt(ui_nextTasklabel, "%s %s", s.next->time, s.next->name);
}
void updateTaskPageTasks(taskStructure& s){
  // update all tasks current, prev, next
  lv_label_set_text_fmt(ui_centerTask, "%s %s", s.current->time, s.current->name);
  lv_label_set_text_fmt(ui_bottomTask, "%s %s", s.next->time, s.next->name);
  lv_label_set_text_fmt(ui_topTask, "%s %s", s.prev->time, s.prev->name);
}

// Iterates the prev,current,and next tasks in a task structure to the next task
void nextTask(taskStructure& s){
  s.prev = s.current;
  s.current = s.next;
  if(taskListEnd == s.current){
    s.next = taskListBegin;
  }
  else{
    s.next ++;//= taskIncrement; 
  }
  Serial.println("nextTask Diagnostic:");
  Serial.println((long)s.prev); Serial.println((long)s.current); Serial.println((long)s.next);
}

// Iterates the prev,current,and next tasks in a task structure to the previous task
void prevTask(taskStructure& s){
  s.next = s.current;
  s.current = s.prev;
  if(taskListBegin == s.current){
    s.prev = taskListEnd;
  }
  else{
    s.prev --;// taskIncrement; 
  }
}

  //   static lv_img_dsc_t my_racon_dsc = {
  //     .header.always_zero = 0;
  //     .header.w
  //   }

  //   LV_IMG_DECLARE(my_racon_dsc);
  //   lv_obj_t * icon = lv_img_create(lv_scr_act(),NULL)
  //   lv_img_set_src(icon, "S:racon.c");