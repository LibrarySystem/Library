
#include "stdafx.h"
#include "Library.h"
#include "BookTypeDlg.h"
#include "InputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBookTypeDlg::CBookTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBookTypeDlg::IDD, pParent)
{}
void CBookTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrList);
}
BEGIN_MESSAGE_MAP(CBookTypeDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_NEW, OnButtonNew)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnButtonModify)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
END_MESSAGE_MAP()
void CBookTypeDlg::OnButtonNew() 
{
	CInputDlg dlg;
	dlg.m_strTypeNo = "图书类型：";
	dlg.m_strNumberNo = "可借天数：";
	if(dlg.DoModal()==IDOK)
	{
		CString strSQL;
		strSQL.Format("select * from bookType where type='%s'",dlg.m_strType);
		if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
		{
			MessageBox("打开数据库失败!","数据库错误",MB_OK);
			return ;
		}
		if(m_recordset.GetRecordCount()!=0)
		{
			m_recordset.Close();
			MessageBox("该记录已经存在！");
			return;
		}
		m_recordset.AddNew();
		m_recordset.m_type = dlg.m_strType;
		m_recordset.m_number = dlg.m_nNumber;
		m_recordset.Update();
		m_recordset.Close();
		RefreshData();
	}
}

void CBookTypeDlg::OnButtonModify() 
{
	int i = m_ctrList.GetSelectionMark();
	if(i<0)
	{
		AfxMessageBox("请选取要修改的记录！");
		return;
	}
	CInputDlg dlg;
	dlg.m_strTypeNo = "图书类型：";
	dlg.m_strNumberNo = "可借天数：";
	dlg.m_strType = m_ctrList.GetItemText(i,0);
	dlg.m_nNumber = atoi(m_ctrList.GetItemText(i,1));
	if(dlg.DoModal()==IDOK)
	{
		CString strSQL;
		strSQL.Format("select * from bookType where type='%s'",dlg.m_strType);
		if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
		{
			MessageBox("打开数据库失败!","数据库错误",MB_OK);
			return ;
		}
		if(m_recordset.GetRecordCount()==0)
		{
			m_recordset.Close();
			MessageBox("该记录不存在！");
			return;
		}
		m_recordset.Edit();
		m_recordset.m_type = dlg.m_strType;
		m_recordset.m_number = dlg.m_nNumber;
		m_recordset.Update();
		m_recordset.Close();
		RefreshData();
	}	
}

void CBookTypeDlg::OnButtonDelete() 
{
	int i = m_ctrList.GetSelectionMark();
	if(i<0)
	{
		AfxMessageBox("请选取要删除的记录！");
		return;
	}	
	//打开记录集
	CString strSQL;
	strSQL.Format("select * from bookType where type='%s'",m_ctrList.GetItemText(i,0));
	if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
	{
		MessageBox("打开数据库失败!","数据库错误",MB_OK);
		return ;
	}
	//判断记录是否不存在
	if(m_recordset.GetRecordCount()==0)
	{
		m_recordset.Close();
		MessageBox("该记录不存在！");
		return;
	}
	//删除记录
	m_recordset.Delete();
	m_recordset.Close();
	//更新列表	
	RefreshData();
}

BOOL CBookTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//设置数据列表
	m_ctrList.InsertColumn(0,"类别名称");
	m_ctrList.SetColumnWidth(0,120);
	m_ctrList.InsertColumn(1,"可借天数");
	m_ctrList.SetColumnWidth(1,60);
	m_ctrList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//显示数据到列表中
	RefreshData();	
	return TRUE;
}
void CBookTypeDlg::RefreshData() 
{

	m_ctrList.SetFocus();
	//清空列表
	m_ctrList.DeleteAllItems();
	m_ctrList.SetRedraw(FALSE);
	
	CString strSQL;
	UpdateData(TRUE);
	strSQL="select * from bookType";
	if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
	{
		MessageBox("打开数据库失败!","数据库错误",MB_OK);
		return ;
	}	
	int i=0;
	char buffer[20];
	while(!m_recordset.IsEOF())
	{
		m_ctrList.InsertItem(i,m_recordset.m_type);
		itoa(m_recordset.m_number,buffer,10);
		m_ctrList.SetItemText(i,1,buffer);
		i++;
		m_recordset.MoveNext();
	}
	m_recordset.Close();
	m_ctrList.SetRedraw(TRUE);

}
