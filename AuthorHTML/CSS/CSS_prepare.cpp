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
  m_charset       = "";
  m_namesp        = "";
  m_css_level     = "CSS2.1";
  m_tokens        = "{};:()@='\"/,\\!$%&*+.<>?[]^`|~";
  m_cur_selector  = "";
  m_cur_at        = "";
  m_cur_property  = "";
  m_cur_sub_value = "";
  m_cur_value     = "";
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
  m_settings["remove_bslash"]        = 1;
  m_settings["compress_colors"]      = 1;
  m_settings["compress_font-weight"] = 0;
  m_settings["lowercase_s"]          = 0;
  m_settings["optimise_shorthands"]  = 1;
  m_settings["remove_last_;"]        = 0;
  m_settings["case_properties"]      = 0;
  m_settings["sort_properties"]      = 0;
  m_settings["sort_selectors"]       = 0;
  m_settings["merge_selectors"]      = 2;
  m_settings["discard_invalid_properties"]  = 0;
  m_settings["allow_html_in_templates"]     = 0;
  m_settings["silent"]               = 0;
  m_settings["preserve_css"]         = 0;
  m_settings["timestamp"]            = 1;

  m_csstemplate.push_back("<span class=\"at\">"); //string before @rule
  m_csstemplate.push_back("</span> <span class=\"format\">{</span>\n"); //bracket after @-rule
  m_csstemplate.push_back("<span class=\"selector\">"); //string before selector
  m_csstemplate.push_back("</span> <span class=\"format\">{</span>\n"); //bracket after selector
  m_csstemplate.push_back("<span class=\"property\">"); //string before property
  m_csstemplate.push_back("</span><span class=\"value\">"); //string after property+before value
  m_csstemplate.push_back("</span><span class=\"format\">;</span>\n"); //string after value
  m_csstemplate.push_back("<span class=\"format\">}</span>"); //closing bracket - selector
  m_csstemplate.push_back("\n\n"); //space between blocks {...}
  m_csstemplate.push_back("\n<span class=\"format\">}</span>\n\n"); //closing bracket @-rule
  m_csstemplate.push_back(""); //indent in @-rule
  m_csstemplate.push_back("<span class=\"comment\">"); // before comment
  m_csstemplate.push_back("</span>\n"); //after comment
  m_csstemplate.push_back("\n"); // after last line @-rule

	m_number_values.push_back("line-height");
	m_number_values.push_back("pitch-range");
	m_number_values.push_back("richness");
	m_number_values.push_back("speech-rate");
	m_number_values.push_back("stress");
	m_number_values.push_back("volume");
	m_number_values.push_back("font");
	m_number_values.push_back("font-weight");
	m_number_values.push_back("z-index");
	m_number_values.push_back("counter-increment");
	m_number_values.push_back("counter-reset");
	m_number_values.push_back("orphans");
	m_number_values.push_back("widows");
	
	m_color_values.push_back("color");
	m_color_values.push_back("background-color");
	m_color_values.push_back("border-color");
	m_color_values.push_back("border-top-color");
	m_color_values.push_back("border-right-color");
	m_color_values.push_back("border-bottom-color");
	m_color_values.push_back("border-left-color");
	m_color_values.push_back("border-color");
	m_color_values.push_back("color");
	m_color_values.push_back("outline-color");

	m_replace_colors["aliceblue"]         = "#F0F8FF";
	m_replace_colors["antiquewhite"]      = "#FAEBD7";
	m_replace_colors["aquamarine"]        = "#7FFFD4";
	m_replace_colors["azure"]             = "#F0FFFF";
	m_replace_colors["beige"]             = "#F5F5DC";
	m_replace_colors["bisque"]            = "#FFE4C4";
	m_replace_colors["blanchedalmond"]    = "#FFEBCD";
	m_replace_colors["blueviolet"]        = "#8A2BE2";
	m_replace_colors["brown"]             = "#A52A2A";
	m_replace_colors["burlywood"]         = "#DEB887";
	m_replace_colors["cadetblue"]         = "#5F9EA0";
	m_replace_colors["chartreuse"]        = "#7FFF00";
	m_replace_colors["chocolate"]         = "#D2691E";
	m_replace_colors["coral"]             = "#FF7F50";
	m_replace_colors["cornflowerblue"]    = "#6495ED";
	m_replace_colors["cornsilk"]          = "#FFF8DC";
	m_replace_colors["crimson"]           = "#DC143C";
	m_replace_colors["cyan"]              = "#00FFFF";
	m_replace_colors["darkblue"]          = "#00008B";
	m_replace_colors["darkcyan"]          = "#008B8B";
	m_replace_colors["darkgoldenrod"]     = "#B8860B";
	m_replace_colors["darkgray"]          = "#A9A9A9";
	m_replace_colors["darkgreen"]         = "#006400";
	m_replace_colors["darkkhaki"]         = "#BDB76B";
	m_replace_colors["darkmagenta"]       = "#8B008B";
	m_replace_colors["darkolivegreen"]    = "#556B2F";
	m_replace_colors["darkorange"]        = "#FF8C00";
	m_replace_colors["darkorchid"]        = "#9932CC";
	m_replace_colors["darkred"]           = "#8B0000";
	m_replace_colors["darksalmon"]        = "#E9967A";
	m_replace_colors["darkseagreen"]      = "#8FBC8F";
	m_replace_colors["darkslateblue"]     = "#483D8B";
	m_replace_colors["darkslategray"]     = "#2F4F4F";
	m_replace_colors["darkturquoise"]     = "#00CED1";
	m_replace_colors["darkviolet"]        = "#9400D3";
	m_replace_colors["deeppink"]          = "#FF1493";
	m_replace_colors["deepskyblue"]       = "#00BFFF";
	m_replace_colors["dimgray"]           = "#696969";
	m_replace_colors["dodgerblue"]        = "#1E90FF";
	m_replace_colors["feldspar"]          = "#D19275";
	m_replace_colors["firebrick"]         = "#B22222";
	m_replace_colors["floralwhite"]       = "#FFFAF0";
	m_replace_colors["forestgreen"]       = "#228B22";
	m_replace_colors["gainsboro"]         = "#DCDCDC";
	m_replace_colors["ghostwhite"]        = "#F8F8FF";
	m_replace_colors["gold"]              = "#FFD700";
	m_replace_colors["goldenrod"]         = "#DAA520";
	m_replace_colors["greenyellow"]       = "#ADFF2F";
	m_replace_colors["honeydew"]          = "#F0FFF0";
	m_replace_colors["hotpink"]           = "#FF69B4";
	m_replace_colors["indianred"]         = "#CD5C5C";
	m_replace_colors["indigo"]            = "#4B0082";
	m_replace_colors["ivory"]             = "#FFFFF0";
	m_replace_colors["khaki"]             = "#F0E68C";
	m_replace_colors["lavender"]          = "#E6E6FA";
	m_replace_colors["lavenderblush"]     = "#FFF0F5";
	m_replace_colors["lawngreen"]         = "#7CFC00";
	m_replace_colors["lemonchiffon"]      = "#FFFACD";
	m_replace_colors["lightblue"]         = "#ADD8E6";
	m_replace_colors["lightcoral"]        = "#F08080";
	m_replace_colors["lightcyan"]         = "#E0FFFF";
	m_replace_colors["lightgoldenrodyellow"] = "#FAFAD2";
	m_replace_colors["lightgrey"]         = "#D3D3D3";
	m_replace_colors["lightgreen"]        = "#90EE90";
	m_replace_colors["lightpink"]         = "#FFB6C1";
	m_replace_colors["lightsalmon"]       = "#FFA07A";
	m_replace_colors["lightseagreen"]     = "#20B2AA";
	m_replace_colors["lightskyblue"]      = "#87CEFA";
	m_replace_colors["lightslateblue"]    = "#8470FF";
	m_replace_colors["lightslategray"]    = "#778899";
	m_replace_colors["lightsteelblue"]    = "#B0C4DE";
	m_replace_colors["lightyellow"]       = "#FFFFE0";
	m_replace_colors["limegreen"]         = "#32CD32";
	m_replace_colors["linen"]             = "#FAF0E6";
	m_replace_colors["magenta"]           = "#FF00FF";
	m_replace_colors["mediumaquamarine"]  = "#66CDAA";
	m_replace_colors["mediumblue"]        = "#0000CD";
	m_replace_colors["mediumorchid"]      = "#BA55D3";
	m_replace_colors["mediumpurple"]      = "#9370D8";
	m_replace_colors["mediumseagreen"]    = "#3CB371";
	m_replace_colors["mediumslateblue"]   = "#7B68EE";
	m_replace_colors["mediumspringgreen"] = "#00FA9A";
	m_replace_colors["mediumturquoise"]   = "#48D1CC";
	m_replace_colors["mediumvioletred"]   = "#C71585";
	m_replace_colors["midnightblue"]      = "#191970";
	m_replace_colors["mintcream"]         = "#F5FFFA";
	m_replace_colors["mistyrose"]         = "#FFE4E1";
	m_replace_colors["moccasin"]          = "#FFE4B5";
	m_replace_colors["navajowhite"]       = "#FFDEAD";
	m_replace_colors["oldlace"]           = "#FDF5E6";
	m_replace_colors["olivedrab"]         = "#6B8E23";
	m_replace_colors["orangered"]         = "#FF4500";
	m_replace_colors["orchid"]            = "#DA70D6";
	m_replace_colors["palegoldenrod"]     = "#EEE8AA";
	m_replace_colors["palegreen"]         = "#98FB98";
	m_replace_colors["paleturquoise"]     = "#AFEEEE";
	m_replace_colors["palevioletred"]     = "#D87093";
	m_replace_colors["papayawhip"]        = "#FFEFD5";
	m_replace_colors["peachpuff"]         = "#FFDAB9";
	m_replace_colors["peru"]              = "#CD853F";
	m_replace_colors["pink"]              = "#FFC0CB";
	m_replace_colors["plum"]              = "#DDA0DD";
	m_replace_colors["powderblue"]        = "#B0E0E6";
	m_replace_colors["rosybrown"]         = "#BC8F8F";
	m_replace_colors["royalblue"]         = "#4169E1";
	m_replace_colors["saddlebrown"]       = "#8B4513";
	m_replace_colors["salmon"]            = "#FA8072";
	m_replace_colors["sandybrown"]        = "#F4A460";
	m_replace_colors["seagreen"]          = "#2E8B57";
	m_replace_colors["seashell"]          = "#FFF5EE";
	m_replace_colors["sienna"]            = "#A0522D";
	m_replace_colors["skyblue"]           = "#87CEEB";
	m_replace_colors["slateblue"]         = "#6A5ACD";
	m_replace_colors["slategray"]         = "#708090";
	m_replace_colors["snow"]              = "#FFFAFA";
	m_replace_colors["springgreen"]       = "#00FF7F";
	m_replace_colors["steelblue"]         = "#4682B4";
	m_replace_colors["tan"]               = "#D2B48C";
	m_replace_colors["thistle"]           = "#D8BFD8";
	m_replace_colors["tomato"]            = "#FF6347";
	m_replace_colors["turquoise"]         = "#40E0D0";
	m_replace_colors["violet"]            = "#EE82EE";
	m_replace_colors["violetred"]         = "#D02090";
	m_replace_colors["wheat"]             = "#F5DEB3";
	m_replace_colors["whitesmoke"]        = "#F5F5F5";
	m_replace_colors["yellowgreen"]       = "#9ACD32";
		
	m_shorthands["border-color"].push_back("border-top-color");
	m_shorthands["border-color"].push_back("border-right-color");
	m_shorthands["border-color"].push_back("border-bottom-color");
	m_shorthands["border-color"].push_back("border-left-color");
	m_shorthands["border-style"].push_back("border-top-style");
	m_shorthands["border-style"].push_back("border-right-style");
	m_shorthands["border-style"].push_back("border-bottom-style");
	m_shorthands["border-style"].push_back("border-left-style");
	m_shorthands["border-width"].push_back("border-top-width");
	m_shorthands["border-width"].push_back("border-right-width");
	m_shorthands["border-width"].push_back("border-bottom-width");
	m_shorthands["border-width"].push_back("border-left-width");
	m_shorthands["margin"].push_back("margin-top");
	m_shorthands["margin"].push_back("margin-right");
	m_shorthands["margin"].push_back("margin-bottom");
	m_shorthands["margin"].push_back("margin-left");
	m_shorthands["padding"].push_back("padding-top");
	m_shorthands["padding"].push_back("padding-right");
	m_shorthands["padding"].push_back("padding-bottom");
	m_shorthands["padding"].push_back("padding-left");
	m_shorthands["-moz-border-radius"].push_back("0");
	
	m_all_properties["background"]            = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["background-color"]      = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["background-image"]      = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["background-repeat"]     = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["background-attachment"] = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["background-position"]   = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border"]                = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-top"]            = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-right"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-bottom"]         = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-left"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-color"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-top-color"]      = "CSS2.0,CSS2.1";
	m_all_properties["border-bottom-color"]   = "CSS2.0,CSS2.1";
	m_all_properties["border-left-color"]     = "CSS2.0,CSS2.1";
	m_all_properties["border-right-color"]    = "CSS2.0,CSS2.1";
	m_all_properties["border-style"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-top-style"]      = "CSS2.0,CSS2.1";
	m_all_properties["border-right-style"]    = "CSS2.0,CSS2.1";
	m_all_properties["border-left-style"]     = "CSS2.0,CSS2.1";
	m_all_properties["border-bottom-style"]   = "CSS2.0,CSS2.1";
	m_all_properties["border-width"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-top-width"]      = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-right-width"]    = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-left-width"]     = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-bottom-width"]   = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["border-collapse"]       = "CSS2.0,CSS2.1";
	m_all_properties["border-spacing"]        = "CSS2.0,CSS2.1";
	m_all_properties["bottom"]                = "CSS2.0,CSS2.1";
	m_all_properties["caption-side"]          = "CSS2.0,CSS2.1";
	m_all_properties["content"]               = "CSS2.0,CSS2.1";
	m_all_properties["clear"]                 = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["clip"]                  = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["color"]                 = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["counter-reset"]         = "CSS2.0,CSS2.1";
	m_all_properties["counter-increment"]     = "CSS2.0,CSS2.1";
	m_all_properties["cursor"]                = "CSS2.0,CSS2.1";
	m_all_properties["empty-cells"]           = "CSS2.0,CSS2.1";
	m_all_properties["display"]               = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["direction"]             = "CSS2.0,CSS2.1";
	m_all_properties["float"]                 = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["font"]                  = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["font-family"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["font-style"]            = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["font-variant"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["font-weight"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["font-stretch"]          = "CSS2.0";
	m_all_properties["font-size-adjust"]      = "CSS2.0";
	m_all_properties["font-size"]             = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["height"]                = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["left"]                  = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["line-height"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["list-style"]            = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["list-style-type"]       = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["list-style-image"]      = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["list-style-position"]   = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["margin"]                = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["margin-top"]            = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["margin-right"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["margin-bottom"]         = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["margin-left"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["marks"]                 = "CSS1.0,CSS2.0";
	m_all_properties["marker-offset"]         = "CSS2.0";
	m_all_properties["max-height"]            = "CSS2.0,CSS2.1";
	m_all_properties["max-width"]             = "CSS2.0,CSS2.1";
	m_all_properties["min-height"]            = "CSS2.0,CSS2.1";
	m_all_properties["min-width"]             = "CSS2.0,CSS2.1";
	m_all_properties["overflow"]              = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["orphans"]               = "CSS2.0,CSS2.1";
	m_all_properties["outline"]               = "CSS2.0,CSS2.1";
	m_all_properties["outline-width"]         = "CSS2.0,CSS2.1";
	m_all_properties["outline-style"]         = "CSS2.0,CSS2.1";
	m_all_properties["outline-color"]         = "CSS2.0,CSS2.1";
	m_all_properties["padding"]               = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["padding-top"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["padding-right"]         = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["padding-bottom"]        = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["padding-left"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["page-break-before"]     = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["page-break-after"]      = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["page-break-inside"]     = "CSS2.0,CSS2.1";
	m_all_properties["page"]                  = "CSS2.0";
	m_all_properties["position"]              = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["quotes"]                = "CSS2.0,CSS2.1";
	m_all_properties["right"]                 = "CSS2.0,CSS2.1";
	m_all_properties["size"]                  = "CSS1.0,CSS2.0";
	m_all_properties["table-layout"]          = "CSS2.0,CSS2.1";
	m_all_properties["top"]                   = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["text-indent"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["text-align"]            = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["text-decoration"]       = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["text-shadow"]           = "CSS2.0";
	m_all_properties["letter-spacing"]        = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["word-spacing"]          = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["text-transform"]        = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["white-space"]           = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["unicode-bidi"]          = "CSS2.0,CSS2.1";
	m_all_properties["vertical-align"]        = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["visibility"]            = "CSS1.0,CSS2.0,CSS2.1";
	m_all_properties["width"]                 = "CSS2.0,CSS2.1";
	m_all_properties["widows"]                = "CSS2.0,CSS2.1";
	m_all_properties["z-index"]               = "CSS1.0,CSS2.0,CSS2.1";
  m_all_properties["zoom"]                  = "CSS2.0,CSS2.1";
	/* Speech */
	m_all_properties["volume"]                = "CSS2.0,CSS2.1";
	m_all_properties["speak"]                 = "CSS2.0,CSS2.1";
	m_all_properties["speak-header"]          = "CSS2.0,CSS2.1";
	m_all_properties["pause"]                 = "CSS2.0,CSS2.1";
	m_all_properties["pause-before"]          = "CSS2.0,CSS2.1";
	m_all_properties["pause-after"]           = "CSS2.0,CSS2.1";
	m_all_properties["cue"]                   = "CSS2.0,CSS2.1";
	m_all_properties["cue-before"]            = "CSS2.0,CSS2.1";
	m_all_properties["cue-after"]             = "CSS2.0,CSS2.1";
	m_all_properties["play-during"]           = "CSS2.0,CSS2.1";
	m_all_properties["azimuth"]               = "CSS2.0,CSS2.1";
	m_all_properties["elevation"]             = "CSS2.0,CSS2.1";
	m_all_properties["speech-rate"]           = "CSS2.0,CSS2.1";
	m_all_properties["voice-family"]          = "CSS2.0,CSS2.1";
	m_all_properties["pitch"]                 = "CSS2.0,CSS2.1";
	m_all_properties["pitch-range"]           = "CSS2.0,CSS2.1";
	m_all_properties["stress"]                = "CSS2.0,CSS2.1";
	m_all_properties["richness"]              = "CSS2.0,CSS2.1";
	m_all_properties["speak-punctuation"]     = "CSS2.0,CSS2.1";
	m_all_properties["speak-numeral"]         = "CSS2.0,CSS2.1";

  // MS-Extensions for IE-4 and above
  // For know we will pretend that they are part of CSS 2.1
  m_all_properties["background-position-x"]       = "CSS2.1";
  m_all_properties["background-position-y"]       = "CSS2.1";
  m_all_properties["ime-mode"]                    = "CSS2.1";
  m_all_properties["layout-grid"]                 = "CSS2.1";
  m_all_properties["layout-grid-mode"]            = "CSS2.1";
  m_all_properties["layout-grid-type"]            = "CSS2.1";
  m_all_properties["layout-grid-char"]            = "CSS2.1";
  m_all_properties["line-break"]                  = "CSS2.1";
  m_all_properties["ruby-align"]                  = "CSS2.1";
  m_all_properties["ruby-overhang"]               = "CSS2.1";
  m_all_properties["ruby-position"]               = "CSS2.1";
  m_all_properties["scrollbar-3dlight-color"]     = "CSS2.1";
  m_all_properties["scrollbar-arrow-color"]       = "CSS2.1";
  m_all_properties["scrollbar-base-color"]        = "CSS2.1";
  m_all_properties["scrollbar-darkshadow-color"]  = "CSS2.1";
  m_all_properties["scrollbar-face-color"]        = "CSS2.1";
  m_all_properties["scrollbar-highlight-color"]   = "CSS2.1";
  m_all_properties["scrollbar-shadow-color"]      = "CSS2.1";
  m_all_properties["scrollbar-track-color"]       = "CSS2.1";
  m_all_properties["text-autospace"]              = "CSS2.1";
  m_all_properties["text-justify"]                = "CSS2.1";
  m_all_properties["word-break"]                  = "CSS2.1";
  m_all_properties["word-wrap"]                   = "CSS2.1";
  m_all_properties["writing-mode"]                = "CSS2.1";   

	m_background_prop_default["background-image"]      = "none";
	m_background_prop_default["background-size"]       = "auto";
	m_background_prop_default["background-repeat"]     = "repeat";
	m_background_prop_default["background-position"]   = "0 0";
	m_background_prop_default["background-attachment"] = "scroll";
	m_background_prop_default["background-clip"]       = "border";
	m_background_prop_default["background-origin"]     = "padding";
	m_background_prop_default["background-color"]      = "transparent";
	
	m_at_rules["page"]      = is;
	m_at_rules["font-face"] = is;
	m_at_rules["charset"]   = iv;
	m_at_rules["import"]    = iv;
	m_at_rules["namespace"] = iv;
	m_at_rules["media"]     = at;

  m_predefined_templates["high"].push_back("<span class=\"at\">");
  m_predefined_templates["high"].push_back("</span> <span class=\"format\">{</span>\n");
  m_predefined_templates["high"].push_back("<span class=\"selector\">");
  m_predefined_templates["high"].push_back("</span><span class=\"format\">{</span>");
  m_predefined_templates["high"].push_back("<span class=\"property\">");
  m_predefined_templates["high"].push_back("</span><span class=\"value\">");
  m_predefined_templates["high"].push_back("</span><span class=\"format\">;</span>");
  m_predefined_templates["high"].push_back("<span class=\"format\">}</span>");
  m_predefined_templates["high"].push_back("\n");
  m_predefined_templates["high"].push_back("\n<span class=\"format\">}\n</span>");
  m_predefined_templates["high"].push_back("");
  m_predefined_templates["high"].push_back("<span class=\"comment\">"); // before comment
  m_predefined_templates["high"].push_back("</span>"); //after comment
  m_predefined_templates["high"].push_back("\n"); // after last line @-rule

  m_predefined_templates["highest"].push_back("<span class=\"at\">");
  m_predefined_templates["highest"].push_back("</span><span class=\"format\">{</span>");
  m_predefined_templates["highest"].push_back("<span class=\"selector\">");
  m_predefined_templates["highest"].push_back("</span><span class=\"format\">{</span>");
  m_predefined_templates["highest"].push_back("<span class=\"property\">");
  m_predefined_templates["highest"].push_back("</span><span class=\"value\">");
  m_predefined_templates["highest"].push_back("</span><span class=\"format\">;</span>");
  m_predefined_templates["highest"].push_back("<span class=\"format\">}</span>");
  m_predefined_templates["highest"].push_back("");
  m_predefined_templates["highest"].push_back("<span class=\"format\">}</span>");
  m_predefined_templates["highest"].push_back("");
  m_predefined_templates["highest"].push_back("<span class=\"comment\">"); // before comment
  m_predefined_templates["highest"].push_back("</span>"); //after comment
  m_predefined_templates["highest"].push_back(""); // after last line @-rule

  m_predefined_templates["low"].push_back("<span class=\"at\">");
  m_predefined_templates["low"].push_back("</span> <span class=\"format\">{</span>\n");
  m_predefined_templates["low"].push_back("<span class=\"selector\">");
  m_predefined_templates["low"].push_back("</span>\n<span class=\"format\">{</span>\n");
  m_predefined_templates["low"].push_back("\t<span class=\"property\">");
  m_predefined_templates["low"].push_back("</span><span class=\"value\">");
  m_predefined_templates["low"].push_back("</span><span class=\"format\">;</span>\n");
  m_predefined_templates["low"].push_back("<span class=\"format\">}</span>");
  m_predefined_templates["low"].push_back("\n\n");
  m_predefined_templates["low"].push_back("\n<span class=\"format\">}</span>\n\n");
  m_predefined_templates["low"].push_back("\t");
  m_predefined_templates["low"].push_back("<span class=\"comment\">"); // before comment
  m_predefined_templates["low"].push_back("</span>\n"); //after comment
  m_predefined_templates["low"].push_back("\n"); // after last line @-rule
}
