#include <pebble.h>
#include "wakeup.h"
#include "main_window.h"
#include "splash_window.h"


Window *wakeupWindow;
//TextLayer *s_text_layer;
//static GBitmap *ticktick;
static ActionBarLayer *s_action_layer;
static TextLayer *s_label_layer;
static ActionBarLayer *s_action_bar_layer;


static GBitmap *s_tick_bitmap, *s_cross_bitmap;





// Click action
static void wakeup_handler(WakeupId id, int32_t reason) { 
  window_stack_push(wakeupWindow , true);
  vibes_double_pulse();
}
void wakeup_up_single_click_handler(ClickRecognizerRef recognizer, void *context){
   window_destroy(wakeupWindow);

}

void wakeup_down_single_click_handler(ClickRecognizerRef recognizer, void *context){
 window_stack_push(splash_window_get_window(), true);
}



//void tick_config_provider(void *context) {
//  window_single_click_subscribe(BUTTON_ID_SELECT, (ClickHandler)select_single_click_handler);
//}

void wakeup_click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)wakeup_up_single_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler) wakeup_down_single_click_handler);
}

static void wakeup(){
  
   // Next occuring (day/hour/minutes)
  printf("wir sind in der wakeupfunktion");

  time_t timestamp_1 = clock_to_timestamp(TODAY, 15, 18);
  //time_t timestamp_2 = clock_to_timestamp(TODAY, 13, 00);
  //time_t timestamp_3 = clock_to_timestamp(TODAY, 17, 00);
  //time_t timestamp_4 = clock_to_timestamp(TODAY, 21, 00);

  // Choose a 'cookie' value representing the reason for the wakeup
  const int cookie = 0;

  // If true, the user will be notified if they missed the wakeup 
  // (i.e. their watch was off)
  const bool notify_if_missed = true;

  // Schedule wakeup event
  WakeupId id_1 = wakeup_schedule(timestamp_1, cookie, notify_if_missed);
  //WakeupId id_2 = wakeup_schedule(timestamp_2, cookie, notify_if_missed);
  //WakeupId id_3 = wakeup_schedule(timestamp_3, cookie, notify_if_missed);
  //WakeupId id_4 = wakeup_schedule(timestamp_4, cookie, notify_if_missed);
  printf("scheduled!");
  // Check the scheduling was successful
   if(id_1 >= 0){
     // Persist the ID so that a future launch can query it
     const int wakeup_id_key = 43;
     persist_write_int(wakeup_id_key, id_1);
   }
  
  // Is the wakeup still scheduled?
if(wakeup_query(id_1, &timestamp_1)==0) {
  // Get the time remaining
  int seconds_remaining = timestamp_1 - time(NULL);
  printf("still scheduled?\n");
  APP_LOG(APP_LOG_LEVEL_INFO, "%d seconds until wakeup", seconds_remaining);
}
}

static void wakeup_window_load(Window *window) {

   Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);
  

  const GEdgeInsets label_insets = {.top = 10, .right = ACTION_BAR_WIDTH+ACTION_BAR_WIDTH / 2, .left = ACTION_BAR_WIDTH / 2};
  s_label_layer = text_layer_create(grect_inset(bounds, label_insets));
  text_layer_set_text(s_label_layer, "Time for three little questions?");
  text_layer_set_background_color(s_label_layer, GColorClear);
  text_layer_set_text_alignment(s_label_layer, GTextAlignmentCenter);
  text_layer_set_font(s_label_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  layer_add_child(window_layer, text_layer_get_layer(s_label_layer));

  s_tick_bitmap = gbitmap_create_with_resource(RESOURCE_ID_TICK);
  s_cross_bitmap = gbitmap_create_with_resource(RESOURCE_ID_CROSS);

  s_action_bar_layer = action_bar_layer_create();
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_UP, s_tick_bitmap);
  action_bar_layer_set_icon(s_action_bar_layer, BUTTON_ID_DOWN, s_cross_bitmap);
  action_bar_layer_add_to_window(s_action_bar_layer, window);
  
  // Set the click config provider:
  action_bar_layer_set_click_config_provider(s_action_bar_layer, wakeup_click_config_provider);
}

static void wakeup_window_unload(Window *window) {
}

void wakeup_window_create() {
   
  //If the app is launched due to a previously scheduled wakeup event
  if(launch_reason() == APP_LAUNCH_WAKEUP) {
    // The app was started by a wakeup event.
    WakeupId id = 0;
    int32_t reason = 0;

    // Get details and handle the event appropriately
    wakeup_get_launch_event(&id, &reason);
  }
  
  wakeup();
    // Subscribe to wakeup service
  
  wakeup_service_subscribe(wakeup_handler);
  
  //Layer *window_layer = window_get_root_layer(wakeupWindow);
  
  wakeupWindow = window_create();
    window_set_window_handlers(wakeupWindow, (WindowHandlers) {
        .load = wakeup_window_load,
        .unload =wakeup_window_unload,
    });
  //s_text_layer = text_layer_create(layer_get_bounds(window_layer));
  
  window_stack_push(wakeupWindow , true);
  
}

void wakeup_window_destroy() {
  window_destroy(wakeupWindow);
  //text_layer_destroy(s_text_layer);
  action_bar_layer_destroy(s_action_layer);
}

Window *wakeup_window_get_window(){
  return wakeupWindow;
}