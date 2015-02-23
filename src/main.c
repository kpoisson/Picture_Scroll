/* cloned and modified from Pebble-SexRoulette by vvzvlad (Thanks, with respect)          */
/* See your own pictures along with the time and date.  Using 3 buttons to scroll through */
/* your pictures, top button as a Previous, bottom Button as Next, center as Random.      */
  
/* Create your pictures for resources on this link:  http://gazs.github.io/canvas-atkinson-dither/    */
/* Import the picture you want, under "Fit Picture In" choose other and set it to 144x168 (approx).   */
/* Save it to your desktop, it will create a file called "download", no extension.  Rename it to      */
/* Pic00.png, etc..., Repeat for up to 28 pictures or as many as will end up compiling/running on the */
/* watch.  Now add them as a resource with "Add New" button set "Identifier" to PIC00,...PIC27        */
/* Compile the watch app - if its over 100KiB, then delete or shrink some pics, it won't load onto    */
/* the watch if over 100Kib, at least that is what I have found - Results may vary!                   */

#include <pebble.h>
#include <string.h>
 
float timer_delay=1;              // Variable for a time to the timer 
Window *window;                   // Pointer to the window 
TextLayer *text_layer;            // Pointer to the text layer 
static BitmapLayer *image_layer;  // Pointer to the graphics layer 
static GBitmap *image;            // Pointer to the image memory 
static bool first_time=true;      // Boolean Flag first run 
int some_nbr=-1;                  // Variable to be used to choose a item from the array 


// Create an array of resource identifiers 
static const uint32_t images[] = {RESOURCE_ID_PIC00,RESOURCE_ID_PIC01,RESOURCE_ID_PIC02,RESOURCE_ID_PIC03,RESOURCE_ID_PIC04,RESOURCE_ID_PIC05,RESOURCE_ID_PIC06,RESOURCE_ID_PIC07,RESOURCE_ID_PIC08,RESOURCE_ID_PIC09,RESOURCE_ID_PIC10,RESOURCE_ID_PIC11,RESOURCE_ID_PIC12,RESOURCE_ID_PIC13,RESOURCE_ID_PIC14,RESOURCE_ID_PIC15,RESOURCE_ID_PIC16,RESOURCE_ID_PIC17,RESOURCE_ID_PIC18,RESOURCE_ID_PIC19,RESOURCE_ID_PIC20,RESOURCE_ID_PIC21,RESOURCE_ID_PIC22,RESOURCE_ID_PIC23,RESOURCE_ID_PIC24,RESOURCE_ID_PIC25,RESOURCE_ID_PIC26,RESOURCE_ID_PIC27,};

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
   
  // Create a long-lived buffer
  static char buffer[] = "00:00    xx/xx/xx";
 	
  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    //Use 24h hour format
    strftime(buffer, sizeof("00:00    xx/xx/xx"), "%H:%M   %m/%d/%y", tick_time);
  } else {
    //Use 12 hour format
    strftime(buffer, sizeof("00:00    xx/xx/xx"), "%I:%M   %m/%d/%y", tick_time);
  }

  // Display this time on the TextLayer
  text_layer_set_text(text_layer, buffer); 
}

 static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

// To avoid duplication of code creates a function that deals initialize and configure text array 
void config_text_layer(int16_t x,int16_t y,int16_t h,int16_t w, const char *font_key)  
{
    // create a text array that specifies the size and position   
    text_layer = text_layer_create(GRect(x, y, h, w)); 
    // Set the text color 
    text_layer_set_text_color(text_layer, GColorWhite);  
    // set the background color 
    text_layer_set_background_color(text_layer, GColorBlack);  
    // install fonts 
    text_layer_set_font(text_layer, fonts_get_system_font(font_key));
    // set centered 
    text_layer_set_text_alignment(text_layer, GTextAlignmentCenter); 
    // connect to the main text layer as a child 
    layer_add_child(window_get_root_layer(window), text_layer_get_layer(text_layer));  
}

void timer_call() // This function is called when the timer 
{
    if (first_time == false)   // if you do not run for the first time... 
      {
        bitmap_layer_destroy(image_layer); // ...then remove the old layer... 
        gbitmap_destroy(image);            // ...and clear the memory of the previous picture 
      }
    first_time = false; // reset flag first run 

   // Create a graphic array specifies the size and position
     image_layer = bitmap_layer_create(GRect(0 , 0, 144, 144)); 
   // connect to the main graphics layer as a child 
    layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(image_layer));
   // Adjustable parameters of the overlay 
    bitmap_layer_set_compositing_mode(image_layer, GCompOpAssign);
  // or you could use bitmap_layer_set_compositing_mode(image_layer, GCompOpAssignInverted); 

  // load into memory the selected picture from the array of connected resources 
    image = gbitmap_create_with_resource(images[some_nbr]);
  // output the downloaded image in the layer 
    bitmap_layer_set_bitmap(image_layer, image); 
	   
    if (timer_delay < 300*100 )        // if the delay has not yet reached 300ms... 
    {
        timer_delay=timer_delay/0.7;   // ...increase the delay... 
      //...and a platoon of the timer again 
        app_timer_register(timer_delay/100, timer_call, NULL); 
    }
    else // if the delay is for more than 300ms ... 
    {
      // resets the timer at the beginning and go - the message and the image we've already shown 
        timer_delay=1; 
        light_enable_interaction(); // Turns backlight 
    }
}

// Function that triggers when you click on the center button 
void clickrnd(ClickRecognizerRef recognizer, void *context)  
{
  if (first_time == true) // If the program is started for the first time... 
  {
    text_layer_destroy(text_layer);  // ...it cleans image
    // ...and create a new layer messages with other coordinates and font 
    config_text_layer(0, 140, 148, 168-140, FONT_KEY_GOTHIC_24); 
  }
  some_nbr = rand() % 27; 
  timer_call(); // platoon timer for quick-change messages 
  // Display this time on the TextLayer
  update_time();
}

// Function that triggers when you click on the bottom button 
void clicknext(ClickRecognizerRef recognizer, void *context)  
{
  if (first_time == true) // If the program is started for the first time... 
  {
    text_layer_destroy(text_layer);   // ...it cleans image... 
     // ...and create a new layer messages with other coordinates and font 
    config_text_layer(0, 140, 148, 168-140, FONT_KEY_GOTHIC_24); 
  }
  if (some_nbr == 27)
    some_nbr=0 ;
  else
    some_nbr++ ;

  timer_call(); // platoon timer for quick-change messages 
  // Display this time on the TextLayer
  update_time();
}

// Function that triggers when you click on the top button 
void clickprev(ClickRecognizerRef recognizer, void *context)  
{
  if (first_time == true) // If the program is started for the first time... 
  {
    text_layer_destroy(text_layer);   // ...it cleans image... 
     // ...and create a new layer messages with other coordinates and font 
    config_text_layer(0, 140, 148, 168-140, FONT_KEY_GOTHIC_24); 
  }
  if (some_nbr <= 0)
    some_nbr=27 ;
  else 
    some_nbr-- ;

  timer_call(); // platoon timer for quick-change messages 
  // Display this time on the TextLayer
  update_time();
}

// function, which should be within the subscription keys 
void WindowsClickConfigProvider(void *context)  
{
    // when you press the top button to start click 
    window_single_click_subscribe(BUTTON_ID_UP, clickprev); 
    // when you press the center button to start click 
    window_single_click_subscribe(BUTTON_ID_SELECT, clickrnd); 
    // when you press the bottom button to start click 
    window_single_click_subscribe(BUTTON_ID_DOWN, clicknext);
}

static void init() {
  window = window_create();  // initialize window 
  window_set_background_color(window, GColorBlack); // set the background color 
  window_set_fullscreen(window, true);  // Includes full screen 
  window_stack_push(window, true);      // open the window 
  
  srand(time(NULL)); // initialize the random number generator of the current time 
  // define a function, which will be subscription buttons 
  window_set_click_config_provider(window, WindowsClickConfigProvider); 
  // Customizable text layer with a greeting message 
  config_text_layer(0, 140, 148, 168-140, FONT_KEY_GOTHIC_24); 
  
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
}

static void deinit() {
    // Destroy Window
    // Make sure the time is displayed from the start 
	  // persist_write_int(1, 1); 
    text_layer_destroy(text_layer);      // destroy the text layer, release resources 
    window_destroy(window);              // destroy the main window, release resources 
    bitmap_layer_destroy(image_layer);   // destroy the graphics layer, release resources 
    if (first_time == false) // if we go out after starting the drawing... 
    {
     // then destroy an array of graphics, frees resources differently - not destroy, because
     // it has not been created 
     gbitmap_destroy(image); 
    }
}

int main(void)
{
    init();
    app_event_loop();  // waiting for events 
    deinit();
}
