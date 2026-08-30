// Basis UI
// 
//
#include <afxcolorbutton.h>

#define AD_Button      CButton   // Derived from CXTButton
#define AD_Checkbox    CButton   // Standard MFC checkbox
#define AD_Radiobox    CButton   // Standard MFC radiobox
#define AD_ColorButton CMFCColorButton

// All color buttons are styled thusly
#define STYLE_COLOR_BUTTONS CPS_XT_EXTENDED | CPS_XT_MORECOLORS | CPS_XT_SHOW3DSELECTION | CPS_XT_SHOWHEXVALUE