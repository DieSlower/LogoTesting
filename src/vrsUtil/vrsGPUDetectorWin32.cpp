/*
* //////////////////////////////////////C++//////////////////////////////////////
* 
* Virtual Reality Simulation Lab
* 
* This file is part of Virtual Reality Simulation Lab software. You can redistribute 
* it and/or modify it only with the authors permission. All unlicensed use and modification
* of this file are prohibited. 
* 
* This does not restrict the user from creating custom modules and additional files to 
* interface with the VR-Sim Lab.
* 
* This software was developed by Maxim Serebrennik for Various projects he worked on.
* It is sold and distributed only with his permission. 
* 
* For more information e-mail him at MaximSter@gmail.com
* 
* @author Maxim Serebrennik 
*////////////////////////////////////////////////////////////////////////////////
#ifndef vrsGPUDetectorWin32_CPP
#define vrsGPUDetectorWin32_CPP 1

#ifdef _WIN32
#include <vrsUtil\vrsGPUDetector.h>

#include <windows.h>
#include <iostream>

#include <osgViewer/CompositeViewer>

namespace vrsUtil
{
	void vrsGPUDetector::DetectGPUMonitorConfigWin32(void)
	{
		DISPLAY_DEVICE GCardDevice;
		GCardDevice.cb = sizeof(DISPLAY_DEVICE);
		
		//Check all the devices with attached monitors. 
		DWORD deviceNum = 0;
		int KeyStart = 0;
		int KeyEnd = 0;
		while( EnumDisplayDevices(NULL, deviceNum, &GCardDevice, 0))
		{	

			//Isolate the Graphics Card Device Key
			std::string CardDeviceKey(GCardDevice.DeviceKey);
			KeyStart = CardDeviceKey.find("{");
			KeyEnd = CardDeviceKey.find("}") - KeyStart + 1;
			
			DISPLAY_DEVICE MonitorDevice = {0};
			MonitorDevice.cb = sizeof(DISPLAY_DEVICE);
			DWORD monitorNum = 0;
			while ( EnumDisplayDevices(GCardDevice.DeviceName, monitorNum, &MonitorDevice, 0))
			{
				//Fill out Screen Data (Set all GPU IDs to 0)				
				ScrGPUCfg ScrCfg;
				ScrCfg.GPUNum = 0;
				ScrCfg.ScrNum = mScrGPUConfig.size();
				ScrCfg.GCardID = CardDeviceKey.substr(KeyStart, KeyEnd);
				mScrGPUConfig.push_back(ScrCfg);

				monitorNum++;
			}
			deviceNum++;
		}

		//Compare the GCard IDs, and label the corect GPU IDs
		for (unsigned int i=0; i<mScrGPUConfig.size(); i++)
		{
			for (unsigned int j=i+1; j<mScrGPUConfig.size(); j++)
			{
				if (mScrGPUConfig[j].GCardID == mScrGPUConfig[i].GCardID)
				{
					mScrGPUConfig[j].GPUNum = mScrGPUConfig[i].GPUNum;
				}	
				else
				{
					mScrGPUConfig[j].GPUNum = (mScrGPUConfig[i].GPUNum + 1);
				}
			}
		}
	}
	#endif
}

#endif // vrsGPUDetectorWin32_CPP