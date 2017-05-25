#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static BitmapLayer *s_arisha_layer;
static GBitmap *s_arisha_bitmap;

void set_arisha_text()
{
  static char lbl[] = "What time is it? IDK!";
  text_layer_set_text(s_time_layer, lbl);
}

void set_time_text()
{ 
  time_t temp = time(NULL);
  struct tm *tick_time = localtime(&temp);
  static char lbl[32];
  strftime(lbl, sizeof(lbl), "Actually, it's %I:%M", tick_time);
  text_layer_set_text(s_time_layer, lbl);
}

void tap_handler(AccelAxisType axis, int32_t direction)
{
  set_time_text();
  app_timer_register(3000, set_arisha_text, NULL);
}


static void main_window_load(Window *window) {
  // step 1: gen window
  Layer *window_layer = window_get_root_layer(window);
  window_set_background_color(s_main_window, GColorBlack);
  
  // step 2: Create GBitmap
  GRect bounds = layer_get_bounds(window_layer);
  s_arisha_bitmap = gbitmap_create_with_resource(RESOURCE_ID_ARISHACONV);
  s_arisha_layer = bitmap_layer_create(GRect(0, 10, bounds.size.w, bounds.size.h-10));
  bitmap_layer_set_bitmap(s_arisha_layer, s_arisha_bitmap);
  layer_add_child(window_layer, bitmap_layer_get_layer(s_arisha_layer));
  
  // step 3: text
  s_time_layer = text_layer_create(GRect(0, 5, bounds.size.w, 30));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorCyan);
  text_layer_set_font(s_time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24));
  text_layer_set_text_alignment(s_time_layer, GTextAlignmentCenter);
  set_arisha_text();
  layer_add_child(window_layer, text_layer_get_layer(s_time_layer));
  
  // step 4: sub to accel
  accel_tap_service_subscribe(tap_handler);

}


static void main_window_unload(Window *window) {
  
  // Destroy GBitmap
  gbitmap_destroy(s_arisha_bitmap);

  // Destroy BitmapLayer
  bitmap_layer_destroy(s_arisha_layer);
  // Destroy TextLayer
  text_layer_destroy(s_time_layer);
  accel_tap_service_unsubscribe();
}

static void init() {
  // Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });

  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
}
static void deinit() {

}

int main(void) {
  init();
  app_event_loop();
  deinit();
}

