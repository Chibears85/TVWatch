#include "pebble_os.h"
#include "pebble_app.h"
#include "pebble_fonts.h"

#define MY_UUID {0x71, 0x12, 0xD6, 0x24, 0x65, 0x55, 0x42, 0x79, 0x89, 0xAE, 0xC0, 0x86, 0xE9, 0x36, 0x94, 0x27}
PBL_APP_INFO(MY_UUID,
	     "TODAY", "Stephen Murphy",
	     3, 0, /* App major/minor version */
	     RESOURCE_ID_IMAGE_MENU_ICON,
	     APP_INFO_WATCH_FACE);

Window window;


BmpContainer background_image;

BmpContainer meter_bar_image;

// TODO: Handle 12/24 mode preference when it's exposed.
BmpContainer time_format_image;


const int DAY_NAME_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DAY_NAME_SUN,
  RESOURCE_ID_IMAGE_DAY_NAME_MON,
  RESOURCE_ID_IMAGE_DAY_NAME_TUE,
  RESOURCE_ID_IMAGE_DAY_NAME_WED,
  RESOURCE_ID_IMAGE_DAY_NAME_THU,
  RESOURCE_ID_IMAGE_DAY_NAME_FRI,
  RESOURCE_ID_IMAGE_DAY_NAME_SAT
};

BmpContainer day_name_image;


const int DATENUM_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_DATENUM_0,
  RESOURCE_ID_IMAGE_DATENUM_1,
  RESOURCE_ID_IMAGE_DATENUM_2,
  RESOURCE_ID_IMAGE_DATENUM_3,
  RESOURCE_ID_IMAGE_DATENUM_4,
  RESOURCE_ID_IMAGE_DATENUM_5,
  RESOURCE_ID_IMAGE_DATENUM_6,
  RESOURCE_ID_IMAGE_DATENUM_7,
  RESOURCE_ID_IMAGE_DATENUM_8,
  RESOURCE_ID_IMAGE_DATENUM_9
};


#define TOTAL_DATE_DIGITS 2
BmpContainer date_digits_images[TOTAL_DATE_DIGITS];


const int BIG_DIGIT_IMAGE_RESOURCE_IDS[] = {
  RESOURCE_ID_IMAGE_NUM_0,
  RESOURCE_ID_IMAGE_NUM_1,
  RESOURCE_ID_IMAGE_NUM_2,
  RESOURCE_ID_IMAGE_NUM_3,
  RESOURCE_ID_IMAGE_NUM_4,
  RESOURCE_ID_IMAGE_NUM_5,
  RESOURCE_ID_IMAGE_NUM_6,
  RESOURCE_ID_IMAGE_NUM_7,
  RESOURCE_ID_IMAGE_NUM_8,
  RESOURCE_ID_IMAGE_NUM_9
};


#define TOTAL_TIME_DIGITS 4
BmpContainer time_digits_images[TOTAL_TIME_DIGITS];


void set_container_image(BmpContainer *bmp_container, const int resource_id, GPoint origin) {

  layer_remove_from_parent(&bmp_container->layer.layer);
  bmp_deinit_container(bmp_container);

  bmp_init_container(resource_id, bmp_container);

  GRect frame = layer_get_frame(&bmp_container->layer.layer);
  frame.origin.x = origin.x;
  frame.origin.y = origin.y;
  layer_set_frame(&bmp_container->layer.layer, frame);

  layer_add_child(&window.layer, &bmp_container->layer.layer);
}


unsigned short get_display_hour(unsigned short hour) {

  if (clock_is_24h_style()) {
    return hour;
  }

  unsigned short display_hour = hour % 12;

  // Converts "0" to "12"
  return display_hour ? display_hour : 12;

}


void update_display(PblTm *current_time) {

  // TODO: Remove leading zero?
  set_container_image(&date_digits_images[0], DATENUM_IMAGE_RESOURCE_IDS[current_time->tm_mday/10], GPoint(117, 127));
  set_container_image(&date_digits_images[1], DATENUM_IMAGE_RESOURCE_IDS[current_time->tm_mday%10], GPoint(131, 127));


  unsigned short display_hour = get_display_hour(current_time->tm_hour);

  // TODO: Remove leading zero?
  set_container_image(&time_digits_images[0], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour/10], GPoint(6, 129));
  set_container_image(&time_digits_images[1], BIG_DIGIT_IMAGE_RESOURCE_IDS[display_hour%10], GPoint(21, 129));

  set_container_image(&time_digits_images[2], BIG_DIGIT_IMAGE_RESOURCE_IDS[current_time->tm_min/10], GPoint(44, 129));
  set_container_image(&time_digits_images[3], BIG_DIGIT_IMAGE_RESOURCE_IDS[current_time->tm_min%10], GPoint(61, 129));

   if (!clock_is_24h_style()) {
   if (current_time->tm_hour <= 3)
  set_container_image(&time_format_image, RESOURCE_ID_NBC, GPoint(0, 0));
  
else if (current_time->tm_hour == 4 && current_time->tm_min <= 29)
  set_container_image(&time_format_image, RESOURCE_ID_ETODAY, GPoint(0, 0));
  
else if (current_time->tm_hour == 4 && current_time->tm_min >= 30)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWSMORN, GPoint(0, 0));
  
else if (current_time->tm_hour == 5)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWSMORN, GPoint(0, 0));
  
else if (current_time->tm_hour == 6)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWSMORN, GPoint(0, 0));
  
else if (current_time->tm_hour == 12 && current_time->tm_min <= 29)
  set_container_image(&time_format_image, RESOURCE_ID_IMAGE_PM_MODE, GPoint(0, 0));
  
else if (current_time->tm_hour == 12 && current_time->tm_min >= 30)
  set_container_image(&time_format_image, RESOURCE_ID_NBC, GPoint(0, 0));

else if (current_time->tm_hour == 13 && current_time->tm_min <= 29)
  set_container_image(&time_format_image, RESOURCE_ID_WTC, GPoint(0, 0));
 
else if (current_time->tm_hour == 13 && current_time->tm_min >= 30)
  set_container_image(&time_format_image, RESOURCE_ID_BEARS, GPoint(0, 0));

else if (current_time->tm_hour == 14 && current_time->tm_min <= 29)
  set_container_image(&time_format_image, RESOURCE_ID_WTC, GPoint(0, 0));
 
else if (current_time->tm_hour == 14 && current_time->tm_min >= 30)
  set_container_image(&time_format_image, RESOURCE_ID_BEARS, GPoint(0, 0));
  
else if (current_time->tm_hour == 15)
  set_container_image(&time_format_image, RESOURCE_ID_NBC, GPoint(0, 0));
  
else if (current_time->tm_hour == 16)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWS4, GPoint(0, 0));
  
else if (current_time->tm_hour == 17 && current_time->tm_min <= 29)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWS5, GPoint(0, 0));

else if (current_time->tm_hour == 17 && current_time->tm_min >= 30)
  set_container_image(&time_format_image, RESOURCE_ID_NN, GPoint(0, 0));
  
else if (current_time->tm_hour == 18 && current_time->tm_min <= 29)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWS6, GPoint(0, 0));

else if (current_time->tm_hour == 18 && current_time->tm_min >= 30)
  set_container_image(&time_format_image, RESOURCE_ID_NBC, GPoint(0, 0));
  
else if (current_time->tm_hour == 19)
  set_container_image(&time_format_image, RESOURCE_ID_BEARS, GPoint(0, 0));
  
else if (current_time->tm_hour == 20)
  set_container_image(&time_format_image, RESOURCE_ID_WTC, GPoint(0, 0));
  
else if (current_time->tm_hour == 21)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWS9, GPoint(0, 0));
  
else if (current_time->tm_hour == 22 && current_time->tm_min <= 34)
  set_container_image(&time_format_image, RESOURCE_ID_9NEWS10, GPoint(0, 0));
  
else if (current_time->tm_hour == 22 && current_time->tm_min >= 35)
  set_container_image(&time_format_image, RESOURCE_ID_NBC, GPoint(0, 0));
  
else if (current_time->tm_hour >= 23)
  set_container_image(&time_format_image, RESOURCE_ID_WTC, GPoint(0, 0));
 
else {
  layer_remove_from_parent(&time_format_image.layer.layer);
  bmp_deinit_container(&time_format_image);
}

    if (display_hour/10 == 0) {
      layer_remove_from_parent(&time_digits_images[0].layer.layer);
      bmp_deinit_container(&time_digits_images[0]);
    }
  }

}


void handle_minute_tick(AppContextRef ctx, PebbleTickEvent *t) {
  (void)ctx;

  update_display(t->tick_time);
}


void handle_init(AppContextRef ctx) {
  (void)ctx;

  window_init(&window, "91 Dub");
  window_stack_push(&window, true /* Animated */);

  resource_init_current_app(&APP_RESOURCES);

  bmp_init_container(RESOURCE_ID_IMAGE_BACKGROUND, &background_image);
  layer_add_child(&window.layer, &background_image.layer.layer);


  bmp_init_container(RESOURCE_ID_IMAGE_METER_BAR, &meter_bar_image);

  meter_bar_image.layer.layer.frame.origin.x = 17;
  meter_bar_image.layer.layer.frame.origin.y = 43;

  layer_add_child(&window.layer, &meter_bar_image.layer.layer);


  if (clock_is_24h_style()) {
    bmp_init_container(RESOURCE_ID_IMAGE_24_HOUR_MODE, &time_format_image);

    time_format_image.layer.layer.frame.origin.x = 17;
    time_format_image.layer.layer.frame.origin.y = 68;

    layer_add_child(&window.layer, &time_format_image.layer.layer);
  }


  // Avoids a blank screen on watch start.
  PblTm tick_time;

  get_time(&tick_time);
  update_display(&tick_time);

}


void handle_deinit(AppContextRef ctx) {
  (void)ctx;

  bmp_deinit_container(&background_image);
  bmp_deinit_container(&meter_bar_image);
  bmp_deinit_container(&time_format_image);
  bmp_deinit_container(&day_name_image);

  for (int i = 0; i < TOTAL_DATE_DIGITS; i++) {
    bmp_deinit_container(&date_digits_images[i]);
  }

  for (int i = 0; i < TOTAL_TIME_DIGITS; i++) {
    bmp_deinit_container(&time_digits_images[i]);
  }

}


void pbl_main(void *params) {
  PebbleAppHandlers handlers = {
    .init_handler = &handle_init,
    .deinit_handler = &handle_deinit,

    .tick_info = {
      .tick_handler = &handle_minute_tick,
      .tick_units = MINUTE_UNIT
    }
  };
  app_event_loop(params, &handlers);
}
