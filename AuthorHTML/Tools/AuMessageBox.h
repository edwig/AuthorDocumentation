//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    AuMessageBox.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: A specialized messagebox with custom buttons and images
//
#pragma once
#include "StdAfx.h"
#include "AuthorImage.h"
#include "AuthorButton.h"

#define ID_ONDERDRUKKEN 1959 // Magic number

#include "resource.h"

class AuMessageBox : CDialog
{
public:
  // Make with labels and styles from strings
  AuMessageBox(CWnd*  p_parent
              ,LPCTSTR p_titel
              ,LPCTSTR p_boodschap
              ,CString p_labels);
  // Make with MB_* styles
  AuMessageBox(CWnd* parent
              ,LPCTSTR titel
              ,LPCTSTR boodschap
              ,int    stijlen);
  // Destructor
  ~AuMessageBox();
  // Link to the resources
  enum { IDD = IDD_AUMESSAGEBOX };

  // The modal loop: later rewrite?
  virtual INT_PTR DoModal();
  // Give the result as a string ("ok","yes","no" etc)
  CString GetResult(int p_id);
  // Give the result as an ID (IDOK, IDYES etc)
	int     GetResultID(int p_id);
  // Standard positive answer (ok)
  CString ReturnStandardPositive();
  // Standard negative answer (no, cancel, annuleer)
  CString ReturnStandardNegative();
  // Standard positive answer via ID
  int     ReturnStandardPositiveID();
  // Standard negative answer via ID
  int     ReturnStandardNegativeID();
  // Check if this ID is the default button
  bool    GetDefault(UINT ID);
  // We press this button
  void    PressOnButton(UINT p_id,CPoint point);

  // Has a message map
  DECLARE_MESSAGE_MAP();

public:
  // Handlers
  afx_msg void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
  afx_msg void OnPaint();
  afx_msg void OnNotAgain();
  // Close the dialog and press the current button. This is only called
  // if the user presses Enter, because we don't have buttons with id IDOK!
  afx_msg void OnOK();

private:
  // METHODS
  // Initialization of the dialog
  virtual BOOL OnInitDialog() override;
  // Initialize button controls
  void InitButtons();
  // Reset the button controls
  void ResetButtons();
  // Split a label string into labels for buttons
  void MakeLabelTextsAndStyles(CString& p_labels);
  // Check a key hit on a button
  void CheckTheAction(UINT nChar);
  // Never show the current message again, always OK.
  void SuppressForever();

  // DATA
  CFont*       m_font;         // Font in which we display it
  bool         m_ownsFont;     // Delete font on destruction?
  CString      m_title;        // Title of the dialog
  CString      m_messsage;     // This is what we want to display
  int          m_styles;       // Signal styles
  int          m_default;      // This button is the default button
  bool         m_def_done;     // Default focus done
  CString      m_signalBMP;    // Signal this bitmap
  AuthorImage  m_image;        // Signal image
  CRect        m_buttonRect;   // Size of a button
  AuButton*    m_button[5];    // Max = 5 Buttons
  CString      m_label[5];     // Max = 5 labels
  CString      m_style[5];     // Max = 5 styles
  int          m_width[5];     // Max = 5 button widths
 
  // For message register
  bool         m_onlyOK;       // Only an OK button
  AD_Checkbox* m_suppress;     // Suppress dialog
  CRect        m_line;         // Line for subdivision
  bool         m_notAgain;     // Do not show again
};

