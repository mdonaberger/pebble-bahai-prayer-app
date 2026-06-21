// ═══════════════════════════════════════════════════════════════
// BAHÁ'Í PRAYER BOOK — Pebble Alloy App v3 (lean)
// main.js — paste into CloudPebble src/embeddedjs/main.js
// ═══════════════════════════════════════════════════════════════

import {} from "piu/MC";
import Button from "pebble/button";

// ═══════════════════════════════════════════════════════════════
// SECTION 1: DATA (inlined — no module import for memory)
// ═══════════════════════════════════════════════════════════════

const PRAYER_DATA = [
    { title: "Obligatory", icon: "O", items: [
        { title: "Short Obligatory", by: "The Báb",
          text: "I bear witness, O my God, that Thou hast created me to know Thee and to worship Thee. I testify, at this moment, to my powerlessness and to Thy might, to my poverty and to Thy wealth.\n\nThere is none other God but Thee, the Help in Peril, the Self-Subsisting." }
    ]},
    { title: "General", icon: "G", items: [
        { title: "For Aid", by: "Bahá'u'lláh",
          text: "O Thou Whose face is the object of my adoration, Whose beauty is my sanctuary, Whose habitation is my goal, Whose praise is my hope, Whose providence is my companion.\n\nSupply me with the good of this world and of the next. Thou, truly, art the King of all men. There is no God but Thee, the Ever-Forgiving, the Most Generous." },
        { title: "For Assistance", by: "Bahá'u'lláh",
          text: "Thou seest me, O my God, holding to Thy Name, the Most Holy, the Most Luminous, the Most Mighty.\n\nSend down upon me that which will enable me to dispense with all except Thee." },
        { title: "For Unity", by: "'Abdu'l-Bahá",
          text: "O my God! Unite the hearts of Thy servants, and reveal to them Thy great purpose.\n\nGrant them strength to serve Thee. Thou art the Helper, the Exalted, the Powerful." },
        { title: "For Tests", by: "'Abdu'l-Bahá",
          text: "O Thou Whose tests are a healing medicine to such as are nigh unto Thee, Whose sword is the ardent desire of them that yearn after Thee!\n\nI beseech Thee, by Thy divine sweetness and by the splendors of Thy glory, to immerse us in the ocean of Thy mercy." }
    ]},
    { title: "Occasional", icon: "C", items: [
        { title: "Naw-Rúz", by: "Bahá'u'lláh",
          text: "This is, O my God, the first of the days on which Thou hast bidden Thy loved ones to observe the Fast.\n\nI beg of Thee, by Thy self and by the glory of Thy Cause, to accept what Thou hast prescribed for them in Thy Book.\n\nGrant that they may be enabled to attain unto that which shall profit them."
        }
    ]}
];

// ═══════════════════════════════════════════════════════════════
// SECTION 2: BADÍ CALENDAR
// ═══════════════════════════════════════════════════════════════

const BM = ["","Bahá","Jalál","Jamál","'Aẓamat","Núr","Raḥmat","Kalimát","Kamál","Asmá","'Izzat","Mashíyyat","'Ilm","Qudrat","Qawl","Masá'il","Sharaf","Sulṭán","Mulk","'Alá"];
const BE = ["","Splendour","Glory","Beauty","Grandeur","Light","Mercy","Words","Perfection","Names","Might","Will","Knowledge","Power","Speech","Questions","Honour","Sovereignty","Dominion","Loftiness"];
const BW = ["Jamál (Sun)","Kamál (Mon)","Fiḍál (Tue)","'Idál (Wed)","Istijlál (Thu)","Istiqlál (Fri)","Jalál (Sat)"];

function getBadi() {
    const d = new Date();
    const y = d.getFullYear();
    const ly = (y%4===0 && y%100!==0) || (y%400===0);
    const md = [0,31,59,90,120,151,181,212,243,273,304,334];
    let doy = md[d.getMonth()] + d.getDate();
    if (d.getMonth() > 1 && ly) doy++;
    const nrz = ly ? 80 : 79;
    let by = y - 1843;
    let ds = doy >= nrz ? doy - nrz : doy + (ly ? 366 : 365) - nrz - 366;
    if (doy < nrz) by = y - 1844;
    const ac = ly ? 5 : 4;
    let m, day, ia = false;
    if (ds < 0) { ds += (ly ? 366 : 365); }
    if (ds < 342) { m = Math.floor(ds/19)+1; day = (ds%19)+1; }
    else if (ds < 342+ac) { ia = true; day = ds-342+1; }
    else { m = 19; day = ds-342-ac+1; }
    return {
        day: day, month: m, year: by, ia: ia,
        str: ia ? ("Ayyám-i-Há " + day) : (day + " " + BM[m] + " (" + BE[m] + ")")
    };
}

// ═══════════════════════════════════════════════════════════════
// SECTION 3: STYLES (consolidated — fewer objects = less heap)
// ═══════════════════════════════════════════════════════════════

const bgSkin = new Skin({ fill: "#1a1a2e" });
const hdrSkin = new Skin({ fill: "black" });
const itemSkin = new Skin({ fill: ["#1a1a2e", "#0f3460"] });

const hdrStyle = new Style({ font: "bold 18px Gothic", color: "#e8e8e8", horizontal: "center", top: 4, bottom: 4 });
const redStyle = new Style({ font: "bold 18px Gothic", color: "#e94560", horizontal: "center", top: 2, bottom: 2 });
const itemStyle = new Style({ font: "bold 18px Gothic", color: ["#e8e8e8", "white"], horizontal: "left", left: 10 });
const subStyle = new Style({ font: "14px Gothic", color: ["#b0b0b0", "white"], horizontal: "left", left: 10 });
const bodyStyle = new Style({ font: "14px Gothic", color: "#e8e8e8", horizontal: "left", leading: 4, left: 10, right: 10, top: 5, bottom: 5 });
const greyStyle = new Style({ font: "14px Gothic", color: "#555555", horizontal: "center", top: 2, bottom: 2 });

// ═══════════════════════════════════════════════════════════════
// SECTION 4: NAV STATE
// ═══════════════════════════════════════════════════════════════

let nav = { screen: "main", mi: 0, si: 0, ci: 0, so: 0, count: 0 };

const MENU = [
    { t: "Prayers", i: "P", type: "data", ci: 0 },
    { t: "Writings", i: "W", type: "data", ci: 1 },
    { t: "Meditations", i: "M", type: "data", ci: 2 },
    { t: "Alláh-u-Abhá", i: "9", type: "counter" },
    { t: "Badí Date", i: "B", type: "badi" }
];

// ═══════════════════════════════════════════════════════════════
// SECTION 5: BEHAVIORS
// ═══════════════════════════════════════════════════════════════

function highlight(app, idx) {
    const c = app.first;
    if (!c) return;
    let ch = c.first, i = -1;
    while (ch) { if (i >= 0) ch.state = (i === idx) ? 1 : 0; ch = ch.next; i++; }
}

class AppBehavior extends Behavior {
    onButtonPush(app, down, type) {
        if (!down) return;  // act on PRESS, not release
        if (nav.screen === "main") {
            const n = MENU.length;
            if (type === "up") { nav.mi = (nav.mi-1+n)%n; highlight(app, nav.mi); }
            else if (type === "down") { nav.mi = (nav.mi+1)%n; highlight(app, nav.mi); }
            else if (type === "select") {
                const item = MENU[nav.mi];
                if (item.type === "data") { nav.ci = item.ci; nav.si = 0; nav.screen = "sub"; app.remove(app.first); app.add(new SubCol(PRAYER_DATA[item.ci], {})); }
                else if (item.type === "counter") { nav.count = 0; nav.screen = "counter"; app.remove(app.first); app.add(new CountCol({})); }
                else if (item.type === "badi") { nav.screen = "badi"; app.remove(app.first); app.add(new BadiCol({})); }
            }
        } else if (nav.screen === "sub") {
            const n = PRAYER_DATA[nav.ci].items.length;
            if (type === "up") { nav.si = (nav.si-1+n)%n; highlight(app, nav.si); }
            else if (type === "down") { nav.si = (nav.si+1)%n; highlight(app, nav.si); }
            else if (type === "select") { nav.so = 0; nav.screen = "reader"; const it = PRAYER_DATA[nav.ci].items[nav.si]; app.remove(app.first); app.add(new ReadCol(it, {})); }
            else if (type === "back") { nav.screen = "main"; app.remove(app.first); app.add(new MainCol(MENU, {})); }
        } else if (nav.screen === "reader") {
            if (type === "up") { nav.so = Math.max(0, nav.so-25); app.first.delegate("onScroll", nav.so); }
            else if (type === "down") { nav.so += 25; app.first.delegate("onScroll", nav.so); }
            else if (type === "back") { nav.screen = "sub"; nav.si = nav.si; app.remove(app.first); app.add(new SubCol(PRAYER_DATA[nav.ci], {})); }
        } else if (nav.screen === "counter") {
            if (type === "up" || type === "select") {
                if (nav.count < 95) { nav.count++; app.first.delegate("onCount", nav.count); }
            } else if (type === "down") {
                if (nav.count > 0) { nav.count--; app.first.delegate("onCount", nav.count); }
            } else if (type === "back") { nav.screen = "main"; app.remove(app.first); app.add(new MainCol(MENU, {})); }
        } else if (nav.screen === "badi") {
            if (type === "back" || type === "select") { nav.screen = "main"; app.remove(app.first); app.add(new MainCol(MENU, {})); }
        }
    }
    onMenuTap(app, idx) { nav.mi = idx; this.onButtonPush(app, false, "select"); }
    onSubTap(app, idx) { nav.si = idx; this.onButtonPush(app, false, "select"); }
}

class ReadBeh extends Behavior {
    onDisplaying(c) { this.sc = c.last; }
    onScroll(c, o) { if (this.sc) this.sc.scrollTo(0, o); }
}

class CountBeh extends Behavior {
    onDisplaying(c) { this.lbl = c.first.next; }
    onCount(c, n) { if (this.lbl) this.lbl.string = String(n) + " / 95"; }
}

// ═══════════════════════════════════════════════════════════════
// SECTION 6: TEMPLATES
// ═══════════════════════════════════════════════════════════════

const MainItem = Row.template($ => ({
    top: 0, height: 34, left: 0, right: 0, skin: itemSkin, active: true,
    contents: [
        Label(null, { width: 28, left: 4, style: redStyle, string: $.i }),
        Label(null, { left: 0, right: 0, style: itemStyle, string: $.t })
    ],
    Behavior: class extends Behavior { onTouchEnded(r) { r.bubble("onMenuTap", $.idx); } }
}));

const SubItem = Label.template($ => ({
    top: 0, height: 32, left: 0, right: 0, skin: itemSkin, style: subStyle, active: true, string: $.t,
    Behavior: class extends Behavior { onTouchEnded(l) { l.bubble("onSubTap", $.idx); } }
}));

const MainCol = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0, skin: bgSkin,
    contents: [
        Label(null, { top: 0, height: 30, left: 0, right: 0, skin: hdrSkin, style: hdrStyle, string: "Prayer Book" }),
        ...$.map((c, i) => new MainItem({ t: c.t, i: c.i, idx: i }))
    ]
}));

const SubCol = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0, skin: bgSkin,
    contents: [
        Label(null, { top: 0, height: 30, left: 0, right: 0, skin: hdrSkin, style: hdrStyle, string: $.title }),
        ...$.items.map((it, i) => new SubItem({ t: it.title, idx: i }))
    ]
}));

const ReadCol = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0, skin: bgSkin,
    contents: [
        Label(null, { top: 0, height: 26, left: 0, right: 0, skin: hdrSkin, style: redStyle, string: $.title }),
        new Scroller(null, {
            left: 0, right: 0, top: 0, bottom: 0, active: true, backgroundTouch: true,
            contents: [
                new Text(null, {
                    left: 5, right: 5, top: 5, style: bodyStyle,
                    string: $.text + "\n\n— " + $.by
                })
            ]
        })
    ],
    Behavior: ReadBeh
}));

const CountCol = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0, skin: bgSkin, active: true,
    contents: [
        Label(null, { top: 0, height: 30, left: 0, right: 0, skin: hdrSkin, style: hdrStyle, string: "Alláh-u-Abhá" }),
        Label(null, { top: 15, height: 40, left: 0, right: 0, style: redStyle, string: "0 / 95" }),
        Label(null, { top: 10, height: 30, left: 0, right: 0, style: greyStyle, string: "Up/Select: count\nDown: undo\nBack: exit" })
    ],
    Behavior: CountBeh
}));

const BadiCol = Column.template($ => {
    const b = getBadi();
    return {
        top: 0, bottom: 0, left: 0, right: 0, skin: bgSkin,
        contents: [
            Label(null, { top: 0, height: 30, left: 0, right: 0, skin: hdrSkin, style: hdrStyle, string: "Badí Calendar" }),
            Label(null, { top: 15, height: 30, left: 10, right: 10, style: redStyle, string: b.str }),
            Label(null, { top: 5, height: 24, left: 10, right: 10, style: greyStyle, string: b.year + " B.E." }),
            Label(null, { top: 10, height: 20, left: 0, right: 0, style: greyStyle, string: "Back: menu" })
        ]
    };
});

// ═══════════════════════════════════════════════════════════════
// SECTION 7: BOOTSTRAP
// ═══════════════════════════════════════════════════════════════

const App = Application.template($ => ({
    skin: bgSkin, style: hdrStyle,
    contents: [ new MainCol(MENU, {}) ],
    Behavior: AppBehavior
}));

const application = new App(null, { displayListLength: 4096, touchCount: 0, pixels: screen.width * 4 });

new Button({
    types: ["select", "up", "down", "back"],
    onPush(down, type) { application.delegate("onButtonPush", down, type); }
});

export default application;
