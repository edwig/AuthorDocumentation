// ImageDefinitions
//
#pragma once

#define BUTT_LAYOUT_NONE    1
#define BUTT_LAYOUT_LEFT    2
#define BUTT_LAYOUT_RIGHT   4
#define BUTT_LAYOUT_BOTTOM  8
#define BUTT_LAYOUT_TOP     16
#define BUTT_LAYOUT_IMAGE   32
#define BUTT_LAYOUT_ALL     (1|2|4|8|16|32)

#define AFB_ICON_LIB  "ICONS"

#define AFB_POS_AANTAL      10  // Aantal in set voor Plaatje
#define AFB_POS_AANTAL_INFO 5   // Aantal in set voor Info

#define AFB_POS_UIT  -2         // geen plaatje tonen
#define AFB_POS_GEEN -1         // nog toe te kennen moet -1 blijven
#define AFB_POS_STAN  0         // Default moet op 0 blijven
#define AFB_POS_FOCUS 1
#define AFB_POS_PRESS 2
#define AFB_POS_DIS   3
#define AFB_POS_EXTRA 4

// Bitflags voor CAfbeeldingEntry
#define AFB_INFO_RESOURCE          1          // Mag laden van resource
#define AFB_INFO_FILE              2          // Mag laden uit een file
#define AFB_INFO_LIB               4          // Mag laden uit een library (directory)
#define AFB_INFO_INMEMORY          8          // Altijd in het geheugen houden
#define AFB_INFO_SYSTEM            16         // Entry mag niet verwijderd worden
#define AFB_INFO_LIJST             32         // Wordt altijd als lijst geladen
#define AFB_INFO_LIBLOAD           64         // Geladen library niet verwijderen
#define AFB_INFO_INVALID           128        // Kan niet laden

#define AFB_PAINT_TRANS            1          // TRANSPARENT tekenen
#define AFB_PAINT_FILL             2          // Fill background van het plaatje
#define AFB_PAINT_SAVESTRETCH      4          // Stretch met behoud van achtergrond
#define AFB_PAINT_DISABLED         8          // Teken diabled state
#define AFB_PAINT_BW               16         // Teken black en white
#define AFB_PAINT_NICE             32         // Zonder flikkering

#define AFB_SET_SET                0          // Geen logica
#define AFB_SET_KNOPPEN            1          // Standaard/Focus/Pressed/Disabled
#define AFB_SET_SERIE              2          // Opeenvolgend

#define AFB_STD_PREFIX             '@'
#define AFB_NAAM_PREFIX            '&'
#define AFB_NULL_PREFIX            '*'
