//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    TopicPropPage5.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Dialog for all topic properties page 5 of 5
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "TopicPropPage5.h"
#include "afxdialogex.h"


// TopicPropPage5 dialog

IMPLEMENT_DYNAMIC(TopicPropPage5Dlg, CDialog)

TopicPropPage5Dlg::TopicPropPage5Dlg(CWnd* p_parent,HtmlFrameSet p_frame,CString& p_base)
	             :CDialog(TopicPropPage5Dlg::IDD, p_parent)
               ,m_base(p_base)
               ,m_frameset(p_frame)
{
}

TopicPropPage5Dlg::~TopicPropPage5Dlg()
{
}

void TopicPropPage5Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(TopicPropPage5Dlg, CDialog)
  ON_EN_CHANGE(IDC_FRAME_COLS,        &TopicPropPage5Dlg::OnEnChangeFrameCols)
  ON_EN_CHANGE(IDC_FRAME_ROWS,        &TopicPropPage5Dlg::OnEnChangeFrameRows)
  ON_EN_CHANGE(IDC_BORDER,            &TopicPropPage5Dlg::OnEnChangeBorder)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN1,  &TopicPropPage5Dlg::OnDeltaposSpin1)
  ON_BN_CLICKED(IDC_BORDER_COLOR,     &TopicPropPage5Dlg::OnBnClickedBorderColor)
  ON_BN_CLICKED(IDC_DO_BC,            &TopicPropPage5Dlg::OnBnClickedDoBc)
  ON_BN_CLICKED(IDC_FRAMEBORDER,      &TopicPropPage5Dlg::OnBnClickedFrameborder)
  ON_EN_CHANGE(IDC_FRAMESPACING,      &TopicPropPage5Dlg::OnEnChangeFramespacing)
  ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2,  &TopicPropPage5Dlg::OnDeltaposSpin2)
  ON_BN_CLICKED(IDC_ID,               &TopicPropPage5Dlg::OnBnClickedId)
  ON_BN_CLICKED(IDC_EVENTS,           &TopicPropPage5Dlg::OnBnClickedEvents)
  ON_BN_CLICKED(IDC_BODY_STYLE,       &TopicPropPage5Dlg::OnBnClickedBodyStyle)
END_MESSAGE_MAP()


void
TopicPropPage5Dlg::UpdateProperties()
{

}

// TopicPropPage5 message handlers


void TopicPropPage5Dlg::OnEnChangeFrameCols()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}


void TopicPropPage5Dlg::OnEnChangeFrameRows()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}


void TopicPropPage5Dlg::OnEnChangeBorder()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}


void TopicPropPage5Dlg::OnDeltaposSpin1(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  // LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;
}


void TopicPropPage5Dlg::OnBnClickedBorderColor()
{
  // TODO: Add your control notification handler code here
}


void TopicPropPage5Dlg::OnBnClickedDoBc()
{
  // TODO: Add your control notification handler code here
}


void TopicPropPage5Dlg::OnBnClickedFrameborder()
{
  // TODO: Add your control notification handler code here
}


void TopicPropPage5Dlg::OnEnChangeFramespacing()
{
  // TODO:  If this is a RICHEDIT control, the control will not
  // send this notification unless you override the CDialog::OnInitDialog()
  // function and call CRichEditCtrl().SetEventMask()
  // with the ENM_CHANGE flag ORed into the mask.

  // TODO:  Add your control notification handler code here
}


void TopicPropPage5Dlg::OnDeltaposSpin2(NMHDR* /*pNMHDR*/, LRESULT *pResult)
{
  // LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
  // TODO: Add your control notification handler code here
  *pResult = 0;
}


void TopicPropPage5Dlg::OnBnClickedId()
{
  // TODO: Add your control notification handler code here
}


void TopicPropPage5Dlg::OnBnClickedEvents()
{
  // TODO: Add your control notification handler code here
}


void TopicPropPage5Dlg::OnBnClickedBodyStyle()
{
  // TODO: Add your control notification handler code here
}
