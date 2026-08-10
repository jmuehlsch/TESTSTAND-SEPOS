
// SEPOSDlg.h : header file
//

#pragma once
#include "CEpos.h"
#include <string>
#include <queue>
#include <chrono>

// CSEPOSDlg dialog
class CSEPOSDlg : public CDialogEx
{
// Private variables
private:

	// EPOS
	CEpos*	_epos;
	//UINT	_sampleRate;
	int _mode;
	std::chrono::high_resolution_clock::time_point start_time;
	std::chrono::high_resolution_clock::time_point now;
	std::chrono::duration<double, std::milli> duration;
	double curr_loop_time;

	// position mode
	double	_eposPosProfVel;
	double	_eposPosProfAccel;
	double	_eposPosProfDeccel;
	double	_eposPosCmd;
	double  _eposPosCmd2;
	double	_eposPosProfDefaultAccel = 1800;
	double	_eposPosProfDefaultTarget = 0;
	// velocity mode
	double	_eposVelProfAccel;
	double	_eposVelProfDeccel;
	double	_eposVelCmd;
	double  _eposVelProfDefaultAccel = 1800;
	// state
	long	_eposCurrPos;
	double  _eposCurrPosDeg;
	double	_eposCurrVel;
	double	_eposCurrVelFilterFrac = 0.8;
	double	_eposCurrVelAvg = 0;
	short   _eposCurrCurr;
	double	_eposCurrCurrFilterFrac = 0.95;
	double  _eposCurrCurrAvg = 0;
	//encoder2 state
	long	_epos2CurrPos;
	double  _epos2CurrPosDeg;
	double	_epos2CurrVel;
	double	_epos2CurrVelFilterFrac = 0.8;
	double	_epos2CurrVelAvg = 0;
	//general
	double  _eposGlobalDefaultVel = 180;


	// file handling
	bool	_isSaving;


// Private functions:
	bool _OpenOrSaveOneFileDialogue(bool open, std::string fileExtension, CString* vPath);
	void _SaveData(CString filePath);
	std::string _GenerateFileName();

// Construction
public:
	CSEPOSDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SEPOS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()

private:
	CEdit _rEditAnalogIn1;
	CEdit _rEditSampleRate;
	CButton _buttonSaveData;

	CEdit _rEditPosProfVel;
	CEdit _rEditPosProfAccel;
	CEdit _rEditPosProfDeccel;
	CEdit _rEditPosCmd;
	CEdit _rEditPosCmd2;

	CEdit _rEditVelProfAccel;
	CEdit _rEditVelProfDeccel;
	CEdit _rEditVelCmd;

	CEdit _rEditVelBatchTestTime;

	CEdit _rEditStatus;

	CButton _buttonVelSaveData;
	CButton _buttonVelBatchTest;

	CProgressCtrl _progressBatchVel;

public:
	afx_msg void OnClickedSaveData();
	virtual void OnOK();
	virtual void OnCancel();

	afx_msg void OnBnClickedEnablePosCtrl();
	afx_msg void OnBnClickedEnableVelCtrl();
	afx_msg void OnBnClickedSendEposPos();
	afx_msg void OnBnClickedSendEposVel();
	afx_msg void OnBnClickedEposEstop();
	afx_msg void OnBnClickedSaveDataVel();
	afx_msg void OnBnClickedSaveDataPos();
	afx_msg void OnBnClickedHome3();
	afx_msg void OnBnClickedCalPrimary();
	afx_msg void OnBnClickedHomePrimary2();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnEnChangeEdit1();
};
