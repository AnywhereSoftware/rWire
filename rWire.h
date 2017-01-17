#pragma once
#include "B4RDefines.h"
//~dependson: <Wire.h>
namespace B4R {
	//~Version: 1.10
	//~shortname: WireMaster
	#define WIRE_MAX_SIZE 32
	class WireMaster {
		private:
			Byte bufferData[WIRE_MAX_SIZE];
			ArrayByte buffer;
		public:
			//Initializes the WireMaster object.
			void Initialize();
			//Requests data from a slave and returns it as an array of bytes.
			//This will return the data prepared by the slave with the call to SetDataForMaster.
			//Note that the returned array length might be smaller than the requested length.
			//Address - Slave address.
			//Length - Number of bytes requested.
			ArrayByte* RequestFrom(Byte Address, Byte Length);
			//Sends the given array to the slave.
			Byte WriteTo(Byte Address, ArrayByte* Data);
			//Similar to WriteTo.
			//SendStop - Whether to send a stop at the end (default is true).
			Byte WriteTo2(Byte Address, bool SendStop, ArrayByte* Data);
	};
	typedef Array* (*SubArrayVoid)(void) ;
	//~shortname: WireSlave
	//~event: NewData (Data() As Byte)
	class WireSlave {
		private:
			Byte bufferData[WIRE_MAX_SIZE];
			ArrayByte buffer;
			Byte rcvBufferData[WIRE_MAX_SIZE];
			ArrayByte rcvBuffer;
			static WireSlave* instance;
			SubVoidArray NewDataSub;
			static void requestEvent();
			static void receiveEvent(int);
			static void receiveEventAfterISR();
		public:
			//Initializes the WireSlave object.
			//Address - The slave address. Must be between 8 to 127.
			//NewDataSub - Sub that will handle the NewData event.
			void Initialize(Byte Address, SubVoidArray NewDataSub);
			//Sets the data that will be sent to the master when the master will call RequestFrom.
			void SetDataForMaster(ArrayByte* Data);
			
	};
	
	
	
	
}