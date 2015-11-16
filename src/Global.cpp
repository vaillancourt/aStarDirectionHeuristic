/**
 * Copyright (c) 2015 Alexandre Vaillancourt. See full MIT license at the root of the repository.
 */
#include "Global.h"

#include <iostream>

bool Global::IsFromTerminate = false;
std::unique_ptr<Global> Global::Instance;

Global::Global()
{}

void 
Global::InitInstance()
{
  if ( Instance )
    std::cout << __FUNCTION__ <<  " already called." << std::endl;
  else
    Instance = std::make_unique<Global>();
}

void 
Global::TerminateInstance()
{
  IsFromTerminate = true;
  Instance.reset();
}

Global& 
Global::GetInstance()
{
  return *(Instance.get());
}

Global::~Global()
{
  if ( !IsFromTerminate )
    std::cout << "Kill the Global singleton via Global::TerminateInstance()" << std::endl;
}
