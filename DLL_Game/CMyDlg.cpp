// CMyDlg.cpp: 实现文件
//

#include "pch.h"
#include "DLL_Game.h"
#include "CMyDlg.h"
#include "CGame.h"


// CMyDlg 对话框

IMPLEMENT_DYNAMIC(CMyDlg, CDialogEx)

CMyDlg::CMyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG1, pParent)
{

}

CMyDlg::~CMyDlg()
{
}

void CMyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMyDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMyDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyDlg::OnBnClickedButton4)
END_MESSAGE_MAP()


// CMyDlg 消息处理程序


void CMyDlg::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	g_CGame.CLLK.KeyDown(VK_F1, g_CGame.wMain);
	//SetFocus();
}


void CMyDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	g_CGame.CLLK.KeyDown(VK_F4, g_CGame.wMain);
	//SetFocus();
}


void CMyDlg::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	g_CGame.CLLK.KeyDown(VK_F12, g_CGame.wMain);
}


void CMyDlg::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	g_CGame.CLLK.KeyDown(VK_F11, g_CGame.wMain);
}
