// MISC.CPP
//
// Miscaleneous functions for CssStyleSheet
//
#include "stdafx.h"
#include "css.h"
#include <math.h>
#include <sstream>

bool 
CssStyleSheet::escaped(const string &istring, const size_t pos) 
{
  if(pos <= 0) 
  {
    return false;
  }
	return !(s_at(istring,pos-1) != '\\' || escaped(istring,pos-1));
}

// Save replacement for .at()
char 
CssStyleSheet::s_at(const string &istring, const size_t pos)
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

vector<string> 
CssStyleSheet::explode(const string e,string s, const bool check)
{
	vector<string> ret;
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
	
 	if(s != "" || check)
 	{
		ret.push_back(s);
	}
	return ret;
}

string 
CssStyleSheet::implode(const string e,const vector<string> s)
{
	string ret;
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


string 
CssStyleSheet::str_replace(const string find, const string replace, string str)
{
  size_t len         = find.length();
  size_t replace_len = replace.length();
  size_t pos         = str.find(find);

  while(pos != string::npos)
	{  
    str.replace(pos, len, replace);
    pos = str.find(find, pos + replace_len);
  }
  return str;
}

string 
CssStyleSheet::str_replace(const vector<string>& find, const string replace, string str)
{
	size_t replace_len = replace.length();
	
	for(size_t i = 0; i < find.size(); ++i)
	{
	  size_t len = find[i].length();
	  size_t pos = str.find(find[i]);

	  while(pos != string::npos)
		{  
	    str.replace(pos, len, replace);
	    pos = str.find(find[i], pos + replace_len);
    }
	}
  return str;
}


bool 
CssStyleSheet::in_char_arr(const char* haystack, const char needle)
{
	for(size_t i = 0; i < strlen(haystack); ++i)
	{
		if(haystack[i] == needle)
		{
			return true;
		}
	}
	return false;
}

bool 
CssStyleSheet::in_str_array(const string& haystack, const char needle)
{
	return (haystack.find_first_of(needle,0) != string::npos);
}

bool 
CssStyleSheet::in_str_array(const vector<string>& haystack, const string needle)
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

string 
CssStyleSheet::htmlspecialchars(string istring, int quotes)
{
	istring = str_replace("&","&amp;",istring);
	istring = str_replace("<","&lt;", istring);
	istring = str_replace(">","&gt;", istring);
	if(quotes > 0) istring = str_replace("\"","&quot;",istring);
	if(quotes > 1) istring = str_replace("'","&#039;", istring);
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
CssStyleSheet::ctype_space(const char c)
{
	return (c == ' ' || c == '\t' || c == '\r' || c == '\n' || c == 11);
}

bool 
CssStyleSheet::ctype_digit(const char c)
{
	return (c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9');
}

vector<string> 
CssStyleSheet::unserialise_sa(const string istring)
{
	size_t strlen = istring.length();
	size_t strpos = 0;
	vector<string> ret;
	
	while(strlen > 0)
	{
		string digit_tmp = "";
		for(size_t i = strpos; ctype_digit(s_at(istring,i)); i++)
		{
			digit_tmp += istring[i];
			--strlen; ++strpos;
		}
		// :
		--strlen; ++strpos;
		
		size_t next_length = static_cast<size_t>(str2f(digit_tmp));
		next_length += strpos;

		string string_tmp = "";
		for(size_t i = strpos; (i < istring.length() && i < next_length); i++)
		{
			string_tmp += istring[i];
			--strlen; ++strpos;
		}
		ret.push_back(string_tmp);
	}
	return ret;
}

string 
CssStyleSheet::serialise_sa(const string istring)
{
	return f2str((float) istring.length()) + ":" + istring;
}

bool 
CssStyleSheet::ctype_xdigit(char c)
{
	c = chartolower(c);
	return (ctype_digit(c) || c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f');
}

bool 
CssStyleSheet::ctype_alpha(char c)
{
	c = chartolower(c);
	return (c == 'a' || c == 'b' || c == 'c' || c == 'd' || c == 'e' || c == 'f' || c == 'g' || 
          c == 'h' || c == 'i' || c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || 
          c == 'o' || c == 'p' || c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'u' || 
          c == 'v' || c == 'w' || c == 'x' || c == 'y' || c == 'z');
}


// CONVERSIONS

string 
CssStyleSheet::strtolower(string istring)
{
  int str_size = (int) istring.length();
  for(int i = 0; i < str_size; i++)
  {
    istring[i] = chartolower(istring[i]);
  }
  return istring;
}

char 
CssStyleSheet::chartolower(const char c)
{
  switch(c)
  {
  case 'A': return 'a';
  case 'B': return 'b';
  case 'C': return 'c';
  case 'D': return 'd';
  case 'E': return 'e';
  case 'F': return 'f';
  case 'G': return 'g';
  case 'H': return 'h';
  case 'I': return 'i';
  case 'J': return 'j';
  case 'K': return 'k';
  case 'L': return 'l';
  case 'M': return 'm';
  case 'N': return 'n';
  case 'O': return 'o';
  case 'P': return 'p';
  case 'Q': return 'q';
  case 'R': return 'r';
  case 'S': return 's';
  case 'T': return 't';
  case 'U': return 'u';
  case 'V': return 'v';
  case 'W': return 'w';
  case 'X': return 'x';
  case 'Y': return 'y';
  case 'Z': return 'z';
  default: return c;
  }
}

string 
CssStyleSheet::strtoupper(string istring)
{
  int str_size = (int) istring.length();
  for(int i = 0; i < str_size; i++)
  {
    istring[i] = chartoupper(istring[i]);
  }
  return istring;
}

char 
CssStyleSheet::chartoupper(const char c)
{
  switch(c)
  {
    case 'a': return 'A';
    case 'b': return 'B';
    case 'c': return 'C';
    case 'd': return 'D';
    case 'e': return 'E';
    case 'f': return 'F';
    case 'g': return 'G';
    case 'h': return 'H';
    case 'i': return 'I';
    case 'j': return 'J';
    case 'k': return 'K';
    case 'l': return 'L';
    case 'm': return 'M';
    case 'n': return 'N';
    case 'o': return 'O';
    case 'p': return 'P';
    case 'q': return 'Q';
    case 'r': return 'R';
    case 's': return 'S';
    case 't': return 'T';
    case 'u': return 'U';
    case 'v': return 'V';
    case 'w': return 'W';
    case 'x': return 'X';
    case 'y': return 'Y';
    case 'z': return 'Z';
    default: return c;
  }
}

/* Didn't find any usable function for this, so here is my version :) */
string 
CssStyleSheet::dechex(const int i)
{
  stringstream sstream;
  sstream << hex << i;
  return sstream.str();
}

double 
CssStyleSheet::hexdec(string istring)
{
  double ret = 0;
  istring = trim(istring);
  for(size_t i = istring.length()-1; i >= 0; --i)
  {
    int num = 0;
    switch(tolower(istring[i]))
    {
      case 'a': num = 10; break;
      case 'b': num = 11; break;
      case 'c': num = 12; break;
      case 'd': num = 13; break;
      case 'e': num = 14; break;
      case 'f': num = 15; break;
      case '1': num = 1;  break;
      case '2': num = 2;  break;
      case '3': num = 3;  break;
      case '4': num = 4;  break;
      case '5': num = 5;  break;
      case '6': num = 6;  break;
      case '7': num = 7;  break;
      case '8': num = 8;  break;
      case '9': num = 9;  break;
      case '0': num = 0;  break;
    }
    ret += num * pow((double) 16, (double) istring.length()-i-1);
  }
  return ret;
}

string 
CssStyleSheet::f2str(const float f)
{
  stringstream sstream;
  sstream << f;
  return sstream.str();
}

float 
CssStyleSheet::str2f(const string istring)
{
  return (float) atof(istring.c_str());
}

string 
CssStyleSheet::char2str(const char c)
{
  string ret = "";
  ret += c;
  return ret;
}

string 
CssStyleSheet::char2str(const char *c)
{
  stringstream sstream;
  sstream << c;
  return sstream.str();
}


// TRIM

const string 
CssStyleSheet::trim(const string istring)
{
  std::string::size_type first = istring.find_first_not_of(" \n\t\r\0xb");
  if (first == std::string::npos) 
  {
    return std::string();
  }
  else 
  {
    std::string::size_type last = istring.find_last_not_of(" \n\t\r\0xb");
    return istring.substr( first, last - first + 1);
  }
}

const string 
CssStyleSheet::ltrim(const string istring)
{
  std::string::size_type first = istring.find_first_not_of(" \n\t\r\0xb");
  if (first == std::string::npos) 
  {
    return std::string();
  }
  else 
  {
    return istring.substr( first );
  }
}


const string 
CssStyleSheet::rtrim(const string istring)
{
  std::string::size_type last = istring.find_last_not_of(" \n\t\r\0xb"); /// must succeed
  return istring.substr( 0, last + 1);
}

const string 
CssStyleSheet::rtrim(const string istring, const string chars)
{
  std::string::size_type last = istring.find_last_not_of(chars); /// must succeed
  return istring.substr( 0, last + 1);
}

string 
CssStyleSheet::strip_tags(string istring)
{
  bool intag = false;
  string new_string;

  for(size_t i = 0; i < istring.length(); i++)
  {
    if(istring[i] != '<' && !intag)
    {
      new_string += istring[i];
    }
    if(istring[i] == '<' && !intag)
    {
      intag = true;
    }
    if(istring[i] == '>' && intag)
    {
      intag = false;
    }			
  }
  return new_string;
}

// IMPORTANT

bool 
CssStyleSheet::is_important(string value)
{
  // Remove whitespaces
  value = rtrim(strtolower(value));

  if(value.length() > 9 && value.substr(value.length()-9,9) == "important")
  {
    value = rtrim(value.substr(0,value.length()-9));
    if(value.substr(value.length()-1,1) == "!") 
    {
      return true;
    }
  }
  return false;
}


string 
CssStyleSheet::gvw_important(string value)
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

string 
CssStyleSheet::c_important(string value)
{
  if(is_important(value))
  {
    value = gvw_important(value) + " !important";
  }
  return value;
}
