#include <std/mem.pat>

struct Vectoru16 {
    u16 x;
    u16 y;
};

struct Vectoru32 {
    u32 x;
    u32 y;
};

struct TileSet {
    u16 firstgid;
    char source[];
};

enum MapType : u8 {
    TileMap
};

struct Map {
    u8 type;
};

struct TileMap : Map {
    char name[];
    Vectoru32 size;
    u16 tiles[size.x * size.y];
};

struct Document {
    char header[3];
    u16 version;
    Vectoru16 tilesz;
    TileSet sets[while(std::mem::read_unsigned($, 2) != 0x0)];
    u16 setsend;
    TileMap maps[];
};

Document doc @ 0x00;

if (doc.header != "C2M")
    std::error("Mismatched header!");