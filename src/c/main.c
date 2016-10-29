#include <pebble.h>
#include "splash_window.h"
#include "main_window.h"


// Timer to see how long you can see the main window
void launch_main_window (){
  window_stack_push(main_window_get_window(), true);
}


int main (){

  //set up
  splash_window_create();
  main_window_create();
  
  window_stack_push(splash_window_get_window(), true);
  app_timer_register(3000, launch_main_window, NULL); 
  
  //main event loop
  app_event_loop();
  
  //clean up
  splash_window_destroy();
}