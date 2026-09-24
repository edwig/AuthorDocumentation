//////////////////////////////////////////////////////////////////////////
//
// System:  AuthorDocumentation
// Program: AuthorHTML
// File:    Misc.cpp
//
// Written by: ir W.E. Huisman
// Dates:      2007 - 2026
//
// Description: Class with miscallaneous functions
// Diverse static functions for HTML and CSS functionality
// Not found in any other class
//
#include <StdAfx.h>
#include "misc.h"
#include "AuthorHTML.h"
#include <string>
#include <vector>
#include <algorithm>
#include <io.h>

// INFO FOR TAGS

TagInfo taginfo[] =
{
   // TAG           DESCRIPTION                BLOCK USERS USED NOT-IE // IE6 in NL
   { _T("a"),          _T("Anchor"),                   0,    0,    0,   0   }
  ,{ _T("abbr"),       _T("Abbreviation"),             0,    0,    0,   1   }
  ,{ _T("acronym"),    _T("Acronym"),                  0,    0,    0,   1   }
  ,{ _T("address"),    _T("Address"),                  1,    1,    0,   0   }  // Adres
  ,{ _T("applet"),     _T("Applet"),                   0,    0,    0,   0   }
  ,{ _T("area"),       _T("Area in image map"),        0,    0,    0,   0   }
  ,{ _T("b"),          _T("Bold"),                     0,    0,    0,   0   }
  ,{ _T("base"),       _T("Base of document"),         0,    0,    0,   0   }
  ,{ _T("basefont"),   _T("Basefont"),                 0,    0,    0,   0   }
  ,{ _T("bdo"),        _T("Bi-directional-override"),  0,    0,    0,   0   }
  ,{ _T("bgsound"),    _T("Background sound"),         0,    0,    0,   0   }
  ,{ _T("big"),        _T("Bigger text"),              0,    0,    0,   0   }
  ,{ _T("blink"),      _T("Blinking text"),            0,    0,    0,   1   }
  ,{ _T("blockquote"), _T("Blockquote"),               1,    0,    0,   0   }
  ,{ _T("body"),       _T("Document"),                 0,    0,    0,   0   }
  ,{ _T("br"),         _T("Break"),                    0,    0,    0,   0   }
  ,{ _T("button"),     _T("Button"),                   0,    0,    0,   0   }
  ,{ _T("caption"),    _T("Table caption"),            1,    0,    0,   0   }
  ,{ _T("center"),     _T("Center text"),              0,    0,    0,   0   }
  ,{ _T("cite"),       _T("Citation"),                 0,    0,    0,   0   }
  ,{ _T("code"),       _T("Program code"),             0,    0,    0,   0   }
  ,{ _T("col"),        _T("Table column"),             0,    0,    0,   0   }
  ,{ _T("colgroup"),   _T("Column group"),             0,    0,    0,   0   }
  ,{ _T("dd"),         _T("Definition data"),          1,    1,    0,   0   }
  ,{ _T("del"),        _T("Deleted text"),             0,    0,    0,   0   }
  ,{ _T("dir"),        _T("Directory"),                1,    1,    0,   0   }  // Menulijst
  ,{ _T("div"),        _T("Generic container"),        0,    1,    0,   0   }
  ,{ _T("dl"),         _T("Definition list"),          1,    1,    0,   0   }  // Definitie
  ,{ _T("dt"),         _T("Definition term"),          1,    1,    0,   0   }  // Definitieterm
  ,{ _T("em"),         _T("Emphasis"),                 0,    0,    0,   0   }
  ,{ _T("embed"),      _T("Embedded object"),          0,    0,    0,   0   }
  ,{ _T("fieldset"),   _T("Set of fields"),            0,    0,    0,   0   }
  ,{ _T("font"),       _T("Font"),                     0,    0,    0,   0   }
  ,{ _T("form"),       _T("Form"),                     0,    0,    0,   0   }
  ,{ _T("frame"),      _T("Frame"),                    0,    0,    0,   0   }
  ,{ _T("frameset"),   _T("Set of frames"),            0,    0,    0,   0   }
  ,{ _T("h1"),         _T("Heading 1"),                1,    1,    0,   0   }
  ,{ _T("h2"),         _T("Heading 2"),                1,    1,    0,   0   }
  ,{ _T("h3"),         _T("Heading 3"),                1,    1,    0,   0   }
  ,{ _T("h4"),         _T("Heading 4"),                1,    1,    0,   0   }
  ,{ _T("h5"),         _T("Heading 5"),                1,    1,    0,   0   }
  ,{ _T("h6"),         _T("Heading 6"),                1,    1,    0,   0   }
  ,{ _T("head"),       _T("Head of document"),         0,    0,    0,   0   }
  ,{ _T("hr"),         _T("Horizontal ruler"),         0,    0,    0,   0   }
  ,{ _T("html"),       _T("HTML"),                     0,    0,    0,   0   }
  ,{ _T("i"),          _T("Italic"),                   0,    0,    0,   0   }
  ,{ _T("iframe"),     _T("Inline frame"),             0,    0,    0,   0   }
  ,{ _T("img"),        _T("Image"),                    0,    0,    0,   0   }
  ,{ _T("input"),      _T("Input field"),              0,    0,    0,   0   }
  ,{ _T("ins"),        _T("Inserted text"),            0,    0,    0,   0   }
  ,{ _T("isindex"),    _T("Is indexable document"),    0,    0,    0,   0   }
  ,{ _T("kbd"),        _T("Keyboard text"),            0,    0,    0,   0   }
  ,{ _T("keygen"),     _T("Key generator"),            0,    0,    0,   1   }
  ,{ _T("label"),      _T("Label"),                    0,    0,    0,   0   }
  ,{ _T("layer"),      _T("Layer"),                    0,    0,    0,   1   }
  ,{ _T("legend"),     _T("Legend of fieldset"),       0,    0,    0,   0   }
  ,{ _T("li"),         _T("List item"),                1,    1,    0,   0   }
  ,{ _T("link"),       _T("CSS Link"),                 0,    0,    0,   0   }
  ,{ _T("map"),        _T("Image map"),                0,    0,    0,   0   }
  ,{ _T("marquee"),    _T("Marquee"),                  1,    0,    0,   0   }
  ,{ _T("menu"),       _T("Menu"),                     1,    1,    0,   0   }  // menu
  ,{ _T("meta"),       _T("Meta information"),         0,    0,    0,   0   }
  ,{ _T("multicol"),   _T("Multicolumn text"),         1,    0,    0,   1   }
  ,{ _T("nobr"),       _T("No line breaks"),           0,    0,    0,   0   }
  ,{ _T("noembed"),    _T("Text i.o. embed's"),        0,    0,    0,   1   }
  ,{ _T("noframes"),   _T("Text i.o. frames"),         0,    0,    0,   0   }
  ,{ _T("nolayer"),    _T("Text i.o. layers"),         0,    0,    0,   1   }
  ,{ _T("noscript"),   _T("Text i.o. scripts"),        0,    0,    0,   1   }
  ,{ _T("object"),     _T("Embedded object"),          0,    0,    0,   0   }
  ,{ _T("ol"),         _T("Ordered list"),             1,    1,    0,   0   }  // genummerde lijst
  ,{ _T("optgroup"),   _T("Optional group menu"),      0,    0,    0,   1   }
  ,{ _T("option"),     _T("Option of select field"),   0,    0,    0,   0   }
  ,{ _T("p"),          _T("Paragraph"),                1,    1,    0,   0   }  // Normaal
  ,{ _T("param"),      _T("Parameter"),                0,    0,    0,   0   }
  ,{ _T("pre"),        _T("Preformatted"),             1,    1,    0,   0   }  // Met opmaak
  ,{ _T("q"),          _T("Inline quote"),             0,    0,    0,   0   }
  ,{ _T("s"),          _T("Strikethrough"),            0,    0,    0,   0   }
  ,{ _T("samp"),       _T("Sample output"),            0,    0,    0,   0   }
  ,{ _T("script"),     _T("Script"),                   0,    0,    0,   0   }
  ,{ _T("select"),     _T("Combobox select"),          0,    0,    0,   0   }
  ,{ _T("server"),     _T("Javascript server"),        0,    0,    0,   1   }
  ,{ _T("small"),      _T("Smaller font"),             0,    0,    0,   0   }
  ,{ _T("spacer"),     _T("Blank space"),              0,    0,    0,   0   }
  ,{ _T("span"),       _T("Text +"),                   0,    1,    0,   0   }
  ,{ _T("strike"),     _T("Strikethrough"),            0,    0,    0,   0   }
  ,{ _T("strong"),     _T("Bold"),                     0,    0,    0,   0   }
  ,{ _T("sub"),        _T("Subscript"),                0,    0,    0,   0   }
  ,{ _T("sup"),        _T("Superscript"),              0,    0,    0,   0   }
  ,{ _T("table"),      _T("Table"),                    0,    0,    0,   0   }
  ,{ _T("tbody"),      _T("Table body"),               0,    0,    0,   0   }
  ,{ _T("td"),         _T("Table cell"),               1,    0,    0,   0   }
  ,{ _T("textarea"),   _T("Form textarea"),            0,    0,    0,   0   }
  ,{ _T("tfoot"),      _T("Table footer"),             0,    0,    0,   0   }
  ,{ _T("th"),         _T("Table header cell"),        0,    0,    0,   0   }
  ,{ _T("thead"),      _T("Table header"),             0,    0,    0,   0   }
  ,{ _T("title"),      _T("Document title"),           0,    0,    0,   0   }
  ,{ _T("tr"),         _T("Table row"),                0,    0,    0,   0   }
  ,{ _T("tt"),         _T("Typewriter text"),          0,    0,    0,   0   }
  ,{ _T("u"),          _T("Underlined"),               0,    0,    0,   0   }
  ,{ _T("ul"),         _T("Unordered list"),           1,    1,    0,   0   }  // Lijst met opsommingstekens
  ,{ _T("var"),        _T("Variable"),                 0,    0,    0,   0   }
  ,{ _T("wbr"),        _T("Potential word break"),     0,    0,    0,   0   }
  ,{ _T("xmp"),        _T("Example"),                  1,    0,    0,   1   }
  ,{ NULL,         NULL,                       0,    0,    0,   0   }
};

// INFO FOR ATTRIBUTES

AttributeInfo attributes[] =
{
  { _T("background-repeat"), _T("no-repeat"),           _T("Don't tile")                       }
 ,{ _T("background-repeat"), _T("repeat-x"),            _T("Tile horizontally")                }
 ,{ _T("background-repeat"), _T("repeat-y"),            _T("Tile vertically")                  }
 ,{ _T("background-repeat"), _T("repeat"),              _T("Tile horizontally and vertically") }
 ,{ _T("background-repeat"), _T("inherit"),             _T("Inherited")                        }

 ,{ _T("list-style-position"), _T("unspecified"),       _T("Unspecified")                      }
 ,{ _T("list-style-position"), _T("inherit"),           _T("Inherited")                        }
 ,{ _T("list-style-position"), _T("inside"),            _T("Inside element box")               }
 ,{ _T("list-style-position"), _T("outside"),           _T("Outside element box")              }

 ,{ _T("list-style-type"),   _T("unspecified"),         _T("Unspecified")                      }
 ,{ _T("list-style-type"),   _T("none"),                _T("No list style type")               }
 ,{ _T("list-style-type"),   _T("inherit"),             _T("Inherited")                        }
 ,{ _T("list-style-type"),   _T("disc"),                _T("Filled up disc")                   }
 ,{ _T("list-style-type"),   _T("circle"),              _T("Open circle")                      }
 ,{ _T("list-style-type"),   _T("square"),              _T("Filled up square")                 }
 ,{ _T("list-style-type"),   _T("decimal"),             _T("Decimal numbers (1,2,3,...)")      }
 ,{ _T("list-style-type"),   _T("decimal-leading-zero"),_T("Decimal numbers with zero (01,02,...,99)") }
 ,{ _T("list-style-type"),   _T("lower-roman"),         _T("Lowercase roman (i,ii,iii,....)")  }
 ,{ _T("list-style-type"),   _T("upper-roman"),         _T("Uppercase roman (I,II,III,....)")  }
 ,{ _T("list-style-type"),   _T("lower-greek"),         _T("Lowercase greek (alfa,beta,gamma...)")     }
 ,{ _T("list-style-type"),   _T("lower-alpha"),         _T("Lowercase alpha (a,b,c,...)")      }
 ,{ _T("list-style-type"),   _T("lower-latin"),         _T("Lowercase latin (a,b,c,...)")      }
 ,{ _T("list-style-type"),   _T("upper-alpha"),         _T("Uppercase alpha (A,B,C,...)")      }
 ,{ _T("list-style-type"),   _T("upper-latin"),         _T("Uppercase latin (A,B,C,...)")      }
 ,{ _T("list-style-type"),   _T("hebrew"),              _T("Traditional Hebrew numbering")     }
 ,{ _T("list-style-type"),   _T("armenian"),            _T("Traditional Armenian numbering")   }
 ,{ _T("list-style-type"),   _T("georgian"),            _T("Georgian (an,ban,gan,...)")        }
 ,{ _T("list-style-type"),   _T("cjk-ideographic"),     _T("Plain Ideografical numbering")     }
 ,{ _T("list-style-type"),   _T("hiragana"),            _T("Hiragana (a,i,u,e,o,ka,ki,...)")            }
 ,{ _T("list-style-type"),   _T("katakana"),            _T("Katakana (A,I,U,E,O,KA,KI,...)")            }
 ,{ _T("list-style-type"),   _T("hiragana-iroha"),      _T("Hiragana-iroha (i,ro,ha,ni,ho,he,to,...)")  }
 ,{ _T("list-style-type"),   _T("katakana-iroha"),      _T("Katakana-iroha (I,RO,HA,NI,HO,HE,TO,...)")  }

 // Single;1.5 lines;Double;
 ,{ _T("line-height"),       _T(""),                    _T("Single")                    }
 ,{ _T("line-height"),       _T("150%"),                _T("1.5 lines")                 }
 ,{ _T("line-height"),       _T("200%"),                _T("Double")                    }

 // Textarea wordwrap
 ,{ _T("wrap"),              _T("off"),                 _T("No word wrap")              }
 ,{ _T("wrap"),              _T("soft"),                _T("Local word wrap")           }
 ,{ _T("wrap"),              _T("hard"),                _T("Word wrap to server")       }
 // IE doesn't know these types
 ,{ _T("wrap"),              _T("virtual"),             _T("Local word wrap")           }
 ,{ _T("wrap"),              _T("physical"),            _T("Word wrap to server")       }

 // target frames
 ,{ _T("target"),            _T(""),                    _T("Page default")              }
 ,{ _T("target"),            _T("_blank"),              _T("New window")                }
 ,{ _T("target"),            _T("_self"),               _T("Same frame")                }
 ,{ _T("target"),            _T("_parent"),             _T("Parent frame")              }
 ,{ _T("target"),            _T("_top"),                _T("Whole page")                }

 // Script types
 ,{ _T("script-type"),       _T("text/javascript"),     _T("Javascript")                }
 ,{ _T("script-type"),       _T("text/Jscript"),        _T("MS-JScript")                }
 ,{ _T("script-type"),       _T("text/vbs"),            _T("VBS")                       }
 ,{ _T("script-type"),       _T("text/vbscript"),       _T("VB-Script")                 }
 ,{ _T("script-type"),       _T("text/ecmascript"),     _T("EcmaScript")                }

 // Language
 ,{ _T("language"),          _T("Javascript"),          _T("Javascript")                }
 ,{ _T("language"),          _T("Javascript1.1"),       _T("Javascript 1.1")            }
 ,{ _T("language"),          _T("Javascript1.2"),       _T("Javascript 1.2")            }
 ,{ _T("language"),          _T("Javascript1.3"),       _T("Javascript 1.3")            }
 ,{ _T("language"),          _T("Javascript1.4"),       _T("Javascript 1.4")            }
 ,{ _T("language"),          _T("JScript"),             _T("MS-JScript")                }
 ,{ _T("language"),          _T("VBScript"),            _T("VB Script")                 }
 ,{ _T("language"),          _T("vbs"),                 _T("VBS")                       }
 ,{ _T("language"),          _T("EcmaScript"),          _T("EcmaScript")                }

 // Shape of a area
 ,{ _T("shape"),             _T("rect"),                _T("Rectangle")                 }
 ,{ _T("shape"),             _T("rectangle"),           _T("Rectangle")                 }
 ,{ _T("shape"),             _T("circ"),                _T("Circle")                    }
 ,{ _T("shape"),             _T("circle"),              _T("Circle")                    }
 ,{ _T("shape"),             _T("poly"),                _T("Polygon")                   }
 ,{ _T("shape"),             _T("polygon"),             _T("Polygon")                   }

 // Style of the border
 ,{ _T("border-style"),      _T("none"),                 _T("None")                     }
 ,{ _T("border-style"),      _T("solid"),                _T("Solid")                    }
 ,{ _T("border-style"),      _T("dotted"),               _T("Dotted")                   }
 ,{ _T("border-style"),      _T("dashed"),               _T("Dashed")                   }
 ,{ _T("border-style"),      _T("double"),               _T("Double")                   }
 ,{ _T("border-style"),      _T("groove"),               _T("Groove")                   }
 ,{ _T("border-style"),      _T("ridge"),                _T("Ridge")                    }
 ,{ _T("border-style"),      _T("inset"),                _T("Inset")                    }
 ,{ _T("border-style"),      _T("outset"),               _T("Outset")                   }
 ,{ _T("border-style"),      _T("inherit"),              _T("Inherit")                  }

 // Events for the <SCRIPT> tag
 ,{ _T("script-event"),      _T("onreadystatechange"),  _T("When the page loading is complete")}
 ,{ _T("script-event"),      _T("onselectstart"),       _T("When the user starts selection")   }

 ,{ NULL,                NULL,                  NULL                        }
};

// INFO FOR COLORS

InternetColors internet_colors[] = 
{
  // Safe name               RGB Color code
  { _T("aliceblue"),            _T("#A0CE00") }
 ,{ _T("antiquewhite"),         _T("#FAEBD7") } 
 ,{ _T("aqua"),                 _T("#00FFFF") }
 ,{ _T("aquamarine"),           _T("#7FFFD4") }
 ,{ _T("azure"),                _T("#F0FFFF") }
 ,{ _T("beige"),                _T("#F5F5DC") }
 ,{ _T("bisque"),               _T("#FFE4C4") }
 ,{ _T("black"),                _T("#000000") }
 ,{ _T("blanchedalmond"),       _T("#FFEBCD") }
 ,{ _T("blue"),                 _T("#0000FF") }
 ,{ _T("blueviolet"),           _T("#8A2BE2") }
 ,{ _T("brown"),                _T("#A52A2A") }
 ,{ _T("burlywood"),            _T("#DEB887") }
 ,{ _T("cadetblue"),            _T("#5F9EA0") }
 ,{ _T("chartreuse"),           _T("#7EFF00") }
 ,{ _T("chocolate"),            _T("#D2691E") }
 ,{ _T("coral"),                _T("#FF7F50") }
 ,{ _T("cornflowerblue"),       _T("#6495ED") }
 ,{ _T("cornsilk"),             _T("#FFF8DC") }
 ,{ _T("crimson"),              _T("#DC143C") }
 ,{ _T("cyan"),                 _T("#00FFFF") }
 ,{ _T("darkblue"),             _T("#00008B") }
 ,{ _T("darkcyan"),             _T("#008B8B") }
 ,{ _T("darkgoldenrod"),        _T("#B8860B") }
 ,{ _T("darkgray"),             _T("#A9A9A9") }
 ,{ _T("darkgreen"),            _T("#006400") }
 ,{ _T("darkkhaki"),            _T("#BDB76B") }
 ,{ _T("darkmagenta"),          _T("#8B008B") }
 ,{ _T("darkolivegreen"),       _T("#556B2F") }
 ,{ _T("darkorange"),           _T("#FF8C00") }
 ,{ _T("darkorchid"),           _T("#9932CC") }
 ,{ _T("darkred"),              _T("#8B0000") }
 ,{ _T("darksalmon"),           _T("#E9967A") }
 ,{ _T("darkseagreen"),         _T("#8FBC8F") }
 ,{ _T("darkslateblue"),        _T("#483D8B") }
 ,{ _T("darkslategray"),        _T("#2F4F4F") }
 ,{ _T("darkturqoise"),         _T("#00CED1") }
 ,{ _T("darkviolet"),           _T("#9400D3") }
 ,{ _T("deeppink"),             _T("#FF1493") }
 ,{ _T("deepskyblue") ,         _T("#00BFFF") }
 ,{ _T("dimgray"),              _T("#696969") }
 ,{ _T("dodgerblue"),           _T("#1E90FF") }
 ,{ _T("fibrebrick"),           _T("#B22222") }
 ,{ _T("floralwhite"),          _T("#FFFAF0") }
 ,{ _T("forestgreen"),          _T("#228B22") }
 ,{ _T("fuchsia"),              _T("#FF00FF") }
 ,{ _T("gainsboro"),            _T("#DCDCDC") }
 ,{ _T("ghostwhite"),           _T("#F8F8FF") }
 ,{ _T("gold"),                 _T("#FFD700") }
 ,{ _T("goldenrod"),            _T("#DAA520") }
 ,{ _T("gray"),                 _T("#808080") }
 ,{ _T("green"),                _T("#008000") }
 ,{ _T("greenyellow"),          _T("#ADFF2F") }
 ,{ _T("honeydew"),             _T("#F0FFF0") }
 ,{ _T("hotpink"),              _T("#FF69B4") }
 ,{ _T("indianred"),            _T("#CD5C5C") }
 ,{ _T("indigo"),               _T("#4B0082") }
 ,{ _T("ivory"),                _T("#FFFFF0") }
 ,{ _T("khaki"),                _T("#F0E68C") }
 ,{ _T("lavender"),             _T("#E6E6FA") }
 ,{ _T("lavenderblush"),        _T("#FFF0F5") }
 ,{ _T("lemonchiffon"),         _T("#FFFACD") }
 ,{ _T("lightblue"),            _T("#ADD8E6") }
 ,{ _T("lightcoral"),           _T("#F08080") }
 ,{ _T("lightcyan"),            _T("#E0FFFF") }
 ,{ _T("lightgoldenrodyellow"), _T("#FAFAD2") }
 ,{ _T("lightgreen"),           _T("#90EE90") }
 ,{ _T("lightgrey"),            _T("#D3D3D3") }
 ,{ _T("lightpink"),            _T("#FFB6C1") }
 ,{ _T("lightsalmon"),          _T("#FFA07A") }
 ,{ _T("lightseagreen"),        _T("#20B2AA") }
 ,{ _T("lightskyblue"),         _T("#87CEFA") }
 ,{ _T("lightslategray"),       _T("#778899") }
 ,{ _T("lightsteelblue"),       _T("#B0C4DE") }
 ,{ _T("lightyellow"),          _T("#FFFFE0") }
 ,{ _T("lime"),                 _T("#00FF00") }
 ,{ _T("limegreen"),            _T("#32CD32") } 
 ,{ _T("linen"),                _T("#FAF0E6") }
 ,{ _T("magenta"),              _T("#FF00FF") }
 ,{ _T("maroon"),               _T("#800000") }
 ,{ _T("mediumaquamarine"),     _T("#66CDAA") }
 ,{ _T("mediumblue"),           _T("#0000CD") }
 ,{ _T("medumorchid"),          _T("#BA55D3") }
 ,{ _T("mediumpurple"),         _T("#9370DB") }
 ,{ _T("mediumseagreen"),       _T("#3CB371") }
 ,{ _T("mediumslateblue"),      _T("#7B68EE") }
 ,{ _T("mediumspringgreen"),    _T("#00FA9A") }
 ,{ _T("mediumturquoise"),      _T("#48D1CC") }
 ,{ _T("mediumvioletred"),      _T("#C71585") }
 ,{ _T("midnightblue"),         _T("#191970") }
 ,{ _T("mintcream"),            _T("#F5FFFA") }
 ,{ _T("mistyrose"),            _T("#FFF4E1") } 
 ,{ _T("navajowhite"),          _T("#FFDEAD") }
 ,{ _T("navy"),                 _T("#000080") }
 ,{ _T("oldlace"),              _T("#FDF5E6") }
 ,{ _T("olive"),                _T("#808000") }
 ,{ _T("olivedrab"),            _T("#6B8E23") }
 ,{ _T("orange"),               _T("#FFA500") }
 ,{ _T("orangered"),            _T("#FF4500") }
 ,{ _T("orchid"),               _T("#DA70D6") }
 ,{ _T("palegoldenrod"),        _T("#EEE8AA") }
 ,{ _T("palegreen"),            _T("#98FB98") }
 ,{ _T("paleturquoise"),        _T("#AFEEEE") }
 ,{ _T("palevioletred"),        _T("#DB7093") }
 ,{ _T("papayawhip"),           _T("#FFEFD5") }
 ,{ _T("peachpuff"),            _T("#FFDAB9") }
 ,{ _T("peru"),                 _T("#CD853F") }
 ,{ _T("pink"),                 _T("#FFC0CB") }
 ,{ _T("plum"),                 _T("#DDA0DD") }
 ,{ _T("powderblue"),           _T("#B0E0E6") }
 ,{ _T("purple"),               _T("#800080") }
 ,{ _T("red"),                  _T("#FF0000") }
 ,{ _T("rosybrown"),            _T("#BC8F8F") }
 ,{ _T("royalblue"),            _T("#4169E1") }
 ,{ _T("saddlebrown"),          _T("#8B4513") }
 ,{ _T("salmon"),               _T("#FA8072") }
 ,{ _T("sandybrown"),           _T("#F4A660") }
 ,{ _T("seagreen"),             _T("#2E8B57") }
 ,{ _T("seashell"),             _T("#FFF5EE") }
 ,{ _T("sienna"),               _T("#A0522D") }
 ,{ _T("silver"),               _T("#C0C0C0") }
 ,{ _T("skyblue"),              _T("#87CEEB") }
 ,{ _T("slateblue"),            _T("#6A5ACD") }
 ,{ _T("slategray"),            _T("#708090") }
 ,{ _T("snow"),                 _T("#FFFAFA") }
 ,{ _T("springgreen"),          _T("#00FF7F") }
 ,{ _T("steelblue"),            _T("#4682B4") }
 ,{ _T("tan"),                  _T("#D2B48C") }
 ,{ _T("teal"),                 _T("#008080") }
 ,{ _T("thistle"),              _T("#D8BFD8") }
 ,{ _T("tomato"),               _T("#FF6347") }
 ,{ _T("turquoise"),            _T("#40E0D0") }
 ,{ _T("violet"),               _T("#EE82EE") }
 ,{ _T("wheat"),                _T("#F5DEB3") }
 ,{ _T("white"),                _T("#FFFFFF") }
 ,{ _T("whitesmoke"),           _T("#F5F5F5") }
 ,{ _T("yellow"),               _T("#FFFF00") }
 ,{ _T("yellowgreen"),          _T("#9ACD32") }
 ,{ NULL,                   NULL      }
};

// INFO FOR DIACRITE CHARACTERS

#pragma warning(disable: 4245) // char to uchar conversion
#pragma warning(disable: 4838) // char to uchar conversion

Diacrites DiacriteLetters[] = 
{
   { _T('a'), _T('à'), ACC_LOWER, _T("&agrave;"), _T("a grave")       }
  ,{ _T('a'), _T('á'), ACC_LOWER, _T("&aacute;"), _T("a acute")       }
  ,{ _T('a'), _T('ä'), ACC_LOWER, _T("&auml;"),   _T("a umlaut")      }
  ,{ _T('a'), _T('â'), ACC_LOWER, _T("&acirc;"),  _T("a circumflex")  }
  ,{ _T('a'), _T('ã'), ACC_LOWER, _T("&atilde;"), _T("a tilde")       }
  ,{ _T('a'), _T('å'), ACC_LOWER, _T("&aring;"),  _T("a ring")        }

  ,{ _T('e'), _T('è'), ACC_LOWER, _T("&egrave;"), _T("e grave")       }
  ,{ _T('e'), _T('é'), ACC_LOWER, _T("&eacute;"), _T("e acute")       }
  ,{ _T('e'), _T('ë'), ACC_LOWER, _T("&euml;"),   _T("e umlaut")      }
  ,{ _T('e'), _T('ê'), ACC_LOWER, _T("&ecirc;"),  _T("e circumflex")  }

  ,{ _T('i'), _T('ì'), ACC_LOWER, _T("&igrave;"), _T("i grave")       }
  ,{ _T('i'), _T('í'), ACC_LOWER, _T("&iacute;"), _T("i acute")       }
  ,{ _T('i'), _T('ï'), ACC_LOWER, _T("&iuml;"),   _T("i umlaut")      }
  ,{ _T('i'), _T('î'), ACC_LOWER, _T("&icirc;"),  _T("i circumflex")  }

  ,{ _T('o'), _T('ò'), ACC_LOWER, _T("&ograve;"), _T("o grave")       }
  ,{ _T('o'), _T('ó'), ACC_LOWER, _T("&oacute;"), _T("o acute")       }
  ,{ _T('o'), _T('ö'), ACC_LOWER, _T("&ouml;"),   _T("o umlaut")      }
  ,{ _T('o'), _T('ô'), ACC_LOWER, _T("&ocirc;"),  _T("o circumflex")  }
  ,{ _T('o'), _T('õ'), ACC_LOWER, _T("&otilde;"), _T("o tilde")       }
  ,{ _T('o'), _T('ø'), ACC_LOWER, _T("&oslash;"), _T("o slash")       }

  ,{ _T('u'), _T('ù'), ACC_LOWER, _T("&ugrave;"), _T("u grave")       }
  ,{ _T('u'), _T('ú'), ACC_LOWER, _T("&uacute;"), _T("u acute")       }
  ,{ _T('u'), _T('ü'), ACC_LOWER, _T("&uuml;"),   _T("u umlaut")      }
  ,{ _T('u'), _T('û'), ACC_LOWER, _T("&ucirc;"),  _T("u circumflex")  }

  ,{ _T('y'), _T('ý'), ACC_LOWER, _T("&yacute;"), _T("y acute")       }
  ,{ _T('y'), _T('ÿ'), ACC_LOWER, _T("&yuml;"),   _T("y umlaut")      }

  ,{ _T('A'), _T('À'), ACC_UPPER, _T("&Agrave;"), _T("A grave")       }
  ,{ _T('A'), _T('Á'), ACC_UPPER, _T("&Aacute;"), _T("A acute")       }
  ,{ _T('A'), _T('Ä'), ACC_UPPER, _T("&Auml;"),   _T("A umlaut")      }
  ,{ _T('A'), _T('Â'), ACC_UPPER, _T("&Acirc;"),  _T("A circumflex")  }
  ,{ _T('A'), _T('Ã'), ACC_UPPER, _T("&Atilde;"), _T("A tilde")       }
  ,{ _T('A'), _T('Å'), ACC_UPPER, _T("&Aring;"),  _T("A ring")        }

  ,{ _T('E'), _T('È'), ACC_UPPER, _T("&Egrave;"), _T("E grave")       }
  ,{ _T('E'), _T('É'), ACC_UPPER, _T("&Eacute;"), _T("E acute")       }
  ,{ _T('E'), _T('Ë'), ACC_UPPER, _T("&Euml;"),   _T("E umlaut")      }
  ,{ _T('E'), _T('Ê'), ACC_UPPER, _T("&Ecirc;"),  _T("E circumflex")  }

  ,{ _T('I'), _T('Ì'), ACC_UPPER, _T("&Igrave;"), _T("I grave")       }
  ,{ _T('I'), _T('Í'), ACC_UPPER, _T("&Iacute;"), _T("I acute")       }
  ,{ _T('I'), _T('Ï'), ACC_UPPER, _T("&Iuml;"),   _T("I umlaut")      }
  ,{ _T('I'), _T('Î'), ACC_UPPER, _T("&Icirc;"),  _T("I circumflex")  }

  ,{ _T('O'), _T('Ò'), ACC_UPPER, _T("&Ograve;"), _T("O grave")       }
  ,{ _T('O'), _T('Ó'), ACC_UPPER, _T("&Oacute;"), _T("O acute")       }
  ,{ _T('O'), _T('Ö'), ACC_UPPER, _T("&Ouml;"),   _T("O umlaut")      }
  ,{ _T('O'), _T('Ô'), ACC_UPPER, _T("&Ocirc;"),  _T("O circumflex")  }
  ,{ _T('O'), _T('Ø'), ACC_UPPER, _T("&Oslash;"), _T("O slash")       }

  ,{ _T('U'), _T('Ù'), ACC_UPPER, _T("&Ugrave;"), _T("U grave")       }
  ,{ _T('U'), _T('Ú'), ACC_UPPER, _T("&Uacute;"), _T("U acute")       }
  ,{ _T('U'), _T('Ü'), ACC_UPPER, _T("&Uuml;"),   _T("U umlaut")      }
  ,{ _T('U'), _T('Û'), ACC_UPPER, _T("&Ucirc;"),  _T("U circumflex")  }

  ,{ _T('Y'), _T('Ý'), ACC_UPPER, _T("&Yacute;"), _T("Y acute")       }
  ,{ _T('Y'), _T('ÿ'), ACC_UPPER, _T("&Yuml;"),   _T("Y umlaut")      }

  ,{ _T('c'), _T('ç'), OTH_LOWER, _T("&ccedil;"), _T("c cedille")     }
  ,{ _T('n'), _T('ñ'), OTH_LOWER, _T("&ntilde;"), _T("n tilde")       }
  ,{ _T('s'), _T('ß'), OTH_LOWER, _T("&szlig;"),  _T("sz ligiture")   }
  ,{ _T('a'), _T('æ'), OTH_LOWER, _T("&aelig;"),  _T("ae ligiture")   }
  ,{ _T('e'), _T('Ð'), OTH_LOWER, _T("&eth;"),    _T("Islandic eth")  }
  ,{ _T('t'), _T('þ'), OTH_LOWER, _T("&thorn;"),  _T("Islandic thorn")}

  ,{ _T('C'), _T('Ç'), OTH_UPPER, _T("&Ccedil;"), _T("C cedille")     }
  ,{ _T('N'), _T('Ñ'), OTH_UPPER, _T("&Ntilde;"), _T("N tilde")       }
  ,{ _T('S'), _T('ß'), OTH_UPPER, _T("&SZlig;"),  _T("SZ ligiture")   }
  ,{ _T('A'), _T('Æ'), OTH_UPPER, _T("&AElig;"),  _T("AE ligature")   }
  ,{ _T('E'), _T('Ð'), OTH_UPPER, _T("&ETH;"),    _T("Islandic ETH")  }
  ,{ _T('T'), _T('Þ'), OTH_UPPER, _T("&THORN;"),  _T("Islandic THORN")}

  ,{ _T('a'), _T('a'), GRE_LOWER, _T("&alpha;"),   _T("alpha")        }
  ,{ _T('b'), _T('b'), GRE_LOWER, _T("&beta;"),    _T("beta")         }
  ,{ _T('g'), _T('g'), GRE_LOWER, _T("&gamma;"),   _T("gamma")        }
  ,{ _T('d'), _T('d'), GRE_LOWER, _T("&delta;"),   _T("delta")        }
  ,{ _T('e'), _T('e'), GRE_LOWER, _T("&epsilon;"), _T("epsilon")      }
  ,{ _T('z'), _T('z'), GRE_LOWER, _T("&zeta;"),    _T("zeta")         }
  ,{ _T('h'), _T('h'), GRE_LOWER, _T("&eta;"),     _T("eta")          }
  ,{ _T('o'), _T('o'), GRE_LOWER, _T("&theta;"),   _T("theta")        }
  ,{ _T('i'), _T('i'), GRE_LOWER, _T("&iota;"),    _T("iota")         }
  ,{ _T('k'), _T('k'), GRE_LOWER, _T("&kappa;"),   _T("kappa")        }
  ,{ _T('l'), _T('l'), GRE_LOWER, _T("&lambda;"),  _T("lambda")       }
  ,{ _T('m'), _T('m'), GRE_LOWER, _T("&mu;"),      _T("mu")           }
  ,{ _T('n'), _T('n'), GRE_LOWER, _T("&nu;"),      _T("nu")           }
  ,{ _T('x'), _T('x'), GRE_LOWER, _T("&xi;"),      _T("xi")           }
  ,{ _T('o'), _T('o'), GRE_LOWER, _T("&omnicron;"),_T("omnicron")     }
  ,{ _T('p'), _T('p'), GRE_LOWER, _T("&pi;"),      _T("pi")           }
  ,{ _T('r'), _T('r'), GRE_LOWER, _T("&rho;"),     _T("rho")          }
  ,{ _T('s'), _T('s'), GRE_LOWER, _T("&sigmaf;"),  _T("sigmaf")       }
  ,{ _T('s'), _T('s'), GRE_LOWER, _T("&sigma;"),   _T("sigma")        }
  ,{ _T('t'), _T('t'), GRE_LOWER, _T("&tau;"),     _T("tau")          }
  ,{ _T('y'), _T('y'), GRE_LOWER, _T("&upsilon;"), _T("upsilon")      }
  ,{ _T('f'), _T('f'), GRE_LOWER, _T("&phi;"),     _T("phi")          }
  ,{ _T('x'), _T('x'), GRE_LOWER, _T("&chi;"),     _T("chi")          }
  ,{ _T('p'), _T('p'), GRE_LOWER, _T("&psi;"),     _T("psi")          }
  ,{ _T('w'), _T('w'), GRE_LOWER, _T("&omega;"),   _T("omega")        }

  ,{ _T('a'), _T('A'), GRE_UPPER, _T("&Alpha;"),   _T("ALPHA")        }
  ,{ _T('b'), _T('B'), GRE_UPPER, _T("&Beta;"),    _T("BETA")         }
  ,{ _T('g'), _T('G'), GRE_UPPER, _T("&Gamma;"),   _T("GAMMA")        }
  ,{ _T('d'), _T('D'), GRE_UPPER, _T("&Delta;"),   _T("DELTA")        }
  ,{ _T('e'), _T('E'), GRE_UPPER, _T("&Epsilon;"), _T("EPSILON")      }
  ,{ _T('z'), _T('Z'), GRE_UPPER, _T("&Zeta;"),    _T("ZETA")         }
  ,{ _T('h'), _T('H'), GRE_UPPER, _T("&Eta;"),     _T("ETA")          }
  ,{ _T('o'), _T('O'), GRE_UPPER, _T("&Theta;"),   _T("THETA")        }
  ,{ _T('i'), _T('I'), GRE_UPPER, _T("&Iota;"),    _T("IOTA")         }
  ,{ _T('k'), _T('K'), GRE_UPPER, _T("&Kappa;"),   _T("KAPPA")        }
  ,{ _T('l'), _T('L'), GRE_UPPER, _T("&Lambda;"),  _T("LAMBDA")       }
  ,{ _T('m'), _T('M'), GRE_UPPER, _T("&Mu;"),      _T("MU")           }
  ,{ _T('n'), _T('N'), GRE_UPPER, _T("&Nu;"),      _T("NU")           }
  ,{ _T('x'), _T('X'), GRE_UPPER, _T("&Xi;"),      _T("XI")           }
  ,{ _T('o'), _T('O'), GRE_UPPER, _T("&Omnicron;"),_T("OMNICRON")     }
  ,{ _T('p'), _T('P'), GRE_UPPER, _T("&Pi;"),      _T("PI")           }
  ,{ _T('r'), _T('R'), GRE_UPPER, _T("&Rho;"),     _T("RHO")          }
  ,{ _T('s'), _T('S'), GRE_UPPER, _T("&Sigma;"),   _T("SIGMA")        }
  ,{ _T('t'), _T('T'), GRE_UPPER, _T("&Tau;"),     _T("TAU")          }
  ,{ _T('y'), _T('Y'), GRE_UPPER, _T("&Upsilon;"), _T("UPSILON")      }
  ,{ _T('f'), _T('F'), GRE_UPPER, _T("&Phi;"),     _T("PHI")          }
  ,{ _T('x'), _T('X'), GRE_UPPER, _T("&Chi;"),     _T("CHI")          }
  ,{ _T('p'), _T('P'), GRE_UPPER, _T("&Psi;"),     _T("PSI")          }
  ,{ _T('w'), _T('W'), GRE_UPPER, _T("&Omega;"),   _T("OMEGA")        }

//,{ _T('a'), _T('€'), COMMON,    _T("&euro;"),   _T("euro")          }
  ,{ _T('@'), _T('@'), COMMON,    _T("&#64;"),    _T("at-sign")       }
  ,{ _T('a'), _T('¢'), COMMON,    _T("&cent;"),   _T("cent")          }
  ,{ _T('a'), _T('£'), COMMON,    _T("&pound;"),  _T("pound")         }
  ,{ _T('a'), _T('¤'), COMMON,    _T("&curren;"), _T("currentcy")     }
  ,{ _T('a'), _T('¥'), COMMON,    _T("&yen;"),    _T("yen")           }
  ,{ _T('a'), _T('§'), COMMON,    _T("&sect;"),   _T("section")       }
  ,{ _T('a'), _T('©'), COMMON,    _T("&copy;"),   _T("copyright")     }
  ,{ _T('a'), _T('ª'), COMMON,    _T("&ordf;"),   _T("femenine ordinal")  }
  ,{ _T('a'), _T('º'), COMMON,    _T("&ordm;"),   _T("masculine ordinal") }
  ,{ _T('a'), _T('«'), COMMON,    _T("&laquo;"),  _T("left angle quote")  }
  ,{ _T('a'), _T('»'), COMMON,    _T("&raquo;"),  _T("right angle quote") }
  ,{ _T('a'), _T('¬'), COMMON,    _T("&not;"),    _T("not")           }
  ,{ _T('a'), _T('®'), COMMON,    _T("&reg;"),    _T("trademark")     }
  ,{ _T('a'), _T('¯'), COMMON,    _T("&macr;"),   _T("macron")        }
  ,{ _T('a'), _T('°'), COMMON,    _T("&deg;"),    _T("degree")        }
  ,{ _T('a'), _T('±'), COMMON,    _T("&plusmn;"), _T("plus minus")    }
  ,{ _T('a'), _T('¹'), COMMON,    _T("&sup1;"),   _T("superscript 1") }
  ,{ _T('a'), _T('²'), COMMON,    _T("&sup2;"),   _T("superscript 2") }
  ,{ _T('a'), _T('³'), COMMON,    _T("&sup3;"),   _T("superscript 3") }
  ,{ _T('a'), _T('µ'), COMMON,    _T("&micro;"),  _T("micro / mu")    }
  ,{ _T('a'), _T('×'), COMMON,    _T("&times;"),  _T("multiply")      }
//,{ _T('a'), _T('ƒ'), COMMON,    _T("&#131;"),   _T("florin")        }
  ,{ _T('a'), _T('¿'), COMMON,    _T("&iquest;"), _T("inverted question")    }
  ,{ _T('a'), _T('¡'), COMMON,    _T("&iexcl;"),  _T("inverted exclamation") }
  ,{ _T('a'), _T('½'), COMMON,    _T("&frac12;"), _T("fraction 1/2")  }
  ,{ _T('a'), _T('¼'), COMMON,    _T("&frac14;"), _T("fraction 1/4")  }
  ,{ _T('a'), _T('¾'), COMMON,    _T("&frac34;"), _T("fraction 3/4")  }
  ,{ _T('a'), _T('¶'), COMMON,    _T("&para;"),   _T("paragraph")     }
  ,{ _T('a'), _T('÷'), COMMON,    _T("&divide;"), _T("division")      }
  ,{ _T('a'), _T('·'), COMMON,    _T("&#149;"),   _T("bullet")        } 
  ,{ 0,       0,       0,         NULL,           NULL                }
};

// INFO for BEHAVIOR EVENTS ON TAGS

TagEvents tagEvents[] =
{
  // EVENTS IN HTML 4.01 standard
   { true,  _T("OnClick"),            _T("When clicking mousebutton"),                        TAGS_ALL    }
  ,{ true,  _T("OnDblClick"),         _T("When double clicking mousebutton"),                 TAGS_ALL    }
  ,{ true,  _T("OnKeypress"),         _T("When a key is pressed"),                            TAGS_ALL    }
  ,{ true,  _T("OnKeydown"),          _T("When a key is pressed down"),                       TAGS_ALL    }
  ,{ true,  _T("OnKeyup"),            _T("When a pressed key is going up"),                   TAGS_ALL    }
  ,{ true,  _T("OnMousedown"),        _T("When mousebutton is going down"),                   TAGS_ALL    }
  ,{ true,  _T("OnMousemove"),        _T("When the mouse moves"),                             TAGS_ALL    }
  ,{ true,  _T("OnMouseout"),         _T("When mouse is leaving element"),                    TAGS_ALL    }
  ,{ true,  _T("OnMouseover"),        _T("When mouse is moving over"),                        TAGS_ALL    }
  ,{ true,  _T("OnMouseup"),          _T("When mousebutton is going up"),                     TAGS_ALL    }
  ,{ true,  _T("OnFocus"),            _T("When an element gets focus"),                       TAGS_ALL    }
  ,{ true,  _T("OnBlur"),             _T("When an element loses focus"),                      TAGS_ALL    }
  ,{ true,  _T("OnLoad"),             _T("When the page loads"),                              TAGS_BODY   }
  ,{ true,  _T("OnUnload"),           _T("When leaving the page"),                            TAGS_BODY   }
  ,{ true,  _T("OnChange"),           _T("When a field has been changed"),                    TAGS_INPUT|TAGS_SELECT|TAGS_TEXTAREA  }
  ,{ true,  _T("OnSelect"),           _T("When some text is selected"),                       TAGS_INPUT|TAGS_TEXTAREA              }
  // EVENTS IN IE5++
  ,{ false, _T("OnAfterUpdate"),      _T("After update of <SCRIPT FOR=elem..."),              TAGS_ALL    }
  ,{ false, _T("OnBeforeUpdate"),     _T("Before update of <SCRIPT FOR=elem..."),             TAGS_ALL    }
  ,{ false, _T("OnDataAvailable"),    _T("If data available for <SCRIPT FOR=elem..."),        TAGS_ALL    }
  ,{ false, _T("OnDataSetChanged"),   _T("If data changed in <SCRIPT FOR=elem..."),           TAGS_ALL    }
  ,{ false, _T("OnDataSetComplete"),  _T("If all data loaded in <SCRIPT FOR=elem..."),        TAGS_ALL    }
  ,{ false, _T("OnErrorUpdate"),      _T("On an error while updating an element"),            TAGS_ALL    }
  ,{ false, _T("OnFilterChange"),     _T("When an image filter changes"),                     TAGS_ALL    }
  ,{ false, _T("OnHelp"),             _T("When the user requests F1 help"),                   TAGS_ALL    }
  ,{ false, _T("OnSelectStart"),      _T("When an element is selected"),                      TAGS_ALL    } 
  ,{ false, _T("OnCellChange"),       _T("When a data cell in a datasource changes"),         TAGS_OBJECT|TAGS_BODY    }
  ,{ false, _T("OnCopy"),             _T("When a users starts a 'copy' action"),              TAGS_ALL    }
  ,{ false, _T("OnCut"),              _T("When a users starts a 'cut' action"),               TAGS_ALL    }
  ,{ false, _T("OnPaste"),            _T("When a users starts a 'paste' action"),             TAGS_ALL    }
  ,{ false, _T("OnDrag"),             _T("When a users drags information"),                   TAGS_ALL    }
  ,{ false, _T("OnDragStart"),        _T("At the beginning of a drag operation"),             TAGS_ALL    }
  ,{ false, _T("OnDragEnd"),          _T("At the end of a drag operation"),                   TAGS_ALL    }
  ,{ false, _T("OnDragEnter"),        _T("When dragged object enters valid drop target"),     TAGS_ALL    }
  ,{ false, _T("OnDragLeave"),        _T("When dragged object leaves valid drop target"),     TAGS_ALL    }
  ,{ false, _T("OnDragOver"),         _T("When dragged object is over a valid drop target"),  TAGS_ALL    }
  ,{ false, _T("OnDrop"),             _T("When dragged object drops on a valid drop target"), TAGS_ALL    }
  ,{ false, _T("OnLoseCapture"),      _T("When an element loses the mouse capture"),          TAGS_ALL    }
  ,{ false, _T("OnPropertyChange"),   _T("When a property of an element changes"),            TAGS_ALL    }
  ,{ false, _T("OnReadyStateChange"), _T("When downloaded data or behavior is ready to use"), TAGS_ALL    }
  ,{ false, _T("OnResize"),           _T("When the size of an element is about to change"),   TAGS_ALL    }
  ,{ false, _T("OnRowEnter"),         _T("When a data source receives data"),                 TAGS_ALL    }
  ,{ false, _T("OnRowExit"),          _T("When a data source deletes data"),                  TAGS_ALL    }
  ,{ false, _T("OnRowsDelete"),       _T("When rows are deleted from a recordset"),           TAGS_OBJECT|TAGS_BODY }
  ,{ false, _T("OnRowsInserted"),     _T("When rows are inserted in a recordset"),            TAGS_OBJECT|TAGS_BODY }
  ,{ false, _T("OnScroll"),           _T("When the user repositions the scroll bar"),         TAGS_ALL    }
  ,{ false, _T("OnActivate"),         _T("When the element becomes active (not focus!)"),     TAGS_ALL    }
  ,{ false, _T("OnDeactivate"),       _T("When the element becomes not active (not focus!)"), TAGS_ALL    }
  ,{ false, _T("OnBeforeActivate"),   _T("Before activation of an element (not focus!)"),     TAGS_ALL    }
  ,{ false, _T("OnBeforeDeactivate"), _T("Before deactivation of an element (not focus!)"),   TAGS_ALL    }
  ,{ false, _T("OnLayoutComplete"),   _T("When the document layout is ready"),                TAGS_ALL    }
  ,{ false, _T("OnMouseEnter"),       _T("When the mouse moves into an element"),             TAGS_ALL    }
  ,{ false, _T("OnMouseLeave"),       _T("When the mouse moves out of an element"),           TAGS_ALL    }
  ,{ false, _T("OnControlSelect"),    _T("When element enters or leaves multi-selection"),    TAGS_ALL    }
  ,{ false, _T("OnResizeEnd"),        _T("After an element enters or leaves multi-selection"),  TAGS_ALL  }
  ,{ false, _T("OnResizeStart"),      _T("Before an element enters or leaves multi-selection"), TAGS_ALL  }
  ,{ false, _T("OnFocusIn"),          _T("Before setting focus on an element"),               TAGS_ALL    }
  ,{ false, _T("OnFocusOut"),         _T("After focus leaves an element"),                    TAGS_ALL    }
  ,{ false, _T("OnMouseWheel"),       _T("When the mouse wheel is rotated"),                  TAGS_ALL    }
  ,{ false, _T("OnBeforeUnload"),     _T("Before the document page is unloaded"),             TAGS_BODY   }
  ,{ false, _T("OnSelect"),           _T("When some text is selected"),                       TAGS_BODY   }
  ,{ false, _T("OnAfterPrint"),       _T("After the document is printed"),                    TAGS_BODY   }
  ,{ false, _T("OnBeforePrint"),      _T("Before the document is printed"),                   TAGS_BODY   }
  ,{ false, _T("OnAbort"),            _T("Error on loading the image leads to abort"),        TAGS_IMG|TAGS_INPUT }
  ,{ false, _T("OnError"),            _T("Error while loading the image/object/script"),      TAGS_IMG|TAGS_INPUT|TAGS_OBJECT|TAGS_SCRIPT|TAGS_STYLE}
  ,{ false, _T("OnLoad"),             _T("After loading the image"),                          TAGS_IMG|TAGS_INPUT|TAGS_STYLE }
  ,{ false, _T("OnBeforeEditFocus"),  _T("Before setting focus on editable content"),         TAGS_BODY   }
  ,{ false, _T("OnContextMenu"),      _T("Before showing the context menu"),                  TAGS_BODY   }
  ,{ false, _T("OnStop"),             _T("When the users uses 'stop' button or leaves page"), TAGS_BODY   }
  ,{ false, _T("OnSelectionChange"),  _T("When the selection state of document changes"),     TAGS_BODY   }
  ,{ false, _T("OnBounce"),           _T("When the direction of the motion changes"),         TAGS_MARQUEE}
  ,{ false, _T("OnFinish"),           _T("When total number of loops is reached"),            TAGS_MARQUEE}
  ,{ false, _T("OnStart"),            _T("At the beginning of every loop"),                   TAGS_MARQUEE}
  ,{ false, _T("OnSubmit"),           _T("When a form submit button is pressed"),             TAGS_FORM   }
  ,{ false, _T("OnReset"),            _T("When a form reset button is pressed"),              TAGS_FORM   } 
  ,{ false, _T(""),                   _T(""),                                                 0           }
};

static CMapStringToPtr  tagMap(80); // Finding tags by 'tag' string
static CMapStringToPtr descMap(80); // Finding tags by description (class/combos)

void
Misc::InitMaps()
{
  // Pointers to taginfo
  if(tagMap.IsEmpty())
  {
    for(int index=0; taginfo[index].tagname; ++index)
    {
      CString tagname  = taginfo[index].tagname;
      CString descript = taginfo[index].description;
      tagname. MakeLower();
      descript.MakeLower();
       tagMap.SetAt(tagname, (void*)(&taginfo[index]));
      descMap.SetAt(descript,(void*)(&taginfo[index]));
    }
  }
}

void
Misc::GetAllTags(vector<XString>* all)
{
  for(int index=0; taginfo[index].tagname; ++index)
  {
    XString tag = taginfo[index].tagname;
    all->push_back(tag);
  }
}

// ###########################  MISC  #################################

void 
Misc::RegWriteString(HKEY h_Class, CString s_Path, CString s_Key, CString s_Value)
{
	CRegKey i_Reg;
	i_Reg.Create  (h_Class, s_Path);
	i_Reg.SetValue(s_Value, s_Key);
  // closed in destructor of CRegKey
}

void 
Misc::RegWriteDword(HKEY h_Class, CString s_Path, CString s_Key, DWORD u32_Value)
{
	CRegKey i_Reg;
	i_Reg.Create  (h_Class,   s_Path);  
	i_Reg.SetValue(u32_Value, s_Key);
  // Key is closed in DTOR of CRegKey
}


// Replaces MIME encoded strings
// "%20" --> " "
// "%FC" --> "ü"
// "%25" --> "%"     etc....
CString 
Misc::DecodeMime(CString s_In)
{
	CString s_Out;
	TCHAR *pt_Out = (TCHAR*) s_Out.GetBuffer(s_In.GetLength());
	TCHAR *pt_In  = (TCHAR*) s_In. GetBuffer(0);

	int O=0;
	for (int I=0; I<s_In.GetLength(); I++)
	{
		TCHAR t_Chr = pt_In[I];

		if (t_Chr == _T('%')) 
		{
			UINT u32_Hex = 0;

			// convert hexadecimal string into UINT ("FA" --> 0xFA)
			for (UINT D=1; D<=2; D++)
			{
				UINT u32_Digit = (UINT) pt_In[I+D];

  					 if (u32_Digit >= (UINT)_T('0') && u32_Digit <= (UINT)_T('9')) u32_Digit -=  (UINT) _T('0');
				else if (u32_Digit >= (UINT)_T('A') && u32_Digit <= (UINT)_T('F')) u32_Digit -= ((UINT) _T('A') -10);
				else if (u32_Digit >= (UINT)_T('a') && u32_Digit <= (UINT)_T('f')) u32_Digit -= ((UINT) _T('a') -10);
				else break;

				u32_Hex = u32_Hex * 16 + u32_Digit;

				if (D == 2) // second hex digit
				{
					t_Chr = (TCHAR) u32_Hex;
					I += 2; // skip two hex digits
				}
			}
		}
		pt_Out[O++] = t_Chr;
	}
	s_Out.ReleaseBuffer(O);
	return s_Out;
}

// converts Unicode to ANSI using HTML Hex character encoding --> "&#xABCD;"
// If compiled with _UNICODE compiler setting, no encoding is required -> just copy string
// bs_Html is freed here at the end
CString 
Misc::EncodeHtml(CComBSTR bs_Html)
{
	CString s_Out;
	UINT u32_Len = SysStringLen(bs_Html);
	if (!u32_Len)
  {
    return _T("");
  }
	UINT u32_BufSize = u32_Len * 8 +1;
	LPTSTR t_Buf     = s_Out.GetBuffer(u32_BufSize); // for worst case

	#ifdef _UNICODE
		wcscpy(t_Buf, bs_Html);
	#else // _MBCS
		// EncodeHtml() modifies s32_Len AND u32_BufSize !!
		EncodeHtml(bs_Html, &u32_Len, t_Buf, &u32_BufSize);
		u32_Len = u32_BufSize; // count of chars copied to t_Buf
	#endif

	SysFreeString(bs_Html);
	s_Out.ReleaseBuffer(u32_Len);
	return s_Out;
}

// converts pu32_WcharToCopy WCHAR's in the Unicode string u16_In which are 
// outside the ASCII code range to ANSI s8_Out using HTML Hex character encoding --> "&#xABCD;"
// s8_Out must have the 8 fold size of u32_WcharToCopy to cover the worst case !!
// As s8_Out will be zero terminated, the minimum buffer size is 9 characters
// If the buffer is too small no conversion takes place
// e.g. The Greek letter Unicode 960 is converted to "&#x03C0;\0"
void 
Misc::EncodeHtml(const WCHAR *u16_In,     // IN=buffer with UNICODE Html code
 					       UINT  *pu32_WcharToCopy, // IN=Wchars to copy, OUT=Converted Wchars
 					       TCHAR  *s8_Out,           // IN=buffer to receive ANSI Html code
 					       UINT  *pu32_OutSize)     // IN=sizeof(s8_Out), OUT=Bytes copied to s8_Out
{
	// Minimum required buffer size is 9 characters !! (for "&#x03C0;\0")
	// In the worst case 9 MBCS chars are required to convert ONE Unicode character !!
	ASSERT(*pu32_OutSize > 8); 

	UINT Pos = 0;
	UINT i   = 0;
	for (i=0; i<*pu32_WcharToCopy && Pos+8<*pu32_OutSize; i++)
	{
		WCHAR Chr = u16_In[i];
		if (Chr == 0) // End of Wstring
			break;

		if (Chr <= _T('~')) // no conversion required
		{
			s8_Out[Pos] = (TCHAR) Chr;
			Pos ++;
		}
		else // encode HTML
		{
			_stprintf(s8_Out +Pos, _T("&#x%04X;"), Chr);
			Pos += 8;
		}
	}

	s8_Out[Pos] = 0; // terminate string

	*pu32_WcharToCopy = i;
	*pu32_OutSize     = Pos;
}

// removes a tag at the begin and end of a Html code
// RemoveTag("<DIV align=right><b>Text</b></DIV>", "Div")   will return "<b>Text</b>"
// See also cHtmlDomNode::Strip() and cHtmlDomNode::Remove()
CString 
Misc::RemoveTag(CString s_Html, CString s_Tag)
{
	s_Html.TrimLeft (_T(" \t\r\n"));
	s_Html.TrimRight(_T(" \t\r\n"));

	s_Tag.MakeUpper();
	CString s_Start = _T("<")  + s_Tag;
	CString s_End   = _T("</") + s_Tag + _T(">");

	if (s_Html.Left (s_Start.GetLength()).CompareNoCase(s_Start) != 0 ||
	  	s_Html.Right(s_End.  GetLength()).CompareNoCase(s_End)   != 0)
  {
    return s_Html;
  }
	UINT s32_Begin = s_Html.Find(_T(">"), s_Start.GetLength()) +1;
	UINT s32_Count = s_Html.GetLength() - s_End.GetLength() - s32_Begin;

	return s_Html.Mid(s32_Begin, s32_Count);
}

// add tag and optionally a class to a piece of HTML code
// s_html = "This is a text", s_tag = "span" and s_class "code" will return
// "<span class="code">This is a TEXT</span>""
CString 
Misc::AddTagClass(CString s_Html,CString s_tag,CString s_class)
{
  CString beginTag = CString(_T("<"))  + s_tag;
  CString endTag   = CString(_T("</")) + s_tag + _T(">");
  if(!s_class.IsEmpty())
  {
    beginTag += CString(_T(" class=\"")) + s_class + _T("\"");
  }
  beginTag += _T(">");
  CString newText = beginTag;
  newText += s_Html;
  newText += endTag;
  return newText;
}

// Removes the string s_Cut once from the beginning (b_End=FALSE) or end (b_End=TRUE) of s_In
// E.g. s_In="Test&nbsp;&nbsp;", s_Cut="&nbsp;", b_End=TRUE --> returns "Test&nbsp;"
CString 
Misc::CutString(CString s_In, CString s_Cut, BOOL b_End)
{
	int s32_InLen  = s_In. GetLength();
	int s32_CutLen = s_Cut.GetLength();
	
	if (s32_InLen < s32_CutLen || s32_InLen == 0 || s32_CutLen == 0)
  {
    return s_In;
  }
	if (b_End)
	{
		if (s_Cut.CompareNoCase(s_In.Right(s32_CutLen)) == 0)
    {
      return s_In.Mid(0, s32_InLen - s32_CutLen);
    }
	}
	else
	{
		if (s_Cut.CompareNoCase(s_In.Left(s32_CutLen)) == 0)
    {
      return s_In.Mid(s32_CutLen);
    }
	}

	return s_In;
}

// required for Visual Studio 6 and bugfix for Visual Studio 7 not converting VT_NULL correctly
// This function is ONLY used to convert strings like "TABLE" or "#FF5500" from CComBSTR to CString
// ATTENTION !!!
// For country dependant conversions of special characters (like öäüß or Greek, Russian) 
// use EncodeHtml() instead !!!!
CString 
Misc::VarToStr(CComVariant &v_Variant)
{
	CString s_Result;

	if (v_Variant.vt == VT_I4)
	{
		s_Result.Format(_T("%d"), v_Variant.intVal);
	}
	else if (v_Variant.vt == VT_BOOL)
	{
		s_Result.Format(_T("%d"), v_Variant.boolVal);
	}
	else if (v_Variant.vt == VT_BSTR) 
	{
		if (!v_Variant.bstrVal)
			return _T("");

		UINT u32_Len = SysStringLen(v_Variant.bstrVal);
		if (!u32_Len)
    {
      return _T("");
    }
		LPTSTR t_Buf = s_Result.GetBuffer(u32_Len);

		#ifdef _UNICODE // Unicode --> Unicode
			wcscpy(t_Buf, v_Variant.bstrVal);
		#else // _MBCS     Unicode --> Ansi
			WideCharToMultiByte(CP_ACP, 0, v_Variant.bstrVal, u32_Len, t_Buf, u32_Len, 0, 0);
		#endif
		
		s_Result.ReleaseBuffer(u32_Len);
	}

	return s_Result;
}

// Decode an internet color code (name or #RGB number)
// to their Red,Green,Blue components
void
Misc::DecodeColor(CString p_color,int& red,int& green,int& blue)
{
  // Default is black!!
  red   = 0;
  green = 0;
  blue  = 0;
  if(p_color.IsEmpty())
  {
    // No color defaults to black
    return;
  }
  if(p_color.GetAt(0) != _T('#'))
  {
    // Search for internet-safe color name
    int ind = 0;
    do
    {
    	if(p_color.CompareNoCase(internet_colors[ind].name) == 0)
      {
        p_color = internet_colors[ind].color;
        break;
      }
    } 
    while(internet_colors[++ind].name);
  }
  if(p_color.GetAt(0) == _T('#'))
  {
    if(p_color.GetLength() == 7)
    {
      // It's a RGB number #RRGGBB
      CString s_red = p_color.Mid(1,2);
      _stscanf(s_red,_T("%x"),&red);
      CString s_green = p_color.Mid(3,2);
      _stscanf(s_green,_T("%x"),&green);
      CString s_blue = p_color.Mid(5,2);
      _stscanf(s_blue,_T("%x"),&blue);
      return;
    }
    else if(p_color.GetLength() == 4)
    {
      // It's a RGB number #RGB
      CString s_red = p_color.Mid(1,1);
      s_red += s_red;
      _stscanf(s_red,_T("%x"),&red);
      CString s_green = p_color.Mid(2,1);
      s_green += s_green;
      _stscanf(s_green,_T("%x"),&green);
      CString s_blue = p_color.Mid(3,1);
      s_blue += s_blue;
      _stscanf(s_blue,_T("%x"),&blue);
      return;
    }
  }
  // Oeps. Still no color. Defaults to black
}

CString
Misc::GetInternetColor(CString& p_color)
{
  // Search for internet-safe color name
  int ind = 0;
  do
  {
    if(p_color.CompareNoCase(internet_colors[ind].name) == 0)
    {
      return internet_colors[ind].color;
    }
  }
  while(internet_colors[++ind].name);
  return _T("");
}

// Generic move the caret to a html element
// Can be used from the document-view or from
// within the property dialogs!
BOOL
Misc::MoveCaretToElement(CComPtr<IHTMLDocument2> doc2
                        ,CComPtr<IHTMLElement>   pElem
                        ,CARET_DIRECTION pDir
                        ,bool select_all)
{
  BOOL    result = FALSE;
  HRESULT hr     = S_FALSE;
  CComPtr<IDisplayServices> pServ;
  CComPtr<IDisplayPointer>  pDisp;
  CComPtr<IHTMLCaret>       pCaret;

  hr  = doc2->QueryInterface(IID_IDisplayServices,(void**)&pServ);
  if(SUCCEEDED(hr))
  {
    hr = pServ->CreateDisplayPointer(&pDisp);
    if(SUCCEEDED(hr))
    {
      DWORD onGlyph = 0;
      POINT pPoint;
      pPoint.x = 0;
      pPoint.y = 0;
      hr = pDisp->MoveToPoint(pPoint
                             ,COORD_SYSTEM_CONTENT
                             ,pElem
                             ,HT_OPT_AllowAfterEOL
                             ,&onGlyph);
      if(SUCCEEDED(hr))
      {
        hr = pServ->GetCaret(&pCaret);
        if(SUCCEEDED(hr))
        {
          // Now move the caret to the new display pointer
          // Scroll into view = TRUE
          // and going in the general direction (FOREWARD, BACKWARD)
          hr = pCaret->MoveCaretToPointer(pDisp,TRUE,pDir);
          if(SUCCEEDED(hr))
          {
            // Now select all the text in the element
            SetCurrentSelectionText(doc2,pElem,select_all);
            result = TRUE;
          }
        }
      }
    }
  }
  return result;
}

// Generic set the current selction in higlight
// Can be used from the document-view or from
// within the property dialogs!
bool
Misc::SetCurrentSelectionText(CComPtr<IHTMLDocument2> pDoc
                             ,CComPtr<IHTMLElement> pElem
                             ,bool select_all)
{
  bool theResult = false;
  CComPtr<IHTMLSelectionObject> pSelObj;

  HRESULT hr = pDoc->get_selection(&pSelObj);
  if (SUCCEEDED(hr)) 
  {
    CComBSTR type;
    hr = pSelObj->get_type(&type);
    // Alleen "None" en "Text" types mogen verwerkt worden
    // Het type "Control" moet worden tegengehouden.
    if(SUCCEEDED(hr) && wcsicmp(type,L"control"))
    {
      CComPtr<IHTMLTxtRange> pTxtRange;
      hr = pSelObj->createRange((IDispatch**)&pTxtRange);
      if (SUCCEEDED(hr)) 
      {
        hr = pTxtRange->moveToElementText(pElem);
        if(SUCCEEDED(hr))
        {
          if(select_all)
          {
            pTxtRange->select();
          }
          theResult = true;
        }
      }
    }
  }
  return theResult;
}

// Make a combined TAG/CLASS name for use in combo's and
// for the formatting results
CString
Misc::CombinedClassName(CString& p_tag
                       ,CString& p_class
                       ,bool&    p_userselect)
{
  CString combined;
  struct _taginfo* tags = taginfo;

  // Defaults not user selectable from the styles control
  p_userselect = false;

  if(p_tag.IsEmpty() && p_class.IsEmpty())
  {
    ASSERT(FALSE);
    return _T("");
  }
  if(p_tag.IsEmpty() && !p_class.IsEmpty())
  {
    // Selector = empty tag with classname
    combined = _T("S: ") + p_class;
    p_userselect = true;
    return combined;
  }
  // Find tag
  p_tag.MakeLower();
  InitMaps();
  if(tagMap.Lookup(p_tag,(void*&)tags))
  {
    if(tags->is_blockformat && p_class.IsEmpty())
    {
      combined = CString(_T("B: ")) + tags->description;
    }
    else
    {
      combined = CString(_T("C: ")) + tags->description;
      if(!p_class.IsEmpty())
      {
        combined += CString(_T(" (")) + p_class + _T(")");
      }
    }
    tags->is_used = true;
    p_userselect = tags->is_userselect;
    return combined;
  } 
  // Show as a TODO bug
  p_userselect = true;
  combined = _T("TODO: ") + p_tag;
  if(!p_class.IsEmpty())
  {
    combined += _T(" (") + p_class + _T(")");
  }
  return combined;
}

// Reconstruct from the combined type/tag/classname text
// the resulting tag/classname
void
Misc::ReconstructTagClass(CString& p_combined
                         ,CString& tagname
                         ,CString& classname)
{
  // See if we should process
  if(p_combined.IsEmpty())        return;
  if(p_combined.GetLength() < 4)  return;
  if(p_combined.Left(3) == _T("TOD")) return;

  classname = _T("");
  CString descript = p_combined.Mid(3);
  
  // Find classname
  int pos = descript.Find(_T("("));
  if(pos >= 0)
  {
    classname = descript.Mid(pos+1);
    classname = classname.Left(classname.GetLength()-1);
    descript  = descript.Left(pos-1);
  }
  struct _taginfo* tag;
  InitMaps();
  descript.MakeLower();
  if(descMap.Lookup(descript,(void*&)tag))
  {
    // Found our description
    tagname = tag->tagname;
  }
}

void
Misc::ExtractStylesList(CComPtr<IHTMLDocument2> doc2, CMFCToolBarComboBoxButton* pCombo)
{
  CComPtr<IHTMLStyleSheetsCollection> coll;
  long            count = 0;
  HRESULT         hr = S_FALSE;
  vector<CString> names;

  // Clear the combo first
  pCombo->RemoveAllItems();
  // Basic no-style as of index zero
  pCombo->AddItem(_T("<No style>"));

  // Reset the tagsinfo
  int index = 0;
  do
  {
  	taginfo[index].is_used = false;
  } 
  while(taginfo[++index].tagname);

  // Get the stylesheets from the document to cycle through them
  hr = doc2->get_styleSheets(&coll);
  if(!SUCCEEDED(hr)) return;

  hr = coll->get_length(&count);
  if(!SUCCEEDED(hr)) return;

  for(int ind=0;ind < count; ++ind)
  {
    CComVariant var;
    V_VT(&var) = VT_I4;
    V_I4(&var) = ind;
    CComVariant style;
    V_VT(&style) = VT_DISPATCH;
    hr = coll->item(&var,&style);
    if(!SUCCEEDED(hr)) continue;

    CComPtr<IHTMLStyleSheet> sheet;
    CComPtr<IHTMLStyleSheetRulesCollection> rules;
    sheet = (IHTMLStyleSheet *) V_DISPATCH(&style);
    hr = sheet->get_rules(&rules);
    if(!SUCCEEDED(hr)) continue;

    long numRules =0;
    hr = rules->get_length(&numRules);
    if(!SUCCEEDED(hr)) continue;

    IHTMLStyleSheetRule* rule;
    for(int num = 0; num < numRules; ++num)
    {
      hr = rules->item(num,&rule);
      if(!SUCCEEDED(hr)) continue;

      CComBSTR selector;
      rule->get_selectorText(&selector);

      CString tag,spanClass;
      CString styleName = CW2CT(selector);
      tag = styleName;
      int pos = styleName.Find(_T('.'));
      if(pos < 0)
      {
        pos = styleName.Find(_T('#'));
      }
      if(pos >= 0)
      {
        // Class name found
        tag = styleName.Left(pos);
        spanClass = styleName.Mid(pos+1);
      }
      bool userselect;
      CString display = CombinedClassName(tag,spanClass,userselect);
      if(userselect)
      {
        names.push_back(display);
      }
    }
  }
  // OK, Stylesheets ready, look for unused blockformats
  index = 0;
  do
  {
    if(taginfo[index].is_blockformat && !taginfo[index].is_used)
    {
      bool userselect;
      CString tag = taginfo[index].tagname;
      CString className;
      CString display = CombinedClassName(tag,className,userselect);
      if(userselect)
      {
        names.push_back(display);
      }
    }
  } 
  while(taginfo[++index].tagname);

  // Now sort all the names for the combo, box
  // CodeJock doesn't support CBS_SORT on these boxes!!
  std::sort(names.begin(),names.end());
  for(unsigned int ind = 0; ind < names.size(); ++ind)
  {
    CString display = names[ind];
    pCombo->AddItem(display);
  }
}

// Find out if a tag is a block-formatted tag
bool
Misc::IsBlockTag(CString tagname)
{
  struct _taginfo* tag;
  InitMaps();

  tagname.MakeLower();
  if(tagMap.Lookup(tagname,(void*&)tag))
  {
    return tag->is_blockformat;
  }
  return 0;
}

// Find out if a block tag can be user selectable
bool
Misc::IsUserSelectableTag(CString tagname)
{
  struct _taginfo* tag;
  InitMaps();

  tagname.MakeLower();
  if(tagMap.Lookup(tagname,(void*&)tag))
  {
    return tag->is_userselect;
  }
  return 0;
}

// Get a description from the tags map
// eg. "H1" returns "Heading 1"
CString
Misc::GetTagDescription(XString tagname)
{
  struct _taginfo* tag;
  InitMaps();

  tagname.MakeLower();
  if(tagMap.Lookup(tagname,(void*&)tag))
  {
    return tag->description;
  }
  return _T("");
}

// Get a description from the tags map
// eg. "H1" returns "Heading 1"
CString
Misc::GetTagDescription(CString tagname)
{
  struct _taginfo* tag;
  InitMaps();

  tagname.MakeLower();
  if(tagMap.Lookup(tagname,(void*&)tag))
  {
    return tag->description;
  }
  return _T("");
}

// Advanced hittest for floating objects
// Takes the x,y position from the TranslateMessage
// and loops through all the elements of a certain type
BOOL
Misc::GetElementByPositionAndTag(CComPtr<IHTMLDocument2> doc
                                ,CComPtr<IHTMLElement>&  pElem
                                ,int xHit
                                ,int yHit
                                ,CString tag)
{
  CComPtr<IHTMLElementCollection> coll;
  CComPtr<IHTMLElement> body;
  POINT         lPoint;
  HRESULT       hr = S_FALSE;
  long          bClientLeft,bClientTop;

  // Record hit point in client area
  lPoint.x = xHit;
  lPoint.y = yHit;

  // Get body for body offsets from client rect
  doc->get_body(&body);
  CComQIPtr<IHTMLElement2,&IID_IHTMLElement2> elem = body;
  bClientLeft = elem->get_clientLeft(&bClientLeft);
  bClientTop  = elem->get_clientTop(&bClientTop);

  // Iterate over all elements
  hr = doc->get_all(&coll);
  if(SUCCEEDED(hr))
  {
    long count = 0;
    hr = coll->get_length(&count);
    if(SUCCEEDED(hr))
    {
      for(int ind = 0; ind < count; ++ind)
      {
        CComPtr<IDispatch> disp;
        CComVariant num;
        V_VT(&num) = VT_I4;
        V_I4(&num) = ind;
        coll->item(num,num,&disp);
        CComQIPtr<IHTMLElement, &IID_IHTMLElement>  pElement  = disp;

        CComBSTR bTag;
        CString  sTag;
        pElement->get_tagName(&bTag);
        sTag = bTag;
        if(sTag.CompareNoCase(tag))
        {
          continue;
        }
        long ol,ow,ot,oh;
        pElement->get_offsetLeft(&ol);
        pElement->get_offsetWidth(&ow);
        pElement->get_offsetTop(&ot);
        pElement->get_offsetHeight(&oh);

        CRect hit;
        hit.top    = bClientTop  + ot;
        hit.left   = bClientLeft + ol;
        hit.bottom = hit.top  + oh;
        hit.right  = hit.left + ow;

        if(hit.PtInRect(lPoint))
        {
          pElem = pElement;
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

// Generic move caret to point
void
Misc::MoveCaretToPoint(CComPtr<IHTMLDocument2> doc,LPPOINT ppt)
{
  CComPtr<IDisplayServices> pServ;
  CComPtr<IDisplayPointer>  pDisp;
  CComPtr<IHTMLCaret>       pCaret;

  HRESULT hr  = doc->QueryInterface(IID_IDisplayServices,(void**)&pServ);
  if(SUCCEEDED(hr))
  {
    hr = pServ->GetCaret(&pCaret);
    if(SUCCEEDED(hr))
    {
      hr = pServ->CreateDisplayPointer(&pDisp);
      if(SUCCEEDED(hr))
      {
        DWORD onGlyph;
        pDisp->MoveToPoint(*ppt,COORD_SYSTEM_GLOBAL,NULL,0,&onGlyph);
        pCaret->MoveCaretToPointer(pDisp,TRUE,CARET_DIRECTION_INDETERMINATE);
      }
    }
  }
}

// Create a new element in the HEAD section (by tagid)
// and return the resulting appended element
CComPtr<IHTMLElement>
Misc::CreateHeadElement(CComPtr<IHTMLDocument2> doc,ELEMENT_TAG_ID tagID,bool p_first /*= false*/)
{
  CComPtr<IHTMLElementCollection> col;
  HRESULT hr = doc->get_all(&col);
  if(SUCCEEDED(hr))
  {
    CComBSTR tagname = L"head";
    CComVariant selector;
    V_VT(&selector) = VT_BSTR;
    V_BSTR(&selector) = tagname;
    CComPtr<IDispatch> tdisp;
    hr = col->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      CComVariant ask;
      V_VT(&ask) = VT_I4;
      V_I4(&ask) = 0;
      CComPtr<IDispatch> item = NULL;
      hr = tagscol->item(ask,ask,&item);
      if(SUCCEEDED(hr))
      {
        CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> head = item;
        CComPtr<IHTMLElement>     elem;
        CComPtr<IMarkupServices>  spMS;
        doc->QueryInterface(IID_IMarkupServices, (void**)&spMS);
        hr = spMS->CreateElement(tagID,NULL,&elem);
        if(SUCCEEDED(hr))
        {
          CComPtr<IHTMLDOMNode> newNode;
          CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> append = elem;
          if(p_first)
          {
            CComVariant first(VT_UNKNOWN);
            first.vt = VT_EMPTY;

            CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> fchild;
            head->get_firstChild(&fchild);
            if(SUCCEEDED(hr) && fchild.p)
            {
              first.vt = VT_UNKNOWN;
              first.punkVal = fchild;
            }
            hr = head->insertBefore(append,first,&newNode);
          }
          else
          {
            hr = head->appendChild(append,&newNode);
          }
          if(SUCCEEDED(hr))
          {
            return elem;
          }
        }
      }
    }
  }
  return NULL;
}

// Create a new element as a child under the parent element
// and return the resulting appended element
CComPtr<IHTMLElement>
Misc::CreateChildElement(CComPtr<IHTMLDocument2> p_doc
                        ,CComPtr<IHTMLElement>   p_parent
                        ,ELEMENT_TAG_ID          p_tagID)

{
  CComPtr<IMarkupServices> spMS;
  CComPtr<IHTMLElement>    elem;

  CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> parentDom = p_parent;
  p_doc->QueryInterface(IID_IMarkupServices, (void**)&spMS);
  HRESULT hr = spMS->CreateElement(p_tagID,NULL,&elem);
  if(SUCCEEDED(hr))
  {
    CComPtr<IHTMLDOMNode> newNode;
    CComQIPtr<IHTMLDOMNode,&IID_IHTMLDOMNode> append = elem;
    hr = parentDom->appendChild(append,&newNode);
    if(SUCCEEDED(hr))
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> newElem = newNode;
      return newElem;
    }
  }
  return NULL;
}


CComPtr<IHTMLElement> 
Misc::AreaToImg(CComPtr<IHTMLDocument2> doc
               ,CComPtr<IHTMLElement> pElement)
{
  CComBSTR tagName;
  pElement->get_tagName(&tagName);
  CString tag = CW2CT(tagName);
  if(tag.CompareNoCase(_T("img")) == 0)
  {
    // Already an image
    return pElement;
  }
  if(tag.CompareNoCase(_T("area")))
  {
    // Not an area. probably just create a new image
    return pElement;
  }
  CComPtr<IHTMLElement> mapElem;
  HRESULT hr = pElement->get_parentElement(&mapElem);
  if(SUCCEEDED(hr) && mapElem.p)
  {
    CComBSTR mapID;
    mapElem->get_id(&mapID);
    CString cmapId = CW2CT(mapID);
    cmapId.MakeLower();

    // Now get the first image with this mapid
    CComPtr<IHTMLElementCollection> imgColl;
    hr = doc->get_images(&imgColl);
    if(SUCCEEDED(hr) && imgColl.p)
    {
      long count = 0;
      imgColl->get_length(&count);
      for(int ind = 0; ind < count; ++ind)
      {
        CComVariant num;
        V_VT(&num) = VT_I4;
        V_I4(&num) = ind;
        CComPtr<IDispatch> disp;
        hr = imgColl->item(num,num,&disp);
        if(SUCCEEDED(hr))
        {
          CComBSTR imgMap;
          CComQIPtr<IHTMLImgElement,&IID_IHTMLImgElement> image = disp;
          image->get_useMap(&imgMap);
          CString cimgMap = CW2CT(imgMap);
          cimgMap.MakeLower();
          if(cimgMap.GetLength() > 0 && cimgMap.GetAt(0) == _T('#'))
          {
            cimgMap = cimgMap.Mid(1);
          }
          if(cmapId == cimgMap)
          {
            // This is the first image with a map id of our area
            CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp;
            return elem;
          }
        }
      }
    }
  }
  return pElement;
}

CString
Misc::MetaTag(CComPtr<IHTMLDocument2>& doc
              ,CString name
              ,CString* value/*=NULL*/
              ,bool p_httpEquiv/*=false*/)
{
  USES_CONVERSION;

  CComPtr<IHTMLElementCollection> col;
  HRESULT hr = doc->get_all(&col);
  if(SUCCEEDED(hr))
  {
    CComBSTR tagname = L"meta";
    CComVariant selector;
    V_VT(&selector) = VT_BSTR;
    V_BSTR(&selector) = tagname;
    CComPtr<IDispatch> tdisp;
    hr = col->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      long num = 0;
      hr = tagscol->get_length(&num);
      if(SUCCEEDED(hr) && num > 0)
      {
        // Loop through all the meta tags
        for(int metas = 0; metas < num; ++metas)
        {
          CComVariant ask;
          V_VT(&ask) = VT_I4;
          V_I4(&ask) = metas;
          CComPtr<IDispatch> item = NULL;
          hr = tagscol->item(ask,ask,&item);
          if(SUCCEEDED(hr))
          {
            CComQIPtr<IHTMLMetaElement,&IID_IHTMLMetaElement> meta = item;
            CComBSTR meta_name;
            CComBSTR meta_content;
            if(p_httpEquiv)
            {
              meta->get_httpEquiv(&meta_name);
            }
            else
            {
              meta->get_name(&meta_name);
            }
            meta->get_content(&meta_content);
            CString metaName = CW2CT(meta_name);
            CString content = CW2CT(meta_content);
            if(!name.IsEmpty() && (metaName.CompareNoCase(name) == 0))
            {
              if(value)
              {
                meta_content = *value;
                meta->put_content(meta_content);
              }
              return content;
            }
          }
        }
      }
    }
  }
  // NOTHING TO DO
  if(!value)
  {
    return _T("");
  }
  // META TAG NOT FOUND. CREATE IT
  // HttpEquivalent tags must always be created at the beginning of the head section
  CComPtr<IHTMLElement> elem = Misc::CreateHeadElement(doc,TAGID_META,p_httpEquiv);
  CComQIPtr<IHTMLMetaElement,&IID_IHTMLMetaElement> meta = elem;
  CComBSTR bName = name;
  CComBSTR bValue = *value;
  if(p_httpEquiv)
  {
    meta->put_httpEquiv(bName);
  }
  else
  {
    meta->put_name(bName);
  }
  meta->put_content(bValue);
  return *value;
}

CString
Misc::BaseTag(CComPtr<IHTMLDocument2>& doc
             ,CString uri
             ,CString target)
{
  CComPtr<IHTMLElementCollection> col;
  HRESULT hr = doc->get_all(&col);
  if(SUCCEEDED(hr))
  {
    CComBSTR tagname = L"base";
    CComVariant selector;
    V_VT(&selector) = VT_BSTR;
    V_BSTR(&selector) = tagname;
    CComPtr<IDispatch> tdisp;
    hr = col->tags(selector,&tdisp);
    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> tagscol = tdisp;
    if(SUCCEEDED(hr))
    {
      long num = 0;
      hr = tagscol->get_length(&num);
      if(SUCCEEDED(hr) && num > 0)
      {
        // Loop through all the meta tags
        for(int bases = 0; bases < num; ++bases)
        {
          CComVariant ask;
          V_VT(&ask) = VT_I4;
          V_I4(&ask) = bases;
          CComPtr<IDispatch> item = NULL;
          hr = tagscol->item(ask,ask,&item);
          if(SUCCEEDED(hr))
          {
            CComQIPtr<IHTMLBaseElement,&IID_IHTMLBaseElement> base = item;
            CComBSTR base_uri;
            CComBSTR base_target;
            base->get_href(&base_uri);
            base->get_target(&base_target);
            CString curi    = CW2CT(base_uri);
            CString ctarget = CW2CT(base_target);
            if(!target.IsEmpty() && (target.CompareNoCase(ctarget) == 0))
            {
              return curi;
            }
            if(target.IsEmpty())
            {
              return curi;
            }
          }
        }
      }
    }
  }
  // NOTHING TO DO
  if(uri.IsEmpty())
  {
    return _T("");
  }
  // BASE TAG NOT FOUND. CREATE IT
  CComPtr<IHTMLElement> elem = Misc::CreateHeadElement(doc,TAGID_BASE);
  CComQIPtr<IHTMLBaseElement,&IID_IHTMLBaseElement> base = elem;
  CComBSTR bUri = uri;
  base->put_href(bUri);
  if(!target.IsEmpty())
  {
    CComBSTR bTarget = target;
    base->put_target(bTarget);
  }
  return uri;
}

long
Misc::GetUniqueID(CComPtr<IHTMLDocument2>& doc,CString tagname)
{
  CComPtr<IHTMLElementCollection> coll;
  long highest = 1;
  USES_CONVERSION;

  HRESULT hr = doc->get_all(&coll);
  if(SUCCEEDED(hr))
  {
    VARIANT tagName;
    tagName.bstrVal = tagname.AllocSysString();
    tagName.vt = VT_BSTR;
    CComPtr<IDispatch> disp;
    hr = coll->tags(tagName,&disp);
    ::VariantClear(&tagName);

    CComQIPtr<IHTMLElementCollection,&IID_IHTMLElementCollection> coll2 = disp;
    if(SUCCEEDED(hr) && coll2.p)
    {
      long count = 0;
      coll2->get_length(&count);
      for(long ind = 0;ind < count; ++ind)
      {
        VARIANT number;
        V_VT(&number) = VT_I4;
        V_I4(&number) = ind;
        CComPtr<IDispatch> disp2;
        hr = coll2->item(number,number,&disp2);
        CComQIPtr<IHTMLElement,&IID_IHTMLElement> elem = disp2;
        if(SUCCEEDED(hr) && elem.p)
        {
          // First element of the collection
          CComBSTR bID;
          elem->get_id(&bID);
          CString cID = CW2CT(bID);
          for(int num=0;num < cID.GetLength(); ++num)
          {
            if(_istdigit(cID.GetAt(num)))
            {
              long number2 = _ttoi(&((cID.GetString())[num]));
              if(number2 >= highest)
              {
                highest = number2 + 1;
              }
            }
          }
        }
      }
    }
  }
  return highest;
}

CString  
Misc::CreateNewMap(CComPtr<IHTMLDocument2>& doc)
{
  CString cMapID;
  cMapID.Format(_T("map%d"),GetUniqueID(doc,_T("map")));
  CString mapping = CString(_T("<MAP id=")) + cMapID + _T("></MAP>");

  CComPtr<IHTMLElement> body;
  HRESULT hr = doc->get_body(&body);
  if(SUCCEEDED(hr))
  {
     CComBSTR bWhere = L"afterBegin";
     CComBSTR bMapping = CT2CW(mapping);
     hr = body->insertAdjacentHTML(bWhere,bMapping);
  }
  if(SUCCEEDED(hr))
  {
    return cMapID;
  }
  return _T("");
}

CComPtr<IHTMLElement>
Misc::GetByID(CComPtr<IHTMLDocument2>& doc,CString ident)
{
  CComPtr<IHTMLElementCollection> coll;
  HRESULT hr = doc->get_all(&coll);
  if(SUCCEEDED(hr))
  {
    CComVariant identity(ident);
    CComVariant number((long)0);
    CComPtr<IDispatch> disp;
    hr = coll->item(identity,number,&disp);
    if(SUCCEEDED(hr) && disp.p)
    {
      CComQIPtr<IHTMLElement,&IID_IHTMLElement> pElement = disp;
      return pElement;
    }
  }
  return NULL;
}

bool
Misc::CreateNewArea(CComPtr<IHTMLDocument2>& doc
                   ,CString mapID
                   ,CString areaID
                   ,CString shape)
{
  CComPtr<IHTMLElement> mapElem = GetByID(doc,mapID);
  if(mapElem.p)
  {
    CString newArea;
    newArea.Format(_T("<AREA id=%s shape=%s>"),areaID.GetString(),shape.GetString());
    CComBSTR bWhere = L"beforeEnd";
    CComBSTR bArea  = CT2CW(newArea);
    HRESULT hr = mapElem->insertAdjacentHTML(bWhere,bArea);
    if(SUCCEEDED(hr))
    {
      return true;
    }
  }
  return false;
}

// Generic strip protocol from URL to form OS filenames
// "file:///c|/Program%20Files/Program%23name/file%25name.exe" =>
// "c:\Program Files\Program#name\file%name.exe"
CString
Misc::StripFileProtocol(CString fileref)
{
  if(fileref.GetLength() > 8)
  {
    if(fileref.Left(8).CompareNoCase(_T("file:///")) == 0)
    {
      fileref = fileref.Mid(8);
    }
  }
  fileref.Replace(_T('/'),_T('\\'));
  fileref.Replace(_T('|'),_T(':'));
  fileref.Replace(_T("%20"),_T(" "));
  fileref.Replace(_T("%23"),_T("#"));
  fileref.Replace(_T("%25"),_T("%"));
  return fileref;
}

// Generic strip popup
// Used before 'OpenTypedDocumentFile'
CString
Misc::StripPopup(CString fileref)
{
  CString file = fileref;
  if(file.GetLength() > 11)
  {
    if(file.Left(11).CompareNoCase(_T("javascript:")) == 0)
    {
      file = file.Mid(11);
    }
  }
  if(file.GetLength() > 11)
  {
    if(file.Left(11).CompareNoCase(_T("BSSCPopup('")) == 0)
    {
      file = file.Mid(11);
      file = file.TrimRight(_T("');"));
    }
  }
  if(file.GetLength() > 14)
  {
    if(file.Left(14).CompareNoCase(_T("ADHShowPopup('")) == 0)
    {
      file = file.Mid(14);
      int pos = file.ReverseFind(_T('\''));
      if(pos >= 0)
      {
        file = file.Left(pos);
      }
    }
  }
  if(file.Find(_T("void(0")) == 0)
  {
    // Help&Manual does this
    file.Empty();
  }
  return file;
}

bool
Misc::SplitMidpageAnchor(const CString& p_fileref,CString& p_file,CString& p_anchor)
{
  bool found = false;
  int pos = p_fileref.Find(_T('#'));
  if(pos >= 0)
  {
    found    = true;
    p_anchor = p_fileref.Mid(pos + 1);
    p_file   = p_fileref.Left(pos);
  }
  else
  {
    p_file = p_fileref;
  }
  return found;
}

// Generic find case-insensitive
int
Misc::FindNoCase(CString line,CString part,int pos/*=0*/)
{
  line.MakeLower();
  part.MakeLower();
  return line.Find(part,pos);
}

// Generic find reverse from pos
int
Misc::FindReverse(CString line,CString part,int pos /*= -1*/)
{
  if(pos >= 0 && pos < line.GetLength())
  {
    line = line.Left(pos);
  }
  line.MakeReverse();
  part.MakeReverse();
  int found = line.Find(part,0);
  if(found >= 0)
  {
    return (line.GetLength() - found);
  }
  return -1;
}

int 
Misc::FindReverseNoCase(CString line,CString part,int pos/*=-1*/)
{
  line.MakeLower();
  part.MakeLower();
  return FindReverse(line,part,pos);
}


// Geef de 'p_Num de' substring binnen deze string gescheiden door p_Delim chars.
// nummer p_Num begint bij 1 te tellen voor eerste string.
// for instance:
// Substring("hallo@dag",'@',1) -> hallo
// Substring("hallo@dag",'@',2) -> dag
// Substring("hallo@dag",'@',3) -> ""
// Substring("hallodag",'@',1) -> "hallodag"
// Substring("hallodag",'@',2) -> ""
// Substring("hallo@@dag",'@',2) -> ""
// Substring("@hallo@dag",'@',1) -> ""
// Substring("@hallo@dag",'@',2) -> "hallo"
CString 
Misc::SubStr(CString& p_string,TCHAR p_Delim,int p_Num)
{
  int nPos2;
  int nPos1 = 0;
  int nNum  = 0;
  do
  {
    nPos2 = p_string.Find(p_Delim,nPos1);
    nNum ++;
    if(nNum==p_Num)
    {
      return (nPos2 >= 0) ? 
              p_string.Mid(nPos1,nPos2 - nPos1)   :
              p_string.Mid(nPos1) ;
    }
    nPos1 = nPos2 + 1 ;
  }
  while(nPos2 >= 0);
  return _T("");
}

CString
Misc::GetParamFromCommandline(CString& commandline
                             ,CString  p_commando
                             ,TCHAR     p_delim /* = '=' */)
{
  int nCmdPos = commandline.Find(p_commando);
  if(nCmdPos < 0)
  {
    nCmdPos = FindNoCase(commandline,p_commando);
  }
  if(nCmdPos<0)
  {
    return _T("");
  }
  int nIsPos = commandline.Find(p_delim,nCmdPos);
  if(nIsPos<0)
  {
    return _T("");
  }
  CString s = commandline.Mid(nIsPos + 1);
  s.TrimLeft();
  if(s.Find(_T('"'))==0)        // mag ook tussen kwootjes
  {
    return Misc::SubStr(s,_T('"'),2);
  }
  return Misc::SubStr(s,_T(' '),1);
}

int
Misc::CountNL(CString& sCurSentence,int pos)
{
  int numNL = 0;
  for(int ind=0; ind < pos; ++ind)
  {
    if(sCurSentence.GetAt(ind) == _T('\n'))
    {
      ++numNL;
    }
  }
  return numNL;
}

CString 
Misc::BasenamePart(CString fullpath)
{
  TCHAR drive [_MAX_DRIVE + 1];
  TCHAR direct[_MAX_DIR   + 1];
  TCHAR fname [_MAX_FNAME + 1];
  TCHAR extens[_MAX_EXT   + 1];

  fullpath = StripFileProtocol(fullpath);
  _tsplitpath(fullpath.GetString(),drive,direct,fname,extens);
  CString filename(fname);
  return filename;
}

CString
Misc::FilenamePart(CString fullpath)
{
  TCHAR drive [_MAX_DRIVE + 1];
  TCHAR direct[_MAX_DIR   + 1];
  TCHAR fname [_MAX_FNAME + 1];
  TCHAR extens[_MAX_EXT   + 1];
 
  fullpath = StripFileProtocol(fullpath);
  _tsplitpath(fullpath.GetString(),drive,direct,fname,extens);
  CString filename = CString(fname) + CString(extens);
  return filename;
}

CString
Misc::ExtensionPart(CString fullpath)
{
  TCHAR drive [_MAX_DRIVE + 1];
  TCHAR direct[_MAX_DIR   + 1];
  TCHAR fname [_MAX_FNAME + 1];
  TCHAR extens[_MAX_EXT   + 1];

  fullpath = StripFileProtocol(fullpath);
  _tsplitpath(fullpath.GetString(),drive,direct,fname,extens);
  return CString(extens);
}

CString
Misc::DirectoryPart(CString fullpath)
{
  TCHAR drive [_MAX_DRIVE + 1];
  TCHAR direct[_MAX_DIR   + 1];
  TCHAR fname [_MAX_FNAME + 1];
  TCHAR extens[_MAX_EXT   + 1];

  fullpath = StripFileProtocol(fullpath);
  _tsplitpath(fullpath.GetString(),drive,direct,fname,extens);
  CString directory = CString(drive) + CString(direct);
  return directory;
}

CString
Misc::RemoveBasePart(CString base,CString fullpath)
{
  fullpath = StripFileProtocol(fullpath);
  fullpath.Replace(_T('/'),_T('\\'));
  if(FindNoCase(fullpath,base,0) == 0)
  {
    return fullpath.Mid(base.GetLength());
  }
  return fullpath;
}

//////////////////////////////////////////////////////////////////////////////
//
// ABOUT ATTRIBUTE VALUES AND DISPLAY NAMES
//
//////////////////////////////////////////////////////////////////////////////

CString
Misc::GetAttributeDisplayname(CString attribute,CString value)
{
  AttributeInfo* attrib = attributes;
  bool    foundPoint = false;
  CString resultDisplay;
  while(attrib->name)
  {
    if(_tcsicmp(attrib->name,attribute)==0)
    {
      foundPoint = true;
      if(_tcsicmp(attrib->value,value)==0)
      {
        resultDisplay = attrib->display;
        break;
      }
    }
    else
    {
      if(foundPoint) break;
    }
    ++attrib;
  }
  return resultDisplay;
}

CString
Misc::GetAttributeValue(CString attribute,CString display)
{
  AttributeInfo* attrib = attributes;
  bool    foundPoint = false;
  CString resultValue;
  while(attrib->name)
  {
    if(_tcsicmp(attrib->name,attribute)==0)
    {
      foundPoint = true;
      if(_tcsicmp(attrib->display,display)==0)
      {
        resultValue = attrib->value;
        break;
      }
    }
    else
    {
      if(foundPoint) break;
    }
    ++attrib;
  }
  return resultValue;
}

void
Misc::GetAllAttributeDisplaynames(CString attribute,vector<XString>* all)
{
  AttributeInfo* attrib = attributes;
  bool foundPoint = false;

  while(attrib->name)
  {
    if(_tcsicmp(attrib->name,attribute)==0)
    {
      foundPoint = true;
      XString display = attrib->display;
      if(find(all->begin(),all->end(),display) == all->end())
      {
        all->push_back(display);
      }
    }
    else
    {
      if(foundPoint) break;
    }
    ++attrib;
  }
}

// Makes a relative pathname from an absolute one
// Absolute: "file:///C:/aaa/bbb/ccc/ddd/eee/file.ext"
// Base    : "file:///C:/aaa/bbb/ccc/rrr/qqq/"
// output
// Relative: "../../ddd/eee/file.ext"
bool
Misc::MakeRelativePathname(CString& p_base
                          ,CString& p_absolute
                          ,CString& p_relative)
{
  p_relative       = _T("");
  CString base     = StripFileProtocol(p_base);
  CString absolute = StripFileProtocol(p_absolute);

  // Special case: no filename
  if(absolute.IsEmpty())
  {
    return true;
  }
  // Special case: base is empty. Cannot make it relative
  // This is a programming error
  if(base.IsEmpty())
  {
    return false;
  }
  // Make all directory seperators the same
  base.Replace(_T('\\'),_T('/'));
  absolute.Replace(_T('\\'),_T('/'));

  // Special case: only a filename, make it relative to the 'this' directory
  if(absolute.Find(_T('/')) < 0)
  {
    // Cannot use this in *.HHP projects!
    // p_relative = CString("./") + absolute;
    p_relative = absolute;
    return true;
  }
  // Special case: already a relative path
  if(absolute.GetAt(0) == _T('.'))
  {
    p_relative = absolute;
    return true;
  }
  // Find the path-parts that are common to both names
  // We can eliminate these parts
  bool notCompatible = true;
  int  pos = absolute.Find(_T('/'));
  while(pos >= 0)
  {
    CString left_base = base    .Left(pos);
    CString left_abso = absolute.Left(pos);
    if(left_base.CompareNoCase(left_abso))
    {
      // Stop here. Pathnames are different from here.
      break;
    }
    // Eliminate this part
    base     = base    .Mid(pos + 1);
    absolute = absolute.Mid(pos + 1);
    // Did at least one elimination
    notCompatible = false;
    // Find next position
    pos = absolute.Find(_T('/'));
  }
  if(notCompatible)
  {
    // Pathnames are not compatible. i.e. are on another filesystem
    // or on another protocol or ....
    return false;
  }
  // Find the path-parts that are different
  // We can substitute these with "../" parts.
  // This is what we have left:
  // Absolute: "ddd/eee/file.ext"
  // Base    : "rrr/qqq/"
  pos = base.Find(_T('/'));
  while(pos >= 0)
  {
    absolute = CString(_T("../")) + absolute;
    base = base.Mid(pos+1);
    pos  = base.Find(_T('/'));
  }
  p_relative = absolute;

  // Relative path is complete.
  // Now warn for files outside the project
  if(p_relative.GetLength() > 1)
  {
    if(p_relative.Left(2) == _T(".."))
    {
      CString mess = _T("The file [") + p_relative + _T("] does not exist inside your project directory structure!\n")
                     _T("This can be a problem for the compilation, style sheets, images, links etc.");
      theApp.MessageBox(mess,_T("Project integrity"),MB_OK|MB_ICONASTERISK);
    }
  }
  return true;
}

// Reduce file path name for RE-BASE of directories
// IN:  C:\direct1\direct2\direct3\..\..\direct4 
// OUT: C:\direct1\direct4
CString
Misc::ReduceDirectoryPath(CString& path)
{
  TCHAR buffer[_MAX_PATH+1];
  _tcsnccpy(buffer,path.GetString(),_MAX_PATH);
  bool foundReduction = true;

  while(foundReduction)
  {
    // Drop out if we find nothing;
    foundReduction = false;

    TCHAR* pnt1 = buffer;
    TCHAR* pnt2 = pnt1;
    TCHAR* pnt3 = pnt1;

    while(*pnt1 && *pnt1!=_T('\\') && *pnt1!=_T('/')) ++pnt1;
    if(!*pnt1++)
    {
      // Not one directory seperator
      return path;
    }
    pnt3 = pnt1;
    while(*pnt1 && *pnt1!=_T('\\') && *pnt1!=_T('/')) ++pnt1;
    if(!*pnt1++)
    {
      // Not a second directory seperator
      return path;
    }
    pnt2 = pnt1;
    while(*pnt1 && *pnt1!=_T('\\') && *pnt1!=_T('/')) ++pnt1;
    while(*pnt1)
    {
      ++pnt1;
      // IN:  C:\direct1\direct2\direct3\..\..\direct4\
      //         |       |       |
      //      pnt3    pnt2    pnt1
      if(_tcsnccmp(pnt2,_T("..\\"),3)==0 || _tcsnccmp(pnt2,_T("../"),3)==0)
      {
        // Space between pnt2 and pnt1 = \..\
        // IN:  C:\direct1\direct2\direct3\..\..\direct4\
        //                         |       |  |
        //                      pnt3    pnt2  pnt1

        // REDUCTION
        _tcscpy(pnt3,pnt1);

        // At least one more loop
        foundReduction = true;
        // Return to the top!!
        break;
      }
      // Next level of directories
      pnt3 = pnt2;
      pnt2 = pnt1;
      while(*pnt1 && *pnt1!=_T('\\') && *pnt1!=_T('/')) ++pnt1;
    }
  }
  return CString(buffer);
}


// Special optimized funtion to resolve %5C -> '\' in pathnames
int
Misc::ResolveSpecialChars(CString& value)
{
  int total = 0;

  int pos = value.Find(_T('%'));
  while (pos >= 0)
  {
    ++total;
    int num = 0;
    CString hexstring = value.Mid(pos+1,2);
    hexstring.MakeUpper();
    if(_istdigit(hexstring.GetAt(0)))
    {
      num = hexstring.GetAt(0) - _T('0');
    }
    else
    { 
      num = hexstring.GetAt(0) - _T('A') + 10;
    }
    num *= 16;
    if(_istdigit(hexstring.GetAt(1)))
    {
      num += hexstring.GetAt(1) - _T('0');
    }
    else
    {
      num += hexstring.GetAt(1) - _T('A') + 10;
    }
    value.SetAt(pos,(TCHAR)num);
    value = value.Left(pos+1) + value.Mid(pos + 3);
    pos = value.Find(_T('%'));
  }
  return total;
}

////////////////////////////////////////////////////////////////////////////////
//
// EVENTS
//
////////////////////////////////////////////////////////////////////////////////

int
Misc::GetAllEvents(vector<CString>& all,bool standard,UINT tags)
{
  int number = 0;
  TagEvents* pnt = tagEvents;
  
  all.clear();
  while(pnt->tags)
  {
    if(pnt->standard || !standard)
    {
      if(!tags || (tags & pnt->tags))
      {
        std::vector<CString>::iterator it = find(all.begin(),all.end(),pnt->event);
        if(it == all.end())
        {
          // Not previously added
          all.push_back(pnt->event);
        }
      }
    }
    ++number;
    ++pnt;
  }
  return number;
}

CString
Misc::GetEventDescription(CString event,UINT tags)
{
  TagEvents* pnt = tagEvents;
  while(pnt->tags)
  {
    if(pnt->event.CompareNoCase(event) == 0)
    {
      if(!tags || (tags & pnt->tags))
      {
        return pnt->description;
      }
    }
    ++pnt;
  }
  return _T("");
}


// Lookup a diacrite letter from input and translate it to HTML equivalent
// e.q. ALT-172 (¼) => "&frac14;"
CString
Misc::GetDiacriteHtml(uchar letter)
{
  Diacrites* diac = DiacriteLetters;
  while(diac->letter)
  {
    if(diac->letter == letter)
    {
      return diac->htmlText;
    }
    ++diac;
  }
  return _T("");
}

void
Misc::ParseNumber(CString& def,unsigned long& number)
{
  // Reset number
  number = 0;

  def.TrimLeft();
  int cpos = def.Find(_T(','));
  CString num = def;
  if(cpos >= 0)
  {
    num = def.Left(cpos);
  }
  int Xpos = num.Find(_T('X'));
  int xpos = num.Find(_T('x'));
  xpos = (Xpos >= 0) ? Xpos : xpos;
  if(xpos >= 0)
  {
    number = 0;
    num = num.Mid(xpos+1);
    while(num.GetLength())
    {
      number *= 16;
      TCHAR c = num.GetAt(0);
      if(_istdigit(c))
      {
        number += (int)(c - _T('0'));
      }
      else
      {
        c = _totupper(c);
        number += (int)(c - _T('A') + 10);
      }
      num = num.Mid(1);
    }
  }
  else
  {
    // Simple number
    number = _ttoi(def.Left(cpos));
  }
  // Parse on (WindowDefinition)
  if(cpos >= 0)
  {
    def = def.Mid(cpos + 1);
  }
}

// "Abc"         -> Normale string
// "124"         -> Normaal nummer
// "4343,123"    -> Nummer (Nederlandse notatie)
// "512.23"      -> Nummer (Engelse notatie)
// " 123"        -> String, want begint met spatie
// "1234 AB"     -> String (want spaties en letters)
// "+123.334E+03"-> Nummer (internationale IEEE notatie)
// " -12"        -> String, Want begint met spatie
// "-123.22 "    -> String, Want eindigt op spatie
// "0X0123"      -> Hex nummer (begint met kleine 'x', gevolgd door 0-9 en A-F)
// "0x0123af"    -> Hex nummer (begint met grote  'X', gevolgd door 0-9 en A-F)
// "0x0123a "    -> String, Want eindigt op spatie
// "0x0123fka"   -> String, want bevat letters anders dan A-F)
// "076"         -> Octaal nummer (begint met 0)
// "-0634"       -> Octaal nummer (na - teken een 0)
// "089"         -> String, want octaal nummer loopt tot 7.
// "076 "        -> String, want bevat spaties
bool
Misc::IsANumber(CString& def,long& number,double& broken)
{
  bool negative = false;
  int  index    = 0;
  TCHAR c;
  // Reset number
  number = 0;
  broken = 0.0;
  
  if(def.IsEmpty())
  {
    // Empty string is not a number
    return false;
  }
  // Parse sign of number
  c = def.GetAt(0);
  if(c == _T('+'))
  {
    // Legaal positief getal
    c = def.GetAt(++index);
  }
  else if(c == _T('-'))
  {
    // Legaal negatief getal
    negative = true;
    c = def.GetAt(++index);
  }
  // Scan for octal or hexadecimal number
  if(def.GetLength() > (index + 1) && def.GetAt(index) == _T('0'))
  {
    bool xpos = (_totupper(def.GetAt(index + 1)) == _T('X'));
    if(xpos)
    {
      // HEXADECIMAL NUMBER
      index += 2;
      c = def.GetAt(index);
      // Possibly a x-digit number
      while(def.GetLength() > index)
      {
        number *= 16;
        if(_istdigit(c))
        {
          number += (int)(c - _T('0'));
        }
        else
        {
          c = _totupper(c);
          if(c >= _T('A') && c <= _T('F'))
          {
            number += (int) (c - _T('A') + 10);
          }
          else
          {
            // Not a hex number -> A String!!
            return false;
          }
        }
        c = def.GetAt(++index);
      }
    }
    else
    {
      // OCTAAL NUMMER
      ++index;
      c = def.GetAt(index);
      while(def.GetLength() > index)
      {
        if(c >= _T('0') && c <= _T('7'))
        {
          number *= 8;
          number += (c - _T('0'));
        }
        else
        {
          break;
        }
        c = def.GetAt(++index);
      }
    }
    if(negative)
    {
      number = (0 - number);
    }
    // Geheel en al kunnen parsen?
    return (def.GetLength() == index);
  }
  // Stuk voor decimale punt,komma
  while(def.GetLength() > index)
  {
    if(!_istdigit(c))
    {
      break;
    }
    number *= 10;
    number += (c - _T('0'));
    c = def.GetAt(++index);
  }
  // Stuk na decimale punt:komma
  if((def.GetLength() > index) && (c == _T('.') || c == _T(',')))
  {
    c = def.GetAt(++index);
    broken = (double) number;
    double factor = 1;
    double part   = 0;
    while(def.GetLength() > index)
    {
      if(!_istdigit(c))
      {
        break;
      }
      factor *= 10;
      part   *= 10;
      part   += (c - _T('0'));
      c = def.GetAt(++index);
    }
    broken += (part / factor);
  }
  // IEEE Exponentional notation
  if(c == _T('E') || c == _T('e'))
  {
    int exp = 0;
    c = def.GetAt(++index);
    bool negexp = false;
    if(c == _T('+'))
    {
      // Legaal positief getal
      c = def.GetAt(++index);
    }
    else if(c == _T('-'))
    {
      // Legaal negatief getal
      negexp = true;
      c = def.GetAt(++index);
    }
    while(def.GetLength() > index)
    {
      if(!_istdigit(c))
      {
        break;
      }
      exp *= 10;
      exp += (c - _T('0'));
      c = def.GetAt(++index);
    }
    if(negexp)
    {
      exp = (0 - exp);
    }
    double factor = pow(10.0,exp);  
    broken *= factor;
  }
  // Verwerk negatieve getallen.
  if(negative)
  {
    number = (0   - number);
    broken = (0.0 - broken);
  }
  // Geheel en al kunnen parsen?
  return (def.GetLength() == index);
}


// Converting pointsize of HTML to the fontsize of 
// normal text processors and the OS
int
Misc::PointSizeToFontSize(int pointsize)
{
  switch (pointsize)
  {
    // Pointsizes
  case 1: return 8;
  case 3: return 12;
  case 4: return 14;
  case 5: return 18;
  case 6: return 24;
  case 7: return 36;
  case 2: // Fall through
  default:return 10;
  }
}

// Converting the fontsize of the OS and text processors
// tot he pointsize of the HTML standard
int
Misc::FontSizeToPointSize(int fontsize)
{
  switch (fontsize)
  {
  case 8:  return 1;
  case 12: return 3;
  case 14: return 4;
  case 18: return 5;
  case 24: return 6;
  case 36: return 7;
  case 10: // Fall through
  default: return 2;
  }
}

//////////////////////////////////////////////////////////////////////////
//
// TOC HCC HHK PRJ Files parsing
//
//////////////////////////////////////////////////////////////////////////

const  TCHAR*    TOKEN_START = _T("<\"");
const  TCHAR*    TOKEN_SEPERATOR = _T(">= \"\'");
static CString  lastWord;
static TOCToken lastToken = PF_NOTOKEN;
static XString  currentstring;
static TCHAR    ungetchbuffer[2] = { 0,0 };
static bool     normalchar = true;

// Call before use
void
Misc::ResetTokenizer()
{
  lastWord.Empty();
  lastToken = PF_NOTOKEN;
  currentstring.Empty();
  ungetchbuffer[0] = 0;
}

bool
Misc::SkipToken(WinFile& file, TOCToken expect, long& linenumber)
{
  CString word;
  TOCToken token = GetToken(file, word, linenumber);
  if (token == expect)
  {
    return true;
  }
  PushToken(word, token);
  return false;
}

void
Misc::PushToken(CString& word, TOCToken token)
{
  lastWord = word;
  lastToken = token;
}

int
Misc::GetNextTokenChar(WinFile& file)
{
  int c = 0;
  normalchar = true;

  if (ungetchbuffer[0])
  {
    c = ungetchbuffer[0];
    ungetchbuffer[0] = 0;
    return c;
  }
  if (currentstring.GetLength() == 0)
  {
    if (!file.Read(currentstring))
    {
      return _TEOF;
    }
  }
  c = currentstring.GetAt(0);
  currentstring = currentstring.Mid(1);

  if(c == _T('&'))
  {
    if(currentstring.Find(_T("amp;")) == 0)
    {
      currentstring = currentstring.Mid(4);
      return _T('&');
    }
    if(currentstring.Find(_T("lt;")) == 0)
    {
      currentstring = currentstring.Mid(3);
      return _T('<');
    }
    if(currentstring.Find(_T("gt;")) == 0)
    {
      currentstring = currentstring.Mid(3);
      return _T('>');
    }
    if(currentstring.Find(_T("apos;")) == 0)
    {
      currentstring = currentstring.Mid(5);
      return _T('\'');
    }
    if(currentstring.Find(_T("quot;")) == 0)
    {
      currentstring = currentstring.Mid(5);
      normalchar = false;
      return _T('\"');
    }
  }
  return c;
}

void
Misc::UngetNextTokenChar(int c)
{
  ungetchbuffer[0] = (TCHAR)c;
}

TOCToken
Misc::GetToken(WinFile& file,CString& word,long& linenumber)
{
  int c = 0;
  int index = 0;
  bool dostring = false;
  TCHAR buffer[MAX_PATH+1];

  // Last token stacked?
  if(lastToken != PF_NOTOKEN)
  {
    word = lastWord;
    TOCToken now = lastToken;
    lastToken = PF_NOTOKEN;
    return now;
  }

  // Trim of all leading space
  // And closing '>' after another seperator, eg: ">
  while(true)
  {
    if((c = GetNextTokenChar(file)) == _TEOF)
    {
      return PF_EOF;
    }
    if(c == _T('\n'))
    {
      ++linenumber;
    }
    if(!_istspace(c) && (c != _T('>')))
    {
      UngetNextTokenChar(c);
      break;
    }
  }
  // Get the token
  while(true)
  {
    if((c = GetNextTokenChar(file)) == _TEOF)
    {
      return PF_EOF;
    }
    if(c == _T('\n')) 
    {
      ++linenumber;
      continue;
    }
    buffer[index++] = c;
    buffer[index  ] = 0;

    // Check for end-of-string
    if (dostring && c == _T('\"') && normalchar)
    {
      buffer[--index] = 0;
      break;
    }
    // Check for end-of-token
    if((index > 1) && _tcschr(TOKEN_SEPERATOR,c))
    {
      if(!dostring)
      {
        UngetNextTokenChar(c);
        buffer[--index] = 0;
        break;
      }
    }
    // Check for start-of-token OR start-of-string
    if(normalchar && _tcschr(TOKEN_START,c))
    {
      // Chop of leading spaces
      index = 0;
      if (c == _T('\"'))
      {
        dostring = true;
      }
    }
  } 
  word = buffer;
  if(dostring)
  {
    return PF_STRING;
  }
     _tcsupr(buffer);
  if(_tcscmp(buffer,_T("HTML"))   == 0) return PF_HTML;
  if(_tcscmp(buffer,_T("HEAD"))   == 0) return PF_HEAD;
  if(_tcscmp(buffer,_T("META"))   == 0) return PF_META;
  if(_tcscmp(buffer,_T("BODY"))   == 0) return PF_BODY;
  if(_tcsncmp(buffer,_T("!--"),3) == 0) return PF_COMMENT;
  if(_tcscmp(buffer,_T("="))      == 0) return PF_EQUAL;
  if(_tcscmp(buffer,_T("OBJECT")) == 0) return PF_OBJECT;
  if(_tcscmp(buffer,_T("UL"))     == 0) return PF_LIST;
  if(_tcscmp(buffer,_T("LI"))     == 0) return PF_LISTITEM;
  if(_tcscmp(buffer,_T("A"))      == 0) return PF_ANCHOR;
  if(_tcscmp(buffer,_T("BR"))     == 0) return PF_BREAK;
  if(_tcscmp(buffer,_T("/HTML"))  == 0) return PF_ENDHTML;
  if(_tcscmp(buffer,_T("/HEAD"))  == 0) return PF_ENDHEAD;
  if(_tcscmp(buffer,_T("/BODY"))  == 0) return PF_ENDBODY;
  if(_tcscmp(buffer,_T("/OBJECT"))== 0) return PF_ENDOBJECT;
  if(_tcscmp(buffer,_T("/UL"))    == 0) return PF_ENDLIST;
  if(_tcscmp(buffer,_T("/LI"))    == 0) return PF_ENDLISTITEM;
  if(_tcscmp(buffer,_T("/A"))     == 0) return PF_ENDANCHOR;
  if(_tcscmp(buffer,_T("TYPE"))   == 0) return PF_TYPE;
  if(_tcscmp(buffer,_T("PARAM"))  == 0) return PF_PARAM;
  if(_tcscmp(buffer,_T("NAME"))   == 0) return PF_NAME;
  if(_tcscmp(buffer,_T("VALUE"))  == 0) return PF_VALUE;
  if(_tcscmp(buffer,_T("!DOCTYPE"))  == 0) return PF_DOCTYPE;
  if(_tcscmp(buffer,_T("CONTENT"))   == 0) return PF_CONTENT;

  return PF_NOTOKEN;
}

CString
Misc::FormatXMLString(const CString& p_string)
{
  CString result;
  TCHAR   c = 0;

  for(int ind = 0; ind < p_string.GetLength(); ++ind)
  {
    c = p_string.GetAt(ind);
    switch(c)
    {
      case _T('&'):  result += _T("&amp;");  break;
      case _T('\''): result += _T("&apos;"); break;
      case _T('\"'): result += _T("&quot;"); break;
      case _T('<'):  result += _T("&lt;");   break;
      case _T('>'):  result += _T("&gt;");   break;
      default:   result += (TCHAR)c; break;
    }
  }
  return result;
}

typedef struct _bomTokens
{
  int m_first;
  int m_second;
  int m_third;
  int m_fourth;
}
BOMTokens;

BOMTokens bomTokens[] =
{
  /* UTF-8  */         { 0x0EF,  0x0BB,  0x0BF,  0      }
  /* UTF-16 (BE) */   ,{ 0x0FE,  0x0FF,  0,      0      }
  /* UTF-16 (LE) */   ,{ 0x0FF,  0xFE,   0,      0      }
  /* UTF-32 (BE) */   ,{ 0x00,   0x00,   0x0FE,  0x0FF  }
  /* UTF-32 (LE) */   ,{ 0x0FF,  0x0FE,  0x000,  0x000  }
  /* UTF-7  */        ,{ 0x02B,  0x02F,  0x076,  0      } // m_fourth -> 4 possibilities!!
  /* UTF-1  */        ,{ 0x0F7,  0x064,  0x04C,  0      }
  /* UTF-EBCDIC */    ,{ 0x0DD,  0x073,  0x066,  0x073  }
  /* SCSU   */        ,{ 0x00E,  0x0FE,  0x0FF,  0      }
  /* BOCU-1 */        ,{ 0x0FB,  0x0EE,  0x028,  0x0FF  } // m_fourth = optional
  /* GB-18030    */   ,{ 0x084,  0x031,  0x095,  0x033  }
  /* CLOSING RECORD*/ ,{ 1,      1,      1,      1      }
};

void
Misc::SkipBOM(FILE* p_file)
{
  // GET FIRST CHAR
  int c1 = _gettc(p_file);
  // Check UTF-8
  if(c1 == 0x0EF)
  {
    // Check second char
    int c2 = _gettc(p_file);
    if(c2 == 0x0BB)
    {
      // check third char
      int c3 = _gettc(p_file);
      if(c3 == 0x0BF)
      {
        // OK-BOM SKIPPED
        return;
      }
    }
    // Rewind file (2nd/3d char)
    fseek(p_file,0L,SEEK_SET);
  }
  else
  {
    // NOT UTF-8
    _ungettc(c1,p_file);
  }
}

// Replacing filenames for all references to the file
// Takes care of the following
// - file protocol
// - basename from the MS-HTML server
// - java scripts
// - anchor bookmarks

bool
Misc::ReplaceFilenameExtra(CString& p_base
                          ,CString& p_old_href
                          ,CString& p_new_href
                          ,CString& p_input_ref
                          ,CString& p_output_ref)
{
  bool didReplace = false;
  // Copy input_ref for javascript
  CString fileref(p_input_ref);
  // detect javascript
  int script_begin = -1;
  int script_end   = -1;
  if(p_input_ref.Find(_T("javascript")) >= 0)
  {
     script_begin = p_input_ref.Find(_T('\''));
     if(script_begin > 0)
     {
       script_end = p_input_ref.Find(_T('\''),script_begin+1);
     }
     fileref = p_input_ref.Mid(script_begin + 1,script_end - script_begin - 1);
  }
  // detect fileprotocol / basename
  fileref = RemoveBasePart(p_base,fileref);
  // detect anchor bookmark
  CString anchor;
  bool hasAnchor = SplitMidpageAnchor(fileref,fileref,anchor);
  // See input equals p_old_href
  if(fileref.CompareNoCase(p_old_href))
  {
    // nothing found, early dropout
    p_output_ref = p_input_ref;
    return false;
  }
  // REPLACE by p_new_href
  fileref = p_new_href;
  didReplace = true;

  // Reconstruct
  if(hasAnchor)
  {
    fileref += _T("#") + anchor;
  }
  // Put result in 'output-ref'
  if(script_begin >= 0)
  {
    // recycle the script
    p_output_ref = p_input_ref.Left(script_begin) +
                   fileref +
                   p_input_ref.Mid(script_end + 1);
  }
  else
  {
    p_output_ref = fileref;
  }
  return didReplace;
}

CString 
Misc::GetExePath()
{
  TCHAR buffer[_MAX_PATH];

  CString application(TECHNICAL_APP);
  GetModuleFileName(GetModuleHandle(application), buffer, _MAX_PATH);
  CString applicationAndPath = buffer;

  int slashPositie = applicationAndPath.ReverseFind(_T('\\'));
  if (slashPositie == 0)
  {
    return _T("");
  }
  return applicationAndPath.Left(slashPositie + 1);
}

int
Misc::StartProgramma(CString& p_program
                    ,CString& p_arguments
                    ,bool     p_currentdir
                    ,bool     p_waitForExit
                    ,bool     p_show /*=false*/)
{
  if(p_currentdir)
  {
    CString pad = GetExePath();
    p_program = pad + p_program;

    if(_taccess(p_program,04) == -1)
    {
      // We do not find this program. Report!
      CString message;
      message.Format(_T("Cannot find the programm: '%s'"),p_program.GetString());
      ::MessageBox(NULL,message,_T("Error"),MB_OK|MB_ICONERROR);
      return -100;
    }
  }
  PROCESS_INFORMATION	processInfo;
  ZeroMemory(&processInfo, sizeof(processInfo));
  STARTUPINFO	startupInfo;
  ZeroMemory(&startupInfo, sizeof(startupInfo));
  startupInfo.cb = sizeof(startupInfo);
  startupInfo.dwFlags = STARTF_USESHOWWINDOW;
  startupInfo.wShowWindow = p_show ? SW_SHOW : SW_HIDE;

  // Creeer een command line 
  TCHAR commandLine[MAX_COMMANDLINE];
  _stprintf_s(commandLine,MAX_COMMANDLINE,_T("\"%s\" %s"),p_program.GetString(),p_arguments.GetString());
  BOOL res = CreateProcess((LPCTSTR)p_program     // Program to start or NULL
                          ,(LPTSTR)  commandLine   // Command line with arguments only
                          ,NULL         				  // Security
                          ,NULL  				          // ThreadAttributes
                          ,FALSE                  // Inherit handles
                          ,NORMAL_PRIORITY_CLASS  // Priority
                          ,NULL 				          // Environment
                          ,NULL            				// Current dir
                          ,&startupInfo           // Startupinfo
                          ,&processInfo		        // procesinfo
                          );
  DWORD exitCode = 0xFFFF;
  if(res)
  {
    if(p_waitForExit)
    {
      if(WaitForSingleObject(processInfo.hProcess,90000) == WAIT_OBJECT_0)
      {
        GetExitCodeProcess(processInfo.hProcess,&exitCode);
      }
    }
    ::CloseHandle(processInfo.hProcess);
    ::CloseHandle(processInfo.hThread);
  }
  return exitCode;
}

