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

void CssStyleSheet::parse_css(string css_input)
{
	m_input_size = (int)css_input.length();
	css_input = str_replace("\r\n","\n",css_input); // Replace all double-newlines
	css_input += "\n";
	parse_status status = is;
  parse_status from   = is;
	m_cur_property = ""; // if you can explain the need for this please do so

	string temp_add,cur_comment,temp;

	vector<string> cur_sub_value_arr;
	char str_char   = ' ';
	bool str_in_str = false;
	bool invalid_at = false;
	bool pn = false;

	int str_size = (int) css_input.length();
	for(int i = 0; i < str_size; ++i)
	{
		if(css_input[i] == '\n' || css_input[i] == '\r')
		{
			++m_line;
		}
		
		switch(status)
		{
			/* Case in-at-block */
			case at:
			if(is_token(css_input,i))
			{
				if(css_input[i] == '/' && s_at(css_input,i+1) == '*')
				{
					status = ic; i += 2;
					from = at;
				}
				else if(css_input[i] == '{')
				{
					status = is;
					add_token(AT_START, m_cur_at);
				}
				else if(css_input[i] == ',')
				{
					m_cur_at += trim(m_cur_at) + ",";
				}
				else if(css_input[i] == '\\') 
				{
					m_cur_at += unicode(css_input,i);
				}
			}
			else
			{
				int lastpos = (int)m_cur_at.length()-1;
				if(lastpos == -1 || !( (ctype_space(m_cur_at[lastpos]) || is_token(m_cur_at,lastpos) && m_cur_at[lastpos] == ',') && ctype_space(css_input[i])))
				{
					m_cur_at += css_input[i];
				}
			}
			break;
			
			/* Case in-selector */
			case is:
			if(is_token(css_input,i))
			{
				if(css_input[i] == '/' && s_at(css_input,i+1) == '*' && trim(m_cur_selector) == "")
				{
					status = ic; ++i;
					from = is;
				}
				else if(css_input[i] == '@' && trim(m_cur_selector) == "")
				{
					// Check for at-rule
					invalid_at = true;
					for(map<string,parse_status>::iterator j = m_at_rules.begin(); j != m_at_rules.end(); ++j )
					{
						if(strtolower(css_input.substr(i+1,j->first.length())) == j->first)
						{
							(j->second == at) ? m_cur_at = "@" + j->first : m_cur_selector = "@" + j->first;
							status = j->second;
							i += (int) j->first.length();
							invalid_at = false;
						}
					}
					if(invalid_at)
					{
						m_cur_selector = "@";
						string invalid_at_name = "";
						for(int j = i+1; j < str_size; ++j)
						{
							if(!ctype_alpha(css_input[j]))
							{
								break;
							}
							invalid_at_name += css_input[j];
						}
						log("Invalid @-rule: " + invalid_at_name + " (removed)",Warning);
					}
				}
				else if(css_input[i] == '"' || css_input[i] == '\'')
				{
					m_cur_selector += css_input[i];
					status   = instr;
					str_char = css_input[i];
					from     = is;
				}
				else if(invalid_at && css_input[i] == ';')
				{
					invalid_at = false;
					status = is;
				}
				else if(css_input[i] == '{')
				{
					status = ip;
					add_token(SEL_START, m_cur_selector);
					++m_selectors;
				}
				else if(css_input[i] == '}')
				{
					add_token(AT_END, m_cur_at);
					m_cur_at = "";
					m_cur_selector = "";
					m_sel_separate = vector<int>();
				}
				else if(css_input[i] == ',') 
				{
					m_cur_selector = trim(m_cur_selector) + ",";
					m_sel_separate.push_back((int)m_cur_selector.length());
				}
				else if(css_input[i] == '\\') 
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
			  if(!((length && ctype_space(m_cur_selector[lastpos]) || (length && is_token(m_cur_selector,lastpos) && m_cur_selector[lastpos] == ',')) && (length && ctype_space(css_input[i]))))
			  {
				  m_cur_selector += css_input[i];
			  }
			}
			break;
			
			/* Case in-property */
			case ip:
			if(is_token(css_input,i))
			{
				if(css_input[i] == ':' || css_input[i] == '=' && m_cur_property != "") // IE really accepts =, so CssStyleSheet will fix those mistakes
				{
					status = iv;
					bool valid = (m_all_properties.count(m_cur_property) > 0 && m_all_properties[m_cur_property].find(m_css_level,0) != string::npos);
					if(valid || !m_settings["discard_invalid_properties"]) {
						add_token(PROPERTY, m_cur_property);
					}
				}
				else if(css_input[i] == '/' && s_at(css_input,i+1) == '*' && m_cur_property == "")
				{
					status = ic; ++i;
					from = ip;
				}
				else if(css_input[i] == '}')
				{
					explode_selectors();
					status = is;
					invalid_at = false;
					add_token(SEL_END, m_cur_selector);
					m_cur_selector = "";
					m_cur_property = "";
				}
				else if(css_input[i] == ';')
				{
					m_cur_property = "";
				}
				else if(css_input[i] == '\\') 
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
			pn = ((css_input[i] == '\n' || css_input[i] == '\r') && property_is_next(css_input,i+1) || i == str_size-1);
			if(pn)
			{
				log("Added semicolon to the end of declaration",Warning);
			}
			if(is_token(css_input,i) || pn)
			{
				if(css_input[i] == '/' && s_at(css_input,i+1) == '*')
				{
					status = ic; ++i;
					from = iv;
				}
				else if(css_input[i] == '"' || css_input[i] == '\'' || css_input[i] == '(')
				{
					m_cur_sub_value += css_input[i];
					str_char = (css_input[i] == '(') ? ')' : css_input[i];
					status = instr;
					from = iv;
				}
				else if(css_input[i] == '\\') 
				{
					m_cur_sub_value += unicode(css_input,i);
				}
				else if(css_input[i] == ';' || pn)
				{
					if(m_cur_selector.substr(0,1) == "@" && m_at_rules.count(m_cur_selector.substr(1)) > 0 && m_at_rules[m_cur_selector.substr(1)] == iv)
					{
						cur_sub_value_arr.push_back(trim(m_cur_sub_value));
						status = is;
						
						if(m_cur_selector == "@charset")   m_charset = cur_sub_value_arr[0];
						if(m_cur_selector == "@namespace") m_namesp = implode(" ",cur_sub_value_arr);
						if(m_cur_selector == "@import")    m_import.push_back(implode(" ",cur_sub_value_arr));
		
						cur_sub_value_arr.clear();
						m_cur_sub_value = "";
						m_cur_selector = "";
						m_sel_separate = vector<int>();
					}
					else
					{
						status = ip;
					}
				}
				else if(css_input[i] != '}')
				{
					m_cur_sub_value += css_input[i];
				}
				if( (css_input[i] == '}' || css_input[i] == ';' || pn) && !m_cur_selector.empty())
				{
					++m_properties;
					
					if(m_cur_at == "")
					{
						m_cur_at = "standard";
					}
	
					// Kill all whitespace
					m_cur_at = trim(m_cur_at); m_cur_selector = trim(m_cur_selector);
					m_cur_value = trim(m_cur_value); m_cur_property = trim(m_cur_property);
					m_cur_sub_value = trim(m_cur_sub_value);
					
					// case m_settings
					if(m_settings["lowercase_s"])
					{
						m_cur_selector = strtolower(m_cur_selector);
					}
					m_cur_property = strtolower(m_cur_property);
					
					
					if(m_cur_sub_value != "")
					{
						m_cur_sub_value = optimise_subvalue(m_cur_sub_value,m_cur_property);
						cur_sub_value_arr.push_back(m_cur_sub_value);
						m_cur_sub_value = "";
					}
	
					m_cur_value = implode(" ",cur_sub_value_arr);
					
					// Compress !important
					temp = c_important(m_cur_value);
					if(temp != m_cur_value)
					{
						log("Optimised !important",Information);
					}
					m_cur_value = temp;
					
					// Optimise shorthand properties
					if(m_shorthands.count(m_cur_property) > 0)
					{
						temp = shorthand(m_cur_value);
						if(temp != m_cur_value)
						{
							log("Optimised shorthand notation (" + m_cur_property + "): Changed \"" + m_cur_value + "\" to \"" + temp + "\"",Information);
						}
						m_cur_value = temp;
					}
					
					// Compress font-weight (tiny compression)
					if(m_cur_property == "font-weight" && m_settings["compress_font-weight"])
					{
						int c_fw = c_font_weight(m_cur_value);
						if(c_fw == 400)
						{
							log("Optimised font-weight: Changed \"bold\" to \"700\"",Information);
						}
						else if(c_fw == 700)
						{
							log("Optimised font-weight: Changed \"normal\" to \"400\"",Information);
						}
					}
					
					bool valid = (m_all_properties.count(m_cur_property) > 0 && m_all_properties[m_cur_property].find(m_css_level,0) != string::npos);
					if((!invalid_at || m_settings["preserve_css"]) && (!m_settings["discard_invalid_properties"] || valid))
					{
						put(m_cur_at,m_cur_selector,m_cur_property,m_cur_value);
						add_token(VALUE, m_cur_value);
							
						// Further Optimisation
						if(m_cur_property == "background" && m_settings["optimise_shorthands"] > 1)
						{
							map<string,string> temp2 = dissolve_short_bg(m_cur_value);
							m_css[m_cur_at][m_cur_selector].erase("background");
							for(map<string,string>::iterator it = temp2.begin(); it != temp2.end(); ++it )
							{
								put(m_cur_at,m_cur_selector,it->first,it->second);
							}
						}
						if(m_shorthands.count(m_cur_property) > 0 && m_settings["optimise_shorthands"] > 0)
						{
							map<string,string> temp3 = dissolve_4value_shorthands(m_cur_property,m_cur_value);
							for(map<string,string>::iterator it = temp3.begin(); it != temp3.end(); ++it )
							{
								put(m_cur_at,m_cur_selector,it->first,it->second);
							}
							if(m_shorthands[m_cur_property][0] != "0")
							{
								m_css[m_cur_at][m_cur_selector].erase(m_cur_property);
							}
						}
					}
					if(!valid)
					{
						if(m_settings["discard_invalid_properties"])
						{
							log("Removed invalid property: " + m_cur_property,Warning);
						}
						else
						{
							log("Invalid property in " + strtoupper(m_css_level) + ": " + m_cur_property,Warning);
						}
					}
					
					//Split multiple selectors here if necessary								
					m_cur_property = "";
					cur_sub_value_arr.clear();
					m_cur_value = "";
				}
				if(css_input[i] == '}')
				{
					explode_selectors();
					add_token(SEL_END, m_cur_selector);
					status = is;
					invalid_at = false;
					m_cur_selector = "";
				}
			}
			else if(!pn)
			{
				m_cur_sub_value += css_input[i];
			
				if(ctype_space(css_input[i]))
				{
					if(trim(m_cur_sub_value) != "")
					{
						m_cur_sub_value = optimise_subvalue(m_cur_sub_value,m_cur_property);
						cur_sub_value_arr.push_back(trim(m_cur_sub_value));
					}
					m_cur_sub_value = "";
				}
			}
			break;
			
			/* Case in-string */
			case instr:
			if(str_char == ')' && css_input[i] == '"' && str_in_str == false && !escaped(css_input,i))
			{
				str_in_str = true;
			}
			else if(str_char == ')' && css_input[i] == '"' && str_in_str == true && !escaped(css_input,i))
			{
				str_in_str = false;
			}
			if(css_input[i] == str_char && !escaped(css_input,i) && str_in_str == false)
			{
				status = from;
			}
			temp_add = ""; temp_add += css_input[i];
			if( (css_input[i] == '\n' || css_input[i] == '\r') && !(css_input[i-1] == '\\' && !escaped(css_input,i-1)) )
			{
				temp_add = "\\A ";
				log("Fixed incorrect newline in string",Warning);
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
			if(css_input[i] == '*' && s_at(css_input,i+1) == '/')
			{
				status = from;
				++i;
				add_token(COMMENT, cur_comment);
				cur_comment = "";
			}
			else
			{
				cur_comment += css_input[i];
			}
			break;
		}
	}

	if(m_settings["merge_selectors"] > 1)
	{
		for(css_struct::iterator i = m_css.begin(); i != m_css.end(); i++ )
		{
			merge_selectors(i->second);
		}
	}

	if(m_settings["optimise_shorthands"] > 0)
	{
		for(css_struct::iterator i = m_css.begin(); i != m_css.end(); ++i )
		{
			for(sstore::iterator j = i->second.begin(); j != i->second.end();)
			{
				merge_4value_shorthands(i->first,j->first);
				if(m_settings["optimise_shorthands"] > 1) 
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

