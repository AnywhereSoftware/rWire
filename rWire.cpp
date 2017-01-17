#include "B4RDefines.h"
namespace B4R {
	void WireMaster::Initialize() {
		buffer.data = bufferData;
		Wire.begin();
	}
	ArrayByte* WireMaster::RequestFrom(Byte Address, Byte Length) {
		Byte len = Wire.requestFrom(Address, Length);
		for (byte i = 0;i < len;i++)
			((Byte*)buffer.data)[i] = Wire.read();
		buffer.length = len;
		return &buffer;
	}
	Byte WireMaster::WriteTo2(Byte Address, bool SendStop, ArrayByte* Data) {
		Wire.beginTransmission(Address);
		Byte b = Wire.write((Byte*)Data->data, Data->length);
		Wire.endTransmission(SendStop);
		return b;
	}
	Byte WireMaster::WriteTo(Byte Address, ArrayByte* Data) {
		return WriteTo2(Address, true, Data);
	}
	
	
	
	WireSlave* WireSlave::instance = NULL;
	void WireSlave::Initialize(Byte Address, SubVoidArray NewDataSub) {
		instance = this;
		buffer.data = bufferData;
		buffer.length = 0;
		rcvBuffer.data = rcvBufferData;
		this->NewDataSub = NewDataSub;
		Wire.onReceive(receiveEvent);
		Wire.onRequest(requestEvent);
		Wire.begin(Address);
	}
	void WireSlave::SetDataForMaster(ArrayByte* Data) {
		int len = Common_Min(WIRE_MAX_SIZE, Data->length);
		memcpy(buffer.data, Data->data, len);
		buffer.length = len;
	}
	void WireSlave::requestEvent() {
		Wire.write((Byte*)instance->buffer.data, instance->buffer.length);
	}
	void WireSlave::receiveEvent(int length) {
		for (int i = 0;i < length;i++)
			((Byte*)instance->rcvBuffer.data)[i] = Wire.read();
		instance->rcvBuffer.length = length;
		pollers.setInterrupt(receiveEventAfterISR);
	}
	void WireSlave::receiveEventAfterISR() {
		WireSlave* me = instance;
		me->NewDataSub(&me->rcvBuffer);
	}
}