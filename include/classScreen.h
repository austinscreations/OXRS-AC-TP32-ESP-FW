#pragma once
#include <lvgl.h>

class classScreen
{
private:
  lv_obj_t *_parent = NULL;
  lv_obj_t *_labelFooter = NULL;
  lv_obj_t *_labelWarning = NULL;
  lv_obj_t *_btnHome = NULL;
  lv_obj_t *_btnHomeImg;
  lv_obj_t *_btnSettings = NULL;
  lv_obj_t *_btnSettingsImg = NULL;
  lv_obj_t *_btnFooter = NULL;

public:
  int screenIdx;
  char screenLabel[32];

  lv_obj_t *screen = NULL;
  lv_obj_t *container = NULL;

  classScreen(void){};
  classScreen(int screenIdx, int style);
  
  void setLabel(const char *labelText);
  const char *getLabel(void);
  void setFooter(const char *footerText);
  const char *getFooter(void);

  void updateBgColor(void);
  void createHomeButton(lv_event_cb_t callBack, const void *img);
  void createSettingsButton(lv_event_cb_t callBack, const void *img);
  void showConnectionStatus(bool connected);
  
  int getScreenNumber(void);
 
  void adWipeEventHandler(lv_event_cb_t callBack);
  void adScreenEventHandler(lv_event_cb_t callBack);
};
