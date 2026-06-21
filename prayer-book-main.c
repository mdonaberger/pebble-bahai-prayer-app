/* ═══════════════════════════════════════════════════════════════
 * BAHÁ'Í PRAYER BOOK — Pebble C SDK v2
 * main.c — main menu + submenu windows + scrollable reader
 *
 * CloudPebble: project type C, target platform emery
 * ═══════════════════════════════════════════════════════════════ */

#include <pebble.h>

/* ── Colors ── */
#define BG_COLOR     GColorBlack
#define FG_COLOR     GColorWhite
#define HIGHLIGHT_BG GColorFromHEX(0x0f3460)
#define HEADER_BG    GColorBlack

/* ═══════════════════════════════════════════════════════════════
 * PRAYER DATA — all 41 prayers from bahaiprayers.org
 * ═══════════════════════════════════════════════════════════════ */

typedef struct {
    const char *title;
    const char *author;
    const char *text;
    int word_count;
    uint32_t resource_id;  /* 0 = use inline text */
} Prayer;

typedef struct {
    const char *name;
    Prayer *items;
    int count;
} PrayerCategory;

static Prayer s_obligatory[] = {
    {"Short Obligatory Prayer",
     "Bahá'u'lláh",
     "I bear witness, O my God, that Thou hast created me to know Thee\n"
        "and to worship Thee.\n"
        "I testify, at this moment, to my powerlessness and to Thy might, to my poverty and to Thy wealth.\n"
        "\n"
        "There is none other God but Thee, the Help in Peril, the Self-Subsisting.",
     50, 0},
    {"Medium Obligatory Prayer",
     "Bahá'u'lláh",
     "God hath exempted women who are in their courses from obligatory prayer and fasting. Let them, instead, after performance of their ablutions, give praise unto God, repeating ninety-five times between the noon of one day and the next \"Glorified be God, the Lord of Splendor and Beauty.\" Thus hath it been decreed in the Book, if ye be of them that comprehend. (The Kitáb-i-Aqdas)\n"
        "\n"
        "Whoso wisheth to pray, let him wash his hands, and while he washeth, let him say:\n"
        "\n"
        "Strengthen my hand, O my God, that it may take hold of Thy Book with such steadfastness that the hosts of the world shall have no power over it. Guard it, then, from meddling with whatsoever doth not belong unto it. Thou art, verily, the Almighty, the Most Powerful.\n"
        "\n"
        "And while washing his face, let him say:\n"
        "\n"
        "I have turned my face unto Thee, O my Lord! Illumine it with the light of Thy countenance. Protect it, then, from turning to anyone but Thee.\n"
        "\n"
        "Then let him stand up, and facing the Qiblih (Point of Adoration, i.e. Bahji, Akka), let him say:\n"
        "\n"
        "God testifieth that there is none other God but Him. His are the kingdoms of Revelation and of creation. He, in truth, hath manifested Him Who is the Dayspring of Revelation, Who conversed on Sinai, through Whom the Supreme Horizon hath been made to shine, and the Lote-Tree beyond which there is no passing hath spoken, and through Whom the call hath been proclaimed unto all who are in heaven and on earth: \"Lo, the All-Possessing is come. Earth and heaven, glory and dominion are God's, the Lord of all men, and the Possessor of the Throne on high and of earth below!\"\n"
        "\n"
        "Let him, then, bend down, with hands resting on the knees, and say:\n"
        "\n"
        "Exalted art Thou above my praise and the praise of anyone beside me, above my description and the description of all who are in heaven and all who are on earth!\n"
        "\n"
        "Then, standing with open hands, palms upward toward the face, let him say:\n"
        "\n"
        "Disappoint not, O my God, him that hath, with beseeching fingers, clung to the hem of Thy mercy and Thy grace, O Thou Who of those who show mercy art the Most Merciful!\n"
        "\n"
        "Let him, then, be seated and say:\n"
        "\n"
        "I bear witness to Thy unity and Thy oneness, and that Thou art God, and that there is none other God beside Thee. Thou hast, verily, revealed thy Cause, fulfilled Thy Covenant, and opened wide the door of Thy grace to all that dwell in heaven and on earth. Blessing and peace, salutation and glory, rest upon Thy loved ones, whom the changes and chances of the world have not deterred from turning unto Thee, and who have given their all, in the hope of obtaining that which is with Thee. Thou art, in truth, the Ever-Forgiving, the All-Bountiful.\n"
        "\n"
        "(If anyone choose to recite instead of the long verse these words: \"God testifieth that there is none other God but Him, the Help in Peril, the Self-Subsisting,\" it would be sufficient. And likewise, it would suffice were he, while seated, to choose to recite these words: \"I bear witness to Thy unity and Thy oneness, and that Thou art God, and that there is none other God beside Thee.\")",
     551, 0},
    {"Long Obligatory Prayer",
     "Bahá'u'lláh",
     NULL,
     1656,
     RESOURCE_ID_LONG_OBLIGATORY},
};

static Prayer s_general[] = {
    {"O Thou Whose face is the object...",
     "Bahá'u'lláh",
     "O Thou Whose face is the object of my adoration, Whose beauty is my sanctuary,\n"
        "Whose habitation is my goal, Whose praise is my hope, Whose providence is my companion, Whose love is the cause of my being,\n"
        "Whose mention is my solace, Whose nearness is my desire, Whose presence is my dearest wish and highest aspiration,\n"
        "I entreat Thee not to withhold from me the things Thou didst ordain for the chosen ones among Thy servants.\n"
        "Supply me, then, with the good of this world and of the next.\n"
        "\n"
        "Thou, truly, art the King of all men.\n"
        "There is no God but Thee, the Ever-Forgiving, the Most Generous.",
     109, 0},
    {"O Thou kind Lord!",
     "'Abdu'l-Bahá",
     "Prayer for America\n"
        "\n"
        "O Thou kind Lord!\n"
        "This gathering is turning to Thee.\n"
        "These hearts are radiant with Thy love.\n"
        "These minds and spirits are exhilarated by the message of Thy glad-tidings.\n"
        "O God!\n"
        "Let this American democracy become glorious in spiritual degrees even as it has aspired to material degrees,\n"
        "and render this just government victorious.\n"
        "Confirm this revered nation to upraise the standard of the oneness of humanity, to promulgate the Most Great Peace,\n"
        "to become thereby most glorious and praiseworthy among all the nations of the world.\n"
        "O God!\n"
        "This American nation is worthy of Thy favors and is deserving of Thy mercy.\n"
        "Make it precious and near to Thee through Thy bounty and bestowal.",
     118, 0},
    {"O Lord, my God! This is a child...",
     "Bahá'u'lláh",
     "O\n"
        "Lord, my God!\n"
        "This is a child that hath sprung from the loins of one of Thy servants to whom Thou hast granted\n"
        "a distinguished station in the Tablets of Thine irrevocable decree in the Books of Thy behest.\n"
        "\n"
        "I beseech Thee by Thy name, whereby everyone is enabled to attain the object of his desire, to\n"
        "grant that this child may become a more mature soul amongst Thy servants; cause him to shine forth\n"
        "through the power of Thy name, enable him to utter Thy praise, to set his face towards Thee and to\n"
        "draw nigh unto Thee.\n"
        "Verily it is Thou Who hast, from everlasting, been powerful to do as Thou willest and Who wilt,\n"
        "to eternity, remain potent to do as Thou pleasest.\n"
        "There is none other God but Thee, the Exalted, the August, the Subduer, the Mighty, the All-Compelling.",
     144, 0},
    {"Prayer for the Dead",
     "Bahá'u'lláh",
     "O\n"
        "my God!\n"
        "This is Thy servant and the son of Thy servant who hath believed in Thee and in Thy signs, and set his face towards Thee,\n"
        "wholly detached from all except Thee.\n"
        "Thou art, verily, of those who show mercy the most merciful.\n"
        "\n"
        "Deal with him, O Thou Who forgivest the sins of men and concealest their faults,\n"
        "as beseemeth the heaven of Thy bounty and the ocean of Thy grace.\n"
        "Grant him admission within the precincts of Thy transcendent mercy that was before the foundation of earth and heaven.\n"
        "There is no God but Thee, the Ever-Forgiving, the Most Generous.\n"
        "\n"
        "Let him, then, repeat six times the greeting “Alláh-u-Abhá,”\n"
        "and then repeat nineteen times each of the following verses:\n"
        "\n"
        "We all, verily, worship God.\n"
        "\n"
        "We all, verily, bow down before God.\n"
        "\n"
        "We all, verily, are devoted unto God.\n"
        "\n"
        "We all, verily, give praise unto God.\n"
        "\n"
        "We all, verily, yield thanks unto God.\n"
        "\n"
        "We all, verily, are patient in God.\n"
        "\n"
        "(If the dead be a woman, let him say: This is Thy handmaiden and the daughter of Thy handmaiden, etc...)",
     182, 0},
    {"Suffer me, O my God...",
     "Bahá'u'lláh",
     "Suffer me, O my God, to draw nigh unto Thee,\n"
        "and to abide within the precincts of Thy court, for remoteness from Thee hath well-nigh consumed me.\n"
        "Cause me to rest under the shadow of the wings of Thy grace, for the flame of my separation from Thee hath melted my heart within me.\n"
        "Draw me nearer unto the river that is life indeed, for my soul burneth with thirst in its ceaseless search after Thee.\n"
        "My sighs, O my God, proclaim the bitterness of mine anguish, and the tears I shed attest my love for Thee.\n"
        "\n"
        "I beseech Thee, by the praise wherewith Thou praisest Thyself and the glory wherewith Thou glorifiest Thine own Essence,\n"
        "to grant that we may be numbered among them that have recognized Thee and acknowledged Thy sovereignty in Thy days.\n"
        "Help us then to quaff, O my God, from the fingers of mercy the living waters of Thy loving-kindness,\n"
        "that we may utterly forget all else except Thee, and be occupied only with Thy Self.\n"
        "Powerful art Thou to do what Thou willest.\n"
        "No God is there beside Thee, the Mighty, the Help in Peril, the Self-Subsisting.\n"
        "\n"
        "Glorified be Thy name, O Thou Who art the King of all Kings!",
     206, 0},
    {"Glory to Thee, O my God!",
     "Bahá'u'lláh",
     "Glory to Thee, O my God!\n"
        "The first stirrings of the spring of Thy grace have appeared and clothed Thine earth with verdure.\n"
        "The clouds of the heaven of Thy bounty have rained their rain on this City within whose walls is imprisoned\n"
        "Him Whose desire is the salvation of Thy creatures.\n"
        "Through it the soil of this City hath been decked forth, and its trees clothed with foliage, and its inhabitants gladdened.\n"
        "\n"
        "The hearts of Thy dear ones, however, will rejoice only at the Divine Springtime of Thy tender mercies,\n"
        "whereby the hearts are quickened, and the souls are renewed, and the trees of human existence bear their fruits.\n"
        "\n"
        "The plants that have sprung forth, O my Lord, in the hearts of Thy loved ones have withered away.\n"
        "Send down upon them, from the clouds of Thy spirit, that which will cause the tender herbs of Thy knowledge and\n"
        "wisdom to grow within their breasts.\n"
        "Rejoice, then, their hearts with the proclamation of Thy Cause and the exaltation of Thy sovereignty.\n"
        "\n"
        "Their eyes, O my Lord, are expectantly turned in the direction of Thy bounty, and their faces are set towards the horizon of Thy grace.\n"
        "Suffer them not, through Thy bounty, to be deprived of Thy grace.\n"
        "Potent art Thou, by Thy sovereign might, over all things.\n"
        "No God is there but Thee, the Almighty, the Help in Peril, the Self-Subsisting.",
     233, 0},
    {"O God, my Master, the Goal of my desire!",
     "Bahá'u'lláh",
     "O\n"
        "my God, my Master, the Goal of my desire!\n"
        "This, Thy servant, seeketh to sleep in the shelter of Thy mercy, and to repose beneath the canopy of Thy grace,\n"
        "imploring Thy care and Thy protection.\n"
        "\n"
        "I beg of Thee, O my Lord, by Thine eye that sleepeth not, to guard mine eyes from beholding aught beside Thee.\n"
        "Strengthen, then, their vision that they may discern Thy signs, and behold the Horizon of Thy Revelation.\n"
        "Thou art He before the revelations of Whose omnipotence the quintessence of power hath trembled.\n"
        "\n"
        "No God is there but Thee, the Almighty, the All-Subduing, the Unconditioned.",
     103, 0},
    {"Glory be unto Thee, O Lord my God!",
     "The Báb",
     "Glory be unto Thee, O Lord my God!\n"
        "I beg Thee to forgive me and those who support Thy Faith.\n"
        "Verily, Thou art the sovereign Lord, the Forgiver, the Most Generous.\n"
        "O my God!\n"
        "Enable such servants of Thine as are deprived of knowledge to be admitted into Thy Cause; for once they learn of Thee,\n"
        "they bear witness to the truth of the Day of Judgment and do not dispute the revelations of Thy bounty.\n"
        "Send down upon them the tokens of Thy grace, and grant them, wherever they reside,\n"
        "a liberal share of that which Thou hast ordained for the pious among Thy servants.\n"
        "Thou art in truth the Supreme Ruler, the All-Bounteous, the Most Benevolent.\n"
        "\n"
        "O my God!\n"
        "Let the outpourings of Thy bounty and blessings descend upon homes whose inmates have embraced Thy Faith,\n"
        "as a token of Thy grace and as a mark of loving-kindness from Thy presence.\n"
        "Verily, unsurpassed art Thou in granting forgiveness.\n"
        "Should Thy bounty be withheld from anyone, how could he be reckoned among the followers of the Faith in Thy Day?\n"
        "\n"
        "Bless me, O my God, and those who will believe in Thy signs on the appointed Day, and such as cherish my love in their\n"
        "hearts—a love which Thou dost instill into them.\n"
        "Verily, Thou art the Lord of righteousness, the Most Exalted.",
     224, 0},
    {"Glory be to Thee, O King of eternity...",
     "Bahá'u'lláh",
     "Glory be to Thee, O King of eternity, and the Maker of nations,\n"
        "and the Fashioner of every moldering bone!\n"
        "I pray Thee, by Thy Name through which Thou didst call all mankind unto the horizon of Thy majesty and glory,\n"
        "and didst guide Thy servants to the court of Thy grace and favors, to number me with such as have rid themselves from everything except Thyself,\n"
        "and have set themselves towards Thee, and have not been kept back by such misfortunes as were decreed by Thee,\n"
        "from turning in the direction of Thy gifts.\n"
        "\n"
        "I have laid hold, O my Lord, on the handle of Thy bounty, and clung steadfastly to the hem of the robe of Thy favor.\n"
        "Send down, then, upon me, out of the clouds of Thy generosity, what will purge out from me the remembrance of anyone except Thee,\n"
        "and make me able to turn unto Him Who is the Object of the adoration of all mankind, against Whom have been arrayed the stirrers of sedition,\n"
        "who have broken Thy covenant, and disbelieved in Thee and in Thy signs.\n"
        "\n"
        "Deny me not, O my Lord, the fragrances of Thy raiment in Thy days, and deprive me not of the breathings of Thy Revelation at the\n"
        "appearance of the splendors of the light of Thy face.\n"
        "Powerful art Thou to do what pleaseth Thee.\n"
        "Naught can resist Thy will, nor frustrate what Thou hast purposed by Thy power.\n"
        "\n"
        "No God is there but Thee, the Almighty, the All-Wise.",
     252, 0},
    {"Glorified art Thou, O Lord my God! I beseech Thee.",
     "Bahá'u'lláh",
     "Glorified art Thou, O Lord my God! I beseech Thee by Thy Chosen Ones,\n"
        "and by the Bearers of Thy Trust, and by Him Whom Thou hast ordained to be the Seal of Thy Prophets and of Thy Messengers,\n"
        "to let Thy remembrance be my companion, and Thy love my aim, and Thy face my goal, and Thy name my lamp, and Thy wish my desire,\n"
        "and Thy pleasure my delight.\n"
        "\n"
        "I am a sinner, O my Lord, and Thou art the Ever-Forgiving.\n"
        "As soon as I recognized Thee, I hastened to attain the exalted court of Thy loving-kindness.\n"
        "Forgive me, O my Lord, my sins which have hindered me from walking in the ways of Thy good pleasure,\n"
        "and from attaining the shores of the ocean of Thy oneness.\n"
        "\n"
        "There is no one, O my Lord, who can deal bountifully with me to whom I can turn my face,\n"
        "and none who can have compassion on me that I may crave his mercy.\n"
        "Cast me not out, I implore Thee, of the presence of Thy grace, neither do Thou withhold from me the outpourings of Thy generosity and bounty.\n"
        "Ordain for me, O my Lord, what Thou hast ordained for them that love Thee, and write down for me what Thou hast written down for Thy chosen ones.\n"
        "My gaze hath, at all times, been fixed on the horizon of Thy gracious providence, and mine eyes bent upon the court of Thy tender mercies.\n"
        "Do with me as beseemeth Thee.\n"
        "No God is there but Thee, the God of power, the God of glory, Whose help is implored by all men.",
     272, 0},
    {"O God, my God!",
     "'Abdu'l-Bahá",
     "All the friends of God... should contribute to the extent possible, however modest their offering may be.\n"
        "God doth not burden a soul beyond its capacity.\n"
        "Such contributions must come from all centers and all believers....\n"
        "O Friends of God!\n"
        "Be ye assured that in place of these contributions, your agriculture, your industry, and your commerce will be blessed by manifold increases,\n"
        "with goodly gifts and bestowals.\n"
        "He who cometh with one goodly deed will receive a tenfold reward.\n"
        "There is no doubt that the living Lord will abundantly confirm those who expend their wealth in His path.\n"
        "\n"
        "O\n"
        "God, my God!\n"
        "Illumine the brows of Thy true lovers, and support them with angelic hosts of certain triumph.\n"
        "Set firm their feet on Thy straight path, and out of Thine ancient bounty open before them the portals of Thy blessings;\n"
        "for they are expending on Thy pathway what Thou hast bestowed upon them, safeguarding Thy Faith,\n"
        "putting their trust in their remembrance of Thee, offering up their hearts for love of Thee,\n"
        "and withholding not what they possess in adoration for Thy Beauty and in their search for ways to please Thee.\n"
        "\n"
        "O my Lord!\n"
        "Ordain for them a plenteous share, a destined recompense and sure reward.\n"
        "\n"
        "Verily, Thou art the Sustainer, the Helper, the Generous, the Bountiful, the Ever-Bestowing.",
     220, 0},
    {"Glorified art Thou, O Lord my God!",
     "Bahá'u'lláh",
     "Glorified art Thou, O Lord my God!\n"
        "I implore Thee by the onrushing winds of Thy grace, and by them Who are the Daysprings of Thy purpose and the Dawning-Places of Thine inspiration,\n"
        "to send down upon me and upon all that have sought Thy face that which beseemeth Thy generosity and bountiful grace,\n"
        "and is worthy of Thy bestowals and favors.\n"
        "Poor and desolate I am, O my Lord!\n"
        "Immerse me in the ocean of Thy wealth; athirst, suffer me to drink from the living waters of Thy loving-kindness.\n"
        "\n"
        "I beseech Thee, by Thine own Self and by Him Whom Thou hast appointed as the Manifestation of Thine own Being and\n"
        "Thy discriminating Word unto all that are in heaven and on earth, to gather together Thy servants beneath the shade of the\n"
        "Tree of Thy gracious providence.\n"
        "Help them, then, to partake of its fruits, to incline their ears to the rustling of its leaves,\n"
        "and to the sweetness of the voice of the Bird that chanteth upon its branches.\n"
        "Thou art, verily, the Help in Peril, the Inaccessible, the Almighty, the Most Bountiful.",
     186, 0},
    {"He is God! Thou seest us, O my God...",
     "'Abdu'l-Bahá",
     "He is God!\n"
        "Thou seest us, O my God, gathered around this table, praising Thy bounty, with our gaze set upon Thy Kingdom.\n"
        "O Lord!\n"
        "Send down upon us Thy heavenly food and confer upon us Thy blessing.\n"
        "Thou art verily the Bestower, the Merciful, the Compassionate.",
     47, 0},
    {"O God my God! I beg of Thee...",
     "Bahá'u'lláh",
     "O\n"
        "God my God!\n"
        "I beg of Thee by the ocean of Thy healing, and by the splendors of the Daystar of Thy grace,\n"
        "and by Thy Name through which Thou didst subdue Thy servants,\n"
        "and by the pervasive power of Thy most exalted Word and the potency of Thy most august Pen,\n"
        "and by Thy mercy that hath preceded the creation of all who are in heaven and on earth,\n"
        "to purge me with the waters of Thy bounty from every affliction and disorder, and from all weakness and feebleness.\n"
        "\n"
        "Thou seest, O my Lord, Thy suppliant waiting at the door of Thy bounty,\n"
        "and him who hath set his hopes on Thee clinging to the cord of Thy generosity.\n"
        "Deny him not, I beseech Thee, the things he seeketh from the ocean of Thy grace and the Daystar of Thy loving-kindness.\n"
        "\n"
        "Powerful art Thou to do what pleaseth Thee.\n"
        "There is none other God save Thee, the Ever-Forgiving, the Most Generous.",
     163, 0},
    {"My God, Whom I worship and adore!",
     "Bahá'u'lláh",
     "My God, Whom I worship and adore!\n"
        "I bear witness unto Thy unity and Thy oneness, and acknowledge Thy gifts, both in the past and in the present.\n"
        "Thou art the All-Bountiful, the overflowing showers of Whose mercy have rained down upon high and low alike,\n"
        "and the splendors of Whose grace have been shed over both the obedient and the rebellious.\n"
        "\n"
        "O God of mercy, before Whose door the quintessence of mercy hath bowed down, and round the sanctuary of Whose Cause loving-kindness,\n"
        "in its inmost spirit, hath circled, we beseech Thee, entreating Thine ancient grace, and seeking Thy present favor,\n"
        "that Thou mayest have mercy upon all who are the manifestations of the world of being,\n"
        "and deny them not the outpourings of Thy grace in Thy days.\n"
        "\n"
        "All are but poor and needy, and Thou, verily, art the All-Possessing, the All-Subduing, the All-Powerful.",
     146, 0},
    {"Thy unity is inscrutable, O my God...",
     "Bahá'u'lláh",
     "Thy unity is inscrutable, O my God,\n"
        "to all except them that have recognized Him Who is the Manifestation of Thy singleness and the Day-Spring of Thy oneness.\n"
        "Whoso assigneth a rival unto Him hath assigned a rival unto Thee, and whoso hath set up a peer for Him hath set up a peer for Thyself.\n"
        "No, no, none can withstand Thee in the whole of creation.\n"
        "Thou hast everlastingly been exalted far above all comparison and likeness.\n"
        "Thy oneness hath been demonstrated by the oneness of Him Who is the Dawning-Place of Thy Revelation.\n"
        "Whosoever denieth this, hath denied Thy unity, and disputed with Thee about Thy sovereignty,\n"
        "and contended with Thee in Thy realm, and repudiated Thy commandments.\n"
        "\n"
        "Assist Thou Thy servants, O my Lord, to recognize Thy unity and to declare Thy oneness,\n"
        "that all may gather together around what Thou didst desire in this Day whereon the sun of Thine essence hath shone forth above the horizon of Thy will,\n"
        "and the moon of Thine own being hath risen from the Day-Spring of Thy behest.\n"
        "Thou art He, O my Lord, from Whose knowledge nothing whatsoever escapeth, and Whom no one can frustrate.\n"
        "Thou doest Thy pleasure, by Thy sovereignty that overshadoweth the worlds.\n"
        "\n"
        "Thou well knowest, O my God, my Best-Beloved, that naught can quench the thirst I suffer in my separation from Thee except the waters of Thy presence,\n"
        "and that the tumult of my heart can never be stilled save through the living fountain of my reunion with Thee.\n"
        "Send down, then, upon me, O my Lord, out of the heaven of Thy bounty what will draw me nearer unto the chalice of Thy gifts,\n"
        "and make me able to quaff the choice sealed Wine, Whose seal hath been loosed in Thy name,\n"
        "and from Which the sweet savors of Thy days have been shed abroad.\n"
        "Thou, in truth, art the All-Bountiful, Whose grace is infinite.\n"
        "\n"
        "The whole universe testifieth to Thy generosity.\n"
        "Have mercy, then, upon me by Thy graciousness, and deal bountifully with me through the power of Thy sovereignty,\n"
        "and suffer me to enjoy near access to Thee by Thy manifold favors.\n"
        "Thou, truly, art the Great Giver, the Almighty, the Ever-Forgiving, the Most Bountiful.",
     375, 0},
    {"He is the Bestower, the Bounteous!",
     "Bahá'u'lláh",
     "The pledge of marriage, the verse to be spoken individually by the bride and the bridegroom in the presence\n"
        "of at least two witnesses acceptable to the Spiritual Assembly is, as stipulated in the Kitáb-i-Aqdas (The Most Holy Book):\n"
        "\n"
        "“We will, all, verily, abide by the Will of God.”\n"
        "\n"
        "He is the Bestower, the Bounteous!\n"
        "\n"
        "Praise be to God, the Ancient, the Ever-Abiding, the Changeless, the Eternal!\n"
        "He Who hath testified in His Own Being that verily He is the One, the Single, the Untrammelled, the Exalted.\n"
        "We bear witness that verily there is no God but Him, acknowledging His oneness, confessing His singleness.\n"
        "He hath ever dwelt in unapproachable heights, in the summits of His loftiness, sanctified from the mention of aught save Himself,\n"
        "free from the description of aught but Him.\n"
        "\n"
        "And when He desired to manifest grace and beneficence to men, and to set the world in order,\n"
        "He revealed observances and created laws; among them He established the law of marriage,\n"
        "made it as a fortress for well-being and salvation,\n"
        "and enjoined it upon us in that which was sent down out of the heaven of sanctity in His Most Holy Book.\n"
        "He saith, great is His glory: “Enter into wedlock, O people, that ye may bring forth one who will make mention of me amid My servants.\n"
        "This is My bidding unto you; hold fast to it as an assistance to yourselves.”",
     237, 0},
    {"O my God and my Master!",
     "Bahá'u'lláh",
     "O\n"
        "my God and my Master!\n"
        "I am Thy servant and the son of Thy servant.\n"
        "I have risen from my couch at this dawntide when the Daystar of Thy oneness hath shone forth from the Dayspring of Thy will,\n"
        "and hath shed its radiance upon the whole world, according to what had been ordained in the Books of Thy Decree.\n"
        "\n"
        "Praise be unto Thee, O my God, that we have wakened to the splendors of the light of Thy knowledge.\n"
        "Send down, then, upon us, O my Lord, what will enable us to dispense with anyone but Thee,\n"
        "and will rid us of all attachment to aught except Thyself.\n"
        "Write down, moreover, for me, and for such as are dear to me, and for my kindred, man and woman alike,\n"
        "the good of this world and the world to come.\n"
        "Keep us safe, then, through Thine unfailing protection, O Thou the Beloved of the entire creation and the Desire of the whole universe,\n"
        "from them whom Thou hast made to be the manifestations of the Evil Whisperer, who whisper in men's breasts.\n"
        "Potent art Thou to do Thy pleasure.\n"
        "Thou art, verily, the Almighty, the Help in Peril, the Self-Subsisting.\n"
        "\n"
        "Bless Thou, O Lord my God, Him Whom Thou hast set over Thy most excellent Titles,\n"
        "and through Whom Thou hast divided between the godly and the wicked, and graciously aid us to do what Thou lovest and desirest.\n"
        "Bless Thou, moreover, O my God, them Who are Thy Words and Thy Letters,\n"
        "and them who have set their faces towards Thee, and turned unto Thy face, and hearkened to Thy Call.\n"
        "\n"
        "Thou art, truly, the Lord and King of all men, and art potent over all things.",
     289, 0},
    {"Glory be to Thee, O my God!",
     "Bahá'u'lláh",
     "Glory be to Thee, O my God!\n"
        "Thou hearest Thine ardent lovers lamenting in their separation from Thee,\n"
        "and such as have recognized Thee wailing because of their remoteness from Thy presence.\n"
        "Open Thou outwardly to their faces, O my Lord, the gates of Thy grace, that they may enter them by Thy leave and in conformity with Thy will,\n"
        "and may stand before the throne of Thy majesty,\n"
        "and catch the accents of Thy voice, and be illumined with the splendors of the light of Thy face.\n"
        "\n"
        "Potent art Thou to do what pleaseth Thee.\n"
        "None can withstand the power of Thy sovereign might.\n"
        "From everlasting Thou wert alone, with none to equal Thee, and wilt unto everlasting remain far above all thought and every description of Thee.\n"
        "Have mercy, then, upon Thy servants by Thy grace and bounty, and suffer them not to be kept back from the shores of the ocean of Thy nearness.\n"
        "If Thou abandonest them, who is there to befriend them; and if Thou puttest them far from Thee,\n"
        "who is he that can favor them?\n"
        "They have none other Lord beside Thee, none to adore except Thyself.\n"
        "Deal Thou generously with them by Thy bountiful grace.\n"
        "\n"
        "Thou, in truth, art the Ever-Forgiving, the Most Compassionate.",
     212, 0},
    {"Unto Thee be praise, O Lord my God!",
     "Bahá'u'lláh",
     "Unto Thee be praise, O Lord my God!\n"
        "I entreat Thee, by Thy signs that have encompassed the entire creation,\n"
        "and by the light of Thy countenance that hath illuminated all that are in heaven and on earth,\n"
        "and by Thy mercy that hath surpassed all created things, and by Thy grace that hath suffused the whole universe,\n"
        "to rend asunder the veils that shut me out from Thee, that I may hasten unto the Fountain-Head of Thy mighty inspiration,\n"
        "and to the Day-Spring of Thy Revelation and bountiful favors, and may be immersed beneath the ocean of Thy nearness and pleasure.\n"
        "\n"
        "Suffer me not, O my Lord, to be deprived of the knowledge of Thee in Thy days, and divest me not of the robe of Thy guidance.\n"
        "Give me to drink of the river that is life indeed, whose waters have streamed forth from the Paradise (Ridván)\n"
        "in which the throne of Thy Name, the All-Merciful, was established, that mine eyes may be opened, and my face be illumined,\n"
        "and my heart be assured, and my soul be enlightened, and my steps be made firm.\n"
        "\n"
        "Thou art He Who from everlasting was, through the potency of His might, supreme over all things, and,\n"
        "through the operation of His will, was able to ordain all things.\n"
        "Nothing whatsoever, whether in Thy heaven or on Thy earth, can frustrate Thy purpose.\n"
        "Have mercy, then, upon me, O my Lord, through Thy gracious providence and generosity,\n"
        "and incline mine ear to the sweet melodies of the birds that warble their praise of Thee, amidst the branches of the tree of Thy oneness.\n"
        "\n"
        "Thou art the Great Giver, the Ever-Forgiving, the Most Compassionate.",
     281, 0},
    {"All praise, O my God, be to Thee...",
     "Bahá'u'lláh",
     "All praise, O my God, be to Thee Who art the Source of all glory and majesty,\n"
        "of greatness and honor, of sovereignty and dominion, of loftiness and grace, of awe and power.\n"
        "Whomsoever Thou willest Thou causest to draw nigh unto the Most Great Ocean,\n"
        "and on whomsoever Thou desirest Thou conferrest the honor of recognizing Thy Most Ancient Name.\n"
        "Of all who are in heaven and on earth, none can withstand the operation of Thy sovereign Will.\n"
        "From all eternity Thou didst rule the entire creation, and Thou wilt continue for evermore to exercise Thy dominion over all created things.\n"
        "There is none other God but Thee, the Almighty, The Most Exalted, the All-Powerful, the All-Wise.\n"
        "\n"
        "Illumine, O Lord, the faces of Thy servants, that they may behold Thee;\n"
        "and cleanse their hearts that they may turn unto the court of Thy heavenly favors,\n"
        "and recognize Him Who is the Manifestation of Thy Self and the Dayspring of Thine Essence.\n"
        "Verily, Thou art the Lord of all worlds.\n"
        "There is no God but Thee, the Unconstrained, the All-Subduing.",
     180, 0},
    {"Lauded be Thy name, O my God! Thou beholdest me...",
     "Bahá'u'lláh",
     "Lauded be Thy name, O my God!\n"
        "Thou beholdest me in the clutches of my oppressors.\n"
        "Every time I turn to my right, I hear the voice of the lamentation of them that are dear to Thee,\n"
        "whom the infidels have made captives for having believed in Thee and in Thy signs,\n"
        "and for having set their faces towards the horizon of Thy grace and of Thy loving-kindness.\n"
        "And when I turn to my left, I hear the clamor of the wicked doers who have disbelieved in Thee and in Thy signs,\n"
        "and persistently striven to put out the light of Thy lamp which sheddeth the radiance of Thine own Self\n"
        "over all that are in Thy heaven and all that are on Thy earth.\n"
        "\n"
        "The hearts of Thy chosen ones, O my Lord, have melted because of their separation from Thee,\n"
        "and the souls of Thy loved ones are burnt up by the fire of their yearning after Thee in Thy days.\n"
        "I implore Thee, O Thou Maker of the heavens and Lord of all names,\n"
        "by Thy most effulgent Self and Thy most exalted and all-glorious Remembrance,\n"
        "to send down upon Thy loved ones that which will draw them nearer unto Thee, and enable them to hearken unto Thine utterances.\n"
        "\n"
        "Tear asunder with the hand of Thy transcendent power, O my Lord, the veil of vain imaginings,\n"
        "that they who are wholly devoted to Thee may see Thee seated on the throne of Thy majesty,\n"
        "and the eyes of such as adore Thy unity may rejoice at the splendors of the glory of Thy face.\n"
        "The doors of hope have been shut against the hearts that long for Thee, O my Lord!\n"
        "Their keys are in Thy hands; open them by the power of Thy might and Thy sovereignty.\n"
        "Potent art Thou to do as Thou pleasest.\n"
        "Thou art, verily, the Almighty, the Beneficent.",
     316, 0},
    {"Praise be to Thee, O Lord my God!",
     "Bahá'u'lláh",
     "Praise be to Thee, O Lord my God!\n"
        "Thou seest and knowest that I have called upon Thy servants to turn nowhere except in the direction of Thy bestowals,\n"
        "and have bidden them observe naught save the things Thou didst prescribe in Thy Perspicuous Book,\n"
        "the Book which hath been sent down according to Thine inscrutable decree and irrevocable purpose.\n"
        "\n"
        "I can utter no word, O my God, unless I be permitted by Thee, and can move in no direction until I obtain Thy sanction.\n"
        "It is Thou, O my God, Who hast called me into being through the power of Thy might, and hast endued me with Thy grace to manifest Thy Cause.\n"
        "Wherefore I have been subjected to such adversities that my tongue hath been hindered from extolling Thee and from magnifying Thy glory.\n"
        "\n"
        "All praise be to Thee, O my God, for the things Thou didst ordain for me through Thy decree and by the power of Thy sovereignty.\n"
        "I beseech Thee that thou wilt fortify both myself and them that love me in our love for Thee, and wilt keep us firm in Thy Cause.\n"
        "I swear by Thy might!\n"
        "O my God!\n"
        "Thy servant's shame is to be shut out as by a veil from Thee, and his glory is to know Thee.\n"
        "Armed with the power of Thy name nothing can ever hurt me, and with Thy love in my heart all the world's afflictions can in no wise alarm me.\n"
        "\n"
        "Send down, therefore, O my Lord, upon me and upon my loved ones that which will protect us from the\n"
        "mischief of those that have repudiated Thy truth and disbelieved in Thy signs.\n"
        "\n"
        "Thou art, verily, the All-Glorious, the Most Bountiful.",
     288, 0},
    {"Magnified be Thy name, O God...",
     "Bahá'u'lláh",
     "Magnified be Thy name, O God, the Lord of heaven!\n"
        "Attire my head with the crown of martyrdom, even as Thou didst attire my body with the\n"
        "ornament of tribulation before all that dwell in Thy land.\n"
        "Grant, moreover, that they whose hearts yearn over Thee may draw nigh unto the horizon of\n"
        "Thy grace, above which the Day-Star of Thy beauty sheddeth its radiance.\n"
        "Ordain, also, for them what will make them rich enough to dispense with aught else except\n"
        "Thee, and rid them of all attachment to such as have repudiated Thy signs.\n"
        "\n"
        "There is none other God but Thee, the Guardian, the Self-Subsisting.",
     106, 0},
    {"O God, and the God of all Names...",
     "Bahá'u'lláh",
     "O God and the God of all Names, and the Maker of the heavens!\n"
        "I entreat Thee by Thy Name through which He Who is the Dayspring of Thy might and the\n"
        "Dawning-Place of Thy power hath been manifested, through which every solid thing hath been made to\n"
        "flow, and every dead corpse hath been quickened, and every moving spirit confirmed—I entreat\n"
        "Thee to enable me to rid myself of all attachment to any one but Thee, and to serve Thy Cause,\n"
        "and to wish what Thou didst wish through the power of Thy sovereignty, and to perform what is\n"
        "the good pleasure of Thy will.\n"
        "\n"
        "I beseech Thee, moreover, O my God, to ordain for me what will make me rich enough to dispense with any one save Thee.\n"
        "Thou seest me, O my God, with my face turned towards Thee, and my hands clinging to the cord of Thy grace.\n"
        "Send down upon me Thy mercy, and write down for me what Thou hast written down for Thy chosen ones.\n"
        "Powerful art Thou to do what pleaseth Thee.\n"
        "No God is there but Thee, the Ever-Forgiving, the All-Bountiful.",
     190, 0},
    {"From the sweet-scented streams...",
     "Bahá'u'lláh",
     "From the sweet-scented streams of Thine eternity give me to drink,\n"
        "O my God, and of the fruits of the tree of Thy being enable me to taste, O my Hope!\n"
        "From the crystal springs of Thy love suffer me to quaff, O my Glory,\n"
        "and beneath the shadow of Thine everlasting providence let me abide, O my Light!\n"
        "Within the meadows of Thy nearness, before Thy presence, make me able to roam, O my Beloved,\n"
        "and at the right hand of the throne of Thy mercy, seat me, O my Desire!\n"
        "From the fragrant breezes of Thy joy let a breath pass over me, O my Goal,\n"
        "and into the heights of the paradise of Thy reality let me gain admission, O my Adored One!\n"
        "To the melodies of the dove of Thy oneness suffer me to hearken, O Resplendent One,\n"
        "and through the spirit of Thy power and Thy might quicken me, O my Provider!\n"
        "In the spirit of Thy love keep me steadfast, O my Succorer,\n"
        "and in the path of Thy good pleasure set firm my steps, O my Maker!\n"
        "Within the garden of Thine immortality, before Thy countenance, let me abide for ever,\n"
        "O Thou Who art merciful unto me, and upon the seat of Thy glory stablish me,\n"
        "O Thou Who art my Possessor!\n"
        "To the heaven of Thy loving-kindness lift me up, O my Quickener,\n"
        "and unto the Daystar of Thy guidance lead me, O Thou my Attractor!\n"
        "Before the revelations of Thine invisible spirit summon me to be present,\n"
        "O Thou Who art my Origin and my Highest Wish, and unto the essence of the fragrance of Thy beauty,\n"
        "which Thou wilt manifest, cause me to return, O Thou Who art my God!\n"
        "\n"
        "Potent art Thou to do what pleaseth Thee.\n"
        "Thou art, verily, the Most Exalted, the All-Glorious, the All-Highest.",
     309, 0},
    {"I magnify Thy Name, O my God...",
     "Bahá'u'lláh",
     "I\n"
        "magnify Thy Name, O my God, and offer thanksgiving unto Thee, O my Desire, inasmuch as Thou hast\n"
        "enabled me to clearly perceive Thy straight Path, hast unveiled Thy Great Announcement before\n"
        "mine eyes and hast aided me to set my face towards the Dayspring of Thy Revelation and the\n"
        "Fountainhead of Thy Cause, whilst Thy servants and Thy people turned away from Thee.\n"
        "I entreat Thee, O Lord of the Kingdom of eternity, by the shrill voice of the Pen of Glory,\n"
        "and by the Burning Fire which calleth aloud from the verdant Tree, and by the Ark which Thou\n"
        "hast specially chosen for the people of Bahá, to grant that I may remain steadfast in\n"
        "my love for Thee, be well pleased with whatsoever Thou hast prescribed for me in Thy Book\n"
        "and may stand firm in Thy service and in the service of Thy loved ones.\n"
        "Graciously assist then Thy servants, O my God, to do that which will serve to exalt Thy\n"
        "Cause and will enable them to observe whatsoever Thou hast revealed in Thy Book.\n"
        "\n"
        "Verily Thou art the Lord of Strength, Thou art potent to ordain whatsoever Thou willest and\n"
        "within Thy grasp Thou holdest the reins of all created things.\n"
        "No God is there but Thee, the All-Powerful, the All-Knowing, the All-Wise.",
     220, 0},
    {"Magnified be Thy name...",
     "Bahá'u'lláh",
     "Magnified be Thy name, O my God,\n"
        "for that Thou hast manifested the Day which is the King of Days,\n"
        "the Day which Thou didst announce unto Thy chosen Ones and Thy Prophets in Thy most excellent Tablets,\n"
        "the Day whereon Thou didst shed the splendor of the glory of all Thy names upon all created things.\n"
        "Great is his blessedness whosoever hath set himself towards Thee,\n"
        "and entered Thy presence, and caught the accents of Thy voice.\n"
        "\n"
        "I beseech Thee, O my Lord, by the name of Him round Whom circleth in adoration the kingdom of Thy names,\n"
        "that Thou wilt graciously assist them that are dear to Thee to glorify Thy word among Thy servants,\n"
        "and to shed abroad Thy praise amidst Thy creatures,\n"
        "so that the ecstasies of Thy revelation may fill the souls of all the dwellers of Thine earth.\n"
        "\n"
        "Since Thou hast guided them, O my Lord, unto the living waters of Thy grace, grant,\n"
        "by Thy bounty, that they may not be kept back from Thee;\n"
        "and since Thou hast summoned them to the habitation of Thy throne,\n"
        "drive them not out from Thy presence, through Thy loving-kindness.\n"
        "Send down upon them what shall wholly detach them from aught else except Thee,\n"
        "and make them able to soar in the atmosphere of Thy nearness,\n"
        "in such wise that neither the ascendancy of the oppressor nor the\n"
        "suggestions of them that have disbelieved in Thy most august and most mighty\n"
        "Self shall be capable of keeping them back from Thee.",
     255, 0},
    {"O Thou incomparable God!",
     "'Abdu'l-Bahá",
     "O\n"
        "Thou incomparable God!\n"
        "O Thou Lord of the Kingdom!\n"
        "These souls are Thy heavenly army.\n"
        "Assist them and, with the cohorts of the Supreme Concourse, make them victorious,\n"
        "so that each one of them may become like unto a regiment and conquer these countries through the love of\n"
        "God and the illumination of divine teachings.\n"
        "\n"
        "O God!\n"
        "Be Thou their supporter and their helper, and in the wilderness, the mountain, the valley,\n"
        "the forests, the prairies and the seas, be Thou their confidant—so that they may cry out through the power of the\n"
        "Kingdom and the breath of the Holy Spirit.\n"
        "\n"
        "Verily, Thou art the Powerful, the Mighty and the Omnipotent, and Thou art the Wise, the Hearing and the Seeing.",
     122, 0},
    {"O Thou Whose tests are a healing medicine...",
     "Bahá'u'lláh",
     "O\n"
        "Thou Whose tests are a healing medicine to such as are nigh unto Thee,\n"
        "Whose sword is the ardent desire of all them that love Thee,\n"
        "Whose dart is the dearest wish of those hearts that yearn after Thee,\n"
        "Whose decree is the sole hope of them that have recognized Thy truth!\n"
        "I implore Thee, by Thy divine sweetness and by the splendors of the glory of Thy face,\n"
        "to send down upon us from Thy retreats on high that which will enable us to draw nigh unto Thee.\n"
        "Set, then, our feet firm, O my God, in Thy Cause, and enlighten our hearts with the effulgence of Thy knowledge,\n"
        "and illumine our breasts with the brightness of Thy names.",
     121, 0},
    {"Praise be to Thee, O Lord my God!",
     "Bahá'u'lláh",
     "Praise be to Thee, O Lord my God!\n"
        "I swear by Thy might!\n"
        "Successive afflictions have withheld the pen of the Most High from laying bare that which is\n"
        "hidden from the eyes of Thy creatures, and incessant trials have hindered the tongue of the\n"
        "Divine Ordainer from proclaiming the wonders of Thy glorification and praise.\n"
        "With a stammering tongue, therefore, I call upon Thee, O my God, and with this my\n"
        "afflicted pen I occupy myself in remembrance of Thy name.\n"
        "\n"
        "Is there any man of insight, O my God, that can behold Thee with Thine own eye, and where is\n"
        "the thirsty one who can direct his face towards the living waters of Thy love?\n"
        "I am the one, O my God, who hath blotted out from his heart the remembrance of all except\n"
        "Thee, and hath graven upon it the mysteries of Thy love.\n"
        "Thine own might beareth me witness!\n"
        "But for tribulations, how could the assured be distinguished from the doubters among Thy servants?\n"
        "They who have been inebriated with the wine of Thy knowledge, these, verily, hasten to\n"
        "meet every manner of adversity in their longing to pass into Thy presence.\n"
        "I implore Thee, O Beloved of my heart and the Object of my soul's adoration, to shield\n"
        "them that love me from the faintest trace of evil and corrupt desires.\n"
        "Supply them, then, with the good of this world and of the next.\n"
        "\n"
        "Thou art, verily, He Whose grace hath guided them aright, He Who hath declared Himself to be the All-Merciful.\n"
        "No God is there but Thee, the All-Glorious, the Supreme Helper.",
     269, 0},
    {"Thou seest, O my God...",
     "Bahá'u'lláh",
     "Thou seest, O my God, how Thy loved ones have been encompassed by the\n"
        "rebellious amongst Thy creatures and the wicked amidst Thy people.\n"
        "No land remaineth but the lamentation of Thy lovers and the wailing of Thy chosen ones were lifted up therein.\n"
        "I beseech Thee by Thy Most Great Name to draw forth the hand of power from the bosom of Thy\n"
        "might and to assist therewith all them that love Thee.\n"
        "\n"
        "Thou beholdest, O my God, their eyes turned towards Thee, their gaze fixed upon the Dayspring of Thy might and Thy loving providence.\n"
        "Transmute, O my Lord, their abasement into glory, their poverty into wealth, and their weakness into a strength born of Thee.\n"
        "\n"
        "Powerful art Thou to do whatsoever Thou willest.\n"
        "No God is there but Thee, the All-Knowing, the All-Informed.",
     136, 0},
    {"O my God! O my God! Unite the hearts...",
     "Bahá'u'lláh",
     "O\n"
        "my God!\n"
        "O my God!\n"
        "Unite the hearts of Thy servants, and reveal to them Thy great purpose.\n"
        "May they follow Thy commandments and abide in Thy law.\n"
        "Help them, O God, in their endeavor, and grant them strength to serve Thee.\n"
        "O God!\n"
        "Leave them not to themselves, but guide their steps by the light of Thy knowledge, and cheer their hearts by Thy love.\n"
        "Verily, Thou art their Helper and their Lord.",
     75, 0},
    {"O Thou Whose face is the object...",
     "Bahá'u'lláh",
     "O\n"
        "Thou Whose face is the object of the adoration of all that yearn after Thee, Whose presence is\n"
        "the hope of such as are wholly devoted to Thy will, Whose nearness is the desire of all that have\n"
        "drawn nigh unto Thy court, Whose countenance is the companion of those who have recognized Thy\n"
        "truth, Whose name is the mover of the souls that long to behold Thy face, Whose voice is the true\n"
        "life of Thy lovers, the words of Whose mouth are as the waters of life unto all who are in heaven\n"
        "and on earth!\n"
        "\n"
        "I beseech Thee, by the wrong Thou hast suffered and the ills inflicted upon Thee by the hosts\n"
        "of wrongful doers, to send down upon me from the clouds of Thy mercy that which will purify me of\n"
        "all that is not of Thee, that I may be worthy to praise Thee and fit to love Thee.\n"
        "\n"
        "Withhold not from me, O my Lord, the things Thou didst ordain for such of Thy handmaidens as\n"
        "circle around Thee, and on whom are poured continually the splendors of the sun of Thy beauty and\n"
        "the beams of the brightness of Thy face.\n"
        "Thou art He Who from everlasting hath succored whosoever hath sought Thee, and bountifully favored\n"
        "him who hath asked Thee.\n"
        "\n"
        "No God is there beside Thee, the Mighty, the Ever-Abiding, the All-Bounteous, the Most Generous.",
     235, 0},
    {"O Thou kind Lord! From the horizon...",
     "'Abdu'l-Bahá",
     "O\n"
        "Thou kind Lord!\n"
        "From the horizon of detachment Thou hast manifested souls that, even as the shining moon, shed\n"
        "radiance upon the realm of heart and soul, rid themselves from the attributes of the world of\n"
        "existence and hastened forth unto the kingdom of immortality.\n"
        "With a drop from the ocean of Thy loving-kindness Thou didst oft-times moisten the gardens of\n"
        "their hearts until they gained incomparable freshness and beauty.\n"
        "The holy fragrance of Thy divine unity was diffused far and wide, shedding its sweet savors over\n"
        "the entire world, causing the regions of the earth to be redolent with perfume.\n"
        "\n"
        "Raise up then, O spirit of Purity, souls who, like those sanctified beings, will become free and\n"
        "pure, will adorn the world of being with a new raiment and a wondrous robe, will seek no one else\n"
        "but Thee, tread no path except the path of Thy good pleasure and will speak of naught but the\n"
        "mysteries of Thy Cause.\n"
        "\n"
        "O Thou kind Lord!\n"
        "Grant that this youth may attain unto that which is the highest aspiration of the holy ones.\n"
        "Endow him with the wings of Thy strengthening grace—wings of detachment and divine aid—that\n"
        "he may soar thereby into the atmosphere of Thy tender mercy, be able to partake of Thy celestial\n"
        "bestowals, may become a sign of divine guidance and a standard of the Concourse on high.\n"
        "Thou art the Potent, the Powerful, the Seeing, the Hearing.",
     241, 0},
};

static Prayer s_occasional[] = {
    {"This is, O my God, the first of the days...",
     "Bahá'u'lláh",
     "The _Kitáb-i-Aqdas_ states: “We have commanded you to pray and fast from the beginning of maturity \\[15 years\\];\n"
        "this is ordained by God, your Lord and the Lord of your forefathers...\n"
        "The traveler, the ailing, those who are with child or giving suck, are not bound by the fast...\n"
        "Abstain from food and drink from sunrise to sundown, and beware lest desire deprive you of this grace that is appointed in the Book.”\n"
        "\n"
        "The period of the Fast is March 2 through March 20.\n"
        "\n"
        "This is, O my God, the first of the days on which Thou hast bidden\n"
        "Thy loved ones to observe the Fast.\n"
        "I ask of Thee by Thy Self and by him who hath fasted out of love for Thee and for Thy good-pleasure—and not out of self and desire,\n"
        "nor out of fear of Thy wrath—and by Thy most excellent names and august attributes,\n"
        "to purify Thy servants from the love of aught except Thee and to draw them nigh unto the Dawning-Place of the lights of\n"
        "Thy countenance and the Seat of the throne of Thy oneness.\n"
        "Illumine their hearts, O my God, with the light of Thy knowledge and brighten their faces with the rays of the\n"
        "Daystar that shineth from the horizon of Thy Will.\n"
        "Potent art Thou to do what pleaseth Thee.\n"
        "No God is there but Thee, the All-Glorious, Whose help is implored by all men.\n"
        "\n"
        "Assist them, O my God, to render Thee victorious and to exalt Thy Word.\n"
        "Suffer them, then, to become as hands of Thy Cause amongst Thy servants,\n"
        "and make them to be revealers of Thy religion and Thy signs amongst mankind,\n"
        "in such wise that the whole world may be filled with Thy remembrance and praise and with Thy proofs and evidences.\n"
        "Thou art, verily, the All-Bounteous, the Most Exalted, the Powerful, the Mighty, and the Merciful.",
     313, 0},
    {"Magnified art Thou, O Lord...",
     "Bahá'u'lláh",
     "Magnified art Thou, O Lord of the entire creation, the One unto Whom all things must turn.\n"
        "With my inner and outer tongues I bear witness that thou hast manifested and revealed Thyself,\n"
        "sent down Thy signs, and proclaimed Thy testimonies.\n"
        "I testify to Thy self-sufficiency from aught else except Thee, and Thy sanctity above all earthly things.\n"
        "I entreat Thee by the transcendent glory of Thy Cause and the supreme potency of Thy Word to grant\n"
        "confirmation unto him who desireth to offer what Thou hast prescribed unto him in Thy Book and to\n"
        "observe that which will shed forth the fragrance of Thine acceptance.\n"
        "Verily Thou art the All-Mighty, the All-Gracious, the All-Forgiving, the All-Generous.",
     117, 0},
    {"Intercalary Days",
     "Bahá'u'lláh",
     "My God, my Fire and my Light!\n"
        "The days which Thou hast named the Ayyám-i-Há¹ in Thy Book have begun,\n"
        "O Thou Who art the King of names, and the Fast which Thy most exalted Pen hath enjoined unto all who are in the\n"
        "kingdom of Thy creation to observe is approaching.\n"
        "I entreat Thee, O my Lord, by these days and by all such as have during that period clung to the cord of Thy commandments,\n"
        "and laid hold on the handle of Thy precepts, to grant that unto every soul may be assigned a place within the precincts of Thy court,\n"
        "and a seat at the revelation of the splendors of the light of Thy countenance.\n"
        "\n"
        "These, O my Lord, are Thy servants whom no corrupt inclination hath kept back from what Thou didst send down in Thy Book.\n"
        "They have bowed themselves before Thy Cause, and received Thy Book with such resolve as is born of Thee,\n"
        "and observed what Thou hadst prescribed unto them, and chosen to follow that which had been sent down by Thee.\n"
        "\n"
        "Thou seest, O my Lord, how they have recognized and confessed whatsoever Thou hast revealed in Thy Scriptures.\n"
        "Give them to drink, O my Lord, from the hands of Thy graciousness the waters of Thine eternity.\n"
        "Write down, then, for them the recompense ordained for him that hath immersed himself in the ocean of Thy presence,\n"
        "and attained unto the choice wine of Thy meeting.\n"
        "\n"
        "I implore Thee, O Thou the King of kings and the Pitier of the downtrodden,\n"
        "to ordain for them the good of this world and of the world to come.\n"
        "Write down for them, moreover, what none of Thy creatures hath discovered,\n"
        "and number them with those who have circled round Thee, and who move about Thy throne in every world of Thy worlds.\n"
        "\n"
        "Thou, truly, art the Almighty, the All-Knowing, the All-Informed.",
     318, 0},
    {"O God, my God! We are servants of Thine...",
     "'Abdu'l-Bahá",
     "O\n"
        "God, my God!\n"
        "We are servants of Thine that have turned with devotion to Thy Holy Face,\n"
        "that have detached ourselves from all besides Thee in this glorious Day.\n"
        "We have gathered in this Spiritual Assembly, united in our views and thoughts,\n"
        "with our purposes harmonized to exalt Thy Word amidst mankind.\n"
        "O Lord, our God!\n"
        "Make us the signs of Thy Divine Guidance, the Standards of Thine exalted Faith amongst men,\n"
        "servants to Thy mighty Covenant, O Thou our Lord Most High, manifestations of Thy Divine Unity in Thine Abhá Kingdom,\n"
        "and resplendent stars shining upon all regions.\n"
        "Lord!\n"
        "Aid us to become seas surging with the billows of Thy wondrous Grace, streams flowing from Thine all-glorious Heights,\n"
        "goodly fruits upon the Tree of Thy heavenly Cause, trees waving through the breezes of Thy Bounty in Thy celestial Vineyard.\n"
        "O God!\n"
        "Make our souls dependent upon the Verses of Thy Divine Unity, our hearts cheered with the outpourings of Thy Grace,\n"
        "that we may unite even as the waves of one sea and become merged together as the rays of Thine effulgent Light;\n"
        "that our thoughts, our views, our feelings may become as one reality, manifesting the spirit of union throughout the world.\n"
        "Thou art the Gracious, the Bountiful, the Bestower, the Almighty, the Merciful, the Compassionate.",
     219, 0},
    {"Tablet of Ahmad",
     "Bahá'u'lláh",
     "He is the King, the All-Knowing, the Wise!\n"
        "Lo, the Nightingale of Paradise singeth upon the twigs of the Tree of Eternity, with holy and sweet melodies,\n"
        "proclaiming to the sincere ones the glad tidings of the nearness of God,\n"
        "calling the believers in the Divine Unity to the court of the Presence of the Generous One,\n"
        "informing the severed ones of the message which hath been revealed by God, the King, the Glorious,\n"
        "the Peerless, guiding the lovers to the seat of sanctity and to this resplendent Beauty.\n"
        "\n"
        "Verily this is that Most Great Beauty, foretold in the Books of the Messengers,\n"
        "through Whom truth shall be distinguished from error and the wisdom of every command shall be tested.\n"
        "Verily He is the Tree of Life that bringeth forth the fruits of God, the Exalted, the Powerful, the Great.\n"
        "\n"
        "O Ahmad!\n"
        "Bear thou witness that verily He is God and there is no God but Him, the King, the Protector, the Incomparable, the Omnipotent.\n"
        "And that the One Whom He hath sent forth by the name of ‘Alí¹ was the true One from God, to Whose commands we are all conforming.\n"
        "\n"
        "Say: O people be obedient to the ordinances of God, which have been enjoined in the Bayán by the Glorious, the Wise One.\n"
        "Verily He is the King of the Messengers and His book is the Mother Book did ye but know.\n"
        "\n"
        "Thus doth the Nightingale utter His call unto you from this prison.\n"
        "He hath but to deliver this clear message.\n"
        "Whosoever desireth, let him turn aside from this counsel and whosoever desireth let him choose the path to his Lord.\n"
        "\n"
        "O people, if ye deny these verses, by what proof have ye believed in God?\n"
        "Produce it, O assemblage of false ones.\n"
        "\n"
        "Nay, by the One in Whose hand is my soul, they are not, and never shall be able to do this, even should they combine to assist one another.\n"
        "\n"
        "O Ahmad!\n"
        "Forget not My bounties while I am absent.\n"
        "Remember My days during thy days, and My distress and banishment in this remote prison.\n"
        "And be thou so steadfast in My love that thy heart shall not waver,\n"
        "even if the swords of the enemies rain blows upon thee and all the heavens and the earth arise against thee.\n"
        "\n"
        "Be thou as a flame of fire to My enemies and a river of life eternal to My loved ones, and be not of those who doubt.\n"
        "\n"
        "And if thou art overtaken by affliction in My path, or degradation for My sake, be not thou troubled thereby.\n"
        "\n"
        "Rely upon God, thy God and the Lord of thy fathers.\n"
        "For the people are wandering in the paths of delusion, bereft of discernment to see God with their own eyes,\n"
        "or hear His Melody with their own ears.\n"
        "Thus have We found them, as thou also dost witness.\n"
        "\n"
        "Thus have their superstitions become veils between them and their own hearts and kept them from the path of God,\n"
        "the Exalted, the Great.\n"
        "\n"
        "Be thou assured in thyself that verily,\n"
        "he who turns away from this Beauty hath also turned away from the\n"
        "Messengers of the past and showeth pride towards God from all eternity to all eternity.\n"
        "\n"
        "Learn well this Tablet, O Ahmad.\n"
        "Chant it during thy days and withhold not thyself therefrom.\n"
        "For verily, God hath ordained for the one who chants it, the reward of a hundred martyrs and a service in both worlds.\n"
        "These favors have We bestowed upon thee as a bounty on Our part and a mercy from Our presence, that thou mayest be of those who are grateful.\n"
        "\n"
        "By God!\n"
        "Should one who is in affliction or grief read this Tablet with absolute sincerity,\n"
        "God will dispel his sadness, solve his difficulties and remove his afflictions.\n"
        "\n"
        "Verily, He is the Merciful, the Compassionate.\n"
        "Praise be to God, the Lord of all the worlds.",
     652, 0},
};

static PrayerCategory CATEGORIES[] = {
    {"Obligatory", s_obligatory, 3},
    {"General", s_general, 35},
    {"Occasional", s_occasional, 5},
};
#define NUM_CATEGORIES 3

/* ═══════════════════════════════════════════════════════════════
 * GLOBALS
 * ═══════════════════════════════════════════════════════════════ */

static const char *MAIN_ITEMS[] = {
    "Prayers (Obligatory)",
    "Prayers (General)",
    "Prayers (Occasional)",
    "All\u00E1h-u-Abh\u00E1 (95x)",
    "Bad\u00ED Calendar",
    "About"
};
#define NUM_MAIN_ITEMS 6

/* ═══════════════════════════════════════════════════════════════
 * MAIN MENU WINDOW
 * ═══════════════════════════════════════════════════════════════ */

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

/* Forward declarations */
static void push_prayer_list(int category_index);
static void push_counter(void);
static void push_badi(void);
static void push_about(void);

static void main_select(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    int row = cell_index->row;
    if (row < NUM_CATEGORIES) {
        push_prayer_list(row);
    } else if (row == 3) {
        push_counter();
    } else if (row == 4) {
        push_badi();
    } else if (row == 5) {
        push_about();
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

/* ═══════════════════════════════════════════════════════════════
 * PRAYER LIST WINDOW (submenu)
 * ═══════════════════════════════════════════════════════════════ */

static Window *s_list_window;
static MenuLayer *s_list_menu;
static int s_current_category;

static uint16_t list_get_num_rows(MenuLayer *menu, uint16_t section, void *data) {
    return CATEGORIES[s_current_category].count;
}

static void list_draw_row(GContext *ctx, const Layer *cell_layer,
                          MenuIndex *cell_index, void *data) {
    Prayer *prayer = &CATEGORIES[s_current_category].items[cell_index->row];
    char subtitle[32];
    snprintf(subtitle, sizeof(subtitle), "%d words \u2014 %s", prayer->word_count, prayer->author);
    menu_cell_basic_draw(ctx, cell_layer, prayer->title, subtitle, NULL);
}

static int16_t list_get_cell_height(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    return 50;  /* taller for title + subtitle */
}

/* Forward declaration */
static void push_reader(int cat, int item);

static void list_select(MenuLayer *menu, MenuIndex *cell_index, void *data) {
    push_reader(s_current_category, cell_index->row);
}

static void list_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);

    s_list_menu = menu_layer_create(bounds);
    menu_layer_set_callbacks(s_list_menu, NULL, (MenuLayerCallbacks) {
        .get_num_rows = list_get_num_rows,
        .draw_row = list_draw_row,
        .get_cell_height = list_get_cell_height,
        .select_click = list_select,
    });
    menu_layer_set_click_config_onto_window(s_list_menu, window);
    menu_layer_set_normal_colors(s_list_menu, BG_COLOR, FG_COLOR);
    menu_layer_set_highlight_colors(s_list_menu, HIGHLIGHT_BG, FG_COLOR);
    layer_add_child(root, menu_layer_get_layer(s_list_menu));
}

static void list_window_unload(Window *window) {
    menu_layer_destroy(s_list_menu);
}

static void push_prayer_list(int category_index) {
    s_current_category = category_index;
    s_list_window = window_create();
    window_set_background_color(s_list_window, BG_COLOR);
    window_set_window_handlers(s_list_window, (WindowHandlers) {
        .load = list_window_load,
        .unload = list_window_unload,
    });
    window_stack_push(s_list_window, true);
}

/* ═══════════════════════════════════════════════════════════════
 * READER WINDOW (scrollable text)
 * ═══════════════════════════════════════════════════════════════ */

static Window *s_reader_window;
static ScrollLayer *s_reader_scroll;
static TextLayer *s_reader_title;
static TextLayer *s_reader_text;
#define READER_BUF_SIZE 16384
static char *s_reader_buf;
static int s_reader_cat;
static int s_reader_item;

static void reader_window_load(Window *window) {
    Layer *root = window_get_root_layer(window);
    GRect bounds = layer_get_bounds(root);

    Prayer *prayer = &CATEGORIES[s_reader_cat].items[s_reader_item];

    /* Allocate reader buffer on heap (not BSS — saves 16KB virtual size) */
    s_reader_buf = malloc(READER_BUF_SIZE);

    /* Title bar */
    GRect title_frame = GRect(0, 0, bounds.size.w, 28);
    s_reader_title = text_layer_create(title_frame);
    text_layer_set_background_color(s_reader_title, HEADER_BG);
    text_layer_set_text_color(s_reader_title, FG_COLOR);
    text_layer_set_font(s_reader_title, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
    text_layer_set_text_alignment(s_reader_title, GTextAlignmentCenter);
    text_layer_set_text(s_reader_title, prayer->title);
    layer_add_child(root, text_layer_get_layer(s_reader_title));

    /* Scroll area below title */
    GRect scroll_frame = GRect(0, 28, bounds.size.w, bounds.size.h - 28);
    s_reader_scroll = scroll_layer_create(scroll_frame);

    /* Build text with attribution */
    if (prayer->resource_id != 0) {
        /* Load from flash resource */
        ResHandle handle = resource_get_handle(prayer->resource_id);
        size_t text_size = resource_load(handle, (uint8_t *)s_reader_buf,
                                         READER_BUF_SIZE - 32);
        s_reader_buf[text_size] = '\0';
        snprintf(s_reader_buf + text_size, READER_BUF_SIZE - text_size,
                 "\n\n\u2014 %s", prayer->author);
    } else {
        snprintf(s_reader_buf, READER_BUF_SIZE, "%s\n\n\u2014 %s",
                 prayer->text, prayer->author);
    }

    /* Text layer with large bounds for measurement */
    GRect text_frame = GRect(5, 5, bounds.size.w - 10, 2000);
    s_reader_text = text_layer_create(text_frame);
    text_layer_set_background_color(s_reader_text, BG_COLOR);
    text_layer_set_text_color(s_reader_text, FG_COLOR);
    text_layer_set_font(s_reader_text, fonts_get_system_font(FONT_KEY_GOTHIC_18));
    text_layer_set_overflow_mode(s_reader_text, GTextOverflowModeWordWrap);
    text_layer_set_text(s_reader_text, s_reader_buf);

    /* Size to content */
    GSize content = text_layer_get_content_size(s_reader_text);
    text_layer_set_size(s_reader_text, content);
    scroll_layer_set_content_size(s_reader_scroll, GSize(bounds.size.w, content.h + 20));

    /* Add text to scroll, scroll to window */
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

static void push_reader(int cat, int item) {
    s_reader_cat = cat;
    s_reader_item = item;
    s_reader_window = window_create();
    window_set_background_color(s_reader_window, BG_COLOR);
    window_set_window_handlers(s_reader_window, (WindowHandlers) {
        .load = reader_window_load,
        .unload = reader_window_unload,
    });
    window_stack_push(s_reader_window, true);
}

/* ═══════════════════════════════════════════════════════════════
 * BADÍ CALENDAR WINDOW
 * ═══════════════════════════════════════════════════════════════ */

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
    if (doy >= nrz) {
        ds = doy - nrz;
    } else {
        by = gyear - 1844;
        ds = doy + (is_leap(gyear - 1) ? 366 : 365) - nrz;
    }
    int ac = is_leap(gyear) ? 5 : 4;
    int month, day;
    bool is_ayyam = false;
    if (ds < 342) {
        month = ds / 19 + 1;
        day = ds % 19 + 1;
    } else if (ds < 342 + ac) {
        is_ayyam = true;
        day = ds - 342 + 1;
        month = 0;
    } else {
        month = 19;
        day = ds - 342 - ac + 1;
    }

    if (is_ayyam) {
        snprintf(s_badi_date_buf, sizeof(s_badi_date_buf), "Ayy\u00E1m-i-H\u00E1 %d", day);
    } else {
        snprintf(s_badi_date_buf, sizeof(s_badi_date_buf), "%d %s (%s)", day, BM[month], BE[month]);
    }
    snprintf(s_badi_year_buf, sizeof(s_badi_year_buf), "%d B.E.", by);

    /* Date label */
    s_badi_date_label = text_layer_create(GRect(0, 60, bounds.size.w, 40));
    text_layer_set_background_color(s_badi_date_label, BG_COLOR);
    text_layer_set_text_color(s_badi_date_label, FG_COLOR);
    text_layer_set_font(s_badi_date_label, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
    text_layer_set_text_alignment(s_badi_date_label, GTextAlignmentCenter);
    text_layer_set_text(s_badi_date_label, s_badi_date_buf);
    layer_add_child(root, text_layer_get_layer(s_badi_date_label));

    /* Year label */
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

/* ═══════════════════════════════════════════════════════════════
 * ABOUT WINDOW
 * ═══════════════════════════════════════════════════════════════ */

static Window *s_about_window;
static ScrollLayer *s_about_scroll;
static TextLayer *s_about_text;

static const char *ABOUT_TEXT =
    "A Pebble App for Bahá'í Life\n"
    "Version 0.5 - First Release\n"
    "\n"
    "Dedicated to my dear friend\n"
    "Meg Moyer.\n"
    "\n"
    "Free & open source.\n"
    "https://forge.mitch.science/\n"
    "mitcheffendi/pebble-bahai-prayer-app\n"
    "\n"
    "Made by:\n"
    "Mitch Donaberger\n"
    "https://donaberger.xyz\n"
    "mitch@donaberger.xyz\n";

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

/* ═══════════════════════════════════════════════════════════════
 * 95x COUNTER WINDOW
 * ═══════════════════════════════════════════════════════════════ */

static Window *s_counter_window;
static TextLayer *s_counter_label;
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
            /* Victory vibration */
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

    /* Header */
    TextLayer *header = text_layer_create(GRect(0, 0, bounds.size.w, 30));
    text_layer_set_background_color(header, HEADER_BG);
    text_layer_set_text_color(header, FG_COLOR);
    text_layer_set_font(header, fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD));
    text_layer_set_text_alignment(header, GTextAlignmentCenter);
    text_layer_set_text(header, "All\u00E1h-u-Abh\u00E1");
    layer_add_child(root, text_layer_get_layer(header));
    /* Store so we can destroy it */
    window_set_user_data(window, header);

    /* Counter number */
    s_counter_label = text_layer_create(GRect(0, 70, bounds.size.w, 40));
    text_layer_set_background_color(s_counter_label, BG_COLOR);
    text_layer_set_text_color(s_counter_label, FG_COLOR);
    text_layer_set_font(s_counter_label, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
    text_layer_set_text_alignment(s_counter_label, GTextAlignmentCenter);
    counter_update();
    layer_add_child(root, text_layer_get_layer(s_counter_label));

    /* Hint text */
    TextLayer *hint = text_layer_create(GRect(0, 130, bounds.size.w, 50));
    text_layer_set_background_color(hint, BG_COLOR);
    text_layer_set_text_color(hint, GColorLightGray);
    text_layer_set_font(hint, fonts_get_system_font(FONT_KEY_GOTHIC_14));
    text_layer_set_text_alignment(hint, GTextAlignmentCenter);
    text_layer_set_text(hint, "Up/Select: count\nDown: undo\nBack: exit");
    layer_add_child(root, text_layer_get_layer(hint));
}

static void counter_window_unload(Window *window) {
    TextLayer *header = (TextLayer *)window_get_user_data(window);
    text_layer_destroy(header);
    text_layer_destroy(s_counter_label);
    /* Hint text layer leaks — fix in production */
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

/* ═══════════════════════════════════════════════════════════════
 * INIT / DEINIT / MAIN
 * ═══════════════════════════════════════════════════════════════ */

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
