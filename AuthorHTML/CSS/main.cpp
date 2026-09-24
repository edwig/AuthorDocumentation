// MAIN.CPP
//
#include "stdafx.h"
using namespace std;
#include <fstream>
#include "css.hpp"

string file_get_contents(const string filename)
{
  ifstream file_input(filename.c_str(),ios::binary);
  string line, file_contents = _T("");

  if(file_input.bad())
  {
    return _T("");
  }
  else
  {
    while(file_input.good())
    {
      getline(file_input,line);
      file_contents += (line + _T("\n"));
    }	    
  }
  file_input.close();

  return file_contents;
}

bool file_exists(const TCHAR *filename)
{
  bool does_exists = false;

  ifstream file_input(filename);
  if(file_input.is_open())
  {
    does_exists = true;
  }
  file_input.close();
  return does_exists;
}

const string trim(const string istring)
{
  std::string::size_type first = istring.find_first_not_of(_T(" \n\t\r\0xb"));
  if (first == std::string::npos) 
  {
    return std::string();
  }
  else 
  {
    std::string::size_type last = istring.find_last_not_of(_T(" \n\t\r\0xb"));
    return istring.substr( first, last - first + 1);
  }
}

string strtoupper(string istring)
{
  int str_size = (int) istring.length();
  for(int i = 0; i < str_size; i++)
  {
    istring[i] = _totupper(istring[i]);
  }
  return istring;
}


int _tmain(int argc, TCHAR *argv[])
{
	CssStyleSheet csst;

	if(argc > 1)
	{
		string filein = argv[1];
		if(filein != _T("-") && !file_exists(argv[1]))
		{
			cout << _T("The file \"") << filein << _T("\" does not exist.") << endl;
			return EXIT_FAILURE;
		}
  	string output_filename;
		
		for(int i = 2; i < argc; ++i)
		{
			bool output_file = true;
			for(map<string,int>::iterator j = csst.m_settings.begin(); j != csst.m_settings.end(); ++j )
			{
				if(trim(argv[i]) == _T("--") + j->first + _T("=false") || 
           trim(argv[i]) == _T("--") + j->first + _T("=0"))
				{
					csst.m_settings[j->first] = 0;
					output_file = false;
				}
				else if(trim(argv[i]) == _T("--") + j->first + _T("=true") || 
                trim(argv[i]) == _T("--") + j->first + _T("=1"))
				{
					csst.m_settings[j->first] = 1;
					output_file = false;
				}
				else if(trim(argv[i]) == _T("--") + j->first + _T("=2"))
				{
					csst.m_settings[j->first] = 2;
					output_file = false;
				}
			}
			if(trim(argv[i]).substr(0,12) == _T("--css_level="))
			{
				csst.m_css_level = strtoupper(trim(argv[i]).substr(12));
				output_file = false;
			}
			else if(trim(argv[i]).substr(0,11) == _T("--template="))
			{
				string template_value = trim(argv[i]).substr(11);
        csst.SetTemplate(template_value);
  			output_file = false;
			}
			if(output_file)
			{
				output_filename = trim(argv[i]);
			}
		}
		
		string css_file;

    if(filein == _T("-")) 
    {
			string temp;
			do 
      {
				getline(cin, temp, _T('\n'));
				css_file += (temp + _T("\n"));
			} 
      while(cin);
		} 
    else 
    {
      css_file = file_get_contents(argv[1]);
    }
		csst.parse_css(css_file);
		
		// Print CSS to screen if no output file is specified
  	csst.print_css(output_filename);

    // Errors and warnings
    string logs = csst.print_logs();
    if(logs.size() > 0)
    {
      cout << _T("-----------------------------------\n\n");
      cout << logs;
    }
		return EXIT_SUCCESS;
	}

  // PRINT USAGE OF THIS TOOL
	cout << endl << _T("Usage:") << endl << endl << _T("CSS input_filename [\n");
	for(map<string,int>::iterator j = csst.m_settings.begin(); j != csst.m_settings.end(); ++j )
	{
		if (j->first == _T("optimise_shorthands") || 
        j->first == _T("merge_selectors")) 
    {
			continue;
		}
		
		cout << _T(" --") << j->first;
		if(j->second == 1)  // 1 = true
		{
			cout << _T("=[true|false] |\n");
		}
		else
		{
			cout << _T("=[false|true] |\n");
		}
	}
	cout << _T(" --merge_selectors=[2|1|0] |\n");
	cout << _T(" --optimise_shorthands=[1|2|0] |\n");
	cout << _T(" --template=[default|filename|low|high|highest] |\n");
	cout << _T(" output_filename ]*") << endl;
	
	return EXIT_SUCCESS;
}
