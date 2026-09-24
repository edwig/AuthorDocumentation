// PARSE CSS
//
// Parse a CSS stylesheet as read in from ASCII file into
// the internal structures
//
#include "stdafx.h"
#include "css.h"

#pragma warning (disable: 4503)

using namespace std;

/* is = in selector
 * ip = in property
 * iv = in value
 * instr = in string (-> ",',( => ignore } and ; etc.)
 * ic = in comment (ignore everything)
 * at = in @-block
 */

void CssStyleSheet::parse_css(XString css_input)
{
	m_input_size = (int)css_input.length();
	css_input = str_replace(_T("\r\n"),_T("\n"),css_input); // Replace all double-newlines
	css_input += _T("\n");
	parse_status status = is;
  parse_status from   = is;
	m_cur_property = _T(""); // if you can explain the need for this please do so

	XString temp_add,cur_comment,temp;

	vector<XString> cur_sub_value_arr;
	TCHAR str_char   = _T(' ');
	bool str_in_str = false;
	bool invalid_at = false;
	bool pn = false;

	int str_size = (int) css_input.length();
	for(int i = 0; i < str_size; ++i)
	{
		if(css_input[i] == _T('\n') || css_input[i] == _T('\r'))
		{
			++m_line;
		}
		
		switch(status)
		{
			/* Case in-at-block */
			case at:
			if(is_token(css_input,i))
			{
				if(css_input[i] == _T('/') && s_at(css_input,i+1) == _T('*'))
				{
					i += 2;
					status = ic;
					from = at;
				}
				else if(css_input[i] == _T('{'))
				{
					status = is;
					add_token(AT_START, m_cur_at);
				}
				else if(css_input[i] == _T(','))
				{
					m_cur_at += trim(m_cur_at) + _T(",");
				}
				else if(css_input[i] == _T('\\')) 
				{
					m_cur_at += unicode(css_input,i);
				}
			}
			else
			{
				int lastpos = (int)m_cur_at.length()-1;
				if(lastpos == -1 || !( (ctype_space(m_cur_at[lastpos]) || is_token(m_cur_at,lastpos) && m_cur_at[lastpos] == _T(',')) && ctype_space(css_input[i])))
				{
					m_cur_at += css_input[i];
				}
			}
			break;
			
			/* Case in-selector */
			case is:
			if(is_token(css_input,i))
			{
				if(css_input[i] == _T('/') && s_at(css_input,i+1) == _T('*') && trim(m_cur_selector) == _T(""))
				{
					status = ic; ++i;
					from = is;
				}
				else if(css_input[i] == _T('@') && trim(m_cur_selector) == _T(""))
				{
					// Check for at-rule
					invalid_at = true;
					for(map<XString,parse_status>::iterator j = m_at_rules.begin(); j != m_at_rules.end(); ++j )
					{
						if(strtolower(css_input.substr(i+1,j->first.length())) == j->first)
						{
							(j->second == at) ? m_cur_at = _T("@") + j->first : m_cur_selector = _T("@") + j->first;
							status = j->second;
							i += (int) j->first.length();
							invalid_at = false;
						}
					}
					if(invalid_at)
					{
						m_cur_selector = _T("@");
						XString invalid_at_name = _T("");
						for(int j = i+1; j < str_size; ++j)
						{
							if(!ctype_alpha(css_input[j]))
							{
								break;
							}
							invalid_at_name += css_input[j];
						}
						log(_T("Invalid @-rule: ") + invalid_at_name + _T(" (removed)"),Warning);
					}
				}
				else if(css_input[i] == _T('"') || css_input[i] == _T('\''))
				{
					m_cur_selector += css_input[i];
					status   = instr;
					str_char = css_input[i];
					from     = is;
				}
				else if(invalid_at && css_input[i] == _T(';'))
				{
					invalid_at = false;
					status = is;
				}
				else if(css_input[i] == _T('{'))
				{
					status = ip;
					add_token(SEL_START, m_cur_selector);
					++m_selectors;
				}
				else if(css_input[i] == _T('}'))
				{
					add_token(AT_END, m_cur_at);
					m_cur_at = _T("");
					m_cur_selector = _T("");
					m_sel_separate = vector<int>();
				}
				else if(css_input[i] == _T(',')) 
				{
					m_cur_selector = trim(m_cur_selector) + _T(",");
					m_sel_separate.push_back((int)m_cur_selector.length());
				}
				else if(css_input[i] == _T('\\')) 
				{
					m_cur_selector += unicode(css_input,i);
				}
				else m_cur_selector += css_input[i];
			}
			else
			{
        size_t lastpos = 0;
        size_t length = m_cur_selector.length();
        if(length > 0)
        {
          lastpos = m_cur_selector.length() - 1;
        }
			  if(!((length && ctype_space(m_cur_selector[lastpos]) || (length && is_token(m_cur_selector,lastpos) && m_cur_selector[lastpos] == _T(','))) && (length && ctype_space(css_input[i]))))
			  {
				  m_cur_selector += css_input[i];
			  }
			}
			break;
			
			/* Case in-property */
			case ip:
			if(is_token(css_input,i))
			{
				if(css_input[i] == _T(':') || css_input[i] == _T('=') && m_cur_property != _T("")) // IE really accepts =, so CssStyleSheet will fix those mistakes
				{
					status = iv;
					bool valid = (m_all_properties.count(m_cur_property) > 0 && m_all_properties[m_cur_property].find(m_css_level,0) != XString::npos);
					if(valid || !m_settings[_T("discard_invalid_properties")]) {
						add_token(PROPERTY, m_cur_property);
					}
				}
				else if(css_input[i] == _T('/') && s_at(css_input,i+1) == _T('*') && m_cur_property == _T(""))
				{
					status = ic; ++i;
					from = ip;
				}
				else if(css_input[i] == _T('}'))
				{
					explode_selectors();
					status = is;
					invalid_at = false;
					add_token(SEL_END, m_cur_selector);
					m_cur_selector = _T("");
					m_cur_property = _T("");
				}
				else if(css_input[i] == _T(';'))
				{
					m_cur_property = _T("");
				}
				else if(css_input[i] == _T('\\')) 
				{
					m_cur_property += unicode(css_input,i);
				}
			}
			else if(!ctype_space(css_input[i]))
			{
				m_cur_property += css_input[i];
			}
			break;
			
			/* Case in-value */
			case iv:
			pn = ((css_input[i] == _T('\n') || css_input[i] == _T('\r')) && property_is_next(css_input,i+1) || i == str_size-1);
			if(pn)
			{
				log(_T("Added semicolon to the end of declaration"),Warning);
			}
			if(is_token(css_input,i) || pn)
			{
				if(css_input[i] == _T('/') && s_at(css_input,i+1) == _T('*'))
				{
					status = ic; ++i;
					from = iv;
				}
				else if(css_input[i] == _T('"') || css_input[i] == _T('\'') || css_input[i] == _T('('))
				{
					m_cur_sub_value += css_input[i];
					str_char = (css_input[i] == _T('(')) ? _T(')') : css_input[i];
					status = instr;
					from = iv;
				}
				else if(css_input[i] == _T('\\')) 
				{
					m_cur_sub_value += unicode(css_input,i);
				}
				else if(css_input[i] == _T(';') || pn)
				{
					if(m_cur_selector.substr(0,1) == _T("@") && m_at_rules.count(m_cur_selector.substr(1)) > 0 && m_at_rules[m_cur_selector.substr(1)] == iv)
					{
						cur_sub_value_arr.push_back(trim(m_cur_sub_value));
						status = is;
						
						if(m_cur_selector == _T("@charset"))   m_charset = cur_sub_value_arr[0];
						if(m_cur_selector == _T("@namespace")) m_namesp = implode(_T(" "),cur_sub_value_arr);
						if(m_cur_selector == _T("@import"))    m_import.push_back(implode(_T(" "),cur_sub_value_arr));
		
						cur_sub_value_arr.clear();
						m_cur_sub_value = _T("");
						m_cur_selector = _T("");
						m_sel_separate = vector<int>();
					}
					else
					{
						status = ip;
					}
				}
				else if(css_input[i] != _T('}'))
				{
					m_cur_sub_value += css_input[i];
				}
				if( (css_input[i] == _T('}') || css_input[i] == _T(';') || pn) && !m_cur_selector.empty())
				{
					++m_properties;
					
					if(m_cur_at == _T(""))
					{
						m_cur_at = _T("standard");
					}
	
					// Kill all whitespace
					m_cur_at = trim(m_cur_at); m_cur_selector = trim(m_cur_selector);
					m_cur_value = trim(m_cur_value); m_cur_property = trim(m_cur_property);
					m_cur_sub_value = trim(m_cur_sub_value);
					
					// case m_settings
					if(m_settings[_T("lowercase_s")])
					{
						m_cur_selector = strtolower(m_cur_selector);
					}
					m_cur_property = strtolower(m_cur_property);
					
					
					if(m_cur_sub_value != _T(""))
					{
						m_cur_sub_value = optimise_subvalue(m_cur_sub_value,m_cur_property);
						cur_sub_value_arr.push_back(m_cur_sub_value);
						m_cur_sub_value = _T("");
					}
	
					m_cur_value = implode(_T(" "),cur_sub_value_arr);
					
					// Compress !important
					temp = c_important(m_cur_value);
					if(temp != m_cur_value)
					{
						log(_T("Optimised !important"),Information);
					}
					m_cur_value = temp;
					
					// Optimise shorthand properties
					if(m_shorthands.count(m_cur_property) > 0)
					{
						temp = shorthand(m_cur_value);
						if(temp != m_cur_value)
						{
							log(_T("Optimised shorthand notation (") + m_cur_property + _T("): Changed \"") + m_cur_value + _T("\" to \"") + temp + _T("\""),Information);
						}
						m_cur_value = temp;
					}
					
					// Compress font-weight (tiny compression)
					if(m_cur_property == _T("font-weight") && m_settings[_T("compress_font-weight")])
					{
						int c_fw = c_font_weight(m_cur_value);
						if(c_fw == 400)
						{
							log(_T("Optimised font-weight: Changed \"bold\" to \"700\""),Information);
						}
						else if(c_fw == 700)
						{
							log(_T("Optimised font-weight: Changed \"normal\" to \"400\""),Information);
						}
					}
					
					bool valid = (m_all_properties.count(m_cur_property) > 0 && m_all_properties[m_cur_property].find(m_css_level,0) != XString::npos);
					if((!invalid_at || m_settings[_T("preserve_css")]) && (!m_settings[_T("discard_invalid_properties")] || valid))
					{
						put(m_cur_at,m_cur_selector,m_cur_property,m_cur_value);
						add_token(VALUE, m_cur_value);
							
						// Further Optimisation
						if(m_cur_property == _T("background") && m_settings[_T("optimise_shorthands")] > 1)
						{
							map<XString,XString> temp2 = dissolve_short_bg(m_cur_value);
							m_css[m_cur_at][m_cur_selector].erase(_T("background"));
							for(map<XString,XString>::iterator it = temp2.begin(); it != temp2.end(); ++it )
							{
								put(m_cur_at,m_cur_selector,it->first,it->second);
							}
						}
						if(m_shorthands.count(m_cur_property) > 0 && m_settings[_T("optimise_shorthands")] > 0)
						{
							map<XString,XString> temp3 = dissolve_4value_shorthands(m_cur_property,m_cur_value);
							for(map<XString,XString>::iterator it = temp3.begin(); it != temp3.end(); ++it )
							{
								put(m_cur_at,m_cur_selector,it->first,it->second);
							}
							if(m_shorthands[m_cur_property][0] != _T("0"))
							{
								m_css[m_cur_at][m_cur_selector].erase(m_cur_property);
							}
						}
					}
					if(!valid)
					{
						if(m_settings[_T("discard_invalid_properties")])
						{
							log(_T("Removed invalid property: ") + m_cur_property,Warning);
						}
						else
						{
							log(_T("Invalid property in ") + strtoupper(m_css_level) + _T(": ") + m_cur_property,Warning);
						}
					}
					
					//Split multiple selectors here if necessary								
					m_cur_property = _T("");
					cur_sub_value_arr.clear();
					m_cur_value = _T("");
				}
				if(css_input[i] == _T('}'))
				{
					explode_selectors();
					add_token(SEL_END, m_cur_selector);
					status = is;
					invalid_at = false;
					m_cur_selector = _T("");
				}
			}
			else if(!pn)
			{
				m_cur_sub_value += css_input[i];
			
				if(ctype_space(css_input[i]))
				{
					if(trim(m_cur_sub_value) != _T(""))
					{
						m_cur_sub_value = optimise_subvalue(m_cur_sub_value,m_cur_property);
						cur_sub_value_arr.push_back(trim(m_cur_sub_value));
					}
					m_cur_sub_value = _T("");
				}
			}
			break;
			
			/* Case in-string */
			case instr:
			if(str_char == _T(')') && css_input[i] == _T('"') && str_in_str == false && !escaped(css_input,i))
			{
				str_in_str = true;
			}
			else if(str_char == _T(')') && css_input[i] == _T('"') && str_in_str == true && !escaped(css_input,i))
			{
				str_in_str = false;
			}
			if(css_input[i] == str_char && !escaped(css_input,i) && str_in_str == false)
			{
				status = from;
			}
			temp_add = _T(""); temp_add += css_input[i];
			if( (css_input[i] == _T('\n') || css_input[i] == _T('\r')) && !(css_input[i-1] == _T('\\') && !escaped(css_input,i-1)) )
			{
				temp_add = _T("\\A ");
				log(_T("Fixed incorrect newline in string"),Warning);
			}
			if(from == iv)
			{
				m_cur_sub_value += temp_add;
			}
			else if(from == is)
			{
				m_cur_selector += temp_add;
			}
			break;
			
			/* Case in-comment */
			case ic:
			if(css_input[i] == _T('*') && s_at(css_input,i+1) == _T('/'))
			{
				status = from;
				++i;
				add_token(COMMENT, cur_comment);
				cur_comment = _T("");
			}
			else
			{
				cur_comment += css_input[i];
			}
			break;
		}
	}

	if(m_settings[_T("merge_selectors")] > 1)
	{
		for(css_struct::iterator i = m_css.begin(); i != m_css.end(); i++ )
		{
			merge_selectors(i->second);
		}
	}

	if(m_settings[_T("optimise_shorthands")] > 0)
	{
		for(css_struct::iterator i = m_css.begin(); i != m_css.end(); ++i )
		{
			for(sstore::iterator j = i->second.begin(); j != i->second.end();)
			{
				merge_4value_shorthands(i->first,j->first);
				if(m_settings[_T("optimise_shorthands")] > 1) 
        {
					merge_bg(j->second);
				}
				
				if(j->second.size() == 0) 
        {
					i->second.erase(j);
				} 
        else 
        {
					 ++j;
				}
			}		
		}
	}
}

