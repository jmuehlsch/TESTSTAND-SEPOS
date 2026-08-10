
// SEPOSDlg.cpp : implementation file
//

#include "pch.h"
#include "framework.h"
#include "SEPOS.h"
#include "SEPOSDlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <cmath>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/// CAboutDlg ****************************************************************************************************************************

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedHome3();
	afx_msg void OnBnClickedHome2();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CSEPOSDlg::OnBnClickedSaveDataPos()
{
	// Open file dialog:
	CString path[3];
	if (!_isSaving) {
		if (!_OpenOrSaveOneFileDialogue(false, "txt", path))
			return;
	}

	// Flip state:
	_isSaving = !_isSaving;
	// Change button state and text:
	if (!_isSaving) _buttonSaveData.SetWindowTextW(L"Start Saving");
	else _buttonSaveData.SetWindowTextW(L"Stop Saving");

	// Open file for saving
	if (_isSaving) {
		_SaveData(path[2]);
	}
	// Close file
	else {
		_SaveData(L"Append");
		_SaveData(L"Close");
	}
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	// Home buttons removed or not present in resources; handlers remain if needed.
END_MESSAGE_MAP()

/// CSEPOSDlg ****************************************************************************************************************************

CSEPOSDlg::CSEPOSDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SEPOS_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//_sampleRate = 100;	// Defalut sample rate 
	//_posCmd = 0; // Default Pos Command

	_epos = NULL;
	_epos = new CEpos();

	_isSaving = false;
}

void CSEPOSDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_ANALOG_IN_1, _rEditAnalogIn1);
	//DDX_Control(pDX, IDC_SAMPLE_RATE, _rEditSampleRate);
	DDX_Control(pDX, IDC_SAVE_DATA, _buttonSaveData);

	DDX_Control(pDX, IDC_POS_PROF_VEL, _rEditPosProfVel);
	DDX_Control(pDX, IDC_POS_PROF_ACCEL, _rEditPosProfAccel);
	DDX_Control(pDX, IDC_POS_PROF_DECCEL, _rEditPosProfDeccel);
	DDX_Control(pDX, IDC_EPOS_POS_CMD, _rEditPosCmd);

	DDX_Control(pDX, IDC_VEL_PROF_ACCEL, _rEditVelProfAccel);
	DDX_Control(pDX, IDC_VEL_PROF_DECCEL, _rEditVelProfDeccel);
	DDX_Control(pDX, IDC_EPOS_VEL_CMD, _rEditVelCmd);
	if (GetDlgItem(IDC_SAVE_DATA_VEL))
		DDX_Control(pDX, IDC_SAVE_DATA_VEL, _buttonVelSaveData);

	if (GetDlgItem(IDC_STATUS_TEXT))
		DDX_Control(pDX, IDC_STATUS_TEXT, _rEditStatus);

}

BEGIN_MESSAGE_MAP(CSEPOSDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()

	ON_BN_CLICKED(IDC_SAVE_DATA, &CSEPOSDlg::OnClickedSaveData)

	ON_BN_CLICKED(IDC_ENABLE_POS_CTRL, &CSEPOSDlg::OnBnClickedEnablePosCtrl)
	ON_BN_CLICKED(IDC_ENABLE_VEL_CTRL, &CSEPOSDlg::OnBnClickedEnableVelCtrl)
	ON_BN_CLICKED(IDC_SEND_EPOS_POS, &CSEPOSDlg::OnBnClickedSendEposPos)
	ON_BN_CLICKED(IDC_SEND_EPOS_VEL, &CSEPOSDlg::OnBnClickedSendEposVel)
	ON_BN_CLICKED(IDC_SAVE_DATA_VEL, &CSEPOSDlg::OnBnClickedSaveDataVel)
	ON_BN_CLICKED(IDC_SAVE_DATA_POS, &CSEPOSDlg::OnBnClickedSaveDataPos)
	ON_BN_CLICKED(IDC_EPOS_ESTOP, &CSEPOSDlg::OnBnClickedEposEstop)
	ON_BN_CLICKED(IDC_HOME_PRIMARY, &CSEPOSDlg::OnBnClickedCalPrimary)
	ON_BN_CLICKED(IDC_HOME_SECONDARY, &CSEPOSDlg::OnBnClickedHomePrimary2)
	ON_BN_CLICKED(IDC_BUTTON1, &CSEPOSDlg::OnBnClickedButton1)
	ON_EN_CHANGE(IDC_EDIT1, &CSEPOSDlg::OnEnChangeEdit1)
END_MESSAGE_MAP()

BOOL CSEPOSDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// Initialize EPOS class:
	bool done = _epos->Initialize();

	start_time = std::chrono::high_resolution_clock::now();

	_mode = 0;

	CString velaccel_text;
	velaccel_text.Format(L"%.0f", _eposVelProfDefaultAccel);
	SetDlgItemTextW(IDC_VEL_PROF_ACCEL, velaccel_text);
	SetDlgItemTextW(IDC_VEL_PROF_DECCEL, velaccel_text);

	CString posaccel_text;
	posaccel_text.Format(L"%.0f", _eposPosProfDefaultAccel);
	SetDlgItemTextW(IDC_POS_PROF_ACCEL, posaccel_text);
	SetDlgItemTextW(IDC_POS_PROF_DECCEL, posaccel_text);

	CString velocity_text;
	velocity_text.Format(L"%.0f", _eposGlobalDefaultVel);
	SetDlgItemTextW(IDC_EPOS_VEL_CMD, velocity_text);
	SetDlgItemTextW(IDC_POS_PROF_VEL, velocity_text);

	CString target_text;
	target_text.Format(L"%.0f", _eposPosProfDefaultTarget);
	SetDlgItemTextW(IDC_EPOS_POS_CMD, target_text);

	// Set timer for OnTimer:
	if (done)	SetTimer(10, 50, NULL);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CSEPOSDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

void CSEPOSDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSEPOSDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CSEPOSDlg::OnTimer(UINT_PTR nIDEvent)
{
	// Capture the current time point
	now = std::chrono::high_resolution_clock::now();
	// relative to start
	duration = now - start_time;
	curr_loop_time = std::chrono::duration<double>(duration).count();
	
	// Read from EPOS:
	_eposCurrPos = _epos->ReadPosition();
	_eposCurrPosDeg = 360.0 * _eposCurrPos / 40000.0;
	_eposCurrVel = _epos->ReadVelocity();
	_eposCurrVelAvg = _eposCurrVelAvg * _eposCurrVelFilterFrac + _eposCurrVel * (1 - _eposCurrVelFilterFrac);
	_eposCurrCurr = _epos->ReadCurrent();
	_eposCurrCurrAvg = _eposCurrCurrAvg * _eposCurrCurrFilterFrac + _eposCurrCurr * (1 - _eposCurrCurrFilterFrac);

	_epos2CurrPos = _epos->ReadPosition2();
	// compute secondary encoder position relative to stored home (if any)
	long secHome = 0;
	if (_epos) secHome = _epos->GetSecondaryHome();
	long relPos2 = _epos2CurrPos - secHome;
	_epos2CurrPosDeg = 360.0 * relPos2 / 40000.0;
	_epos2CurrVel = _epos->ReadVelocity2();
	_epos2CurrVelAvg = _epos2CurrVelAvg * _epos2CurrVelFilterFrac + _epos2CurrVel * (1 - _epos2CurrVelFilterFrac);


	// Write it in Edit
	CString pos_text;	
	pos_text.Format(L"%.2f", _eposCurrPosDeg);
	SetDlgItemTextW(IDC_EPOS_CURR_POS, pos_text);

	CString vel_text;	
	vel_text.Format(L"%.2f", _eposCurrVelAvg);
	SetDlgItemTextW(IDC_EPOS_CURR_VEL, vel_text);

	CString curr_text;
	curr_text.Format(L"%.0f", _eposCurrCurrAvg);
	SetDlgItemTextW(IDC_EPOS_CURR_CURR, curr_text);


	CString pos_text2;
	pos_text2.Format(L"%.2f", _epos2CurrPosDeg);
	SetDlgItemTextW(IDC_EPOS2_CURR_POS, pos_text2);

	CString vel_text2;
	vel_text2.Format(L"%.2f", _epos2CurrVelAvg);
	SetDlgItemTextW(IDC_EPOS2_CURR_VEL, vel_text2);

	CDialog::OnTimer(nIDEvent);
}

void CSEPOSDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	KillTimer(1);

	// Close EPOS and delete object::
	if (_epos) {
		_epos->Close();
		delete _epos;
		_epos = NULL;
	}
}


void CSEPOSDlg::OnClickedSaveData()
{
	// Open file dialog:
	CString path[3];
	if (!_isSaving)
		if (!_OpenOrSaveOneFileDialogue(false, "txt", path))
			return;

	// Flip state:
	_isSaving = !_isSaving;
	// Change button state and text:
	_buttonSaveData.SetState(_isSaving);
	if(!_isSaving)	_buttonSaveData.SetWindowTextW(L"Start Saving");
	else			_buttonSaveData.SetWindowTextW(L"Stop Saving");

	// Opn file for saving
	if (_isSaving)	_SaveData(path[2]);
	// Close file
	else			_SaveData(L"Close");
}

void CSEPOSDlg::_SaveData(CString filePath)
{
	static std::ofstream file;

	if (filePath == L"Close") {
		file.close();
		return;
	}

	CString			cStr;
	std::string		sStr;

	if (filePath == L"Append") {
		//while (fibre1_queue->size() > 0) {
			//fibres[0] = fibre1_queue->front();
			//fibres[1] = fibre2_queue->front();
			//fibres[2] = fibre3_queue->front();
			//incrementalPos = incrementalPos_queue->front();
			//halTick = halTick_queue->front();
			//incrementalPos2 = incrementalPos2_queue->front();
			//cStr.Format(L"%d, %d, %d, %d, %d, %d\n", fibres[0], fibres[1], fibres[2], incrementalPos, halTick, incrementalPos2);
			//sStr = CT2CA(cStr.operator LPCWSTR());
			//file << sStr;
			//fibre1_queue->pop();
			//fibre2_queue->pop();
			//fibre3_queue->pop();
			//incrementalPos_queue->pop();
			//halTick_queue->pop();
			//incrementalPos2_queue->pop();
		//}
		//delete fibre1_queue;
		//delete fibre2_queue;
		//delete fibre3_queue;
		//delete incrementalPos_queue;
		//delete halTick_queue;
		//delete incrementalPos2_queue;
		return;
	}

	/// Open file:
	std::string sFilePath = CT2CA(filePath.operator LPCWSTR()); 
	file.open(sFilePath, std::ofstream::out);
}


std::string CSEPOSDlg::_GenerateFileName() {
	// Type 1 is servo, Type 2 is Stepper
	std::string fileExtension = "txt";
	time_t now = time(NULL);
	struct tm timeInfo;
	localtime_s(&timeInfo, &now);
	char strTime[50];
	strftime(strTime, sizeof(strTime), "%y-%m-%d_%H;%M;%S", &timeInfo);

	CString cStr;
	std::string velStr;

	_rEditVelCmd.GetWindowText(cStr);
	velStr = CT2CA(cStr.operator LPCWSTR());

	// Set File name
	std::string filename = std::string(strTime) + "_Vel_" + velStr + "." + fileExtension;
	return filename;
	}


bool CSEPOSDlg::_OpenOrSaveOneFileDialogue(bool open, std::string fileExtension, CString* vPath)
{	/// open = true to Open / false to Save (follows CFileDialog fileDlg)
	/// vPath MUST be a vector of [3]
	/// Returns [Folder, FileName (no extension), Folder\FileName.extension]

	std::string filename = _GenerateFileName();

	// Convert uppercase file extension:
	std::string FE = fileExtension;
//	std::transform(FE.begin(), FE.end(), FE.begin(), ::toupper);

	// Set file filter:
	std::string strF = FE + " File (*." + fileExtension + ")|*." + fileExtension + "|All Files|*.*||";
	std::wstring wstrF = std::wstring(strF.begin(), strF.end());
	const wchar_t* szFilter = wstrF.c_str();

	// Set Default extension:
	std::wstring wstrE = std::wstring(fileExtension.begin(), fileExtension.end());
	const wchar_t* szDefExt = wstrE.c_str();

	// File dialogue:
	CFileDialog fileDlg(open, szDefExt, CA2T(filename.c_str()), OFN_OVERWRITEPROMPT, szFilter);

	// Return filePath:
	if (fileDlg.DoModal() == IDOK) {
		vPath[0] = fileDlg.GetFolderPath() + L"\\";
		vPath[1] = fileDlg.GetFileTitle();
		vPath[2] = fileDlg.GetPathName();
		return true;
	}
	else	return false;
}

void CSEPOSDlg::OnBnClickedSaveDataVel()
{
	// Open file dialog:
	CString path[3];
	if (!_isSaving) {
		if (!_OpenOrSaveOneFileDialogue(false, "txt", path))
			return;
		//fibre1_queue = new std::queue<unsigned long>;
		//fibre2_queue = new std::queue<unsigned long>;
		//fibre3_queue = new std::queue<unsigned long>;
		//incrementalPos_queue = new std::queue<long>;
		//halTick_queue = new std::queue<long>;
		//incrementalPos2_queue = new std::queue<long>;
	}

	// Flip state:
	_isSaving = !_isSaving;
	// Change button state and text:
	//_buttonVelSaveData.SetState(_isSaving);
	if (!_isSaving)	_buttonVelSaveData.SetWindowTextW(L"Start Saving");
	else			_buttonVelSaveData.SetWindowTextW(L"Stop Saving");

	// Opn file for saving
	if (_isSaving) {
		_SaveData(path[2]);
	}
	// Close file
	else {
		_SaveData(L"Append");
		_SaveData(L"Close");
	}
}

void CSEPOSDlg::OnOK()
{
	// Don't close on Enter
//	CDialogEx::OnOK();
}
void CSEPOSDlg::OnCancel()
{
	// Comes here on Windows close and on ESC
	if (AfxMessageBox(L"Exit Program ?", MB_ICONQUESTION | MB_YESNO) == IDNO)		return;
	CDialogEx::OnCancel();
}


void CSEPOSDlg::OnBnClickedEnablePosCtrl()
{
	_mode = 1;
	_epos->ActivatePosProfMode();
}


void CSEPOSDlg::OnBnClickedEnableVelCtrl()
{
	_mode = 2;
	_epos->ActivateVelProfMode();
}


void CSEPOSDlg::OnBnClickedSendEposPos()
{
		CString text;

		_rEditPosProfVel.GetWindowText(text);
		_eposPosProfVel = _wtof(text);

		_rEditPosProfAccel.GetWindowText(text);
		_eposPosProfAccel = _wtof(text);

		_rEditPosProfDeccel.GetWindowText(text);
		_eposPosProfDeccel = _wtof(text);

		_rEditPosCmd.GetWindowText(text);
		_eposPosCmd = _wtof(text);

		_epos->SetPositionProfile(_eposPosProfVel, _eposPosProfAccel, _eposPosProfDeccel);
		_epos->SetPosition(_eposPosCmd);
}


void CSEPOSDlg::OnBnClickedSendEposVel()
{
	CString text;

	_rEditVelProfAccel.GetWindowText(text);
	_eposVelProfAccel = _wtof(text);

	_rEditVelProfDeccel.GetWindowText(text);
	_eposVelProfDeccel = _wtof(text);

	_rEditVelCmd.GetWindowText(text);
	_eposVelCmd = _wtof(text);

	_epos->SetVelocityProfile(_eposVelProfAccel, _eposVelProfDeccel);
	_epos->SetVelocity(_eposVelCmd);
}


void CSEPOSDlg::OnBnClickedEposEstop()
{
	_mode = 0;
	_epos->Disable();
}


void CSEPOSDlg::OnBnClickedHome3()
{

}

void CAboutDlg::OnBnClickedHome2()
{

}

void CSEPOSDlg::OnBnClickedCalPrimary()
{
	_epos->HomePrimary(0);
}

void CSEPOSDlg::OnBnClickedHomePrimary2()
{
	// Read current secondary encoder position and store it in CEpos as the home reference
	if (_epos) {
		long pos2 = _epos->ReadPosition2();
		_epos->SetSecondaryHome(pos2);

		// Show value in status edit (if present). Since we just set the home to pos2,
		// the relative position will be zero.
		CString s;
		double posDeg = 360.0 * (pos2 - _epos->GetSecondaryHome()) / 40000.0; // will be zero
		s.Format(L"Secondary home set: %ld (%.2f deg)", pos2, posDeg);
		if (GetDlgItem(IDC_STATUS_TEXT)) SetDlgItemTextW(IDC_STATUS_TEXT, s);
	}

}


void CSEPOSDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
}

void CSEPOSDlg::OnEnChangeEdit1()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}
