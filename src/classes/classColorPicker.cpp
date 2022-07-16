#include <classColorPicker.h>
#include <globalDefines.h>

extern lv_color_t colorOn;
extern lv_color_t colorBg;

void classColorPicker::_createColorPicker(void)
{
  // full screen overlay / semi transparent
  ovlPanel = lv_obj_create(lv_scr_act());
  lv_obj_remove_style_all(ovlPanel);
  lv_obj_set_size(ovlPanel, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_align(ovlPanel, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(ovlPanel, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(ovlPanel, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(ovlPanel, 150, LV_PART_MAIN | LV_STATE_DEFAULT);

  // active upper part / colorBg
  _ovlPanel2 = lv_obj_create(ovlPanel);
  lv_obj_remove_style_all(_ovlPanel2);
  lv_obj_set_size(_ovlPanel2, SCREEN_WIDTH, SCREEN_HEIGHT);
  lv_obj_set_align(_ovlPanel2, LV_ALIGN_TOP_MID);
  lv_obj_clear_flag(_ovlPanel2, LV_OBJ_FLAG_SCROLLABLE);
  lv_obj_set_style_bg_color(_ovlPanel2, colorBg, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // back button closes overlay
  _btnExit = lv_btn_create(_ovlPanel2);
  lv_obj_set_size(_btnExit, 80, 50);
  lv_obj_align(_btnExit, LV_ALIGN_BOTTOM_LEFT, 5, -5);
  lv_obj_set_style_bg_color(_btnExit, lv_color_hex(0xffffff), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_btnExit, WP_OPA_BG_OFF, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_t *label = lv_label_create(_btnExit);
  lv_label_set_text(label, LV_SYMBOL_LEFT);
  lv_obj_align(label, LV_ALIGN_CENTER, 0, 0);

  lv_obj_add_event_cb(_btnExit, _exitButtonEventHandler, LV_EVENT_CLICKED, this);

  // configure the color wheel
  lv_obj_t *_panel = _ovlPanel2;
  _colorWheel = lv_colorwheel_create(_panel, true);
  lv_obj_set_size(_colorWheel, 220, 220);
  lv_obj_align(_colorWheel, LV_ALIGN_TOP_LEFT, 15, 15);
  lv_obj_set_style_arc_width(_colorWheel, 50, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_colorwheel_set_mode(_colorWheel, LV_COLORWHEEL_MODE_HUE);
  lv_colorwheel_set_mode_fixed(_colorWheel, true);

  _labelRed = lv_label_create(_panel);
  lv_obj_set_size(_labelRed, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelRed, "0");
  lv_obj_align_to(_labelRed, _colorWheel, LV_ALIGN_OUT_BOTTOM_MID, -60, 25);

  _labelGreen = lv_label_create(_panel);
  lv_obj_set_size(_labelGreen, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelGreen, "0");
  lv_obj_align_to(_labelGreen, _colorWheel, LV_ALIGN_OUT_BOTTOM_MID, 0, 25);

  _labelBlue = lv_label_create(_panel);
  lv_obj_set_size(_labelBlue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelBlue, "0");
  lv_obj_align_to(_labelBlue, _colorWheel, LV_ALIGN_OUT_BOTTOM_MID, 60, 25);

  _labelColorTemp = lv_label_create(_panel);
  lv_obj_set_size(_labelColorTemp, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelColorTemp, "0");
  lv_obj_align(_labelColorTemp,  LV_ALIGN_BOTTOM_RIGHT, -80, -25);

  _labelBrightness = lv_label_create(_panel);
  lv_obj_set_size(_labelBrightness, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelBrightness, "0");
  lv_obj_align(_labelBrightness, LV_ALIGN_BOTTOM_RIGHT, -80, -45);

  // ui_Bar1

  _barCTlow = lv_bar_create(_panel);
  lv_bar_set_range(_barCTlow, 0, 100);
  lv_bar_set_value(_barCTlow, 25, LV_ANIM_OFF);

  lv_obj_set_width(_barCTlow, 137);
  lv_obj_set_height(_barCTlow, 40);

  lv_obj_set_x(_barCTlow, -47);
  lv_obj_set_y(_barCTlow, 150);

  lv_obj_set_align(_barCTlow, LV_ALIGN_CENTER);

  lv_obj_set_style_radius(_barCTlow, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barCTlow, lv_color_hex(0xc68935), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barCTlow, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barCTlow, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barCTlow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barCTlow, 200, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barCTlow, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_barCTlow, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barCTlow, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _barCThigh

  _barCThigh = lv_bar_create(_panel);
  lv_bar_set_range(_barCThigh, 0, 100);
  lv_bar_set_value(_barCThigh, 25, LV_ANIM_OFF);

  lv_obj_set_width(_barCThigh, 137);
  lv_obj_set_height(_barCThigh, 40);

  lv_obj_set_x(_barCThigh, 57);
  lv_obj_set_y(_barCThigh, 150);

  lv_obj_set_align(_barCThigh, LV_ALIGN_CENTER);

  lv_obj_set_style_radius(_barCThigh, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barCThigh, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barCThigh, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barCThigh, lv_color_hex(0x2493DD), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barCThigh, 30, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barCThigh, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barCThigh, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_barCThigh, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barCThigh, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _sliderCT

  _sliderCT = lv_slider_create(_panel);
  lv_slider_set_range(_sliderCT, 2000, 6000);
  lv_slider_set_value(_sliderCT, 4000, LV_ANIM_OFF);
  if (lv_slider_get_mode(_sliderCT) == LV_SLIDER_MODE_RANGE)
    lv_slider_set_left_value(_sliderCT, 0, LV_ANIM_OFF);

  lv_obj_set_width(_sliderCT, 215);
  lv_obj_set_height(_sliderCT, 35);

  lv_obj_set_x(_sliderCT, 6);
  lv_obj_set_y(_sliderCT, 150);

  lv_obj_set_align(_sliderCT, LV_ALIGN_CENTER);

  lv_obj_set_style_bg_color(_sliderCT, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderCT, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderCT, lv_color_hex(0xC8FFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderCT, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderCT, lv_color_hex(0x9C9999), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderCT, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderCT, lv_color_hex(0x3C3C3C), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderCT, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderCT, 4, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);

  // _barBrightness

  _barBrightness = lv_bar_create(_panel);
  lv_bar_set_range(_barBrightness, 0, 100);
  lv_bar_set_value(_barBrightness, 25, LV_ANIM_OFF);

  lv_obj_set_width(_barBrightness, 243);
  lv_obj_set_height(_barBrightness, 40);

  lv_obj_set_x(_barBrightness, 7);
  lv_obj_set_y(_barBrightness, 90);

  lv_obj_set_align(_barBrightness, LV_ALIGN_CENTER);

  lv_obj_set_style_radius(_barBrightness, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barBrightness, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barBrightness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barBrightness, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barBrightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barBrightness, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barBrightness, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_barBrightness, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barBrightness, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _sliderBrightness

  _sliderBrightness = lv_slider_create(_panel);
  lv_slider_set_range(_sliderBrightness, 0, 100);
  lv_slider_set_value(_sliderBrightness, 50, LV_ANIM_OFF);
  if (lv_slider_get_mode(_sliderBrightness) == LV_SLIDER_MODE_RANGE)
    lv_slider_set_left_value(_sliderBrightness, 0, LV_ANIM_OFF);

  lv_obj_set_width(_sliderBrightness, 215);
  lv_obj_set_height(_sliderBrightness, 35);

  lv_obj_set_x(_sliderBrightness, 6);
  lv_obj_set_y(_sliderBrightness, 90);

  lv_obj_set_align(_sliderBrightness, LV_ALIGN_CENTER);

  lv_obj_set_style_bg_color(_sliderBrightness, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderBrightness, lv_color_hex(0xC8FFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightness, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderBrightness, lv_color_hex(0x9C9999), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightness, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderBrightness, lv_color_hex(0x3C3C3C), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderBrightness, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderBrightness, 4, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);

  // _barLuminanz

  _barLuminanz = lv_bar_create(_panel);
  lv_bar_set_range(_barLuminanz, 0, 100);
  lv_bar_set_value(_barLuminanz, 25, LV_ANIM_OFF);

  lv_obj_set_width(_barLuminanz, 40);
  lv_obj_set_height(_barLuminanz, 220);

  lv_obj_set_x(_barLuminanz, -20);
  lv_obj_set_y(_barLuminanz, 15);

  lv_obj_set_align(_barLuminanz, LV_ALIGN_TOP_RIGHT);

  lv_obj_set_style_radius(_barLuminanz, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barLuminanz, lv_color_hex(0xF6FF2D), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barLuminanz, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barLuminanz, lv_color_hex(0x000000), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barLuminanz, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barLuminanz, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barLuminanz, LV_GRAD_DIR_VER, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_barLuminanz, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barLuminanz, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _sliderLuminanz

  _sliderLuminanz = lv_slider_create(_panel);
  lv_slider_set_range(_sliderLuminanz, 0, 100);
  lv_slider_set_value(_sliderLuminanz, 100, LV_ANIM_OFF);

  lv_obj_set_width(_sliderLuminanz, 35);
  lv_obj_set_height(_sliderLuminanz, 180);

  lv_obj_set_x(_sliderLuminanz, -23);
  lv_obj_set_y(_sliderLuminanz, 30);

  lv_obj_set_align(_sliderLuminanz, LV_ALIGN_TOP_RIGHT);

  lv_obj_set_style_bg_color(_sliderLuminanz, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderLuminanz, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderLuminanz, lv_color_hex(0xC8FFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderLuminanz, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderLuminanz, lv_color_hex(0x9C9999), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderLuminanz, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderLuminanz, lv_color_hex(0x3C3C3C), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderLuminanz, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderLuminanz, 4, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderLuminanz, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderLuminanz, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderLuminanz, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderLuminanz, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
}

void classColorPicker::_exitButtonEventHandler(lv_event_t *e)
{
  lv_obj_t *btn = lv_event_get_target(e);
  lv_obj_t *panel2 = lv_obj_get_parent(btn);
  lv_obj_t *panel1 = lv_obj_get_parent(panel2);
  lv_obj_del(panel1);
}

classColorPicker::classColorPicker(classTile *tile, lv_event_cb_t colorPickerEventHandler)
{
  // layout the drop down overlay
  _createColorPicker();
  _callingTile = tile;

  // add event handler
  lv_obj_add_event_cb(_colorWheel, colorPickerEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_sliderLuminanz, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);
  lv_obj_add_event_cb(_sliderCT, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);
  lv_obj_add_event_cb(_sliderBrightness, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);
}

void classColorPicker::updateRGB(void)
{
  lv_color_t color = lv_colorwheel_get_rgb(_colorWheel);
  int luminanz = lv_slider_get_value(_sliderLuminanz);
  lv_label_set_text_fmt(_labelRed, "%d", (((color.ch.red * 255) / 31) * luminanz) / 100);
  lv_label_set_text_fmt(_labelGreen, "%d", (((color.ch.green * 255) / 63) * luminanz) / 100);
  lv_label_set_text_fmt(_labelBlue, "%d", (((color.ch.blue * 255) / 31) * luminanz) / 100);

  lv_obj_set_style_bg_color(_barLuminanz, color, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_label_set_text_fmt(_labelColorTemp, "%d", lv_slider_get_value(_sliderCT));
  lv_label_set_text_fmt(_labelBrightness, "%d", lv_slider_get_value(_sliderBrightness));
}

// get reference of calling tile
classTile* classColorPicker::getTile(void)
{
  return _callingTile;
}



