
// 3200101042SX11Dlg.cpp: 实现文件
//

#include "pch.h"
#include "framework.h"
#include "3200101042SX11.h"
#include "3200101042SX11Dlg.h"
#include "afxdialogex.h"
#include <fstream>
#include <iostream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMy3200101042SX11Dlg 对话框



CMy3200101042SX11Dlg::CMy3200101042SX11Dlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MY3200101042SX11_DIALOG, pParent)
	, ImgPath(_T(""))
	, RoiPath(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy3200101042SX11Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_FILEA, ImgPath);
	DDX_Text(pDX, IDC_MFCEDITBROWSE_FILEB, RoiPath);
}

BEGIN_MESSAGE_MAP(CMy3200101042SX11Dlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CMy3200101042SX11Dlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CMy3200101042SX11Dlg 消息处理程序

BOOL CMy3200101042SX11Dlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	GDALAllRegister();
	CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "NO");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CMy3200101042SX11Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMy3200101042SX11Dlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMy3200101042SX11Dlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CMy3200101042SX11Dlg::ComputeClassNum(GDALDataset* dataset, int& max)
{
	int col = dataset->GetRasterXSize();
	int row = dataset->GetRasterYSize();
	GDALRasterBand* band = dataset->GetRasterBand(1);
	int* p = new int[col];
	band->RasterIO(GF_Read, 0, 0, 1, 1, &max, 1, 1, GDT_Int32, 0, 0);
	for (int i = 0; i < row; i++) {
		band->RasterIO(GF_Read, 0, i, col, 1, p, col, 1, GDT_Int32, 0, 0);
		for (int j = 0; j < col; j++) {
			int temp = p[j];
			if (temp > max) max = temp;
		}
	}
	delete[] p;
}



void CMy3200101042SX11Dlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	GDALDataset* poImg = (GDALDataset*)GDALOpen(ImgPath, GA_ReadOnly);
	GDALDataset* poRoi = (GDALDataset*)GDALOpen(RoiPath, GA_ReadOnly);
	if (poImg == NULL || poRoi == NULL) {
		MessageBox("文件打开失败！");
		return;
	}

	GDALDriver* poDriver = GetGDALDriverManager()->GetDriverByName("GTiff");
	if (poDriver == NULL) {
		GDALClose((GDALDatasetH)poImg);
		GDALClose((GDALDatasetH)poRoi);
		MessageBox("驱动运行失败！");
		return;
	}

	int XSize = poImg->GetRasterXSize();
	int YSize = poImg->GetRasterYSize();
	int RXSize = poRoi->GetRasterXSize();
	int RYSize = poRoi->GetRasterYSize();
	if (XSize != RXSize || YSize != RYSize) {
		MessageBox("长宽不一致！");
	}
	GDALRasterBand* pBandI = poImg->GetRasterBand(1);
	GDALRasterBand* pBandR = poRoi->GetRasterBand(1);
	int* I = new int[XSize];
	int* R = new int[XSize];

	int ClassNum = 0;
	int RClassNum = 0;
	//类别数
	ComputeClassNum(poImg, ClassNum);
	ComputeClassNum(poRoi, RClassNum);
	if (ClassNum > 10) {
		MessageBox("类别数过多！");
	}
	if (ClassNum != RClassNum) {
		MessageBox("类别数不一致！");
	}
	int CM[10][10] = { 0 };
	float PA[10] = { 0.0 };
	float PU[10] = { 0.0 };
	//混淆矩阵、生产者精度、用户精度
	int count = 0;
	for (int i = 0; i < YSize; i++) {
		//读取数据
		pBandI->RasterIO(GF_Read, 0, i, XSize, 1, I, XSize, 1, GDT_Int32, 0, 0);
		pBandR->RasterIO(GF_Read, 0, i, XSize, 1, R, XSize, 1, GDT_Int32, 0, 0);
		for (int j = 0; j < XSize; j++) {
			if (I[j] > 0) {
				count++;
			}
			if (R[j] != 0) {
				/*CString tips;
				tips.Format("%d, %d", R[j], I[j]);
				MessageBox(tips);*/
				CM[I[j] - 1][R[j] - 1]++;
			}
		}
	}
	//混淆矩阵排序
	int time = 0;//记录交换到第几列
	for (int ii = 0; ii < ClassNum; ii++) {
		int tmp;
		switch (ii + 1) {
		case 1: 
			for (int jj = 0; jj < ClassNum; jj++) {
				tmp = CM[ii][jj];
				CM[ii][jj] = CM[3][jj];
				CM[3][jj] = tmp;
			}
			break;
		case 2:
			for (int jj = 0; jj < ClassNum; jj++) {
				tmp = CM[ii][jj];
				CM[ii][jj] = CM[4][jj];
				CM[4][jj] = tmp;
			}
			break;
		case 3:
			for (int jj = 0; jj < ClassNum; jj++) {
				tmp = CM[ii][jj];
				CM[ii][jj] = CM[3][jj];
				CM[3][jj] = tmp;
			}
			break;
		case 4:
			for (int jj = 0; jj < ClassNum; jj++) {
				tmp = CM[ii][jj];
				CM[ii][jj] = CM[4][jj];
				CM[4][jj] = tmp;
			}
			break;
		}
	}

	//计算生产者精度和用户精度
	for (int ii = 0; ii < ClassNum; ii++) {
		int count = 0;
		for (int jj = 0; jj < ClassNum; jj++) {
			count += CM[ii][jj];
		}
		PU[ii] = 100.0 * (float)CM[ii][ii] / (float)count;
	}
	for (int ii = 0; ii < ClassNum; ii++) {
		int count = 0;
		for (int jj = 0; jj < ClassNum; jj++) {
			count += CM[jj][ii];
		}
		PA[ii] = 100.0 * (float)CM[ii][ii] / (float)count;
	}
	CString tips;
	tips.Format("%d", count);
	MessageBox(tips);
	//text写出
	ofstream out("混淆矩阵.txt");
	out << "Class		Class1		Class2		Class3		Class4		Class5		PU(100%)\n";
	for (int ii = 0; ii < ClassNum; ii++) {
		out << "Class" << ii + 1;
		for (int jj = 0; jj < ClassNum; jj++) {
			out << "		" << CM[ii][jj];
		}
		out << "		" << PU[ii] << "\n";
	}
	out << "PA(100%)";
	for (int ii = 0; ii < ClassNum; ii++) {
		out << "		" << PA[ii];
	}
	out.close();
	GDALClose((GDALDatasetH)poImg);
	GDALClose((GDALDatasetH)poRoi);
	MessageBox("Done!");
}
