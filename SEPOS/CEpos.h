#pragma once
class CEpos
{
public:

	CEpos();
	~CEpos();


	bool	Initialize();
	void	Close();

	int		_vel_exp = -3;
	int		_reg_exp = -3;
	int		_vel_scale = 6;

	WORD	ReadAnalogInput(void);

	long	ReadPosition(void);
	double	ReadVelocity(void);
	long	ReadPosition2(void);
	double	ReadVelocity2(void);
	short	ReadCurrent(void);
	void	ActivatePosProfMode();
	void	SetPositionProfile(double _vel, double _accel, double _deccel);
	void	SetPosition(long _pos);
	void	ActivateVelProfMode();
	void	SetVelocityProfile(double _accel, double _deccel);
	void	SetVelocityGains(double _p, double _i);
	void	SetVelocity(double _vel);
	bool	SetPositionBlocking(long _pos);
	void	Disable();
	void	SetVelUnits();

	// VCS_SetVelocityRegulatorGain // change PID gains to make sure system is stable...
	// VCS_SetVelocityRegulatorFeedForward
	// VCS_SetPositionRegulatorGain // change PID gains to make sure system is stable...
	// VCS_SetPositionRegulatorFeedForward


private:
	HANDLE	_KeyHandle;
	DWORD	_ulErrorCode;
	BOOL	_success;
	
	static const WORD	_nNodes = 2;		// Number of Nodes (Axes)
	static const WORD	_node[_nNodes];		// Number of Nodes (Axes)
	static const WORD	_aI[_nNodes];		// Digital Inputs
	
	static const WORD	_Node = 1U;

};

