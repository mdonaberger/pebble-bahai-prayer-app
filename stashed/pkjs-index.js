// ═══════════════════════════════════════════════════════════════
// PKJS — PebbleKit JS (runs on phone)
// Stream prayer text to watch on demand
// ═══════════════════════════════════════════════════════════════
// File: src/pkjs/index.js
//
// Message keys (must match package.json messageKeys):
//   PRAYER_ID     - watch sends prayer ID (int)
//   PRAYER_TITLE  - PKJS responds with title (string)
//   PRAYER_AUTHOR - PKJS responds with author (string)
//   PRAYER_TEXT   - PKJS responds with text chunk (string)
//   PRAYER_MORE   - 1 = more chunks coming, 0 = last chunk
//   PKJS_READY    - 1 = PKJS is loaded and ready
// ═══════════════════════════════════════════════════════════════

const PRAYERS = require('./full-prayers.json');
const PRAYER_LIST = PRAYERS.prayers;

const CHUNK_SIZE = 800;

console.log("Prayer Book PKJS ready — " + PRAYER_LIST.length + " prayers loaded");

Pebble.addEventListener('ready', function() {
    console.log("PKJS sending ready signal");
    Pebble.sendAppMessage({ 'PKJS_READY': 1 });
});

Pebble.addEventListener('appmessage', function(e) {
    const id = e.payload.PRAYER_ID;
    if (id !== undefined && id > 0) {
        const prayer = PRAYER_LIST.find(function(p) { return p.id === id; });
        if (prayer) {
            sendPrayer(prayer);
        } else {
            console.log("Prayer not found: " + id);
        }
    }
});

function sendPrayer(prayer) {
    // Send title + author first
    Pebble.sendAppMessage({
        'PRAYER_TITLE': prayer.title,
        'PRAYER_AUTHOR': prayer.author || "",
        'PRAYER_TEXT': "",
        'PRAYER_MORE': 1
    }, function() {
        // Ack received — now send text in chunks
        sendChunks(prayer.body || "");
    }, function() {
        console.log("Failed to send prayer header");
    });
}

function sendChunks(text) {
    var pos = 0;
    
    function sendNext() {
        var chunk = text.substring(pos, pos + CHUNK_SIZE);
        var isLast = (pos + CHUNK_SIZE) >= text.length;
        
        Pebble.sendAppMessage({
            'PRAYER_TEXT': chunk,
            'PRAYER_MORE': isLast ? 0 : 1
        }, function() {
            // Ack — send next chunk if more
            if (!isLast) {
                pos += CHUNK_SIZE;
                sendNext();
            }
        }, function() {
            console.log("Chunk failed at pos " + pos + ", retrying...");
            // Retry after short delay
            setTimeout(sendNext, 200);
        });
    }
    
    sendNext();
}
