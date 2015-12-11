
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
	dlg.m_strTypeNo = "ͼ�����ͣ�";
	dlg.m_strNumberNo = "�ɽ�������";
	if(dlg.DoModal()==IDOK)
	{
		CString strSQL;
		strSQL.Format("select * from bookType where type='%s'",dlg.m_strType);
		if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
		{
			MessageBox("�����ݿ�ʧ��!","���ݿ����",MB_OK);
			return ;
		}
		if(m_recordset.GetRecordCount()!=0)
		{
			m_recordset.Close();
			MessageBox("�ü�¼�Ѿ����ڣ�");
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
		AfxMessageBox("��ѡȡҪ�޸ĵļ�¼��");
		return;
	}
	CInputDlg dlg;
	dlg.m_strTypeNo = "ͼ�����ͣ�";
	dlg.m_strNumberNo = "�ɽ�������";
	dlg.m_strType = m_ctrList.GetItemText(i,0);
	dlg.m_nNumber = atoi(m_ctrList.GetItemText(i,1));
	if(dlg.DoModal()==IDOK)
	{
		CString strSQL;
		strSQL.Format("select * from bookType where type='%s'",dlg.m_strType);
		if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
		{
			MessageBox("�����ݿ�ʧ��!","���ݿ����",MB_OK);
			return ;
		}
		if(m_recordset.GetRecordCount()==0)
		{
			m_recordset.Close();
			MessageBox("�ü�¼�����ڣ�");
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
		AfxMessageBox("��ѡȡҪɾ���ļ�¼��");
		return;
	}	
	//�򿪼�¼��
	CString strSQL;
	strSQL.Format("select * from bookType where type='%s'",m_ctrList.GetItemText(i,0));
	if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
	{
		MessageBox("�����ݿ�ʧ��!","���ݿ����",MB_OK);
		return ;
	}
	//�жϼ�¼�Ƿ񲻴���
	if(m_recordset.GetRecordCount()==0)
	{
		m_recordset.Close();
		MessageBox("�ü�¼�����ڣ�");
		return;
	}
	//ɾ����¼
	m_recordset.Delete();
	m_recordset.Close();
	//�����б�	
	RefreshData();
}

BOOL CBookTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	//���������б�
	m_ctrList.InsertColumn(0,"�������");
	m_ctrList.SetColumnWidth(0,120);
	m_ctrList.InsertColumn(1,"�ɽ�����");
	m_ctrList.SetColumnWidth(1,60);
	m_ctrList.SetExtendedStyle(LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	//��ʾ���ݵ��б���
	RefreshData();	
	return TRUE;
}
void CBookTypeDlg::RefreshData() 
{

	m_ctrList.SetFocus();
	//����б�
	m_ctrList.DeleteAllItems();
	m_ctrList.SetRedraw(FALSE);
	
	CString strSQL;
	UpdateData(TRUE);
	strSQL="select * from bookType";
	if(!m_recordset.Open(AFX_DB_USE_DEFAULT_TYPE,strSQL))
	{
		MessageBox("�����ݿ�ʧ��!","���ݿ����",MB_OK);
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
