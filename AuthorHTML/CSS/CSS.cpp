// CSS STYLE SHEET
//
// Main methods of the CssStyleSheet class
//
#include "stdafx.h"
#include "Css.h"
#include <WinFile.h>

#pragma warning (disable: 4503)

// Constructor
// Call prepare to load all data and defaults
//
CssStyleSheet::CssStyleSheet()
{ 
  prepare();
} 

bool
CssStyleSheet::SetFile(LPCTSTR p_filename,bool emptyOK /*=false*/)
{
  m_filename = p_filename;
  XString contents = file_get_contents(m_filename);
  if(contents == _T(""))
  {
    if(emptyOK)
    {
      return true;
    }
    XString warn = _T("Empty CSS file or no file found: ") + m_filename;
    log(warn,Error);
    return false;
  }
  parse_css(contents);
  return true;
}

// Add stylesheet tokens unparsed
//
void CssStyleSheet::add_token(const token_type ttype, const XString data, const bool force)
{
	if(m_settings[_T("preserve_css")] || force)
  {
		token temp;
		temp.type = ttype;
		temp.data = (ttype == COMMENT) ? data : trim(data);
		m_csstokens.push_back(temp);
	}
}

void CssStyleSheet::copy(const XString media, const XString selector, const XString media_new, const XString selector_new)
{
	for(int k = 0; k < m_css[media][selector].size(); k++)
	{
		XString property = m_css[media][selector].at(k);
		XString value = m_css[media][selector][property];
		put(media_new,selector_new,property,value);
	}
}

// A real workhorse.
// ADDS/DELETES/SETS a media/selector/property to a value
//
void CssStyleSheet::put(const XString& media, const XString& selector, const XString& property, const XString& value)
{
	if(m_settings[_T("preserve_css")])
  {
		return;
	}

  XString val = trim(value);
	if(m_css[media][selector].has(property))
	{
		if( !is_important(m_css[media][selector][property]) || 
        (is_important(m_css[media][selector][property]) && is_important(value)) )
		{
			m_css[media][selector].erase(property);
      if(!val.empty())
      {
        m_css[media][selector][property] = trim(value);
      }
		}
	}
	else
	{
    if(val.empty())
    {
      m_css[media][selector].erase(property);
    }
    else
    {
      m_css[media][selector][property] = trim(value);
    }
	}
}

XString
CssStyleSheet::get(const XString& media, const XString& selector, const XString& property)
{
  if(m_settings[_T("preserve_css")])
  {
    // Cannot get unparsed data
    return _T("");
  }
  XString value;

  if(m_css[media][selector].has(property))
  {
    value = m_css[media][selector][property];
  }
  return value;
}

void
CssStyleSheet::GetSelectors(const XString& media,vector<XString>* selectors)
{
	for(css_struct::iterator i = m_css.begin(); i != m_css.end(); ++i )
	{
    if(i->first == media)
    {
		  for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
		  {
        XString selector = j->first;
        selectors->push_back(selector);
      }
    }
  }
}

// Get all properties from a media/selector pair
void
CssStyleSheet::GetProperties(const XString& media
                            ,const XString& selector
                            ,vector<XString>* properties)
{
	for(css_struct::iterator i = m_css.begin(); i != m_css.end(); ++i )
	{
    if(i->first == media)
    {
		  for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
		  {
        if(j->first == selector)
        {
          for(pstore::iterator k = j->second.begin(); k != j->second.end(); ++k)
          {
            XString property = k->first;
            properties->push_back(property);
          }
        }
      }
    }
  }
}

// Delete a specific selector
void
CssStyleSheet::del_selector(const XString& media,const XString& selector)
{
  if(m_css[media].has(selector))
  {
    m_css[media].erase(selector);
  }
}

void
CssStyleSheet::GetAllAttributes(vector<XString>* attributes)
{
  map<XString,XString>::iterator i;
  for(i = m_all_properties.begin();i != m_all_properties.end(); ++i)
  {
    XString attrib = i->first;
    attributes->push_back(attrib);
  }
}

void CssStyleSheet::log(const XString msg, const message_type type, int iline)
{
	message new_msg;
	new_msg.m = msg;
	new_msg.t = type;

	if(iline == 0)
	{
		iline = m_line;
	}
	if(m_logs.count(m_line) > 0)
	{
		for(int i = 0; i < (int) m_logs[m_line].size(); ++i)
		{
			if(m_logs[m_line][i].m == new_msg.m && 
         m_logs[m_line][i].t == new_msg.t)
			{
				return;
			}
		}
	}
	m_logs[m_line].push_back(new_msg);
}

#pragma warning(disable:4244)
XString CssStyleSheet::unicode(XString& istring,int& i)
{
	++i;
	XString add = _T("");
	bool replaced = false;
	
	while(i < (int)istring.length() && (ctype_xdigit(istring[i]) || ctype_space(istring[i])) && add.length()< 6)
	{
		add += istring[i];

		if(ctype_space(istring[i]))
		{
			break;
		}
		i++;
	}

	if(hexdec(add) > 47 && hexdec(add) < 58 || hexdec(add) > 64 && hexdec(add) < 91 || hexdec(add) > 96 && hexdec(add) < 123)
	{
		XString msg = _T("Replaced unicode notation: Changed \\") + rtrim(add) + _T(" to ");
		add = static_cast<int>(hexdec(add));
		msg += add;
		log(msg,Information);
		replaced = true;
	}
	else
	{
		add = trim(_T("\\") + add);
	}

	if(ctype_xdigit(istring[i+1]) && ctype_space(istring[i]) && !replaced || !ctype_space(istring[i]))
	{
		i--;
	}
	
	if(add != _T("\\") || !m_settings[_T("remove_bslash")] || in_str_array(m_tokens,istring[i+1]))
	{
		return add;
	}
	if(add == _T("\\"))
	{
		log(_T("Removed unnecessary backslash"),Information);
	}
	return _T("");
}

bool CssStyleSheet::is_token(XString& istring,const size_t i)
{
	return (in_str_array(m_tokens,istring[i]) && !escaped(istring,i));
}

void CssStyleSheet::merge_4value_shorthands(XString media, XString selector)
{
	for(map< XString, vector<XString> >::iterator i = m_shorthands.begin(); i != m_shorthands.end(); ++i )
	{
		XString temp;

		if(m_css[media][selector].has(i->second[0]) && m_css[media][selector].has(i->second[1])
		&& m_css[media][selector].has(i->second[2]) && m_css[media][selector].has(i->second[3]))
		{
			XString important = _T("");
			for(int j = 0; j < 4; ++j)
			{
				XString val = m_css[media][selector][i->second[j]];
				if(is_important(val))
				{
					important = _T(" !important");
					temp += gvw_important(val)+ _T(" ");
				}
				else
				{
					temp += val + _T(" ");
				}
				m_css[media][selector].erase(i->second[j]);
			}
			put(media, selector, i->first, shorthand(trim(temp + important)));		
		}
	}
} 

map<XString,XString> CssStyleSheet::dissolve_4value_shorthands(XString property, XString value)
{
	map<XString, XString> ret;

	if(m_shorthands[property][0] == _T("0"))
	{
		ret[property] = value;
		return ret;
	}

	XString important = _T("");
	if(is_important(value))
	{
		value = gvw_important(value);
		important = _T(" !important");
	}
	vector<XString> values = explode(_T(" "),value);

	if(values.size() == 4)
	{
		for(int i=0; i < 4; ++i)
		{
			ret[m_shorthands[property][i]] = values[i] + important;
		}
	}
	else if(values.size() == 3)
	{
		ret[m_shorthands[property][0]] = values[0] + important;
		ret[m_shorthands[property][1]] = values[1] + important;
		ret[m_shorthands[property][3]] = values[1] + important;
		ret[m_shorthands[property][2]] = values[2] + important;
	}
	else if(values.size() == 2)
	{
		for(int i = 0; i < 4; ++i)
		{
			ret[m_shorthands[property][i]] = ((i % 2 != 0)) ? values[1] + important : values[0] + important;
		}
	}
	else
	{
		for(int i = 0; i < 4; ++i)
		{
			ret[m_shorthands[property][i]] = values[0] + important;
		}	
	}
	
	return ret;
}

void CssStyleSheet::explode_selectors()
{
	// Explode multiple selectors
  if (m_settings[_T("merge_selectors")] == 1)
  {
    vector<XString> new_sels;
    int lastpos = 0;
    m_sel_separate.push_back((int)m_cur_selector.length());
    
    for (int i = 0; i < (int) m_sel_separate.size(); ++i)
    {
      if (i == (int)m_sel_separate.size()-1) 
      {
        m_sel_separate[i] += 1;
      }
      new_sels.push_back(m_cur_selector.substr(lastpos,m_sel_separate[i]-lastpos-1));
      lastpos = m_sel_separate[i];
    }

    if (new_sels.size() > 1)
    {
      for (int i = 0; i < (int) new_sels.size(); ++i)
      {
		    for (pstore::iterator j = m_css[m_cur_at][m_cur_selector].begin(); j != m_css[m_cur_at][m_cur_selector].end(); ++j)
		    {
          put(m_cur_at, new_sels[i], j->first, j->second);
		    }
      }
      m_css[m_cur_at].erase(m_cur_selector);
    }
  }
  m_sel_separate = vector<int>();
}

void
CssStyleSheet::SetTemplate(XString value)
{
  if(value == _T("high") || value == _T("highest") || value == _T("low"))
  {
    m_csstemplate = m_predefined_templates[value];
  }
  else if(value != _T("default"))
  {
    XString tpl_content = file_get_contents(value);
    if(tpl_content != _T(""))
    {
      vector<XString> tpl_arr = explode(_T("|"),tpl_content,true);
      m_csstemplate = tpl_arr;
    }
  }
}

void
CssStyleSheet::AddImport(XString value)
{
  m_import.push_back(value);
}

void
CssStyleSheet::DeleteImport(XString value)
{
  vector<XString>::iterator it;
  it = std::find(m_import.begin(),m_import.end(),value);
  if(it != m_import.end())
  {
    m_import.erase(it);
  }
}	

bool
CssStyleSheet::Valid()
{
  if(m_css.empty() && m_charset == _T("") && m_namesp == _T("") && m_import.empty() && m_csstokens.empty())
  {
    return false;
  }
  return true;
}


bool
CssStyleSheet::has_errors()
{
  if(!Valid())
  {
    // Style sheet is invalid. So it contains errors!
    return true;
  }
  for(map<int, vector<message> >::iterator j = m_logs.begin(); j != m_logs.end(); j++ )
  {
    for(int i = 0; i < (int)j->second.size(); ++i)
    {
      if(j->second[i].t == Error)
      {
        // At least one error found
        return true;
      }
    }
  }
  // No Errors
  return false;
}

XString
CssStyleSheet::file_get_contents(const XString filename)
{
  WinFile file(filename);
  if(!file.Open(winfile_read | open_trans_text))
  {
    return _T("");
  }
  XString line;
  XString file_contents;

  while(file.Read(line))
  {
    file_contents += line;
  }
  file.Close();

  return file_contents;
}

// For internal stylesheets
XString
CssStyleSheet::GetTheSheet()
{
  return m_theSheet;
}
