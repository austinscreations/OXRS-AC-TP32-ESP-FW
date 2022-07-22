#pragma once
#include <lvgl.h>
#include <classTile.h>

class classColorPicker

{
private:
  classTile* _callingTile = NULL;
  lv_obj_t* _parent = NULL;
  lv_obj_t* _ovlPanel2 = NULL;
  lv_obj_t* _btnExit = NULL;
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
  lv_obj_t* ovlPanel = NULL;

  classColorPicker(void){};
  classColorPicker(classTile* tile, lv_event_cb_t colorPickerEventHandler);
  classTile* getTile(void);
  void updateAll(void);
  void setRGB(int red, int green, int blue);

};
