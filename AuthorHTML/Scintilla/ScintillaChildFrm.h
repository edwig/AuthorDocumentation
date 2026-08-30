/////////////////////////////////////////////////////////////////////////////
//
// interface of the ScintillaChildFrame class
//
/////////////////////////////////////////////////////////////////////////////

#pragma once

class ScintillaChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(ScintillaChildFrame)
public:
	ScintillaChildFrame();
  // Overrides
  virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
 // virtual void OnUpdateFrameMenu(BOOL bActivate, CWnd* pActivateWnd, HMENU hMenuAlt);

public:
	virtual ~ScintillaChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
  virtual BOOL    OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
          int     OnCreate(LPCREATESTRUCT lpCreateStruct);
  DECLARE_MESSAGE_MAP()
  afx_msg void    OnSysCommand(UINT nID,LPARAM lParam);
  afx_msg void    OnDestroy ();
  afx_msg void    OnMDIActivate (BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
  afx_msg LRESULT OnSetText (WPARAM, LPARAM lParam);
	afx_msg void    OnMove(int x, int y);
	afx_msg void    OnSize(UINT nType, int cx, int cy);
};
