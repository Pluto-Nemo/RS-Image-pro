
// 3200101042SX11Dlg.h: 头文件
//

#pragma once
#include "gdal201/gdal/include/gdal_priv.h"
#include "gdal201/gdal/include/gdalwarper.h"
#include "gdal201/gdal/include/ogrsf_frmts.h"
#pragma comment(lib, "gdal_i.lib")

// CMy3200101042SX11Dlg 对话框
class CMy3200101042SX11Dlg : public CDialogEx
{
// 构造
public:
	CMy3200101042SX11Dlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MY3200101042SX11_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
	void ComputeClassNum(GDALDataset* dataset, int& max);
public:
	CString ImgPath;
	CString RoiPath;
	afx_msg void OnBnClickedButton1();
};
