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
   { "a",          "Anchor",                   0,    0,    0,   0   }
  ,{ "abbr",       "Abbreviation",             0,    0,    0,   1   }
  ,{ "acronym",    "Acronym",                  0,    0,    0,   1   }
  ,{ "address",    "Address",                  1,    1,    0,   0   }  // Adres
  ,{ "applet",     "Applet",                   0,    0,    0,   0   }
  ,{ "area",       "Area in image map",        0,    0,    0,   0   }
  ,{ "b",          "Bold",                     0,    0,    0,   0   }
  ,{ "base",       "Base of document",         0,    0,    0,   0   }
  ,{ "basefont",   "Basefont",                 0,    0,    0,   0   }
  ,{ "bdo",        "Bi-directional-override",  0,    0,    0,   0   }
  ,{ "bgsound",    "Background sound",         0,    0,    0,   0   }
  ,{ "big",        "Bigger text",              0,    0,    0,   0   }
  ,{ "blink",      "Blinking text",            0,    0,    0,   1   }
  ,{ "blockquote", "Blockquote",               1,    0,    0,   0   }
  ,{ "body",       "Document",                 0,    0,    0,   0   }
  ,{ "br",         "Break",                    0,    0,    0,   0   }
  ,{ "button",     "Button",                   0,    0,    0,   0   }
  ,{ "caption",    "Table caption",            1,    0,    0,   0   }
  ,{ "center",     "Center text",              0,    0,    0,   0   }
  ,{ "cite",       "Citation",                 0,    0,    0,   0   }
  ,{ "code",       "Program code",             0,    0,    0,   0   }
  ,{ "col",        "Table column",             0,    0,    0,   0   }
  ,{ "colgroup",   "Column group",             0,    0,    0,   0   }
  ,{ "dd",         "Definition data",          1,    1,    0,   0   }
  ,{ "del",        "Deleted text",             0,    0,    0,   0   }
  ,{ "dir",        "Directory",                1,    1,    0,   0   }  // Menulijst
  ,{ "div",        "Generic container",        0,    1,    0,   0   }
  ,{ "dl",         "Definition list",          1,    1,    0,   0   }  // Definitie
  ,{ "dt",         "Definition term",          1,    1,    0,   0   }  // Definitieterm
  ,{ "em",         "Emphasis",                 0,    0,    0,   0   }
  ,{ "embed",      "Embedded object",          0,    0,    0,   0   }
  ,{ "fieldset",   "Set of fields",            0,    0,    0,   0   }
  ,{ "font",       "Font",                     0,    0,    0,   0   }
  ,{ "form",       "Form",                     0,    0,    0,   0   }
  ,{ "frame",      "Frame",                    0,    0,    0,   0   }
  ,{ "frameset",   "Set of frames",            0,    0,    0,   0   }
  ,{ "h1",         "Heading 1",                1,    1,    0,   0   }
  ,{ "h2",         "Heading 2",                1,    1,    0,   0   }
  ,{ "h3",         "Heading 3",                1,    1,    0,   0   }
  ,{ "h4",         "Heading 4",                1,    1,    0,   0   }
  ,{ "h5",         "Heading 5",                1,    1,    0,   0   }
  ,{ "h6",         "Heading 6",                1,    1,    0,   0   }
  ,{ "head",       "Head of document",         0,    0,    0,   0   }
  ,{ "hr",         "Horizontal ruler",         0,    0,    0,   0   }
  ,{ "html",       "HTML",                     0,    0,    0,   0   }
  ,{ "i",          "Italic",                   0,    0,    0,   0   }
  ,{ "iframe",     "Inline frame",             0,    0,    0,   0   }
  ,{ "img",        "Image",                    0,    0,    0,   0   }
  ,{ "input",      "Input field",              0,    0,    0,   0   }
  ,{ "ins",        "Inserted text",            0,    0,    0,   0   }
  ,{ "isindex",    "Is indexable document",    0,    0,    0,   0   }
  ,{ "kbd",        "Keyboard text",            0,    0,    0,   0   }
  ,{ "keygen",     "Key generator",            0,    0,    0,   1   }
  ,{ "label",      "Label",                    0,    0,    0,   0   }
  ,{ "layer",      "Layer",                    0,    0,    0,   1   }
  ,{ "legend",     "Legend of fieldset",       0,    0,    0,   0   }
  ,{ "li",         "List item",                1,    1,    0,   0   }
  ,{ "link",       "CSS Link",                 0,    0,    0,   0   }
  ,{ "map",        "Image map",                0,    0,    0,   0   }
  ,{ "marquee",    "Marquee",                  1,    0,    0,   0   }
  ,{ "menu",       "Menu",                     1,    1,    0,   0   }  // menu
  ,{ "meta",       "Meta information",         0,    0,    0,   0   }
  ,{ "multicol",   "Multicolumn text",         1,    0,    0,   1   }
  ,{ "nobr",       "No line breaks",           0,    0,    0,   0   }
  ,{ "noembed",    "Text i.o. embed's",        0,    0,    0,   1   }
  ,{ "noframes",   "Text i.o. frames",         0,    0,    0,   0   }
  ,{ "nolayer",    "Text i.o. layers",         0,    0,    0,   1   }
  ,{ "noscript",   "Text i.o. scripts",        0,    0,    0,   1   }
  ,{ "object",     "Embedded object",          0,    0,    0,   0   }
  ,{ "ol",         "Ordered list",             1,    1,    0,   0   }  // genummerde lijst
  ,{ "optgroup",   "Optional group menu",      0,    0,    0,   1   }
  ,{ "option",     "Option of select field",   0,    0,    0,   0   }
  ,{ "p",          "Paragraph",                1,    1,    0,   0   }  // Normaal
  ,{ "param",      "Parameter",                0,    0,    0,   0   }
  ,{ "pre",        "Preformatted",             1,    1,    0,   0   }  // Met opmaak
  ,{ "q",          "Inline quote",             0,    0,    0,   0   }
  ,{ "s",          "Strikethrough",            0,    0,    0,   0   }
  ,{ "samp",       "Sample output",            0,    0,    0,   0   }
  ,{ "script",     "Script",                   0,    0,    0,   0   }
  ,{ "select",     "Combobox select",          0,    0,    0,   0   }
  ,{ "server",     "Javascript server",        0,    0,    0,   1   }
  ,{ "small",      "Smaller font",             0,    0,    0,   0   }
  ,{ "spacer",     "Blank space",              0,    0,    0,   0   }
  ,{ "span",       "Text +",                   0,    1,    0,   0   }
  ,{ "strike",     "Strikethrough",            0,    0,    0,   0   }
  ,{ "strong",     "Bold",                     0,    0,    0,   0   }
  ,{ "sub",        "Subscript",                0,    0,    0,   0   }
  ,{ "sup",        "Superscript",              0,    0,    0,   0   }
  ,{ "table",      "Table",                    0,    0,    0,   0   }
  ,{ "tbody",      "Table body",               0,    0,    0,   0   }
  ,{ "td",         "Table cell",               1,    0,    0,   0   }
  ,{ "textarea",   "Form textarea",            0,    0,    0,   0   }
  ,{ "tfoot",      "Table footer",             0,    0,    0,   0   }
  ,{ "th",         "Table header cell",        0,    0,    0,   0   }
  ,{ "thead",      "Table header",             0,    0,    0,   0   }
  ,{ "title",      "Document title",           0,    0,    0,   0   }
  ,{ "tr",         "Table row",                0,    0,    0,   0   }
  ,{ "tt",         "Typewriter text",          0,    0,    0,   0   }
  ,{ "u",          "Underlined",               0,    0,    0,   0   }
  ,{ "ul",         "Unordered list",           1,    1,    0,   0   }  // Lijst met opsommingstekens
  ,{ "var",        "Variable",                 0,    0,    0,   0   }
  ,{ "wbr",        "Potential word break",     0,    0,    0,   0   }
  ,{ "xmp",        "Example",                  1,    0,    0,   1   }
  ,{ NULL,         NULL,                       0,    0,    0,   0   }
};

// INFO FOR ATTRIBUTES

AttributeInfo attributes[] =
{
  { "background-repeat", "no-repeat",           "Don't tile"                       }
 ,{ "background-repeat", "repeat-x",            "Tile horizontally"                }
 ,{ "background-repeat", "repeat-y",            "Tile vertically"                  }
 ,{ "background-repeat", "repeat",              "Tile horizontally and vertically" }
 ,{ "background-repeat", "inherit",             "Inherited"                        }

 ,{ "list-style-position", "unspecified",       "Unspecified"                      }
 ,{ "list-style-position", "inherit",           "Inherited"                        }
 ,{ "list-style-position", "inside",            "Inside element box"               }
 ,{ "list-style-position", "outside",           "Outside element box"              }

 ,{ "list-style-type",   "unspecified",         "Unspecified"                      }
 ,{ "list-style-type",   "none",                "No list style type"               }
 ,{ "list-style-type",   "inherit",             "Inherited"                        }
 ,{ "list-style-type",   "disc",                "Filled up disc"                   }
 ,{ "list-style-type",   "circle",              "Open circle"                      }
 ,{ "list-style-type",   "square",              "Filled up square"                 }
 ,{ "list-style-type",   "decimal",             "Decimal numbers (1,2,3,...)"      }
 ,{ "list-style-type",   "decimal-leading-zero","Decimal numbers with zero (01,02,...,99)" }
 ,{ "list-style-type",   "lower-roman",         "Lowercase roman (i,ii,iii,....)"  }
 ,{ "list-style-type",   "upper-roman",         "Uppercase roman (I,II,III,....)"  }
 ,{ "list-style-type",   "lower-greek",         "Lowercase greek (alfa,beta,gamma...)"     }
 ,{ "list-style-type",   "lower-alpha",         "Lowercase alpha (a,b,c,...)"      }
 ,{ "list-style-type",   "lower-latin",         "Lowercase latin (a,b,c,...)"      }
 ,{ "list-style-type",   "upper-alpha",         "Uppercase alpha (A,B,C,...)"      }
 ,{ "list-style-type",   "upper-latin",         "Uppercase latin (A,B,C,...)"      }
 ,{ "list-style-type",   "hebrew",              "Traditional Hebrew numbering"     }
 ,{ "list-style-type",   "armenian",            "Traditional Armenian numbering"   }
 ,{ "list-style-type",   "georgian",            "Georgian (an,ban,gan,...)"        }
 ,{ "list-style-type",   "cjk-ideographic",     "Plain Ideografical numbering"     }
 ,{ "list-style-type",   "hiragana",            "Hiragana (a,i,u,e,o,ka,ki,...)"            }
 ,{ "list-style-type",   "katakana",            "Katakana (A,I,U,E,O,KA,KI,...)"            }
 ,{ "list-style-type",   "hiragana-iroha",      "Hiragana-iroha (i,ro,ha,ni,ho,he,to,...)"  }
 ,{ "list-style-type",   "katakana-iroha",      "Katakana-iroha (I,RO,HA,NI,HO,HE,TO,...)"  }

 // Single;1.5 lines;Double;
 ,{ "line-height",       "",                    "Single"                    }
 ,{ "line-height",       "150%",                "1.5 lines"                 }
 ,{ "line-height",       "200%",                "Double"                    }

 // Textarea wordwrap
 ,{ "wrap",              "off",                 "No word wrap"              }
 ,{ "wrap",              "soft",                "Local word wrap"           }
 ,{ "wrap",              "hard",                "Word wrap to server"       }
 // IE doesn't know these types
 ,{ "wrap",              "virtual",             "Local word wrap"           }
 ,{ "wrap",              "physical",            "Word wrap to server"       }

 // target frames
 ,{ "target",            "",                    "Page default"              }
 ,{ "target",            "_blank",              "New window"                }
 ,{ "target",            "_self",               "Same frame"                }
 ,{ "target",            "_parent",             "Parent frame"              }
 ,{ "target",            "_top",                "Whole page"                }

 // Script types
 ,{ "script-type",       "text/javascript",     "Javascript"                }
 ,{ "script-type",       "text/Jscript",        "MS-JScript"                }
 ,{ "script-type",       "text/vbs",            "VBS"                       }
 ,{ "script-type",       "text/vbscript",       "VB-Script"                 }
 ,{ "script-type",       "text/ecmascript",     "EcmaScript"                }

 // Language
 ,{ "language",          "Javascript",          "Javascript"                }
 ,{ "language",          "Javascript1.1",       "Javascript 1.1"            }
 ,{ "language",          "Javascript1.2",       "Javascript 1.2"            }
 ,{ "language",          "Javascript1.3",       "Javascript 1.3"            }
 ,{ "language",          "Javascript1.4",       "Javascript 1.4"            }
 ,{ "language",          "JScript",             "MS-JScript"                }
 ,{ "language",          "VBScript",            "VB Script"                 }
 ,{ "language",          "vbs",                 "VBS"                       }
 ,{ "language",          "EcmaScript",          "EcmaScript"                }

 // Shape of a area
 ,{ "shape",             "rect",                "Rectangle"                 }
 ,{ "shape",             "rectangle",           "Rectangle"                 }
 ,{ "shape",             "circ",                "Circle"                    }
 ,{ "shape",             "circle",              "Circle"                    }
 ,{ "shape",             "poly",                "Polygon"                   }
 ,{ "shape",             "polygon",             "Polygon"                   }

 // Style of the border
 ,{ "border-style",      "none",                 "None"                     }
 ,{ "border-style",      "solid",                "Solid"                    }
 ,{ "border-style",      "dotted",               "Dotted"                   }
 ,{ "border-style",      "dashed",               "Dashed"                   }
 ,{ "border-style",      "double",               "Double"                   }
 ,{ "border-style",      "groove",               "Groove"                   }
 ,{ "border-style",      "ridge",                "Ridge"                    }
 ,{ "border-style",      "inset",                "Inset"                    }
 ,{ "border-style",      "outset",               "Outset"                   }
 ,{ "border-style",      "inherit",              "Inherit"                  }

 // Events for the <SCRIPT> tag
 ,{ "script-event",      "onreadystatechange",  "When the page loading is complete"}
 ,{ "script-event",      "onselectstart",       "When the user starts selection"   }

 ,{ NULL,                NULL,                  NULL                        }
};

// INFO FOR COLORS

InternetColors internet_colors[] = 
{
  // Safe name               RGB Color code
  { "aliceblue",            "#A0CE00" }
 ,{ "antiquewhite",         "#FAEBD7" } 
 ,{ "aqua",                 "#00FFFF" }
 ,{ "aquamarine",           "#7FFFD4" }
 ,{ "azure",                "#F0FFFF" }
 ,{ "beige",                "#F5F5DC" }
 ,{ "bisque",               "#FFE4C4" }
 ,{ "black",                "#000000" }
 ,{ "blanchedalmond",       "#FFEBCD" }
 ,{ "blue",                 "#0000FF" }
 ,{ "blueviolet",           "#8A2BE2" }
 ,{ "brown",                "#A52A2A" }
 ,{ "burlywood",            "#DEB887" }
 ,{ "cadetblue",            "#5F9EA0" }
 ,{ "chartreuse",           "#7EFF00" }
 ,{ "chocolate",            "#D2691E" }
 ,{ "coral",                "#FF7F50" }
 ,{ "cornflowerblue",       "#6495ED" }
 ,{ "cornsilk",             "#FFF8DC" }
 ,{ "crimson",              "#DC143C" }
 ,{ "cyan",                 "#00FFFF" }
 ,{ "darkblue",             "#00008B" }
 ,{ "darkcyan",             "#008B8B" }
 ,{ "darkgoldenrod",        "#B8860B" }
 ,{ "darkgray",             "#A9A9A9" }
 ,{ "darkgreen",            "#006400" }
 ,{ "darkkhaki",            "#BDB76B" }
 ,{ "darkmagenta",          "#8B008B" }
 ,{ "darkolivegreen",       "#556B2F" }
 ,{ "darkorange",           "#FF8C00" }
 ,{ "darkorchid",           "#9932CC" }
 ,{ "darkred",              "#8B0000" }
 ,{ "darksalmon",           "#E9967A" }
 ,{ "darkseagreen",         "#8FBC8F" }
 ,{ "darkslateblue",        "#483D8B" }
 ,{ "darkslategray",        "#2F4F4F" }
 ,{ "darkturqoise",         "#00CED1" }
 ,{ "darkviolet",           "#9400D3" }
 ,{ "deeppink",             "#FF1493" }
 ,{ "deepskyblue" ,         "#00BFFF" }
 ,{ "dimgray",              "#696969" }
 ,{ "dodgerblue",           "#1E90FF" }
 ,{ "fibrebrick",           "#B22222" }
 ,{ "floralwhite",          "#FFFAF0" }
 ,{ "forestgreen",          "#228B22" }
 ,{ "fuchsia",              "#FF00FF" }
 ,{ "gainsboro",            "#DCDCDC" }
 ,{ "ghostwhite",           "#F8F8FF" }
 ,{ "gold",                 "#FFD700" }
 ,{ "goldenrod",            "#DAA520" }
 ,{ "gray",                 "#808080" }
 ,{ "green",                "#008000" }
 ,{ "greenyellow",          "#ADFF2F" }
 ,{ "honeydew",             "#F0FFF0" }
 ,{ "hotpink",              "#FF69B4" }
 ,{ "indianred",            "#CD5C5C" }
 ,{ "indigo",               "#4B0082" }
 ,{ "ivory",                "#FFFFF0" }
 ,{ "khaki",                "#F0E68C" }
 ,{ "lavender",             "#E6E6FA" }
 ,{ "lavenderblush",        "#FFF0F5" }
 ,{ "lemonchiffon",         "#FFFACD" }
 ,{ "lightblue",            "#ADD8E6" }
 ,{ "lightcoral",           "#F08080" }
 ,{ "lightcyan",            "#E0FFFF" }
 ,{ "lightgoldenrodyellow", "#FAFAD2" }
 ,{ "lightgreen",           "#90EE90" }
 ,{ "lightgrey",            "#D3D3D3" }
 ,{ "lightpink",            "#FFB6C1" }
 ,{ "lightsalmon",          "#FFA07A" }
 ,{ "lightseagreen",        "#20B2AA" }
 ,{ "lightskyblue",         "#87CEFA" }
 ,{ "lightslategray",       "#778899" }
 ,{ "lightsteelblue",       "#B0C4DE" }
 ,{ "lightyellow",          "#FFFFE0" }
 ,{ "lime",                 "#00FF00" }
 ,{ "limegreen",            "#32CD32" } 
 ,{ "linen",                "#FAF0E6" }
 ,{ "magenta",              "#FF00FF" }
 ,{ "maroon",               "#800000" }
 ,{ "mediumaquamarine",     "#66CDAA" }
 ,{ "mediumblue",           "#0000CD" }
 ,{ "medumorchid",          "#BA55D3" }
 ,{ "mediumpurple",         "#9370DB" }
 ,{ "mediumseagreen",       "#3CB371" }
 ,{ "mediumslateblue",      "#7B68EE" }
 ,{ "mediumspringgreen",    "#00FA9A" }
 ,{ "mediumturquoise",      "#48D1CC" }
 ,{ "mediumvioletred",      "#C71585" }
 ,{ "midnightblue",         "#191970" }
 ,{ "mintcream",            "#F5FFFA" }
 ,{ "mistyrose",            "#FFF4E1" } 
 ,{ "navajowhite",          "#FFDEAD" }
 ,{ "navy",                 "#000080" }
 ,{ "oldlace",              "#FDF5E6" }
 ,{ "olive",                "#808000" }
 ,{ "olivedrab",            "#6B8E23" }
 ,{ "orange",               "#FFA500" }
 ,{ "orangered",            "#FF4500" }
 ,{ "orchid",               "#DA70D6" }
 ,{ "palegoldenrod",        "#EEE8AA" }
 ,{ "palegreen",            "#98FB98" }
 ,{ "paleturquoise",        "#AFEEEE" }
 ,{ "palevioletred",        "#DB7093" }
 ,{ "papayawhip",           "#FFEFD5" }
 ,{ "peachpuff",            "#FFDAB9" }
 ,{ "peru",                 "#CD853F" }
 ,{ "pink",                 "#FFC0CB" }
 ,{ "plum",                 "#DDA0DD" }
 ,{ "powderblue",           "#B0E0E6" }
 ,{ "purple",               "#800080" }
 ,{ "red",                  "#FF0000" }
 ,{ "rosybrown",            "#BC8F8F" }
 ,{ "royalblue",            "#4169E1" }
 ,{ "saddlebrown",          "#8B4513" }
 ,{ "salmon",               "#FA8072" }
 ,{ "sandybrown",           "#F4A660" }
 ,{ "seagreen",             "#2E8B57" }
 ,{ "seashell",             "#FFF5EE" }
 ,{ "sienna",               "#A0522D" }
 ,{ "silver",               "#C0C0C0" }
 ,{ "skyblue",              "#87CEEB" }
 ,{ "slateblue",            "#6A5ACD" }
 ,{ "slategray",            "#708090" }
 ,{ "snow",                 "#FFFAFA" }
 ,{ "springgreen",          "#00FF7F" }
 ,{ "steelblue",            "#4682B4" }
 ,{ "tan",                  "#D2B48C" }
 ,{ "teal",                 "#008080" }
 ,{ "thistle",              "#D8BFD8" }
 ,{ "tomato",               "#FF6347" }
 ,{ "turquoise",            "#40E0D0" }
 ,{ "violet",               "#EE82EE" }
 ,{ "wheat",                "#F5DEB3" }
 ,{ "white",                "#FFFFFF" }
 ,{ "whitesmoke",           "#F5F5F5" }
 ,{ "yellow",               "#FFFF00" }
 ,{ "yellowgreen",          "#9ACD32" }
 ,{ NULL,                   NULL      }
};

// INFO FOR DIACRITE CHARACTERS

#pragma warning(disable: 4245) // char to uchar conversion
#pragma warning(disable: 4838) // char to uchar conversion

Diacrites DiacriteLetters[] = 
{
   { 'a', 'à', ACC_LOWER, "&agrave;", "a grave"       }
  ,{ 'a', 'á', ACC_LOWER, "&aacute;", "a acute"       }
  ,{ 'a', 'ä', ACC_LOWER, "&auml;",   "a umlaut"      }
  ,{ 'a', 'â', ACC_LOWER, "&acirc;",  "a circumflex"  }
  ,{ 'a', 'ã', ACC_LOWER, "&atilde;", "a tilde"       }
  ,{ 'a', 'å', ACC_LOWER, "&aring;",  "a ring"        }

  ,{ 'e', 'è', ACC_LOWER, "&egrave;", "e grave"       }
  ,{ 'e', 'é', ACC_LOWER, "&eacute;", "e acute"       }
  ,{ 'e', 'ë', ACC_LOWER, "&euml;",   "e umlaut"      }
  ,{ 'e', 'ê', ACC_LOWER, "&ecirc;",  "e circumflex"  }

  ,{ 'i', 'ì', ACC_LOWER, "&igrave;", "i grave"       }
  ,{ 'i', 'í', ACC_LOWER, "&iacute;", "i acute"       }
  ,{ 'i', 'ï', ACC_LOWER, "&iuml;",   "i umlaut"      }
  ,{ 'i', 'î', ACC_LOWER, "&icirc;",  "i circumflex"  }

  ,{ 'o', 'ò', ACC_LOWER, "&ograve;", "o grave"       }
  ,{ 'o', 'ó', ACC_LOWER, "&oacute;", "o acute"       }
  ,{ 'o', 'ö', ACC_LOWER, "&ouml;",   "o umlaut"      }
  ,{ 'o', 'ô', ACC_LOWER, "&ocirc;",  "o circumflex"  }
  ,{ 'o', 'õ', ACC_LOWER, "&otilde;", "o tilde"       }
  ,{ 'o', 'ø', ACC_LOWER, "&oslash;", "o slash"       }

  ,{ 'u', 'ù', ACC_LOWER, "&ugrave;", "u grave"       }
  ,{ 'u', 'ú', ACC_LOWER, "&uacute;", "u acute"       }
  ,{ 'u', 'ü', ACC_LOWER, "&uuml;",   "u umlaut"      }
  ,{ 'u', 'û', ACC_LOWER, "&ucirc;",  "u circumflex"  }

  ,{ 'y', 'ý', ACC_LOWER, "&yacute;", "y acute"       }
  ,{ 'y', 'ÿ', ACC_LOWER, "&yuml;",   "y umlaut"      }

  ,{ 'A', 'À', ACC_UPPER, "&Agrave;", "A grave"       }
  ,{ 'A', 'Á', ACC_UPPER, "&Aacute;", "A acute"       }
  ,{ 'A', 'Ä', ACC_UPPER, "&Auml;",   "A umlaut"      }
  ,{ 'A', 'Â', ACC_UPPER, "&Acirc;",  "A circumflex"  }
  ,{ 'A', 'Ã', ACC_UPPER, "&Atilde;", "A tilde"       }
  ,{ 'A', 'Å', ACC_UPPER, "&Aring;",  "A ring"        }

  ,{ 'E', 'È', ACC_UPPER, "&Egrave;", "E grave"       }
  ,{ 'E', 'É', ACC_UPPER, "&Eacute;", "E acute"       }
  ,{ 'E', 'Ë', ACC_UPPER, "&Euml;",   "E umlaut"      }
  ,{ 'E', 'Ê', ACC_UPPER, "&Ecirc;",  "E circumflex"  }

  ,{ 'I', 'Ì', ACC_UPPER, "&Igrave;", "I grave"       }
  ,{ 'I', 'Í', ACC_UPPER, "&Iacute;", "I acute"       }
  ,{ 'I', 'Ï', ACC_UPPER, "&Iuml;",   "I umlaut"      }
  ,{ 'I', 'Î', ACC_UPPER, "&Icirc;",  "I circumflex"  }

  ,{ 'O', 'Ò', ACC_UPPER, "&Ograve;", "O grave"       }
  ,{ 'O', 'Ó', ACC_UPPER, "&Oacute;", "O acute"       }
  ,{ 'O', 'Ö', ACC_UPPER, "&Ouml;",   "O umlaut"      }
  ,{ 'O', 'Ô', ACC_UPPER, "&Ocirc;",  "O circumflex"  }
  ,{ 'O', 'Ø', ACC_UPPER, "&Oslash;", "O slash"       }

  ,{ 'U', 'Ù', ACC_UPPER, "&Ugrave;", "U grave"       }
  ,{ 'U', 'Ú', ACC_UPPER, "&Uacute;", "U acute"       }
  ,{ 'U', 'Ü', ACC_UPPER, "&Uuml;",   "U umlaut"      }
  ,{ 'U', 'Û', ACC_UPPER, "&Ucirc;",  "U circumflex"  }

  ,{ 'Y', 'Ý', ACC_UPPER, "&Yacute;", "Y acute"       }
  ,{ 'Y', 'ÿ', ACC_UPPER, "&Yuml;",   "Y umlaut"      }

  ,{ 'c', 'ç', OTH_LOWER, "&ccedil;", "c cedille"     }
  ,{ 'n', 'ñ', OTH_LOWER, "&ntilde;", "n tilde"       }
  ,{ 's', 'ß', OTH_LOWER, "&szlig;",  "sz ligiture"   }
  ,{ 'a', 'æ', OTH_LOWER, "&aelig;",  "ae ligiture"   }
  ,{ 'e', 'Ð', OTH_LOWER, "&eth;",    "Islandic eth"  }
  ,{ 't', 'þ', OTH_LOWER, "&thorn;",  "Islandic thorn"}

  ,{ 'C', 'Ç', OTH_UPPER, "&Ccedil;", "C cedille"     }
  ,{ 'N', 'Ñ', OTH_UPPER, "&Ntilde;", "N tilde"       }
  ,{ 'S', 'ß', OTH_UPPER, "&SZlig;",  "SZ ligiture"   }
  ,{ 'A', 'Æ', OTH_UPPER, "&AElig;",  "AE ligature"   }
  ,{ 'E', 'Ð', OTH_UPPER, "&ETH;",    "Islandic ETH"  }
  ,{ 'T', 'Þ', OTH_UPPER, "&THORN;",  "Islandic THORN"}

  ,{ 'a', 'a', GRE_LOWER, "&alpha;",   "alpha"        }
  ,{ 'b', 'b', GRE_LOWER, "&beta;",    "beta"         }
  ,{ 'g', 'g', GRE_LOWER, "&gamma;",   "gamma"        }
  ,{ 'd', 'd', GRE_LOWER, "&delta;",   "delta"        }
  ,{ 'e', 'e', GRE_LOWER, "&epsilon;", "epsilon"      }
  ,{ 'z', 'z', GRE_LOWER, "&zeta;",    "zeta"         }
  ,{ 'h', 'h', GRE_LOWER, "&eta;",     "eta"          }
  ,{ 'o', 'o', GRE_LOWER, "&theta;",   "theta"        }
  ,{ 'i', 'i', GRE_LOWER, "&iota;",    "iota"         }
  ,{ 'k', 'k', GRE_LOWER, "&kappa;",   "kappa"        }
  ,{ 'l', 'l', GRE_LOWER, "&lambda;",  "lambda"       }
  ,{ 'm', 'm', GRE_LOWER, "&mu;",      "mu"           }
  ,{ 'n', 'n', GRE_LOWER, "&nu;",      "nu"           }
  ,{ 'x', 'x', GRE_LOWER, "&xi;",      "xi"           }
  ,{ 'o', 'o', GRE_LOWER, "&omnicron;","omnicron"     }
  ,{ 'p', 'p', GRE_LOWER, "&pi;",      "pi"           }
  ,{ 'r', 'r', GRE_LOWER, "&rho;",     "rho"          }
  ,{ 's', 's', GRE_LOWER, "&sigmaf;",  "sigmaf"       }
  ,{ 's', 's', GRE_LOWER, "&sigma;",   "sigma"        }
  ,{ 't', 't', GRE_LOWER, "&tau;",     "tau"          }
  ,{ 'y', 'y', GRE_LOWER, "&upsilon;", "upsilon"      }
  ,{ 'f', 'f', GRE_LOWER, "&phi;",     "phi"          }
  ,{ 'x', 'x', GRE_LOWER, "&chi;",     "chi"          }
  ,{ 'p', 'p', GRE_LOWER, "&psi;",     "psi"          }
  ,{ 'w', 'w', GRE_LOWER, "&omega;",   "omega"        }

  ,{ 'a', 'A', GRE_UPPER, "&Alpha;",   "ALPHA"        }
  ,{ 'b', 'B', GRE_UPPER, "&Beta;",    "BETA"         }
  ,{ 'g', 'G', GRE_UPPER, "&Gamma;",   "GAMMA"        }
  ,{ 'd', 'D', GRE_UPPER, "&Delta;",   "DELTA"        }
  ,{ 'e', 'E', GRE_UPPER, "&Epsilon;", "EPSILON"      }
  ,{ 'z', 'Z', GRE_UPPER, "&Zeta;",    "ZETA"         }
  ,{ 'h', 'H', GRE_UPPER, "&Eta;",     "ETA"          }
  ,{ 'o', 'O', GRE_UPPER, "&Theta;",   "THETA"        }
  ,{ 'i', 'I', GRE_UPPER, "&Iota;",    "IOTA"         }
  ,{ 'k', 'K', GRE_UPPER, "&Kappa;",   "KAPPA"        }
  ,{ 'l', 'L', GRE_UPPER, "&Lambda;",  "LAMBDA"       }
  ,{ 'm', 'M', GRE_UPPER, "&Mu;",      "MU"           }
  ,{ 'n', 'N', GRE_UPPER, "&Nu;",      "NU"           }
  ,{ 'x', 'X', GRE_UPPER, "&Xi;",      "XI"           }
  ,{ 'o', 'O', GRE_UPPER, "&Omnicron;","OMNICRON"     }
  ,{ 'p', 'P', GRE_UPPER, "&Pi;",      "PI"           }
  ,{ 'r', 'R', GRE_UPPER, "&Rho;",     "RHO"          }
  ,{ 's', 'S', GRE_UPPER, "&Sigma;",   "SIGMA"        }
  ,{ 't', 'T', GRE_UPPER, "&Tau;",     "TAU"          }
  ,{ 'y', 'Y', GRE_UPPER, "&Upsilon;", "UPSILON"      }
  ,{ 'f', 'F', GRE_UPPER, "&Phi;",     "PHI"          }
  ,{ 'x', 'X', GRE_UPPER, "&Chi;",     "CHI"          }
  ,{ 'p', 'P', GRE_UPPER, "&Psi;",     "PSI"          }
  ,{ 'w', 'W', GRE_UPPER, "&Omega;",   "OMEGA"        }

  ,{ 'a', '€', COMMON,    "&euro;",   "euro"          }
  ,{ '@', '@', COMMON,    "&#64;",    "at-sign"       }
  ,{ 'a', '¢', COMMON,    "&cent;",   "cent"          }
  ,{ 'a', '£', COMMON,    "&pound;",  "pound"         }
  ,{ 'a', '¤', COMMON,    "&curren;", "currentcy"     }
  ,{ 'a', '¥', COMMON,    "&yen;",    "yen"           }
  ,{ 'a', '§', COMMON,    "&sect;",   "section"       }
  ,{ 'a', '©', COMMON,    "&copy;",   "copyright"     }
  ,{ 'a', 'ª', COMMON,    "&ordf;",   "femenine ordinal"  }
  ,{ 'a', 'º', COMMON,    "&ordm;",   "masculine ordinal" }
  ,{ 'a', '«', COMMON,    "&laquo;",  "left angle quote"  }
  ,{ 'a', '»', COMMON,    "&raquo;",  "right angle quote" }
  ,{ 'a', '¬', COMMON,    "&not;",    "not"           }
  ,{ 'a', '®', COMMON,    "&reg;",    "trademark"     }
  ,{ 'a', '¯', COMMON,    "&macr;",   "macron"        }
  ,{ 'a', '°', COMMON,    "&deg;",    "degree"        }
  ,{ 'a', '±', COMMON,    "&plusmn;", "plus minus"    }
  ,{ 'a', '¹', COMMON,    "&sup1;",   "superscript 1" }
  ,{ 'a', '²', COMMON,    "&sup2;",   "superscript 2" }
  ,{ 'a', '³', COMMON,    "&sup3;",   "superscript 3" }
  ,{ 'a', 'µ', COMMON,    "&micro;",  "micro / mu"    }
  ,{ 'a', '×', COMMON,    "&times;",  "multiply"      }
  ,{ 'a', 'ƒ', COMMON,    "&#131;",   "florin"        }
  ,{ 'a', '¿', COMMON,    "&iquest;", "inverted question"    }
  ,{ 'a', '¡', COMMON,    "&iexcl;",  "inverted exclamation" }
  ,{ 'a', '½', COMMON,    "&frac12;", "fraction 1/2"  }
  ,{ 'a', '¼', COMMON,    "&frac14;", "fraction 1/4"  }
  ,{ 'a', '¾', COMMON,    "&frac34;", "fraction 3/4"  }
  ,{ 'a', '¶', COMMON,    "&para;",   "paragraph"     }
  ,{ 'a', '÷', COMMON,    "&divide;", "division"      }
  ,{ 'a', '·', COMMON,    "&#149;",   "bullet"        } 
  ,{ 0,   0,   0,         NULL,       NULL            }
};

// INFO for BEHAVIOR EVENTS ON TAGS

TagEvents tagEvents[] =
{
  // EVENTS IN HTML 4.01 standard
   { true,  "OnClick",            "When clicking mousebutton",                        TAGS_ALL    }
  ,{ true,  "OnDblClick",         "When double clicking mousebutton",                 TAGS_ALL    }
  ,{ true,  "OnKeypress",         "When a key is pressed",                            TAGS_ALL    }
  ,{ true,  "OnKeydown",          "When a key is pressed down",                       TAGS_ALL    }
  ,{ true,  "OnKeyup",            "When a pressed key is going up",                   TAGS_ALL    }
  ,{ true,  "OnMousedown",        "When mousebutton is going down",                   TAGS_ALL    }
  ,{ true,  "OnMousemove",        "When the mouse moves",                             TAGS_ALL    }
  ,{ true,  "OnMouseout",         "When mouse is leaving element",                    TAGS_ALL    }
  ,{ true,  "OnMouseover",        "When mouse is moving over",                        TAGS_ALL    }
  ,{ true,  "OnMouseup",          "When mousebutton is going up",                     TAGS_ALL    }
  ,{ true,  "OnFocus",            "When an element gets focus",                       TAGS_ALL    }
  ,{ true,  "OnBlur",             "When an element loses focus",                      TAGS_ALL    }
  ,{ true,  "OnLoad",             "When the page loads",                              TAGS_BODY   }
  ,{ true,  "OnUnload",           "When leaving the page",                            TAGS_BODY   }
  ,{ true,  "OnChange",           "When a field has been changed",                    TAGS_INPUT|TAGS_SELECT|TAGS_TEXTAREA  }
  ,{ true,  "OnSelect",           "When some text is selected",                       TAGS_INPUT|TAGS_TEXTAREA              }
  // EVENTS IN IE5++
  ,{ false, "OnAfterUpdate",      "After update of <SCRIPT FOR=elem...",              TAGS_ALL    }
  ,{ false, "OnBeforeUpdate",     "Before update of <SCRIPT FOR=elem...",             TAGS_ALL    }
  ,{ false, "OnDataAvailable",    "If data available for <SCRIPT FOR=elem...",        TAGS_ALL    }
  ,{ false, "OnDataSetChanged",   "If data changed in <SCRIPT FOR=elem...",           TAGS_ALL    }
  ,{ false, "OnDataSetComplete",  "If all data loaded in <SCRIPT FOR=elem...",        TAGS_ALL    }
  ,{ false, "OnErrorUpdate",      "On an error while updating an element",            TAGS_ALL    }
  ,{ false, "OnFilterChange",     "When an image filter changes",                     TAGS_ALL    }
  ,{ false, "OnHelp",             "When the user requests F1 help",                   TAGS_ALL    }
  ,{ false, "OnSelectStart",      "When an element is selected",                      TAGS_ALL    } 
  ,{ false, "OnCellChange",       "When a data cell in a datasource changes",         TAGS_OBJECT|TAGS_BODY    }
  ,{ false, "OnCopy",             "When a users starts a 'copy' action",              TAGS_ALL    }
  ,{ false, "OnCut",              "When a users starts a 'cut' action",               TAGS_ALL    }
  ,{ false, "OnPaste",            "When a users starts a 'paste' action",             TAGS_ALL    }
  ,{ false, "OnDrag",             "When a users drags information",                   TAGS_ALL    }
  ,{ false, "OnDragStart",        "At the beginning of a drag operation",             TAGS_ALL    }
  ,{ false, "OnDragEnd",          "At the end of a drag operation",                   TAGS_ALL    }
  ,{ false, "OnDragEnter",        "When dragged object enters valid drop target",     TAGS_ALL    }
  ,{ false, "OnDragLeave",        "When dragged object leaves valid drop target",     TAGS_ALL    }
  ,{ false, "OnDragOver",         "When dragged object is over a valid drop target",  TAGS_ALL    }
  ,{ false, "OnDrop",             "When dragged object drops on a valid drop target", TAGS_ALL    }
  ,{ false, "OnLoseCapture",      "When an element loses the mouse capture",          TAGS_ALL    }
  ,{ false, "OnPropertyChange",   "When a property of an element changes",            TAGS_ALL    }
  ,{ false, "OnReadyStateChange", "When downloaded data or behavior is ready to use", TAGS_ALL    }
  ,{ false, "OnResize",           "When the size of an element is about to change",   TAGS_ALL    }
  ,{ false, "OnRowEnter",         "When a data source receives data",                 TAGS_ALL    }
  ,{ false, "OnRowExit",          "When a data source deletes data",                  TAGS_ALL    }
  ,{ false, "OnRowsDelete",       "When rows are deleted from a recordset",           TAGS_OBJECT|TAGS_BODY }
  ,{ false, "OnRowsInserted",     "When rows are inserted in a recordset",            TAGS_OBJECT|TAGS_BODY }
  ,{ false, "OnScroll",           "When the user repositions the scroll bar",         TAGS_ALL    }
  ,{ false, "OnActivate",         "When the element becomes active (not focus!)",     TAGS_ALL    }
  ,{ false, "OnDeactivate",       "When the element becomes not active (not focus!)", TAGS_ALL    }
  ,{ false, "OnBeforeActivate",   "Before activation of an element (not focus!)",     TAGS_ALL    }
  ,{ false, "OnBeforeDeactivate", "Before deactivation of an element (not focus!)",   TAGS_ALL    }
  ,{ false, "OnLayoutComplete",   "When the document layout is ready",                TAGS_ALL    }
  ,{ false, "OnMouseEnter",       "When the mouse moves into an element",             TAGS_ALL    }
  ,{ false, "OnMouseLeave",       "When the mouse moves out of an element",           TAGS_ALL    }
  ,{ false, "OnControlSelect",    "When element enters or leaves multi-selection",    TAGS_ALL    }
  ,{ false, "OnResizeEnd",        "After an element enters or leaves multi-selection",  TAGS_ALL  }
  ,{ false, "OnResizeStart",      "Before an element enters or leaves multi-selection", TAGS_ALL  }
  ,{ false, "OnFocusIn",          "Before setting focus on an element",               TAGS_ALL    }
  ,{ false, "OnFocusOut",         "After focus leaves an element",                    TAGS_ALL    }
  ,{ false, "OnMouseWheel",       "When the mouse wheel is rotated",                  TAGS_ALL    }
  ,{ false, "OnBeforeUnload",     "Before the document page is unloaded",             TAGS_BODY   }
  ,{ false, "OnSelect",           "When some text is selected",                       TAGS_BODY   }
  ,{ false, "OnAfterPrint",       "After the document is printed",                    TAGS_BODY   }
  ,{ false, "OnBeforePrint",      "Before the document is printed",                   TAGS_BODY   }
  ,{ false, "OnAbort",            "Error on loading the image leads to abort",        TAGS_IMG|TAGS_INPUT }
  ,{ false, "OnError",            "Error while loading the image/object/script",      TAGS_IMG|TAGS_INPUT|TAGS_OBJECT|TAGS_SCRIPT|TAGS_STYLE}
  ,{ false, "OnLoad",             "After loading the image",                          TAGS_IMG|TAGS_INPUT|TAGS_STYLE }
  ,{ false, "OnBeforeEditFocus",  "Before setting focus on editable content",         TAGS_BODY   }
  ,{ false, "OnContextMenu",      "Before showing the context menu",                  TAGS_BODY   }
  ,{ false, "OnStop",             "When the users uses 'stop' button or leaves page", TAGS_BODY   }
  ,{ false, "OnSelectionChange",  "When the selection state of document changes",     TAGS_BODY   }
  ,{ false, "OnBounce",           "When the direction of the motion changes",         TAGS_MARQUEE}
  ,{ false, "OnFinish",           "When total number of loops is reached",            TAGS_MARQUEE}
  ,{ false, "OnStart",            "At the beginning of every loop",                   TAGS_MARQUEE}
  ,{ false, "OnSubmit",           "When a form submit button is pressed",             TAGS_FORM   }
  ,{ false, "OnReset",            "When a form reset button is pressed",              TAGS_FORM   } 
  ,{ false, "",                   "",                                                 0           }
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
Misc::GetAllTags(vector<string>* all)
{
  for(int index=0; taginfo[index].tagname; ++index)
  {
    string tag = taginfo[index].tagname;
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

		if (t_Chr == '%') 
		{
			UINT u32_Hex = 0;

			// convert hexadecimal string into UINT ("FA" --> 0xFA)
			for (UINT D=1; D<=2; D++)
			{
				UINT u32_Digit = (UINT) pt_In[I+D];

  					 if (u32_Digit >= (UINT)'0' && u32_Digit <= (UINT)'9') u32_Digit -=  (UINT) '0';
				else if (u32_Digit >= (UINT)'A' && u32_Digit <= (UINT)'F') u32_Digit -= ((UINT) 'A' -10);
				else if (u32_Digit >= (UINT)'a' && u32_Digit <= (UINT)'f') u32_Digit -= ((UINT) 'a' -10);
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
 					       char  *s8_Out,           // IN=buffer to receive ANSI Html code
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

		if (Chr <= '~') // no conversion required
		{
			s8_Out[Pos] = (char) Chr;
			Pos ++;
		}
		else // encode HTML
		{
			sprintf(s8_Out +Pos, "&#x%04X;", Chr);
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
  CString beginTag = CString("<")  + s_tag;
  CString endTag   = CString("</") + s_tag + ">";
  if(!s_class.IsEmpty())
  {
    beginTag += CString(" class=\"") + s_class + "\"";
  }
  beginTag += ">";
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
  if(p_color.GetAt(0) != '#')
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
  if(p_color.GetAt(0) == '#')
  {
    if(p_color.GetLength() == 7)
    {
      // It's a RGB number #RRGGBB
      CString s_red = p_color.Mid(1,2);
      sscanf(s_red,"%x",&red);
      CString s_green = p_color.Mid(3,2);
      sscanf(s_green,"%x",&green);
      CString s_blue = p_color.Mid(5,2);
      sscanf(s_blue,"%x",&blue);
      return;
    }
    else if(p_color.GetLength() == 4)
    {
      // It's a RGB number #RGB
      CString s_red = p_color.Mid(1,1);
      s_red += s_red;
      sscanf(s_red,"%x",&red);
      CString s_green = p_color.Mid(2,1);
      s_green += s_green;
      sscanf(s_green,"%x",&green);
      CString s_blue = p_color.Mid(3,1);
      s_blue += s_blue;
      sscanf(s_blue,"%x",&blue);
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
  return "";
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
    return "";
  }
  if(p_tag.IsEmpty() && !p_class.IsEmpty())
  {
    // Selector = empty tag with classname
    combined = "S: " + p_class;
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
      combined = CString("B: ") + tags->description;
    }
    else
    {
      combined = CString("C: ") + tags->description;
      if(!p_class.IsEmpty())
      {
        combined += CString(" (") + p_class + ")";
      }
    }
    tags->is_used = true;
    p_userselect = tags->is_userselect;
    return combined;
  } 
  // Show as a TODO bug
  p_userselect = true;
  combined = "TODO: " + p_tag;
  if(!p_class.IsEmpty())
  {
    combined += " (" + p_class + ")";
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
  if(p_combined.Left(3) == "TOD") return;

  classname = "";
  CString descript = p_combined.Mid(3);
  
  // Find classname
  int pos = descript.Find("(");
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
  pCombo->AddItem("<No style>");

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
      int pos = styleName.Find('.');
      if(pos < 0)
      {
        pos = styleName.Find('#');
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
Misc::GetTagDescription(CString tagname)
{
  struct _taginfo* tag;
  InitMaps();

  tagname.MakeLower();
  if(tagMap.Lookup(tagname,(void*&)tag))
  {
    return tag->description;
  }
  return "";
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
Misc::CreateHeadElement(CComPtr<IHTMLDocument2> doc,ELEMENT_TAG_ID tagID)
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
          hr = head->appendChild(append,&newNode);
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
  if(tag.CompareNoCase("img") == 0)
  {
    // Already an image
    return pElement;
  }
  if(tag.CompareNoCase("area"))
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
          if(cimgMap.GetLength() > 0 && cimgMap.GetAt(0) == '#')
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
             ,CString* value/*=NULL*/)
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
            meta->get_name(&meta_name);
            meta->get_content(&meta_content);
            CString metaName = CW2CT(meta_name);
            CString content  = CW2CT(meta_content);
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
    return "";
  }
  // META TAG NOT FOUND. CREATE IT
  CComPtr<IHTMLElement> elem = Misc::CreateHeadElement(doc,TAGID_META);
  CComQIPtr<IHTMLMetaElement,&IID_IHTMLMetaElement> meta = elem;
  CComBSTR bName  = name;
  CComBSTR bValue = *value;
  meta->put_name   (bName);
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
    return "";
  }
  // BASE TAG NOT FOUND. CREATE IT
  CComPtr<IHTMLElement> elem = Misc::CreateHeadElement(doc,TAGID_BASE);
  CComQIPtr<IHTMLBaseElement,&IID_IHTMLBaseElement> base = elem;
  base->put_href(CT2CW(uri));
  if(!target.IsEmpty())
  {
    base->put_target(CT2CW(target));
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
    V_VT(&tagName)   = VT_BSTR;
    V_BSTR(&tagName) = CT2CW(tagname);
    CComPtr<IDispatch> disp;
    hr = coll->tags(tagName,&disp);
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
            if(isdigit(cID.GetAt(num)))
            {
              long number2 = atoi(&((cID.GetString())[num]));
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
  cMapID.Format("map%d",GetUniqueID(doc,"map"));
  CString mapping = CString("<MAP id=") + cMapID + "></MAP>";

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
  return "";
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
    newArea.Format("<AREA id=%s shape=%s>",areaID.GetString(),shape.GetString());
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
    if(fileref.Left(8).CompareNoCase("file:///") == 0)
    {
      fileref = fileref.Mid(8);
    }
  }
  fileref.Replace('/','\\');
  fileref.Replace('|',':');
  fileref.Replace("%20"," ");
  fileref.Replace("%23","#");
  fileref.Replace("%25","%");
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
    if(file.Left(11).CompareNoCase("javascript:") == 0)
    {
      file = file.Mid(11);
    }
  }
  if(file.GetLength() > 11)
  {
    if(file.Left(11).CompareNoCase("BSSCPopup('") == 0)
    {
      file = file.Mid(11);
      file = file.TrimRight("');");
    }
  }
  if(file.GetLength() > 14)
  {
    if(file.Left(14).CompareNoCase("ADHShowPopup('") == 0)
    {
      file = file.Mid(14);
      int pos = file.ReverseFind('\'');
      if(pos >= 0)
      {
        file = file.Left(pos);
      }
    }
  }
  return file;
}

bool
Misc::SplitMidpageAnchor(const CString& p_fileref,CString& p_file,CString& p_anchor)
{
  bool found = false;
  int pos = p_fileref.Find('#');
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
Misc::SubStr(CString& p_string,char p_Delim,int p_Num)
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
  return "";
}

CString
Misc::GetParamFromCommandline(CString& commandline
                             ,CString  p_commando
                             ,char     p_delim /* = '=' */)
{
  int nCmdPos = commandline.Find(p_commando);
  if(nCmdPos < 0)
  {
    nCmdPos = FindNoCase(commandline,p_commando);
  }
  if(nCmdPos<0)
  {
    return "";
  }
  int nIsPos = commandline.Find(p_delim,nCmdPos);
  if(nIsPos<0)
  {
    return "";
  }
  CString s = commandline.Mid(nIsPos + 1);
  s.TrimLeft();
  if(s.Find('"')==0)        // mag ook tussen kwootjes
  {
    return Misc::SubStr(s,'"',2);
  }
  return Misc::SubStr(s,' ',1);
}

int
Misc::CountNL(CString& sCurSentence,int pos)
{
  int numNL = 0;
  for(int ind=0; ind < pos; ++ind)
  {
    if(sCurSentence.GetAt(ind) == '\n')
    {
      ++numNL;
    }
  }
  return numNL;
}

CString 
Misc::BasenamePart(CString fullpath)
{
  char drive [_MAX_DRIVE + 1];
  char direct[_MAX_DIR   + 1];
  char fname [_MAX_FNAME + 1];
  char extens[_MAX_EXT   + 1];

  fullpath = StripFileProtocol(fullpath);
  _splitpath(fullpath.GetString(),drive,direct,fname,extens);
  CString filename(fname);
  return filename;
}

CString
Misc::FilenamePart(CString fullpath)
{
  char drive [_MAX_DRIVE + 1];
  char direct[_MAX_DIR   + 1];
  char fname [_MAX_FNAME + 1];
  char extens[_MAX_EXT   + 1];
 
  fullpath = StripFileProtocol(fullpath);
  _splitpath(fullpath.GetString(),drive,direct,fname,extens);
  CString filename = CString(fname) + CString(extens);
  return filename;
}

CString
Misc::ExtensionPart(CString fullpath)
{
  char drive [_MAX_DRIVE + 1];
  char direct[_MAX_DIR   + 1];
  char fname [_MAX_FNAME + 1];
  char extens[_MAX_EXT   + 1];

  fullpath = StripFileProtocol(fullpath);
  _splitpath(fullpath.GetString(),drive,direct,fname,extens);
  return CString(extens);
}

CString
Misc::DirectoryPart(CString fullpath)
{
  char drive [_MAX_DRIVE + 1];
  char direct[_MAX_DIR   + 1];
  char fname [_MAX_FNAME + 1];
  char extens[_MAX_EXT   + 1];

  fullpath = StripFileProtocol(fullpath);
  _splitpath(fullpath.GetString(),drive,direct,fname,extens);
  CString directory = CString(drive) + CString(direct);
  return directory;
}

CString
Misc::RemoveBasePart(CString base,CString fullpath)
{
  fullpath = StripFileProtocol(fullpath);
  fullpath.Replace('/','\\');
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
    if(stricmp(attrib->name,attribute)==0)
    {
      foundPoint = true;
      if(stricmp(attrib->value,value)==0)
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
    if(stricmp(attrib->name,attribute)==0)
    {
      foundPoint = true;
      if(stricmp(attrib->display,display)==0)
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
Misc::GetAllAttributeDisplaynames(CString attribute,vector<string>* all)
{
  AttributeInfo* attrib = attributes;
  bool foundPoint = false;

  while(attrib->name)
  {
    if(stricmp(attrib->name,attribute)==0)
    {
      foundPoint = true;
      string display = attrib->display;
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
  p_relative       = "";
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
  base.Replace('\\','/');
  absolute.Replace('\\','/');

  // Special case: only a filename, make it relative to the 'this' directory
  if(absolute.Find('/') < 0)
  {
    // Cannot use this in *.HHP projects!
    // p_relative = CString("./") + absolute;
    p_relative = absolute;
    return true;
  }
  // Special case: already a relative path
  if(absolute.GetAt(0) == '.')
  {
    p_relative = absolute;
    return true;
  }
  // Find the path-parts that are common to both names
  // We can eliminate these parts
  bool notCompatible = true;
  int  pos = absolute.Find('/');
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
    pos = absolute.Find('/');
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
  pos = base.Find('/');
  while(pos >= 0)
  {
    absolute = CString("../") + absolute;
    base = base.Mid(pos+1);
    pos  = base.Find('/');
  }
  p_relative = absolute;

  // Relative path is complete.
  // Now warn for files outside the project
  if(p_relative.GetLength() > 1)
  {
    if(p_relative.Left(2) == "..")
    {
      CString mess = "The file [" + p_relative + "] does not exist inside your project directory structure!\n"
                     "This can be a problem for the compilation, style sheets, images, links etc.";
      theApp.MessageBox(mess,"Project integrity",MB_OK|MB_ICONASTERISK);
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
  char buffer[_MAX_PATH+1];
  strncpy(buffer,path.GetString(),_MAX_PATH);
  bool foundReduction = true;

  while(foundReduction)
  {
    // Drop out if we find nothing;
    foundReduction = false;

    char* pnt1 = buffer;
    char* pnt2 = pnt1;
    char* pnt3 = pnt1;

    while(*pnt1 && *pnt1!='\\' && *pnt1!='/') ++pnt1;
    if(!*pnt1++)
    {
      // Not one directory seperator
      return path;
    }
    pnt3 = pnt1;
    while(*pnt1 && *pnt1!='\\' && *pnt1!='/') ++pnt1;
    if(!*pnt1++)
    {
      // Not a second directory seperator
      return path;
    }
    pnt2 = pnt1;
    while(*pnt1 && *pnt1!='\\' && *pnt1!='/') ++pnt1;
    while(*pnt1)
    {
      ++pnt1;
      // IN:  C:\direct1\direct2\direct3\..\..\direct4\
      //         |       |       |
      //      pnt3    pnt2    pnt1
      if(strncmp(pnt2,"..\\",3)==0 || strncmp(pnt2,"../",3)==0)
      {
        // Space between pnt2 and pnt1 = \..\
        // IN:  C:\direct1\direct2\direct3\..\..\direct4\
        //                         |       |  |
        //                      pnt3    pnt2  pnt1

        // REDUCTION
        strcpy(pnt3,pnt1);

        // At least one more loop
        foundReduction = true;
        // Return to the top!!
        break;
      }
      // Next level of directories
      pnt3 = pnt2;
      pnt2 = pnt1;
      while(*pnt1 && *pnt1!='\\' && *pnt1!='/') ++pnt1;
    }
  }
  return CString(buffer);
}


// Special optimized funtion to resolve %5C -> '\' in pathnames
int
Misc::ResolveSpecialChars(CString& value)
{
  int total = 0;

  int pos = value.Find('%');
  while (pos >= 0)
  {
    ++total;
    int num = 0;
    CString hexstring = value.Mid(pos+1,2);
    hexstring.MakeUpper();
    if(isdigit(hexstring.GetAt(0)))
    {
      num = hexstring.GetAt(0) - '0';
    }
    else
    { 
      num = hexstring.GetAt(0) - 'A' + 10;
    }
    num *= 16;
    if(isdigit(hexstring.GetAt(1)))
    {
      num += hexstring.GetAt(1) - '0';
    }
    else
    {
      num += hexstring.GetAt(1) - 'A' + 10;
    }
    value.SetAt(pos,(char)num);
    value = value.Left(pos+1) + value.Mid(pos + 3);
    pos = value.Find('%');
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
  return "";
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
  return "";
}

void
Misc::ParseNumber(CString& def,unsigned long& number)
{
  // Reset number
  number = 0;

  def.TrimLeft();
  int cpos = def.Find(',');
  CString num = def;
  if(cpos >= 0)
  {
    num = def.Left(cpos);
  }
  int Xpos = num.Find('X');
  int xpos = num.Find('x');
  xpos = (Xpos >= 0) ? Xpos : xpos;
  if(xpos >= 0)
  {
    number = 0;
    num = num.Mid(xpos+1);
    while(num.GetLength())
    {
      number *= 16;
      char c = num.GetAt(0);
      if(isdigit(c))
      {
        number += (int)(c - '0');
      }
      else
      {
        c = toupper(c);
        number += (int)(c - 'A' + 10);
      }
      num = num.Mid(1);
    }
  }
  else
  {
    // Simple number
    number = atoi(def.Left(cpos));
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
  char c;
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
  if(c == '+')
  {
    // Legaal positief getal
    c = def.GetAt(++index);
  }
  else if(c == '-')
  {
    // Legaal negatief getal
    negative = true;
    c = def.GetAt(++index);
  }
  // Scan for octal or hexadecimal number
  if(def.GetLength() > (index + 1) && def.GetAt(index) == '0')
  {
    bool xpos = (toupper(def.GetAt(index + 1)) == 'X');
    if(xpos)
    {
      // HEXADECIMAL NUMBER
      index += 2;
      c = def.GetAt(index);
      // Possibly a x-digit number
      while(def.GetLength() > index)
      {
        number *= 16;
        if(isdigit(c))
        {
          number += (int)(c - '0');
        }
        else
        {
          c = toupper(c);
          if(c >= 'A' && c <= 'F')
          {
            number += (int) (c - 'A' + 10);
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
        if(c >= '0' && c <= '7')
        {
          number *= 8;
          number += (c - '0');
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
    if(!isdigit(c))
    {
      break;
    }
    number *= 10;
    number += (c - '0');
    c = def.GetAt(++index);
  }
  // Stuk na decimale punt:komma
  if((def.GetLength() > index) && (c == '.' || c == ','))
  {
    c = def.GetAt(++index);
    broken = (double) number;
    double factor = 1;
    double part   = 0;
    while(def.GetLength() > index)
    {
      if(!isdigit(c))
      {
        break;
      }
      factor *= 10;
      part   *= 10;
      part   += (c - '0');
      c = def.GetAt(++index);
    }
    broken += (part / factor);
  }
  // IEEE Exponentional notation
  if(c == 'E' || c == 'e')
  {
    int exp = 0;
    c = def.GetAt(++index);
    bool negexp = false;
    if(c == '+')
    {
      // Legaal positief getal
      c = def.GetAt(++index);
    }
    else if(c == '-')
    {
      // Legaal negatief getal
      negexp = true;
      c = def.GetAt(++index);
    }
    while(def.GetLength() > index)
    {
      if(!isdigit(c))
      {
        break;
      }
      exp *= 10;
      exp += (c - '0');
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
  switch(pointsize)
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
  switch(fontsize)
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

const  char* TOKEN_START     = "<\"";
const  char* TOKEN_SEPERATOR = ">= \"\'";
static CString lastWord      = "";
static TOCToken lastToken    = PF_NOTOKEN;

bool
Misc::SkipToken(FILE *file,TOCToken expect,long& linenumber)
{
  CString word;
  TOCToken token = GetToken(file,word,linenumber);
  if(token == expect)
  {
    return true;
  }
  PushToken(word,token);
  return false;
}

void
Misc::PushToken(CString& word,TOCToken token)
{
  lastWord  = word;
  lastToken = token;
}

TOCToken
Misc::GetToken(FILE* file,CString& word,long& linenumber)
{
  int c = 0;
  int index = 0;
  bool dostring = false;
  char buffer[MAX_PATH+1];

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
    if((c = fgetc(file)) == EOF)
    {
      return PF_EOF;
    }
    if(c == '\n')
    {
      ++linenumber;
    }
    if(!isspace(c) && (c != '>'))
    {
      ungetc(c,file);
      break;
    }
  }
  // Get the token
  while(true)
  {
    if((c = fgetc(file)) == EOF)
    {
      return PF_EOF;
    }
    if(c == '\n') 
    {
      ++linenumber;
      continue;
    }
    buffer[index++] = c;
    buffer[index  ] = 0;

    if((index > 1) && strchr(TOKEN_SEPERATOR,c))
    {
      if(dostring && c == '\"')
      {
        buffer[--index] = 0;
        break;
      }
      //if(!(c == ' ' && dostring))
      if(!dostring)
      {
        ungetc(c,file);
        buffer[--index] = 0;
        break;
      }
    }
    if(strchr(TOKEN_START,c))
    {
      // Chop of leading spaces
      index = 0;
      if(c == '\"')
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
  strupr(buffer);
  if(strcmp(buffer,"HTML")   == 0) return PF_HTML;
  if(strcmp(buffer,"HEAD")   == 0) return PF_HEAD;
  if(strcmp(buffer,"BODY")   == 0) return PF_BODY;
  if(strncmp(buffer,"!--",3) == 0) return PF_COMMENT;
  if(strcmp(buffer,"=")      == 0) return PF_EQUAL;
  if(strcmp(buffer,"OBJECT") == 0) return PF_OBJECT;
  if(strcmp(buffer,"UL")     == 0) return PF_LIST;
  if(strcmp(buffer,"LI")     == 0) return PF_LISTITEM;
  if(strcmp(buffer,"A")      == 0) return PF_ANCHOR;
  if(strcmp(buffer,"BR")     == 0) return PF_BREAK;
  if(strcmp(buffer,"/HTML")  == 0) return PF_ENDHTML;
  if(strcmp(buffer,"/HEAD")  == 0) return PF_ENDHEAD;
  if(strcmp(buffer,"/BODY")  == 0) return PF_ENDBODY;
  if(strcmp(buffer,"/OBJECT")== 0) return PF_ENDOBJECT;
  if(strcmp(buffer,"/UL")    == 0) return PF_ENDLIST;
  if(strcmp(buffer,"/LI")    == 0) return PF_ENDLISTITEM;
  if(strcmp(buffer,"/A")     == 0) return PF_ENDANCHOR;
  if(strcmp(buffer,"TYPE")   == 0) return PF_TYPE;
  if(strcmp(buffer,"PARAM")  == 0) return PF_PARAM;
  if(strcmp(buffer,"NAME")   == 0) return PF_NAME;
  if(strcmp(buffer,"VALUE")  == 0) return PF_VALUE;

  return PF_NOTOKEN;
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
  int c1 = getc(p_file);
  // Check UTF-8
  if(c1 == 0x0EF)
  {
    // Check tweede char
    int c2 = getc(p_file);
    if(c2 == 0x0BB)
    {
      // check derde char
      int c3 = getc(p_file);
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
    // Geen UTF-8
    ungetc(c1,p_file);
  }
}

// Replacing filenames for all references to the file
// Takes care of the following
// - file protocol
// - basename from the MS-HTML server
// - javascripts
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
  if(p_input_ref.Find("javascript") >= 0)
  {
     script_begin = p_input_ref.Find('\'');
     if(script_begin > 0)
     {
       script_end = p_input_ref.Find('\'',script_begin+1);
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
    fileref += "#" + anchor;
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
  char buffer[_MAX_PATH];

  CString application(TECHNICAL_APP);
  GetModuleFileName(GetModuleHandle(application), buffer, _MAX_PATH);
  CString applicationAndPath = buffer;

  int slashPositie = applicationAndPath.ReverseFind('\\');
  if (slashPositie == 0)
  {
    return "";
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

    if(_access(p_program,04) == -1)
    {
      // We do not find this program. Report!
      CString message;
      message.Format("Cannot find the programm: '%s'",p_program.GetString());
      ::MessageBox(NULL,message,"Error",MB_OK|MB_ICONERROR);
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
  char commandLine[MAX_COMMANDLINE];
  sprintf_s(commandLine,MAX_COMMANDLINE,"\"%s\" %s",p_program.GetString(),p_arguments.GetString());
  BOOL res = CreateProcess((LPCTSTR)p_program     // Program to start or NULL
                          ,(LPSTR)  commandLine   // Command line with arguments only
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

