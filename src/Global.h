/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#pragma once

#define FRAME_RATE 60
#define FRAME_DURATION_MS (1000 / FRAME_RATE)

#define COLOUR_BACKGROUND 127, 255, 127
#define COLOUR_FULL_DIRTY   0,   0,   0
#define COLOUR_ROAD       200, 200, 200
#define COLOUR_POLICE_R   255,   0,   0
#define COLOUR_POLICE_B     0,   0, 255

#include <memory>

class Global
{
  Global(const Global& that) = delete;
  Global& operator=(const Global&) = delete;
public:
  static void InitInstance();
  static void TerminateInstance();
  static Global& GetInstance();

  int getWindowWidth() const { return 256; }
  int getWindowHeight() const { return 192; }

  ~Global();
  Global();

private:
  static std::unique_ptr<Global> Instance;
  static bool                    IsFromTerminate;;
};