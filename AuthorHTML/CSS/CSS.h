// CSS STYLE SHEET.
// Interface header

#pragma once
#include "umap.hpp"
#include "CSS_utils.h"

class CssStyleSheet 
{ 
public:
  CssStyleSheet();

  // Set and parse a filename
  bool SetFile(LPCTSTR p_filename,bool emptyOK = false);
  // Set a predefined template before parsing
  void SetTemplate(XString value);

  // Get/Adds/Delete/Copy a property-value pair to an existing CSS structure
  XString get (const XString& media, const XString& selector, const XString& property);
  void    put (const XString& media, const XString& selector, const XString& property,  const XString& value);
  void    copy(const XString  media, const XString  selector, const XString  media_new, const XString selector_new);

  // Parse a piece of CSS code
  void parse_css(XString css_input);

  // Called after "parse_css". True if valid CSS
  bool Valid();

  // Prints parsed CSS code
  bool print_css();

  // Get all selectors in a vector
  void GetSelectors(const XString& media,vector<XString>* selectors);
  // Get all properties of a selector
  void GetProperties(const XString& media
                    ,const XString& selector
                    ,vector<XString>* properties);
  // Delete a specific selector
  void del_selector(const XString& media,const XString& selector);
  // Get a vector with all attributes
  void GetAllAttributes(vector<XString>* attributes);
  // Print the logfiles to a string
  XString print_logs();
  // See if the style sheet contains errors
  bool   has_errors();
  // For internal stylesheets
  XString GetTheSheet();
  void    AddImport(XString value);
  void    DeleteImport(XString value);

public:
		int                        m_properties;
    int                        m_selectors;
		XString                    m_charset;
    XString                    m_namesp;
    XString                    m_css_level;
		vector<XString>            m_import;
    vector<XString>            m_csstemplate;
		map<int,vector<message> >  m_logs;
    map<XString, int>         m_settings;
	
	private:
    // Prepare all data structures
    void    prepare();
		void    add_token(const token_type ttype, const XString data, const bool force = false);
		void    convert_css_to_tokens();
		// Add a message to the message log
		void    log(const XString msg, const message_type type, int iline = 0);

    XString file_get_contents(const XString filename);
		int     SeekNoComment(const int key, const int move);
		XString HtmlSpecials(const XString istring, const bool plain);
		XString optimise_subvalue(XString subvalue, const XString property);
		void    explode_selectors();
    /* Merges properties like margin */
    void    merge_4value_shorthands(XString media, XString selector);
    /* Dissolves properties like padding:10px 10px 10px to padding-top:10px;padding-bottom:10px;... */
    map<XString,XString> dissolve_4value_shorthands(XString property, XString value);
		// Parses unicode notations
		XString unicode(XString& istring,int& i);
		// Checks if the chat in istring at i is a token
		bool    is_token(XString& istring,const size_t i);

    // GLOBAL OPTIMIZER
    //
    // Color compression function. Converts all rgb() values to #-values and uses the short-form if possible. Also replaces color names and codes.
    XString cut_color(XString color);
    // Compresses shorthand values. Example: margin:1px 1px 1px 1px -> margin:1px
    XString shorthand(XString value);
    // Compresses numbers (ie. 1.0 -> 1 or 1.100 -> 1.1
    XString compress_numbers(XString subvalue, XString property = _T(""));
    // Checks if the next word in a string from pos is a CSS property
    bool    property_is_next(XString istring, const size_t pos);
    // Compress font-weight
    int     c_font_weight(XString& value);
    // Merges selectors which have the same properties
    void    merge_selectors(sstore& input);

    // BACKGROUND OPTIMIZER
    //
    // Dissolves the background property
    map<XString,XString> dissolve_short_bg(XString istring);
    // Same as explode, but not within a string
    vector<XString> explode_ws(TCHAR sep,XString istring);
    // Merge backgrounds
    void merge_bg(umap<XString,XString>& css_input);

    // MISCALEANEOUS FUNCTIONS
    //
    // Checks if a charcter is escaped
    bool escaped(const XString &istring, size_t pos);
    // Returns a char of a string at pos but checks the string-length before
    TCHAR s_at(const XString &istring, size_t pos);
    // Splits a string at e
    vector<XString> explode(const XString e, XString s, const bool check = false);
    // Implodes a string at e
    XString implode(const XString e, const vector<XString> s);
    // Replaces <find> with <replace> in <str>
    XString str_replace(const XString find, const XString replace, XString str);
    // Replaces all values of <find> with <replace> in <str>
    XString str_replace(const vector<XString>& find, const XString replace, XString str);
    // Checks if a string exists in a string-array
    bool in_char_arr(const TCHAR* haystack, const TCHAR needle);
    bool in_str_array(const XString& haystack, const TCHAR needle);
    bool in_str_array(const vector<XString>& haystack, const XString needle);
    // Replaces certain chars with their entities
    XString htmlspecialchars(XString istring, int quotes = 0);
    // Rounds a float value
    float round(const float &number, const int num_digits);
    // Replacement for max (so that I don't have to include unnecessary things)
    int cssmax(const int i1, const int i2);
    /* isspace() and isdigit() do not work correctly with UTF-8 strings */
    bool ctype_space(const TCHAR c);
    bool ctype_digit(const TCHAR c);
    bool ctype_xdigit(TCHAR c);
    bool ctype_alpha(TCHAR c);
    /* Unserialize string arrays */
    vector<XString> unserialise_sa(const XString istring);
    /* Serialize a string */
    XString serialise_sa(const XString istring);

    // CONVERSIONS
    //
    // Returns the lowercase version of a string
    XString strtolower(XString istring);
    // Apparently faster replacement for tolower
    TCHAR chartolower(const TCHAR c);
    // Returns the uppercase version of a string
    XString strtoupper(XString istring);
    TCHAR  chartoupper(const TCHAR c);
    // Converts an integer to a hex-string
    XString dechex(const int i);
    // Converts a hexadecimal number (string) to a decimal number
    double hexdec(XString istring);
    // Converts float to string
    XString f2str(const float f);
    // Converts a string to float
    float str2f(const XString istring);
    // Converts a char to a string
    XString char2str(const TCHAR c);
    XString char2str(const TCHAR *c);

    // TRIM
    // Removes whitespace at the end and beginning of a string
    const XString trim(const XString istring);
    // Removes whitespace at the beginning of a string
    const XString ltrim(const XString istring);
    // Removes whitespace at the end of a string
    const XString rtrim(const XString istring);
    const XString rtrim(const XString istring, const XString chars);
    // Removes HTML tags
    XString strip_tags(XString istring);

    // IMPORTANT
    //
    // Checks if value is important
    bool is_important(XString value);
    // Get value without !important
    XString gvw_important(XString value);
    // Compresses !important (for example if someone uses "! important")
    XString c_important(XString value);

    //////////////////////////////////////////////////////////
    //
    // DATA
    //
    //////////////////////////////////////////////////////////

    // Parsed CSS sheet, broken down in SELECTORS/PROPERTIES/VALUES
    css_struct    m_css;
    // Unparsed CSS sheet
    vector<token> m_csstokens;
    // Original file
    XString       m_filename;
    // Helpers for parsing
    XString       m_tokens;
    XString       m_cur_selector;
    XString       m_cur_at;
    XString       m_cur_property;
    XString       m_cur_sub_value;
    XString       m_cur_value;
    int           m_line;
    int           m_input_size;
    int           m_output_size;
    float         m_compress_ratio;
    vector<int>   m_sel_separate;
    // For internal stylesheets
    XString       m_theSheet;

    // General prepared data structures to parse CSS
    vector<XString>                m_number_values;
    vector<XString>                m_color_values;
    map<XString,XString>           m_background_prop_default;
    map<XString,XString>           m_replace_colors;
    map<XString,XString>           m_all_properties;
    map<XString,vector<XString> >  m_shorthands;
    map<XString,vector<XString> >  m_predefined_templates;
    map<XString,parse_status>      m_at_rules;
};
