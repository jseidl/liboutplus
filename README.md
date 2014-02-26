liboutplus
==========

C library for structured output in many formats as text, csv, json, xml, html etc 

[![Build Status](https://travis-ci.org/jseidl/liboutplus.png)](https://travis-ci.org/jseidl/liboutplus)


Compinling
-------------

Building the lib

```
gcc -c outplus.c -o outplus.o
```

Compiling your code with outplus

```
 gcc -Wall -o your_program outplus.o your_program.c -I<path to lib outplus>
```


How to use
-------------

Simple example:

```c
#include <stdio.h>
#include "outplus.h"

int main(int argc, char *argv[])
{

    if (argc != 2) {
        printf("Usage %s <output type>", argv[0]);
        return 1;
    }//end :: if


    int rv = OUTPLUS_E_OK; // our return values
    OUTPLUS_SECTOR *output = NULL; // Declare your global output sector

    // Create a output sector and append to global output. Declare and initialize the sector
    OUTPLUS_SECTOR *output1 = NULL;
    rv = outplus_add_sector("Sector One", &output1, &output);
    if (rv != OUTPLUS_E_OK) return 1; // outplus functions must return OUTPLUS_E_OK or they failed

    // Add text to it!
    outplus_add_line("Key One Sector One", "Value One Sector One", output1);
    if (rv != OUTPLUS_E_OK) return 1;

    // Add a second sector
    OUTPLUS_SECTOR *output2 = NULL;
    rv = outplus_add_sector("Sector Two", &output2, &output);
    if (rv != OUTPLUS_E_OK) return 1;  

    // Add text to the second sector
    outplus_add_line("Key One Sector Two", "Value One Sector Two", output2);
    if (rv != OUTPLUS_E_OK) return 1;


    // You can also add child sector to sector two
    OUTPLUS_SECTOR *output2c = NULL;
    rv = outplus_add_child_sector("Sector Two One", &output2c, &output2);
    if (rv != OUTPLUS_E_OK) return 1;

    // Add lines to child sector
    rv = outplus_add_line("Key One Sector Two Sub-Sector One", "Value One Sector Two Sub-Sector One", output2c);
    if (rv != OUTPLUS_E_OK) return 1;

    // Finally use the appropriate functions to dump your output into the desired format

    /* Directly calling a format dump

        void outplus_dump_xml(output);
        void outplus_dump_text(output);
        void outplus_dump_csv(output);
        void outplus_dump_html(output);
        void outplus_dump_json(output);

    */

    // if using above, remember to call outplus_free(output);

    outplus_parse_format(argv[1]);
    outplus_dump(output); // this will call outplus_free(output) for you

    return 0;

}//end :: main
```

### TODO ###
* Add file output (printf->outplus_output)
* Add some more examples here
