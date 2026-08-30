// Auhtor HTML Message Box
#pragma once
#include "StdAfx.h"
#include "AuthorImage.h"
#include "AuthorButton.h"

#define ID_ONDERDRUKKEN 1959 // Magic number

#include "resource.h"

class AuMessageBox : CDialog
{
public:
  // Maak door middel van label/stijlen strings
  AuMessageBox(CWnd*  p_parent
              ,LPCSTR p_titel
              ,LPCSTR p_boodschap
              ,CString p_labels);
  // Maak door middel van MB_* stijlen
  AuMessageBox(CWnd* parent
              ,LPCSTR titel
              ,LPCSTR boodschap
              ,int    stijlen);
  // Destructor
  ~AuMessageBox();
  // Koppeling aan de resources
  enum { IDD = IDD_AUMESSAGEBOX };

  // De modal-loop: later zelf herschrijven?
  virtual INT_PTR DoModal();
  // Geef het resultaat als string ("ok","ja","nee" etc)
  CString GeefResultaat(int p_id);
  // Geef het resultaat als een ID (IDOK, IDYES etc)
	int    GetResultID(int p_id);
  // Standaard Postief antwoord (ok)
  CString GeefStandaardPositief();
  // Standaard negatief antwoord (no, cancel, annuleer)
  CString GeefStandaardNegatief();
  // Standaard Positief antwoord via ID
  int    GeefStandaardPositiefID();
  // Standaard negatief antwoord via ID
  int    GeefStandaardNegatiefID();
  // Controleer of dit ID de default knop is
  bool   GeefDefault(UINT ID);
  // We drukken op deze knop
  void   DrukOpKnop(UINT p_id,CPoint point);

  // Er komt een key binnen
  void OnKeyDown(UINT nChar,UINT nRepCnt,UINT nFlags);
  // Heeft een message map
  DECLARE_MESSAGE_MAP();
  // Override van de message loop -> Voor message box problemen
  // LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
  // Herteken de dialoog
  afx_msg void OnPaint();
  // Op de niet-meer check box gelicked
  afx_msg void OnNietMeer();

  // Sluit de dialoog en druk op de huidige knop. Dit wordt alleen aangeroepen
  // als de gebruiker op Enter drukt, want we hebben geen knoppen met id IDOK!
  void OnOK();

private:
  // METHODEN
  // Initialisatie van de dialoog
  virtual BOOL OnInitDialog();
  // Initialize button controls
  void InitButtons();
  // Reset de button controls
  void ResetButtons();
  // Splits een labelstring naar labels voor knoppen
  void MaakLabelTekstenEnStijlen(CString& p_labels);
  // Controleer een keyhit op een knop
  void ControleerActie(UINT nChar);
  // Huidige boodschap nooit meer tonen, altijd OK.
  void OnderdrukVoortaan();

  // DATA
  CFont* m_font;            // Font waarin we het tonen
  bool   m_ownsFont;        // Font wissen bij destructie?
  CString m_titel;          // Titel van het dialoog
  CString m_boodschap;      // Dit willen we tonen
  int    m_stijlen;         // signaal stijlen
  int    m_default;         // Deze knop is de default knop
  bool   m_def_gedaan;      // Default focus gedaan
  CString m_afbeelding;     // Signal this bitmap
  AuthorImage m_image;      // Signaal plaatje 
  CRect     m_tekstRect;    // Hier komt de boodschap
  CRect     m_buttonRect;   // Grootte van een knop
  CButtonC* m_button[5];    // Max = 5 Buttons
  CString   m_label[5];     // Max = 5 labels
  CString   m_style[5];     // Max = 5 styles
  int       m_breed[5];     // Max = 5 knop breedtes
 
  // Voor boodschap register
  bool         m_alleenOK;      // Aleen een OK knop
  AD_Checkbox* m_onderdruk;     // Onderdruk dialoog
  CRect        m_lijn;          // Lijn ter onderverdeling
  bool         m_nietMeer;      // Niet meer tonen
};

