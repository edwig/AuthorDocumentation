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

string 
CssStyleSheet::shorthand(string value)
{
	string important = "";
	
	if(is_important(value))
	{
		value = gvw_important(value);
		important = " !important";
	}
	
	vector<string> values = explode(" ",value);
	switch(values.size())
	{
		case 4:
		if(values[0] == values[1] && values[0] == values[2] && values[0] == values[3])
		{
			return values[0] + important;
		}
		else if(values[1] == values[3] && values[0] == values[2])
		{
			return values[0] + " " + values[1] + important;
		}
		else if(values[1] == values[3])
		{
			return values[0] + " " + values[1] + " " + values[2] + important;
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
			return values[0] + " " + values[1] + important;
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

string 
CssStyleSheet::compress_numbers(string subvalue, string property)
{
	string units[] = 
  {
     "in"     // Inches
    ,"cm"     // Centimeters
    ,"mm"     // Milimeters
    ,"pt"     // Points
    ,"pc"     // Picas
    ,"px"     // Pixels
    ,"rem"    // 
    ,"%"      // Percentage
    ,"ex"     // x-height of relevant font
    ,"gd"     //
    ,"em"     // The 'font-size' of relevant font (em-space)
    ,"vw"
    ,"vh"
    ,"vm"
    ,"deg"    // Degrees ( 90 degree = straight angle)
    ,"grad"   // Grades  (100 grades = straight angle)
    ,"rad"    // Radians (2pi = half circle)
    ,"ms"     // Miliseconds
    ,"s"      // Seconds
    ,"khz"    // Kiloherz
    ,"hz"     // Herz
  }; 
	           
	vector<string> temp;
	if(property == "font")
	{
		temp = explode("/",subvalue);
	}
	else
	{
		temp.push_back(subvalue);
	}
		
	for (int i = 0; i < (int) temp.size(); ++i)
	{
		if(!(temp[i].length() > 0 && (ctype_digit(temp[i][0]) || temp[i][0] == '+' || temp[i][0] == '-' ) ))
		{
			continue;
		}
		
		if(in_str_array(m_color_values,property))
		{
			temp[i] = "#" + temp[i];
		}
	
		if(str2f(temp[i]) == 0)
		{
			temp[i] = "0";
		}
		else
		{
			bool unit_found = false;
			temp[i] = strtolower(temp[i]);
			for(int j = 0; j < 21; ++j )
			{
				if(temp[i].find(units[j]) != string::npos)
				{
					temp[i] = f2str(str2f(temp[i])) + units[j];
					unit_found = true;
					break;
				}
			}
			if(!unit_found && !in_str_array(m_number_values,property))
			{
				temp[i] = f2str(str2f(temp[i]));
				temp[i] += "px";
			}
			else if(!unit_found)
			{
				temp[i] = f2str(str2f(temp[i]));
			}
		}
	}
	return (temp.size() > 1) ? temp[0] + "/" + temp[1] : temp[0];
}

bool 
CssStyleSheet::property_is_next(string istring, size_t pos)
{
	istring = istring.substr(pos,istring.length()-pos);
	pos = istring.find_first_of(':',0);
	if(pos == string::npos)
	{
		return false;
	}
	istring = strtolower(trim(istring.substr(0,pos)));
	return (m_all_properties.count(istring) > 0);
}

string 
CssStyleSheet::cut_color(string color)
{
	if(strtolower(color.substr(0,4)) == "rgb(")
	{
		vector<string> color_tmp = explode(",",color.substr(4,color.length()-5));

		for (int i = 0; i < (int)color_tmp.size(); ++i)
		{
			color_tmp[i] = trim(color_tmp[i]);
			if(color_tmp[i].at(color_tmp[i].length()-1) == '%')
			{
				color_tmp[i] = f2str((float)round((float)255 * atoi(color_tmp[i].c_str())/100,0));
			}
			if(atoi(color_tmp[i].c_str()) > 255) 
      {
        color_tmp[i] = "255";
      }
		}
		
		color = "#";
		for (int i = 0; i < (int)color_tmp.size(); ++i)
		{
			if(atoi(color_tmp[i].c_str()) < 16)
			{
				color += "0" + dechex(atoi(color_tmp[i].c_str()));
			}
			else
			{
				color += dechex(atoi(color_tmp[i].c_str()));
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
		string color_temp = strtoupper(color);

		if(color_temp[0] == '#' && color_temp[1] == color_temp[2] && color_temp[3] == color_temp[4] && color_temp[5] == color_temp[6])
		{
			color = "#";
			color += color_temp[2];
			color += color_temp[3];
			color += color_temp[5];
		}
	}

	string temp = strtolower(color);
	/* color name -> hex code */
	if(temp == "black")		return "#000";
	if(temp == "fuchsia")	return "#F0F";
	if(temp == "white")		return "#FFF";
	if(temp == "yellow")	return "#FF0";		
	/* hex code -> color name */
	if(temp == "#800000")	return "maroon";
	if(temp == "#ffa500")	return "orange";
	if(temp == "#808000")	return "olive";
	if(temp == "#800080")	return "purple";
	if(temp == "#008000")	return "green";
	if(temp == "#000080")	return "navy";
	if(temp == "#008080")	return "teal";
	if(temp == "#c0c0c0")	return "silver";
	if(temp == "#808080")	return "gray";
	if(temp == "#f00")		return "red";	

	return color;
}

int 
CssStyleSheet::c_font_weight(string& value)
{
	string important = "";
	if(is_important(value))
	{
		important = " !important";
		value = gvw_important(value);
	}
	if(value == "bold")
	{
		value = "700"+important;
		return 700;
	}
	else if(value == "normal")
	{
		value = "400"+important;
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
		string newsel = "";
	
		// Check if properties also exist in another selector
		vector<string> keys;
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
				newsel += "," + keys[k];
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

string CssStyleSheet::optimise_subvalue(string subvalue, const string property)
{
	subvalue = trim(subvalue);
	string temp = compress_numbers(subvalue,property);
	if(temp != subvalue)
	{
		if(temp.length() > subvalue.length())
		{
			log("Fixed invalid number: Changed \"" + subvalue + "\" to \"" + temp + "\"",Warning);
		}
		else
		{
			log("Optimised number: Changed \"" + subvalue + "\" to \"" + temp + "\"",Information);
		}
		subvalue = temp;
	}
	if(m_settings["compress_colors"])
	{
		temp = cut_color(subvalue);
		if(temp != subvalue)
		{
			if(m_replace_colors.count(subvalue) > 0)
			{
				log("Fixed invalid color name: Changed \"" + subvalue + "\" to \"" + temp + "\"",Warning);
			}
			else
			{
				log("Optimised color: Changed \"" + subvalue + "\" to \"" + temp + "\"",Information);
			}
			subvalue = temp;
		}
	}
	return subvalue;
}

// BACKGROUND OPTIMIZER

map<string,string> 
CssStyleSheet::dissolve_short_bg(string istring)
{
  vector<string> repeat,attachment,clip,origin,pos,str_values;

  repeat.push_back("repeat"); 
  repeat.push_back("repeat-x"); 
  repeat.push_back("repeat-y");
  repeat.push_back("no-repeat"); 
  repeat.push_back("space");
  attachment.push_back("scroll"); 
  attachment.push_back("fixed"); 
  attachment.push_back("local");
  clip.push_back("border"); 
  clip.push_back("padding");
  origin.push_back("border"); 
  origin.push_back("padding"); 
  origin.push_back("content");
  pos.push_back("top"); 
  pos.push_back("center"); 
  pos.push_back("bottom"); 
  pos.push_back("left"); 
  pos.push_back("right");
  string important = "";

  map<string,string> ret;
  map<string,bool> have;
  ret["background-image"] = "";
  ret["background-size"] = "";
  ret["background-repeat"] = "";
  ret["background-position"] = "";
  ret["background-attachment"] = "";
  ret["background-clip"] = "";
  ret["background-origin"] = "";
  ret["background-color"] = "";

  if(is_important(istring))
  {
    important = " !important";
    istring = gvw_important(istring);
  }

  str_values = explode_ws(',',istring);
  for(int i = 0; i < (int) str_values.size(); i++)
  {
    have["clip"] = false; have["pos"] = false;
    have["color"] = false; have["bg"] = false;

    vector<string> temp_values = explode_ws(' ',trim(str_values[i]));

    for(int j = 0; j < (int) temp_values.size(); j++)
    {
      if(have["bg"] == false && ((temp_values[j]).substr(0,4) == "url(" || temp_values[j] == "none"))
      {
        ret["background-image"] += temp_values[j];
        ret["background-image"] += ",";
        have["bg"] = true;
      }
      else if(in_str_array(repeat,temp_values[j]))
      {
        ret["background-repeat"] += temp_values[j];
        ret["background-repeat"] += ",";
      }
      else if(in_str_array(attachment,temp_values[j]))
      {
        ret["background-attachment"] += temp_values[j];
        ret["background-attachment"] += ",";
      }
      else if(in_str_array(clip,temp_values[j]) && !have["clip"])
      {
        ret["background-clip"] += temp_values[j];
        ret["background-clip"] += ",";
        have["clip"] = true;
      }
      else if(in_str_array(origin,temp_values[j]))
      {
        ret["background-origin"] += temp_values[j];
        ret["background-origin"] += ",";
      }
      else if(temp_values[j][0] == '(')
      {
        ret["background-size"] += (temp_values[j]).substr(1,temp_values[j].length()-2);
        ret["background-size"] += ",";
      }
      else if(in_str_array(pos,temp_values[j]) || isdigit(temp_values[j][0]) || temp_values[j][0] == 0)
      {
        ret["background-position"] += temp_values[j];
        if(!have["pos"]) ret["background-position"] += " "; else ret["background-position"] += ",";
        have["pos"] = true;
      }
      else if(!have["color"])
      {
        ret["background-color"] += temp_values[j];
        ret["background-color"] += ",";
        have["color"] = true;
      }
    }
  }

  for(map<string,string>::iterator it = m_background_prop_default.begin(); it != m_background_prop_default.end(); it++ )
  {
    if(ret[it->first] != "")
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

vector<string> 
CssStyleSheet::explode_ws(char sep,string istring)
{
  // 1 = st // 2 = str
  int status = 1;
  char to = ' ';

  vector<string> output;
  output.push_back("");
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
        output.push_back("");
      }
      else if(istring[i] == '"' || istring[i] == '\'' || istring[i] == '(' && !escaped(istring,i))
      {
        status = 2;
        to = (istring[i] == '(') ? ')' : istring[i];
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
CssStyleSheet::merge_bg(umap<string,string>& css_input)
{
  // Max number of background images. CSS3 not yet fully implemented
  int number_of_values = cssmax(((int)explode_ws(',',css_input["background-image"]).size()),(int)(explode_ws(',',css_input["background-color"])).size());
  // Array with background images to check if BG image exists
  vector<string> bg_img_array = explode_ws(',',gvw_important(css_input["background-image"]));
  string new_bg_value,important = "";

  for(int i = 0; i < number_of_values; i++)
  {
    for(map<string,string>::iterator it = m_background_prop_default.begin(); it != m_background_prop_default.end(); it++ )
    {			
      // Skip if property does not exist
      if(!css_input.has(it->first))
      {
        continue;
      }

      string cur_value = css_input[it->first];

      // Skip some properties if there is no background image
      if(((int)bg_img_array.size() <= i || bg_img_array[i] == "none")
        && (it->first == "background-size"       || it->first == "background-position"
         || it->first == "background-attachment" || it->first == "background-repeat"))
      {
        continue;
      }

      // Remove !important
      if(is_important(cur_value))
      {
        important = " !important";
        cur_value = gvw_important(cur_value);
      }

      // Do not add default values
      if(cur_value == it->second)
      {
        continue;
      }

      vector<string> temp = explode_ws(',',cur_value);

      if((int)temp.size() > i)
      {					
        if(it->first == "background-size")
        {
          new_bg_value += "(";
          new_bg_value += temp[i];
          new_bg_value += ") ";
        }
        else
        {
          new_bg_value += temp[i];
          new_bg_value += " ";
        }
      }			
    }

    new_bg_value = trim(new_bg_value);
    if(i != number_of_values-1) new_bg_value += ",";
  }

  // Delete all background-properties
  for(map<string,string>::iterator it = m_background_prop_default.begin(); it != m_background_prop_default.end(); it++ )
  {
    css_input.erase(it->first);
  }

  // Add new background property
  if(new_bg_value != "")
  {
    css_input["background"] = new_bg_value + important;
  }
}
