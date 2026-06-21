// ═══════════════════════════════════════════════════════════════
// BAHÁ'Í PRAYER BOOK — Pebble Alloy App v5
// All 41 prayers via flat-string data module
// ═══════════════════════════════════════════════════════════════

import {} from "piu/MC";
import Button from "pebble/button";
import { CATS, getPrayer } from "./prayer-data";

// ═══════════════════════════════════════════════════════════════
// SECTION 1: BADÍ CALENDAR
// ═══════════════════════════════════════════════════════════════

const BM = ["","Bahá","Jalál","Jamál","'Aẓamat","Núr","Raḥmat","Kalimát","Kamál","Asmá","'Izzat","Mashíyyat","'Ilm","Qudrat","Qawl","Masá'il","Sharaf","Sulṭán","Mulk","'Alá"];
const BE = ["","Splendour","Glory","Beauty","Grandeur","Light","Mercy","Words","Perfection","Names","Might","Will","Knowledge","Power","Speech","Questions","Honour","Sovereignty","Dominion","Loftiness"];

function isLeapYear(y) { return (y%4===0 && y%100!==0) || (y%400===0); }

function getBadi() {
    const d = new Date();
    const y = d.getFullYear();
    const ly = isLeapYear(y);
    const md = [0,31,59,90,120,151,181,212,243,273,304,334];
    let doy = md[d.getMonth()] + d.getDate();
    if (d.getMonth() > 1 && ly) doy++;
    const nrz = ly ? 80 : 79;
    let by = y - 1843;
    let ds;
    if (doy >= nrz) { ds = doy - nrz; }
    else { by = y - 1844; ds = doy + (isLeapYear(y-1) ? 366 : 365) - nrz; }
    const ac = ly ? 5 : 4;
    let m, day, ia = false;
    if (ds < 342) { m = Math.floor(ds/19)+1; day = (ds%19)+1; }
    else if (ds < 342+ac) { ia = true; day = ds-342+1; }
    else { m = 19; day = ds-342-ac+1; }
    return { str: ia ? ("Ayyám-i-Há " + day) : (day + " " + BM[m] + " (" + BE[m] + ")"), year: by };
}

// ═══════════════════════════════════════════════════════════════
// SECTION 2: STYLES
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
// SECTION 3: NAV STATE
// ═══════════════════════════════════════════════════════════════

let nav = { screen: "main", mi: 0, si: 0, ci: 0, so: 0, count: 0 };

const MENU = [
    { t: "Obligatory", i: "O", type: "data", ci: 0 },
    { t: "General", i: "G", type: "data", ci: 1 },
    { t: "Occasional", i: "C", type: "data", ci: 2 },
    { t: "Alláh-u-Abhá", i: "9", type: "counter" },
    { t: "Badí Date", i: "B", type: "badi" }
];

// ═══════════════════════════════════════════════════════════════
// SECTION 4: BEHAVIORS
// ═══════════════════════════════════════════════════════════════

function highlight(app, idx) {
    const c = app.first;
    if (!c) return;
    let ch = c.first, i = -1;
    while (ch) { if (i >= 0) ch.state = (i === idx) ? 1 : 0; ch = ch.next; i++; }
}

class AppBehavior extends Behavior {
    onButtonPush(app, down, type) {
        if (!down) return;
        if (nav.screen === "main") {
            const n = MENU.length;
            if (type === "up") { nav.mi = (nav.mi-1+n)%n; highlight(app, nav.mi); }
            else if (type === "down") { nav.mi = (nav.mi+1)%n; highlight(app, nav.mi); }
            else if (type === "select") {
                const item = MENU[nav.mi];
                if (item.type === "data") { nav.ci = item.ci; nav.si = 0; nav.screen = "sub"; app.remove(app.first); app.add(new SubCol(CATS[item.ci], {})); }
                else if (item.type === "counter") { nav.count = 0; nav.screen = "counter"; app.remove(app.first); app.add(new CountCol({})); }
                else if (item.type === "badi") { nav.screen = "badi"; app.remove(app.first); app.add(new BadiCol({})); }
            }
        } else if (nav.screen === "sub") {
            const n = CATS[nav.ci].n;
            if (type === "up") { nav.si = (nav.si-1+n)%n; highlight(app, nav.si); }
            else if (type === "down") { nav.si = (nav.si+1)%n; highlight(app, nav.si); }
            else if (type === "select") {
                nav.so = 0; nav.screen = "reader";
                const p = getPrayer(nav.ci, nav.si);
                app.remove(app.first);
                app.add(new ReadCol(p, {}));
            }
            else if (type === "back") { nav.screen = "main"; app.remove(app.first); app.add(new MainCol(MENU, {})); }
        } else if (nav.screen === "reader") {
            if (type === "up") { nav.so = Math.max(0, nav.so-25); app.first.delegate("onScroll", nav.so); }
            else if (type === "down") { nav.so += 25; app.first.delegate("onScroll", nav.so); }
            else if (type === "back") { nav.screen = "sub"; app.remove(app.first); app.add(new SubCol(CATS[nav.ci], {})); }
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
// SECTION 5: TEMPLATES
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
    top: 0, height: 32, left: 0, right: 0, skin: itemSkin, style: subStyle, active: true, string: $.title,
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
        Label(null, { top: 0, height: 30, left: 0, right: 0, skin: hdrSkin, style: hdrStyle, string: $.t }),
        ...range($.n).map(i => {
            const titles = $.T.split("\x01");
            return new SubItem({ title: titles[i], idx: i });
        })
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

function range(n) { const a = []; for (let i = 0; i < n; i++) a.push(i); return a; }

// ═══════════════════════════════════════════════════════════════
// SECTION 6: BOOTSTRAP
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
