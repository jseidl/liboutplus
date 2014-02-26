/*
    liboutplus - C structured output library

    outplus.c - main library code

    Copyright (C) 2012 - 2014 Jan Seidl

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
        current_sector = (OUTPLUS_SECTOR *) current_sector->next;

    }//end :: while

    return NULL;

}//end :: outplus_get_last_sector

int outplus_add_child_sector(char *name, OUTPLUS_SECTOR ** new_sector, OUTPLUS_SECTOR ** parent_sector)
{

    int rv = OUTPLUS_E_OK;

    rv = outplus_add_sector(name, new_sector, (OUTPLUS_SECTOR **) &((*parent_sector)->child));
    if (rv != OUTPLUS_E_OK) return rv;

    (*new_sector)->parent = (OUTPLUS_SECTOR_PTR) *parent_sector;

    return rv;

}//end :: outplus_add_child_sector

int outplus_add_sector(char *name, OUTPLUS_SECTOR ** new_sector, OUTPLUS_SECTOR ** sector)
{

    int rv = OUTPLUS_E_OK;

    *new_sector = (OUTPLUS_SECTOR *) malloc(sizeof(OUTPLUS_SECTOR));

    if (*new_sector == NULL) return OUTPLUS_E_ALLOCATION_FAILURE;

    strncpy((*new_sector)->title, name, OUTPLUS_TITLE_MAX_LEN);

    (*new_sector)->first_line = NULL;
    (*new_sector)->child = NULL;
    (*new_sector)->parent = NULL;
    (*new_sector)->prev = NULL;
    (*new_sector)->next = NULL;

    OUTPLUS_SECTOR *last_sector;
    last_sector =  (OUTPLUS_SECTOR *) outplus_get_last_sector(*sector);

    // First sector?
    if (NULL != last_sector)
    {
        last_sector->next = (OUTPLUS_SECTOR_PTR) *new_sector;
        (*new_sector)->prev = (OUTPLUS_SECTOR_PTR) last_sector;
    } else {
        if (NULL == *sector) {
            *sector = *new_sector;
        }//end :: if
    }//end :: if

    return rv;
        
}//end :: outplus_add_sector

OUTPLUS_LINE * outplus_get_last_sector_line(OUTPLUS_SECTOR *sector)
{

    OUTPLUS_LINE *current_line = (OUTPLUS_LINE *) sector->first_line;

    while (current_line != NULL)
    {

        if (current_line->next == NULL) return current_line;
        current_line = (OUTPLUS_LINE *) current_line->next;

    }//end :: while

    return NULL;

}//end :: outplus_get_last_sector_line

int outplus_add_line(char *key, char *value, OUTPLUS_SECTOR *sector)
{

    OUTPLUS_LINE *line;
    line = (OUTPLUS_LINE *) malloc(sizeof(OUTPLUS_LINE));

    if (line == NULL) return OUTPLUS_E_ALLOCATION_FAILURE;


    strncpy(line->key, key, OUTPLUS_KEY_MAX_LEN);
    strncpy(line->value, value, OUTPLUS_VALUE_MAX_LEN);

    OUTPLUS_LINE *last_outplus_line;
    last_outplus_line = outplus_get_last_sector_line(sector);

    if (NULL != last_outplus_line)
    {
        last_outplus_line->next = (OUTPLUS_LINE_PTR) line;
        line->prev = (OUTPLUS_LINE_PTR) last_outplus_line;
    } else {
        sector->first_line = line;
    }

    line->next = NULL;

    return OUTPLUS_E_OK;

}//end :: outplus_add_line

//************** TEXT OUTPUT

int outplus_dump_sector_text(OUTPLUS_SECTOR *sector, unsigned int depth) 
{

    OUTPLUS_SECTOR *current = sector;

    int rv = OUTPLUS_E_OK;
    char *tabs = NULL;
    rv = outplus_create_tabs(&tabs, depth);

    if (rv != OUTPLUS_E_OK) return rv;

    while (current != NULL)
    {
        printf("%s%s\n", tabs, current->title);

        OUTPLUS_LINE *current_line = (OUTPLUS_LINE *) current->first_line;

        while (current_line != NULL)
        {

            printf("%s\t%s: %s\n", tabs, current_line->key, current_line->value);
            current_line = (OUTPLUS_LINE *) current_line->next;

        }//end :: while

        printf("\n");

        if (current->child != NULL) outplus_dump_sector_text((OUTPLUS_SECTOR *) current->child, (depth+1)); // Print child by recursion

        current = (OUTPLUS_SECTOR *) current->next;
    }//end :: while

    free(tabs);

    return OUTPLUS_E_OK;

}//end :: outplus_dump_sector_text

int outplus_dump_text(OUTPLUS_SECTOR *sector) 
{

    return outplus_dump_sector_text(sector, 0);


}//end :: outplus_dump_text

//************** JSON OUTPUT

int outplus_dump_sector_json(OUTPLUS_SECTOR *sector, unsigned int depth)
{

    OUTPLUS_SECTOR *current = sector;

    int rv = OUTPLUS_E_OK;
    char *tabs = NULL;
    rv = outplus_create_tabs(&tabs, (depth+1));

    if (rv != OUTPLUS_E_OK) return rv;

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
            current_line = (OUTPLUS_LINE *) current_line->next;

        }//end :: while

        if (current->child != NULL) outplus_dump_sector_json((OUTPLUS_SECTOR *) current->child, (depth+1)); // Print child by recursion

        printf("%s}", tabs);
        if (NULL != current->next) printf(",");
        printf("\n");

        current = (OUTPLUS_SECTOR *) current->next;
    }//end :: while

    free(tabs);

    return OUTPLUS_E_OK;

}//end :: outplus_dump_sector_json

int outplus_dump_json(OUTPLUS_SECTOR *sector)
{

    int rv = OUTPLUS_E_OK;

    printf("{\n");

    rv = outplus_dump_sector_json(sector, 0);

    printf("}\n");

    return rv;

}//end :: outplus_dump_json


//************** CSV OUTPUT

int outplus_dump_sector_csv(OUTPLUS_SECTOR *sector, unsigned int depth)
{
    OUTPLUS_SECTOR *current = sector;

    while (current != NULL)
    {
        OUTPLUS_LINE *current_line = current->first_line;

        while (current_line != NULL)
        {

            printf("\"%s\",\"%s\",\"%s\"\n", current->title, current_line->key, current_line->value);
            current_line = (OUTPLUS_LINE *) current_line->next;

        }//end :: while

        if (current->child != NULL) outplus_dump_sector_csv((OUTPLUS_SECTOR *) current->child, (depth+1)); // Print child by recursion

        current = (OUTPLUS_SECTOR *) current->next;
    }//end :: while

    return OUTPLUS_E_OK;

}//end :: outplus_dump_sector_csv

int outplus_dump_csv(OUTPLUS_SECTOR *sector)
{

    return outplus_dump_sector_csv(sector, 0);

}//end :: outplus_dump_csv

//************** OUTPLUS_HTML OUTPUT

void outplus_html_print_header()
{

    printf(OUTPLUS_HTML_HEAD);
    printf("\t<h1>%s</h1>\n", OUTPLUS_HTML_TITLE);

}//end :: outplus_html_print_header

void outplus_html_print_footer()
{

    printf(OUTPLUS_HTML_FOOTER);

}//end :: outplus_html_print_footer

int outplus_dump_sector_html(OUTPLUS_SECTOR *sector, unsigned int depth)
{

    OUTPLUS_SECTOR *current = sector;

    int rv = OUTPLUS_E_OK;
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
            current_line = (OUTPLUS_LINE *) current_line->next;

            row_count++;

        }//end :: while

        printf("\t</dl>\n");

        if (current->child != NULL) {
            rv = outplus_dump_sector_html((OUTPLUS_SECTOR *) current->child, (depth+1));
            if (rv != OUTPLUS_E_OK) return rv; 
        }//end :: if

        current = (OUTPLUS_SECTOR *) current->next;

    }//end :: while

    return rv;

}//end :: outplus_dump_sector_html

int outplus_dump_html(OUTPLUS_SECTOR *sector)
{

    int rv = OUTPLUS_E_OK;

    outplus_html_print_header();

    rv = outplus_dump_sector_html(sector, 0);

    outplus_html_print_footer();

    return rv;

}//end :: outplus_dump_html

//************** XML OUTPUT

void outplus_xml_print_header()
{

    printf(OUTPLUS_XML_HEAD);

}//end :: outplus_xml_print_header

void outplus_xml_print_footer()
{

    printf(OUTPLUS_XML_FOOTER);

}//end :: outplus_xml_print_footer

int outplus_dump_sector_xml(OUTPLUS_SECTOR *sector, unsigned int depth)
{

    OUTPLUS_SECTOR *current = sector;

    int rv = OUTPLUS_E_OK;
    char *tabs = NULL;
    rv = outplus_create_tabs(&tabs, (depth+1));

    if (rv != OUTPLUS_E_OK) return rv;

    while (NULL != current) {

        char sector_slug[OUTPLUS_TITLE_MAX_LEN];

        strncpy(sector_slug, current->title, OUTPLUS_TITLE_MAX_LEN);
        outplus_slug(sector_slug);

        printf("%s<%s title=\"%s\">\n", tabs, sector_slug, current->title);

        OUTPLUS_LINE *current_line = current->first_line;

        while (current_line != NULL)
        {

            printf("%s\t<%s>%s</%s>\n", tabs, outplus_slug(current_line->key), current_line->value, outplus_slug(current_line->key));
            current_line = (OUTPLUS_LINE *) current_line->next;

        }//end :: while


        if (current->child != NULL) {
            rv = outplus_dump_sector_xml((OUTPLUS_SECTOR *) current->child, (depth+1)); 
            if (rv != OUTPLUS_E_OK) return rv;
        }//end :: if

        printf("%s</%s>\n", tabs, outplus_slug(current->title));

        current = (OUTPLUS_SECTOR *) current->next;
    }//end :: while

    free(tabs); // cleanup!

    return rv;

}//end :: outplus_dump_sector_xml

int outplus_dump_xml(OUTPLUS_SECTOR *sector)
{

    int rv = OUTPLUS_E_OK;

    outplus_xml_print_header();

    rv = outplus_dump_sector_xml(sector, 0);

    outplus_xml_print_footer();

    return rv;

}//end :: outplus_dump_xml


//************** UTIL FUNCTIONS

int outplus_parse_format(const char *optarg)
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
        outplus_format = OUTPLUS_FORMAT_HTML;
    else
        return OUTPLUS_E_INVALID_FORMAT;

    return OUTPLUS_E_OK;

}//end :: outplus_parse_format

int outplus_dump(OUTPLUS_SECTOR *sector) 
{

    int rv = OUTPLUS_E_OK;

    switch (outplus_format)
    {
        case OUTPLUS_FORMAT_XML:
            rv = outplus_dump_xml(sector);
            break;
        case OUTPLUS_FORMAT_CSV:
            rv = outplus_dump_csv(sector);
            break;
        case OUTPLUS_FORMAT_JSON:
            rv = outplus_dump_json(sector);
            break;
        case OUTPLUS_FORMAT_HTML:
            rv = outplus_dump_html(sector);
            break;
        case OUTPLUS_FORMAT_TEXT:
        default:
            rv = outplus_dump_text(sector);
    }//end :: switch

    outplus_free_output(sector);

    return rv;

}//end :: outplus_dump

void outplus_free_output(OUTPLUS_SECTOR *sector) 
{

    OUTPLUS_SECTOR *current = sector;

    while (current != NULL)
    {

        OUTPLUS_LINE *current_line = (OUTPLUS_LINE *) current->first_line;

        while (current_line != NULL)
        {

            OUTPLUS_LINE *temp_line;
            temp_line = current_line;

            current_line = (OUTPLUS_LINE *) current_line->next;

            free(temp_line);

        }//end :: while

        if (current->child != NULL) outplus_free_output((OUTPLUS_SECTOR *) current->child); // Clear child nodes via recursion
        
        OUTPLUS_SECTOR *temp_sector;
        temp_sector = current;
        current = (OUTPLUS_SECTOR *) current->next;
        free(temp_sector);
    }//end :: while


}//end :: outplus_free_output

int outplus_create_tabs(char ** tabs, unsigned int count)
{

    unsigned int i;

    if (count > 0)
    {
        *tabs = malloc(sizeof(char)*(count+1));
    } else {
        *tabs = malloc(sizeof(char));
    }

    if (*tabs == NULL) return OUTPLUS_E_ALLOCATION_FAILURE;

    (*tabs)[0] = '\0';

    for (i=0; i<count; i++) (*tabs)[i] = '\t';

    return OUTPLUS_E_OK;

}//end :: outplus_create_tabs

char * outplus_slug(char *string)
{

    char *cp;
    cp = string;

    while (*cp) {
        if (*cp >= 65 && *cp <= 90) *cp = *cp+32;
        if (*cp < 97 || *cp > 122) *cp = '_';
        /*
        if (64 < *cp && 91 > *cp) *cp = *cp+32;
        if (64 < *cp && 91 > *cp) *cp = *cp+32;
        if (32 == *cp || 45 == *cp) *cp = '_';
        if ('(' == *cp || ')' == *cp || '/' == *cp || '\\' == *cp || '.' == *cp) *cp = '_';
        */

        cp++;
    }//end :: while
    return string;

}//end :: outplus_slug
