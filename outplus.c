/*
    liboutplus - C structured output library

    outplus.c - main library code

    Copyright (C) 2012 Jan Seidl

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.
    
    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include "outplus.h"

extern OUTPLUS_FORMAT_E outplus_format;

//************** SECTOR/LINE MANAGEMENT

OUTPLUS_SECTOR * outplus_get_last_sector(OUTPLUS_SECTOR *sector)
{

    if (!sector) return NULL;

    OUTPLUS_SECTOR *current_sector = sector;

    while (current_sector != NULL)
    {

        if (current_sector->next == NULL) {
            return current_sector;
        }
        current_sector = current_sector->next;

    }//end :: while

    return NULL;

}//end :: outplus_get_last_sector

OUTPLUS_SECTOR * outplus_add_child_sector(char *name, OUTPLUS_SECTOR ** currentOutplusSector)
{

    OUTPLUS_SECTOR *sector;

    sector = outplus_add_sector(name, &((*currentOutplusSector)->child));
    sector->parent = *currentOutplusSector;

    return sector;

}//end :: outplus_add_child_sector

OUTPLUS_SECTOR * outplus_add_sector(char *name, OUTPLUS_SECTOR ** currentOutplusSector)
{

    OUTPLUS_SECTOR *sector;

    sector = (OUTPLUS_SECTOR *)xmalloc(sizeof(OUTPLUS_SECTOR));

    strncpy(sector->title, name, OUTPLUS_TITLE_MAX_LEN);

    sector->first_line = NULL;
    sector->child = NULL;
    sector->parent = NULL;

    OUTPLUS_SECTOR *lastOutplusSector;
    lastOutplusSector =  (OUTPLUS_SECTOR *) outplus_get_last_sector(*currentOutplusSector);

    // First sector?
    if (NULL != lastOutplusSector)
    {
        OUTPLUS_SECTOR *last = lastOutplusSector->next;
        lastOutplusSector->next = sector;
        if (last != NULL) last->prev = sector;
        sector->prev = lastOutplusSector;
    } else {
        sector->prev = NULL;
        if (NULL == *currentOutplusSector) {
            *currentOutplusSector = sector;
        }//end :: if
    }//end :: if
    sector->next = NULL;

    return sector;
        
}//end :: outplus_add_sector

OUTPLUS_LINE * outplus_get_last_sector_line(OUTPLUS_SECTOR *sector)
{

    OUTPLUS_LINE *current_line = sector->first_line;

    while (current_line != NULL)
    {

        if (current_line->next == NULL) return current_line;
        current_line = current_line->next;

    }//end :: while

    return NULL;

}//end :: outplus_get_last_sector_line

OUTPLUS_LINE outplus_add_line(char *key, char *value, OUTPLUS_SECTOR *sector)
{

    OUTPLUS_LINE *line;
    line = (OUTPLUS_LINE *)xmalloc(sizeof(OUTPLUS_LINE));


    strncpy(line->key, key, OUTPLUS_KEY_MAX_LEN);
    strncpy(line->value, value, OUTPLUS_VALUE_MAX_LEN);

    OUTPLUS_LINE *last_OUTPLUS_LINE;
    last_OUTPLUS_LINE = (OUTPLUS_LINE *) outplus_get_last_sector_line(sector);

    if (NULL != last_OUTPLUS_LINE)
    {
        last_OUTPLUS_LINE->next = line;
        line->prev = last_OUTPLUS_LINE;
    } else {
        sector->first_line = line;
    }

    line->next = NULL;

    return *line;

}//end :: outplus_add_line

//************** TEXT OUTPUT

void outplus_dump_sector_text(OUTPLUS_SECTOR *sector, unsigned int depth) 
{

    OUTPLUS_SECTOR *current = sector;

    char *tabs = outplus_create_tabs(depth);

    while (current != NULL)
    {
        printf("%s%s\n", tabs, current->title);

        OUTPLUS_LINE *current_line = current->first_line;

        while (current_line != NULL)
        {

            printf("%s\t%s: %s\n", tabs, current_line->key, current_line->value);
            current_line = current_line->next;

        }//end :: while

        printf("\n");

        if (current->child != NULL) outplus_dump_sector_text(current->child, (depth+1)); // Print child by recursion

        current = current->next;
    }//end :: while

    free(tabs);

}//end :: outplus_dump_sector_text

void outplus_dump_text(OUTPLUS_SECTOR *sector) 
{

    outplus_dump_sector_text(sector, 0);

}//end :: outplus_dump_text

//************** JSON OUTPUT

void outplus_dump_sector_json(OUTPLUS_SECTOR *sector, unsigned int depth)
{

    OUTPLUS_SECTOR *current = sector;

    char *tabs = outplus_create_tabs((depth+1));

    while (current != NULL)
    {
        OUTPLUS_LINE *current_line = current->first_line;

        char sector_slug[OUTPLUS_TITLE_MAX_LEN];

        strncpy(sector_slug, current->title, OUTPLUS_TITLE_MAX_LEN);
        outplus_slug(sector_slug);

        printf("%s\"%s\":\n", tabs, sector_slug);
        printf("%s{\n", tabs);

        while (current_line != NULL)
        {

            printf("%s\t\"%s\": \"%s\"", tabs, outplus_slug(current_line->key), current_line->value);
            if (NULL != current_line->next) printf(",");
            printf("\n");
            current_line = current_line->next;

        }//end :: while

        if (current->child != NULL) outplus_dump_sector_json(current->child, (depth+1)); // Print child by recursion

        printf("%s}", tabs);
        if (NULL != current->next) printf(",");
        printf("\n");

        current = current->next;
    }//end :: while

    free(tabs);

}//end :: outplus_dump_sector_json

void outplus_dump_json(OUTPLUS_SECTOR *sector)
{

    printf("{\n");

    outplus_dump_sector_json(sector, 0);

    printf("}\n");

}//end :: outplus_dump_json


//************** CSV OUTPUT

void outplus_dump_sector_csv(OUTPLUS_SECTOR *sector, unsigned int depth)
{
    OUTPLUS_SECTOR *current = sector;

    while (current != NULL)
    {
        OUTPLUS_LINE *current_line = current->first_line;

        while (current_line != NULL)
        {

            printf("\"%s\",\"%s\",\"%s\"\n", current->title, current_line->key, current_line->value);
            current_line = current_line->next;

        }//end :: while

        if (current->child != NULL) outplus_dump_sector_csv(current->child, (depth+1)); // Print child by recursion

        current = current->next;
    }//end :: while
}//end :: outplus_dump_sector_csv

void outplus_dump_csv(OUTPLUS_SECTOR *sector)
{

    outplus_dump_sector_csv(sector, 0);

}//end :: outplus_dump_csv

//************** OUTPLUS_HTML OUTPUT

void outplus_html_print_header()
{

    printf(OUTPLUS_HTML_HEAD);
    printf("\t<h1>PEV Output</h1>\n");

}//end :: outplus_html_print_header

void outplus_html_print_footer()
{

    printf(OUTPLUS_HTML_FOOTER);

}//end :: outplus_html_print_footer

void outplus_dump_sector_html(OUTPLUS_SECTOR *sector, unsigned int depth)
{

    OUTPLUS_SECTOR *current = sector;

    unsigned short int header_type;
    unsigned int row_count;
    char css_class[4];

    header_type = (depth+2);

    if (header_type > 5) header_type = 5; // h5 is the last header level available

    while (NULL != current) {
        printf("\t<h%d>%s</h%d>\n", header_type, current->title, header_type);

        OUTPLUS_LINE *current_line = current->first_line;

        printf("\t<dl>\n");

        row_count = 0;

        while (current_line != NULL)
        {

            if ( row_count % 2 == 0 ) 
            {
                snprintf(css_class, 5, "even");
            } else {
                snprintf(css_class, 4, "odd");
            }//end :: if

            printf("\t\t<dt class=\"%s\">%s</dt>\n", css_class, current_line->key);
            printf("\t\t<dd class=\"%s\">%s</dd>\n", css_class, current_line->value);
            current_line = current_line->next;

            row_count++;

        }//end :: while

        printf("\t</dl>\n");

        if (current->child != NULL) outplus_dump_sector_html(current->child, (depth+1)); // Print child by recursion

        current = current->next;
    }//end :: while


}//end :: outplus_dump_sector_html

void outplus_dump_html(OUTPLUS_SECTOR *sector)
{


    outplus_html_print_header();

    outplus_dump_sector_html(sector, 0);

    outplus_html_print_footer();

}//end :: outplus_dump_html

//************** XML OUTPUT

void outplus_xml_print_header()
{

    printf(XML_HEAD);

}//end :: outplus_xml_print_header

void outplus_xml_print_footer()
{

    printf(XML_FOOTER);

}//end :: outplus_xml_print_footer

void outplus_dump_sector_xml(OUTPLUS_SECTOR *sector, unsigned int depth)
{

    OUTPLUS_SECTOR *current = sector;

    char *tabs = outplus_create_tabs((depth+1));

    while (NULL != current) {

        char sector_slug[OUTPLUS_TITLE_MAX_LEN];

        strncpy(sector_slug, current->title, OUTPLUS_TITLE_MAX_LEN);
        outplus_slug(sector_slug);

        printf("%s<%s title=\"%s\">\n", tabs, sector_slug, current->title);

        OUTPLUS_LINE *current_line = current->first_line;

        while (current_line != NULL)
        {

            printf("%s\t<%s>%s</%s>\n", tabs, outplus_slug(current_line->key), current_line->value, outplus_slug(current_line->key));
            current_line = current_line->next;

        }//end :: while


        if (current->child != NULL) outplus_dump_sector_xml(current->child, (depth+1)); // Print child by recursion

        printf("%s</%s>\n", tabs, outplus_slug(current->title));

        current = current->next;
    }//end :: while

    free(tabs); // cleanup!

}//end :: outplus_dump_sector_xml

void outplus_dump_xml(OUTPLUS_SECTOR *sector)
{


    outplus_xml_print_header();

    outplus_dump_sector_xml(sector, 0);

    outplus_xml_print_footer();

}//end :: outplus_dump_xml


//************** UTIL FUNCTIONS

void outplus_parse_format(const char *optarg)
{

    if (! strcmp(optarg, "text"))
        outplus_format = OUTPLUS_FORMAT_TEXT;
    else if (! strcmp(optarg, "json"))
        outplus_format = OUTPLUS_FORMAT_JSON;
    else if (! strcmp(optarg, "xml"))
        outplus_format = OUTPLUS_FORMAT_XML;
    else if (! strcmp(optarg, "csv"))
        outplus_format = OUTPLUS_FORMAT_CSV;
    else if (! strcmp(optarg, "html"))
        outplus_format = OUTPLUS_FORMAT_OUTPLUS_HTML;
    else
        EXIT_ERROR("invalid outplus_format option");

}//end :: outplus_parse_format

void outplus_dump(OUTPLUS_SECTOR *sector) 
{

    switch (outplus_format)
    {
        case OUTPLUS_FORMAT_XML:
            outplus_dump_xml(sector);
            break;
        case OUTPLUS_FORMAT_CSV:
            outplus_dump_csv(sector);
            break;
        case OUTPLUS_FORMAT_JSON:
            outplus_dump_json(sector);
            break;
        case OUTPLUS_FORMAT_OUTPLUS_HTML:
            outplus_dump_html(sector);
            break;
        case OUTPLUS_FORMAT_TEXT:
        default:
            outplus_dump_text(sector);
    }//end :: switch

    outplus_free_output(sector);

}//end :: outplus_dump

void outplus_free_output(OUTPLUS_SECTOR *sector) 
{

    OUTPLUS_SECTOR *current = sector;

    while (current != NULL)
    {

        OUTPLUS_LINE *current_line = current->first_line;

        while (current_line != NULL)
        {

            OUTPLUS_LINE *temp_line;
            temp_line = current_line;

            current_line = current_line->next;

            free(temp_line);

        }//end :: while

        if (current->child != NULL) outplus_free_output(current->child); // Clear child nodes via recursion
        
        OUTPLUS_SECTOR *temp_sector;
        temp_sector = current;
        current = current->next;
        free(temp_sector);
    }//end :: while


}//end :: outplus_free_output

char * outplus_create_tabs(unsigned int count)
{

    char *tabs;

    if (count > 0)
    {
        tabs = xmalloc(sizeof(char)*count);
    } else {
        tabs = xmalloc(sizeof(char));
    }

    tabs[0] = '\0';

    for (unsigned int i = 0; i<count; i++) tabs[i] = '\t';

    return tabs;

}//end :: outplus_create_tabs

char * outplus_slug(char *string)
{

    char *cp;
    cp = string;

    while (*cp) {
        if (64 < *cp && 91 > *cp) *cp = *cp+32;
        if (32 == *cp) *cp = '_';
        if ('(' == *cp || ')' == *cp || '/' == *cp || '\\' == *cp || '.' == *cp) *cp = '_';

        cp++;
    }//end :: while
    return string;

}//end :: outplus_slug
