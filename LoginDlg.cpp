#include "stdafx.h"
#include "Library.h"
#include "LoginDlg.h"
#include "userset.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLoginDlg::CLoginDlg(CWnd* pParent )
	: CDialog(CLoginDlg::IDD, pParent)
{
	m_strUser = _T("");
	m_strPass = _T("");
}


void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_ctrPass);
	DDX_Control(pDX, IDC_COMBO1, m_ctrUser);
	DDX_CBString(pDX, IDC_COMBO1, m_strUser);
	DDX_Text(pDX, IDC_EDIT1, m_strPass);
}
BEGIN_MESSAGE_MAP(CLoginDlg, CDialog)
END_MESSAGE_MAP()

void CLoginDlg::OnOK() 
{
	CUserSet recordset;
	CString strSQL;
	UpdateData(TRUE);
	if(m_strUser.IsEmpty())
	{
		AfxMessageBox("请输入用户名！");
		m_ctrUser.SetFocus();
		return;
	}
	if(m_strPass.IsEmpty())
	{
		AfxMessageBox("请输入密码！");
		m_ctrPass.SetFocus();
		return;
	}
	CLibraryApp*  ptheApp = (CLibraryApp *) AfxGetApp();
	strSQL.Format("select * from user where user='%s' AND passwd='%s'",m_strUser,m_strPass);

	if(!recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
	{
		MessageBox("打开数据库失败!","数据库错误",MB_OK);
		return ;
	}	
	if(recordset.GetRecordCount()==0)
	{
		recordset.Close();
		MessageBox("密码错误，请重新输入！");
		m_strPass="";
		m_ctrPass.SetFocus();
		UpdateData(FALSE);
		
	}
	else
	{
		ptheApp->m_bIsAdmin = recordset.m_isadmin;
		ptheApp->m_strOperator = recordset.m_user;
		recordset.Close();
		CDialog::OnOK();
	}
}
BOOL CLoginDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CUserSet recordset ;
	CString strSQL;
	UpdateData(TRUE);
	strSQL="select * from user";
	if(!recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
	{
		MessageBox("打开数据库失败!","数据库错误",MB_OK);
		return FALSE;
	}	
	while(!recordset.IsEOF())
	{
		m_ctrUser.AddString(recordset.m_user);
		recordset.MoveNext();
	}
	recordset.Close();
	return TRUE;  
}

void CLoginDlg::OnCancel() 
{

	CDialog::OnCancel();
}
