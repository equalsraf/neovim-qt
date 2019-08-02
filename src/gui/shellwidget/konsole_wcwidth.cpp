/* $XFree86: xc/programs/xterm/wcwidth.characters,v 1.9 2006/06/19 00:36:52 dickey Exp $ */

/*
 * This is an implementation of wcwidth() and wcswidth() (defined in
 * IEEE Std 1002.1-2001) for Unicode.
 *
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcwidth.html
 * http://www.opengroup.org/onlinepubs/007904975/functions/wcswidth.html
 *
 * In fixed-width output devices, Latin characters all occupy a single
 * "cell" position of equal width, whereas ideographic CJK characters
 * occupy two such cells. Interoperability between terminal-line
 * applications and (teletype-style) character terminals using the
 * UTF-8 encoding requires agreement on which character should advance
 * the cursor by how many cell positions. No established formal
 * standards exist at present on which Unicode character shall occupy
 * how many cell positions on character terminals. These routines are
 * a first attempt of defining such behavior based on simple rules
 * applied to data provided by the Unicode Consortium.
 *
 * For some graphical characters, the Unicode standard explicitly
 * defines a character-cell width via the definition of the East Asian
 * FullWidth (F), Wide (W), Half-width (H), and Narrow (Na) classes.
 * In all these cases, there is no ambiguity about which width a
 * terminal shall use. For characters in the East Asian Ambiguous (A)
 * class, the width choice depends purely on a preference of backward
 * compatibility with either historic CJK or Western practice.
 * Choosing single-width for these characters is easy to justify as
 * the appropriate long-term solution, as the CJK practice of
 * displaying these characters as double-width comes from historic
 * implementation simplicity (8-bit encoded characters were displayed
 * single-width and 16-bit ones double-width, even for Greek,
 * Cyrillic, etc.) and not any typographic considerations.
 *
 * Much less clear is the choice of width for the Not East Asian
 * (Neutral) class. Existing practice does not dictate a width for any
 * of these characters. It would nevertheless make sense
 * typographically to allocate two character cells to characters such
 * as for instance EM SPACE or VOLUME INTEGRAL, which cannot be
 * represented adequately with a single-width glyph. The following
 * routines at present merely assign a single-cell width to all
 * neutral characters, in the interest of simplicity. This is not
 * entirely satisfactory and should be reconsidered before
 * establishing a formal standard in this area. At the moment, the
 * decision which Not East Asian (Neutral) characters should be
 * represented by double-width glyphs cannot yet be answered by
 * applying a simple rule from the Unicode database content. Setting
 * up a proper standard for the behavior of UTF-8 character terminals
 * will require a careful analysis not only of each Unicode character,
 * but also of each presentation form, something the author of these
 * routines has avoided to do so far.
 *
 * http://www.unicode.org/unicode/reports/tr11/
 *
 * Markus Kuhn -- 2007-05-25 (Unicode 5.0)
 *
 * Permission to use, copy, modify, and distribute this software
 * for any purpose and without fee is hereby granted. The author
 * disclaims all warranties with regard to this software.
 *
 * Latest version: http://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c
 */
/*
 *  Adaptions for KDE by Waldo Bastian <bastian@kde.org> and
 *    Francesco Cecconi <francesco.cecconi@gmail.com>
 *  See COPYING.Unicode for the license for the original wcwidth.c
 */

// Own
#include "konsole_wcwidth.h"

struct interval {
    unsigned long first;
    unsigned long last;
};

/* auxiliary function for binary search in interval table */
static int bisearch(unsigned long ucs, const struct interval* table, int max)
{
    int min = 0;
    int mid;

    if (ucs < table[0].first || ucs > table[max].last)
        return 0;
    while (max >= min) {
        mid = (min + max) / 2;
        if (ucs > table[mid].last)
            min = mid + 1;
        else if (ucs < table[mid].first)
            max = mid - 1;
        else
            return 1;
    }

    return 0;
}

/* The following functions define the column width of an ISO 10646
 * character as follows:
 *
 *    - The null character (U+0000) has a column width of 0.
 *
 *    - Other C0/C1 control characters and DEL will lead to a return
 *      value of -1.
 *
 *    - Non-spacing and enclosing combining characters (general
 *      category code Mn or Me in the Unicode database) have a
 *      column width of 0.
 *
 *    - Other format characters (general category code Cf in the Unicode
 *      database) and ZERO WIDTH SPACE (U+200B) have a column width of 0.
 *
 *    - Hangul Jamo medial vowels and final consonants (U+1160-U+11FF)
 *      have a column width of 0.
 *
 *    - Spacing characters in the East Asian Wide (W) or East Asian
 *      FullWidth (F) category as defined in Unicode Technical
 *      Report #11 have a column width of 2.
 *
 *    - All remaining characters (including all printable
 *      ISO 8859-1 and WGL4 characters, Unicode control characters,
 *      etc.) have a column width of 1.
 *
 * This implementation assumes that quint16 characters are encoded
 * in ISO 10646.
 */

int konsole_wcwidth(uint ucs)
{
    /* sorted list of non-overlapping intervals of non-spacing characters */
    /* generated by "uniset +cat=Me +cat=Mn +cat=Cf -00AD +1160-11FF +200B c" */
    static const struct interval combining[] = {
        { 0x0300, 0x036F }, { 0x0483, 0x0486 }, { 0x0488, 0x0489 },
        { 0x0591, 0x05BD }, { 0x05BF, 0x05BF }, { 0x05C1, 0x05C2 },
        { 0x05C4, 0x05C5 }, { 0x05C7, 0x05C7 }, { 0x0600, 0x0603 },
        { 0x0610, 0x0615 }, { 0x064B, 0x065E }, { 0x0670, 0x0670 },
        { 0x06D6, 0x06E4 }, { 0x06E7, 0x06E8 }, { 0x06EA, 0x06ED },
        { 0x070F, 0x070F }, { 0x0711, 0x0711 }, { 0x0730, 0x074A },
        { 0x07A6, 0x07B0 }, { 0x07EB, 0x07F3 }, { 0x0901, 0x0902 },
        { 0x093C, 0x093C }, { 0x0941, 0x0948 }, { 0x094D, 0x094D },
        { 0x0951, 0x0954 }, { 0x0962, 0x0963 }, { 0x0981, 0x0981 },
        { 0x09BC, 0x09BC }, { 0x09C1, 0x09C4 }, { 0x09CD, 0x09CD },
        { 0x09E2, 0x09E3 }, { 0x0A01, 0x0A02 }, { 0x0A3C, 0x0A3C },
        { 0x0A41, 0x0A42 }, { 0x0A47, 0x0A48 }, { 0x0A4B, 0x0A4D },
        { 0x0A70, 0x0A71 }, { 0x0A81, 0x0A82 }, { 0x0ABC, 0x0ABC },
        { 0x0AC1, 0x0AC5 }, { 0x0AC7, 0x0AC8 }, { 0x0ACD, 0x0ACD },
        { 0x0AE2, 0x0AE3 }, { 0x0B01, 0x0B01 }, { 0x0B3C, 0x0B3C },
        { 0x0B3F, 0x0B3F }, { 0x0B41, 0x0B43 }, { 0x0B4D, 0x0B4D },
        { 0x0B56, 0x0B56 }, { 0x0B82, 0x0B82 }, { 0x0BC0, 0x0BC0 },
        { 0x0BCD, 0x0BCD }, { 0x0C3E, 0x0C40 }, { 0x0C46, 0x0C48 },
        { 0x0C4A, 0x0C4D }, { 0x0C55, 0x0C56 }, { 0x0CBC, 0x0CBC },
        { 0x0CBF, 0x0CBF }, { 0x0CC6, 0x0CC6 }, { 0x0CCC, 0x0CCD },
        { 0x0CE2, 0x0CE3 }, { 0x0D41, 0x0D43 }, { 0x0D4D, 0x0D4D },
        { 0x0DCA, 0x0DCA }, { 0x0DD2, 0x0DD4 }, { 0x0DD6, 0x0DD6 },
        { 0x0E31, 0x0E31 }, { 0x0E34, 0x0E3A }, { 0x0E47, 0x0E4E },
        { 0x0EB1, 0x0EB1 }, { 0x0EB4, 0x0EB9 }, { 0x0EBB, 0x0EBC },
        { 0x0EC8, 0x0ECD }, { 0x0F18, 0x0F19 }, { 0x0F35, 0x0F35 },
        { 0x0F37, 0x0F37 }, { 0x0F39, 0x0F39 }, { 0x0F71, 0x0F7E },
        { 0x0F80, 0x0F84 }, { 0x0F86, 0x0F87 }, { 0x0F90, 0x0F97 },
        { 0x0F99, 0x0FBC }, { 0x0FC6, 0x0FC6 }, { 0x102D, 0x1030 },
        { 0x1032, 0x1032 }, { 0x1036, 0x1037 }, { 0x1039, 0x1039 },
        { 0x1058, 0x1059 }, { 0x1160, 0x11FF }, { 0x135F, 0x135F },
        { 0x1712, 0x1714 }, { 0x1732, 0x1734 }, { 0x1752, 0x1753 },
        { 0x1772, 0x1773 }, { 0x17B4, 0x17B5 }, { 0x17B7, 0x17BD },
        { 0x17C6, 0x17C6 }, { 0x17C9, 0x17D3 }, { 0x17DD, 0x17DD },
        { 0x180B, 0x180D }, { 0x18A9, 0x18A9 }, { 0x1920, 0x1922 },
        { 0x1927, 0x1928 }, { 0x1932, 0x1932 }, { 0x1939, 0x193B },
        { 0x1A17, 0x1A18 }, { 0x1B00, 0x1B03 }, { 0x1B34, 0x1B34 },
        { 0x1B36, 0x1B3A }, { 0x1B3C, 0x1B3C }, { 0x1B42, 0x1B42 },
        { 0x1B6B, 0x1B73 }, { 0x1DC0, 0x1DCA }, { 0x1DFE, 0x1DFF },
        { 0x200B, 0x200F }, { 0x202A, 0x202E }, { 0x2060, 0x2063 },
        { 0x206A, 0x206F }, { 0x20D0, 0x20EF }, { 0x302A, 0x302F },
        { 0x3099, 0x309A }, { 0xA806, 0xA806 }, { 0xA80B, 0xA80B },
        { 0xA825, 0xA826 }, { 0xFB1E, 0xFB1E }, { 0xFE00, 0xFE0F },
        { 0xFE20, 0xFE23 }, { 0xFEFF, 0xFEFF }, { 0xFFF9, 0xFFFB },
        { 0x10A01, 0x10A03 }, { 0x10A05, 0x10A06 }, { 0x10A0C, 0x10A0F },
        { 0x10A38, 0x10A3A }, { 0x10A3F, 0x10A3F }, { 0x1D167, 0x1D169 },
        { 0x1D173, 0x1D182 }, { 0x1D185, 0x1D18B }, { 0x1D1AA, 0x1D1AD },
        { 0x1D242, 0x1D244 }, { 0xE0001, 0xE0001 }, { 0xE0020, 0xE007F },
        { 0xE0100, 0xE01EF }
    };
    static const struct interval emoji_width[] = {
        {0x1f1e6, 0x1f1ff},
        {0x1f321, 0x1f321},
        {0x1f324, 0x1f32c},
        {0x1f336, 0x1f336},
        {0x1f37d, 0x1f37d},
        {0x1f396, 0x1f397},
        {0x1f399, 0x1f39b},
        {0x1f39e, 0x1f39f},
        {0x1f3cb, 0x1f3ce},
        {0x1f3d4, 0x1f3df},
        {0x1f3f3, 0x1f3f5},
        {0x1f3f7, 0x1f3f7},
        {0x1f43f, 0x1f43f},
        {0x1f441, 0x1f441},
        {0x1f4fd, 0x1f4fd},
        {0x1f549, 0x1f54a},
        {0x1f56f, 0x1f570},
        {0x1f573, 0x1f579},
        {0x1f587, 0x1f587},
        {0x1f58a, 0x1f58d},
        {0x1f590, 0x1f590},
        {0x1f5a5, 0x1f5a5},
        {0x1f5a8, 0x1f5a8},
        {0x1f5b1, 0x1f5b2},
        {0x1f5bc, 0x1f5bc},
        {0x1f5c2, 0x1f5c4},
        {0x1f5d1, 0x1f5d3},
        {0x1f5dc, 0x1f5de},
        {0x1f5e1, 0x1f5e1},
        {0x1f5e3, 0x1f5e3},
        {0x1f5e8, 0x1f5e8},
        {0x1f5ef, 0x1f5ef},
        {0x1f5f3, 0x1f5f3},
        {0x1f5fa, 0x1f5fa},
        {0x1f6cb, 0x1f6cf},
        {0x1f6e0, 0x1f6e5},
        {0x1f6e9, 0x1f6e9},
        {0x1f6f0, 0x1f6f0},
        {0x1f6f3, 0x1f6f3},
    };
    static const struct interval doublewidth[] = {
        {0x1100, 0x115f},
        {0x231a, 0x231b},
        {0x2329, 0x232a},
        {0x23e9, 0x23ec},
        {0x23f0, 0x23f0},
        {0x23f3, 0x23f3},
        {0x25fd, 0x25fe},
        {0x2614, 0x2615},
        {0x2648, 0x2653},
        {0x267f, 0x267f},
        {0x2693, 0x2693},
        {0x26a1, 0x26a1},
        {0x26aa, 0x26ab},
        {0x26bd, 0x26be},
        {0x26c4, 0x26c5},
        {0x26ce, 0x26ce},
        {0x26d4, 0x26d4},
        {0x26ea, 0x26ea},
        {0x26f2, 0x26f3},
        {0x26f5, 0x26f5},
        {0x26fa, 0x26fa},
        {0x26fd, 0x26fd},
        {0x2705, 0x2705},
        {0x270a, 0x270b},
        {0x2728, 0x2728},
        {0x274c, 0x274c},
        {0x274e, 0x274e},
        {0x2753, 0x2755},
        {0x2757, 0x2757},
        {0x2795, 0x2797},
        {0x27b0, 0x27b0},
        {0x27bf, 0x27bf},
        {0x2b1b, 0x2b1c},
        {0x2b50, 0x2b50},
        {0x2b55, 0x2b55},
        {0x2e80, 0x2e99},
        {0x2e9b, 0x2ef3},
        {0x2f00, 0x2fd5},
        {0x2ff0, 0x2ffb},
        {0x3000, 0x303e},
        {0x3041, 0x3096},
        {0x3099, 0x30ff},
        {0x3105, 0x312f},
        {0x3131, 0x318e},
        {0x3190, 0x31ba},
        {0x31c0, 0x31e3},
        {0x31f0, 0x321e},
        {0x3220, 0x3247},
        {0x3250, 0x4dbf},
        {0x4e00, 0xa48c},
        {0xa490, 0xa4c6},
        {0xa960, 0xa97c},
        {0xac00, 0xd7a3},
        {0xf900, 0xfaff},
        {0xfe10, 0xfe19},
        {0xfe30, 0xfe52},
        {0xfe54, 0xfe66},
        {0xfe68, 0xfe6b},
        {0xff01, 0xff60},
        {0xffe0, 0xffe6},
        {0x16fe0, 0x16fe3},
        {0x17000, 0x187f7},
        {0x18800, 0x18af2},
        {0x1b000, 0x1b11e},
        {0x1b150, 0x1b152},
        {0x1b164, 0x1b167},
        {0x1b170, 0x1b2fb},
        {0x1f004, 0x1f004},
        {0x1f0cf, 0x1f0cf},
        {0x1f18e, 0x1f18e},
        {0x1f191, 0x1f19a},
        {0x1f200, 0x1f202},
        {0x1f210, 0x1f23b},
        {0x1f240, 0x1f248},
        {0x1f250, 0x1f251},
        {0x1f260, 0x1f265},
        {0x1f300, 0x1f320},
        {0x1f32d, 0x1f335},
        {0x1f337, 0x1f37c},
        {0x1f37e, 0x1f393},
        {0x1f3a0, 0x1f3ca},
        {0x1f3cf, 0x1f3d3},
        {0x1f3e0, 0x1f3f0},
        {0x1f3f4, 0x1f3f4},
        {0x1f3f8, 0x1f43e},
        {0x1f440, 0x1f440},
        {0x1f442, 0x1f4fc},
        {0x1f4ff, 0x1f53d},
        {0x1f54b, 0x1f54e},
        {0x1f550, 0x1f567},
        {0x1f57a, 0x1f57a},
        {0x1f595, 0x1f596},
        {0x1f5a4, 0x1f5a4},
        {0x1f5fb, 0x1f64f},
        {0x1f680, 0x1f6c5},
        {0x1f6cc, 0x1f6cc},
        {0x1f6d0, 0x1f6d2},
        {0x1f6d5, 0x1f6d5},
        {0x1f6eb, 0x1f6ec},
        {0x1f6f4, 0x1f6fa},
        {0x1f7e0, 0x1f7eb},
        {0x1f90d, 0x1f971},
        {0x1f973, 0x1f976},
        {0x1f97a, 0x1f9a2},
        {0x1f9a5, 0x1f9aa},
        {0x1f9ae, 0x1f9ca},
        {0x1f9cd, 0x1f9ff},
        {0x1fa70, 0x1fa73},
        {0x1fa78, 0x1fa7a},
        {0x1fa80, 0x1fa82},
        {0x1fa90, 0x1fa95},
        {0x20000, 0x2fffd},
        {0x30000, 0x3fffd},
    };

    /* test for 8-bit control characters */
    if (ucs == 0)
        return 0;
    if (ucs < 32 || (ucs >= 0x7f && ucs < 0xa0))
        return -1;

    /* binary search in table of non-spacing characters */
    if (bisearch(ucs, combining,
                 sizeof(combining) / sizeof(struct interval) - 1))
        return 0;

    /* binary search for known wide characters */
    if (bisearch(ucs, doublewidth,
                sizeof(doublewidth) / sizeof(struct interval) - 1))
        return 2;

    /* binary search for wide emoji */
    if (bisearch(ucs, emoji_width,
                 sizeof(emoji_width) / sizeof(struct interval) - 1))
        return 2;

    return 1;
}

int string_width(const QString& text)
{
    int w = 0;
    for (int i = 0; i < text.length(); ++i)
        w += konsole_wcwidth(text[i].unicode());
    return w;
}

