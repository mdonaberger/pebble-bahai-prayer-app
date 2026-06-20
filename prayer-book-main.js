// ═══════════════════════════════════════════════════════════════
// BAHÁ'Í PRAYER BOOK — Pebble Alloy App
// Template: main.js (paste into CloudPebble src/embeddedjs/main.js)
// ═══════════════════════════════════════════════════════════════
//
// HOW TO USE:
// 1. Open CloudPebble → your project → src/embeddedjs/main.js
// 2. Replace ALL content with this file
// 3. Click "Build and Run"
// 4. Use Up/Down to navigate, Select to open, Back to go back
//    (On touch screens: tap items to select)
//
// NAVIGATION: Menu → Submenu → Reader (scrollable text)
// ═══════════════════════════════════════════════════════════════

import {} from "piu/MC";
import Button from "pebble/button";

// ═══════════════════════════════════════════════════════════════
// SECTION 1: DATA — Edit these to customize your prayer book
// ═══════════════════════════════════════════════════════════════

const PRAYER_DATA = [
    {
        title: "Prayers",
        icon: "P",
        items: [
            {
                title: "Morning Prayer",
                text: "I have wakened in Thy shelter, O my God, and within the refuge of Thy care. I have entered Thy dwelling-place, and have found Thy beloved ones gathered in Thy courtyard.\n\nI beseech Thee, O my Lord, by Thy hidden, Thy ancient name, to make me one of them that are steadfast in Thy Cause, and to reveal unto me whatsoever Thou hast decreed for them.\n\nThou art, verily, the Almighty, the All-Knowing."
            },
            {
                title: "Evening Prayer",
                text: "O Thou Who art the Lord of all names and the Maker of the heavens! I entreat Thee, by Them Who are the Dawning-Places of Thine absolute Essence and the Repositories of Thy might, to ordain for me the good of this world and of the world to come.\n\nReveal then unto me, O my God, that which will enrich me from every other thing besides Thee. Thou art, verily, the Lord of all worlds."
            },
            {
                title: "Short Obligatory",
                text: "I bear witness, O my God, that Thou hast created me to know Thee and to worship Thee. I testify, at this moment, to my powerlessness and to Thy might, to my poverty and to Thy wealth.\n\nThere is none other God but Thee, the Help in Peril, the Self-Subsisting."
            },
            {
                title: "For Assistance",
                text: "Thou seest me, O my God, holding to Thy Name, the Most Holy, the Most Luminous, the Most Mighty, the Most Great, the Most Exalted, the Most Glorious, and clinging to the hem of the robe of Thine infinite favors.\n\nSend down upon me, by Thy grace, that which will enable me to dispense with all except Thee, and destine for me from Thyself what will exalt me above all that is in Thy creation."
            },
            {
                title: "For Unity",
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
                text: "O SON OF SPIRIT!\n\nMy first counsel is this: Possess a pure, kindly and radiant heart, that thine may be a sovereignty ancient, imperishable and everlasting."
            },
            {
                title: "Hidden Word Ar 2",
                text: "O SON OF SPIRIT!\n\nThe best beloved of all things in My sight is Justice; turn not away therefrom if thou desirest Me, and neglect it not that I may confide in thee. By its aid thou shalt see with thine own eyes and not through the eyes of others, and shalt know of thine own knowledge and not through the knowledge of thy neighbor.\n\nPonder this in thy heart; how it behoveth thee to be. Verily justice is My gift to thee and the sign of My loving-kindness. Set it then before thine eyes."
            },
            {
                title: "Hidden Word Pe 1",
                text: "O SON OF DESIRE!\n\nThe learned and the wise have for long years striven and endured hardships and trials, have ascended into the heaven of knowledge, and attained to the heights of science and art. Know therefore of a certainty that true life is not the life of the flesh but the life of the spirit."
            },
            {
                title: "On Peace",
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
                text: "The source of all good is trust in God, submission unto His command, and contentment with His holy will and pleasure.\n\nThe essence of wisdom is the fear of God, the dread of His scourge and punishment, and the apprehension of His justice and decree."
            },
            {
                title: "On Service",
                text: "It is incumbent upon each and every one, great or small, high or low, rich or poor, to serve the Cause of God.\n\nWhoso ariseth to serve the Cause of God, him will the Concourse on High assist. So great are the confirmations that will descend upon him that he will be enabled to perform whatsoever is the desire of his heart."
            },
            {
                title: "On Joy",
                text: "Joy gives us wings! In times of joy our strength is more vital, our intellect keener, and our understanding less clouded.\n\nWe seem better able to cope with the world and to find our sphere of usefulness."
            }
        ]
    }
];

// ═══════════════════════════════════════════════════════════════
// SECTION 2: STYLES & SKINS — Theme your app here
// ═══════════════════════════════════════════════════════════════

const bgSkin = new Skin({ fill: "#1a1a2e" });
const headerSkin = new Skin({ fill: "#16213e" });
const itemSkin = new Skin({ fill: ["#1a1a2e", "#0f3460"] });  // [normal, selected]

const titleStyle = new Style({
    font: "bold 18px Gothic", color: "#e8e8e8",
    horizontal: "center", top: 4, bottom: 4
});
const iconStyle = new Style({
    font: "bold 16px Gothic", color: "#e94560",
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
    left: 10, right: 10, top: 5, bottom: 15
});

// ═══════════════════════════════════════════════════════════════
// SECTION 3: BEHAVIORS
// ═══════════════════════════════════════════════════════════════

// Main app — manages navigation between screens + button input
class AppBehavior extends Behavior {
    onCreate(app, data) {
        this.data = data;
        this.screen = "main";
        this.menuIndex = 0;
        this.subIndex = 0;
        this.categoryIndex = 0;
        this.scrollOffset = 0;
    }

    // ── Screen navigation ──
    showMain(app) {
        this.screen = "main";
        this.scrollOffset = 0;
        const screen = new MainColumn(this.data, { menuIndex: this.menuIndex });
        app.replace(app.first, screen);
    }

    showSub(app, categoryIndex) {
        this.screen = "sub";
        this.categoryIndex = categoryIndex;
        this.subIndex = 0;
        this.scrollOffset = 0;
        const category = this.data[categoryIndex];
        const screen = new SubColumn(category, { subIndex: 0 });
        app.replace(app.first, screen);
    }

    showReader(app, categoryIndex, itemIndex) {
        this.screen = "reader";
        this.categoryIndex = categoryIndex;
        this.scrollOffset = 0;
        const item = this.data[categoryIndex].items[itemIndex];
        const screen = new ReaderColumn(item, {});
        app.replace(app.first, screen);
    }

    // ── Touch: item tapped ──
    onMenuTapped(app, index) {
        this.menuIndex = index;
        this.showSub(app, index);
    }
    onSubTapped(app, index) {
        this.subIndex = index;
        this.showReader(app, this.categoryIndex, index);
    }

    // ── Hardware buttons ──
    onPush(app, down, type) {
        if (!down) return;

        if (this.screen === "main") {
            const count = this.data.length;
            if (type === "up") {
                this.menuIndex = (this.menuIndex - 1 + count) % count;
                app.first.delegate("onHighlight", this.menuIndex);
            } else if (type === "down") {
                this.menuIndex = (this.menuIndex + 1) % count;
                app.first.delegate("onHighlight", this.menuIndex);
            } else if (type === "select") {
                this.showSub(app, this.menuIndex);
            }
        } else if (this.screen === "sub") {
            const count = this.data[this.categoryIndex].items.length;
            if (type === "up") {
                this.subIndex = (this.subIndex - 1 + count) % count;
                app.first.delegate("onHighlight", this.subIndex);
            } else if (type === "down") {
                this.subIndex = (this.subIndex + 1) % count;
                app.first.delegate("onHighlight", this.subIndex);
            } else if (type === "select") {
                this.showReader(app, this.categoryIndex, this.subIndex);
            } else if (type === "back") {
                this.showMain(app);
            }
        } else if (this.screen === "reader") {
            if (type === "up") {
                this.scrollOffset = Math.max(0, this.scrollOffset - 25);
                app.first.delegate("onScroll", this.scrollOffset);
            } else if (type === "down") {
                this.scrollOffset += 25;
                app.first.delegate("onScroll", this.scrollOffset);
            } else if (type === "back") {
                this.showSub(app, this.categoryIndex);
            }
        }
    }
}

// Menu behavior — manages highlight state for up/down navigation
class MenuBehavior extends Behavior {
    onCreate(column) {
        this.selectedIndex = 0;
    }
    onDisplaying(column) {
        this.highlight(column, 0);
    }
    onHighlight(column, index) {
        this.highlight(column, index);
    }
    highlight(column, index) {
        // Iterate children: skip header (first child), highlight items
        let child = column.first;
        let i = -1;  // first child is the header
        while (child) {
            if (i >= 0) {
                child.state = (i === index) ? 1 : 0;
            }
            child = child.next;
            i++;
        }
        this.selectedIndex = index;
    }
}

// Reader behavior — manages scroll position
class ReaderBehavior extends Behavior {
    onCreate(column) {
        this.scroller = column.last;  // Scroller is the last child
    }
    onScroll(column, offset) {
        if (this.scroller) {
            this.scroller.scrollTo(0, offset);
        }
    }
}

// ═══════════════════════════════════════════════════════════════
// SECTION 4: TEMPLATES
// ═══════════════════════════════════════════════════════════════

// ── Main menu item: icon + title row ──
const MainItem = Row.template($ => ({
    top: 0, height: 38, left: 0, right: 0,
    skin: itemSkin,
    active: true,
    contents: [
        Label(null, {
            width: 30, left: 4,
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
        onTouchEnded(item) {
            item.bubble("onMenuTapped", $.index);
        }
    }
}));

// ── Sub menu item: title label ──
const SubItem = Label.template($ => ({
    top: 0, height: 34, left: 0, right: 0,
    skin: itemSkin,
    style: subItemStyle,
    active: true,
    string: $.title,
    Behavior: class extends Behavior {
        onTouchEnded(item) {
            item.bubble("onSubTapped", $.index);
        }
    }
}));

// ── Main menu screen ──
const MainColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    contents: [
        Label(null, {
            top: 0, height: 32, left: 0, right: 0,
            skin: headerSkin,
            style: titleStyle,
            string: "Prayer Book"
        }),
        ...$.map((cat, i) =>
            new MainItem({ title: cat.title, icon: cat.icon, index: i })
        )
    ],
    Behavior: MenuBehavior
}));

// ── Sub menu screen ──
const SubColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    contents: [
        Label(null, {
            top: 0, height: 32, left: 0, right: 0,
            skin: headerSkin,
            style: titleStyle,
            string: $.title
        }),
        ...$.items.map((item, i) =>
            new SubItem({ title: item.title, index: i })
        )
    ],
    Behavior: MenuBehavior
}));

// ── Reader screen: title + scrollable text ──
const ReaderColumn = Column.template($ => ({
    top: 0, bottom: 0, left: 0, right: 0,
    skin: bgSkin,
    contents: [
        Label(null, {
            top: 0, height: 28, left: 0, right: 0,
            skin: headerSkin,
            style: readerTitleStyle,
            string: $.title
        }),
        new Scroller(null, {
            left: 0, right: 0, top: 0, bottom: 0,
            active: true,
            backgroundTouch: true,
            contents: [
                new Text(null, {
                    left: 0, right: 0, top: 5,
                    style: textStyle,
                    string: $.text
                })
            ]
        })
    ],
    Behavior: ReaderBehavior
}));

// ═══════════════════════════════════════════════════════════════
// SECTION 5: APP BOOTSTRAP
// ═══════════════════════════════════════════════════════════════

const application = new Application(null, {
    skin: bgSkin,
    style: titleStyle,
    displayListLength: 8192,
    touchCount: 0,
    pixels: screen.width * 4,
    contents: [
        new MainColumn(PRAYER_DATA, { })
    ],
    Behavior: AppBehavior
});

new Button({
    types: ["select", "up", "down", "back"],
    onPush(down, type) {
        application.delegate("onPush", down, type);
    }
});

export default application;
