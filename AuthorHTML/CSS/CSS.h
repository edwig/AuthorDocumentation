// CSS STYLE SHEET.
// Interface header

#pragma once
#include "umap.hpp"

typedef umap<string, string>   pstore;
typedef umap<string, pstore >  sstore;
typedef umap<string, sstore>   css_struct;

enum parse_status
{
   is     // In a selector
  ,ip     // In a property
  ,iv     // In a value
  ,instr  // In a string
  ,ic     // In a comment
  ,at     // In an @ at-block
};

enum message_type
{
   Information
  ,Warning
  ,Error
};

enum token_type
{
   AT_START
  ,AT_END
  ,SEL_START
  ,SEL_END
  ,PROPERTY
  ,VALUE
  ,COMMENT
};

struct token
{
  token_type type;
  string     data;
};

struct message
{
  string       m;
  message_type t;
};

class CssStyleSheet 
{ 
public:
  CssStyleSheet();

  // Set and parse a filename
  bool SetFile(LPCTSTR p_filename,bool emptyOK = false);
  // Set a predefined template before parsing
  void SetTemplate(string value);

  // Get/Adds/Delete/Copy a property-value pair to an existing CSS structure
  string get (const string& media, const string& selector, const string& property);
  void   put (const string& media, const string& selector, const string& property,  const string& value);
  void   copy(const string  media, const string  selector, const string  media_new, const string selector_new);

  // Parse a piece of CSS code
  void parse_css(string css_input);

  // Called after "parse_css". True if valid CSS
  bool Valid();

  // Prints parsed CSS code
  bool print_css();

  // Get all selectors in a vector
  void GetSelectors(const string& media,vector<string>* selectors);
  // Get all properties of a selector
  void GetProperties(const string& media
                    ,const string& selector
                    ,vector<string>* properties);
  // Delete a specific selector
  void del_selector(const string& media,const string& selector);
  // Get a vector with all attributes
  void GetAllAttributes(vector<string>* attributes);
  // Print the logfiles to a string
  string print_logs();
  // See if the style sheet contains errors
  bool   has_errors();
  // For internal stylesheets
  string GetTheSheet();
  void   AddImport(string value);
  void   DeleteImport(string value);

public: 
		int                        m_properties;
    int                        m_selectors;
		string                     m_charset;
    string                     m_namesp;
    string                     m_css_level;
		vector<string>             m_import;
    vector<string>             m_csstemplate;
		map<int,vector<message> >  m_logs;
    map<string, int>           m_settings;
	
	private:
    // Prepare all data structures
    void    prepare();
		void    add_token(const token_type ttype, const string data, const bool force = false);
		void    convert_css_to_tokens();
		// Add a message to the message log
		void    log(const string msg, const message_type type, int iline = 0);
		
    string  file_get_contents(const string filename);
		int     SeekNoComment(const int key, const int move);
		string  HtmlSpecials(const string istring, const bool plain);	
		string  optimise_subvalue(string subvalue, const string property);
		void    explode_selectors();
    // Check byte order mark
    void    check_bom(string& line);
    /* Merges properties like margin */
    void    merge_4value_shorthands(string media, string selector);
    /* Dissolves properties like padding:10px 10px 10px to padding-top:10px;padding-bottom:10px;... */
    map<string,string> dissolve_4value_shorthands(string property, string value);
		// Parses unicode notations
		string  unicode(string& istring,int& i);
		// Checks if the chat in istring at i is a token
		bool    is_token(string& istring,const size_t i);

    // GLOBAL OPTIMIZER
    //
    // Color compression function. Converts all rgb() values to #-values and uses the short-form if possible. Also replaces color names and codes.
    string  cut_color(string color);
    // Compresses shorthand values. Example: margin:1px 1px 1px 1px -> margin:1px
    string  shorthand(string value);
    // Compresses numbers (ie. 1.0 -> 1 or 1.100 -> 1.1 
    string  compress_numbers(string subvalue, string property = "");
    // Checks if the next word in a string from pos is a CSS property
    bool    property_is_next(string istring, const size_t pos);
    // Compress font-weight
    int     c_font_weight(string& value);
    // Merges selectors which have the same properties
    void    merge_selectors(sstore& input);

    // BACKGROUND OPTIMIZER
    //
    // Dissolves the background property
    map<string,string> dissolve_short_bg(string istring);
    // Same as explode, but not within a string
    vector<string> explode_ws(char sep,string istring);
    // Merge backgrounds
    void merge_bg(umap<string,string>& css_input);

    // MISCALEANEOUS FUNCTIONS
    //
    // Checks if a charcter is escaped
    bool escaped(const string &istring, size_t pos);
    // Returns a char of a string at pos but checks the string-length before
    char s_at(const string &istring, size_t pos);
    // Splits a string at e
    vector<string> explode(const string e, string s, const bool check = false);
    // Implodes a string at e
    std::string implode(const string e, const vector<string> s);
    // Replaces <find> with <replace> in <str>
    string str_replace(const string find, const string replace, string str);
    // Replaces all values of <find> with <replace> in <str>
    string str_replace(const vector<string>& find, const string replace, string str);
    // Checks if a string exists in a string-array
    bool in_char_arr(const char* haystack, const char needle);
    bool in_str_array(const string& haystack, const char needle);
    bool in_str_array(const vector<string>& haystack, const string needle);
    // Replaces certain chars with their entities
    string htmlspecialchars(string istring, int quotes = 0);
    // Rounds a float value
    float round(const float &number, const int num_digits);
    // Replacement for max (so that I don't have to include unnecessary things)
    int cssmax(const int i1, const int i2);
    /* isspace() and isdigit() do not work correctly with UTF-8 strings */
    bool ctype_space(const char c);
    bool ctype_digit(const char c);
    bool ctype_xdigit(char c);
    bool ctype_alpha(char c);
    /* Unserialize string arrays */
    vector<string> unserialise_sa(const string istring);
    /* Serialize a string */
    string serialise_sa(const string istring);

    // CONVERSIONS
    //
    // Returns the lowercase version of a string
    string strtolower(string istring);
    // Apparently faster replacement for tolower
    char chartolower(const char c);
    // Returns the uppercase version of a string
    string strtoupper(string istring);
    char chartoupper(const char c);
    // Converts an integer to a hex-string
    string dechex(const int i);
    // Converts a hexadecimal number (string) to a decimal number
    double hexdec(string istring);
    // Converts float to string
    string f2str(const float f);
    // Converts a string to float
    float str2f(const string istring);
    // Converts a char to a string
    string char2str(const char c);
    string char2str(const char *c);

    // TRIM
    // Removes whitespace at the end and beginning of a string
    const string trim(const string istring);
    // Removes whitespace at the beginning of a string
    const string ltrim(const string istring);
    // Removes whitespace at the end of a string
    const string rtrim(const string istring);
    const string rtrim(const string istring, const string chars);
    // Removes HTML tags
    string strip_tags(string istring);

    // IMPORTANT
    //
    // Checks if value is important
    bool is_important(string value);
    // Get value without !important
    string gvw_important(string value);
    // Compresses !important (for example if someone uses "! important")
    string c_important(string value);

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
    string        m_filename;
    // Helpers for parsing
    string        m_tokens;
    string        m_cur_selector;
    string        m_cur_at;
    string        m_cur_property;
    string        m_cur_sub_value;
    string        m_cur_value;
    int           m_line;
    int           m_input_size;
    int           m_output_size;
    float         m_compress_ratio;
    vector<int>   m_sel_separate;
    // For internal stylesheets
    string        m_theSheet;

    // General prepared data structures to parse CSS
    vector<string>                m_number_values;
    vector<string>                m_color_values;
    map<string,string>            m_background_prop_default;
    map<string,string>            m_replace_colors;
    map<string,string>            m_all_properties;
    map<string,vector<string> >   m_shorthands;
    map<string,vector<string> >   m_predefined_templates;
    map<string,parse_status>      m_at_rules;
};
