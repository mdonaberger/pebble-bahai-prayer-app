// Bahá'í Prayer Book — Pebble C SDK v4
// main.c — 263 prayers, 4-level menu, flash resources
// 263 prayers across 43 sort categories
//
// Resources:
//   PRAYERS         — raw binary (prayers.bin)
//   LONG_OBLIGATORY — raw binary (Long Obligatory Prayer text)

#include <pebble.h>

#define BG_COLOR     GColorFromHEX(0x170a57)
#define FG_COLOR     GColorWhite
#define HIGHLIGHT_BG GColorFromHEX(0xe6a550)
#define HEADER_BG    GColorBlack
#define FROM_LONG_RESOURCE  0xFFFFFFFF
#define NO_PREFACE           0

typedef struct {
    const char *title;
    const char *author;
    uint32_t preface_offset;
    uint32_t preface_length;
    uint32_t text_offset;
    uint32_t text_length;
    int word_count;
} Prayer;

typedef struct {
    const char *name;
    int start_index;   // index into ALL_PRAYERS
    int count;
} SortCategory;

typedef struct {
    const char *name;
    SortCategory *categories;
    int count;
} TopCategory;

static Prayer ALL_PRAYERS[263] = {
    { "Short Obligatory Prayer", "Bahá’u’lláh", NO_PREFACE, 0, 49, 258, 50 },
    { "Medium Obligatory Prayer", "Bahá’u’lláh", 307, 148, 455, 2593, 471 },
    { "Long Obligatory Prayer", "Bahá’u’lláh", 3048, 284, FROM_LONG_RESOURCE, 0, 1538 },
    { "O Thou Whose face is the object...", "Bahá’u’lláh", NO_PREFACE, 0, 3332, 592, 109 },
    { "My God, my Adored One, my King...", "Bahá’u’lláh", NO_PREFACE, 0, 3924, 793, 149 },
    { "Lauded be Thy Name, O Lord our God!", "The Báb", NO_PREFACE, 0, 4717, 1588, 279 },
    { "O my God, my Lord and my Master!", "The Báb", NO_PREFACE, 0, 6305, 391, 74 },
    { "Lord! Pitiful are we, grant us Thy favor...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 6696, 687, 129 },
    { "O Thou kind Lord! We are servants...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 7383, 1048, 192 },
    { "Remove not, O Lord, the festal board...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 8431, 679, 116 },
    { "O Thou kind Lord!", "‘Abdu’l-Bahá", 9110, 18, 9128, 679, 115 },
    { "O Lord, my God! This is a child...", "Bahá’u’lláh", NO_PREFACE, 0, 9807, 773, 144 },
    { "Praised be Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 10580, 873, 155 },
    { "O Thou peerless Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 11453, 215, 39 },
    { "O God! Rear this little babe...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 11668, 385, 75 },
    { "O God! Educate these children.", "‘Abdu’l-Bahá", NO_PREFACE, 0, 12053, 375, 69 },
    { "O Thou kind Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 12428, 397, 68 },
    { "O Lord! Make these children excellent plants.", "‘Abdu’l-Bahá", NO_PREFACE, 0, 12825, 536, 94 },
    { "O God, guide me, protect me...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 13361, 114, 23 },
    { "O my Lord! O my Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 13475, 606, 118 },
    { "O Peerless Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 14081, 593, 104 },
    { "He is God! O God, my God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 14674, 73, 17 },
    { "O my God! O my God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 14747, 492, 91 },
    { "O Lord! I am a child...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 15239, 349, 66 },
    { "O Thou most glorious Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 15588, 362, 64 },
    { "Prayer for the Dead", "Bahá’u’lláh", 15950, 615, 16565, 1020, 182 },
    { "Glory be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 17585, 1217, 221 },
    { "He is God, exalted is He...", "Bahá’u’lláh", NO_PREFACE, 0, 18802, 2732, 500 },
    { "O my God! O Thou forgiver of sins...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 21534, 459, 79 },
    { "O my God! O my God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 21993, 937, 166 },
    { "O Thou forgiving Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 22930, 695, 119 },
    { "O my God! O Forgiver of sins...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 23625, 567, 97 },
    { "O Lord, O Thou Whose mercy...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 24192, 1036, 172 },
    { "O Thou Kind Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 25228, 1655, 299 },
    { "Suffer me, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 26883, 1115, 206 },
    { "Glorified art Thou, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 27998, 1099, 207 },
    { "Lauded be Thy name, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 29097, 898, 180 },
    { "Many a chilled heart, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 29995, 1181, 216 },
    { "I know not, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 31176, 1145, 214 },
    { "Praise be unto Thee, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 32321, 1691, 324 },
    { "In the Name of Thy Lord, the Creator...", "The Báb", NO_PREFACE, 0, 34012, 1434, 270 },
    { "O Lord! Unto Thee I repair for refuge...", "The Báb", NO_PREFACE, 0, 35446, 476, 92 },
    { "O God, my God! Thou art my Hope...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 35922, 806, 139 },
    { "O God, my God! Fill up for me the cup...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 36728, 963, 172 },
    { "Magnified, O Lord my God, be Thy Name...", "Bahá’u’lláh", NO_PREFACE, 0, 37691, 1144, 201 },
    { "Glory to Thee, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 38835, 1316, 233 },
    { "O God, my Master, the Goal of my desire!", "Bahá’u’lláh", NO_PREFACE, 0, 40151, 587, 103 },
    { "How can I choose to sleep, O God...", "Bahá’u’lláh", NO_PREFACE, 0, 40738, 879, 173 },
    { "O Lord, I have turned my face unto Thy kingdom of oneness...", "‘Abdu’l-Bahá", 41617, 145, 41762, 463, 92 },
    { "O my Lord! I have drawn nigh...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 42225, 4784, 859 },
    { "Glory be unto Thee, O Lord my God!", "The Báb", NO_PREFACE, 0, 47009, 1231, 224 },
    { "I beg Thy forgiveness, O my God...", "The Báb", NO_PREFACE, 0, 48240, 744, 143 },
    { "O Lord! In this Most Great Dispensation...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 48984, 506, 90 },
    { "O God, my God! This Thy handmaid...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 49490, 916, 169 },
    { "My Lord! My Lord! I praise Thee...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 50406, 678, 123 },
    { "Glory be to Thee, O King of eternity...", "Bahá’u’lláh", NO_PREFACE, 0, 51084, 1341, 252 },
    { "He is the Mighty, the Pardoner...", "Bahá’u’lláh", NO_PREFACE, 0, 52425, 1199, 225 },
    { "Make firm our steps, O Lord, in Thy path...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 53624, 1107, 199 },
    { "O compassionate God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 54731, 851, 156 },
    { "O my Lord and my Hope!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 55582, 465, 82 },
    { "Glorified art Thou, O Lord my God! I beseech Thee...", "Bahá’u’lláh", NO_PREFACE, 0, 56047, 1373, 272 },
    { "I am he, O my Lord, that hath set his face...", "Bahá’u’lláh", NO_PREFACE, 0, 57420, 668, 124 },
    { "Thou seest me, O my Lord, with my face turned...", "Bahá’u’lláh", NO_PREFACE, 0, 58088, 836, 164 },
    { "Glorified art Thou, O Lord my God! Every time...", "Bahá’u’lláh", NO_PREFACE, 0, 58924, 1637, 306 },
    { "Lauded be Thy name, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 60561, 861, 163 },
    { "O God our Lord!", "The Báb", NO_PREFACE, 0, 61422, 1126, 187 },
    { "Praise be unto Thee, O Lord.", "The Báb", NO_PREFACE, 0, 62548, 414, 73 },
    { "I beg Thee to forgive me, O my Lord...", "The Báb", NO_PREFACE, 0, 62962, 469, 90 },
    { "Glory be unto Thee, O God.", "The Báb", NO_PREFACE, 0, 63431, 1024, 185 },
    { "O Thou forgiving Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 64455, 968, 170 },
    { "O holy Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 65423, 1972, 365 },
    { "I am aware, O Lord...", "The Báb", NO_PREFACE, 0, 67395, 733, 130 },
    { "O God, my God!", "‘Abdu’l-Bahá", 68128, 586, 68714, 711, 122 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 69425, 1025, 186 },
    { "O Thou merciful God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 70450, 1206, 208 },
    { "O Thou kind Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 71656, 1143, 205 },
    { "O my God! O my God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 72799, 843, 147 },
    { "O Thou forgiving God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 73642, 1704, 307 },
    { "O Divine Providence!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 75346, 841, 144 },
    { "O Thou loving Provider!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 76187, 451, 77 },
    { "O God! Dispel all those elements...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 76638, 391, 72 },
    { "He is God! Thou seest us, O my God...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 77029, 260, 47 },
    { "He is God! How can we render Thee thanks...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 77289, 369, 66 },
    { "O God my God! I beg of Thee...", "Bahá’u’lláh", NO_PREFACE, 0, 77658, 855, 163 },
    { "Thy name is my healing, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 78513, 289, 56 },
    { "Glory be to Thee, O Lord my God! I implore...", "Bahá’u’lláh", NO_PREFACE, 0, 78802, 1358, 231 },
    { "Praised be Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 80160, 1379, 261 },
    { "Glory be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 81539, 1397, 246 },
    { "He is the Healer...", "Bahá’u’lláh", NO_PREFACE, 0, 82936, 7368, 1345 },
    { "Thou art He, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 90304, 1589, 283 },
    { "O Divine Providence!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 91893, 904, 163 },
    { "My God, Whom I worship and adore!", "Bahá’u’lláh", NO_PREFACE, 0, 92797, 848, 146 },
    { "O Thou compassionate Lord...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 93645, 777, 139 },
    { "O Thou kind Lord! O Thou who art generous...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 94422, 1062, 192 },
    { "O Thou kind Lord! Thou hast created...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 95484, 1259, 227 },
    { "O God, O Thou Who hast cast Thy splendor...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 96743, 1077, 186 },
    { "Thy unity is inscrutable, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 97820, 2119, 375 },
    { "Lauded be Thy name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 99939, 2876, 519 },
    { "Magnified be Thy name, O Lord my God...", "Bahá’u’lláh", NO_PREFACE, 0, 102815, 1540, 285 },
    { "Praised be Thou, O Lord my God! Every time...", "Bahá’u’lláh", NO_PREFACE, 0, 104355, 3462, 630 },
    { "Lauded be Thy name, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 107817, 1837, 349 },
    { "Praised be Thou, O Lord my God! I implore...", "Bahá’u’lláh", NO_PREFACE, 0, 109654, 2541, 442 },
    { "Lauded be Thy name, O Lord my God! I testify...", "Bahá’u’lláh", NO_PREFACE, 0, 112195, 8982, 1629 },
    { "Send down Thy blessings...", "The Báb", NO_PREFACE, 0, 121177, 885, 166 },
    { "He is the Bestower, the Bounteous!", "Bahá’u’lláh", 122062, 647, 122709, 1060, 188 },
    { "He is God! O peerless Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 123769, 1083, 194 },
    { "Glory be unto Thee, O my God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 124852, 820, 147 },
    { "O my Lord! O my Lord!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 125672, 565, 96 },
    { "O my God and my Master!", "Bahá’u’lláh", NO_PREFACE, 0, 126237, 1522, 289 },
    { "I have wakened in Thy shelter...", "Bahá’u’lláh", NO_PREFACE, 0, 127759, 340, 62 },
    { "I have risen this morning...", "Bahá’u’lláh", NO_PREFACE, 0, 128099, 449, 84 },
    { "I give praise to Thee, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 128548, 1484, 270 },
    { "O Thou the Lord of the visible...", "Bahá’u’lláh", NO_PREFACE, 0, 130032, 1835, 334 },
    { "All-praise be to Thee, O Lord...", "Bahá’u’lláh", NO_PREFACE, 0, 131867, 2562, 461 },
    { "O my God, O my Lord, O my Master!", "The Báb", NO_PREFACE, 0, 134429, 225, 42 },
    { "Lauded and glorified art Thou...", "The Báb", NO_PREFACE, 0, 134654, 2124, 388 },
    { "All praise be unto God...", "The Báb", NO_PREFACE, 0, 136778, 4307, 740 },
    { "Glory be to Thee, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 141085, 1177, 212 },
    { "Verily I am Thy servant...", "The Báb", NO_PREFACE, 0, 142262, 1853, 319 },
    { "O my God! There is no one...", "The Báb", NO_PREFACE, 0, 144115, 1701, 326 },
    { "O God, my God, my Beloved...", "The Báb", NO_PREFACE, 0, 145816, 45, 9 },
    { "Unto Thee be praise, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 145861, 1560, 281 },
    { "Faded now is all that erstwhile flourished...", "Bahá’u’lláh", NO_PREFACE, 0, 147421, 2702, 489 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 150123, 1159, 216 },
    { "Glory be unto Thee, O Lord my God!", "The Báb", NO_PREFACE, 0, 151282, 1717, 305 },
    { "All praise, O my God, be to Thee...", "Bahá’u’lláh", NO_PREFACE, 0, 152999, 1030, 180 },
    { "In the Name of God, the Most High!", "Bahá’u’lláh", NO_PREFACE, 0, 154029, 1732, 305 },
    { "Magnified be Thy Name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 155761, 915, 170 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 156676, 966, 178 },
    { "Praised be Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 157642, 1414, 267 },
    { "All majesty and glory, O my God...", "The Báb", NO_PREFACE, 0, 159056, 1768, 321 },
    { "Lauded be Thy name, O my God! Thou beholdest me...", "Bahá’u’lláh", NO_PREFACE, 0, 160824, 1637, 316 },
    { "Glorified be Thy name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 162461, 1802, 336 },
    { "Lauded be Thy name, O Thou Who beholdest...", "Bahá’u’lláh", NO_PREFACE, 0, 164263, 1160, 204 },
    { "Thou beholdest, O my God, the Day-Star...", "Bahá’u’lláh", NO_PREFACE, 0, 165423, 1665, 313 },
    { "Lauded be Thy name, O Lord my God! Thou seest me...", "Bahá’u’lláh", NO_PREFACE, 0, 167088, 1195, 228 },
    { "Praised be Thou, O Lord my God! I implore Thee...", "Bahá’u’lláh", NO_PREFACE, 0, 168283, 1334, 242 },
    { "Thou beholdest, O my God, Thy servant...", "Bahá’u’lláh", NO_PREFACE, 0, 169617, 1864, 338 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 171481, 4726, 887 },
    { "Praised be Thou, O my God! Thou seest me...", "Bahá’u’lláh", NO_PREFACE, 0, 176207, 1870, 359 },
    { "Unto Thee be praise, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 178077, 1442, 268 },
    { "Thou seest, O my Lord...", "The Báb", NO_PREFACE, 0, 179519, 908, 158 },
    { "How can I praise Thee, O Lord...", "The Báb", NO_PREFACE, 0, 180427, 4908, 890 },
    { "Praise be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 185335, 1510, 288 },
    { "Praised be Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 186845, 760, 150 },
    { "Lauded be Thy name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 187605, 687, 126 },
    { "O God, my God! I have set out...", "Bahá’u’lláh", NO_PREFACE, 0, 188292, 536, 101 },
    { "Ordain for me, O my Lord...", "The Báb", NO_PREFACE, 0, 188828, 929, 168 },
    { "Glory be to Thee, O God!", "The Báb", NO_PREFACE, 0, 189757, 762, 138 },
    { "In the Name of God, the Lord...", "The Báb", NO_PREFACE, 0, 190519, 2137, 380 },
    { "O God, my God! Shield Thy trusted servants...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 192656, 634, 104 },
    { "O my Lord! Thou knowest...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 193290, 792, 135 },
    { "O Thou kind and loving Providence!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 194082, 2885, 504 },
    { "In His Name, the Exalted...", "Bahá’u’lláh", NO_PREFACE, 0, 196967, 1516, 274 },
    { "Magnified be Thy name, O God...", "Bahá’u’lláh", NO_PREFACE, 0, 198483, 593, 106 },
    { "Glorified be Thou, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 199076, 1262, 246 },
    { "Praise be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 200338, 1397, 255 },
    { "How numerous the souls...", "The Báb", NO_PREFACE, 0, 201735, 1676, 290 },
    { "Glory be unto Thee, O Lord!", "The Báb", NO_PREFACE, 0, 203411, 2727, 490 },
    { "O Divine Providence!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 206138, 693, 129 },
    { "O God, and the God of all Names...", "Bahá’u’lláh", NO_PREFACE, 0, 206831, 997, 190 },
    { "I give praise to Thee...", "Bahá’u’lláh", NO_PREFACE, 0, 207828, 3168, 596 },
    { "From the sweet-scented streams...", "Bahá’u’lláh", NO_PREFACE, 0, 210996, 1602, 309 },
    { "Create in me a pure heart...", "Bahá’u’lláh", NO_PREFACE, 0, 212598, 770, 147 },
    { "He is the Gracious, the All-Bountiful!", "Bahá’u’lláh", NO_PREFACE, 0, 213368, 1431, 268 },
    { "O my Lord! Make Thy beauty to be my food...", "Bahá’u’lláh", NO_PREFACE, 0, 214799, 438, 79 },
    { "Lauded be Thy Name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 215237, 1123, 206 },
    { "O my God, the God of bounty and mercy!", "Bahá’u’lláh", NO_PREFACE, 0, 216360, 512, 89 },
    { "He is the prayer-hearing, prayer-answering God!", "Bahá’u’lláh", NO_PREFACE, 0, 216872, 2395, 437 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 219267, 1361, 259 },
    { "Vouchsafe unto me, O my God...", "The Báb", NO_PREFACE, 0, 220628, 1401, 255 },
    { "O God! Refresh and gladden my spirit.", "‘Abdu’l-Bahá", NO_PREFACE, 0, 222029, 440, 91 },
    { "O my God! O my God! Glory be unto Thee...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 222469, 792, 152 },
    { "O my Lord! O my Lord! This is a lamp...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 223261, 343, 65 },
    { "O my God! O my God! This, Thy servant...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 223604, 527, 96 },
    { "O God, my God! This is Thy radiant servant...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 224131, 1068, 188 },
    { "O God, my God! These are Thy feeble servants...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 225199, 1047, 187 },
    { "He is God! O God, my God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 226246, 2118, 363 },
    { "O Thou Provider! Thou hast breathed over...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 228364, 1519, 274 },
    { "I beseech Thee, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 229883, 315, 60 },
    { "Glorified be Thy name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 230198, 1120, 215 },
    { "O Thou Whose nearness is my wish...", "Bahá’u’lláh", NO_PREFACE, 0, 231318, 1179, 224 },
    { "O God, my God! I have turned in repentence...", "Bahá’u’lláh", NO_PREFACE, 0, 232497, 1198, 228 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 233695, 988, 183 },
    { "Praise be to Thee, O Lord...", "The Báb", NO_PREFACE, 0, 234683, 1591, 296 },
    { "Praised and glorified art Thou...", "The Báb", NO_PREFACE, 0, 236274, 717, 123 },
    { "O Lord my God! Assist Thy loved ones...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 236991, 344, 60 },
    { "O Thou, my God, Who guidest the seeker...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 237335, 1520, 264 },
    { "I magnify Thy Name, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 238855, 1202, 220 },
    { "Magnified be Thy name...", "Bahá’u’lláh", NO_PREFACE, 0, 240057, 1396, 255 },
    { "Praise be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 241453, 1188, 219 },
    { "Glory be unto Thee, O Lord of the world...", "Bahá’u’lláh", NO_PREFACE, 0, 242641, 1407, 259 },
    { "O God, Who art the Author...", "Bahá’u’lláh", NO_PREFACE, 0, 244048, 1213, 222 },
    { "O my God, aid Thou Thy servant...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 245261, 289, 53 },
    { "O God, my God! Aid Thou...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 245550, 362, 62 },
    { "Thou seest me, O my God...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 245912, 1493, 262 },
    { "Say: Magnified be Thy Name...", "Bahá’u’lláh", NO_PREFACE, 0, 247405, 1145, 206 },
    { "Thou knowest, O God...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 248550, 1006, 191 },
    { "O Thou incomparable God!", "‘Abdu’l-Bahá", 249556, 59, 249615, 692, 122 },
    { "O God, my God! Thou seest me...", "‘Abdu’l-Bahá", 250307, 191, 250498, 1095, 188 },
    { "O God! O God! Thou seest my weakness...", "‘Abdu’l-Bahá", 251593, 173, 251766, 1531, 267 },
    { "O God, my God! Thou seest how black darkness...", "‘Abdu’l-Bahá", 253297, 59, 253356, 3021, 517 },
    { "O Thou kind Lord! Praise be unto Thee...", "‘Abdu’l-Bahá", 256377, 131, 256508, 1496, 274 },
    { "O my God! O my God! Thou seest me...", "‘Abdu’l-Bahá", 258004, 231, 258235, 746, 138 },
    { "O Lord, my God! Praise and thanksgiving...", "‘Abdu’l-Bahá", 258981, 127, 259108, 999, 177 },
    { "O God! O God! This is a broken-winged bird...", "‘Abdu’l-Bahá", 260107, 103, 260210, 831, 152 },
    { "Praise be to Thee, O my God!", "‘Abdu’l-Bahá", 261041, 37, 261078, 670, 113 },
    { "O God, my God! Thou beholdest...", "‘Abdu’l-Bahá", 261748, 118, 261866, 844, 150 },
    { "O Thou Whose tests are a healing medicine...", "Bahá’u’lláh", NO_PREFACE, 0, 262710, 629, 121 },
    { "Glory to Thee, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 263339, 1304, 249 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 264643, 1039, 188 },
    { "Thou knowest full well, O my God...", "The Báb", NO_PREFACE, 0, 265682, 1284, 236 },
    { "Praise be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 266966, 1473, 269 },
    { "Magnified be Thy name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 268439, 1211, 222 },
    { "Lauded be Thy name, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 269650, 1270, 230 },
    { "All praise be to Thee, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 270920, 1130, 195 },
    { "Unto Thee be praise...", "Bahá’u’lláh", NO_PREFACE, 0, 272050, 4596, 819 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 276646, 1123, 216 },
    { "O God, My God! Be Thou not far from me...", "Bahá’u’lláh", NO_PREFACE, 0, 277769, 1326, 238 },
    { "Lauded be Thy name, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 279095, 1153, 214 },
    { "Glory be unto Thee, O Lord...", "The Báb", NO_PREFACE, 0, 280248, 1247, 224 },
    { "O Lord! Provide for the speedy growth...", "The Báb", NO_PREFACE, 0, 281495, 634, 112 },
    { "O Lord! Render victorious...", "The Báb", NO_PREFACE, 0, 282129, 1314, 233 },
    { "O Lord! Enable all the peoples...", "The Báb", NO_PREFACE, 0, 283443, 288, 49 },
    { "O God, my God! Praise be unto Thee...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 283731, 1067, 190 },
    { "He is God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 284798, 1652, 294 },
    { "Thou seest, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 286450, 766, 136 },
    { "Glory be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 287216, 457, 84 },
    { "Suffer me not, O God, my God...", "Bahá’u’lláh", NO_PREFACE, 0, 287673, 494, 90 },
    { "O my God! O my God! Unite the hearts...", "Bahá’u’lláh", NO_PREFACE, 0, 288167, 394, 75 },
    { "O my God! O my God! Verily, I invoke Thee...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 288561, 1178, 207 },
    { "O God! The trials Thou sendest...", "Bahá’u’lláh", NO_PREFACE, 0, 289739, 1224, 230 },
    { "Magnified be Thy name, O Lord my God! Behold Thou mine eye...", "Bahá’u’lláh", NO_PREFACE, 0, 290963, 1239, 226 },
    { "Glorified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 292202, 1432, 264 },
    { "Glory to Thee, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 293634, 1195, 234 },
    { "O Thou Whose face is the object...", "Bahá’u’lláh", NO_PREFACE, 0, 294829, 1230, 235 },
    { "O Thou, at Whose dreadful majesty...", "Bahá’u’lláh", NO_PREFACE, 0, 296059, 1364, 258 },
    { "Magnified art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 297423, 1184, 225 },
    { "Glory be to Thee, O my God!", "Bahá’u’lláh", NO_PREFACE, 0, 298607, 907, 170 },
    { "Praised be Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 299514, 1366, 250 },
    { "Thou seest, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 300880, 826, 153 },
    { "Magnified be Thy name, O Thou in Whose grasp...", "Bahá’u’lláh", NO_PREFACE, 0, 301706, 1751, 317 },
    { "O Thou kind Lord! From the horizon...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 303457, 1373, 241 },
    { "O Thou kind Lord! Graciously bestow...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 304830, 818, 141 },
    { "O Thou kind Lord! Bestow heavenly confirmation...", "‘Abdu’l-Bahá", NO_PREFACE, 0, 305648, 465, 79 },
    { "Praise and glory be to Thee, O Lord my God!", "‘Abdu’l-Bahá", NO_PREFACE, 0, 306113, 1463, 254 },
    { "I beseech Thee, O my God...", "Bahá’u’lláh", NO_PREFACE, 0, 307576, 7983, 1563 },
    { "Praise be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 315559, 1246, 235 },
    { "These are, O my God, the days...", "Bahá’u’lláh", NO_PREFACE, 0, 316805, 5268, 955 },
    { "Glory be to Thee, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 322073, 10781, 1949 },
    { "This is, O my God, the first of the days...", "Bahá’u’lláh", 332854, 467, 333321, 1221, 229 },
    { "In the Name of Him Who hath been promised...", "Bahá’u’lláh", NO_PREFACE, 0, 334542, 830, 154 },
    { "Praise be unto Thee, O Lord my God! We have observed...", "Bahá’u’lláh", NO_PREFACE, 0, 335372, 614, 112 },
    { "O my God and my Master!", "Bahá’u’lláh", NO_PREFACE, 0, 335986, 987, 177 },
    { "Praised be Thou, O God, my God!", "Bahá’u’lláh", NO_PREFACE, 0, 336973, 804, 149 },
    { "Exalted art Thou, O Lord my God!", "Bahá’u’lláh", NO_PREFACE, 0, 337777, 15684, 2828 },
    { "Magnified art Thou, O Lord...", "Bahá’u’lláh", 353461, 296, 353757, 698, 117 },
    { "Glorified art Thou, O my compassionate Lord!", "Bahá’u’lláh", NO_PREFACE, 0, 354455, 1313, 251 },
    { "Intercalary Days", "Bahá’u’lláh", 355768, 159, 355927, 1718, 318 },
    { "O God, my God! We are servants of Thine...", "‘Abdu’l-Bahá", 357645, 226, 357871, 1287, 219 },
    { "O Thou Lord of the Kingdom!", "‘Abdu’l-Bahá", 359158, 99, 359257, 750, 141 },
    { "O God! O God! From the unseen kingdom...", "‘Abdu’l-Bahá", 360007, 72, 360079, 1172, 204 },
    { "Tablet of Ahmad", "Bahá’u’lláh", 361251, 510, 361761, 3520, 652 },
};

static SortCategory s_obli_cats[] = {
    { "Short Obligatory", 0, 1 },
    { "Medium Obligatory", 1, 1 },
    { "Long Obligatory", 2, 1 },
};

static SortCategory s_gene_cats[] = {
    { "Aid and Assistance", 3, 7 },
    { "America", 10, 1 },
    { "Children", 11, 14 },
    { "The Departed", 25, 9 },
    { "Detachment", 34, 10 },
    { "Divine Springtime", 44, 2 },
    { "Evening", 46, 4 },
    { "Families", 50, 5 },
    { "Firmness in the Covenant", 55, 5 },
    { "Forgiveness", 60, 12 },
    { "The Fund", 72, 1 },
    { "Gatherings", 73, 8 },
    { "Grace at Table", 81, 2 },
    { "Healing", 83, 8 },
    { "Humanity", 91, 5 },
    { "Manifestation of God", 96, 8 },
    { "Marriage", 104, 4 },
    { "Morning", 108, 4 },
    { "Nearness to God", 112, 9 },
    { "Paradise", 121, 4 },
    { "Praise and Gratitude", 125, 6 },
    { "Prison", 131, 12 },
    { "Protection", 143, 11 },
    { "Sacrifice", 154, 6 },
    { "Service", 160, 2 },
    { "Spiritual Growth", 162, 18 },
    { "Steadfastness", 180, 9 },
    { "Teaching", 189, 9 },
    { "Teaching (Divine Plan)", 198, 10 },
    { "Tests and Difficulties", 208, 4 },
    { "Trials", 212, 7 },
    { "Triumph of the Cause", 219, 10 },
    { "Unity", 229, 2 },
    { "Women", 231, 11 },
    { "Youth", 242, 4 },
};

static SortCategory s_occa_cats[] = {
    { "The Fast", 246, 10 },
    { "Huququ'llah", 256, 2 },
    { "Ayyam-i-Ha", 258, 1 },
    { "Spiritual Assembly", 259, 3 },
    { "Special Tablets", 262, 1 },
};

static TopCategory TOP_CATEGORIES[] = {
    { "Obligatory", s_obli_cats, 3 },
    { "General", s_gene_cats, 35 },
    { "Occasional", s_occa_cats, 5 },
};
#define NUM_TOP_CATEGORIES 3


// =========================================================
// GLOBALS
// =========================================================

static int s_top_index;
static int s_sort_index;
static int s_prayer_index;

// =========================================================
// MAIN MENU
// =========================================================

static const char *MAIN_ITEMS[] = {
    "Prayers",
    "All\u00E1h-u-Abh\u00E1 (95x)",
    "Bad\u00ED Calendar",
    "About"
};
#define NUM_MAIN_ITEMS 4

static Window *s_main_window;
static MenuLayer *s_main_menu;

static uint16_t main_get_num_rows(MenuLayer *menu, uint16_t section, void *data) {
    return NUM_MAIN_ITEMS;
}

static void main_draw_row(GContext *ctx, const Layer *cell_layer,
                          MenuIndex *cell_index, void *data) {
    menu_cell_basic_draw(ctx, cell_layer, MAIN_ITEMS[cell_index->row], NULL, NULL);
}

static int16_t main_get_cell_height(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    return 44;
}

static void push_top_categories(void);
static void push_counter(void);
static void push_badi(void);
static void push_about(void);

static void main_select(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    switch (cell_index->row) {
        case 0: push_top_categories(); break;
        case 1: push_counter(); break;
        case 2: push_badi(); break;
        case 3: push_about(); break;
    }
}

static void main_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);
    s_main_menu = menu_layer_create(bounds);
    menu_layer_set_callbacks(s_main_menu, NULL, (MenuLayerCallbacks) {
        .get_num_rows = main_get_num_rows,
        .draw_row = main_draw_row,
        .get_cell_height = main_get_cell_height,
        .select_click = main_select,
    });
    menu_layer_set_click_config_onto_window(s_main_menu, window);
    menu_layer_set_normal_colors(s_main_menu, BG_COLOR, FG_COLOR);
    menu_layer_set_highlight_colors(s_main_menu, HIGHLIGHT_BG, FG_COLOR);
    layer_add_child(root, menu_layer_get_layer(s_main_menu));
}

static void main_window_unload(Window *window) {
    menu_layer_destroy(s_main_menu);
}

// =========================================================
// LEVEL 1: TOP CATEGORY SUBMENU (Obligatory / General / Occasional)
// =========================================================

static Window *s_top_window;
static MenuLayer *s_top_menu;

static uint16_t top_get_num_rows(MenuLayer *menu, uint16_t section, void *data) {
    return NUM_TOP_CATEGORIES;
}

static void top_draw_row(GContext *ctx, const Layer *cell_layer,
                         MenuIndex *cell_index, void *data) {
    TopCategory *tc = &TOP_CATEGORIES[cell_index->row];
    int total = 0;
    for (int i = 0; i < tc->count; i++)
        total += tc->categories[i].count;
    char subtitle[32];
    snprintf(subtitle, sizeof(subtitle), "%d categories, %d prayers", tc->count, total);
    menu_cell_basic_draw(ctx, cell_layer, tc->name, subtitle, NULL);
}

static int16_t top_get_cell_height(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    return 50;
}

static void push_sort_categories(int top_index);
static void push_reader_direct(int flat_index);

static void top_select(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    s_top_index = cell_index->row;
    push_sort_categories(s_top_index);
}

static void top_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);
    s_top_menu = menu_layer_create(bounds);
    menu_layer_set_callbacks(s_top_menu, NULL, (MenuLayerCallbacks) {
        .get_num_rows = top_get_num_rows,
        .draw_row = top_draw_row,
        .get_cell_height = top_get_cell_height,
        .select_click = top_select,
    });
    menu_layer_set_click_config_onto_window(s_top_menu, window);
    menu_layer_set_normal_colors(s_top_menu, BG_COLOR, FG_COLOR);
    menu_layer_set_highlight_colors(s_top_menu, HIGHLIGHT_BG, FG_COLOR);
    layer_add_child(root, menu_layer_get_layer(s_top_menu));
}

static void top_window_unload(Window *window) {
    menu_layer_destroy(s_top_menu);
}

static void push_top_categories(void) {
    s_top_window = window_create();
    window_set_background_color(s_top_window, BG_COLOR);
    window_set_window_handlers(s_top_window, (WindowHandlers) {
        .load = top_window_load,
        .unload = top_window_unload,
    });
    window_stack_push(s_top_window, true);
}

// =========================================================
// LEVEL 2: SORT CATEGORY SUBMENU (e.g. Healing, Unity, etc.)
// =========================================================

static Window *s_sort_window;
static MenuLayer *s_sort_menu;

static uint16_t sort_get_num_rows(MenuLayer *menu, uint16_t section, void *data) {
    return TOP_CATEGORIES[s_top_index].count;
}

static void sort_draw_row(GContext *ctx, const Layer *cell_layer,
                          MenuIndex *cell_index, void *data) {
    SortCategory *sc = &TOP_CATEGORIES[s_top_index].categories[cell_index->row];
    char subtitle[32];
    snprintf(subtitle, sizeof(subtitle), "%d prayer%s", sc->count, sc->count == 1 ? "" : "s");
    menu_cell_basic_draw(ctx, cell_layer, sc->name, subtitle, NULL);
}

static int16_t sort_get_cell_height(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    return 44;
}

static void push_prayer_list(int top_index, int sort_index);
static void push_reader(int flat_index);

static void sort_select(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    s_sort_index = cell_index->row;
    SortCategory *sc = &TOP_CATEGORIES[s_top_index].categories[s_sort_index];
    if (sc->count == 1) {
        // Only one prayer, go straight to reader
        push_reader(sc->start_index);
    } else {
        push_prayer_list(s_top_index, s_sort_index);
    }
}

static void sort_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);
    s_sort_menu = menu_layer_create(bounds);
    menu_layer_set_callbacks(s_sort_menu, NULL, (MenuLayerCallbacks) {
        .get_num_rows = sort_get_num_rows,
        .draw_row = sort_draw_row,
        .get_cell_height = sort_get_cell_height,
        .select_click = sort_select,
    });
    menu_layer_set_click_config_onto_window(s_sort_menu, window);
    menu_layer_set_normal_colors(s_sort_menu, BG_COLOR, FG_COLOR);
    menu_layer_set_highlight_colors(s_sort_menu, HIGHLIGHT_BG, FG_COLOR);
    layer_add_child(root, menu_layer_get_layer(s_sort_menu));
}

static void sort_window_unload(Window *window) {
    menu_layer_destroy(s_sort_menu);
}

static void push_sort_categories(int top_index) {
    s_top_index = top_index;
    s_sort_window = window_create();
    window_set_background_color(s_sort_window, BG_COLOR);
    window_set_window_handlers(s_sort_window, (WindowHandlers) {
        .load = sort_window_load,
        .unload = sort_window_unload,
    });
    window_stack_push(s_sort_window, true);
}

// =========================================================
// LEVEL 3: PRAYER LIST (within a sort category)
// =========================================================

static Window *s_plist_window;
static MenuLayer *s_plist_menu;
static int s_plist_start;
static int s_plist_count;

static uint16_t plist_get_num_rows(MenuLayer *menu, uint16_t section, void *data) {
    return s_plist_count;
}

static void plist_draw_row(GContext *ctx, const Layer *cell_layer,
                           MenuIndex *cell_index, void *data) {
    Prayer *prayer = &ALL_PRAYERS[s_plist_start + cell_index->row];
    char subtitle[48];
    snprintf(subtitle, sizeof(subtitle), "%d words \u2014 %s",
             prayer->word_count, prayer->author);
    menu_cell_basic_draw(ctx, cell_layer, prayer->title, subtitle, NULL);
}

static int16_t plist_get_cell_height(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    return 50;
}

static void plist_select(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    push_reader(s_plist_start + cell_index->row);
}

static void plist_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);
    s_plist_menu = menu_layer_create(bounds);
    menu_layer_set_callbacks(s_plist_menu, NULL, (MenuLayerCallbacks) {
        .get_num_rows = plist_get_num_rows,
        .draw_row = plist_draw_row,
        .get_cell_height = plist_get_cell_height,
        .select_click = plist_select,
    });
    menu_layer_set_click_config_onto_window(s_plist_menu, window);
    menu_layer_set_normal_colors(s_plist_menu, BG_COLOR, FG_COLOR);
    menu_layer_set_highlight_colors(s_plist_menu, HIGHLIGHT_BG, FG_COLOR);
    layer_add_child(root, menu_layer_get_layer(s_plist_menu));
}

static void plist_window_unload(Window *window) {
    menu_layer_destroy(s_plist_menu);
}

static void push_prayer_list(int top_index, int sort_index) {
    s_top_index = top_index;
    s_sort_index = sort_index;
    SortCategory *sc = &TOP_CATEGORIES[s_top_index].categories[s_sort_index];
    s_plist_start = sc->start_index;
    s_plist_count = sc->count;
    s_plist_window = window_create();
    window_set_background_color(s_plist_window, BG_COLOR);
    window_set_window_handlers(s_plist_window, (WindowHandlers) {
        .load = plist_window_load,
        .unload = plist_window_unload,
    });
    window_stack_push(s_plist_window, true);
}

// =========================================================
// READER (loads preface + body from flash resources)
// =========================================================

static Window *s_reader_window;
static ScrollLayer *s_reader_scroll;
static TextLayer *s_reader_title;
static TextLayer *s_reader_text;
#define READER_BUF_SIZE 16384
static char *s_reader_buf;

static void push_reader_direct(int flat_index) {
    push_reader(flat_index);
}

static void reader_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);

    Prayer *prayer = &ALL_PRAYERS[s_prayer_index];
    s_reader_buf = malloc(READER_BUF_SIZE);

    // Title bar
    s_reader_title = text_layer_create(GRect(0, 0, bounds.size.w, 28));
    text_layer_set_background_color(s_reader_title, HEADER_BG);
    text_layer_set_text_color(s_reader_title, FG_COLOR);
    text_layer_set_font(s_reader_title, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
    text_layer_set_text_alignment(s_reader_title, GTextAlignmentCenter);
    text_layer_set_text(s_reader_title, prayer->title);
    layer_add_child(root, text_layer_get_layer(s_reader_title));

    // Scroll area
    GRect scroll_frame = GRect(0, 28, bounds.size.w, bounds.size.h - 28);
    s_reader_scroll = scroll_layer_create(scroll_frame);

    int offset = 0;

    // Load preface from flash (if present)
    if (prayer->preface_offset != NO_PREFACE && prayer->preface_length > 0) {
        ResHandle h = resource_get_handle(RESOURCE_ID_PRAYERS);
        size_t sz = resource_load_byte_range(h, prayer->preface_offset,
                  (uint8_t *)s_reader_buf, prayer->preface_length);
        s_reader_buf[sz] = '\0';
        offset = strlen(s_reader_buf);
        offset += snprintf(s_reader_buf + offset, READER_BUF_SIZE - offset, "\n\n");
    }

    // Load prayer body from flash
    if (prayer->text_offset == FROM_LONG_RESOURCE) {
        ResHandle h = resource_get_handle(RESOURCE_ID_LONG_OBLIGATORY);
        size_t sz = resource_load(h, (uint8_t *)(s_reader_buf + offset),
                                  READER_BUF_SIZE - offset - 32);
        s_reader_buf[offset + sz] = '\0';
        offset += sz;
    } else {
        ResHandle h = resource_get_handle(RESOURCE_ID_PRAYERS);
        size_t sz = resource_load_byte_range(h, prayer->text_offset,
                  (uint8_t *)(s_reader_buf + offset), prayer->text_length);
        s_reader_buf[offset + sz] = '\0';
        offset += sz;
    }

    // Append attribution
    offset += snprintf(s_reader_buf + offset, READER_BUF_SIZE - offset,
                       "\n\n\u2014 %s", prayer->author);

    // Text layer
    GRect text_frame = GRect(5, 5, bounds.size.w - 10, 2000);
    s_reader_text = text_layer_create(text_frame);
    text_layer_set_background_color(s_reader_text, BG_COLOR);
    text_layer_set_text_color(s_reader_text, FG_COLOR);
    text_layer_set_font(s_reader_text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_overflow_mode(s_reader_text, GTextOverflowModeWordWrap);
    text_layer_set_text(s_reader_text, s_reader_buf);

    GSize content = text_layer_get_content_size(s_reader_text);
    text_layer_set_size(s_reader_text, content);
    scroll_layer_set_content_size(s_reader_scroll, GSize(bounds.size.w, content.h + 20));
    scroll_layer_add_child(s_reader_scroll, text_layer_get_layer(s_reader_text));
    scroll_layer_set_click_config_onto_window(s_reader_scroll, window);
    layer_add_child(root, scroll_layer_get_layer(s_reader_scroll));
}

static void reader_window_unload(Window *window) {
    text_layer_destroy(s_reader_title);
    text_layer_destroy(s_reader_text);
    scroll_layer_destroy(s_reader_scroll);
    free(s_reader_buf);
    s_reader_buf = NULL;
}

static void push_reader(int flat_index) {
    s_prayer_index = flat_index;
    s_reader_window = window_create();
    window_set_background_color(s_reader_window, BG_COLOR);
    window_set_window_handlers(s_reader_window, (WindowHandlers) {
        .load = reader_window_load,
        .unload = reader_window_unload,
    });
    window_stack_push(s_reader_window, true);
}


// =========================================================
// BAD\u00cd CALENDAR
// =========================================================

static Window *s_badi_window;
static TextLayer *s_badi_date_label;
static TextLayer *s_badi_year_label;
static char s_badi_date_buf[64];
static char s_badi_year_buf[32];

static const char *BM[] = {
    "", "Bah\u00E1", "Jal\u00E1l", "Jam\u00E1l", "'A\u1E93amat", "N\u00FAr",
    "Ra\u1E25mat", "Kalim\u00E1t", "Kam\u00E1l", "Asm\u00E1", "'Izzat",
    "Mash\u00EDyyat", "'Ilm", "Qudrat", "Qawl", "Mas\u00E1'il",
    "Sharaf", "Sul\u1E6D\u00E1n", "Mulk", "'Al\u00E1"
};
static const char *BE[] = {
    "", "Splendour", "Glory", "Beauty", "Grandeur", "Light",
    "Mercy", "Words", "Perfection", "Names", "Might",
    "Will", "Knowledge", "Power", "Speech", "Questions",
    "Honour", "Sovereignty", "Dominion", "Loftiness"
};

static bool is_leap(int y) {
    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

static void badi_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    int gyear = t->tm_year + 1900;
    int month_days[] = {0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334};
    int doy = month_days[t->tm_mon] + t->tm_mday;
    if (t->tm_mon > 1 && is_leap(gyear)) doy++;
    int nrz = is_leap(gyear) ? 80 : 79;
    int by = gyear - 1843;
    int ds;
    if (doy >= nrz) { ds = doy - nrz; }
    else { by = gyear - 1844; ds = doy + (is_leap(gyear - 1) ? 366 : 365) - nrz; }
    int ac = is_leap(gyear) ? 5 : 4;
    int month, day;
    bool is_ayyam = false;
    if (ds < 342) { month = ds / 19 + 1; day = ds % 19 + 1; }
    else if (ds < 342 + ac) { is_ayyam = true; day = ds - 342 + 1; month = 0; }
    else { month = 19; day = ds - 342 - ac + 1; }

    if (is_ayyam) {
        snprintf(s_badi_date_buf, sizeof(s_badi_date_buf), "Ayy\u00E1m-i-H\u00E1 %d", day);
    } else {
        snprintf(s_badi_date_buf, sizeof(s_badi_date_buf), "%d %s (%s)", day, BM[month], BE[month]);
    }
    snprintf(s_badi_year_buf, sizeof(s_badi_year_buf), "%d B.E.", by);

    s_badi_date_label = text_layer_create(GRect(0, 60, bounds.size.w, 40));
    text_layer_set_background_color(s_badi_date_label, BG_COLOR);
    text_layer_set_text_color(s_badi_date_label, FG_COLOR);
    text_layer_set_font(s_badi_date_label, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(s_badi_date_label, GTextAlignmentCenter);
    text_layer_set_text(s_badi_date_label, s_badi_date_buf);
    layer_add_child(root, text_layer_get_layer(s_badi_date_label));

    s_badi_year_label = text_layer_create(GRect(0, 105, bounds.size.w, 30));
    text_layer_set_background_color(s_badi_year_label, BG_COLOR);
    text_layer_set_text_color(s_badi_year_label, FG_COLOR);
    text_layer_set_font(s_badi_year_label, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_text_alignment(s_badi_year_label, GTextAlignmentCenter);
    text_layer_set_text(s_badi_year_label, s_badi_year_buf);
    layer_add_child(root, text_layer_get_layer(s_badi_year_label));
}

static void badi_window_unload(Window *window) {
    text_layer_destroy(s_badi_date_label);
    text_layer_destroy(s_badi_year_label);
}

static void push_badi(void) {
    s_badi_window = window_create();
    window_set_background_color(s_badi_window, BG_COLOR);
    window_set_window_handlers(s_badi_window, (WindowHandlers) {
        .load = badi_window_load,
        .unload = badi_window_unload,
    });
    window_stack_push(s_badi_window, true);
}

// =========================================================
// ABOUT
// =========================================================

static Window *s_about_window;
static ScrollLayer *s_about_scroll;
static TextLayer *s_about_text;

static const char *ABOUT_TEXT =
    "Dedicated to my dear friend\n"
    "Meg Moyer.\n"
    "\n"
    "Free & open source.\n"
    "\n"
    "To all of those who labor in\n"
    "service of the unity of all\n"
    "humankind.\n"
    "\n"
    "Made by:\n"
    "Mitch Donaberger\n"
    "https://donaberger.xyz\n"
    "mitch@donaberger.xyz\n"
    "\n"
    "Version 1.0 - Full Prayer Book";

static void about_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);
    s_about_scroll = scroll_layer_create(bounds);
    scroll_layer_set_click_config_onto_window(s_about_scroll, window);
    GRect text_frame = GRect(5, 5, bounds.size.w - 10, 2000);
    s_about_text = text_layer_create(text_frame);
    text_layer_set_background_color(s_about_text, BG_COLOR);
    text_layer_set_text_color(s_about_text, FG_COLOR);
    text_layer_set_font(s_about_text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_overflow_mode(s_about_text, GTextOverflowModeWordWrap);
    text_layer_set_text(s_about_text, ABOUT_TEXT);
    GSize content = text_layer_get_content_size(s_about_text);
    text_layer_set_size(s_about_text, content);
    scroll_layer_set_content_size(s_about_scroll, GSize(bounds.size.w, content.h + 20));
    scroll_layer_add_child(s_about_scroll, text_layer_get_layer(s_about_text));
    layer_add_child(root, scroll_layer_get_layer(s_about_scroll));
}

static void about_window_unload(Window *window) {
    text_layer_destroy(s_about_text);
    scroll_layer_destroy(s_about_scroll);
}

static void push_about(void) {
    s_about_window = window_create();
    window_set_background_color(s_about_window, BG_COLOR);
    window_set_window_handlers(s_about_window, (WindowHandlers) {
        .load = about_window_load,
        .unload = about_window_unload,
    });
    window_stack_push(s_about_window, true);
}

// =========================================================
// 95x COUNTER
// =========================================================

static Window *s_counter_window;
static TextLayer *s_counter_header;
static TextLayer *s_counter_label;
static TextLayer *s_counter_hint;
static char s_counter_buf[16];
static int s_counter_count;

static void counter_update(void) {
    snprintf(s_counter_buf, sizeof(s_counter_buf), "%d / 95", s_counter_count);
    text_layer_set_text(s_counter_label, s_counter_buf);
}

static void counter_up_handler(ClickRecognizerRef recognizer, void *context) {
    if (s_counter_count < 95) {
        s_counter_count++;
        vibes_short_pulse();
        counter_update();
        if (s_counter_count == 95) {
            static const uint32_t seg[] = {300, 100, 300, 100, 300};
            VibePattern pat = {.durations = seg, .num_segments = ARRAY_LENGTH(seg)};
            vibes_enqueue_custom_pattern(pat);
        }
    }
}

static void counter_down_handler(ClickRecognizerRef recognizer, void *context) {
    if (s_counter_count > 0) {
        s_counter_count--;
        counter_update();
    }
}

static void counter_click_config(void *context) {
    window_single_click_subscribe(BUTTON_ID_UP, counter_up_handler);
    window_single_click_subscribe(BUTTON_ID_SELECT, counter_up_handler);
    window_single_click_subscribe(BUTTON_ID_DOWN, counter_down_handler);
}

static void counter_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);
    s_counter_count = 0;
    s_counter_header = text_layer_create(GRect(0, 0, bounds.size.w, 30));
    text_layer_set_background_color(s_counter_header, HEADER_BG);
    text_layer_set_text_color(s_counter_header, FG_COLOR);
    text_layer_set_font(s_counter_header, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(s_counter_header, GTextAlignmentCenter);
    text_layer_set_text(s_counter_header, "All\u00E1h-u-Abh\u00E1");
    layer_add_child(root, text_layer_get_layer(s_counter_header));
    s_counter_label = text_layer_create(GRect(0, 70, bounds.size.w, 40));
    text_layer_set_background_color(s_counter_label, BG_COLOR);
    text_layer_set_text_color(s_counter_label, FG_COLOR);
    text_layer_set_font(s_counter_label, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_counter_label, GTextAlignmentCenter);
    counter_update();
    layer_add_child(root, text_layer_get_layer(s_counter_label));
    s_counter_hint = text_layer_create(GRect(0, 130, bounds.size.w, 50));
    text_layer_set_background_color(s_counter_hint, BG_COLOR);
    text_layer_set_text_color(s_counter_hint, GColorLightGray);
    text_layer_set_font(s_counter_hint, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_alignment(s_counter_hint, GTextAlignmentCenter);
    text_layer_set_text(s_counter_hint, "Up/Select: count\nDown: undo\nBack: exit");
    layer_add_child(root, text_layer_get_layer(s_counter_hint));
}

static void counter_window_unload(Window *window) {
    text_layer_destroy(s_counter_header);
    text_layer_destroy(s_counter_label);
    text_layer_destroy(s_counter_hint);
}

static void push_counter(void) {
    s_counter_window = window_create();
    window_set_background_color(s_counter_window, BG_COLOR);
    window_set_click_config_provider(s_counter_window, counter_click_config);
    window_set_window_handlers(s_counter_window, (WindowHandlers) {
        .load = counter_window_load,
        .unload = counter_window_unload,
    });
    window_stack_push(s_counter_window, true);
}

// =========================================================
// INIT / DEINIT / MAIN
// =========================================================

static void init(void) {
    s_main_window = window_create();
    window_set_background_color(s_main_window, BG_COLOR);
    window_set_window_handlers(s_main_window, (WindowHandlers) {
        .load = main_window_load,
        .unload = main_window_unload,
    });
    window_stack_push(s_main_window, true);
}

static void deinit(void) {
    window_destroy(s_main_window);
}

int main(void) {
    init();
    app_event_loop();
    deinit();
}
