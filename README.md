liboutplus
==========

C library for structured output in many formats as text, csv, json, xml, html etc 

[![Build Status](https://travis-ci.org/liboutplus/liboutplus.png)](https://travis-ci.org/jseidl/liboutplus)


How to use
-------------
Declare your output holder

```c
OUTPLUS_SECTOR *output = NULL;
```

Create a output block (sector) and append to global output

OUTPLUS_SECTOR *outSectorSections = outplus_add_sector("Samples", &output);


Add text to it!

```c
char s[OUTPLUS_VALUE_MAX_LEN]; // string holder
char strName = "Sample value" // example text
snprintf(s, OUTPLUS_VALUE_MAX_LEN, "%s", strName);
outplus_add_line("Sample item", s, outSectorSections);
```


You can also add child blocks (sectors)

```c
OUTPLUS_SECTOR *outSubSectSection = outplus_add_child_sector("Section", &outSectorSections);
```

And add more lines to it

```c
char strName = "Another sample value" // example text
snprintf(s, OUTPLUS_VALUE_MAX_LEN, "%s", strName);
outplus_add_line("Another sample item", s, outSubSectSection);
```


Finally use the appropriate functions to dump your output into the desired format

```c
void outplus_dump_sector_xml(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_text(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_csv(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_html(OUTPLUS_SECTOR *sector, unsigned int depth);
void outplus_dump_sector_json(OUTPLUS_SECTOR *sector, unsigned int depth);
```


### TODO ###
* Add file output (printf->outplus_output)

