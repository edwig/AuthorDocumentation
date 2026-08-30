// CSS STYLE SHEET
//
// Main methods of the CssStyleSheet class
//
#include "stdafx.h"
#include "Css.h"
#include <fstream>

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
  string contents = file_get_contents(m_filename);
  if(contents == "")
  {
    if(emptyOK)
    {
      return true;
    }
    string warn = "Empty CSS file or no file found: " + m_filename;
    log(warn,Error);
    return false;
  }
  parse_css(contents);
  return true;
}

// Add stylesheet tokens unparsed
//
void CssStyleSheet::add_token(const token_type ttype, const string data, const bool force)
{
	if(m_settings["preserve_css"] || force)
  {
		token temp;
		temp.type = ttype;
		temp.data = (ttype == COMMENT) ? data : trim(data);
		m_csstokens.push_back(temp);
	}
}

void CssStyleSheet::copy(const string media, const string selector, const string media_new, const string selector_new)
{
	for(int k = 0; k < m_css[media][selector].size(); k++)
	{	
		string property = m_css[media][selector].at(k);
		string value = m_css[media][selector][property];
		put(media_new,selector_new,property,value);
	}
}

// A real workhorse.
// ADDS/DELETES/SETS a media/selector/property to a value
//
void CssStyleSheet::put(const string& media, const string& selector, const string& property, const string& value)
{
	if(m_settings["preserve_css"]) 
  {
		return;
	}
	
  string val = trim(value);
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

string
CssStyleSheet::get(const string& media, const string& selector, const string& property)
{
  if(m_settings["preserve_css"]) 
  {
    // Cannot get unparsed data
    return "";
  }
  string value;

  if(m_css[media][selector].has(property))
  {
    value = m_css[media][selector][property];
  }
  return value;
}

void
CssStyleSheet::GetSelectors(const string& media,vector<string>* selectors)
{
	for(css_struct::iterator i = m_css.begin(); i != m_css.end(); ++i )
	{
    if(i->first == media)
    {
		  for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
		  {
        string selector = j->first;
        selectors->push_back(selector);
      }
    }
  }
}

// Get all properties from a media/selector pair
void
CssStyleSheet::GetProperties(const string& media
                            ,const string& selector
                            ,vector<string>* properties)
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
            string property = k->first;
            properties->push_back(property);
          }
        }
      }
    }
  }
}

// Delete a specific selector
void
CssStyleSheet::del_selector(const string& media,const string& selector)
{
  if(m_css[media].has(selector))
  {
    m_css[media].erase(selector);
  }
}

void
CssStyleSheet::GetAllAttributes(vector<string>* attributes)
{
  map<string,string>::iterator i;
  for(i = m_all_properties.begin();i != m_all_properties.end(); ++i)
  {
    string attrib = i->first;
    attributes->push_back(attrib);
  }
}

void CssStyleSheet::log(const string msg, const message_type type, int iline)
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
string CssStyleSheet::unicode(string& istring,int& i)
{
	++i;
	string add = "";
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
		string msg = "Replaced unicode notation: Changed \\" + rtrim(add) + " to ";
		add = static_cast<int>(hexdec(add));
		msg += add;
		log(msg,Information);
		replaced = true;
	}
	else
	{
		add = trim("\\" + add);
	}

	if(ctype_xdigit(istring[i+1]) && ctype_space(istring[i]) && !replaced || !ctype_space(istring[i]))
	{
		i--;
	}
	
	if(add != "\\" || !m_settings["remove_bslash"] || in_str_array(m_tokens,istring[i+1]))
	{
		return add;
	}
	if(add == "\\")
	{
		log("Removed unnecessary backslash",Information);
	}
	return "";
}

bool CssStyleSheet::is_token(string& istring,const size_t i)
{
	return (in_str_array(m_tokens,istring[i]) && !escaped(istring,i));
}

void CssStyleSheet::merge_4value_shorthands(string media, string selector)
{
	for(map< string, vector<string> >::iterator i = m_shorthands.begin(); i != m_shorthands.end(); ++i )
	{
		string temp;

		if(m_css[media][selector].has(i->second[0]) && m_css[media][selector].has(i->second[1])
		&& m_css[media][selector].has(i->second[2]) && m_css[media][selector].has(i->second[3]))
		{
			string important = "";
			for(int j = 0; j < 4; ++j)
			{
				string val = m_css[media][selector][i->second[j]];
				if(is_important(val))
				{
					important = " !important";
					temp += gvw_important(val)+ " ";
				}
				else
				{
					temp += val + " ";
				}
				m_css[media][selector].erase(i->second[j]);
			}
			put(media, selector, i->first, shorthand(trim(temp + important)));		
		}
	}
} 

map<string,string> CssStyleSheet::dissolve_4value_shorthands(string property, string value)
{
	map<string, string> ret;
	
	if(m_shorthands[property][0] == "0")
	{
		ret[property] = value;
		return ret;
	}
	
	string important = "";
	if(is_important(value))
	{
		value = gvw_important(value);
		important = " !important";
	}
	vector<string> values = explode(" ",value);

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
  if (m_settings["merge_selectors"] == 1)
  {
    vector<string> new_sels;
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
CssStyleSheet::SetTemplate(string value)
{
  if(value == "high" || value == "highest" || value == "low")
  {
    m_csstemplate = m_predefined_templates[value];
  }
  else if(value != "default")
  {
    string tpl_content = file_get_contents(value);
    if(tpl_content != "")
    {
      vector<string> tpl_arr = explode("|",tpl_content,true);
      m_csstemplate = tpl_arr;
    }
  }
}

void
CssStyleSheet::AddImport(string value)
{
  m_import.push_back(value);
}

void
CssStyleSheet::DeleteImport(string value)
{
  vector<string>::iterator it;
  it = std::find(m_import.begin(),m_import.end(),value);
  if(it != m_import.end())
  {
    m_import.erase(it);
  }
}

bool
CssStyleSheet::Valid()
{
  if(m_css.empty() && m_charset == "" && m_namesp == "" && m_import.empty() && m_csstokens.empty())
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

string 
CssStyleSheet::file_get_contents(const string filename)
{
  ifstream file_input(filename.c_str(),ios::binary);
  string line, file_contents = "";
  bool firstline = true;

  if(file_input.bad())
  {
    return "";
  }
  else
  {
    while(file_input.good())
    {
      getline(file_input,line);
      if(firstline)
      {
        firstline = false;
        check_bom(line);
      }
      file_contents += (line + "\n");
    }	    
  }
  file_input.close();

  return file_contents;
}

// Check byte order mark
void
CssStyleSheet::check_bom(string& line)
{
  do 
  {
    unsigned char ch = line.front();
    if(ch == 0xEF || ch == 0xBB || ch == 0xBF ||  // UTF-8
       ch == 0xFF || ch == 0xFE)                  // UTF-16
    {
      line.erase(0,1);
    }
    else
    {
      break;
    }
  } 
  while(line.size());
}

// For internal stylesheets
string 
CssStyleSheet::GetTheSheet()
{
  return m_theSheet;
}
