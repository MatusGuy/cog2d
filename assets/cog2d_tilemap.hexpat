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

struct Rect {
    Vector pos;
    Vector size;
};

struct Rects32 {
    Vectors32 pos;
    Vectors32 size;
};

struct Color {
    u8 a, r, g, b;
};

struct TileSet {
    u16 firstgid;
    char source[];
};

enum LayerType : u8 {
    TileLayer = 0,
    ObjectLayer = 1
};

using TileLayerData;
using ObjectLayerData;

struct Layer {
    LayerType type;
    
    match (type) {
        (LayerType::TileLayer): TileLayerData data;
        (LayerType::ObjectLayer): ObjectLayerData data;
    }
};

struct TileLayerData {
    u8 type;
    Vectoru32 size;
    u16 tiles[size.x * size.y];
};

using Actor;

struct ObjectLayerData {
    // FIXME
    Actor actors[while(!std::mem::eof())];
};

enum PropertyType : u8 {
    Int = 1,
    Bool,
    Float,
    String,
    Vector,
    Vectori,
    Rect,
    Recti,
    Color
};

struct ActorProperty {
    u8 idx;
    PropertyType type;
    
    match (type) {
        (PropertyType::Int): s32 data;
        (PropertyType::Float): float data;
        (PropertyType::String): char data[];
        (PropertyType::Vector): Vector data;
        (PropertyType::Vectori): Vectors32 data;
        (PropertyType::Rect): Rect data;
        (PropertyType::Recti): Rects32 data;
        // TODO: Color
    }
};

struct Actor {
    char classname[];
    
    ActorProperty properties[while(std::mem::read_unsigned($, 1) != 0x0)];
    padding[1];
};

struct Document {
    char header[3];
    u16 version;
    Vectoru16 tilesz;
    TileSet sets[while(std::mem::read_unsigned($, 2) != 0x0)];
    padding[2];
    Layer layers[while(!std::mem::eof())];
    //Layer layers[1];
};

Document doc @ 0x00;

if (doc.header != "C2M")
    std::error("Mismatched header!");