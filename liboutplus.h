/*
    pev - the PE file analyzer toolkit
    
    output.h - definitions for output results in differents outplus_formats
    
    Copyright (C) 2012 Fernando Mercês

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef OUTPUT_H
#define OUTPUT_H

/* Macros */

#define OUTPLUS_KEY_MAX_LEN 255
#define OUTPLUS_VALUE_MAX_LEN 1000
#define OUTPLUS_TITLE_MAX_LEN 1000

#define OUTPLUS_HTMLHEAD "<!DOCTYPE HTML>\n<html>\n\t<head>\n\t\t<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />\n\t\t<title>OUTPLUS Output</title>\n\t\t<style type=\"text/css\">\n\t\t\tbody { font-family: Sans, Arial; }\n\t\t\th1 { padding-top: 11px; padding-bottom: 11px; padding-left: 70px; background: transparent url(data:image/jpeg;base64,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) scroll no-repeat center left; }\n\t\t\th2,h3,h4,h5 { clear: both; float: none; margin-bottom: 0px; }\n\t\t\tdl { margin-bottom: 20px; float: left; }\n\t\t\t.even { background-color: #EEE; }\n\t\t\tdt { padding: 2px 5px; float: left; margin-right: 0px; clear: left; width: 240px; font-weight: bold; margin-bottom: 5px; font-weight: bold; }\n\t\t\tdd { width: 360px; margin-left: 0px; padding: 2px 5px; float: left; }\n\t\t</style>\n\t</head>\n\t<body>\n"
#define OUTPLUS_HTMLFOOTER "\t</body>\n</html>"

#define OUTPLUS_XMLHEAD "<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<output>\n"
#define OUTPLUS_XMLFOOTER "</output>"

/* structs, enums, typedefs etc... */

typedef enum {
    OUTPLUS_FORMAT_TEXT = 1,
    OUTPLUS_FORMAT_HTML = 2,
    OUTPLUS_FORMAT_XML = 3,
    OUTPLUS_FORMAT_CSV = 4,
    OUTPLUS_FORMAT_JSON = 5
} OUTPLUS_FORMAT_E;

OUTPLUS_FORMAT_E outplus_format;

typedef struct _OUTPLUS_SECTOR
{

    char title[OUTPLUS_TITLE_MAX_LEN];

    struct _OUTPLUS_LINE *first_line;

    struct _OUTPLUS_SECTOR *parent;
    struct _OUTPLUS_SECTOR *child;

    struct _OUTPLUS_SECTOR *next;
    struct _OUTPLUS_SECTOR *prev;

} OUTPLUS_SECTOR;//end :: struct :: _OUTPLUS_SECTOR

typedef struct _OUTPLUS_LINE
{
    char key[OUTPLUS_KEY_MAX_LEN];
    char value[OUTPLUS_VALUE_MAX_LEN];

    struct _OUTPLUS_LINE *next;
    struct _OUTPLUS_LINE *prev;
} OUTPLUS_LINE;//end :: struct :: _OUTPLUS_LINE

/* Prototypes */
// Core Functions
OUTPLUS_SECTOR * outplus_get_last_sector(OUTPLUS_SECTOR *sector);
OUTPLUS_SECTOR * outplus_add_sector(char *name, OUTPLUS_SECTOR ** current_OUTPLUS_SECTOR);
OUTPLUS_SECTOR * outplus_add_child_sector(char *name, OUTPLUS_SECTOR ** current_OUTPLUS_SECTOR);
OUTPLUS_LINE * outplus_get_last_sector_line(OUTPLUS_SECTOR *sector);
OUTPLUS_LINE outplus_add_line(char *key, char *value, OUTPLUS_SECTOR *sector);

// Output Parsers
void outplus_dump_text(OUTPLUS_SECTOR *sector);
void outplus_dump_html(OUTPLUS_SECTOR *sector);
void outplus_dump_xml(OUTPLUS_SECTOR *sector);
void outplus_dump_csv(OUTPLUS_SECTOR *sector);
void outplus_dump_json(OUTPLUS_SECTOR *sector);

void outplus_dump(OUTPLUS_SECTOR *sector);

void outplus_dump_sector_xml(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_text(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_csv(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_html(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_json(OUTPLUS_SECTOR *sector, unsigned int depth);

// Util Functions
void outplus_free_output(OUTPLUS_SECTOR *sector);
void outplus_parse_format(const char *optarg);
char *outplus_slug(char *string);
char *outplus_create_tabs(unsigned int count);

// Parser functions
void outplus_html_print_header();
void outplus_html_print_footer();
void outplus_xml_print_header();
void outplus_xml_print_footer();
#endif

