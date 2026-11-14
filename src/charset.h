
/* void use_locale(void); */
void use_default_charset(void);

void strip_wchar(CHAR *locale, char *stripped_locale);

void init_multibyte(void);

char* get_iconv_charset(void);
char* get_iconv_output_charset(void);
void set_iconv_charset(char*);
void set_iconv_output_charset(char*);

#ifdef MULTIBYTE
#include <iconv.h>
iconv_t open_output_iconv(void);
#endif

int convert_character(int number, CHAR *out);

