#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <ctype.h>
#include <string.h>

#include "ConvertUTF.h"
#include "smutil.h"

const bool mux_isdigit[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

const bool mux_isspace[256] =
{
//  0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F
//
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0,  // 0
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 1
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 2
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 3
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 4
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 5
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 6
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 7

    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 8
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // 9
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // A
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // B
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // C
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // D
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // E
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // F
};

#define mux_isdigit(x) (mux_isdigit[(unsigned char)(x)])
#define mux_isspace(x) (mux_isspace[(unsigned char)(x)])

const char TableATOI[16][10] =
{
    {  0,  1,  2,  3,  4,  5,  6,  7,  8,  9},
    { 10, 11, 12, 13, 14, 15, 16, 17, 18, 19},
    { 20, 21, 22, 23, 24, 25, 26, 27, 28, 29},
    { 30, 31, 32, 33, 34, 35, 36, 37, 38, 39},
    { 40, 41, 42, 43, 44, 45, 46, 47, 48, 49},
    { 50, 51, 52, 53, 54, 55, 56, 57, 58, 59},
    { 60, 61, 62, 63, 64, 65, 66, 67, 68, 69},
    { 70, 71, 72, 73, 74, 75, 76, 77, 78, 79},
    { 80, 81, 82, 83, 84, 85, 86, 87, 88, 89},
    { 90, 91, 92, 93, 94, 95, 96, 97, 98, 99}
};

long mux_atol(const char *pString)
{
    long sum = 0;
    int LeadingCharacter = 0;

    // Convert ASCII digits
    //
    unsigned int c1;
    unsigned int c0 = pString[0];
    if (!mux_isdigit(c0))
    {
        while (mux_isspace(pString[0]))
        {
            pString++;
        }
        LeadingCharacter = pString[0];
        if (  LeadingCharacter == '-'
           || LeadingCharacter == '+')
        {
            pString++;
        }
        c0 = pString[0];
        if (!mux_isdigit(c0))
        {
            return 0;
        }
    }

    do
    {
        c1 = pString[1];
        if (mux_isdigit(c1))
        {
            sum = 100 * sum + TableATOI[c0-'0'][c1-'0'];
            pString += 2;
        }
        else
        {
            sum = 10 * sum + (c0-'0');
            break;
        }
    } while (mux_isdigit(c0 = pString[0]));

    // Interpret sign
    //
    if (LeadingCharacter == '-')
    {
        sum = -sum;
    }
    return sum;
}

// Korean Hangul Constants.
//
const int SBase = 0xAC00;
const int LBase = 0x1100;
const int VBase = 0x1161;
const int TBase = 0x11A7;
const int SCount = 11172;
const int LCount = 19;
const int VCount = 21;
const int TCount = 28;
const int NCount = VCount * TCount;

const size_t codepoints = 1114109;

#define CATEGORY_LETTER               0x0001000 
#define SUBCATEGORY_UPPER             0x0000001   // Lu
#define SUBCATEGORY_LOWER             0x0000002   // Ll
#define SUBCATEGORY_TITLE             0x0000004   // Lt
#define SUBCATEGORY_MODIFIER          0x0000004   // Lm
#define SUBCATEGORY_LET_OTHER         0x0000008   // Lo

#define CATEGORY_MARK                 0x0002000
#define SUBCATEGORY_NONSPACING        0x0000001   // Mn
#define SUBCATEGORY_SPACING_COMBINING 0x0000002   // Mc
#define SUBCATEGORY_SPACING_ENCLOSING 0x0000004   // Me

#define CATEGORY_NUMBER               0x0004000
#define SUBCATEGORY_DECIMAL_DIGIT     0x0000001   // Nd
#define SUBCATEGORY_LETTER            0x0000002   // Nl
#define SUBCATEGORY_NUM_OTHER         0x0000004   // No

#define CATEGORY_PUNCTUATION          0x0008000
#define SUBCATEGORY_CONNECTOR         0x0000001   // Pc
#define SUBCATEGORY_DASH              0x0000002   // Pd
#define SUBCATEGORY_OPEN              0x0000004   // Ps
#define SUBCATEGORY_CLOSE             0x0000008   // Pe
#define SUBCATEGORY_INITIAL_QUOTE     0x0000010   // Pi
#define SUBCATEGORY_FINAL_QUOTE       0x0000020   // Pf
#define SUBCATEGORY_PUNC_OTHER        0x0000040   // Po

#define CATEGORY_SYMBOL               0x0010000
#define SUBCATEGORY_MATH              0x0000001   // Sm
#define SUBCATEGORY_CURRENCY          0x0000002   // Sc
#define SUBCATEGORY_MODIFIER          0x0000004   // Sk
#define SUBCATEGORY_SYM_OTHER         0x0000008   // So

#define CATEGORY_SEPARATOR            0x0020000 
#define SUBCATEGORY_SPACE             0x0000001   // Zs
#define SUBCATEGORY_LINE              0x0000002   // Zl
#define SUBCATEGORY_PARAGRAPH         0x0000004   // Zp

#define CATEGORY_OTHER                0x0040000 
#define SUBCATEGORY_CONTROL           0x0000001   // Cc
#define SUBCATEGORY_FORMAT            0x0000002   // Cf
#define SUBCATEGORY_SURROGATE         0x0000004   // Cs
#define SUBCATEGORY_PRIVATE_USE       0x0000008   // Co
#define SUBCATEGORY_NOT_ASSIGNED      0x0000010   // Cn

static struct
{
    int   cat;
    char *catlet;
} CategoryTable[] = 
{
    { CATEGORY_LETTER|SUBCATEGORY_UPPER,              "Lu" },
    { CATEGORY_LETTER|SUBCATEGORY_LOWER,              "Ll" },
    { CATEGORY_LETTER|SUBCATEGORY_TITLE,              "Lt" },
    { CATEGORY_LETTER|SUBCATEGORY_MODIFIER,           "Lm" },
    { CATEGORY_LETTER|SUBCATEGORY_LET_OTHER,          "Lo" },
    { CATEGORY_MARK|SUBCATEGORY_NONSPACING,           "Mn" },
    { CATEGORY_MARK|SUBCATEGORY_SPACING_COMBINING,    "Mc" },
    { CATEGORY_MARK|SUBCATEGORY_SPACING_ENCLOSING,    "Me" },
    { CATEGORY_NUMBER|SUBCATEGORY_DECIMAL_DIGIT,      "Nd" },
    { CATEGORY_NUMBER|SUBCATEGORY_LETTER,             "Nl" },
    { CATEGORY_NUMBER|SUBCATEGORY_NUM_OTHER,          "No" },
    { CATEGORY_PUNCTUATION|SUBCATEGORY_CONNECTOR,     "Pc" },
    { CATEGORY_PUNCTUATION|SUBCATEGORY_DASH,          "Pd" },
    { CATEGORY_PUNCTUATION|SUBCATEGORY_OPEN,          "Ps" },
    { CATEGORY_PUNCTUATION|SUBCATEGORY_CLOSE,         "Pe" },
    { CATEGORY_PUNCTUATION|SUBCATEGORY_INITIAL_QUOTE, "Pi" },
    { CATEGORY_PUNCTUATION|SUBCATEGORY_FINAL_QUOTE,   "Pf" },
    { CATEGORY_PUNCTUATION|SUBCATEGORY_PUNC_OTHER,    "Po" },
    { CATEGORY_SYMBOL|SUBCATEGORY_MATH,               "Sm" },
    { CATEGORY_SYMBOL|SUBCATEGORY_CURRENCY,           "Sc" },
    { CATEGORY_SYMBOL|SUBCATEGORY_MODIFIER,           "Sk" },
    { CATEGORY_SYMBOL|SUBCATEGORY_SYM_OTHER,          "So" },
    { CATEGORY_SEPARATOR|SUBCATEGORY_SPACE,           "Zs" },
    { CATEGORY_SEPARATOR|SUBCATEGORY_LINE,            "Zl" },
    { CATEGORY_SEPARATOR|SUBCATEGORY_PARAGRAPH,       "Zp" },
    { CATEGORY_OTHER|SUBCATEGORY_CONTROL,             "Cc" },
    { CATEGORY_OTHER|SUBCATEGORY_FORMAT,              "Cf" },
    { CATEGORY_OTHER|SUBCATEGORY_SURROGATE,           "Cs" },
    { CATEGORY_OTHER|SUBCATEGORY_PRIVATE_USE,         "Co" },
    { 0, NULL }
};

#define BIDI_LEFT_TO_RIGHT               1
#define BIDI_LEFT_TO_RIGHT_EMBEDDING     2
#define BIDI_LEFT_TO_RIGHT_OVERRIDE      3
#define BIDI_RIGHT_TO_LEFT               4
#define BIDI_RIGHT_TO_LEFT_ARABIC        5
#define BIDI_RIGHT_TO_LEFT_EMBEDDING     6
#define BIDI_RIGHT_TO_LEFT_OVERRIDE      7
#define BIDI_POP_DIRECTIONAL_FORMAT      8
#define BIDI_EUROPEAN_NUMBER             9
#define BIDI_EUROPEAN_NUMBER_SEPARATOR  10
#define BIDI_EUROPEAN_NUMBER_TERMINATOR 11
#define BIDI_ARABIC_NUMBER              12
#define BIDI_COMMON_NUMBER_SEPARATOR    13
#define BIDI_NON_SPACING_MARK           14
#define BIDI_BOUNDARY_NEUTRAL           15
#define BIDI_PARAGRAPH_SEPARATOR        16
#define BIDI_SEGMENT_SEPARATOR          17
#define BIDI_WHITESPACE                 18
#define BIDI_OTHER_NEUTRALS             19

static struct
{
    int   BiDi;
    char *BiDiLet;
} BiDiTable[] =
{
    { BIDI_LEFT_TO_RIGHT,              "L"   },
    { BIDI_LEFT_TO_RIGHT_EMBEDDING,    "LRE" },
    { BIDI_LEFT_TO_RIGHT_OVERRIDE,     "LRO" },
    { BIDI_RIGHT_TO_LEFT,              "R"   },
    { BIDI_RIGHT_TO_LEFT_ARABIC,       "AL"  },
    { BIDI_RIGHT_TO_LEFT_EMBEDDING,    "RLE" },
    { BIDI_RIGHT_TO_LEFT_OVERRIDE,     "RLO" },
    { BIDI_POP_DIRECTIONAL_FORMAT,     "PDF" },
    { BIDI_EUROPEAN_NUMBER,            "EN"  },
    { BIDI_EUROPEAN_NUMBER_SEPARATOR,  "ES"  },
    { BIDI_EUROPEAN_NUMBER_TERMINATOR, "ET"  },
    { BIDI_ARABIC_NUMBER,              "AN"  },
    { BIDI_COMMON_NUMBER_SEPARATOR,    "CS"  },
    { BIDI_NON_SPACING_MARK,           "NSM" },
    { BIDI_BOUNDARY_NEUTRAL,           "BN"  },
    { BIDI_PARAGRAPH_SEPARATOR,        "B"   },
    { BIDI_SEGMENT_SEPARATOR,          "S"   },
    { BIDI_WHITESPACE,                 "WS"  },
    { BIDI_OTHER_NEUTRALS,             "ON"  },
    { 0, NULL }
};

class CodePoint
{
public:
    CodePoint();
    ~CodePoint();

    bool IsDefined(void) { return m_bDefined; };

    void SetDescription(char *pDescription);
    char *GetDescription(void) { return m_pDescription; };

    void SetCategory(int category) { m_category = category; };
    int  GetCategory(void);
    char *GetCategoryName(void);

    void SetCombiningClass(int cc) { m_class = cc; };
    int  GetCombiningClass(void) { return m_class; };

    void SetBiDi(int bidi) { m_bidi = bidi; };
    int  GetBiDi(void) { return m_bidi; };
    char *GetBiDiName(void);

private:
    bool  m_bDefined;
    char *m_pDescription;
    int   m_category;
    int   m_class;
    int   m_bidi;
};

CodePoint::CodePoint()
{
    m_bDefined = false;
    m_pDescription = NULL;
}

CodePoint::~CodePoint()
{
    if (NULL != m_pDescription)
    {
        delete [] m_pDescription;
        m_pDescription = NULL;
    }
    m_bDefined = false;
}

void CodePoint::SetDescription(char *pDescription)
{
    if (NULL != m_pDescription)
    {
        delete [] m_pDescription;
        m_pDescription = NULL;
    }
    size_t n = strlen(pDescription);
    m_pDescription = new char[n+1];
    memcpy(m_pDescription, pDescription, n+1);
    m_bDefined = true;
}

class UniData
{
public:
    UniData() {};
    ~UniData() {};

    void LoadUnicodeDataFile(void);
    void LoadUnicodeDataLine(UTF32 codepoint, int nFields, char *aFields[]);
    void LoadUnicodeHanFile(void);

    void SaveMasterFile(void);

private:
    CodePoint cp[codepoints];
};

UniData *g_UniData = NULL;

int main(int argc, char *argv[])
{
    if (1 != argc)
    {
        printf("Usage: buildFiles uses UnicodeData.txt\n");
        return 0;
    }

    g_UniData = new UniData;
    g_UniData->LoadUnicodeDataFile();
    g_UniData->LoadUnicodeHanFile();

    g_UniData->SaveMasterFile();
    return 0;
}

void UniData::LoadUnicodeDataFile(void)
{
    FILE *fp = fopen("UnicodeData.txt", "r");
    if (NULL != fp)
    {
        char buffer1[1024];
        char buffer2[1024];
        while (NULL != ReadLine(fp, buffer1, sizeof(buffer1)))
        {
            bool  bRange = false;
            bool  bRangeValid = false;

            int   nFields1, nFields2;
            char *aFields1[15], *aFields2[15];

            ParseFields(buffer1, 15, nFields1, aFields1);
            if (  2 <= nFields1
               && aFields1[1][0] == '<')
            {
                size_t n1 = strlen(aFields1[1]);
                if (  9 <= n1
                   && strcmp(&aFields1[1][n1-8], ", First>") == 0)
                {
                    bRange = true;
                    if (NULL != ReadLine(fp, buffer2, sizeof(buffer2)))
                    {
                        ParseFields(buffer2, 15, nFields2, aFields2);
                        if (  2 <= nFields2
                           && aFields2[1][0] == '<')
                        {
                            size_t n2 = strlen(aFields2[1]);
                            if (  8 <= n2
                               && strcmp(&aFields2[1][n2-7], ", Last>") == 0)
                            {
                                aFields1[1][n1-8] = '\0';
                                aFields1[1]++;
                                aFields2[1][n2-7] = '\0';
                                aFields2[1]++;

                                bRangeValid = true;
                            }
                        }
                    }
                }
            }

            if (!bRange)
            {
                // Single Code Point.
                //
                UTF32 pt = DecodeCodePoint(aFields1[0]);
                LoadUnicodeDataLine(pt, nFields1, aFields1);
            }
            else if (bRangeValid)
            {
                // Range of Code Points.
                //
                bool bMatches = false;
                if (nFields1 == nFields2)
                {
                    bMatches = true;
                    for (int i = 1; i < nFields1; i++)
                    {
                        if (strcmp(aFields1[i], aFields2[i]) != 0)
                        {
                            bMatches = false;
                        }
                    }
                }

                if (bMatches)
                {
                    //printf("%s - %s\n", aFields1[0], aFields2[0]);
                    UTF32 pt1 = DecodeCodePoint(aFields1[0]);
                    UTF32 pt2 = DecodeCodePoint(aFields2[0]);
                    if (pt1 != SBase)
                    {
                        // non-Hangul range.
                        //
                        for (UTF32 pt = pt1; pt <= pt2; pt++)
                        {
                            LoadUnicodeDataLine(pt, nFields1, aFields1);
                        }
                    }
                    else
                    {
                        // Korean Hangul range.
                        //
                        for (UTF32 pt = pt1; pt <= pt2; pt++)
                        {
                            UTF32 SIndex = pt - SBase;
                            UTF32 L = LBase + SIndex / NCount;
                            UTF32 V = VBase + (SIndex % NCount) / TCount;
                            UTF32 T = TBase + SIndex % TCount;

                            char Hangul[1024];
                            if (T == TBase)
                            {
                                sprintf(Hangul, "%04X %04X", L, V);
                            }
                            else
                            {
                                sprintf(Hangul, "%04X %04X %04X", L, V, T);
                            }
                            aFields1[5] = Hangul;
                            LoadUnicodeDataLine(pt, nFields1, aFields1);
                        }
                    }
                }
                else
                {
                    printf("***ERROR: %s in range form does not agree with %s\n", aFields1[0], aFields2[0]);
                }
            }
            else
            {
                printf("***ERROR: Invalid Range form near %s\n", aFields1[0]);
            }
        }
        fclose(fp);
    }
}

void UniData::LoadUnicodeDataLine(UTF32 codepoint, int nFields, char *aFields[])
{
    if (codepoint <= codepoints)
    {
        // Description
        //
        if (2 < nFields)
        {
            cp[codepoint].SetDescription(aFields[1]);
        }

        // Category
        //
        if (3 < nFields)
        {
            bool bValid = false;
            int i = 0;
            while (CategoryTable[i].catlet)
            {
                if (strcmp(aFields[2], CategoryTable[i].catlet) == 0)
                {
                    cp[codepoint].SetCategory(CategoryTable[i].cat);
                    bValid = true;
                    break;
                }
                i++;
            }

            if (!bValid)
            {
                printf("***ERROR: Invalid Category %s for U+%04X\n", aFields[2], codepoint);
                exit(0);
            }
        }

        // Combining Class.
        //
        if (4 < nFields)
        {
            int cc = mux_atol(aFields[3]);
            cp[codepoint].SetCombiningClass(cc);
        }

        // Bidi algorithm.
        //
        if (5 < nFields)
        {
            bool bValid = false;
            int i = 0;
            while (BiDiTable[i].BiDiLet)
            {
                if (strcmp(aFields[4], BiDiTable[i].BiDiLet) == 0)
                {
                    cp[codepoint].SetBiDi(BiDiTable[i].BiDi);
                    bValid = true;
                    break;
                }
                i++;
            }

            if (!bValid)
            {
                printf("***ERROR: Invalid BiDi %s for U+%04X\n", aFields[4], codepoint);
                exit(0);
            }
        }
    }
}

void UniData::SaveMasterFile(void)
{
    for (UTF32 pt = 0; pt <= codepoints; pt++)
    {
        if (cp[pt].IsDefined())
        {
            printf("%04X;%s;%s;%d;%s\n", pt, cp[pt].GetDescription(), cp[pt].GetCategoryName(),
                cp[pt].GetCombiningClass(), cp[pt].GetBiDiName());
        }
    }
}

char *CodePoint::GetCategoryName(void)
{
    int i = 0;
    while (CategoryTable[i].catlet)
    {
        if ((CategoryTable[i].cat & m_category) == m_category)
        {
            return CategoryTable[i].catlet;
        }
        i++;
    }
    return NULL;
}

char *CodePoint::GetBiDiName(void)
{
    int i = 0;
    while (BiDiTable[i].BiDiLet)
    {
        if ((BiDiTable[i].BiDi & m_bidi) == m_bidi)
        {
            return BiDiTable[i].BiDiLet;
        }
        i++;
    }
    return NULL;
}

void UniData::LoadUnicodeHanFile(void)
{
    FILE *fp = fopen("UnicodeHan.txt", "r");
    if (NULL != fp)
    {
        char buffer[1024];
        while (NULL != ReadLine(fp, buffer, sizeof(buffer)))
        {
            bool  bValid = false;
            int   nFields;
            char *aFields[2];

            ParseFields(buffer, 2, nFields, aFields);
            if (2 == nFields)
            {
                UTF32 pt1, pt2;
                char *p = strchr(aFields[0], '-');
                if (NULL != p)
                {
                    *p++ = '\0';
                    pt1 = DecodeCodePoint(aFields[0]);
                    pt2 = DecodeCodePoint(p);
                }
                else
                {
                    pt1 = DecodeCodePoint(aFields[0]);
                    pt1 = pt2;
                }

                for (UTF32 pt = pt1; pt <= pt2; pt++)
                {
                    char hex[32];
                    char desc[1024];

                    sprintf(hex, "%04X", pt);
                    sprintf(desc, "%s CHARACTER %04X", aFields[1], pt);

                    int nFields1 = 15;
                    char *aFields1[15];
                    aFields1[0] = hex;
                    aFields1[1] = desc;
                    aFields1[2] = "So";
                    aFields1[3] = "0";
                    aFields1[4] = "ON";
                    aFields1[5] = "";
                    aFields1[6] = "";
                    aFields1[7] = "";
                    aFields1[8] = "";
                    aFields1[9] = "N";
                    aFields1[10] = "";
                    aFields1[11] = "";
                    aFields1[12] = "";
                    aFields1[13] = "";
                    aFields1[14] = "";

                    LoadUnicodeDataLine(pt, nFields1, aFields1);
                }
            }
        }
        fclose(fp);
    }
}
