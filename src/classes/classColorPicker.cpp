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
  lv_obj_set_style_bg_color(_ovlPanel2, lv_color_lighten(colorBg, 50) , LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_ovlPanel2, 255, LV_PART_MAIN | LV_STATE_DEFAULT);

  // back button closes overlay
  _btnExit = lv_btn_create(_ovlPanel2);
  lv_obj_set_size(_btnExit, 80, 50);
  lv_obj_align(_btnExit, LV_ALIGN_BOTTOM_LEFT, 15, -5);
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
  lv_obj_set_style_arc_width(_colorWheel, 40, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_colorwheel_set_mode(_colorWheel, LV_COLORWHEEL_MODE_HUE);

  _labelRed = lv_label_create(_panel);
  lv_obj_set_size(_labelRed, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelRed, "0");
  lv_obj_align(_labelRed, LV_ALIGN_TOP_LEFT, 260, 170);

  _labelGreen = lv_label_create(_panel);
  lv_obj_set_size(_labelGreen, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelGreen, "0");
  lv_obj_align(_labelGreen, LV_ALIGN_TOP_LEFT, 260, 190);

  _labelBlue = lv_label_create(_panel);
  lv_obj_set_size(_labelBlue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelBlue, "0");
  lv_obj_align(_labelBlue,  LV_ALIGN_TOP_LEFT, 260, 210);

  _labelColorTemp = lv_label_create(_panel);
  lv_obj_set_size(_labelColorTemp, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelColorTemp, "4000 K");
  lv_obj_align(_labelColorTemp, LV_ALIGN_RIGHT_MID, -20, 115);

  _labelBrightness = lv_label_create(_panel);
  lv_obj_set_size(_labelBrightness, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelBrightness, "100 %");
  lv_obj_align(_labelBrightness, LV_ALIGN_RIGHT_MID, -20, 40);

  // ui_Bar1

  _barCTlow = lv_bar_create(_panel);
  lv_bar_set_range(_barCTlow, 0, 100);

  lv_obj_set_size(_barCTlow, 163, 40);
  lv_obj_align(_barCTlow, LV_ALIGN_LEFT_MID, 15, 145);

  lv_obj_set_style_radius(_barCTlow, 20, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_color(_barCTlow, lv_color_hex(0xFA9C19), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barCTlow, 255, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_color(_barCTlow, lv_color_hex(0xFFFFFF), LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_main_stop(_barCTlow, 0, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_stop(_barCTlow, 220, LV_PART_MAIN | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_grad_dir(_barCTlow, LV_GRAD_DIR_HOR, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_barCTlow, lv_color_hex(0xFFFFFF), LV_PART_INDICATOR | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_barCTlow, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  // _barCThigh

  _barCThigh = lv_bar_create(_panel);
  lv_bar_set_range(_barCThigh, 0, 100);

  lv_obj_set_size(_barCThigh, 163, 40);
  lv_obj_align(_barCThigh, LV_ALIGN_RIGHT_MID, -15, 145);

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

  lv_obj_set_size(_sliderCT, 260, 35);
  lv_obj_align(_sliderCT, LV_ALIGN_LEFT_MID, 30, 145);

  lv_obj_set_style_bg_opa(_sliderCT, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_opa(_sliderCT, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderCT, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderCT, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderCT, lv_color_hex(0xE6E6E6), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderCT, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderCT, 4, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_color(_sliderCT, lv_color_hex(0x1E1E1E), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(_sliderCT, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(_sliderCT, 3, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(_sliderCT, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderCT, -5, LV_PART_KNOB | LV_STATE_DEFAULT);

  // _barBrightness

  _barBrightness = lv_bar_create(_panel);
  lv_bar_set_range(_barBrightness, 0, 100);

  lv_obj_set_size(_barBrightness, 290, 40);
  lv_obj_align(_barBrightness, LV_ALIGN_LEFT_MID, 15, 70);

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

  lv_obj_set_size(_sliderBrightness, 260, 35);
  lv_obj_align(_sliderBrightness, LV_ALIGN_CENTER, 0, 70);

  lv_obj_set_style_bg_opa(_sliderBrightness, 0, LV_PART_MAIN | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_opa(_sliderBrightness, 0, LV_PART_INDICATOR | LV_STATE_DEFAULT);

  lv_obj_set_style_bg_color(_sliderBrightness, lv_color_hex(0xFFFFFF), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_bg_opa(_sliderBrightness, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_color(_sliderBrightness, lv_color_hex(0xE6E6E6), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_opa(_sliderBrightness, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_border_width(_sliderBrightness, 4, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_color(_sliderBrightness, lv_color_hex(0x1E1E1E), LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_opa(_sliderBrightness, 255, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_width(_sliderBrightness, 3, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_outline_pad(_sliderBrightness, 0, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_left(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_right(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_top(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
  lv_obj_set_style_pad_bottom(_sliderBrightness, -5, LV_PART_KNOB | LV_STATE_DEFAULT);
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
  // layout the color picker pop up
  _createColorPicker();

  // update controls with stored values from calling tile
  _callingTile = tile;
  lv_colorwheel_set_rgb(_colorWheel, _callingTile->getColorPickerRGB());
  lv_slider_set_value(_sliderCT, _callingTile->getColorPickerCCT(), LV_ANIM_OFF);
  lv_slider_set_value(_sliderBrightness, _callingTile->getColorPickerBrightness(), LV_ANIM_OFF);

  // add event handler
  lv_obj_add_event_cb(_colorWheel, colorPickerEventHandler, LV_EVENT_ALL, _callingTile);
  lv_obj_add_event_cb(_sliderCT, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);
  lv_obj_add_event_cb(_sliderBrightness, colorPickerEventHandler, LV_EVENT_VALUE_CHANGED, _callingTile);
}

void classColorPicker::updateAll(void)
{
  lv_color_t color = lv_colorwheel_get_rgb(_colorWheel);
  lv_color32_t color32;
  color32.full = lv_color_to32(color);
  lv_label_set_text_fmt(_labelRed, "r: %3d", color32.ch.red);
  lv_label_set_text_fmt(_labelGreen, "g: %3d", color32.ch.green);
  lv_label_set_text_fmt(_labelBlue, "b: %3d", color32.ch.blue);

  lv_label_set_text_fmt(_labelColorTemp, "%d K", lv_slider_get_value(_sliderCT));
  lv_label_set_text_fmt(_labelBrightness, "%d %%", lv_slider_get_value(_sliderBrightness));

  _callingTile->setColorPickerRGB(color);
  _callingTile->setColorPickerCCT(lv_slider_get_value(_sliderCT));
  _callingTile->setColorPickerBrightness(lv_slider_get_value(_sliderBrightness));
}

// get reference of calling tile
classTile *classColorPicker::getTile(void)
{
    return _callingTile;
}



