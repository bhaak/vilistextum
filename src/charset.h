
/* void use_locale(); */
void use_default_charset();

void strip_wchar(CHAR *locale, char *stripped_locale);

void init_multibyte();

char* get_iconv_charset();
void set_iconv_charset(char*);

int convert_character(int number, CHAR *out);

