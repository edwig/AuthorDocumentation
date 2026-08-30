// PRINT CSS
//
// Prints out the internal structure of a CssStyleSheet object
// back to a persistent ASCII file.
//
#include "stdafx.h"
#include "css.h"
#include <cstdlib>
#include <sstream>
#include <iostream>
#include <fstream>
#include <time.h>

#pragma warning (disable: 4503)

using namespace std;

string CssStyleSheet::HtmlSpecials(const string istring, const bool plain)
{
  if (!plain) 
  {
    return htmlspecialchars(istring);
  }
  return istring;
}

void CssStyleSheet::convert_css_to_tokens()
{
	m_csstokens = vector<token>();
	
	m_css.sort();
        
  for (css_struct::iterator i = m_css.begin(); i != m_css.end(); ++i)
  {
    if (m_settings["sort_selectors"]) 
    {
      i->second.sort();
    }
    if (i->first != "standard") 
    {
      add_token(AT_START, i->first, true);
    }
    
    for(sstore::iterator j = i->second.begin(); j != i->second.end(); ++j)
    {
      if (m_settings["sort_properties"]) 
      {
        j->second.sort();
      }
      add_token(SEL_START, j->first, true);
      
      for(umap<string,string>::iterator k = j->second.begin(); k != j->second.end(); ++k)
      {
        add_token(PROPERTY, k->first, true);
        add_token(VALUE, k->second, true);
      }
      add_token(SEL_END, j->first, true);
    }
    if (i->first != "standard") 
    {
      add_token(AT_END, i->first, true);
    }
  }
}

int 
CssStyleSheet::SeekNoComment(const int key, int move)
{
  int go = (move > 0) ? 1 : -1;
  for (int i = key + 1; abs(key-i)-1 < abs(move); i += go) 
  {
    if (i < 0 || i >= (int)m_csstokens.size()) 
    {
      return -1;
    }
    if (m_csstokens[i].type == COMMENT) 
    {
      move += 1;
      continue;
    }
    return m_csstokens[i].type;
  }
  return 0;
}

// Print CSS back to it's file whence it came
bool 
CssStyleSheet::print_css()
{
  if(!Valid())
  {
    log("Invalid CSS!",Error);
		return false;
	}

	ofstream file_output;
	if(m_filename != "")
	{
		file_output.open(m_filename.c_str()); // ,ios::binary);
		if(file_output.bad())
		{
			if(!m_settings["silent"]) 
      {
        log("Error when trying to save the output file!",Error);
      }
			return false;
		}
	}
  //else
  //{
  //  log("No output filename givven",Error);
  //  return false;
  //}
	
	if(!m_settings["allow_html_in_templates"])
	{
		for(int i = 0; i < (int)m_csstemplate.size(); ++i)
		{
			m_csstemplate[i] = strip_tags(m_csstemplate[i]);
		}
	}
	
  // Complete stylesheet is in csstokens (if preserve_css is true)
  // or is in "css" and must be converted to tokens
  if(!m_settings["preserve_css"]) 
  {
    convert_css_to_tokens();
  }

  stringstream output, in_at_out;
	
	if (m_settings["timestamp"] && m_filename != "") 
  {
      struct tm now;
		  time_t rawtime;
		  time(&rawtime);
		  token temp;
      temp.data = " AuthorHTML CSS-Editor rewritten: ";
      localtime_s(&now,&rawtime);
      char buffer[100];
      asctime_s(buffer,100,&now);
		  temp.data += rtrim(string(buffer));
		  temp.type = COMMENT;
		  m_csstokens.insert(m_csstokens.begin(), temp);
	}
	
	if(m_charset != "")
	{
		output << m_csstemplate[0] << "@charset " << m_csstemplate[5] << m_charset << m_csstemplate[6];
	}
	
	if(m_import.size() > 0)
	{
		for(int i = 0; i < (int)m_import.size(); i ++)
		{
			output  << m_csstemplate[0] << "@import " << m_csstemplate[5] << m_import[i] << m_csstemplate[6];
		}
	}
	
	if(m_namesp != "")
	{
		output << m_csstemplate[0] << "@namespace " << m_csstemplate[5] << m_namesp << m_csstemplate[6];
	}
	
	output << m_csstemplate[13];
	stringstream* out =& output;
	   
  bool plain = !m_settings["allow_html_in_templates"];
  string amp;
  bool isamp;

  for (int i = 0; i < (int)m_csstokens.size(); ++i)
  {
    switch (m_csstokens[i].type)
    {
      case AT_START:
           *out << m_csstemplate[0] << HtmlSpecials(m_csstokens[i].data, plain) + m_csstemplate[1];
           out =& in_at_out;
           break;
            
      case SEL_START:
           if(m_settings["lowercase_s"]) 
           {
             m_csstokens[i].data = strtolower(m_csstokens[i].data);
           }
           amp   = m_csstokens[i].data;
           isamp = false;
           if(!amp.empty() && amp.compare("@") == 0)
           {
             isamp = true;
           }
//         *out << ((m_csstokens[i].data[0] != '@') ? m_csstemplate[2] + HtmlSpecials(m_csstokens[i].data, plain) 
//                                                  : m_csstemplate[0] + HtmlSpecials(m_csstokens[i].data, plain));
           *out << (isamp ? m_csstemplate[2] + HtmlSpecials(m_csstokens[i].data, plain) 
                          : m_csstemplate[0] + HtmlSpecials(m_csstokens[i].data, plain));

           *out << m_csstemplate[3];
           break;
                
      case PROPERTY:
           if(m_settings["case_properties"] == 2) m_csstokens[i].data = strtoupper(m_csstokens[i].data);
           if(m_settings["case_properties"] == 1) m_csstokens[i].data = strtolower(m_csstokens[i].data);
           *out << m_csstemplate[4] << HtmlSpecials(m_csstokens[i].data, plain) << ":" << m_csstemplate[5];
           break;
            
      case VALUE:
           *out << HtmlSpecials(m_csstokens[i].data, plain);
           if(SeekNoComment(i, 1) == SEL_END && m_settings["remove_last_;"]) 
           {
             *out << str_replace(";", "", m_csstemplate[6]);
           } 
           else 
           {
             *out << m_csstemplate[6];
           }
           break;

      case SEL_END:
           *out << m_csstemplate[7];
           if(SeekNoComment(i, 1) != AT_END) 
           {
             *out << m_csstemplate[8];
           }
           break;
           
      case AT_END:
				   out = &output;
           *out << m_csstemplate[10] << str_replace("\n", "\n" + m_csstemplate[10], in_at_out.str());
           in_at_out.str("");
           *out << m_csstemplate[9];
           break;

      case COMMENT:
           *out << m_csstemplate[11] <<  "/*" << HtmlSpecials(m_csstokens[i].data, plain) << "*/" << m_csstemplate[12];
           break;
    }
  }
	string output_string = trim(output.str());
  m_output_size = (int)output_string.length();
		
	if(!m_settings["silent"]) 
  {
    string info;
    char   number[20];

    float i_b   = round(((float) m_input_size )/1024,3);
    float o_b   = round(((float) m_output_size)/1024,3);
    float ratio = round(((m_input_size - (float) output_string.length())/m_input_size)*100,2);

    info = "Selectors  : "; _itoa_s(m_selectors, number,20,10); info += number; log(info,Information);
    info = "Properties : "; _itoa_s(m_properties,number,20,10); info += number; log(info,Information);
    info = "Input size : "; sprintf_s(number,20,"%f",i_b);      info += number; info += " kB"; log(info,Information);
    info = "Output size: "; sprintf_s(number,20,"%f",o_b);      info += number; info += " kB"; log(info,Information);
    info = "Compression: "; sprintf_s(number,20,"%f",ratio);    info += number; info += " %";  log(info,Information);
	}

  // NOW REALLY OUTPUT IT TO FILE
  m_theSheet = output_string;
  if(m_filename != "")
  {
    file_output << output_string;
    file_output.close();
  }
  return true;
}

string
CssStyleSheet::print_logs()
{
  string loglines;

  if(!Valid())
  {
    loglines = "Empty CSS stylesheet!";
    return loglines;
  }
  // Print the conversion logs
  if(m_logs.size() > 0 && !m_settings["silent"])
  {
    for(map<int, vector<message> >::iterator j = m_logs.begin(); j != m_logs.end(); j++ )
    {
      for(int i = 0; i < (int)j->second.size(); ++i)
      {
        char number[10];
        _itoa_s(j->first,number,10,10);
        loglines += number;
        loglines += ": ";
        loglines += j->second[i].m;
        loglines += "\n";
      }
    }
  }
  return loglines;
}
