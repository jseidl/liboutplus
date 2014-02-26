/*
    liboutplus - C structured output library

    outplus_test.c - test outplus library functions

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

#include <stdio.h>
#include "outplus.h"

int main(int argc, char *argv[])
{


    if (argc != 2) {
        printf("Usage %s <output type>", argv[0]);
        return 1;
    }//end :: if

    OUTPLUS_SECTOR *output = NULL;
    int rv;
    
    OUTPLUS_SECTOR *output1 = NULL;
    rv = outplus_add_sector("Sector One", &output1, &output);
    if (rv != OUTPLUS_E_OK) return 1;

    outplus_add_line("Key One-One", "Value One-One", output);
    outplus_add_line("Key One-Two", "Value One-Two", output);


    OUTPLUS_SECTOR *output2 = NULL;
    rv = outplus_add_sector("Sector Two", &output2, &output);
    if (rv != OUTPLUS_E_OK) return 1;

    outplus_add_line("Key Two-One", "Value Two-One", output2);

    OUTPLUS_SECTOR *output2c = NULL;
    rv = outplus_add_child_sector("Sector Two One", &output2c, &output2);
    if (rv != OUTPLUS_E_OK) return 1;

    outplus_add_line("Key Two-One-One", "Value Two-One-One", output2c);

    OUTPLUS_SECTOR *output3 = NULL;
    rv = outplus_add_sector("Sector Three", &output3, &output);
    if (rv != OUTPLUS_E_OK) return 1;
    outplus_add_line("Key Three", "Value Three", output3);

    //outplus_dump_xml(output);
    outplus_parse_format(argv[1]);
    outplus_dump(output);

    return 0;

}
