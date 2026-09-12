//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    CSS_utils.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Utility definitions and functions for CSS manipulations
//
#include "StdAfx.h"
#include "AuthorHTML.h"
#include "CSS_utils.h"
#include "CSS.h"
#include <math.h>

CString
CSSPropertyGet(CssStyleSheet* css
              ,string& selector
              ,string  property
              ,bool unspec /*=true*/)
{
  string media = "standard";
  CString value;

  value = css->get(media,selector,property).c_str();
  if(unspec && value.IsEmpty())
  {
    value = "unspecified";
  }
  if(isalpha(value.GetAt(0)))
  {
    value.MakeLower();
    value.SetAt(0,(char)toupper(value[0]));
  }
  return value;
}

void
CSSPropertyPut(CssStyleSheet* css
              ,string& selector
              ,string  p_property
              ,CString value
              ,bool    unspec)
{
  string media = "standard";
  string val   = value;

  if(unspec && value.CompareNoCase("Unspecified") == 0)
  {
    val = "";
  }
  css->put(media,selector,p_property,val);
}

void
CssSplitValueUnits(CString p_property,CString& value,CString& units)
{
  value = "";
  units = "";
  if(p_property.IsEmpty())
  {
    return;
  }
  static char* digitsep = "+-.,";
  if(isdigit(p_property[0]) || strchr(digitsep,p_property[0]))
  {
    while(p_property.GetLength()>0 && 
         (isdigit(p_property[0]) || strchr(digitsep,p_property[0])))
    {
      value += p_property[0];
      p_property = p_property.Mid(1);
    }
    units = p_property;
  }
  else
  {
    // Parse through (smaller,x-etc)
    value = p_property;
  }
}

CComBSTR
CssCtoB(CString value)
{
  if(value.CompareNoCase("Unspecified") == 0)
  {
    value= "";
  }
  CComBSTR bString = CT2CW(value);
  return bString;
}

//////////////////////////////////////////////////////////////////////////
//
// See: https://developer.mozilla.org/en-US/docs/Learn_web_development/Core/Styling_basics/Values_and_units
//
//////////////////////////////////////////////////////////////////////////

CString
CssConvertToUnit(CString  p_value
                ,CString  p_units
                ,CString& p_origUnit
                ,bool     p_warning  /* =false */)
{
  CString value;
  CString units;
  double  convert = 0.0;

  // Split value and unit definition
  CssSplitValueUnits(p_value,value,units);

  // Possibly preserve the original units
  if(p_origUnit && !units.IsEmpty())
  {
    p_origUnit = units;
  }

  // No conversion possible, or not needed
  if(units.IsEmpty() || units.Compare(p_units) == 0)
  {
    if(units.IsEmpty())
    {
      p_origUnit = p_units;
    }
    return value;
  }

  // No conversion is possible for a percentage of the browser
  // the viewport-width or the viewport-height
  // And also not for the "ex" and "em" space of the relative (root) font.
  if(units == "%"    || p_units == "%"    ||  // Percentage
     units == "vw"   || p_units == "vw"   ||  // Viewport-width
     units == "vh"   || p_units == "vh"   ||  // Viewport-height
     units == "ex"   || p_units == "ex"   ||  // Height of font's 'x'
     units == "em"   || p_units == "em"   ||  // Font m size in width
     units == "rem"  || p_units == "rem"  ||  // em size of the root font
     units == "vmin" || p_units == "vmin" ||  // Viewport minimum (height / width)
     units == "vmax" || p_units == "vmax" )   // Viewport maximum (height / width)
  {
    if(p_warning)
    {
      theApp.Panic("\nWARNING !!\n\n"
                   "You requested to convert a absolute to a relative unit of measurement! Or vice versa!\n"
                   "This cannot be done on the current value of the CSS property. The units are switched,"
                   "but you yourself are responsible to provide the correct new value!");
    }
    // Possibly preserve the original units
    if(p_origUnit && !p_units.IsEmpty())
    {
      p_origUnit = p_units;
    }
    return value;
  }

  // Get the value as a floating point
  convert = atof(value.GetString());

  // Convert everything back to inches
  // Please be aware that a pixel density of 96 is ALWAYS assumed !!
  if(units.Compare("in"))
  {
         if(units.Compare("cm") == 0) convert /= 2.54;
    else if(units.Compare("mm") == 0) convert /= 25.4;
    else if(units.Compare("Q")  == 0) convert /= 101.6;
    else if(units.Compare("pc") == 0) convert /= 6;
    else if(units.Compare("pt") == 0) convert /= 72;
    else if(units.Compare("px") == 0) convert /= 96;
  }

  // Convert to requested unit
  // Please be aware that a pixel density of 96 is ALWAYS assumed !!
  if(p_units.Compare("in"))
  {
         if(p_units.Compare("cm") == 0) convert *= 2.54;
    else if(p_units.Compare("mm") == 0) convert *= 25.4;
    else if(p_units.Compare("Q")  == 0) convert *= 101.6;
    else if(p_units.Compare("pc") == 0) convert *= 6;
    else if(p_units.Compare("pt") == 0) convert *= 72;
    else if(p_units.Compare("px") == 0) convert *= 96;
  }

  // Correct by rounding
  if(p_units.Compare("px") == 0)
  {
    // Pixels are always an integer
    convert = round(convert);
    value.Format("%d",static_cast<int>(convert));
  }
  else
  {
    // Measurements in 4 decimal places
    long result = round(convert * 10000 + 1);
    convert = static_cast<double>(result) / 10000;
    value.Format("%.4f",convert);
    value.TrimRight('0');
  }

  // If we came to here, the conversion is done
  // so now we return the new units
  if (p_origUnit && !p_units.IsEmpty())
  {
    p_origUnit = p_units;
  }

  return value;
}

// Setting all the size options in the combobox for a measurement
void
CSSComboBoxUnits(CComboBox& p_combo,CString p_init /*="px"*/)
{
  p_combo.AddString("");        // No units, no values
  // Absolute size
  p_combo.AddString("px");      // Pixels ( 1/96 of an inch)
  p_combo.AddString("in");      // Inch
  p_combo.AddString("cm");      // Centimeter (1/2.54  of an inch)
  p_combo.AddString("mm");      // Millimeter (1/25.4  of an inch)
  p_combo.AddString("Q");       // Quarter    (1/101.6 of an inch)
  p_combo.AddString("pt");      // Point      (1/72    of an inch)
  p_combo.AddString("pc");      // Pica       (1/6     of an inch)
  // Relative size
  p_combo.AddString("%");       // Percentage of the browser viewport
  p_combo.AddString("ex");      // Height of the fonts 'x' character
  p_combo.AddString("em");      // Width of the em of a font
  p_combo.AddString("rem");     // Width of the em of the root font
  p_combo.AddString("vh");      // Percentage of the viewport height
  p_combo.AddString("vw");      // Percentage of the viewport width
  p_combo.AddString("vmin");    // Percentage of the minimum of viewport(width and height)
  p_combo.AddString("vmax");    // Percentage of the maximum of viewport(width and height)

  if(!p_init.IsEmpty())
  {
    int ind = p_combo.FindStringExact(-1,p_init);
    if (ind < 0)
    {
      // If not found, default to "px" pixels
      ind = 1;
    }
    p_combo.SetCurSel(ind);
  }
}
