/*********************************************************************************
 *  MIT License
 *  
 *  Copyright (c) 2023 Gregg E. Berman
 *  
 *  https://github.com/HomeSpan/HomeStep
 *  
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to deal
 *  in the Software without restriction, including without limitation the rights
 *  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *  copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *  
 *  The above copyright notice and this permission notice shall be included in all
 *  copies or substantial portions of the Software.
 *  
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 *  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 *  SOFTWARE.
 *  
 ********************************************************************************/
 
#pragma once

#include <Arduino.h>

[[maybe_unused]] static const char* TAG = "StepperControl";

//////////////////////////

class StepperControl {

  public:

  enum {
    FULL_STEP_ONE_PHASE=0,
    FULL_STEP_TWO_PHASE=1,
    HALF_STEP=2,
    QUARTER_STEP=4,
    EIGHTH_STEP=8
  };    
  
  enum endAction_t {
    NONE,
    DISABLE,
    BRAKE
  };

  private:
  
  struct upLink_t {
    int nSteps;
    uint32_t msDelay;
    boolean absoluteStep;
    endAction_t endAction;
  };

  struct downLink_t {
    int stepsRemaining;
    int position;
  };
  
  float accelSteps=1;
  float accelSize=0;
  downLink_t downLinkData = { .stepsRemaining=0, .position=0 };

  TaskHandle_t motorTaskHandle;
  QueueHandle_t upLinkQueue;
  QueueHandle_t downLinkQueue;

  virtual void onStep(boolean direction)=0;
  virtual void onEnable(){};
  virtual void onDisable(){};
  virtual void onBrake(){};
  static void motorTask(void *args);

  public:

  StepperControl(uint32_t priority=2, uint32_t cpu=0);
  virtual void setStepType(int mode){};
  void setAccel(float accelSize, float accelSteps);
  void move(int nSteps, uint32_t msDelay, endAction_t endAction=NONE);
  void moveTo(int nPosition, uint32_t msDelay, endAction_t endAction=NONE);
  int position();
  int stepsRemaining();
  void enable();
  void disable();
  void brake();
};

//////////////////////////