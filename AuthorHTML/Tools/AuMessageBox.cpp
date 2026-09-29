// AUTHOR HTML MessageBox
//
#include "stdafx.h"
#include "AuthorHTML.h"
#include "AuMessageBox.h"
#include "ControlsInfo.h"

const int OFFSET       =   8;         // Space between controls and texts
const int MAX_LABELS   =   5;         // Maximum number of labels/buttons
const int ID_OFFSET    =  10;         // Stay away from IDOK / IDCANCEL
const double ButtonWidthFactor = 8;   // Width of a button, in "W" letters

BEGIN_MESSAGE_MAP(AuMessageBox, CDialog)
  ON_WM_PAINT()
  ON_WM_KEYDOWN()
  ON_BN_CLICKED(ID_ONDERDRUKKEN,OnNotAgain)
END_MESSAGE_MAP()

AuMessageBox::AuMessageBox(CWnd*  parent
                          ,LPCTSTR titel
                          ,LPCTSTR boodschap
                          ,CString labels)
            :CDialog(AuMessageBox::IDD,parent)
            ,m_title(titel)
            ,m_messsage(boodschap)
            ,m_styles(0)
            ,m_default(0)
            ,m_def_done(false)
            ,m_suppress(NULL)
            ,m_onlyOK(false)
            ,m_notAgain(false)
            ,m_ownsFont(false)
            ,m_font(NULL)
{
  InitButtons();

  // Look for a signal image on the first label
  int piep = 0;
  if(labels.GetAt(0) == _T('!')) 
  {
    piep = MB_ICONEXCLAMATION;
    m_image.ZetAfbeelding(m_signalBMP = _T("EXCLAMATION"));
  }
  if(labels.GetAt(0) == _T('?')) 
  {
    piep = MB_ICONQUESTION;
    m_image.ZetAfbeelding(m_signalBMP = _T("QUESTION"));
  }
  if(labels.GetAt(0) == _T('.')) 
  {
    piep = MB_ICONHAND;
    m_image.ZetAfbeelding(m_signalBMP = _T("STOP"));
  }
  if(labels.GetAt(0) == _T('#')) 
  {
    piep = MB_ICONASTERISK;
    m_image.ZetAfbeelding(m_signalBMP = _T("INFORMATION"));
  }

  if(!m_signalBMP.IsEmpty())
  {
    // Remove character !?.# from the label
    labels = labels.Mid(1);
    // Give the correct beep, according to the style
    MessageBeep(piep);
  }

  if(labels == _T("ok cancel"))
  {
    labels = _T("ok cancel$ca");
  }
  if(labels == _T("cancel retry ignore"))
  {
    labels = _T("cancel$ca retry$ok ignore$ig");
  }
  if(labels == _T("yes no"))
  {
    labels = _T("yes$ye no$no");
  }
  if(labels == _T("yes no cancel"))
  {
    labels = _T("yes$ok no$ca cancel$ig");
  }
  if(labels == _T("retry ignore"))
  {
    labels = _T("retry$ok ignore$ca");
  }
  MakeLabelTextsAndStyles(labels);
}

AuMessageBox::AuMessageBox(CWnd* parent
                    ,LPCTSTR titel
                    ,LPCTSTR boodschap
                    ,int    stijlen)
          :CDialog(AuMessageBox::IDD,parent)
          ,m_title(titel)
          ,m_messsage(boodschap)
          ,m_default(0)
          ,m_def_done(false)
          ,m_styles(stijlen)
          ,m_suppress(NULL)
          ,m_onlyOK(false)
          ,m_notAgain(false)
          ,m_font(NULL)
{
  InitButtons();

  CString labels;
  if((stijlen & 0x0f) == MB_OK)       
  {
    labels = _T("ok");
  }
  if((stijlen & 0x0f) == MB_OKCANCEL) 
  {
    labels = _T("ok cancel$ca");
  }
  if((stijlen & 0x0f) == MB_ABORTRETRYIGNORE) 
  {
    labels = _T("abort$ca retry$ok ignore$ig");
  }
  if((stijlen & 0x0f) == MB_YESNO)            
  {
    labels = _T("yes$ok no$no");
  }
  if((stijlen & 0x0f) == MB_YESNOCANCEL)
  {
    labels = _T("yes$ok no$no cancel$ca");
  }
  if((stijlen & 0x0f) == MB_RETRYCANCEL)
  {
    labels = _T("retry$ok cancel$ca");
  }
  MakeLabelTextsAndStyles(labels);

  if(stijlen & 0x0f0)
  {
    // (MB_ICONHAND | MB_ICONQUESTION | MB_ICONEXCLAMATION | MB_ICONASTERISK)
    int stijl = stijlen & 0x0f0;
    if(stijl == MB_ICONASTERISK)     m_signalBMP = _T("INFORMATION");
    if(stijl == MB_ICONEXCLAMATION)  m_signalBMP = _T("EXCLAMATION");
    if(stijl == MB_ICONHAND)         m_signalBMP = _T("STOP");
    if(stijl == MB_ICONQUESTION)     m_signalBMP = _T("QUESTION");
    m_image.ZetAfbeelding(m_signalBMP);
    m_onlyOK = false;
    // Give the correct beep, according to the style
    MessageBeep(stijl);
  }
  // Possibly set another default button.
  if(stijlen & 0xf00)
  {
    if((stijlen & 0xf00) == MB_DEFBUTTON1) m_default = ID_OFFSET + 0;
    if((stijlen & 0xf00) == MB_DEFBUTTON2) m_default = ID_OFFSET + 1;
    if((stijlen & 0xf00) == MB_DEFBUTTON3) m_default = ID_OFFSET + 2;
  }
}

// Destructor
AuMessageBox::~AuMessageBox()
{
  SuppressForever();
  ResetButtons();
  if (m_ownsFont)
  {
    delete m_font;
  }
}

// Init buttons
void
AuMessageBox::InitButtons()
{
  for(int i = 0;i < MAX_LABELS; ++i)
  {
    m_button[i] = NULL;
  }
}

// Reset the button controls
void
AuMessageBox::ResetButtons()
{
  for(int i = 0;i < MAX_LABELS; ++i)
  {
    delete m_button[i];
    m_button[i] = NULL;
  }
  if(m_suppress)
  {
    delete m_suppress;
    m_suppress = NULL;
  }
}

// Split the label text into individual labels
// ok -> ok
// "yes no" -> "yes", "no" 
// Also extract the button type from the label
// "buttontext$ab" -> label = "buttontext" style = "ab"
//
void
AuMessageBox::MakeLabelTextsAndStyles(CString& p_labels)
{
  // Reset all
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    m_label[i] = CString(_T(""));
    m_style[i] = CString(_T(""));
    m_width[i] = 0;
  }
  // Walk through the label string
  CString rest = p_labels;
  int spatiePos = 0;
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    spatiePos = rest.Find(_T(' '));
    if(spatiePos > 0)
    {
      m_label[i] = rest.Left(spatiePos);
      rest = rest.Mid(spatiePos + 1);
      rest.TrimLeft();
    }
    else
    {
      m_label[i] = rest;
    }
    rest.TrimLeft();
    if(m_label[i].Find(_T('$')) > 0)
    {
      int pos = m_label[i].Find(_T('$'));
      m_style[i] = m_label[i].Mid(pos + 1);
      m_label[i] = m_label[i].Left(pos);
    }
    m_label[i].Replace(_T('_'),_T(' '));
    if(spatiePos < 0)
    {
      break;
    }
  }

  // Look for the default button (only the first one is found)
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_label[i].IsEmpty())
    {
      if(m_label[i].GetAt(0) == _T('@'))
      {
        m_default  = i + ID_OFFSET;
        m_label[i] = m_label[i].Mid(1);
        break;
      }
    }
  }
  // Set the first character to uppercase (exception = OK)
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_label[i].IsEmpty())
    {
      if(m_label[i] == _T("ok"))
      {
        m_label[i] = _T("OK");
      }
      else
      {
        m_label[i].SetAt(0,(TCHAR)_totupper(m_label[i].GetAt(0)));
      }
    }
  }
  // If no styles are found
  // but there are labels
  // Then look for the button style in controlsinfo
  ControlsInfo* info = theApp.GetControlsInfo();
  for (int i = 0; i < MAX_LABELS; ++i)
  {
    if(m_style[i].IsEmpty())
    {
      UINT ID = info->ResourceIDFromNaam(m_label[i]);
      if(ID)
      {
        commandInfo* com = info->ZoekCommandInfo(ID);
        if(com)
        {
          m_style[i] = com->code;
        }
      }
    }
  }
  // Check for only an OK button
  if((m_label[0].CompareNoCase(_T("ok")) == 0) && m_label[1].IsEmpty())
  {
    if(m_signalBMP.IsEmpty())
    {
      m_onlyOK = true;
    }
  }
}

void
AuMessageBox::OnOK()
{
  EndDialog(GetFocus()->GetDlgCtrlID());
}

// Translate the button ID of the result
// IDCANCEL = ESC   key
// ID       = ID of the control
CString
AuMessageBox::GetResult(int p_id)
{
  CString resultaat;
  if((p_id >= ID_OFFSET) && (p_id < (ID_OFFSET + MAX_LABELS)))
  {
    int label = p_id - ID_OFFSET;
    if(label >=0 && label < MAX_LABELS)
    {
      resultaat = m_label[label];
    }
  }
  else
  {
    if(p_id == IDCANCEL)
    {
      // Pressed the ESCape key
      resultaat = _T("");
    }
  }
  resultaat.MakeLower();
  return resultaat;
}

// Give the result of a standard message
int
AuMessageBox::GetResultID(int p_id)
{
  int altOK = 0;
  switch(m_styles & 0x0f)
  {
    case MB_OK:          if(p_id == (ID_OFFSET + 0)) return IDOK;
                         break;
    case MB_YESNO:       altOK = IDYES;
                         if(p_id == (ID_OFFSET + 0)) return IDYES;
                         if(p_id == (ID_OFFSET + 1)) return IDNO;
                         break;
    case MB_RETRYCANCEL: altOK = IDRETRY;
                         if(p_id == (ID_OFFSET + 0)) return IDRETRY;
                         if(p_id == (ID_OFFSET + 1)) return IDCANCEL;
                         break;
    case MB_OKCANCEL:    if(p_id == (ID_OFFSET + 0)) return IDOK;
                         if(p_id == (ID_OFFSET + 1)) return IDCANCEL;
                         break;
    case MB_YESNOCANCEL: altOK = IDYES;
                         if(p_id == (ID_OFFSET + 0)) return IDYES;
                         if(p_id == (ID_OFFSET + 1)) return IDNO;
                         if(p_id == (ID_OFFSET + 2)) return IDCANCEL;
                         break;
    case MB_ABORTRETRYIGNORE: 
                         altOK = IDABORT;
                         if(p_id == (ID_OFFSET + 0)) return IDABORT;
                         if(p_id == (ID_OFFSET + 1)) return IDRETRY;
                         if(p_id == (ID_OFFSET + 2)) return IDIGNORE;
                         break;
  }
  if(p_id == IDCANCEL)
  {
    // Pressed the ESCape key
    return p_id;
  }
  // Unknown result
  return 0;
}

CString
AuMessageBox::ReturnStandardPositive()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase(_T("ok")) == 0)
      {
        return m_label[i];
      }
    }
  }
  return _T("");
}

CString
AuMessageBox::ReturnStandardNegative()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase(_T("ca")) == 0)
      {
        return m_label[i];
      }
    }
  }
  return _T("");
}

int
AuMessageBox::ReturnStandardPositiveID()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase(_T("ok")) == 0)
      {
        return i + ID_OFFSET;
      }
    }
  }
  return 0;
}

int
AuMessageBox::ReturnStandardNegativeID()
{
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_style[i].IsEmpty())
    {
      if(m_style[i].CompareNoCase(_T("ca")) == 0)
      {
        return i + ID_OFFSET;
      }
    }
  }
  return 0;
}

/*************************************************************\
*                                                             *
*   Creating and drawing the message                          *
*                                                             *
\*************************************************************/

// Recalculate the entire message dialog when it starts
BOOL
AuMessageBox::OnInitDialog()
{
  CDialog::OnInitDialog();


  m_font = new CFont;
  LOGFONT lf = ControlsInfo::MaakLOGFONTVanString(_T(""));
  m_font->CreateFontIndirect( &lf );
  m_ownsFont = true;

  SetWindowText(m_title); 

  // Get the control and set the font
  CEdit* edit = (CEdit*) GetDlgItem(IDC_AUMESSAGEBOX);
  edit->SetFont(m_font);

  // GetDC does NOT provide a dc with the correct font.
  // Therefore, also set the font here for the calculation.
  // All calculations must then also use this dc.
  CDC* dc = edit->GetDC();
  dc->SelectObject(m_font);

  // Determine message text
  CString text = m_messsage;

  // Replace all occurrences of \n with \r\n
  text.Replace(_T("\r\n"), _T("\n"));
  text.Replace(_T("\n"), _T("\r\n"));


  // Determine width/height
  CRect tekstRect = CRect(0, 0, 0, 0);
  tekstRect.right = GetSystemMetrics(SM_CXSCREEN) * 90 / 100;
  dc->DrawText(text, &tekstRect, 
               DT_CALCRECT|DT_LEFT|DT_NOPREFIX|DT_WORDBREAK|DT_EXPANDTABS|DT_EDITCONTROL);

  // Add the internal margins of the edit control to the dimensions of the text part,
  // so that the edit control is large enough to also contain its own margins
  CRect margins;
  edit->GetRect(margins);
  tekstRect.right += margins.left * 2;
  tekstRect.bottom += margins.top * 2;

  // Do not let it become too tall, otherwise place a scrollbar
  int maxHeight = GetSystemMetrics(SM_CYSCREEN) * 80 / 100;
  if(tekstRect.bottom > maxHeight)
  {
    edit->ModifyStyle(0, WS_VSCROLL);
    tekstRect.bottom = maxHeight;
    tekstRect.right += GetSystemMetrics(SM_CXHTHUMB);
  }

  // Add fixed offsets (frames)
  tekstRect.OffsetRect(OFFSET,OFFSET);

  // Information icon
  if(!m_signalBMP.IsEmpty())
  {
    // Make extra space on the left (32 pixels) for the image
    tekstRect.OffsetRect(32 + OFFSET,0);
    if(tekstRect.bottom < 32)
    {
      // Vertically center the text
      tekstRect.bottom = 32 + OFFSET;
    }
  }

  // Position and set the text
  edit->MoveWindow(tekstRect);
  edit->SetWindowText(text);

  // Create the buttons
  // For the width we use the width of 
  // a "W", for the height the actual font height.
  CSize tsize = dc->GetTextExtent(_T("W"));
  int buttonTop    = tekstRect.bottom + OFFSET;
  int buttonWidth  = (int)(ButtonWidthFactor * tsize.cx + (3 * OFFSET));
  int buttonHeight = tsize.cy + 4 * GetSystemMetrics(SM_CYFIXEDFRAME);
  int totalWidth   = OFFSET;

  // Determine the layout of the buttons
  int layout = theApp.GetButtonLayout();
  if(layout == BUTT_LAYOUT_IMAGE)
  {
    buttonWidth = buttonHeight;
  }
  if(layout & (BUTT_LAYOUT_TOP | BUTT_LAYOUT_BOTTOM))
  {
    buttonHeight += 16;
    buttonWidth  -= (3 * OFFSET);
  }
  if(layout & BUTT_LAYOUT_NONE)
  {
    buttonWidth -= (3 * OFFSET);
  }
  m_buttonRect.left   = 0;
  m_buttonRect.top    = 0;
  m_buttonRect.right  = buttonWidth;
  m_buttonRect.bottom = buttonHeight;
  // Calculate the total width of all buttons together
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    if(!m_label[i].IsEmpty())
    {
      if(totalWidth > OFFSET) 
      {
        totalWidth += OFFSET;
      }
      m_width[i] = buttonWidth;
      if(layout != BUTT_LAYOUT_IMAGE)
      {
        int breedte = (((m_label[i].GetLength() * tsize.cx) * 2) / 3) + (3 * OFFSET);
        if(breedte > buttonWidth)
        {
          m_width[i] = breedte;
        }
      }
      totalWidth += m_width[i];
    }
  }
  // Calculate the starting position of the buttons.
  // If the text is wider than the buttons, use the text as the total width
  int buttonBegin = OFFSET;
  if(tekstRect.right > totalWidth)
  {
    buttonBegin += tekstRect.right - totalWidth;
    totalWidth   = tekstRect.right;
  }
  // Create the buttons
  long buttonStyle = BS_OWNERDRAW | WS_TABSTOP | BS_NOTIFY | WS_CHILD | WS_VISIBLE;
  for(int i = 0; i < MAX_LABELS; ++i)
  {
    CString sButtonTekst = m_label[i];
    if(!sButtonTekst.IsEmpty())
    {
      if(sButtonTekst.Find(_T('&')) < 0)
      {
        sButtonTekst = _T("&") + sButtonTekst;
      }
      buttonWidth = m_width[i];
      CRect rect(buttonBegin,buttonTop,buttonBegin + buttonWidth,buttonTop + buttonHeight);
      m_button[i]  = new AuButton(m_style[i]);
      m_button[i]->Create(sButtonTekst  // Button text
                         ,buttonStyle                 // MS-Windows window style
                         ,rect                        // Rectangle
                         ,this                        // My child
                         ,i + ID_OFFSET);             // CtrlID of this button
      m_button[i]->SetFont(m_font);
      // For the next button
      buttonBegin += buttonWidth + OFFSET;
    }
  }
  // Recalculate the size of the window
  totalWidth += OFFSET;
  int totalHeight = buttonTop + OFFSET + buttonHeight;
  if(m_onlyOK)
  {
    int hcb = tsize.cy + 2*GetSystemMetrics(SM_CYFIXEDFRAME);
    int wcb = tsize.cx + 2*GetSystemMetrics(SM_CXFIXEDFRAME);
    totalHeight += hcb + OFFSET;

    CString sNietMeerHerhalen = _T("Do not show again      ");

    // Determine the width of the text
    CRect textRect = CRect(0, 0, 0, 0);
    dc->DrawText(sNietMeerHerhalen, &textRect,DT_CALCRECT|DT_LEFT|DT_NOPREFIX|DT_SINGLELINE|DT_EXPANDTABS);


    // Create checkbox
    int top = buttonTop + buttonHeight + 2 * OFFSET;
    m_line.left   = OFFSET;
    m_line.top    = top - OFFSET;
    m_line.right  = wcb + textRect.Width() - OFFSET;
    m_line.bottom = m_line.top;
    CRect brect(OFFSET,top,m_line.right,top + hcb);
    totalWidth = max(totalWidth, wcb + textRect.Width() + OFFSET);


    m_suppress = new AD_Checkbox();

    m_suppress->Create(sNietMeerHerhalen
                       ,WS_TABSTOP | BS_AUTOCHECKBOX | BS_NOTIFY | WS_CHILD | WS_VISIBLE
                       ,brect
                       ,this    
                       ,ID_ONDERDRUKKEN);
    m_suppress->SetFont(m_font);
  }

  CRect rect(0, 0, totalWidth, totalHeight);
  AdjustWindowRect(rect,GetStyle(),false);
  MoveWindow(rect);
  CenterWindow(AfxGetMainWnd());

  return TRUE;
}

void
AuMessageBox::OnPaint()
{
  CDialog::OnPaint();
  
  CDC* dc = GetDC();
  int saveDC = dc->SaveDC();

  // Draw the signal
  if(!m_signalBMP.IsEmpty())
  {
    CRect rect(OFFSET,OFFSET,40,40);
    m_image.PaintBitmap(*dc,rect,0,AFB_PAINT_TRANS);
  }
  // Optionally draw the line
  if(m_onlyOK)
  {
    dc->DrawEdge(m_line,EDGE_ETCHED,BF_TOP);
  }
  dc->RestoreDC(saveDC);
}

// Override for suppressible message
INT_PTR
AuMessageBox::DoModal()
{
  // Optionally suppress the message
  //AutoIBSMessageLock lock;
  if(m_onlyOK)
  {
    if(theApp.IsSuppressedMessage(m_messsage))
    {
      return IDOK;
    }
  }
  // Request attention, only if we are not already the foreground window.
  CWnd *w = GetForegroundWindow();
  bool bFlash = true;
  while(w)
  {
    if(w == AfxGetMainWnd())
    {
      bFlash = false;
      break;
    }
    w = w->GetParent();
  }

  if(AfxGetMainWnd() && bFlash)
  {
    AfxGetMainWnd()->FlashWindowEx(FLASHW_TRAY, 3, 0);
  }
  // Show dialog
  return CDialog::DoModal();
}

void
AuMessageBox::OnKeyDown(UINT nChar, 
                     UINT nRepCnt, 
                     UINT nFlags)
{
  CheckTheAction(nChar);
  CDialog::OnKeyDown(nChar,nRepCnt,nFlags);
}

// Check if the pressed key matches the first character
// of one of our button labels
void
AuMessageBox::CheckTheAction(UINT nChar)
{
  if(nChar == _T(' '))
  {
    // Space selects the current button with focus
    AuButton* wnd = (AuButton *) GetFocus();
    for(int i = 0; i < MAX_LABELS; ++i)
    {
      if(m_button[i] == wnd)
      {
        EndDialog(i + ID_OFFSET);
      }
    }
  }
  for(int i = 0;i < MAX_LABELS; ++i)
  {
    // Otherwise, find the label that starts with this letter
    if(!m_label[i].IsEmpty())
    {
      if(_totlower(m_label[i].GetAt(0)) == _totlower(nChar))
      {
        EndDialog(i + ID_OFFSET);
        return;
      }
    }
  }
}

// We press this button
// Only a hit if the mouse is still over the button
void
AuMessageBox::PressOnButton(UINT p_id,CPoint point)
{
  if(p_id >= ID_OFFSET && p_id < ID_OFFSET + MAX_LABELS)
  {
    m_buttonRect.right = m_width[p_id - ID_OFFSET];
    if(m_buttonRect.PtInRect(point))
    {
      EndDialog(p_id);
    }
  }
}

// Request from the button whether it is the default button
// Also handle initial focus
bool
AuMessageBox::GetDefault(UINT ID)
{
  if(!m_def_done)
  {
    // This is the first moment after the dialog has started and
    // before the first button is drawn. Quickly set the focus.
    m_def_done = true;
    if(m_default)
    {
      GotoDlgCtrl(m_button[m_default - ID_OFFSET]);
    }
  }
  if(ID == (UINT)m_default)
  {
    return true;
  }
  return false;
}

// Check box handler for suppressing
void
AuMessageBox::OnNotAgain()
{
  m_notAgain = !m_notAgain;
}

// Never show the current message again, always OK.
void
AuMessageBox::SuppressForever()
{
  if(m_notAgain)
  {
    theApp.SuppressMessage(m_messsage);
  }
}
