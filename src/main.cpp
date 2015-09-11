//
// This file is part of the GNU ARM Eclipse distribution.
// Copyright (c) 2014 Liviu Ionescu.
//

// ----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "cmsis_device.h"
#include "f10x-pcd8544.h"
#include "GY521Sensor.h"
// ----------------------------------------------------------------------------
//
// Standalone STM32F1 empty sample (trace via DEBUG).
//
// Trace support is enabled by adding the TRACE macro definition.
// By default the trace messages are forwarded to the DEBUG output,
// but can be rerouted to any device or completely suppressed, by
// changing the definitions required in system/src/diag/trace_impl.c
// (currently OS_USE_TRACE_ITM, OS_USE_TRACE_SEMIHOSTING_DEBUG/_STDOUT).
//

// ----- main() ---------------------------------------------------------------

// Sample pragmas to cope with warnings. Please note the related line at
// the end of this function, used to pop the compiler diagnostics status.
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#include "Viewer.h"
#include "GY521Sensor.h"

Viewer view;

int
main(int argc, char* argv[])
{
  // At this stage the system clock should have already been configured
  // at high speed.
	GY521Sensor sensor(I2C1);
	float x,y,z,temp;
	x=y=z=temp=0.0;

	view.Init();
	if(!sensor.Init())
		return 0;
	while (1)
    {
		sensor.ReadData();
		view.SetData(x,y,z,sensor.Temperature());
    }
}

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
