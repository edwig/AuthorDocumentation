// MISC.CPP
//
// Miscaleneous functions for CssStyleSheet
//
#include "stdafx.h"
#include "css.h"
#include <math.h>

bool
CssStyleSheet::escaped(const XString &istring, const size_t pos)
{
  if(pos <= 0)
  {
    return false;
  }
	return !(s_at(istring,pos-1) != _T('\\') || escaped(istring,pos-1));
}

// Save replacement for .at()
TCHAR
CssStyleSheet::s_at(const XString &istring, const size_t pos)
{
	if(pos > (istring.length()-1) && pos < 0)
	{
		return 0;
	}
	else
	{
		return istring[pos];
	}
}

vector<XString>
CssStyleSheet::explode(const XString e,XString s, const bool check)
{
	vector<XString> ret;
	size_t iPos = s.find(e, 0);
	size_t iPit = e.length();

	while(iPos > -1)
	{
		if(iPos != 0 || check)
		{
			ret.push_back(s.substr(0,iPos));
		}
		s.erase(0,iPos+iPit);
		iPos = s.find(e, 0);
	}

 	if(s != _T("") || check)
 	{
		ret.push_back(s);
	}
	return ret;
}

XString
CssStyleSheet::implode(const XString e,const vector<XString> s)
{
	XString ret;
	for(size_t i = 0; i < s.size(); i++)
	{
		ret += s[i];
		if(i != s.size()-1) 
    {
      ret += e;
    }
	}
	return ret;
}

float 
CssStyleSheet::round(const float &number, const int num_digits)
{
  float doComplete5i, doComplete5(number * powf(10.0f, (float) (num_digits + 1)));
    
  if(number < 0.0f)
  {
    doComplete5 -= 5.0f;
  }
  else
  {
    doComplete5 += 5.0f;
  }
  doComplete5 /= 10.0f;
  modff(doComplete5, &doComplete5i);
    
  return doComplete5i / powf(10.0f, (float) num_digits);
}


XString
CssStyleSheet::str_replace(const XString find, const XString replace, XString str)
{
  size_t len         = find.length();
  size_t replace_len = replace.length();
  size_t pos         = str.find(find);

  while(pos != XString::npos)
	{
    str.replace(pos, len, replace);
    pos = str.find(find, pos + replace_len);
  }
  return str;
}

XString
CssStyleSheet::str_replace(const vector<XString>& find, const XString replace, XString str)
{
	size_t replace_len = replace.length();

	for(size_t i = 0; i < find.size(); ++i)
	{
	  size_t len = find[i].length();
	  size_t pos = str.find(find[i]);

	  while(pos != XString::npos)
		{
	    str.replace(pos, len, replace);
	    pos = str.find(find[i], pos + replace_len);
    }
	}
  return str;
}


bool 
CssStyleSheet::in_char_arr(const TCHAR* haystack, const TCHAR needle)
{
	for(size_t i = 0; i < _tcslen(haystack); ++i)
	{
		if(haystack[i] == needle)
		{
			return true;
		}
	}
	return false;
}

bool
CssStyleSheet::in_str_array(const XString& haystack, const TCHAR needle)
{
	return (haystack.find_first_of(needle,0) != XString::npos);
}

bool
CssStyleSheet::in_str_array(const vector<XString>& haystack, const XString needle)
{
	for(size_t i = 0; i < haystack.size(); ++i)
	{
		if(haystack[i] == needle)
		{
			return true;
		}
	}
	return false;
}

XString
CssStyleSheet::htmlspecialchars(XString istring, int quotes)
{
	istring = str_replace(_T("&"),_T("&amp;"),istring);
	istring = str_replace(_T("<"),_T("&lt;"), istring);
	istring = str_replace(_T(">"),_T("&gt;"), istring);
	if(quotes > 0) istring = str_replace(_T("\""),_T("&quot;"),istring);
	if(quotes > 1) istring = str_replace(_T("'"), _T("&#039;"),istring);
	return istring;
}

int 
CssStyleSheet::cssmax(const int i1, const int i2)
{
	if(i1 > i2)
	{
		return i1;
	}
	else
	{
		return i2;
	}
}

bool 
CssStyleSheet::ctype_space(const TCHAR c)
{
	return (c == _T(' ') || c == _T('\t') || c == _T('\r') || c == _T('\n') || c == 11);
}

bool 
CssStyleSheet::ctype_digit(const TCHAR c)
{
	return (c == _T('0') || c == _T('1') || c == _T('2') || c == _T('3') || c == _T('4') || c == _T('5') || c == _T('6') || c == _T('7') || c == _T('8') || c == _T('9'));
}

vector<XString>
CssStyleSheet::unserialise_sa(const XString istring)
{
	size_t _tcslen = istring.length();
	size_t strpos = 0;
	vector<XString> ret;

	while(_tcslen > 0)
	{
		XString digit_tmp = _T("");
		for(size_t i = strpos; ctype_digit(s_at(istring,i)); i++)
		{
			digit_tmp += istring[i];
			--_tcslen; ++strpos;
		}
		// :
		--_tcslen; ++strpos;

		size_t next_length = static_cast<size_t>(str2f(digit_tmp));
		next_length += strpos;

		XString string_tmp = _T("");
		for(size_t i = strpos; (i < istring.length() && i < next_length); i++)
		{
			string_tmp += istring[i];
			--_tcslen; ++strpos;
		}
		ret.push_back(string_tmp);
	}
	return ret;
}

XString
CssStyleSheet::serialise_sa(const XString istring)
{
	return f2str((float) istring.length()) + _T(":") + istring;
}

bool 
CssStyleSheet::ctype_xdigit(TCHAR c)
{
	c = chartolower(c);
	return (ctype_digit(c) || c == _T('a') || c == _T('b') || c == _T('c') || c == _T('d') || c == _T('e') || c == _T('f'));
}

bool 
CssStyleSheet::ctype_alpha(TCHAR c)
{
	c = chartolower(c);
	return (c == _T('a') || c == _T('b') || c == _T('c') || c == _T('d') || c == _T('e') || c == _T('f') || c == _T('g') || 
          c == _T('h') || c == _T('i') || c == _T('j') || c == _T('k') || c == _T('l') || c == _T('m') || c == _T('n') || 
          c == _T('o') || c == _T('p') || c == _T('q') || c == _T('r') || c == _T('s') || c == _T('t') || c == _T('u') || 
          c == _T('v') || c == _T('w') || c == _T('x') || c == _T('y') || c == _T('z'));
}


// CONVERSIONS

XString
CssStyleSheet::strtolower(XString istring)
{
  int str_size = (int) istring.length();
  for(int i = 0; i < str_size; i++)
  {
    istring[i] = chartolower(istring[i]);
  }
  return istring;
}

TCHAR 
CssStyleSheet::chartolower(const TCHAR c)
{
  switch(c)
  {
  case _T('A'): return _T('a');
  case _T('B'): return _T('b');
  case _T('C'): return _T('c');
  case _T('D'): return _T('d');
  case _T('E'): return _T('e');
  case _T('F'): return _T('f');
  case _T('G'): return _T('g');
  case _T('H'): return _T('h');
  case _T('I'): return _T('i');
  case _T('J'): return _T('j');
  case _T('K'): return _T('k');
  case _T('L'): return _T('l');
  case _T('M'): return _T('m');
  case _T('N'): return _T('n');
  case _T('O'): return _T('o');
  case _T('P'): return _T('p');
  case _T('Q'): return _T('q');
  case _T('R'): return _T('r');
  case _T('S'): return _T('s');
  case _T('T'): return _T('t');
  case _T('U'): return _T('u');
  case _T('V'): return _T('v');
  case _T('W'): return _T('w');
  case _T('X'): return _T('x');
  case _T('Y'): return _T('y');
  case _T('Z'): return _T('z');
  default: return c;
  }
}

XString
CssStyleSheet::strtoupper(XString istring)
{
  int str_size = (int) istring.length();
  for(int i = 0; i < str_size; i++)
  {
    istring[i] = chartoupper(istring[i]);
  }
  return istring;
}

TCHAR
CssStyleSheet::chartoupper(const TCHAR c)
{
  if(c >= _T('a') && c <= _T('z'))
  {
    return c - (_T('a') - _T('A'));
  }
  else
  {
    return c;
  }
}

/* Didn't find any usable function for this, so here is my version :) */
XString
CssStyleSheet::dechex(const int i)
{
  XString result;
  int number(i);

  do
  {
    int remainder = number % 16;
    if(remainder < 10)
    {
      result += XString((TCHAR)(remainder + _T('0')),1);
    }
    else
    {
      result += XString((TCHAR)(remainder + _T('a') - 10),1);
    }
    number /= 16;
  }
  while(number);

  return result;
}

double
CssStyleSheet::hexdec(XString istring)
{
  double ret = 0;
  istring = trim(istring);
  for(size_t i = istring.length()-1; i >= 0; --i)
  {
    int num = 0;
    switch(_totlower(istring[i]))
    {
      case _T('a'): num = 10; break;
      case _T('b'): num = 11; break;
      case _T('c'): num = 12; break;
      case _T('d'): num = 13; break;
      case _T('e'): num = 14; break;
      case _T('f'): num = 15; break;
      case _T('1'): num = 1;  break;
      case _T('2'): num = 2;  break;
      case _T('3'): num = 3;  break;
      case _T('4'): num = 4;  break;
      case _T('5'): num = 5;  break;
      case _T('6'): num = 6;  break;
      case _T('7'): num = 7;  break;
      case _T('8'): num = 8;  break;
      case _T('9'): num = 9;  break;
      case _T('0'): num = 0;  break;
    }
    ret += num * pow((double) 16, (double) istring.length()-i-1);
  }
  return ret;
}

XString
CssStyleSheet::f2str(const float f)
{
  XString buffer;

  buffer.Format(_T("%f"),f);
  buffer.TrimRight(_T('0'));
  buffer.TrimRight(_T('.'));

  return buffer;
}

float
CssStyleSheet::str2f(const XString istring)
{
  return (float) _tstof(istring.c_str());
}

XString
CssStyleSheet::char2str(const TCHAR c)
{
  return XString(c,1);
}

XString
CssStyleSheet::char2str(const TCHAR *c)
{
  return XString(c);
}

// TRIM

const XString
CssStyleSheet::trim(const XString istring)
{
  XString::size_type first = istring.find_first_not_of(_T(" \n\t\r\0xb"));
  if (first == XString::npos)
  {
    return XString();
  }
  else
  {
    XString::size_type last = istring.find_last_not_of(_T(" \n\t\r\0xb"));
    return istring.substr( first, last - first + 1);
  }
}

const XString
CssStyleSheet::ltrim(const XString istring)
{
  XString::size_type first = istring.find_first_not_of(_T(" \n\t\r\0xb"));
  if (first == XString::npos)
  {
    return XString();
  }
  else
  {
    return istring.substr( first );
  }
}


const XString
CssStyleSheet::rtrim(const XString istring)
{
  XString::size_type last = istring.find_last_not_of(_T(" \n\t\r\0xb")); /// must succeed
  return istring.substr( 0, last + 1);
}

const XString
CssStyleSheet::rtrim(const XString istring, const XString chars)
{
  XString::size_type last = istring.find_last_not_of(chars); /// must succeed
  return istring.substr( 0, last + 1);
}

XString
CssStyleSheet::strip_tags(XString istring)
{
  bool intag = false;
  XString new_string;

  for(size_t i = 0; i < istring.length(); i++)
  {
    if(istring[i] != _T('<') && !intag)
    {
      new_string += istring[i];
    }
    if(istring[i] == _T('<') && !intag)
    {
      intag = true;
    }
    if(istring[i] == _T('>') && intag)
    {
      intag = false;
    }			
  }
  return new_string;
}

// IMPORTANT

bool
CssStyleSheet::is_important(XString value)
{
  // Remove whitespaces
  value = rtrim(strtolower(value));

  if(value.length() > 9 && value.substr(value.length()-9,9) == _T("important"))
  {
    value = rtrim(value.substr(0,value.length()-9));
    if(value.substr(value.length()-1,1) == _T("!")) 
    {
      return true;
    }
  }
  return false;
}


XString
CssStyleSheet::gvw_important(XString value)
{
  if(is_important(value))
  {
    value = trim(value);
    value = value.substr(0,value.length()-9);
    value = trim(value);
    value = value.substr(0,value.length()-1);
    value = trim(value);
  }
  return value;
}

XString
CssStyleSheet::c_important(XString value)
{
  if(is_important(value))
  {
    value = gvw_important(value) + _T(" !important");
  }
  return value;
}
