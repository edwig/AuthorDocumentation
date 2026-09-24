// STYLE SHEET OPTIMIZER
//
// Optimizes the CssStyleSheet by it's internal structures
//
// The following optimizations are done
// 1) Shorthands of border/margin/padding if all sides are equal
// 2) Compression of numbers (leave out units if possible)
// 3) Compression of numbers (leave out unnessesary decimals)
// 4) Optimize colors (6->4) and 'shorter names than codes'/'Shorter codes than names'
// 5) Optimize font weight names/sizes
// 6) Merge selectors with same definition
// 7) Background optimization on duplace definitions
// 8) Background optimization on image overriden properties
// 9) Optimize subvalues
//
#include "stdafx.h"
#include "css.h"

using namespace std;

XString
CssStyleSheet::shorthand(XString value)
{
	XString important = _T("");

	if(is_important(value))
	{
		value = gvw_important(value);
		important = _T(" !important");
	}

	vector<XString> values = explode(_T(" "),value);
	switch(values.size())
	{
		case 4:
		if(values[0] == values[1] && values[0] == values[2] && values[0] == values[3])
		{
			return values[0] + important;
		}
		else if(values[1] == values[3] && values[0] == values[2])
		{
			return values[0] + _T(" ") + values[1] + important;
		}
		else if(values[1] == values[3])
		{
			return values[0] + _T(" ") + values[1] + _T(" ")	 + values[2] + important;
		}
		else return value + important;
		break;
		
		case 3:
		if(values[0] == values[1] && values[0] == values[2])
		{
			return values[0] + important;
		}
		else if(values[0] == values[2])
		{
			return values[0] + _T(" ") + values[1] + important;
		}
		else return value + important;
		break;
		
		case 2:
		if(values[0] == values[1])
		{
			return values[0] + important;
		}
		else return value + important;
		break;
		
		default:
		return value + important;
	}
}

XString
CssStyleSheet::compress_numbers(XString subvalue, XString property)
{
	XString units[] =
  {
     _T("in")     // Inches
    ,_T("cm")     // Centimeters
    ,_T("mm")     // Milimeters
    ,_T("pt")     // Points
    ,_T("pc")     // Picas
    ,_T("px")     // Pixels
    ,_T("rem")    // 
    ,_T("%")      // Percentage
    ,_T("ex")     // x-height of relevant font
    ,_T("gd")     //
    ,_T("em")     // The 'font-size' of relevant font (em-space)
    ,_T("vw")
    ,_T("vh")
    ,_T("vm")	
    ,_T("deg")    // Degrees ( 90 degree = straight angle)
    ,_T("grad")   // Grades  (100 grades = straight angle)
    ,_T("rad")    // Radians (2pi = half circle)
    ,_T("ms")     // Miliseconds
    ,_T("s")      // Seconds
    ,_T("khz")    // Kiloherz
    ,_T("hz")     // Herz
  }; 

	vector<XString> temp;
	if(property == _T("font"))
	{
		temp = explode(_T("/"),subvalue);
	}
	else
	{
		temp.push_back(subvalue);
	}
		
	for (int i = 0; i < (int) temp.size(); ++i)
	{
		if(!(temp[i].length() > 0 && (ctype_digit(temp[i][0]) || temp[i][0] == _T('+') || temp[i][0] == _T('-') ) ))
		{
			continue;
		}
		
		if(in_str_array(m_color_values,property))
		{
			temp[i] = _T("#") + temp[i];
		}
	
		if(str2f(temp[i]) == 0)
		{
			temp[i] = _T("0");
		}
		else
		{
			bool unit_found = false;
			temp[i] = strtolower(temp[i]);
			for(int j = 0; j < 21; ++j )
			{
				if(temp[i].find(units[j]) != XString::npos)
				{
					temp[i] = f2str(str2f(temp[i])) + units[j];
					unit_found = true;
					break;
				}
			}
			if(!unit_found && !in_str_array(m_number_values,property))
			{
				temp[i] = f2str(str2f(temp[i]));
				temp[i] += _T("px");
			}
			else if(!unit_found)
			{
				temp[i] = f2str(str2f(temp[i]));
			}
		}
	}
	return (temp.size() > 1) ? temp[0] + _T("/") + temp[1] : temp[0];
}

bool
CssStyleSheet::property_is_next(XString istring, size_t pos)
{
	istring = istring.substr(pos,istring.length()-pos);
	pos = istring.find_first_of(_T(':'),0);
	if(pos == XString::npos)
	{
		return false;
	}
	istring = strtolower(trim(istring.substr(0,pos)));
	return (m_all_properties.count(istring) > 0);
}

XString
CssStyleSheet::cut_color(XString color)
{
	if(strtolower(color.substr(0,4)) == _T("rgb("))
	{
		vector<XString> color_tmp = explode(_T(","),color.substr(4,color.length()-5));

		for (int i = 0; i < (int)color_tmp.size(); ++i)
		{
			color_tmp[i] = trim(color_tmp[i]);
			if(color_tmp[i].at(color_tmp[i].length()-1) == _T('%'))
			{
				color_tmp[i] = f2str((float)round((float)255 * _ttoi(color_tmp[i].c_str())/100,0));
			}
			if(_ttoi(color_tmp[i].c_str()) > 255) 
      {
        color_tmp[i] = _T("255")	;
      }
		}
		
		color = _T("#");
		for (int i = 0; i < (int)color_tmp.size(); ++i)
		{
			if(_ttoi(color_tmp[i].c_str()) < 16)
			{
				color += _T("0") + dechex(_ttoi(color_tmp[i].c_str()));
			}
			else
			{
				color += dechex(_ttoi(color_tmp[i].c_str()));
			}
		}
	}

	// Fix bad color names
	if(m_replace_colors.count(strtolower(color)) > 0)
	{
		color = m_replace_colors[strtolower(color)];
	}

	if(color.length() == 7)
	{
		XString color_temp = strtoupper(color);

		if(color_temp[0] == _T('#') && color_temp[1] == color_temp[2] && color_temp[3] == color_temp[4] && color_temp[5] == color_temp[6])
		{
			color = _T("#");
			color += color_temp[2];
			color += color_temp[3];
			color += color_temp[5];
		}
	}

	XString temp = strtolower(color);
	/* color name -> hex code */
	if(temp == _T("black"))		return _T("#000");
	if(temp == _T("fuchsia"))	return _T("#F0F");
	if(temp == _T("white"))		return _T("#FFF");
	if(temp == _T("yellow"))	return _T("#FF0");		
	/* hex code -> color name */
	if(temp == _T("#800000"))	return _T("maroon");
	if(temp == _T("#ffa500"))	return _T("orange");
	if(temp == _T("#808000"))	return _T("olive");
	if(temp == _T("#800080"))	return _T("purple");
	if(temp == _T("#008000"))	return _T("green");
	if(temp == _T("#000080"))	return _T("navy");
	if(temp == _T("#008080"))	return _T("teal");
	if(temp == _T("#c0c0c0"))	return _T("silver");
	if(temp == _T("#808080"))	return _T("gray");
	if(temp == _T("#f00"))		return _T("red");	

	return color;
}

int
CssStyleSheet::c_font_weight(XString& value)
{
	XString important = _T("");
	if(is_important(value))
	{
		important = _T(" !important");
		value = gvw_important(value);
	}
	if(value == _T("bold"))
	{
		value = _T("700")+important;
		return 700;
	}
	else if(value == _T("normal"))
	{
		value = _T("400")+important;
		return 400;
	}
	return 0;
}


void 
CssStyleSheet::merge_selectors(sstore& input)
{
  //sstore::iterator last;
	for(sstore::iterator i = input.begin(),last = i; i != input.end();)
	{
		XString newsel = _T("");

		// Check if properties also exist in another selector
		vector<XString> keys;
		for(sstore::iterator j = input.begin(); j != input.end(); j++ )
		{
			if(j->first == i->first)
			{
				continue;
			}
			
			if(input[j->first] == input[i->first])
			{
				keys.push_back(j->first);
			}
		}

		if(keys.size() > 0)
		{
			newsel = i->first;

			for(int k = 0; k < (int)keys.size(); ++k)
			{
				input.erase(keys[k]);
				newsel += _T(",") + keys[k];
			}

			input[newsel] = i->second;
			
      sstore::iterator e = i;
      i = last;
			input.erase(e);
      i++;
			//e = input.end();
		} 
    else 
    {
      last = i;
			i++;
		}
	}
}

XString CssStyleSheet::optimise_subvalue(XString subvalue, const XString property)
{
	subvalue = trim(subvalue);
	XString temp = compress_numbers(subvalue,property);
	if(temp != subvalue)
	{
		if(temp.length() > subvalue.length())
		{
			log(_T("Fixed invalid number: Changed \"") + subvalue + _T("\" to \"") + temp + _T("\""),Warning);
		}
		else
		{
			log(_T("Optimised number: Changed \"") + subvalue + _T("\" to \"") + temp + _T("\""),Information);
		}
		subvalue = temp;
	}
	if(m_settings[_T("compress_colors")])
	{
		temp = cut_color(subvalue);
		if(temp != subvalue)
		{
			if(m_replace_colors.count(subvalue) > 0)
			{
				log(_T("Fixed invalid color name: Changed \"") + subvalue + _T("\" to \"") + temp + _T("\""),Warning);
			}
			else
			{
				log(_T("Optimised color: Changed \"") + subvalue + _T("\" to \"") + temp + _T("\""),Information);
			}
			subvalue = temp;
		}
	}
	return subvalue;
}

// BACKGROUND OPTIMIZER

map<XString,XString>
CssStyleSheet::dissolve_short_bg(XString istring)
{
  vector<XString> repeat,attachment,clip,origin,pos,str_values;

  repeat.push_back(_T("repeat")); 
  repeat.push_back(_T("repeat-x")); 
  repeat.push_back(_T("repeat-y"));
  repeat.push_back(_T("no-repeat")); 
  repeat.push_back(_T("space"));
  attachment.push_back(_T("scroll")); 
  attachment.push_back(_T("fixed")); 
  attachment.push_back(_T("local"));
  clip.push_back(_T("border")); 
  clip.push_back(_T("padding"));
  origin.push_back(_T("border")); 
  origin.push_back(_T("padding")); 
  origin.push_back(_T("content"));
  pos.push_back(_T("top")); 
  pos.push_back(_T("center")); 
  pos.push_back(_T("bottom")); 
  pos.push_back(_T("left")); 
  pos.push_back(_T("right"));
  XString important = _T("");

  map<XString,XString> ret;
  map<XString,bool> have;
  ret[_T("background-image")] = _T("");
  ret[_T("background-size")] = _T("");
  ret[_T("background-repeat")] = _T("");
  ret[_T("background-position")] = _T("");
  ret[_T("background-attachment")] = _T("");
  ret[_T("background-clip")] = _T("");
  ret[_T("background-origin")] = _T("");
  ret[_T("background-color")] = _T("");

  if(is_important(istring))
  {
    important = _T(" !important");
    istring = gvw_important(istring);
  }

  str_values = explode_ws(_T(','),istring);
  for(int i = 0; i < (int) str_values.size(); i++)
  {
    have[_T("clip")] = false; have[_T("pos")] = false;
    have[_T("color")] = false; have[_T("bg")] = false;

    vector<XString> temp_values = explode_ws(_T(' '),trim(str_values[i]));

    for(int j = 0; j < (int) temp_values.size(); j++)
    {
      if(have[_T("bg")] == false && ((temp_values[j]).substr(0,4) == _T("url(") || temp_values[j] == _T("none")))
      {
        ret[_T("background-image")] += temp_values[j];
        ret[_T("background-image")] += _T(",");
        have[_T("bg")] = true;
      }
      else if(in_str_array(repeat,temp_values[j]))
      {
        ret[_T("background-repeat")] += temp_values[j];
        ret[_T("background-repeat")] += _T(",");
      }
      else if(in_str_array(attachment,temp_values[j]))
      {
        ret[_T("background-attachment")] += temp_values[j];
        ret[_T("background-attachment")] += _T(",");
      }
      else if(in_str_array(clip,temp_values[j]) && !have[_T("clip")])
      {
        ret[_T("background-clip")] += temp_values[j];
        ret[_T("background-clip")] += _T(",");
        have[_T("clip")] = true;
      }
      else if(in_str_array(origin,temp_values[j]))
      {
        ret[_T("background-origin")] += temp_values[j];
        ret[_T("background-origin")] += _T(",");
      }
      else if(temp_values[j][0] == _T('('))
      {
        ret[_T("background-size")] += (temp_values[j]).substr(1,temp_values[j].length()-2);
        ret[_T("background-size")] += _T(",");
      }
      else if(in_str_array(pos,temp_values[j]) || _istdigit(temp_values[j][0]) || temp_values[j][0] == 0)
      {
        ret[_T("background-position")] += temp_values[j];
        if(!have[_T("pos")]) ret[_T("background-position")] += _T(" "); else ret[_T("background-position")] += _T(",");
        have[_T("pos")] = true;
      }
      else if(!have[_T("color")])
      {
        ret[_T("background-color")] += temp_values[j];
        ret[_T("background-color")] += _T(",");
        have[_T("color")] = true;
      }
    }
  }

  for(map<XString,XString>::iterator it = m_background_prop_default.begin(); it != m_background_prop_default.end(); it++ )
  {
    if(ret[it->first] != _T(""))
    {
      ret[it->first] = (ret[it->first]).substr(0,ret[it->first].length()-1);
      ret[it->first] += important;
    }
    else
    {
      ret[it->first] = it->second;
      ret[it->first] += important;
    }
  }

  return ret;	
}

vector<XString>
CssStyleSheet::explode_ws(TCHAR sep,XString istring)
{
  // 1 = st // 2 = str
  int status = 1;
  TCHAR to = _T(' ');

  vector<XString> output;
  output.push_back(_T(""));
  int num = 0;
  int len = (int) istring.length();
  for(int i = 0;i < len; i++)
  {
    switch(status)
    {
    case 1:
      if(istring[i] == sep && !escaped(istring,i))
      {
        ++num;
        output.push_back(_T(""));
      }
      else if(istring[i] == _T('"') || istring[i] == _T('\'') || istring[i] == _T('(') && !escaped(istring,i))
      {
        status = 2;
        to = (istring[i] == _T('(')) ? _T(')') : istring[i];
        output[num] += istring[i];
      }
      else
      {
        output[num] += istring[i];
      }
      break;

    case 2:
      if(istring[i] == to && !escaped(istring,i))
      {
        status = 1;
      }
      output[num] += istring[i];
      break;
    }
  }

  return output;
}

void 
CssStyleSheet::merge_bg(umap<XString,XString>& css_input)
{
  // Max number of background images. CSS3 not yet fully implemented
  int number_of_values = cssmax(((int)explode_ws(_T(','),css_input[_T("background-image")]).size()),(int)(explode_ws(_T(','),css_input[_T("background-color")])).size());
  // Array with background images to check if BG image exists
  vector<XString> bg_img_array = explode_ws(_T(','),gvw_important(css_input[_T("background-image")]));
  XString new_bg_value,important = _T("");

  for(int i = 0; i < number_of_values; i++)
  {
    for(map<XString,XString>::iterator it = m_background_prop_default.begin(); it != m_background_prop_default.end(); it++ )
    {
      // Skip if property does not exist
      if(!css_input.has(it->first))
      {
        continue;
      }

      XString cur_value = css_input[it->first];

      // Skip some properties if there is no background image
      if(((int)bg_img_array.size() <= i || bg_img_array[i] == _T("none"))
        && (it->first == _T("background-size")       || it->first == _T("background-position")
         || it->first == _T("background-attachment") || it->first == _T("background-repeat")))
      {
        continue;
      }

      // Remove !important
      if(is_important(cur_value))
      {
        important = _T(" !important");
        cur_value = gvw_important(cur_value);
      }

      // Do not add default values
      if(cur_value == it->second)
      {
        continue;
      }

      vector<XString> temp = explode_ws(_T(','),cur_value);

      if((int)temp.size() > i)
      {					
        if(it->first == _T("background-size"))
        {
          new_bg_value += _T("(");
          new_bg_value += temp[i];
          new_bg_value += _T(") ");
        }
        else
        {
          new_bg_value += temp[i];
          new_bg_value += _T(" ");
        }
      }			
    }

    new_bg_value = trim(new_bg_value);
    if(i != number_of_values-1) new_bg_value += _T(",");
  }

  // Delete all background-properties
  for(map<XString,XString>::iterator it = m_background_prop_default.begin(); it != m_background_prop_default.end(); it++ )
  {
    css_input.erase(it->first);
  }

  // Add new background property
  if(new_bg_value != _T(""))
  {
    css_input[_T("background")] = new_bg_value + important;
  }
}
