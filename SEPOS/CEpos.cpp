#include "pch.h"
#include "CEpos.h"
#include "Definitions.h"
#include <cmath>

// Node mapping:
const WORD	 CEpos::_node[_nNodes] = { 1 };
// Analog input mapping:
const WORD	 CEpos::_aI[_nNodes] = { 1 };
// NOTE: secondary home position stored as member in CEpos (declared in CEpos.h)

CEpos::CEpos()
{
	_KeyHandle = NULL;
	_ulErrorCode = 0;
}
CEpos::~CEpos()
{

}

bool CEpos::Initialize()
{
	// Connect to EPOS:
	//-----------------------------------------------------------------------------------------------

	//Close Previous Device
	if (_KeyHandle) {
		VCS_CloseDevice(_KeyHandle, &_ulErrorCode);
		_KeyHandle = 0;
	}

	// Open device
	char deviceName[] = "EPOS4";
	char protocalStackName[] = "MAXON SERIAL V2";
	char interfaceName[] = "USB";
	char portName[] = "USB0";

	// Open EPOS with local handle, otherwise does not die clean if not done
	_KeyHandle = VCS_OpenDevice(deviceName, protocalStackName, interfaceName, portName, &_ulErrorCode);
	if (!_KeyHandle) {
		AfxMessageBox(L"Can't open EPOS !\nCheck power and restart", MB_ICONEXCLAMATION | MB_OK);
		return false;
		// If not responding the program does not close clean
	}

	// Select maximum baudrate
	DWORD baudrateSel;
	BOOL endOfSelection = FALSE;
	if (VCS_GetBaudrateSelection(deviceName, protocalStackName, interfaceName, portName, TRUE, &baudrateSel, &endOfSelection, &_ulErrorCode)) {
		while (!endOfSelection) {
			VCS_GetBaudrateSelection(deviceName, protocalStackName, interfaceName, portName, TRUE, &baudrateSel, &endOfSelection, &_ulErrorCode);
		}
	}

	// Set baudrate and timeout
	if (!VCS_SetProtocolStackSettings(_KeyHandle, baudrateSel/*(DWORD)1000000*/, (DWORD)500, &_ulErrorCode)) {
		AfxMessageBox(L"VCS_SetProtocolStackSettings !", MB_ICONEXCLAMATION | MB_OK);
		return false;
	}

	//  VCS_ClearFault changes the device state from “fault” to “disable”.
	//if (!VCS_ClearFault(_KeyHandle, _Node, &_ulErrorCode)) {
	//	AfxMessageBox(L"VCS_ClearFault !", MB_ICONEXCLAMATION | MB_OK);
	//	return false;
	//}

	// Configure AI

	short n = 0;
//	if (!VCS_AnalogInputConfiguration(_KeyHandle, _node[n], _aI[n], AIC_GENERAL_PURPOSE_A, true, &_ulErrorCode)) {
//	if(!VCS_AnalogInputConfiguration(_KeyHandle, 1, 1,	WORD Configuration, BOOL ExecutionMask, DWORD * pErrorCode))
//		AfxMessageBox(L"VCS_AnalogInputConfiguration !", MB_ICONEXCLAMATION | MB_OK);
//	}

	//Configure Velocity Profile and Activate
	VCS_ClearFault(_KeyHandle, _node[0], &_ulErrorCode);
	//VCS_ActivateProfilePositionMode(_KeyHandle, _node[0], &_ulErrorCode);
	//VCS_SetPositionProfile(_KeyHandle, _node[0], 8000, 2500, 2500, &_ulErrorCode); //8000,10000,10000
	//VCS_SetEnableState(_KeyHandle, _node[0], &_ulErrorCode);
	//VCS_MoveToPosition(_KeyHandle, _node[0], 0, true, true, &_ulErrorCode);
	//VCS_WaitForTargetReached(_KeyHandle, _node[0], 100, &_ulErrorCode);
	return true;
}

void CEpos::Close()
{
	if (!_KeyHandle)	return;
	
	// Close it:
	VCS_ClearFault(_KeyHandle, _node[0], &_ulErrorCode);
	VCS_SetQuickStopState(_KeyHandle, _node[0], &_ulErrorCode);
	Sleep(1000);
	VCS_SetDisableState(_KeyHandle, _node[0], &_ulErrorCode);
	VCS_CloseDevice(_KeyHandle, &_ulErrorCode);
	_KeyHandle = 0;
}

WORD CEpos::ReadAnalogInput(void)
{
	WORD ai;
	short n = 0;
	VCS_GetAnalogInput(_KeyHandle, _node[n], _aI[n], &ai, &_ulErrorCode);


	return ai;
}

long CEpos::ReadPosition(void)
{
	long pos;
	short n = 0;
	VCS_GetPositionIs(_KeyHandle, _node[n], &pos, &_ulErrorCode);
	//long posdeg = std::round(360 * pos / 40000);
	return pos;
}

double CEpos::ReadVelocity(void)
{
	long _vel;
	short n = 0;
	VCS_GetVelocityIsAveraged(_KeyHandle, _node[n], &_vel, &_ulErrorCode);
	double vel = _vel * pow(10, _vel_exp) * _vel_scale;
	return vel;
}

long CEpos::ReadPosition2(void)
{
	long pos;
	short n = 0;
	unsigned long dataread;
	VCS_GetObject(_KeyHandle, _node[n], 0x60E4, 0x02, &pos,4,&dataread,&_ulErrorCode);
	//long posdeg = std::round(360 * pos / 40000);
	return pos;
}

double CEpos::ReadVelocity2(void)
{
	long _vel;
	short n = 0;
	unsigned long dataread;
	VCS_GetObject(_KeyHandle, _node[n], 0x60E5, 0x0A, &_vel, 4, &dataread, &_ulErrorCode);
	double vel = _vel * pow(10, _vel_exp) * _vel_scale;
	return vel;
}

short CEpos::ReadCurrent(void)
{
	short _curr;
	short n = 0;
	VCS_GetCurrentIsAveraged(_KeyHandle, _node[n], &_curr, &_ulErrorCode);
	return _curr;
}

void CEpos::ActivatePosProfMode()
{
	short n = 0;
	VCS_ClearFault(_KeyHandle, _node[0], &_ulErrorCode);
	SetVelUnits();
	VCS_SetEnableState(_KeyHandle, _node[0], &_ulErrorCode);
	VCS_ActivateProfilePositionMode(_KeyHandle, _node[0], &_ulErrorCode);
}

void CEpos::SetPositionProfile(double _vel, double _accel, double _deccel)
{
	short n = 0;
	DWORD vel = abs(int(_vel));
	DWORD accel = abs(int(_accel));
	DWORD deccel = abs(int(_deccel));
	VCS_SetPositionProfile(_KeyHandle, _node[n], 40000*vel/360, accel/6, deccel/6, &_ulErrorCode);
}

void CEpos::SetPosition(double _posDeg)
{
	short n = 0;
	long counts = lround(40000.0 * _posDeg / 360.0);
	VCS_MoveToPosition(_KeyHandle, _node[n], counts, true, true, &_ulErrorCode);
}

// TODO Verify Acceleration Units
void CEpos::ActivateVelProfMode()
{
	short n = 0;
	VCS_ClearFault(_KeyHandle, _node[0], &_ulErrorCode);
	VCS_SetEnableState(_KeyHandle, _node[0], &_ulErrorCode);
	VCS_ActivateProfileVelocityMode(_KeyHandle, _node[0], &_ulErrorCode);
}

void CEpos::SetVelocityProfile(double _accel, double _deccel)
{
	DWORD accel = abs(int(_accel/_vel_scale));
	DWORD deccel = abs(int(_deccel/_vel_scale));
	short n = 0;
	VCS_SetVelocityProfile(_KeyHandle, _node[n], accel, deccel, &_ulErrorCode);
}

void CEpos::SetVelocityGains(double _p, double _i) {
	WORD p = int(_p * pow(10, _reg_exp));
	WORD i = int(_i * pow(10, _reg_exp));
	short n = 0;
	VCS_SetVelocityProfile(_KeyHandle, _node[n], p, i, &_ulErrorCode);

}

void CEpos::SetVelocity(double _vel)
{
	long vel = int((_vel/_vel_scale) * pow(10,-_vel_exp));
	short n = 0;
	VCS_MoveWithVelocity(_KeyHandle, _node[n], vel, &_ulErrorCode);
}

bool CEpos::SetPositionBlocking(double _posDeg)
{
	short n = 0;
	long counts = lround(_posDeg);
	// If caller passed degrees, convert to counts; if they already passed counts, they should use other overloads.
	// Here we assume degrees input to be consistent with SetPosition(double).
	counts = lround(40000.0 * _posDeg / 360.0);
	VCS_MoveToPosition(_KeyHandle, _node[n], counts, true, true, &_ulErrorCode);
	return VCS_WaitForTargetReached(_KeyHandle, _node[n], 10000, &_ulErrorCode);
}

void CEpos::Disable()
{
	short n = 0;
	VCS_SetQuickStopState(_KeyHandle, _node[0], &_ulErrorCode);
	Sleep(1000);
	VCS_SetDisableState(_KeyHandle, _node[0], &_ulErrorCode);
}

void CEpos::SetVelUnits() {
	byte vel_type = 0xA4;
	char vel_exp = char(_vel_exp);
	VCS_SetVelocityUnits(_KeyHandle, _node[0], vel_type, vel_exp, &_ulErrorCode);
}

bool CEpos::HomePrimary(short _nodeID) {
	short n = _nodeID;
	VCS_ActivateHomingMode(_KeyHandle, _node[n], &_ulErrorCode);
	return VCS_FindHome(_KeyHandle, _node[n], 35, &_ulErrorCode);
}

double CEpos::HomeSecondary(void) {
	double baseposition = ReadPosition2();
	return baseposition;
}