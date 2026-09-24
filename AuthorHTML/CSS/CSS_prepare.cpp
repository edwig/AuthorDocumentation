// PREPARE.CPP
//
// Prepares all global variables in the CssStyleSheet object
// to start off with the beasty for a CSS scan.
//
#include "stdafx.h"
#include "css.h"

void CssStyleSheet::prepare()
{
  m_properties    = 0;
  m_selectors     = 0;
  m_input_size    = 0;
  m_output_size   = 0;
  m_charset       = _T("");
  m_namesp        = _T("");
  m_css_level     = _T("CSS2.1");
  m_tokens        = _T("{};:()@='\"/,\\!$%&*+.<>?[]^`|~");
  m_cur_selector  = _T("");
  m_cur_at        = _T("");
  m_cur_property  = _T("");
  m_cur_sub_value = _T("");
  m_cur_value     = _T("");
  m_line          = 1;

  m_css.clear();
  m_logs.clear();
  m_import.clear();
  m_settings.clear();
  m_at_rules.clear();
  m_csstokens.clear();
  m_shorthands.clear();
  m_csstemplate.clear();
  m_sel_separate.clear();
  m_color_values.clear();
  m_number_values.clear();
  m_replace_colors.clear();
  m_all_properties.clear();
  m_predefined_templates.clear();
  m_background_prop_default.clear();

  //m_settings["optimizer"]            = 0;
  m_settings[_T("remove_bslash")]        = 1;
  m_settings[_T("compress_colors")]      = 1;
  m_settings[_T("compress_font-weight")] = 0;
  m_settings[_T("lowercase_s")]          = 0;
  m_settings[_T("optimise_shorthands")]  = 1;
  m_settings[_T("remove_last_;")]        = 0;
  m_settings[_T("case_properties")]      = 0;
  m_settings[_T("sort_properties")]      = 0;
  m_settings[_T("sort_selectors")]       = 0;
  m_settings[_T("merge_selectors")]      = 2;
  m_settings[_T("discard_invalid_properties")]  = 0;
  m_settings[_T("allow_html_in_templates")]     = 0;
  m_settings[_T("silent")]               = 0;
  m_settings[_T("preserve_css")]         = 0;
  m_settings[_T("timestamp")]            = 1;

  m_csstemplate.push_back(_T("<span class=\"at\">")); //string before @rule
  m_csstemplate.push_back(_T("</span> <span class=\"format\">{</span>\n")); //bracket after @-rule
  m_csstemplate.push_back(_T("<span class=\"selector\">")); //string before selector
  m_csstemplate.push_back(_T("</span> <span class=\"format\">{</span>\n")); //bracket after selector
  m_csstemplate.push_back(_T("<span class=\"property\">")); //string before property
  m_csstemplate.push_back(_T("</span><span class=\"value\">")); //string after property+before value
  m_csstemplate.push_back(_T("</span><span class=\"format\">;</span>\n")); //string after value
  m_csstemplate.push_back(_T("<span class=\"format\">}</span>")); //closing bracket - selector
  m_csstemplate.push_back(_T("\n\n")); //space between blocks {...}
  m_csstemplate.push_back(_T("\n<span class=\"format\">}</span>\n\n")); //closing bracket @-rule
  m_csstemplate.push_back(_T("")); //indent in @-rule
  m_csstemplate.push_back(_T("<span class=\"comment\">")); // before comment
  m_csstemplate.push_back(_T("</span>\n")); //after comment
  m_csstemplate.push_back(_T("\n")); // after last line @-rule

	m_number_values.push_back(_T("line-height"));
	m_number_values.push_back(_T("pitch-range"));
	m_number_values.push_back(_T("richness"));
	m_number_values.push_back(_T("speech-rate"));
	m_number_values.push_back(_T("stress"));
	m_number_values.push_back(_T("volume"));
	m_number_values.push_back(_T("font"));
	m_number_values.push_back(_T("font-weight"));
	m_number_values.push_back(_T("z-index"));
	m_number_values.push_back(_T("counter-increment"));
	m_number_values.push_back(_T("counter-reset"));
	m_number_values.push_back(_T("orphans"));
	m_number_values.push_back(_T("widows"));
	
	m_color_values.push_back(_T("color"));
	m_color_values.push_back(_T("background-color"));
	m_color_values.push_back(_T("border-color"));
	m_color_values.push_back(_T("border-top-color"));
	m_color_values.push_back(_T("border-right-color"));
	m_color_values.push_back(_T("border-bottom-color"));
	m_color_values.push_back(_T("border-left-color"));
	m_color_values.push_back(_T("border-color"));
	m_color_values.push_back(_T("color"));
	m_color_values.push_back(_T("outline-color"));

	m_replace_colors[_T("aliceblue")]         = _T("#F0F8FF");
	m_replace_colors[_T("antiquewhite")]      = _T("#FAEBD7");
	m_replace_colors[_T("aquamarine")]        = _T("#7FFFD4");
	m_replace_colors[_T("azure")]             = _T("#F0FFFF");
	m_replace_colors[_T("beige")]             = _T("#F5F5DC");
	m_replace_colors[_T("bisque")]            = _T("#FFE4C4");
	m_replace_colors[_T("blanchedalmond")]    = _T("#FFEBCD");
	m_replace_colors[_T("blueviolet")]        = _T("#8A2BE2");
	m_replace_colors[_T("brown")]             = _T("#A52A2A");
	m_replace_colors[_T("burlywood")]         = _T("#DEB887");
	m_replace_colors[_T("cadetblue")]         = _T("#5F9EA0");
	m_replace_colors[_T("chartreuse")]        = _T("#7FFF00");
	m_replace_colors[_T("chocolate")]         = _T("#D2691E");
	m_replace_colors[_T("coral")]             = _T("#FF7F50");
	m_replace_colors[_T("cornflowerblue")]    = _T("#6495ED");
	m_replace_colors[_T("cornsilk")]          = _T("#FFF8DC");
	m_replace_colors[_T("crimson")]           = _T("#DC143C");
	m_replace_colors[_T("cyan")]              = _T("#00FFFF");
	m_replace_colors[_T("darkblue")]          = _T("#00008B");
	m_replace_colors[_T("darkcyan")]          = _T("#008B8B");
	m_replace_colors[_T("darkgoldenrod")]     = _T("#B8860B");
	m_replace_colors[_T("darkgray")]          = _T("#A9A9A9");
	m_replace_colors[_T("darkgreen")]         = _T("#006400");
	m_replace_colors[_T("darkkhaki")]         = _T("#BDB76B");
	m_replace_colors[_T("darkmagenta")]       = _T("#8B008B");
	m_replace_colors[_T("darkolivegreen")]    = _T("#556B2F");
	m_replace_colors[_T("darkorange")]        = _T("#FF8C00");
	m_replace_colors[_T("darkorchid")]        = _T("#9932CC");
	m_replace_colors[_T("darkred")]           = _T("#8B0000");
	m_replace_colors[_T("darksalmon")]        = _T("#E9967A");
	m_replace_colors[_T("darkseagreen")]      = _T("#8FBC8F");
	m_replace_colors[_T("darkslateblue")]     = _T("#483D8B");
	m_replace_colors[_T("darkslategray")]     = _T("#2F4F4F");
	m_replace_colors[_T("darkturquoise")]     = _T("#00CED1");
	m_replace_colors[_T("darkviolet")]        = _T("#9400D3");
	m_replace_colors[_T("deeppink")]          = _T("#FF1493");
	m_replace_colors[_T("deepskyblue")]       = _T("#00BFFF");
	m_replace_colors[_T("dimgray")]           = _T("#696969");
	m_replace_colors[_T("dodgerblue")]        = _T("#1E90FF");
	m_replace_colors[_T("feldspar")]          = _T("#D19275");
	m_replace_colors[_T("firebrick")]         = _T("#B22222");
	m_replace_colors[_T("floralwhite")]       = _T("#FFFAF0");
	m_replace_colors[_T("forestgreen")]       = _T("#228B22");
	m_replace_colors[_T("gainsboro")]         = _T("#DCDCDC");
	m_replace_colors[_T("ghostwhite")]        = _T("#F8F8FF");
	m_replace_colors[_T("gold")]              = _T("#FFD700");
	m_replace_colors[_T("goldenrod")]         = _T("#DAA520");
	m_replace_colors[_T("greenyellow")]       = _T("#ADFF2F");
	m_replace_colors[_T("honeydew")]          = _T("#F0FFF0");
	m_replace_colors[_T("hotpink")]           = _T("#FF69B4");
	m_replace_colors[_T("indianred")]         = _T("#CD5C5C");
	m_replace_colors[_T("indigo")]            = _T("#4B0082");
	m_replace_colors[_T("ivory")]             = _T("#FFFFF0");
	m_replace_colors[_T("khaki")]             = _T("#F0E68C");
	m_replace_colors[_T("lavender")]          = _T("#E6E6FA");
	m_replace_colors[_T("lavenderblush")]     = _T("#FFF0F5");
	m_replace_colors[_T("lawngreen")]         = _T("#7CFC00");
	m_replace_colors[_T("lemonchiffon")]      = _T("#FFFACD");
	m_replace_colors[_T("lightblue")]         = _T("#ADD8E6");
	m_replace_colors[_T("lightcoral")]        = _T("#F08080");
	m_replace_colors[_T("lightcyan")]         = _T("#E0FFFF");
	m_replace_colors[_T("lightgoldenrodyellow")] = _T("#FAFAD2");
	m_replace_colors[_T("lightgrey")]         = _T("#D3D3D3");
	m_replace_colors[_T("lightgreen")]        = _T("#90EE90");
	m_replace_colors[_T("lightpink")]         = _T("#FFB6C1");
	m_replace_colors[_T("lightsalmon")]       = _T("#FFA07A");
	m_replace_colors[_T("lightseagreen")]     = _T("#20B2AA");
	m_replace_colors[_T("lightskyblue")]      = _T("#87CEFA");
	m_replace_colors[_T("lightslateblue")]    = _T("#8470FF");
	m_replace_colors[_T("lightslategray")]    = _T("#778899");
	m_replace_colors[_T("lightsteelblue")]    = _T("#B0C4DE");
	m_replace_colors[_T("lightyellow")]       = _T("#FFFFE0");
	m_replace_colors[_T("limegreen")]         = _T("#32CD32");
	m_replace_colors[_T("linen")]             = _T("#FAF0E6");
	m_replace_colors[_T("magenta")]           = _T("#FF00FF");
	m_replace_colors[_T("mediumaquamarine")]  = _T("#66CDAA");
	m_replace_colors[_T("mediumblue")]        = _T("#0000CD");
	m_replace_colors[_T("mediumorchid")]      = _T("#BA55D3");
	m_replace_colors[_T("mediumpurple")]      = _T("#9370D8");
	m_replace_colors[_T("mediumseagreen")]    = _T("#3CB371");
	m_replace_colors[_T("mediumslateblue")]   = _T("#7B68EE");
	m_replace_colors[_T("mediumspringgreen")] = _T("#00FA9A");
	m_replace_colors[_T("mediumturquoise")]   = _T("#48D1CC");
	m_replace_colors[_T("mediumvioletred")]   = _T("#C71585");
	m_replace_colors[_T("midnightblue")]      = _T("#191970");
	m_replace_colors[_T("mintcream")]         = _T("#F5FFFA");
	m_replace_colors[_T("mistyrose")]         = _T("#FFE4E1");
	m_replace_colors[_T("moccasin")]          = _T("#FFE4B5");
	m_replace_colors[_T("navajowhite")]       = _T("#FFDEAD");
	m_replace_colors[_T("oldlace")]           = _T("#FDF5E6");
	m_replace_colors[_T("olivedrab")]         = _T("#6B8E23");
	m_replace_colors[_T("orangered")]         = _T("#FF4500");
	m_replace_colors[_T("orchid")]            = _T("#DA70D6");
	m_replace_colors[_T("palegoldenrod")]     = _T("#EEE8AA");
	m_replace_colors[_T("palegreen")]         = _T("#98FB98");
	m_replace_colors[_T("paleturquoise")]     = _T("#AFEEEE");
	m_replace_colors[_T("palevioletred")]     = _T("#D87093");
	m_replace_colors[_T("papayawhip")]        = _T("#FFEFD5");
	m_replace_colors[_T("peachpuff")]         = _T("#FFDAB9");
	m_replace_colors[_T("peru")]              = _T("#CD853F");
	m_replace_colors[_T("pink")]              = _T("#FFC0CB");
	m_replace_colors[_T("plum")]              = _T("#DDA0DD");
	m_replace_colors[_T("powderblue")]        = _T("#B0E0E6");
	m_replace_colors[_T("rosybrown")]         = _T("#BC8F8F");
	m_replace_colors[_T("royalblue")]         = _T("#4169E1");
	m_replace_colors[_T("saddlebrown")]       = _T("#8B4513");
	m_replace_colors[_T("salmon")]            = _T("#FA8072");
	m_replace_colors[_T("sandybrown")]        = _T("#F4A460");
	m_replace_colors[_T("seagreen")]          = _T("#2E8B57");
	m_replace_colors[_T("seashell")]          = _T("#FFF5EE");
	m_replace_colors[_T("sienna")]            = _T("#A0522D");
	m_replace_colors[_T("skyblue")]           = _T("#87CEEB");
	m_replace_colors[_T("slateblue")]         = _T("#6A5ACD");
	m_replace_colors[_T("slategray")]         = _T("#708090");
	m_replace_colors[_T("snow")]              = _T("#FFFAFA");
	m_replace_colors[_T("springgreen")]       = _T("#00FF7F");
	m_replace_colors[_T("steelblue")]         = _T("#4682B4");
	m_replace_colors[_T("tan")]               = _T("#D2B48C");
	m_replace_colors[_T("thistle")]           = _T("#D8BFD8");
	m_replace_colors[_T("tomato")]            = _T("#FF6347");
	m_replace_colors[_T("turquoise")]         = _T("#40E0D0");
	m_replace_colors[_T("violet")]            = _T("#EE82EE");
	m_replace_colors[_T("violetred")]         = _T("#D02090");
	m_replace_colors[_T("wheat")]             = _T("#F5DEB3");
	m_replace_colors[_T("whitesmoke")]        = _T("#F5F5F5");
	m_replace_colors[_T("yellowgreen")]       = _T("#9ACD32");
		
	m_shorthands[_T("border-color")].push_back(_T("border-top-color"));
	m_shorthands[_T("border-color")].push_back(_T("border-right-color"));
	m_shorthands[_T("border-color")].push_back(_T("border-bottom-color"));
	m_shorthands[_T("border-color")].push_back(_T("border-left-color"));
	m_shorthands[_T("border-style")].push_back(_T("border-top-style"));
	m_shorthands[_T("border-style")].push_back(_T("border-right-style"));
	m_shorthands[_T("border-style")].push_back(_T("border-bottom-style"));
	m_shorthands[_T("border-style")].push_back(_T("border-left-style"));
	m_shorthands[_T("border-width")].push_back(_T("border-top-width"));
	m_shorthands[_T("border-width")].push_back(_T("border-right-width"));
	m_shorthands[_T("border-width")].push_back(_T("border-bottom-width"));
	m_shorthands[_T("border-width")].push_back(_T("border-left-width"));
	m_shorthands[_T("margin")].push_back(_T("margin-top"));
	m_shorthands[_T("margin")].push_back(_T("margin-right"));
	m_shorthands[_T("margin")].push_back(_T("margin-bottom"));
	m_shorthands[_T("margin")].push_back(_T("margin-left"));
	m_shorthands[_T("padding")].push_back(_T("padding-top"));
	m_shorthands[_T("padding")].push_back(_T("padding-right"));
	m_shorthands[_T("padding")].push_back(_T("padding-bottom"));
	m_shorthands[_T("padding")].push_back(_T("padding-left"));
	m_shorthands[_T("-moz-border-radius")].push_back(_T("0"));
	
	m_all_properties[_T("background")]            = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("background-color")]      = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("background-image")]      = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("background-repeat")]     = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("background-attachment")] = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("background-position")]   = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border")]                = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-top")]            = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-right")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-bottom")]         = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-left")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-color")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-top-color")]      = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-bottom-color")]   = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-left-color")]     = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-right-color")]    = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-style")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-top-style")]      = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-right-style")]    = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-left-style")]     = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-bottom-style")]   = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-width")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-top-width")]      = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-right-width")]    = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-left-width")]     = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-bottom-width")]   = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("border-collapse")]       = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("border-spacing")]        = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("bottom")]                = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("caption-side")]          = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("content")]               = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("clear")]                 = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("clip")]                  = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("color")]                 = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("counter-reset")]         = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("counter-increment")]     = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("cursor")]                = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("empty-cells")]           = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("display")]               = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("direction")]             = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("float")]                 = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("font")]                  = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("font-family")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("font-style")]            = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("font-variant")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("font-weight")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("font-stretch")]          = _T("CSS2.0");
	m_all_properties[_T("font-size-adjust")]      = _T("CSS2.0");
	m_all_properties[_T("font-size")]             = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("height")]                = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("left")]                  = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("line-height")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("list-style")]            = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("list-style-type")]       = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("list-style-image")]      = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("list-style-position")]   = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("margin")]                = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("margin-top")]            = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("margin-right")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("margin-bottom")]         = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("margin-left")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("marks")]                 = _T("CSS1.0,CSS2.0");
	m_all_properties[_T("marker-offset")]         = _T("CSS2.0");
	m_all_properties[_T("max-height")]            = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("max-width")]             = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("min-height")]            = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("min-width")]             = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("overflow")]              = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("orphans")]               = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("outline")]               = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("outline-width")]         = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("outline-style")]         = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("outline-color")]         = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("padding")]               = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("padding-top")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("padding-right")]         = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("padding-bottom")]        = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("padding-left")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("page-break-before")]     = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("page-break-after")]      = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("page-break-inside")]     = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("page")]                  = _T("CSS2.0");
	m_all_properties[_T("position")]              = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("quotes")]                = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("right")]                 = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("size")]                  = _T("CSS1.0,CSS2.0");
	m_all_properties[_T("table-layout")]          = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("top")]                   = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("text-indent")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("text-align")]            = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("text-decoration")]       = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("text-shadow")]           = _T("CSS2.0");
	m_all_properties[_T("letter-spacing")]        = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("word-spacing")]          = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("text-transform")]        = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("white-space")]           = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("unicode-bidi")]          = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("vertical-align")]        = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("visibility")]            = _T("CSS1.0,CSS2.0,CSS2.1");
	m_all_properties[_T("width")]                 = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("widows")]                = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("z-index")]               = _T("CSS1.0,CSS2.0,CSS2.1");
  m_all_properties[_T("zoom")]                  = _T("CSS2.0,CSS2.1");
	/* Speech */
	m_all_properties[_T("volume")]                = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("speak")]                 = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("speak-header")]          = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("pause")]                 = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("pause-before")]          = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("pause-after")]           = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("cue")]                   = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("cue-before")]            = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("cue-after")]             = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("play-during")]           = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("azimuth")]               = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("elevation")]             = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("speech-rate")]           = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("voice-family")]          = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("pitch")]                 = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("pitch-range")]           = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("stress")]                = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("richness")]              = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("speak-punctuation")]     = _T("CSS2.0,CSS2.1");
	m_all_properties[_T("speak-numeral")]         = _T("CSS2.0,CSS2.1");

  // MS-Extensions for IE-4 and above
  // For know we will pretend that they are part of CSS 2.1
  m_all_properties[_T("background-position-x")]       = _T("CSS2.1");
  m_all_properties[_T("background-position-y")]       = _T("CSS2.1");
  m_all_properties[_T("ime-mode")]                    = _T("CSS2.1");
  m_all_properties[_T("layout-grid")]                 = _T("CSS2.1");
  m_all_properties[_T("layout-grid-mode")]            = _T("CSS2.1");
  m_all_properties[_T("layout-grid-type")]            = _T("CSS2.1");
  m_all_properties[_T("layout-grid-char")]            = _T("CSS2.1");
  m_all_properties[_T("line-break")]                  = _T("CSS2.1");
  m_all_properties[_T("ruby-align")]                  = _T("CSS2.1");
  m_all_properties[_T("ruby-overhang")]               = _T("CSS2.1");
  m_all_properties[_T("ruby-position")]               = _T("CSS2.1");
  m_all_properties[_T("scrollbar-3dlight-color")]     = _T("CSS2.1");
  m_all_properties[_T("scrollbar-arrow-color")]       = _T("CSS2.1");
  m_all_properties[_T("scrollbar-base-color")]        = _T("CSS2.1");
  m_all_properties[_T("scrollbar-darkshadow-color")]  = _T("CSS2.1");
  m_all_properties[_T("scrollbar-face-color")]        = _T("CSS2.1");
  m_all_properties[_T("scrollbar-highlight-color")]   = _T("CSS2.1");
  m_all_properties[_T("scrollbar-shadow-color")]      = _T("CSS2.1");
  m_all_properties[_T("scrollbar-track-color")]       = _T("CSS2.1");
  m_all_properties[_T("text-autospace")]              = _T("CSS2.1");
  m_all_properties[_T("text-justify")]                = _T("CSS2.1");
  m_all_properties[_T("word-break")]                  = _T("CSS2.1");
  m_all_properties[_T("word-wrap")]                   = _T("CSS2.1");
  m_all_properties[_T("writing-mode")]                = _T("CSS2.1");   

	m_background_prop_default[_T("background-image")]      = _T("none");
	m_background_prop_default[_T("background-size")]       = _T("auto");
	m_background_prop_default[_T("background-repeat")]     = _T("repeat");
	m_background_prop_default[_T("background-position")]   = _T("0 0");
	m_background_prop_default[_T("background-attachment")] = _T("scroll");
	m_background_prop_default[_T("background-clip")]       = _T("border");
	m_background_prop_default[_T("background-origin")]     = _T("padding");
	m_background_prop_default[_T("background-color")]      = _T("transparent");
	
	m_at_rules[_T("page")]      = is;
	m_at_rules[_T("font-face")] = is;
	m_at_rules[_T("charset")]   = iv;
	m_at_rules[_T("import")]    = iv;
	m_at_rules[_T("namespace")] = iv;
	m_at_rules[_T("media")]     = at;

  m_predefined_templates[_T("high")].push_back(_T("<span class=\"at\">"));
  m_predefined_templates[_T("high")].push_back(_T("</span> <span class=\"format\">{</span>\n"));
  m_predefined_templates[_T("high")].push_back(_T("<span class=\"selector\">"));
  m_predefined_templates[_T("high")].push_back(_T("</span><span class=\"format\">{</span>"));
  m_predefined_templates[_T("high")].push_back(_T("<span class=\"property\">"));
  m_predefined_templates[_T("high")].push_back(_T("</span><span class=\"value\">"));
  m_predefined_templates[_T("high")].push_back(_T("</span><span class=\"format\">;</span>"));
  m_predefined_templates[_T("high")].push_back(_T("<span class=\"format\">}</span>"));
  m_predefined_templates[_T("high")].push_back(_T("\n"));
  m_predefined_templates[_T("high")].push_back(_T("\n<span class=\"format\">}\n</span>"));
  m_predefined_templates[_T("high")].push_back(_T(""));
  m_predefined_templates[_T("high")].push_back(_T("<span class=\"comment\">")); // before comment
  m_predefined_templates[_T("high")].push_back(_T("</span>")); //after comment
  m_predefined_templates[_T("high")].push_back(_T("\n")); // after last line @-rule

  m_predefined_templates[_T("highest")].push_back(_T("<span class=\"at\">"));
  m_predefined_templates[_T("highest")].push_back(_T("</span><span class=\"format\">{</span>"));
  m_predefined_templates[_T("highest")].push_back(_T("<span class=\"selector\">"));
  m_predefined_templates[_T("highest")].push_back(_T("</span><span class=\"format\">{</span>"));
  m_predefined_templates[_T("highest")].push_back(_T("<span class=\"property\">"));
  m_predefined_templates[_T("highest")].push_back(_T("</span><span class=\"value\">"));
  m_predefined_templates[_T("highest")].push_back(_T("</span><span class=\"format\">;</span>"));
  m_predefined_templates[_T("highest")].push_back(_T("<span class=\"format\">}</span>"));
  m_predefined_templates[_T("highest")].push_back(_T(""));
  m_predefined_templates[_T("highest")].push_back(_T("<span class=\"format\">}</span>"));
  m_predefined_templates[_T("highest")].push_back(_T(""));
  m_predefined_templates[_T("highest")].push_back(_T("<span class=\"comment\">")); // before comment
  m_predefined_templates[_T("highest")].push_back(_T("</span>")); //after comment
  m_predefined_templates[_T("highest")].push_back(_T("")); // after last line @-rule

  m_predefined_templates[_T("low")].push_back(_T("<span class=\"at\">"));
  m_predefined_templates[_T("low")].push_back(_T("</span> <span class=\"format\">{</span>\n"));
  m_predefined_templates[_T("low")].push_back(_T("<span class=\"selector\">"));
  m_predefined_templates[_T("low")].push_back(_T("</span>\n<span class=\"format\">{</span>\n"));
  m_predefined_templates[_T("low")].push_back(_T("\t<span class=\"property\">"));
  m_predefined_templates[_T("low")].push_back(_T("</span><span class=\"value\">"));
  m_predefined_templates[_T("low")].push_back(_T("</span><span class=\"format\">;</span>\n"));
  m_predefined_templates[_T("low")].push_back(_T("<span class=\"format\">}</span>"));
  m_predefined_templates[_T("low")].push_back(_T("\n\n"));
  m_predefined_templates[_T("low")].push_back(_T("\n<span class=\"format\">}</span>\n\n"));
  m_predefined_templates[_T("low")].push_back(_T("\t"));
  m_predefined_templates[_T("low")].push_back(_T("<span class=\"comment\">")); // before comment
  m_predefined_templates[_T("low")].push_back(_T("</span>\n")); //after comment
  m_predefined_templates[_T("low")].push_back(_T("\n")); // after last line @-rule
}
