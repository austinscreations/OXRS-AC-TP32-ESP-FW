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
  lv_obj_set_size(_ovlPanel2, SCREEN_WIDTH, SCREEN_HEIGHT - 35);
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

  lv_obj_add_event_cb(_btnExit, closeEvt, LV_EVENT_CLICKED, this);

  // configure the color wheel
  lv_obj_t *_panel = _ovlPanel2;
  _colorWheel = lv_colorwheel_create(_panel, true);
  lv_obj_set_size(_colorWheel, 250, 250);
  lv_obj_align(_colorWheel, LV_ALIGN_TOP_MID, 0, 35);
  lv_obj_set_style_arc_width(_colorWheel, 60, LV_PART_MAIN | LV_STATE_DEFAULT);

  _labelRed = lv_label_create(_panel);
  lv_obj_set_size(_labelRed, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelRed, "0");
  lv_obj_align_to(_labelRed, _colorWheel, LV_ALIGN_OUT_BOTTOM_MID, -60, 50);

  _labelGreen = lv_label_create(_panel);
  lv_obj_set_size(_labelGreen, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelGreen, "0");
  lv_obj_align_to(_labelGreen, _colorWheel, LV_ALIGN_OUT_BOTTOM_MID, 0, 50);

  _labelBlue = lv_label_create(_panel);
  lv_obj_set_size(_labelBlue, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelBlue, "0");
  lv_obj_align_to(_labelBlue, _colorWheel, LV_ALIGN_OUT_BOTTOM_MID, 60, 50);

  _labelMode = lv_label_create(_panel);
  lv_obj_set_size(_labelMode, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
  lv_label_set_text(_labelMode, "");
  lv_obj_align_to(_labelMode, _colorWheel, LV_ALIGN_OUT_BOTTOM_MID, -60, 35);
}

classColorPicker::classColorPicker(classTile *tile, lv_event_cb_t colorPickerEventHandler)
{
  // layout the drop down overlay
  _createColorPicker();
  _callingTile = tile;

  // add event handler
  lv_obj_add_event_cb(_colorWheel, colorPickerEventHandler, LV_EVENT_ALL, _callingTile);
}

void classColorPicker::updateRGB(void)
{
  lv_color_t color = lv_colorwheel_get_rgb(_colorWheel);
  lv_label_set_text_fmt(_labelRed, "%d", (color.ch.red * 255) / 31);
  lv_label_set_text_fmt(_labelGreen, "%d", (color.ch.green * 255) / 63);
  lv_label_set_text_fmt(_labelBlue, "%d", (color.ch.blue * 255) / 31);

  lv_colorwheel_mode_t mode = lv_colorwheel_get_color_mode(_colorWheel);
  char str_mode[32];
  strcpy(str_mode, "Mode : ");
  switch (mode)
  {
    case LV_COLORWHEEL_MODE_HUE: strcat(str_mode, "Color"); break;
    case LV_COLORWHEEL_MODE_SATURATION: strcat(str_mode, "Saturation"); break;
    case LV_COLORWHEEL_MODE_VALUE: strcat(str_mode, "Brightness"); break;
    default : strcat(str_mode, "unknown"); break;
  }
  lv_label_set_text(_labelMode, str_mode);
}

void classColorPicker::closeEvt(lv_event_t *e)
{
  lv_obj_t *btn = lv_event_get_target(e);
  lv_obj_t *panel2 = lv_obj_get_parent(btn);
  lv_obj_t *panel1 = lv_obj_get_parent(panel2);
  lv_obj_del(panel1);
}

// get reference of calling tile
classTile* classColorPicker::getTile(void)
{
  return _callingTile;
}



