#include <std/mem.pat>

struct Vector {
    float x;
    float y;
};

struct Vectoru16 {
    u16 x;
    u16 y;
};

struct Vectoru32 {
    u32 x;
    u32 y;
};

struct Vectors16 {
    s16 x;
    s16 y;
};

struct Vectors32 {
    s32 x;
    s32 y;
};

struct TileSet {
    u16 firstgid;
    char source[];
};

enum LayerType : u8 {
    TileLayer,
    ObjectLayer
};

using TileLayerData;
using ObjectLayerData;

struct Layer {
    LayerType type;
    char name[];

    match (type) {
        (LayerType::TileLayer): TileLayerData data;
        (LayerType::ObjectLayer): ObjectLayerData data;
    }
};

struct TileLayerData {
    Vectoru32 size;
    u16 tiles[size.x * size.y];
};

using Actor;

struct ObjectLayerData {
    Vectoru32 size;
    Actor actors[];
};

enum PropertyType : u8 {
    Uint8,
    Uint16,
    Uint32,
    Uint64,

    Int8,
    Int16,
    Int32,
    Int64,

    Float,

    String,

    Vector,
    Vectoru32,
    Vectori32,
};

struct ActorProperty {
    PropertyType type;

    match (type) {
        (PropertyType::Uint8): u8 data;
        (PropertyType::Uint16): u16 data;
        (PropertyType::Uint32): u32 data;
        (PropertyType::Uint64): u64 data;

        (PropertyType::Int8): s8 data;
        (PropertyType::Int16): s16 data;
        (PropertyType::Int32): s32 data;
        (PropertyType::Int64): s64 data;

        (PropertyType::Float): float data;

        (PropertyType::String): char data[];

        (PropertyType::Vector): Vector data;
        (PropertyType::Vectoru32): Vectoru32 data;
        (PropertyType::Vectori32): Vectors32 data;
    }
};

struct Actor {
    char name[];

    ActorProperty properties[];
};

struct Document {
    char header[3];
    u16 version;
    Vectoru16 tilesz;
    TileSet sets[while(std::mem::read_unsigned($, 2) != 0x0)];
    padding[2];
    Layer layers[];
};

Document doc @ 0x00;

if (doc.header != "C2M")
    std::error("Mismatched header!");
