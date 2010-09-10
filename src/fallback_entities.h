#ifndef fallback_entities_h
#define fallback_entities_h       1

int unicode_entity(CHAR *s);
int ligature_entity(CHAR *s);
int fallback_entity(CHAR *s);

struct fallback_entities_struct {
	int codepoint;
	char *description;
	char *replacement;
};

struct fallback_entities_struct fallback_entities[] = {
/* Spacing Modifier Letters */

/* General punctation */
{0x2018, "LEFT SINGLE QUOTATION MARK",  STRING("'")},
{0x2019, "RIGHT SINGLE QUOTATION MARK", STRING("'")},
{0x201C, "LEFT DOUBLE QUOTATION MARK",  STRING("\"")},
{0x201D, "RIGHT DOUBLE QUOTATION MARK", STRING("\"")},
{0x2024, "ONE DOT LEADER", STRING(".")},
{0x2025, "TWO DOT LEADER", STRING("..")},
{0x2026, "HORIZONTAL ELLIPSIS", STRING("...")},

{0x2032, "PRIME", STRING("'")},
{0x2033, "DOUBLE PRIME", STRING("''")},
{0x2034, "TRIPLE PRIME", STRING("'''")},
{0x2035, "REVERSED PRIME", STRING("`")},
{0x2036, "REVERSED DOUBLE PRIME", STRING("``")},
{0x2037, "REVERSED TRIPLE PRIME", STRING("```")},
{0x2038, "CARET", STRING("^")},
{0x2039, "SINGLE LEFT-POINTING ANGLE QUOTATION MARK", STRING("<")},
{0x203A, "SINGLE RIGHT-POINTING ANGLE QUOTATION MARK", STRING(">")},

{0x2044, "FRACTION SLASH", STRING("/")},

/* @@      2150    Number Forms    218F */
/* @               Fractions */
{0x2150, "VULGAR FRACTION ONE SEVENTH", STRING("1/7")},
{0x2151, "VULGAR FRACTION ONE NINTH",   STRING("1/9")},
{0x2152, "VULGAR FRACTION ONE TENTH",   STRING("1/10")},

{0x2153, "VULGAR FRACTION ONE THIRD",     STRING("1/3")},
{0x2154, "VULGAR FRACTION TWO THIRDS",    STRING("2/3")},
{0x2155, "VULGAR FRACTION ONE FIFTH",     STRING("1/5")},
{0x2156, "VULGAR FRACTION TWO FIFTHS",    STRING("2/5")},
{0x2157, "VULGAR FRACTION THREE FIFTHS",  STRING("3/5")},
{0x2158, "VULGAR FRACTION FOUR FIFTHS",   STRING("4/5")},
{0x2159, "VULGAR FRACTION ONE SIXTH",     STRING("1/6")},
{0x215A, "VULGAR FRACTION FIVE SIXTHS",   STRING("5/6")},
{0x215B, "VULGAR FRACTION ONE EIGHTH",    STRING("1/8")},
{0x215C, "VULGAR FRACTION THREE EIGHTHS", STRING("3/8")},
{0x215D, "VULGAR FRACTION FIVE EIGHTHS",  STRING("5/8")},
{0x215E, "VULGAR FRACTION SEVEN EIGHTHS", STRING("7/8")},

{0x215F, "FRACTION NUMERATOR ONE",        STRING("1/")},
{0x2189, "VULGAR FRACTION ZERO THIRDS",   STRING("0/3")},

{0x2CFD, "COPTIC FRACTION ONE HALF",      STRING("1/2")},

{0x0D73, "MALAYALAM FRACTION ONE QUARTER",    STRING("1/4")},
{0x0D74, "MALAYALAM FRACTION ONE HALF",       STRING("1/2")},
{0x0D75, "MALAYALAM FRACTION THREE QUARTERS", STRING("3/4")},

{0xA830, "NORTH INDIC FRACTION ONE QUARTER", STRING("1/4")},
{0xA831, "NORTH INDIC FRACTION ONE HALF", STRING("1/2")},
{0xA832, "NORTH INDIC FRACTION THREE QUARTERS", STRING("3/4")},
{0xA833, "NORTH INDIC FRACTION ONE SIXTEENTH", STRING("1/16")},
{0xA834, "NORTH INDIC FRACTION ONE EIGHTH", STRING("1/8")},
{0xA835, "NORTH INDIC FRACTION THREE SIXTEENTHS", STRING("3/16")},

{0x10E7B, "RUMI FRACTION ONE HALF", STRING("1/2")},
{0x10E7C, "RUMI FRACTION ONE QUARTER", STRING("1/4")},
{0x10E7D, "RUMI FRACTION ONE THIRD", STRING("1/3")},
{0x10E7E, "RUMI FRACTION TWO THIRDS", STRING("2/3")},


/* Roman numerals */
{0x2160, "ROMAN NUMERAL ONE",                STRING("I")},
{0x2161, "ROMAN NUMERAL TWO",                STRING("II")},
{0x2162, "ROMAN NUMERAL THREE",              STRING("III")},
{0x2163, "ROMAN NUMERAL FOUR",               STRING("IV")},
{0x2164, "ROMAN NUMERAL FIVE",               STRING("V")},
{0x2165, "ROMAN NUMERAL SIX",                STRING("VI")},
{0x2166, "ROMAN NUMERAL SEVEN",              STRING("VII")},
{0x2167, "ROMAN NUMERAL EIGHT",              STRING("VIII")},
{0x2168, "ROMAN NUMERAL NINE",               STRING("IX")},
{0x2169, "ROMAN NUMERAL TEN",                STRING("X")},
{0x216A, "ROMAN NUMERAL ELEVEN",             STRING("XI")},
{0x216B, "ROMAN NUMERAL TWELVE",             STRING("XII")},
{0x216C, "ROMAN NUMERAL FIFTY",              STRING("L")},
{0x216D, "ROMAN NUMERAL ONE HUNDRED",        STRING("C")},
{0x216E, "ROMAN NUMERAL FIVE HUNDRED",       STRING("D")},
{0x216F, "ROMAN NUMERAL ONE THOUSAND",       STRING("M")},
{0x2170, "SMALL ROMAN NUMERAL ONE",          STRING("i")},
{0x2171, "SMALL ROMAN NUMERAL TWO",          STRING("ii")},
{0x2172, "SMALL ROMAN NUMERAL THREE",        STRING("iii")},
{0x2173, "SMALL ROMAN NUMERAL FOUR",         STRING("iv")},
{0x2174, "SMALL ROMAN NUMERAL FIVE",         STRING("v")},
{0x2175, "SMALL ROMAN NUMERAL SIX",          STRING("vi")},
{0x2176, "SMALL ROMAN NUMERAL SEVEN",        STRING("vii")},
{0x2177, "SMALL ROMAN NUMERAL EIGHT",        STRING("viii")},
{0x2178, "SMALL ROMAN NUMERAL NINE",         STRING("ix")},
{0x2179, "SMALL ROMAN NUMERAL TEN",          STRING("x")},
{0x217A, "SMALL ROMAN NUMERAL ELEVEN",       STRING("xi")},
{0x217B, "SMALL ROMAN NUMERAL TWELVE",       STRING("xii")},
{0x217C, "SMALL ROMAN NUMERAL FIFTY",        STRING("l")},
{0x217D, "SMALL ROMAN NUMERAL ONE HUNDRED",  STRING("c")},
{0x217E, "SMALL ROMAN NUMERAL FIVE HUNDRED", STRING("d")},
{0x217F, "SMALL ROMAN NUMERAL ONE THOUSAND", STRING("m")},

/* Addition for German typography */
{0x1E9E, "LATIN CAPITAL LETTER SHARP S", STRING("SS")},

{0, "", STRING("")}
};

#endif
