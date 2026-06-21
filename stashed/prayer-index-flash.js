// prayer-index.js — lightweight title-only index
// Full prayer text lives in raw resource "prayers" (flash)

const OBLI_T = "Short Obligatory Prayer";
const GENE_T = "O Thou Whose face is the object...\x01O Thou kind Lord!\x01O Lord, my God! This is a child...\x01Prayer for the Dead\x01Suffer me, O my God...\x01Glory to Thee, O my God!\x01O God, my Master, the Goal of my desire!\x01Glory be unto Thee, O Lord my God!\x01Glory be to Thee, O King of eternity...\x01Glorified art Thou, O Lord my God! I beseech \x01O God, my God!\x01Glorified art Thou, O Lord my God!\x01He is God! Thou seest us, O my God...\x01O God my God! I beg of Thee...\x01My God, Whom I worship and adore!\x01Thy unity is inscrutable, O my God...\x01He is the Bestower, the Bounteous!\x01O my God and my Master!\x01Glory be to Thee, O my God!\x01Unto Thee be praise, O Lord my God!\x01All praise, O my God, be to Thee...\x01Lauded be Thy name, O my God! Thou beholdest \x01Praise be to Thee, O Lord my God!\x01Magnified be Thy name, O God...\x01O God, and the God of all Names...\x01From the sweet-scented streams...\x01I magnify Thy Name, O my God...\x01Magnified be Thy name...\x01O Thou incomparable God!\x01O Thou Whose tests are a healing medicine...\x01Praise be to Thee, O Lord my God!\x01Thou seest, O my God...\x01O my God! O my God! Unite the hearts...\x01O Thou Whose face is the object...\x01O Thou kind Lord! From the horizon...";
const OCCA_T = "This is, O my God, the first of the days...\x01Magnified art Thou, O Lord...\x01Intercalary Days\x01O God, my God! We are servants of Thine...\x01Tablet of Ahmad";

export const CATS = [
    { t: "Obligatory", i: "O", n: 1, T: OBLI_T },
    { t: "General", i: "G", n: 35, T: GENE_T },
    { t: "Occasional", i: "C", n: 5, T: OCCA_T },
];