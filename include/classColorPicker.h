#pragma once
#include <lvgl.h>
#include <classTile.h>

class classColorPicker

{
private:
  classTile* _callingTile = NULL;
  lv_obj_t* _parent = NULL;
  lv_obj_t *_ovlPanel = NULL;
  lv_obj_t *_panelRGB = NULL;
  lv_obj_t *_panelCCT = NULL;
  lv_obj_t *_btnExit = NULL;
  lv_obj_t *_colorWheel = NULL;
  lv_obj_t *_labelRed = NULL;
  lv_obj_t *_labelGreen = NULL;
  lv_obj_t *_labelBlue = NULL;
  lv_obj_t *_labelColorTemp = NULL;
  lv_obj_t *_labelBrightness = NULL;
  lv_obj_t *_barCTlow = NULL;
  lv_obj_t *_barCThigh = NULL;
  lv_obj_t *_sliderCT = NULL;
  lv_obj_t *_barBrightness = NULL;
  lv_obj_t *_sliderBrightness = NULL;

  void _createColorPicker(void);
  static void _exitButtonEventHandler(lv_event_t *e);

public:

  classColorPicker(void){};
  classColorPicker(classTile* tile, lv_event_cb_t colorPickerEventHandler, bool enRGB, bool enCCT);
  classTile* getTile(void);
  void updateAll(void);

};
