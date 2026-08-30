#pragma once
#include "StdAfx.h"
#include "resource.h"

// FormLabelDlg dialog

class FormLabelDlg : public CDialog
{
	DECLARE_DYNAMIC(FormLabelDlg)

public:
	FormLabelDlg(CWnd* pParent,CString cfor);
	virtual ~FormLabelDlg();
  CString  GetFor();

// Dialog Data
	enum { IDD = IDD_FORMLABEL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
  CString   m_for;
  AD_Button   m_buttonOK;
  AD_Button   m_buttonCancel;
  AD_Button   m_buttonHelp;
public:
  afx_msg void OnEnChangeFlFor();
  afx_msg void OnBnClickedOk();
  afx_msg void OnBnClickedCancel();
  afx_msg void OnBnClickedFlHelp();
};

inline CString
FormLabelDlg::GetFor()
{
  return m_for;
}