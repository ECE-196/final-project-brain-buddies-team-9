// This file was generated by SquareLine Studio
// SquareLine Studio version: SquareLine Studio 1.5.0
// LVGL version: 8.3.6
// Project name: SquareLine_Project

#include "../ui.h"

void ui_Home_Page_screen_init(void)
{
ui_Home_Page = lv_obj_create(NULL);
lv_obj_clear_flag( ui_Home_Page, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Home_Page, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Home_Page, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_homePageLabel = lv_label_create(ui_Home_Page);
lv_obj_set_width( ui_homePageLabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_homePageLabel, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_homePageLabel, 0 );
lv_obj_set_y( ui_homePageLabel, -128 );
lv_obj_set_align( ui_homePageLabel, LV_ALIGN_CENTER );
lv_label_set_text(ui_homePageLabel,"HOME ");
lv_obj_set_style_text_color(ui_homePageLabel, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_homePageLabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_homePageLabel, &lv_font_montserrat_26, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_taskFromHome = lv_btn_create(ui_Home_Page);
lv_obj_set_width( ui_taskFromHome, 65);
lv_obj_set_height( ui_taskFromHome, 20);
lv_obj_set_x( ui_taskFromHome, 206 );
lv_obj_set_y( ui_taskFromHome, 0 );
lv_obj_set_align( ui_taskFromHome, LV_ALIGN_CENTER );
lv_obj_set_style_bg_color(ui_taskFromHome, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_taskFromHome, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_taskFromHome, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_taskFromHome, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_taskButtonFromHome = lv_label_create(ui_taskFromHome);
lv_obj_set_width( ui_taskButtonFromHome, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_taskButtonFromHome, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_taskButtonFromHome, -1 );
lv_obj_set_y( ui_taskButtonFromHome, 0 );
lv_obj_set_align( ui_taskButtonFromHome, LV_ALIGN_CENTER );
lv_label_set_text(ui_taskButtonFromHome,"TASK");

ui_focusFromHome = lv_btn_create(ui_Home_Page);
lv_obj_set_width( ui_focusFromHome, 65);
lv_obj_set_height( ui_focusFromHome, 20);
lv_obj_set_x( ui_focusFromHome, -207 );
lv_obj_set_y( ui_focusFromHome, 0 );
lv_obj_set_align( ui_focusFromHome, LV_ALIGN_CENTER );
lv_obj_set_style_bg_color(ui_focusFromHome, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_focusFromHome, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_color(ui_focusFromHome, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_focusFromHome, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_focusButtonFromHome = lv_label_create(ui_focusFromHome);
lv_obj_set_width( ui_focusButtonFromHome, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_focusButtonFromHome, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_focusButtonFromHome, -1 );
lv_obj_set_y( ui_focusButtonFromHome, 0 );
lv_obj_set_align( ui_focusButtonFromHome, LV_ALIGN_CENTER );
lv_label_set_text(ui_focusButtonFromHome,"FOCUS");

ui_nextTask = lv_obj_create(ui_Home_Page);
lv_obj_set_width( ui_nextTask, 260);
lv_obj_set_height( ui_nextTask, 50);
lv_obj_set_x( ui_nextTask, 0 );
lv_obj_set_y( ui_nextTask, 50 );
lv_obj_set_align( ui_nextTask, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_nextTask, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_nextTask, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_nextTask, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_nextTasklabel = lv_label_create(ui_nextTask);
lv_obj_set_width( ui_nextTasklabel, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_nextTasklabel, LV_SIZE_CONTENT);   /// 1
lv_label_set_text(ui_nextTasklabel,"Next Task");
lv_obj_set_style_text_color(ui_nextTasklabel, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_nextTasklabel, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_nextTasklabel, &lv_font_montserrat_18, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_currentTask = lv_obj_create(ui_Home_Page);
lv_obj_set_width( ui_currentTask, 348);
lv_obj_set_height( ui_currentTask, 69);
lv_obj_set_x( ui_currentTask, 2 );
lv_obj_set_y( ui_currentTask, -20 );
lv_obj_set_align( ui_currentTask, LV_ALIGN_CENTER );
lv_obj_clear_flag( ui_currentTask, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_currentTask, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_currentTask, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_align(ui_currentTask, LV_TEXT_ALIGN_LEFT, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_currentTaskLable = lv_label_create(ui_currentTask);
lv_obj_set_width( ui_currentTaskLable, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_currentTaskLable, LV_SIZE_CONTENT);   /// 1
lv_label_set_text(ui_currentTaskLable,"Current Task");
lv_obj_set_style_text_color(ui_currentTaskLable, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_currentTaskLable, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_currentTaskLable, &lv_font_montserrat_26, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_Container1 = lv_obj_create(ui_Home_Page);
lv_obj_remove_style_all(ui_Container1);
lv_obj_set_width( ui_Container1, 330);
lv_obj_set_height( ui_Container1, 214);
lv_obj_set_x( ui_Container1, 1 );
lv_obj_set_y( ui_Container1, 1 );
lv_obj_set_align( ui_Container1, LV_ALIGN_CENTER );
lv_obj_add_state( ui_Container1, LV_STATE_DISABLED );     /// States
lv_obj_add_flag( ui_Container1, LV_OBJ_FLAG_HIDDEN | LV_OBJ_FLAG_CHECKABLE | LV_OBJ_FLAG_EVENT_BUBBLE );   /// Flags
lv_obj_clear_flag( ui_Container1, LV_OBJ_FLAG_SCROLLABLE );    /// Flags
lv_obj_set_style_bg_color(ui_Container1, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_Container1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_color(ui_Container1, lv_color_hex(0x3C3737), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_border_opa(ui_Container1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_border_width(ui_Container1, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_color(ui_Container1, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_shadow_opa(ui_Container1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_width(ui_Container1, 5, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_shadow_spread(ui_Container1, 0, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_time = lv_label_create(ui_Container1);
lv_obj_set_width( ui_time, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_time, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_time, -70 );
lv_obj_set_y( ui_time, 25 );
lv_obj_set_align( ui_time, LV_ALIGN_CENTER );
lv_label_set_text(ui_time,"11:00 PM");
lv_obj_set_style_text_color(ui_time, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_time, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_selectedTask = lv_label_create(ui_Container1);
lv_obj_set_width( ui_selectedTask, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_selectedTask, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_selectedTask, -70 );
lv_obj_set_y( ui_selectedTask, -15 );
lv_obj_set_align( ui_selectedTask, LV_ALIGN_CENTER );
lv_label_set_text(ui_selectedTask,"\"Selected Task\"");
lv_obj_set_style_text_color(ui_selectedTask, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_selectedTask, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_selectedTask, &lv_font_montserrat_20, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_topButton = lv_btn_create(ui_Container1);
lv_obj_set_width( ui_topButton, 65);
lv_obj_set_height( ui_topButton, 20);
lv_obj_set_x( ui_topButton, 65 );
lv_obj_set_y( ui_topButton, 22 );
lv_obj_set_align( ui_topButton, LV_ALIGN_CENTER );
lv_obj_set_style_bg_color(ui_topButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_topButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_skip = lv_label_create(ui_topButton);
lv_obj_set_width( ui_skip, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_skip, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_skip, LV_ALIGN_CENTER );
lv_label_set_text(ui_skip,"Skip");
lv_obj_set_style_text_color(ui_skip, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_skip, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_skip, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_bottomButton = lv_btn_create(ui_Container1);
lv_obj_set_width( ui_bottomButton, 65);
lv_obj_set_height( ui_bottomButton, 20);
lv_obj_set_x( ui_bottomButton, 66 );
lv_obj_set_y( ui_bottomButton, 84 );
lv_obj_set_align( ui_bottomButton, LV_ALIGN_CENTER );
lv_obj_set_style_bg_color(ui_bottomButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_bottomButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_done = lv_label_create(ui_bottomButton);
lv_obj_set_width( ui_done, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_done, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_done, LV_ALIGN_CENTER );
lv_label_set_text(ui_done,"Done");
lv_obj_set_style_text_color(ui_done, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_done, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_done, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_selectButton = lv_btn_create(ui_Container1);
lv_obj_set_width( ui_selectButton, 65);
lv_obj_set_height( ui_selectButton, 20);
lv_obj_set_x( ui_selectButton, 120 );
lv_obj_set_y( ui_selectButton, 50 );
lv_obj_set_align( ui_selectButton, LV_ALIGN_CENTER );
lv_obj_set_style_bg_color(ui_selectButton, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_bg_opa(ui_selectButton, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_back = lv_label_create(ui_selectButton);
lv_obj_set_width( ui_back, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_back, LV_SIZE_CONTENT);   /// 1
lv_obj_set_align( ui_back, LV_ALIGN_CENTER );
lv_label_set_text(ui_back,"Back");
lv_obj_set_style_text_color(ui_back, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_back, 255, LV_PART_MAIN| LV_STATE_DEFAULT);
lv_obj_set_style_text_font(ui_back, &lv_font_montserrat_14, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_decor1 = lv_label_create(ui_Home_Page);
lv_obj_set_width( ui_decor1, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_decor1, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_decor1, 216 );
lv_obj_set_y( ui_decor1, 141 );
lv_obj_set_align( ui_decor1, LV_ALIGN_CENTER );
lv_label_set_text(ui_decor1,"<3 ");
lv_obj_set_style_text_color(ui_decor1, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_decor1, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

ui_decor2 = lv_label_create(ui_Home_Page);
lv_obj_set_width( ui_decor2, LV_SIZE_CONTENT);  /// 1
lv_obj_set_height( ui_decor2, LV_SIZE_CONTENT);   /// 1
lv_obj_set_x( ui_decor2, -229 );
lv_obj_set_y( ui_decor2, -143 );
lv_obj_set_align( ui_decor2, LV_ALIGN_CENTER );
lv_label_set_text(ui_decor2,":)");
lv_obj_set_style_text_color(ui_decor2, lv_color_hex(0x003049), LV_PART_MAIN | LV_STATE_DEFAULT );
lv_obj_set_style_text_opa(ui_decor2, 255, LV_PART_MAIN| LV_STATE_DEFAULT);

lv_obj_add_event_cb(ui_taskFromHome, ui_event_taskFromHome, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_focusFromHome, ui_event_focusFromHome, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_currentTask, ui_event_currentTask, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_topButton, ui_event_topButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_done, ui_event_done, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_bottomButton, ui_event_bottomButton, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_back, ui_event_back, LV_EVENT_ALL, NULL);
lv_obj_add_event_cb(ui_selectButton, ui_event_selectButton, LV_EVENT_ALL, NULL);

}