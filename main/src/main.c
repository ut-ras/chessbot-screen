/**
 * @file main
 *
 */
// #define LV_OS_PTHREAD       1
// #define LV_USE_OS LV_OS_PTHREAD
// //  #include "lv_conf.h"
//  #include "lvgl/src/osal/lv_os.h"

/*********************
 *      INCLUDES
 *********************/
#define _DEFAULT_SOURCE /* needed for usleep() */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "lvgl/lvgl.h"
#include "lvgl/examples/lv_examples.h"
#include "lvgl/demos/lv_demos.h"
#include "glob.h"
static lv_display_t * hal_init(int32_t w, int32_t h)
;
// Custom delay function to replace lv_os_delay_ms
void custom_delay_ms(uint32_t ms) {
    usleep(ms * 1000);
}

// Thread function type
typedef void* (*pthread_func_t)(void*);

// Thread creation wrapper function
int custom_thread_create(pthread_t *thread, void *(*thread_func)(void*), size_t stack_size, int priority, void *param) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    // Set stack size if needed
    if (stack_size > 0) {
        pthread_attr_setstacksize(&attr, stack_size);
    }

    // Note: priority is ignored in this basic implementation
    // For proper priority handling, you would need to use pthread_setschedparam

    int ret = pthread_create(thread, &attr, thread_func, param);
    if (ret != 0) {
        printf("Error creating thread: %d\n", ret);
    }
    pthread_attr_destroy(&attr);
    return ret;
}

/*********************
 *      DEFINES
 *********************/
/**********************
 *      TYPEDEFS
 **********************/

/**********************

/**********************
 *  STATIC VARIABLES
 **********************/

/**********************
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

extern void freertos_main(void);

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

/**********************
 *      VARIABLES
 **********************/

/**********************
 *  STATIC PROTOTYPES
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_style_t background_grid_array[64];
lv_obj_t * piece_circles [64];
// lv_obj_t **pause_address;
// lv_obj_t **reset_address;
bool play = false;
bool pieces[64];
bool test_toggle = true;

 void lv_grid_1(void)
 {
     static int32_t col_dsc[] = {40, 40, 40, 40, 40, 40, 40, 40, LV_GRID_TEMPLATE_LAST};
     static int32_t row_dsc[] = {40, 40, 40, 40, 40, 40, 40, 40, LV_GRID_TEMPLATE_LAST};

     /*Create a container with grid*/
     lv_obj_t * cont = lv_obj_create(lv_screen_active());
     lv_obj_set_style_grid_column_dsc_array(cont, col_dsc, 0);
     lv_obj_set_style_grid_row_dsc_array(cont, row_dsc, 0);
     lv_obj_set_size(cont, 480, 320);
     lv_obj_center(cont);
     lv_obj_set_layout(cont, LV_LAYOUT_GRID);
     lv_obj_set_style_pad_left(cont, 0, 0);
     lv_obj_set_style_pad_top(cont, 0, 0);
     lv_obj_set_style_pad_row(cont, 0, 0);
     lv_obj_set_style_pad_column(cont, 0, 0);

     lv_obj_t * label;
     lv_obj_t * obj;
     lv_obj_t * piece;

     uint8_t i;
     for(i = 0; i < 64; i++) {
         uint8_t col = i % 8;
         uint8_t row = i / 8;

         obj = lv_button_create(cont);
         /*Stretch the cell horizontally and vertically too
         *Set span to 1 to make the cell 1 column/row sized*/
         lv_style_t* style_cell = &background_grid_array[i];
         lv_style_init(style_cell);
         bool black = col % 2 ^ row % 2;
         if (!black) {
            lv_style_set_bg_color(style_cell, lv_color_hex(0xEEEED2));
         }
         else {
            lv_style_set_bg_color(style_cell, lv_color_hex(0x769656));
         }
         lv_style_set_radius(style_cell, 0);
         lv_style_set_bg_opa(style_cell, LV_OPA_100);
         // lv_style_set_pad_all(style_cell, 0);
         lv_obj_add_style(obj, style_cell, 0);
        //  lv_obj_set_style_pad_row(obj, 0, 0);
        //  lv_obj_set_style_pad_column(obj, 0, 0);
        //  lv_obj_set_style_pad_all(obj, 0, 0);
         lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                              LV_GRID_ALIGN_STRETCH, row, 1);

        //  label = lv_label_create(obj);
        //  lv_label_set_text_fmt(label, "c%d, r%d", col, row);
        //  lv_obj_center(label);

        piece = lv_obj_create(cont);
        piece_circles[i] = piece;
        pieces[i] = false;
        lv_obj_set_size(piece, 40, 40);
        lv_obj_set_grid_cell(piece, LV_GRID_ALIGN_STRETCH, col, 1,
                              LV_GRID_ALIGN_STRETCH, row, 1);
        lv_obj_set_style_radius(piece, 20, 0);
        lv_obj_set_style_opa(piece, LV_OPA_0, 0);
        lv_obj_set_style_border_width(piece, 0, 1);
        if (i < 16) {
          lv_obj_set_style_bg_color(piece, lv_color_black(), 0);
          lv_obj_set_style_border_color(piece, lv_color_white(), 0);
        }
        else if (i > 47) {
          lv_obj_set_style_bg_color(piece, lv_color_white(), 0);
          lv_obj_set_style_border_color(piece, lv_color_black(), 0);
        }
        else {
          lv_obj_set_style_bg_color(piece, lv_color_hex(0xAAAAAA), 0);
          lv_obj_set_style_border_color(piece, lv_color_hex(0x666666), 0);
        }
        // lv_style_t style_circle;
        // lv_style_init(&style_circle);

     }

    static lv_style_t style_grid;
    lv_style_init(&style_grid);
    lv_style_set_bg_color(&style_grid, lv_color_white());
    lv_style_set_bg_opa(&style_grid, LV_OPA_100);
    lv_style_set_border_width(&style_grid, 0);
    lv_style_set_border_color(&style_grid, lv_color_black());
    lv_obj_add_style(cont, &style_grid, 0);

 }

 static void pause_cb(lv_event_t * e)
 {
     lv_event_code_t code = lv_event_get_code(e);
     lv_obj_t * btn = lv_event_get_target(e);
    //  printf(e);
    //  printf("\n");
    //  printf(&pause_address);
    //  if (e != &pause_address) {
    //   printf("d");
    //  }
     if(code == LV_EVENT_CLICKED) {
        if (play) {
          play = false;
          // pause game
        }
        else {
          play = true;
        }
     }
 }

 static lv_color_t reset_cb(lv_event_t * e)
 {
    //  printf("hmm");
     lv_event_code_t code = lv_event_get_code(e);
     lv_obj_t * btn = lv_event_get_target(e);
     if(code == LV_EVENT_CLICKED) {
        // reset

     }
 }

 void lv_buttons() {
    lv_obj_t * pause_btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    // pause_address = pause_btn;
    lv_obj_set_pos(pause_btn, 320, 0);                            /*Set its position*/
    lv_obj_set_size(pause_btn, 155, 80);                          /*Set its size*/
    lv_obj_set_style_radius(pause_btn, 0, 0);
    lv_obj_add_event_cb(pause_btn, pause_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/
    lv_obj_t * label = lv_label_create(pause_btn);          /*Add a label to the button*/
    if (play) {
      // printf("Play");
      lv_label_set_text(label, "Play");                    /*Set the labels text*/
    }
    else {
      // printf("Pause");
      lv_label_set_text(label, "Pause");
    }
    lv_obj_center(label);

    lv_obj_t * reset_btn = lv_btn_create(lv_scr_act());     /*Add a button the current screen*/
    lv_obj_set_pos(reset_btn, 320, 80);                            /*Set its position*/
    lv_obj_set_size(reset_btn, 155, 80);                          /*Set its size*/
    lv_obj_set_style_radius(reset_btn, 0, 0);
    lv_obj_add_event_cb(reset_btn, reset_cb, LV_EVENT_ALL, NULL);           /*Assign a callback to the button*/
    lv_obj_set_style_bg_color(reset_btn, lv_color_hex(0x8E1600), 0);
    label = lv_label_create(reset_btn);          /*Add a label to the button*/
    lv_label_set_text(label, "Reset");                    /*Set the labels text*/
    lv_obj_center(label);
 }

 // Add mutex for LVGL operations
pthread_mutex_t lvgl_mutex;

// Modified thread function for lv_timer_handler loop
void* lvgl_loop_thread(void *param) {
  (void)param;
  while (1) {
      pthread_mutex_lock(&lvgl_mutex);
      lv_tick_inc(5);         // Update LVGL tick count
      lv_timer_handler();       // Handle LVGL tasks
      pthread_mutex_unlock(&lvgl_mutex);
      custom_delay_ms(5);       // Use a short delay for smooth updates
  }
  return NULL;
}

// Modified test function with mutex protection
static void test() {
  while (1) {
    uint8_t i;
    test_toggle = !test_toggle;

    for (i = 0; i < 64; i++) {
          // Lock mutex before modifying UI
    pthread_mutex_lock(&lvgl_mutex);

      printf("i: %d\n", i);
      if (!test_toggle) {
        pieces[i] = true;
        printf("iiii: %d\n", i);

        // Check if piece_circles[i] is valid before setting opacity
        if (piece_circles[i] != NULL && (i < 16 || i > 47)) {
          lv_obj_set_style_opa(piece_circles[i], LV_OPA_100, 0);
        } else {
          printf("piece ciecle is null booooo\n");
          i += 31;
        }
        printf("i: %d\n", i);

        // if (i > 2) {
        //   pieces[i - 3] = false;
        //   printf("b: %d\n", i);

        //   lv_obj_set_style_opa(piece_circles[i - 3], LV_OPA_0, 0);
        //   printf("c: %d\n", i);

        // }
      }
      else {
        pieces[i] = false;
        printf("iiii: %d\n", i);

        // Check if piece_circles[i] is valid before setting opacity
        if (piece_circles[i] != NULL && (i < 16 || i > 47)) {
          lv_obj_set_style_opa(piece_circles[i], LV_OPA_0, 0);
        } else {
          printf("piece ciecle is null booooo\n");
          i += 31;
        }
        printf("i: %d\n", i);
      printf("d: %d\n", i);
      }
    lv_tick_inc(100); // Example if you're simulating some delay
    printf("e: %d\n", i);

    // Unlock mutex after modifying UI
    pthread_mutex_unlock(&lvgl_mutex);

    custom_delay_ms(100); // portable LVGL delay
    }

  }
}

// Thread function for test()
void* test_thread(void *param) {
  (void)param;

  test();

  return NULL;
}

int main(int argc, char **argv)
{
  (void)argc; /*Unused*/
  (void)argv; /*Unused*/

  /*Initialize LVGL*/
  lv_init();

  // Initialize mutex for LVGL operations
  pthread_mutex_init(&lvgl_mutex, NULL);

  /*Initialize the HAL (display, input devices, tick) for LVGL*/
  hal_init(480,320);
 // lv_demo_widgets();
 pthread_mutex_lock(&lvgl_mutex);
 lv_grid_1();
 pthread_mutex_unlock(&lvgl_mutex);
  // Create test thread
  pthread_t test_thread_handle;
  custom_thread_create(&test_thread_handle, test_thread, 4096, 1, NULL);



  while(1) {
    pthread_mutex_lock(&lvgl_mutex);
    lv_buttons();
    /* Periodically call the lv_task handler.
     * It could be done in a timer interrupt or an OS task too.*/
    lv_timer_handler();
    lv_tick_inc(5); // Update LVGL tick count
    pthread_mutex_unlock(&lvgl_mutex);

    usleep(5 * 1000);
  }

  // This will never be reached
  pthread_mutex_destroy(&lvgl_mutex);

  return 0;
}

/**********************
 *   STATIC FUNCTIONS
 **********************/

/**
 * Initialize the Hardware Abstraction Layer (HAL) for the LVGL graphics
 * library
 */
static lv_display_t * hal_init(int32_t w, int32_t h)
{

  lv_group_set_default(lv_group_create());

  lv_display_t * disp = lv_sdl_window_create(w, h);

  lv_indev_t * mouse = lv_sdl_mouse_create();
  lv_indev_set_group(mouse, lv_group_get_default());
  lv_indev_set_display(mouse, disp);
  lv_display_set_default(disp);

  LV_IMAGE_DECLARE(mouse_cursor_icon); /*Declare the image file.*/
  lv_obj_t * cursor_obj;
  cursor_obj = lv_image_create(lv_screen_active()); /*Create an image object for the cursor */
  lv_image_set_src(cursor_obj, &mouse_cursor_icon);           /*Set the image source*/
  lv_indev_set_cursor(mouse, cursor_obj);             /*Connect the image  object to the driver*/

  lv_indev_t * mousewheel = lv_sdl_mousewheel_create();
  lv_indev_set_display(mousewheel, disp);
  lv_indev_set_group(mousewheel, lv_group_get_default());

  lv_indev_t * kb = lv_sdl_keyboard_create();
  lv_indev_set_display(kb, disp);
  lv_indev_set_group(kb, lv_group_get_default());

  return disp;
}
