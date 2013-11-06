liboutplus
==========

C library for structured output in many formats as text, csv, json, xml, html etc


How to use
-------------
*Declare your output holder*
    OUTPLUS_SECTOR *output = NULL;

*Create a output block (sector) and append to global output*
    OUTPLUS_SECTOR *outSectorSections = outplus_add_sector("Sections", &output);


*Add text to it!*
    char s[OUTPLUS_VALUE_MAX_LEN]; // string holder
    char strName = "Some Text" // example text
    snprintf(s, OUTPLUS_VALUE_MAX_LEN, "%s", strName);
    outplus_add_line("Name", s, outSubSectSection);


*You can also add child blocks (sectors) *
    OUTPLUS_SECTOR *outSubSectSection = outplus_add_child_sector("Section", &outSectorSections);


*Finally use the appropriate functions to dump your output into the desired format*
    void outplus_dump_sector_xml(OUTPLUS_SECTOR *sector, unsigned int depth);
    void outplus_dump_sector_text(OUTPLUS_SECTOR *sector, unsigned int depth);
    void outplus_dump_sector_csv(OUTPLUS_SECTOR *sector, unsigned int depth);
    void outplus_dump_sector_html(OUTPLUS_SECTOR *sector, unsigned int depth);
    void outplus_dump_sector_json(OUTPLUS_SECTOR *sector, unsigned int depth);



TODO
--------
* Add file output (printf->outplus_output)
