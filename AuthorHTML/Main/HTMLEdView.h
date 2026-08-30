//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    HTMLEdView.h
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Iimplementation of the CHTMLEdView class
// VIEW with the MSHMTL control for a HTML page
//
// This is the main view for a HTML editor
// Contains and calls all the other HTML stuff.
//
#pragma  once
#include "StdAfx.h"
#include "HTMLEdDoc.h"
#include "AuthorHTMLControlSite.h"
#include "HTMLDocEditDesigner.h"
#include "FindReplaceDialog.h"
#include "HTMLTableCell.h"
#include "FormDlg.h"
#include "FormFieldDlg.h"
#include "FormButtonDlg.h"
#include "FormTextareaDlg.h"
#include "FormImageDlg.h"
#include "FormLabelDlg.h"
#include "FormSelectDlg.h"
#include "StyleSheetsDlg.h"
#include "IMGControlSite.h"

//interface IDAuthorHTML;

typedef std::multimap<CString,IHighlightSegment*> HighLights;

class CHTMLEdView : public CHtmlEditView
{
protected: 
  // create from serialization only
	CHTMLEdView();
 ~CHTMLEdView();

	DECLARE_DYNCREATE(CHTMLEdView)
	//DECLARE_DHTMLEDITING_CMDMAP(CHTMLEdView);
// Attributes
public:
	CHTMLEdDoc* GetDocument();
  // Operations
public:
  // Overrides
	public:
	virtual void OnInitialUpdate();
  HRESULT GetDHtmlDocument(IHTMLDocument2** doc);
protected:
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

  // Implementation
public:
  CString GetBase();
  CString RegisteredURL();
  CString RebasedURL();
  bool    DoSave();
  bool    IsComplete();
  void    LoadGlyphs(int type,bool tags,bool paragraph);
	void    OnUpdateColor(CCmdUI *pCmdUI);
	void    OnDisplayChanged();
	virtual HRESULT OnShowContextMenu(DWORD dwID,
							  LPPOINT ppt,
							  LPUNKNOWN pcmdtReserved,
							  LPDISPATCH pdispReserved);
	virtual HRESULT OnUpdateUI();

  // Overrides
  int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	void UpdateView();
  void Navigate(LPCTSTR t_URL
               ,DWORD dwFlags               = 0
               ,LPCTSTR lpszTargetFrameName = NULL
               ,LPCTSTR lpszHeaders         = NULL
               ,LPVOID lpvPostData          = NULL
               ,DWORD dwPostDataLen         = 0);

  void OnBeforeNavigate2(LPCTSTR     t_URL
                        ,DWORD       u32_Flags
                        ,LPCTSTR     t_TargetFrame
                        ,CByteArray& i_PostedData
                        ,LPCTSTR     t_Headers
                        ,BOOL*       pb_Cancel);
  HRESULT OnDocumentReadyStateChange(IHTMLElement* phtmlElem);
  void    OnNavigateComplete2(LPCTSTR t_URL);
  void    OnDocumentComplete(LPDISPATCH pDisp, LPCTSTR szUrl);
  void    OnDownloadComplete();

  void    SetClassName(CString name);
  void    PasteTextHTML(CString p_text);
  void    SetOurGenerator();
  virtual BOOL CreateControlSite(COleControlContainer* pContainer,
                                 COleControlSite** ppSite,
                                 UINT nID, REFCLSID clsid);
  void    SpellCheckOnIdle();
  void    ReleaseHighlight(CString& word);
  // Check for Popup SCRIPT section in head
  void    CheckPopupSCRIPT();
  // Renaming of files
  int     RenameReferences(CString& p_old_href,CString& p_new_href);
  int     RenameReferencesTag(CString& p_old_href,CString& p_new_href,char* p_tag);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  virtual BOOL    PreTranslateMessage(MSG* pMsg);

// Generated message map functions
public:
  void OnProperties();
protected:
  void OnSpecialTest();
  void OnSpecialTest2();
	void PopColorMenu();
	void UpdateFontCombos();
  void ShowTableBorders();
  void UpdateStructure (CString& p_tagname,CString& p_classname);
  void UpdateClassCombo(CString& p_tagname,CString& p_classname);
  HtmlElement GetBody();
  BOOL OnDeleteKey();
  BOOL OnBackspaceKey();
  void OnSoftHyphen();
  BOOL OnDiacrite(WPARAM letter);
  CString GetMetaTag(CString p_name);
	void OnUpdateToolbarButtons(CCmdUI* pCmdUI);
  void OnUpdateTableBorders(CCmdUI *pCmdUi);
  void OnUpdateModeTags(CCmdUI *pCmdUI);
  void OnUpdateParagraph(CCmdUI *pCmdUI);
  void OnUpdateModeBrowse(CCmdUI *pCmdUI);
  void OnUpdateTableCommands(CCmdUI *pCmdUi);
  void OnUpdateComment(CCmdUI *pCmdUi);
  void OnUpdateFormatParagraph(CCmdUI *pCmdUi);
  void OnUpdateGrid(CCmdUI *pCmdUi);
  void OnUpdateSnap(CCmdUI *pCmdUi);
  void OnUpdateForm(CCmdUI *pCmdUi);
  void OnUpdateFormElements(CCmdUI *pCmdUi);
  void OnUpdateUISelection(CCmdUI* pCmdUI);
  void OnUpdateUINoSelection(CCmdUI* pCmdUI);
  void OnUpdateInsertKey();
  void OnTableBorders();
  void OnInsertKey();
  void OnViewTags();
  void OnViewParagraph();
  void OnViewBrowse();
  void OnTableInsert();
  void OnTableInsertRow(BOOL b_Below);
  void OnTableInsertRowBelow();
  void OnTableInsertRowAbove();
  void OnTableInsertColumn(BOOL b_Right);
  void OnTableInsertColumnBefore();
  void OnTableInsertColumnAfter();
  void OnTableDeleteRow();
  void OnTableDeleteColumn();
  void OnTableCombine();
  void OnTableSplitCell();
  void OnTableProperties();
  void OnTableProperties(CComPtr<IHTMLElement> pElement);
  void OnCellProperties();
  void OnCellProperties(CComPtr<IHTMLElement> pElement);
  void OnAnchor();
  void OnImage();
  BOOL OnDoubleClick(int xPos,int yPos);
  BOOL OnEditElement            (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickAnchor      (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickArea        (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickParagraph   (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHyperlink   (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickImage       (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickComment     (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickMarquee     (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickObject      (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickAcronym     (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickAddress     (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickButton      (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickCaption     (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickDiv         (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFieldset    (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFont        (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickForm        (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFormField   (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFormButton  (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFormInput   (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFormImage   (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFormTextArea(CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFormLabel   (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickFormSelect  (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHeading1    (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHeading2    (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHeading3    (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHeading4    (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHeading5    (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHeading6    (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickIFrame      (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickInlineTag   (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickLegend      (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickScript      (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickSpan        (CComPtr<IHTMLElement> pElement);
  void OnDoubleClickSpanDiv(CString p_tag,CComPtr<IHTMLElement> pElement);
  void OnDoubleClickHeading(CString p_tag,CComPtr<IHTMLElement> pElement);
  void OnRemoveLayer            (CComPtr<IHTMLElement> pElement);
  void OnRemoveArea             (CComPtr<IHTMLElement> pElement);

  void OnInsertBR();
  void OnInsertHR();
  void OnInsertComment();
  void OnRemoveHyperlink();
  void PasteText    (CString p_text);
  void OnEditComment();
  void OnCellTop();
  void OnCellMiddle();
  void OnCellBottom();
  void OnInsertSymbol();
  bool InTableCell();     // Inside "TD" or "TH"
  bool InTable();         // Inside "TABLE"
  bool InTableRowGroup(); // Inside "TBODY","THEAD" or "TFOOT"
  BOOL GotoNextCell();
  BOOL GotoPreviousCell();
  void OnFind();
  void OnReplace();
  void OnSearchText(bool findOnly);
  void OnMarquee();
  void OnBreakBelowImages();
  void OnEditStyleSheets();
  void OnFormatParagraph();
  void OnShowGrid();
  void OnSnapToGrid();
  void OnGridProperties();
  void OnInsertAcronym();
  void OnInsertAddress();
  void OnUnsupportedHTML(CString tag);
  void OnBig();
  void OnBlockQuote();
  void OnCite();
  void OnCode();
  void OnDeletedText();
  void OnInsertedText();
  void OnKbd();
  void OnPre();
  void OnQuotation();
  void OnSample();
  void OnSmaller();
  void OnSubscript();
  void OnSuperscript();
  void OnTeletype();
  void OnParaStyleParagraph();
  void OnParaStyleHeading1();
  void OnParaStyleHeading2();
  void OnParaStyleHeading3();
  void OnParaStyleHeading4();
  void OnParaStyleHeading5();
  void OnParaStyleHeading6();
  void OnParaStyleAddress();
  void OnParaStylePreformat();
  void OnInsertForm();
  void OnInsertFormField();
  void OnInsertFormButton();
  void OnInsertFormTextarea();
  void OnInsertFormImage();
  void OnInsertFormLabel();
  void OnInsertFormSelect();
  void OnInsertFieldset();
  void OnInsertIFrame();
  void OnInsertButton();
  void OnInsertScript();
  void OnInsertObject();
  void OnInsertALink();
  void OnInsertALinkCommand();
  void OnInsertLayer();
  void OnInsertAreaRectangle();
  void OnInsertAreaCircle();
  void OnInsertAreaPolygon();
  void OnSpellCheckDocument();
  void OnSpellOptions();
  void OnBookmark();
  void OnBold();
  void OnItalic();
  void OnUnderline();
  void OnBulletList();
  void OnNumberList();
  void OnFont();
  void OnIndent();
  void OnOutdent();
  void OnPrintPreview();
  void OnPageSetup();
  void OnCut();
  void OnCopy();
  void OnPaste();
  void OnUndo();
  void OnRedo();
  void OnSelectAll();
  void OnNoFormat();
  void OnLeftJustify();
  void OnCenterJustify();
  void OnRightJustify();
  void OnJustify();
  void OnAbsolute();
  void OnStatic();
  void OnUpdateAnchor(CCmdUI* pCmdUI);
  void OnUpdateHyperlink(CCmdUI* pCmdUI);
  void OnUpdateBold(CCmdUI* pCmdUI);
  void OnUpdateItalic(CCmdUI* pCmdUI);
  void OnUpdateUnderline(CCmdUI* pCmdUI);
  void OnUpdateBulletList(CCmdUI* pCmdUI);
  void OnUpdateNumberList(CCmdUI* pCmdUI);
  void OnUpdateIndent(CCmdUI* pCmdUI);
  void OnUpdateOutdent(CCmdUI* pCmdUI);
  void OnUpdatePrintPreview(CCmdUI* pCmdUI);
  void OnUpdatePageSetup(CCmdUI* pCmdUI);
  void OnUpdateCut(CCmdUI* pCmdUI);
  void OnUpdateCopy(CCmdUI* pCmdUI);
  void OnUpdatePaste(CCmdUI* pCmdUI);
  void OnUpdateUndo(CCmdUI* pCmdUI);
  void OnUpdateRedo(CCmdUI* pCmdUI);
  void OnUpdateSelectAll(CCmdUI* pCmdUI);
  void OnUpdateNoFormat(CCmdUI* pCmdUI);
  void OnUpdateLeftJustify(CCmdUI* pCmdUI);
  void OnUpdateCenterJustify(CCmdUI* pCmdUI);
  void OnUpdateRightJustify(CCmdUI* pCmdUI);
  void OnUpdateJustify(CCmdUI* pCmdUI);
  void OnUpdateAbsolute(CCmdUI* pCmdUI);
  void OnUpdateStatic(CCmdUI* pCmdUI);

  DECLARE_MESSAGE_MAP()

private:
  // Generic services
  HRESULT GetCurrentElement(IHTMLElement** element);
  // Are we currently within a tag of this name
  bool    InsideTag(CString tag);
  // Are we currently within a tag of this name, return the element of this tag
  bool    InsideTag(CString tag,CComPtr<IHTMLElement>& pElement);
  // Are we currently within a tag of this name, starting at a specific point
  bool    InsideTag(CComPtr<IHTMLElement> start,CString tag);
  // Are we currently within a tag of this name, starting at a specific point, returning the element of this tag
  bool    InsideTag(CComPtr<IHTMLElement> start,CString tag,CComPtr<IHTMLElement>& pElement);

  // Are we currently around a tag of this name
  bool    OutsideTag(CString tag,CComPtr<IHTMLElement>& pElement);
  // Are we currently around a tag of this name, return the element of this tag
  bool    OutsideTag(CComPtr<IHTMLElement> start,CString tag,CComPtr<IHTMLElement>& pElement);


  SIZE    GetDocumentSize();
  HtmlTableCell GetSelectedCell();
  HtmlTableRow  GetSelectedRow();
  CString GetCurrentSelectionText();
  void    ThemesEnable();
  BOOL    GetDesignMode();
  CString GetAttribute(CComPtr<IHTMLElement> pElement,CString attrib_name);
  void    ExtractStyles();
  void    OnDoubleClickHR(CComPtr<IHTMLElement> pElement);
  int     SearchInCollection(CComPtr<IHTMLElementCollection> coll
                            ,bool&    bFindOnly
                            ,CString& textToFind
                            ,CString& textToReplace
                            ,bool&    bDidNothing
                            ,UINT&    iOptions
                            ,INT_PTR& iActions);
  void    GetStyleSheets(CSSVector& list,bool link);
  void    GetStyleSheets(CStringArray* list);
  // Create a new HTML element of a givven TAG-ID
  CComPtr<IHTMLElement> CreateNewElement(ELEMENT_TAG_ID tag,CString* p_attributes);
  // Translate an area to the underlying image
  void    OnSelectionWarning(CString forTag);
  void    FormDlgToFormElement(FormDlg& dlg,CComPtr<IHTMLFormElement> form);
  void    FormFieldDlgToInputElement (FormFieldDlg&  dlg,CComPtr<IHTMLInputElement> field);
  void    FormButtonDlgToInputElement(FormButtonDlg& dlg,CComPtr<IHTMLInputElement> field);
  void    FormLabelDlgToLabelElement (FormLabelDlg&  dlg,CComPtr<IHTMLLabelElement> label);
  void    FormTextareaDlgToTextAreaElement(FormTextareaDlg* dlg,CComPtr<IHTMLTextAreaElement> area);
  void    FormSelectDlgToSelectElement(FormSelectDlg& dlg,FormOptions& options,CComPtr<IHTMLSelectElement> select);
  bool    SpellCheck(CComPtr<IHTMLElement> pElement,int& errors,int &changed);
  void    ReleaseHighlights();
  void    HighLightSpellErrors();
  void    GetTagsMenu(CMenu* menu,vector<IHTMLElement*>& allTags);
  // Get a unique number for a tag of a specified name (a, div-layer)
  long    GetUniqueID(CString tagname);
  void    CheckBase();
  // MAP/AREA/IMG
  void    ConnectIMGBehaviors();
  void    IMGBehaviorSetMap(IMGControlSite* site,CString map);
  void    AreaMessage(CString type,CString start,CString next);

  // Now editing this file
  CString m_URL;
  CString m_baseURL;
  bool    m_bCleanUp;

  bool    m_bTableBorders;
  bool    m_bTagsMode;
  bool    m_bParaMode;
  bool    m_bDesignMode;
  bool    m_overwriteKey;
  int     m_bookmarkNumber;

  int     m_HITxPos;
  int     m_HITyPos;

  // Dirty (editting done): Do SpellChecking in OnIdle() time
  bool    m_dirty;

  // Jump table for double clicking on tags
  typedef void (CHTMLEdView::*dclickFunction)(CComPtr<IHTMLElement>);
  typedef void (CHTMLEdView::*unsuppFunction)(CString);
  typedef struct _tagJumps
  {
      CString tagname;
      void    (CHTMLEdView::*DoubleClick) (CComPtr<IHTMLElement>);
      void    (CHTMLEdView::*Unsupported) (CString);
      CString message;
  }
  TagJumps;
  static  TagJumps jumps[];

  // Is our document complete (Navigate2 and OnDownloadComplete)
  bool m_complete;
  // Control site for our document
  CHTMLEditControlSite* m_pEditSite;
  // This is our EditDesigner
  HTMLDocEditDesigner m_designer;
  // THIS IS OUR DOCUMENT
	CComQIPtr<IHTMLDocument2> m_Doc2;
  // Map with highLight segements for spelling errors
  HighLights m_highLights;
};

#ifndef _DEBUG  // debug version in HTMLEdView.cpp
inline CHTMLEdDoc* CHTMLEdView::GetDocument()
{
  return (CHTMLEdDoc*)m_pDocument; 
}
#endif

inline CString
CHTMLEdView::GetBase()
{
  return m_baseURL;
}

inline bool
CHTMLEdView::IsComplete()
{
  return m_complete;
}