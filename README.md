![image](https://images.mitch.science/i/9d611fea-0068-408c-92e9-eec66b0ad412.jpg)

# 🪨 Mitch's Bahá'í Prayer App (for Pebble Time 2 & Pebble Round 2)

## Where Do I Get It?

Grab the latest release from the Pebble app store, or click [here](https://apps.repebble.com/87069cdc983047dca17368d6).

## What Is It?

A Bahá'í prayer book app for the latest Pebble smartwatches, built with the native C SDK. All 249 prayers from the standard prayer book, an Alláh-u-Abhá 95× counter, a Badí calendar, and more — all on your wrist.

> Dedicated to my dear friend Meg Moyer.
> Free & open source.
> To all of those who labor in service of the unity of all humankind.

## Features

- **249 prayers** across three categories:
  - Obligatory Prayers (Short, Medium, Long)
  - General Prayers
  - Occasional Prayers
- **Scrollable reader** with word counts shown in the submenu for each prayer
- **Alláh-u-Abhá counter** — tap to count, vibrates at 95, resettable
- **Badí calendar** — shows today's Bahá'í date (day, month, and year B.E.)
- Built entirely in C — no phone connection required

## Target Platforms

| Platform | Watch | Display | Supported |
|----------|-------|---------|-----------|
| **Emery** | Pebble Time 2 | 200×228 color | ✅ Primary |
| **Gabbro** | Pebble 2 Duo | 260×260 round | ✅ Primary |

Older platforms (Aplite, Basalt, Chalk, Diorite, Flint) are not supported due to 64KB RAM limits and 64KB app binary size constraints.

## Building

### Prerequisites

- A [CloudPebble](https://cloudpebble.net/) account, or the [Pebble SDK](https://developer.rebble.io/sdk/) installed locally

### CloudPebble

1. Create a new project → choose **C**
2. Set target platforms to **Emery** and **Gabbro** only
3. Upload `long_obligatory.txt` as a resource:
   - **Identifier:** `LONG_OBLIGATORY`
   - **Type:** `raw`
4. Paste `src/c/main.c` into the source editor
5. Build and Run

### Local SDK

```bash
pebble build
pebble install --emulator emery
```

## Technical Notes

- **Long Obligatory Prayer** (1,656 words) is stored as a flash resource rather than inlined in the binary, keeping the app under the 64KB binary size limit
- **Reader buffer** is heap-allocated (`malloc`/`free` per window) to avoid inflating the app's virtual memory size
- Prayer strings are stored as C string constants in `.rodata` (flash), not copied to RAM
- Memory footprint: ~74KB RAM used, ~57KB heap free at runtime

## Project Structure

```
src/
  c/
    main.c              — entire app (menus, reader, counter, calendar, about)
    mdbl.c              — Moddable bridge (required, do not modify)
resources/
  data/
    long_obligatory.txt — Long Obligatory Prayer text (flash resource)
```

## Version

**0.5** — First Release

## License

Free & open source.

## Author

**Mitch Donaberger**  
[donaberger.xyz](https://donaberger.xyz)  
mitch@donaberger.xyz

