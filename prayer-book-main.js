// ═══════════════════════════════════════════════════════════════
// BAHÁ'Í PRAYER BOOK — Pebble Alloy App v2
// main.js — paste into CloudPebble src/embeddedjs/main.js
// ═══════════════════════════════════════════════════════════════

import {} from "piu/MC";
import Button from "pebble/button";
import Vibes from "pebble/vibes";
import Compass from "embedded:sensor/Compass";

// ═══════════════════════════════════════════════════════════════
// SECTION 1: DATA
// ═══════════════════════════════════════════════════════════════

const PRAYER_DATA = [
    {
        title: "Prayers",
        icon: "P",
        items: [
            {
                title: "Morning Prayer",
                attribution: "— Bahá'u'lláh",
                text: "I have wakened in Thy shelter, O my God, and within the refuge of Thy care. I have entered Thy dwelling-place, and have found Thy beloved ones gathered in Thy courtyard.\n\nI beseech Thee, O my Lord, by Thy hidden, Thy ancient name, to make me one of them that are steadfast in Thy Cause, and to reveal unto me whatsoever Thou hast decreed for them.\n\nThou art, verily, the Almighty, the All-Knowing."
            },
            {
                title: "Evening Prayer",
                attribution: "— Bahá'u'lláh",
                text: "O Thou Who art the Lord of all names and the Maker of the heavens! I entreat Thee, by Them Who are the Dawning-Places of Thine absolute Essence and the Repositories of Thy might, to ordain for me the good of this world and of the world to come.\n\nReveal then unto me, O my God, that which will enrich me from every other thing besides Thee. Thou art, verily, the Lord of all worlds."
            },
            {
                title: "Short Obligatory",
                attribution: "— The Báb",
                text: "I bear witness, O my God, that Thou hast created me to know Thee and to worship Thee. I testify, at this moment, to my powerlessness and to Thy might, to my poverty and to Thy wealth.\n\nThere is none other God but Thee, the Help in Peril, the Self-Subsisting."
            },
            {
                title: "For Assistance",
                attribution: "— Bahá'u'lláh",
                text: "Thou seest me, O my God, holding to Thy Name, the Most Holy, the Most Luminous, the Most Mighty, the Most Great, the Most Exalted, the Most Glorious, and clinging to the hem of the robe of Thine infinite favors.\n\nSend down upon me, by Thy grace, that which will enable me to dispense with all except Thee, and destine for me from Thyself what will exalt me above all that is in Thy creation."
            },
            {
                title: "For Unity",
                attribution: "— 'Abdu'l-Bahá",
                text: "O my God! O my God! Unite the hearts of Thy servants, and reveal to them Thy great purpose. May they follow Thy commandments and abide in Thy law.\n\nHelp them, O God, in their endeavors, and grant them strength to serve Thee. Verily, Thou art the Helper, the Exalted, the Powerful."
            }
        ]
    },
    {
        title: "Writings",
        icon: "W",
        items: [
            {
                title: "Hidden Word Ar 1",
                attribution: "— Bahá'u'lláh, Arabic Hidden Words",
                text: "O SON OF SPIRIT!\n\nMy first counsel is this: Possess a pure, kindly and radiant heart, that thine may be a sovereignty ancient, imperishable and everlasting."
            },
            {
                title: "Hidden Word Ar 2",
                attribution: "— Bahá'u'lláh, Arabic Hidden Words",
                text: "O SON OF SPIRIT!\n\nThe best beloved of all things in My sight is Justice; turn not away therefrom if thou desirest Me, and neglect it not that I may confide in thee. By its aid thou shalt see with thine own eyes and not through the eyes of others, and shalt know of thine own knowledge and not through the knowledge of thy neighbor.\n\nPonder this in thy heart; how it behoveth thee to be. Verily justice is My gift to thee and the sign of My loving-kindness. Set it then before thine eyes."
            },
            {
                title: "Hidden Word Pe 1",
                attribution: "— Bahá'u'lláh, Persian Hidden Words",
                text: "O SON OF DESIRE!\n\nThe learned and the wise have for long years striven and endured hardships and trials, have ascended into the heaven of knowledge, and attained to the heights of science and art. Know therefore of a certainty that true life is not the life of the flesh but the life of the spirit."
            },
            {
                title: "On Peace",
                attribution: "— Bahá'u'lláh, Tablets",
                text: "O ye children of men! The fundamental purpose animating the Faith of God and His Religion is to safeguard the interests and promote the unity of the human race, and to foster the spirit of love and fellowship amongst men.\n\nWhatsoever is raised on this foundation, the changes and chances of the world can never impair its strength, nor will the revolution of countless centuries undermine its structure."
            }
        ]
    },
    {
        title: "Meditations",
        icon: "M",
        items: [
            {
                title: "On Detachment",
                attribution: "— 'Abdu'l-Bahá, Paris Talks",
                text: "The source of all good is trust in God, submission unto His command, and contentment with His holy will and pleasure.\n\nThe essence of wisdom is the fear of God, the dread of His scourge and punishment, and the apprehension of His justice and decree."
            },
            {
                title: "On Service",
                attribution: "— 'Abdu'l-Bahá",
                text: "It is incumbent upon each and every one, great or small, high or low, rich or poor, to serve the Cause of God.\n\nWhoso ariseth to serve the Cause of God, him will the Concourse on High assist. So great are the confirmations that will descend upon him that he will be enabled to perform whatsoever is the desire of his heart."
            },
            {
                title: "On Joy",
                attribution: "— 'Abdu'l-Bahá, Paris Talks",
                text: "Joy gives us wings! In times of joy our strength is more vital, our intellect keener, and our understanding less clouded.\n\nWe seem better able to cope with the world and to find our sphere of usefulness."
            }
        ]
    }
];

// ═══════════════════════════════════════════════════════════════
// SECTION 2: BADÍ CALENDAR
// ═══════════════════════════════════════════════════════════════

const BADI_MONTHS = [
    "", "Bahá", "Jalál", "Jamál", "'Aẓamat", "Núr",
    "Raḥmat", "Kalimát", "Kamál", "Asmá", "'Izzat",
    "Mashíyyat", "'Ilm", "Qudrat", "Qawl", "Masá'il",
    "Sharaf", "Sulṭán", "Mulk", "'Alá"
];

const BADI_DAYS = BADI_MONTHS;

const BADI_WEEKDAYS = [
    "Jamál (Sun)", "Kamál (Mon)", "Fiḍál (Tue)",
    "'Idál (Wed)", "Istijlál (Thu)", "Istiqlál (Fri)", "Jalál (Sat)"
];

function isLeapYear(y) {
    return (y % 4 === 0 && y % 100 !== 0) || (y % 400 === 0);
}

function getBadiDate() {
    const now = new Date();
    const gYear = now.getFullYear();
    const monthDays = [0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334];
    let dayOfYear = monthDays[now.getMonth()] + now.getDate();
    if (now.getMonth() > 1 && isLeapYear(gYear)) dayOfYear++;

    const nawruzDOY = isLeapYear(gYear) ? 80 : 79;

    let badiYear = gYear - 1843;
    let daysSince;

    if (dayOfYear >= nawruzDOY) {
        daysSince = dayOfYear - nawruzDOY;
    } else {
        badiYear = gYear - 1844;
        const prevDays = isLeapYear(gYear - 1) ? 366 : 365;
        daysSince = dayOfYear + prevDays - nawruzDOY;
    }

    const ayyamCount = isLeapYear(gYear) ? 5 : 4;

    let month, day, inAyyam = false;

    if (daysSince < 342) {
        month = Math.floor(daysSince / 19) + 1;
        day = (daysSince % 19) + 1;
    } else if (daysSince < 342 + ayyamCount) {
        inAyyam = true;
        day = daysSince - 342 + 1;
    } else {
        month = 19;
        day = daysSince - 342 - ayyamCount + 1;
    }

    const weekday = BADI_WEEKDAYS[now.getDay()];

    let dateStr;
    if (inAyyam) {
        dateStr = "Ayyám-i-Há " + day + "\n" + badiYear + " B.E.";
    } else {
        dateStr = BADI_DAYS[day] + " " + BADI_MONTHS[month] + "\n" + badiYear + " B.E.";
    }

    return {
        year: badiYear,
        month: month,
        day: day,
        inAyyam: inAyyam,
        weekday: weekday,
        string: dateStr
    };
}

// ═══════════════════════════════════════════════════════════════
// SECTION 3: QIBLIH BEARING
// ═══════════════════════════════════════════════════════════════

const QIBLIH_LAT = 32.9434;
const QIBLIH_LON = 35.0924;

// Default: Philadelphia, PA
const DEFAULT_LAT = 40.03;
const DEFAULT_LON = -75.22;

function qiblaBearing(lat1, lon1) {
    const phi1 = lat1 * Math.PI / 180;
    const phi2 = QIBLIH_LAT * Math.PI / 180;
    const dLambda = (QIBLIH_LON - lon1) * Math.PI / 180;

    const y = Math.sin(dLambda) * Math.cos(phi2);
    const x = Math.cos(phi1) * Math.sin(phi2) -
              Math.sin(phi1) * Math.cos(phi2) * Math.cos(dLambda);

    let theta = Math.atan2(y, x) * 180 / Math.PI;
    return (theta + 360) % 360;
}

// ═══════════════════════════════════════════════════════════════
// SECTION 4: STYLES & SKINS
// ═══════════════════════════════════════════════════════════════

const bgSkin = new Skin({ fill: "#1a1a2e" });
const headerSkin = new Skin({ fill: "black" });
const itemSkin = new Skin({ fill: ["#1a1a2e", "#0f3460"] });

const titleStyle = new Style({
    font: "bold 18px Gothic", color: "#e8e8e8",
    horizontal: "center", top: 4, bottom: 4
});
const iconStyle = new Style({
    font: "bold 14px Gothic", color: "#e94560",
    horizontal: "center", left: 4
});
const mainItemStyle = new Style({
    font: "bold 18px Gothic", color: ["#e8e8e8", "white"],
    horizontal: "left", left: 10
});
const subItemStyle = new Style({
    font: "14px Gothic", color: ["#b0b0b0", "white"],
    horizontal: "left", left: 10
});
const readerTitleStyle = new Style({
    font: "bold 14px Gothic", color: "#e94560",
    horizontal: "center", top: 4, bottom: 4
});
const textStyle = new Style({
    font: "14px Gothic", color: "#e8e8e8",
    horizontal: "left", leading: 4,
    left: 10, right: 10, top: 5, bottom: 5
});
const dropcapStyle = new Style({
    font: "bold 24px Gothic", color: "#e94560",
    horizontal: "left", leading: 2,
    left: 10, right: 5, top: 5
});
const attributionStyle = new Style({
    font: "14px Gothic", color: "#888888",
    horizontal: "right", top: 10, bottom: 15,
    left: 10, right: 10
});
const hintStyle = new Style({
    font: "14px Gothic", color: "#444444",
    horizontal: "center", top: 2, bottom: 2
});
const counterStyle = new Style({
    font: "bold 28px Gothic", color: "#e94560",
    horizontal: "center", top: 0, bottom: 0
});
const counterLabelStyle = new Style({
    font: "bold 14px Gothic", color: "#e8e8e8",
    horizontal: "center", top: 4, bottom: 4
});
const badiStyle = new Style({
    font: "bold 18px Gothic", color: "#e94560",
    horizontal: "center", top: 8, bottom: 4
});
const badiSubStyle = new Style({
    font: "14px Gothic", color: "#e8e8e8",
    horizontal: "center", top: 2, bottom: 2
});
const compassStyle = new Style({
    font: "bold 18px Gothic", color: "#e94560",
    horizontal: "center", top: 0, bottom: 0
});
const compassHintStyle = new Style({
    font: "14px Gothic", color: "#e8e8e8",
    horizontal: "center", top: 3, bottom: 3
});
const alignedStyle = new Style({
    font: "bold 18px Gothic", color: "#00ff88",
    horizontal: "center", top: 3, bottom: 3
});

// ═══════════════════════════════════════════════════════════════
// SECTION 5: NAVIGATION STATE
// ═══════════════════════════════════════════════════════════════

let nav = {
    screen: "main",
    menuIndex: 0,
    subIndex: 0,
    catIndex: 0,
    scrollOffset: 0,
    counter: 0,
    counterVibeInterval: null
};

const MENU_ITEMS = [
    { title: "Prayers", icon: "P", type: "data", catIndex: 0 },
    { title: "Writings", icon: "W", type: "data", catIndex: 1 },
    { title: "Meditations", icon: "M", type: "data", catIndex: 2 },
    { title: "Alláh-u-Abhá", icon: "9", type: "counter" },
    { title: "Badí Date", icon: "B", type: "badi" },
    { title: "Qiblih", icon: "Q", type: "compass" }
];

// ═══════════════════════════════════════════════════════════════
// SECTION 6: BEHAVIORS
// ═══════════════════════════════════════════════════════════════

class AppBehavior extends Behavior {
    onDisplaying(app) { }

    onButtonPush(app, down, type) {
        if (!down) return;

        if (nav.screen === "main") {
            const count = MENU_ITEMS.length;
            if (type === "up") {
                nav.menuIndex = (nav.menuIndex - 1 + count) % count;
                highlightMenu(app, nav.menuIndex);
            } else if (type === "down") {
                nav.menuIndex = (nav.menuIndex + 1) % count;
                highlightMenu(app, nav.menuIndex);
            } else if (type === "select") {
                this.openMenuItem(app, nav.menuIndex);
            }
        } else if (nav.screen === "sub") {
            const count = PRAYER_DATA[nav.catIndex].items.length;
            if (type === "up") {
                nav.subIndex = (nav.subIndex - 1 + count) % count;
                highlightMenu(app, nav.subIndex);
            } else if (type === "down") {
                nav.subIndex = (nav.subIndex + 1) % count;
                highlightMenu(app, nav.subIndex);
            } else if (type === "select") {
                this.showReader(app, nav.catIndex, nav.subIndex);
            } else if (type === "back") {
                this.showMain(app);
            }
        } else if (nav.screen === "reader") {
            if (type === "up") {
                nav.scrollOffset = Math.max(0, nav.scrollOffset - 25);
                app.first.delegate("onScrollTo", nav.scrollOffset);
            } else if (type === "down") {
                nav.scrollOffset += 25;
                app.first.delegate("onScrollTo", nav.scrollOffset);
            } else if (type === "back") {
                this.showSub(app, nav.catIndex);
            }
        } else if (nav.screen === "counter") {
            this.onCounterButton(app, type);
        } else if (nav.screen === "badi") {
            if (type === "back" || type === "select") {
                this.showMain(app);
            }
        } else if (nav.screen === "compass") {
            if (type === "back" || type === "select") {
                app.first.delegate("onCompassClose");
                this.showMain(app);
            }
        }
    }

    openMenuItem(app, index) {
        const item = MENU_ITEMS[index];
        if (item.type === "data") {
            this.showSub(app, item.catIndex);
        } else if (item.type === "counter") {
            this.showCounter(app);
        } else if (item.type === "badi") {
            this.showBadi(app);
        } else if (item.type === "compass") {
            this.showCompass(app);
        }
    }

    showMain(app) {
        nav.screen = "main";
        nav.scrollOffset = 0;
        app.remove(app.first);
        app.add(new MainColumn(MENU_ITEMS, {}));
    }

    showSub(app, catIndex) {
        nav.screen = "sub";
        nav.catIndex = catIndex;
        nav.subIndex = 0;
        nav.scrollOffset = 0;
        app.remove(app.first);
        app.add(new SubColumn(PRAYER_DATA[catIndex], {}));
    }

    showReader(app, catIndex, itemIndex) {
        nav.screen = "reader";
        nav.scrollOffset = 0;
        const item = PRAYER_DATA[catIndex].items[itemIndex];
        app.remove(app.first);
        app.add(new ReaderColumn(item, {}));
    }

    showCounter(app) {
        nav.screen = "counter";
        nav.counter = 0;
        if (nav.counterVibeInterval) {
            clearInterval(nav.counterVibeInterval);
            nav.counterVibeInterval = null;
        }
        app.remove(app.first);
        app.add(new CounterColumn({ count: 0 }, {}));
    }

    showBadi(app) {
        nav.screen = "badi";
        app.remove(app.first);
        app.add(new BadiColumn({}, {}));
    }

    showCompass(app) {
        nav.screen = "compass";
        app.remove(app.first);
        app.add(new CompassColumn({}, {}));
    }

    onCounterButton(app, type) {
        if (type === "up" || type === "select") {
            if (nav.counter < 95) {
                nav.counter++;
                Vibes.shortPulse();
                app.first.delegate("onCountUpdate", nav.counter);
                if (nav.counter === 95) {
                    app.first.delegate("onCountComplete");
                    nav.counterVibeInterval = setInterval(() => {
                        Vibes.pattern([300, 100, 300, 100, 300]);
                    }, 800);
                }
            }
        } else if (type === "down") {
            if (nav.counter > 0) {
                nav.counter--;
                app.first.delegate("onCountUpdate", nav.counter);
            }
        } else if (type === "back") {
            if (nav.counterVibeInterval) {
                clearInterval(nav.counterVibeInterval);
                nav.counterVibeInterval = null;
                Vibes.cancel();
            }
            this.showMain(app);
        }
    }

    onCounterDismiss(app) {
        if (nav.counterVibeInterval) {
            clearInterval(nav.counterVibeInterval);
            nav.counterVibeInterval = null;
            Vibes.cancel();
        }
        nav.counter = 0;
        this.showMain(app);
    }

    onMenuTap(app, index) {
        nav.menuIndex = index;
        this.openMenuItem(app, index);
    }

    onSubTap(app, index) {
        nav.subIndex = index;
        this.showReader(app, nav.catIndex, index);
    }
}

function highlightMenu(app, index) {
    const col = app.first;
    if (!col) return;
    let child = col.first;
    let i = -1;
    while (child) {
        if (i >= 0) {
            child.state = (i === index) ? 1 : 0;
        }
        child = child.next;
        i++;
    }
}

class ReaderBehavior extends Behavior {
    onDisplaying(column) {
        this.scroller = column.last;
    }
    onScrollTo(column, offset) {
        if (this.scroller) {
            this.scroller.scrollTo(0, offset);
        }
    }
}

class CounterBehavior extends Behavior {
    onDisplaying(column) {
        this.countNumLabel = column.first.next.next;
        this.countTrackLabel = column.first.next.next.next;
        this.complete = false;
    }
    onCountUpdate(column, count) {
        if (this.countNumLabel) {
            this.countNumLabel.string = String(count);
        }
        if (this.countTrackLabel) {
            this.countTrackLabel.string = String(count) + " / 95";
        }
    }
    onCountComplete(column) {
        this.complete = true;
        if (this.countNumLabel) {
            this.countNumLabel.string = "95";
        }
        if (this.countTrackLabel) {
            this.countTrackLabel.string = "Complete! Tap to dismiss";
        }
    }
    onTouchEnded(column) {
        if (this.complete) {
            column.bubble("onCounterDismiss");
        }
    }
}

class CompassBehavior extends Behavior {
    onDisplaying(column) {
        this.directionLabel = column.first.next;
        this.headingLabel = column.first.next.next;
        this.bearingLabel = column.first.next.next.next;
        this.bearing = qiblaBearing(DEFAULT_LAT, DEFAULT_LON);

        this.compass = new Compass({
            onSample: () => {
                const sample = this.compass.sample();
                const heading = sample.heading;
                const diff = ((this.bearing - heading + 540) % 360) - 180;
                const absDiff = Math.abs(diff);

                this.headingLabel.string = "Heading: " + Math.round(heading) + " deg";
                this.bearingLabel.string = "Qiblih: " + Math.round(this.bearing) + " deg";

                if (absDiff < 10) {
                    this.directionLabel.string = "** ALIGNED **";
                    this.directionLabel.style = alignedStyle;
                } else if (diff > 0) {
                    this.directionLabel.string = ">> TURN RIGHT >>";
                    this.directionLabel.style = compassStyle;
                } else {
                    this.directionLabel.string = "<< TURN LEFT <<";
                    this.directionLabel.style = compassStyle;
                }
            }
        });
    }
    onCompassClose(column) {
        if (this.compass) {
            this.compass.close();
            this.compass = null;
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// SECTION 7: TEMPLATES
// ═══════════════════════════════════════════════════════════════

const MainItem = Row.template($ => ({
    top: 0, height: 34, left: 0, right: 0,
    skin: itemSkin,
    active: true,
    contents: [
        Label(null, {
            width: 28, left: 4,
            style: iconStyle,
            string: $.icon
        }),
        Label(null, {
            left: 0, right: 0,
            style: mainItemStyle,
            string: $.title
        })
    ],
    Behavior: class extends Behavior {
        onTouchEnded(row) {
            row.bubble("onMenuTap", $.index);
        }
    }
}));

const SubItem = Label.template($ => ({
    top: 0, height: 32, left: 0, right: 0,
    skin: itemSkin,
    style: subItemStyle,
    active: true,
    string: $.title,
    Behavior: class extends Behavior {
        onTouchEnded(label) {
            label.bubble("onSubTap", $.index);
        }
    }
}));

const MainColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    contents: [
        Label(null, {
            top: 0, height: 30, left: 0, right: 0,
            skin: headerSkin,
            style: titleStyle,
            string: "Prayer Book"
        }),
        ...$.map((cat, i) =>
            new MainItem({ title: cat.title, icon: cat.icon, index: i })
        )
    ]
}));

const SubColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    contents: [
        Label(null, {
            top: 0, height: 30, left: 0, right: 0,
            skin: headerSkin,
            style: titleStyle,
            string: $.title
        }),
        ...$.items.map((item, i) =>
            new SubItem({ title: item.title, index: i })
        )
    ]
}));

const ReaderColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    contents: [
        Label(null, {
            top: 0, height: 26, left: 0, right: 0,
            skin: headerSkin,
            style: readerTitleStyle,
            string: $.title
        }),
        new Scroller(null, {
            left: 0, right: 0, top: 0, bottom: 0,
            active: true,
            backgroundTouch: true,
            contents: [
                new Column(null, {
                    top: 0, left: 0, right: 0,
                    contents: [
                        Label(null, {
                            top: 2, height: 16, left: 0, right: 0,
                            style: hintStyle,
                            string: "< hold back"
                        }),
                        new Text(null, {
                            left: 0, right: 0, top: 3,
                            style: textStyle,
                            blocks: [
                                {
                                    spans: [
                                        { style: dropcapStyle, spans: $.text.split(" ")[0] + " " },
                                        { style: textStyle, spans: $.text.split(" ").slice(1).join(" ") }
                                    ]
                                }
                            ]
                        }),
                        new Text(null, {
                            left: 0, right: 0, top: 5,
                            style: attributionStyle,
                            string: $.attribution || ""
                        })
                    ]
                })
            ]
        })
    ],
    Behavior: ReaderBehavior
}));

const CounterColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    active: true,
    contents: [
        Label(null, {
            top: 0, height: 30, left: 0, right: 0,
            skin: headerSkin,
            style: titleStyle,
            string: "Alláh-u-Abhá"
        }),
        Label(null, {
            top: 10, height: 50, left: 0, right: 0,
            style: counterStyle,
            string: "0"
        }),
        Label(null, {
            top: 5, height: 24, left: 0, right: 0,
            style: counterLabelStyle,
            string: "0 / 95"
        }),
        Label(null, {
            top: 10, height: 30, left: 0, right: 0,
            style: hintStyle,
            string: "Up/Select: count\nDown: undo\nBack: exit"
        })
    ],
    Behavior: CounterBehavior
}));

const BadiColumn = Column.template($ => {
    const badi = getBadiDate();
    const lines = badi.string.split("\n");
    return {
        top: 0, bottom: 0, left: 0, right: 0,
        skin: bgSkin,
        contents: [
            Label(null, {
                top: 0, height: 30, left: 0, right: 0,
                skin: headerSkin,
                style: titleStyle,
                string: "Badí Calendar"
            }),
            Label(null, {
                top: 15, height: 50, left: 10, right: 10,
                style: badiStyle,
                string: lines[0]
            }),
            Label(null, {
                top: 5, height: 28, left: 10, right: 10,
                style: badiSubStyle,
                string: lines[1]
            }),
            Label(null, {
                top: 5, height: 24, left: 10, right: 10,
                style: hintStyle,
                string: badi.weekday
            }),
            Label(null, {
                top: 10, height: 24, left: 0, right: 0,
                style: hintStyle,
                string: "Select/Back: menu"
            })
        ]
    };
});

const CompassColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    contents: [
        Label(null, {
            top: 0, height: 30, left: 0, right: 0,
            skin: headerSkin,
            style: titleStyle,
            string: "Qiblih Compass"
        }),
        Label(null, {
            top: 15, height: 28, left: 0, right: 0,
            style: compassStyle,
            string: ">> INITIALIZING <<"
        }),
        Label(null, {
            top: 5, height: 24, left: 0, right: 0,
            style: compassHintStyle,
            string: "Heading: ---"
        }),
        Label(null, {
            top: 0, height: 24, left: 0, right: 0,
            style: compassHintStyle,
            string: "Qiblih: ---"
        }),
        Label(null, {
            top: 10, height: 24, left: 0, right: 0,
            style: hintStyle,
            string: "Back: return to menu"
        })
    ],
    Behavior: CompassBehavior
}));

// ═══════════════════════════════════════════════════════════════
// SECTION 8: APP BOOTSTRAP
// ═══════════════════════════════════════════════════════════════

const PrayerBookApp = Application.template($ => ({
    skin: bgSkin,
    style: titleStyle,
    contents: [
        new MainColumn(MENU_ITEMS, {})
    ],
    Behavior: AppBehavior
}));

const application = new PrayerBookApp(null, {
    displayListLength: 8192,
    touchCount: 0,
    pixels: screen.width * 4
});

new Button({
    types: ["select", "up", "down", "back"],
    onPush(down, type) {
        application.delegate("onButtonPush", down, type);
    }
});

export default application;
