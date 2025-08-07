/// <reference types="@mapeditor/tiled-api" />
import TOML from "smol-toml";

// @ts-ignore
const PLATFORM = File.exists("C:/Windows/System32/KERNELBASE.dll") ? "windows" : "unix";

// - Settings
const SETTING_EXE_PATH = "cog2d.executable"
const SETTING_MAP_COMPRESS = "cog2d.mapCompress"
const SETTING_EMBED_TILESETS = "cog2d.embedTilesets"

// - Actions
const ACTION_TEST = "cog2d.TestMap"

// Types
type TomlDocument = Object & {
	type: string;
	version: number
}

type TomlTileSetData = Object & {
	tilewidth: number;
	tileheight: number;
	columns: number;
	imageheight: number;
	image: string;
};

type TomlTileSetDocument = TomlDocument & Partial<TomlTileSetData>;

type TomlEmbeddedTileSet = Object & {
	firstgid: number;
} & Partial<{ source: string }> & Partial<TomlTileSetData>;

type TomlTileLayer = Object & {
	name: string;
	type: "tilelayer";
	width: number;
	height: number;
	
	data: number[];
};

type TomlTileMapDocument = TomlDocument & {
	tilesets: TomlEmbeddedTileSet[];
	layers: TomlTileLayer[];
};

let process = new Process();

function getTempPath(): string {
	switch (PLATFORM) {
	case "windows": return process.getEnv("APPDATA") + "/Temp/";
	case "unix": return "/tmp/";
	}
}

function escapeRegExp(text: string): string {
	return text.replace(/[.*+?^${}()|[\]\\]/g, '\\$&');
}

function getDirPath(path: string): string {
	const matches = path.match(/^.*\//);
	return matches != null ? matches[0] : "";
}

function getAssetFilePath(path: string): string {
	let r = new RegExp(`(${escapeRegExp(getDirPath(tiled.project.fileName))})(.*)$`, "");
	const matches = path.match(r);
	tiled.log(`Converting ${path} to ${matches}`);
	return matches != null && matches.length > 1 ? matches[2] : "";
}

/// Replace source asset path with a path used in game
function getRealAssetFilePath(path: string): string {
	return path.replace(/src\/(?!.*src\/)/, "");
}

function getPathFileName(path: string): string {
	const matches = path.match(/[^\\/]+$/);
	return matches != null ? matches[0] : "";
}

function getPathBaseName(path: string): string {
	const matches = path.match(/[^\\/]+(?=\..*$)/);
	return matches != null ? matches[0] : "";
}

function getTilesetData(tileset: Tileset): TomlTileSetData {
	return {
		tilewidth: tileset.tileWidth,
		tileheight: tileset.tileHeight,
		columns: tileset.columnCount,
		imageheight: tileset.imageHeight,
		image: getAssetFilePath(tileset.imageFileName),
	};
};

let tomlSet: ScriptedTilesetFormat = {
	name: "cog2d TOML Tileset",
	extension: "toml",

	write: function (tileset: Tileset, fileName: string): string | undefined {
		let doc: TomlTileSetDocument = {
			type: "tileset",
			version: 1
		};

		Object.assign(doc, getTilesetData(tileset));

		let file = new TextFile(fileName, TextFile.WriteOnly);
		file.write(TOML.stringify(doc));
		file.commit();

		return undefined;
	}
};
tiled.registerTilesetFormat("c2tomlset", tomlSet);

function getTileId(layer: TileLayer, x: number, y: number): number {
	const tile = layer.tileAt(x, y);
	if (layer.className != "") {
		return tile ? tile.id + 1 : 0;
	}

	const firstgid = tile != null ? tile.tileset.property("cog2d._firstgid") as number : 0;
	return tile != null ? tile.id + firstgid : 0;
}

function writeTiles(layer: TileLayer, data: number[]) {
	for (let y = 0; y < layer.height; ++y) {
		for (let x = 0; x < layer.width; ++x) {
			const id = getTileId(layer, x, y);
			data.push(id);
		}
	}
};

function writeTilesCompressed(layer: TileLayer, data: number[]) {
	tiled.warn("Compressed tiles are currently not supported by cog2d.", () => {});

	let repeatid = 0;
	let repeats = 1;

	for (let y = 0; y < layer.height; ++y) {
		for (let x = 0; x < layer.width; ++x) {
			const id = getTileId(layer, x, y);

			if (repeatid == id) {
				repeats++;
				continue;
			}

			if (repeats != 1)
				data.push(-repeats);

			repeats = 1;
			data.push(id);

			repeatid = id;
		}
	}
};

let tomlMap: ScriptedMapFormat = {
	name: "cog2d TOML Map",
	extension: "toml",

	write: function (map: TileMap, fileName: string): string | undefined {
		let fileDat: TomlTileMapDocument = {
			type: "tilemap",
			version: 1,
			tilesets: [],
			layers: [],
		};

		let nextGid = 1;

		let usedTilesets = map.usedTilesets();

		for (let setIndex = 0; setIndex < usedTilesets.length; setIndex++) {
			let activeSet = usedTilesets[setIndex];
			let setData: TomlEmbeddedTileSet = {
				firstgid: nextGid
			}

			if (tiled.project.property(SETTING_EMBED_TILESETS)) {
				Object.assign(setData, getTilesetData(activeSet));
			} else {
				setData.source = getPathFileName(activeSet.fileName);
			}

			activeSet.setProperty("cog2d._firstgid", nextGid);
			nextGid += activeSet.tileCount;

			fileDat.tilesets.push(setData);

			tiled.log(`Processed tileset '${setIndex}'\nResult: ${TOML.stringify(setData)}`);
		}

		for (let layerIndex = 0; layerIndex < map.layerCount; layerIndex++) {
			let layer: Layer = map.layerAt(layerIndex);

			if (!layer.isTileLayer) {
				tiled.log(`Processed tilemap layer '${layerIndex}' with early return`)
				continue;
			}

			let tilelayer: TileLayer = layer as TileLayer;

			let data: TomlTileLayer = {
				name: tilelayer.name,
				type: "tilelayer",
				width: tilelayer.width,
				height: tilelayer.height,
				data: []
			};

			if (tiled.project.property(SETTING_MAP_COMPRESS))
				writeTilesCompressed(tilelayer, data.data);
			else
				writeTiles(tilelayer, data.data);

			fileDat.layers.push(data);

			tiled.log(`Processed tilemap layer '${layerIndex}'`)
		}

		let file = new TextFile(fileName, TextFile.WriteOnly);
		file.write(TOML.stringify(fileDat));
		file.commit();

		return undefined;
	},
};

type ObjectSize = 1 | 2 | 4;

class BinaryFileStream {
	file: BinaryFile

	constructor(file: BinaryFile) {
		this.file = file;
	}

	write_array(data: number[], size: ObjectSize, unsigned: boolean) {
		let buf = new ArrayBuffer(data.length * size);
		let arr: any = null;
		if (unsigned) {
			switch (size) {
			case 1: arr = new Uint8Array(buf); break;
			case 2: arr = new Uint16Array(buf); break;
			case 4: arr = new Uint32Array(buf); break;
			}
		} else {
			switch (size) {
			case 1: arr = new Int8Array(buf); break;
			case 2: arr = new Int16Array(buf); break;
			case 4: arr = new Int32Array(buf); break;
			}
		}

		arr.set(data, 0);
		this.file.write(buf);
	}

	write_array_float(data: number[]) {
		let buf = new ArrayBuffer(data.length * 4);
		let arr = new Float32Array(buf);
		arr.set(data, 0);
		this.file.write(buf);
	}

	write_object(data: number, size: ObjectSize, unsigned: boolean) {
		if (Number.isInteger(data))
			this.write_array([data], size, unsigned);
		else
			this.write_array_float([data]);
	}

	write_string(data: string) {
		const arr = Array.from(data + "\0").map(char => char.charCodeAt(0));
		this.write_array(arr, 1, true);
	}
}

enum PropertyType {
	None,
	Int,
	Float,
	Bool,
	String,
	Vector,
	Vectori,
	Rect,
	Recti,
	Color
};

type Vector = {
	x: number;
	y: number;
};

type Vectori = {
	x: number;
	y: number;
};

type Rect = {
	pos: Vector;
	size: Vector;
};

type Recti = {
	pos: Vectori;
	size: Vectori;
};

class Color {
	a!: number;
	r!: number;
	g!: number;
	b!: number;

	constructor(col: color) {
		let str: string = col.toString();
		if (!str.startsWith("#"))
			return;

		let off = 1;
		this.a = 0xFF;

		// Seven characters because one hashtag + six hexadecimal digits
		if (str.length > 7) {
			// This color has an explicit alpha value. (not 0xFF)
			this.a = parseInt(str.substring(off, off + 2), 16);
			off += 2;
		}

		this.r = parseInt(str.substring(off, off + 2), 16);
		off += 2;

		this.g = parseInt(str.substring(off, off + 2), 16);
		off += 2;

		this.b = parseInt(str.substring(off, off + 2), 16);
	}
}

function propTypeFromProperty(value: TiledObjectPropertyValue): PropertyType {
	switch (typeof value) {
	case "number":
		if (Number.isInteger(value))
			return PropertyType.Int;
		else
			return PropertyType.Float;

	case "boolean":
		return PropertyType.Bool;

	case "string":
		return PropertyType.String;

	case "object":
		if (value.toString().startsWith("#"))
			return PropertyType.Color;

		let propval = value as PropertyValue;

		if (!propval)
			break;

		if ((typeof propval.value) == "number")
			return PropertyType.Int;

		switch (propval.typeName) {
		case "cog2d.Vector":
			return PropertyType.Vector;

		case "cog2d.Vectori":
			return PropertyType.Vectori;

		case "cog2d.Rect":
			return PropertyType.Rect;

		case "cog2d.Recti":
			return PropertyType.Recti;
		}
	}

	tiled.log("oops");
	return PropertyType.None;
}

function writePropertyBin(stream: BinaryFileStream, id: number, value: TiledObjectPropertyValue) {
	stream.write_object(id + 1, 1, true);

	let typeid = propTypeFromProperty(value);
	stream.write_object(typeid, 1, true);

	switch (typeid) {
		case PropertyType.Int:
		case PropertyType.Float:
			stream.write_object(value as number, 4, false);
			break;

		case PropertyType.Bool:
			stream.write_object((value as boolean) ? 1 : 0, 1, true);
			break;

		case PropertyType.String:
			stream.write_string(value as string);
			break;

		case PropertyType.Color: {
			let col = new Color(value as color);
			stream.write_array([col.r, col.g, col.b, col.a], 1, true);
			break;
		}

		case PropertyType.Vector: {
			let p = value as PropertyValue;
			let v = p.value as Vector;
			stream.write_array_float([v.x, v.y]);
			break;
		}

		case PropertyType.Vectori: {
			let p = value as PropertyValue;
			let v = p.value as Vectori;
			stream.write_array([v.x, v.y], 4, false);
			break;
		}

		case PropertyType.Rect: {
			let p = value as PropertyValue;
			let r = p.value as Rect;
			stream.write_array_float([r.pos.x, r.pos.y, r.size.x, r.size.y]);
			break;
		}

		case PropertyType.Recti: {
			let p = value as PropertyValue;
			let r = p.value as Recti;
			stream.write_array([r.pos.x, r.pos.y, r.size.x, r.size.y], 4, false);
			break;
		}
	}
}

/*
// Because Tiled does not give resolved object properties on member objects
function getResolvedObjectProperties(props: TiledObjectProperties): TiledObjectProperties {

	let out = props;

	let keys = Object.keys(out);
	for (let i = 0; i < keys.length; ++i) {
		let prop = out[keys[i]];

		if (typeof prop != "object" || prop["values"] == undefined)
			// This is not a PropertyValue.
			continue;

		let propval = prop as PropertyValue;
		let proptype = tiled.project.findTypeByName(prop.typeName);

		if (!proptype || !proptype.isClass)
			continue;

		let cl = proptype as ClassPropertyType;
	}

	return out;
}
*/

function resolveSuperclassProperties(obj: any) {
	let out: TiledObjectProperties = {};

	// If I have to write another line of TypeScript... That's it. No more Marty.
	let superclass = obj["cog2d.super"] as PropertyValue;
	if (superclass != undefined)
		Object.assign(out, resolveSuperclassProperties(superclass.value as any));

	Object.assign(out, obj);

	if (superclass != undefined)
		delete out["cog2d.super"];

	return out;
}

/// Resolves superclass properties
function resolveObjectProperties(obj: TiledObject): TiledObjectProperties {
	let out: TiledObjectProperties = {};

	// If I have to write another line of TypeScript... That's it. No more Marty.
	let superclass = obj.resolvedProperties()["cog2d.super"] as PropertyValue;
	if (superclass != undefined)
		Object.assign(out, resolveSuperclassProperties(superclass.value as any));

	Object.assign(out, obj.properties());

	return out;
}

function writeObjectGroupBin(stream: BinaryFileStream, group: ObjectGroup) {
	tiled.log(`Writing object group '${group.name}'`);
	stream.write_object(1, 1, true);

	for (let i = 0; i < group.objectCount; ++i) {
		let obj = group.objectAt(i);

		let props = resolveObjectProperties(obj);

		if (props["skip"] as boolean) {
			tiled.log(`Skipping '${obj.name}' as requested`);
			continue;
		}

		let memberOrder = props["memberOrder"]?.toString().split("\n");
		if (memberOrder == undefined) {
			tiled.warn(`Skipping '${obj.name}' because it has no member order.\n` +
					   `Is it even derived from cog2d.Actor?`, () => {});
			continue;
		}

		tiled.log(`Writing object '${obj.name}' of class '${obj.className}'`);
		stream.write_string(obj.className);

		tiled.log(` Properties: ${JSON.stringify(props)}`);

		// Increment this every time a cog2d-only property is written.
		let startidx = 0;

		let comps = props.components?.toString().split("\n");
		if (comps) {
			if (comps.find(comp => comp == "Geometry")) {
				writePropertyBin(stream, startidx, {
					value: { x: obj.pos.x, y: obj.pos.y },
					typeId: 69420, // u can tell it doesnt matter what i put here
					typeName: "cog2d.Vector"
				});
				++startidx;
			}
			// add more soon...
		}

		let keys = Object.keys(props);
		for (let j = 0; j < keys.length; ++j) {
			let key = keys[j];

			let value = props[key];

			if (value == undefined) {
				tiled.warn(` Skipping '${key}' because gotta love typescript!`, () => {});
				continue;
			}

			let propid = memberOrder.findIndex(prop => prop == key);
			if (propid == -1) {
				tiled.log(` Skipping '${key}' because it is not specified in member order.`);
				continue;
			}

			tiled.log(` Writing '${key}' with value ${value.toString()}`);
			writePropertyBin(stream, startidx + propid, value);
		}
		stream.write_object(0, 1, true);
	}
	stream.write_object(0, 1, true);
}

function writeTileLayerBin(stream: BinaryFileStream, layer: TileLayer) {
	tiled.log(`Writing tilelayer '${layer.name}'`);
	stream.write_object(0, 1, true);

	let layerprops = resolveObjectProperties(layer);
	tiled.log(` Properties: ${JSON.stringify(layerprops)}`);

	let typeidx = layerprops["typeIndex"] as number;
	stream.write_object(typeidx == undefined ? 0 : typeidx, 1, true);

	stream.write_object(layer.width, 4, true);
	stream.write_object(layer.height, 4, true);

	let tiles: number[] = [];
	let compress = tiled.project.property(SETTING_MAP_COMPRESS) as boolean;
	if (compress)
		writeTilesCompressed(layer, tiles);
	else
		writeTiles(layer, tiles);

	stream.write_array(tiles, 2, !compress);
}

let binMap: ScriptedMapFormat = {
	name: "cog2d Binary Map",
	extension: "dat",

	write: function (map: TileMap, fileName: string): string | undefined {
		let out = new BinaryFileStream(new BinaryFile(fileName, BinaryFile.WriteOnly));

		// Header
		//                  C     2     M
		out.write_array([0x43, 0x32, 0x4D], 1, true);

		// Version
		out.write_object(1, 2, true);

		let nextGid = 1;
		let usedTilesets = map.usedTilesets();

		// There can only be one tilesize per map.
		out.write_object(usedTilesets[0].tileWidth, 2, true);
		out.write_object(usedTilesets[0].tileHeight, 2, true);

		for (let setIndex = 0; setIndex < usedTilesets.length; setIndex++) {
			let activeSet = usedTilesets[setIndex];

			// TODO: Customizable thru editor
			if (activeSet.name == "collision")
				continue;

			out.write_object(nextGid, 2, true);

			let setName = getPathBaseName(activeSet.fileName);
			tiled.log(`Referencing set '${setName}' with path '${activeSet.fileName}'`);
			if (setName == "") {
				tiled.error("Could not retrieve filepath basename from path " +
							activeSet.fileName + ". Make sure Embed Tilesets is disabled.",
							() => {});
				return;
			}

			const abspath = `${getDirPath(activeSet.fileName)}${setName}.toml`;
			out.write_string(getRealAssetFilePath(getAssetFilePath(abspath)));

			activeSet.setProperty("cog2d._firstgid", nextGid);
			nextGid += activeSet.tileCount;
		}
		out.write_object(0, 2, true);

		for (let layerIndex = 0; layerIndex < map.layerCount; layerIndex++) {
			let layer: Layer = map.layerAt(layerIndex);

			tiled.log(`Writing ${layer.name}`);

			if (layer.isTileLayer) {
				writeTileLayerBin(out, layer as TileLayer);
			} else if (layer.isObjectLayer) {
				writeObjectGroupBin(out, layer as ObjectGroup);
			}
		}

		out.file.commit();

		return undefined;
	},
};

tiled.registerMapFormat("c2map", binMap);

// Playtest
let testing: boolean = false;

function tryTest(_action: Action) {
	const program = tiled.project.property(SETTING_EXE_PATH) as string;

	// @ts-ignore
	if (!File.exists(program)) {
		tiled.error(`Could not find executable: "${program}"\n` +
		            `Correct the value of the ${SETTING_EXE_PATH} project setting ` +
		            "(Project > Project Settings) to the path of " +
		            "the game or project's executable in order to test levels/maps.",
		            () => {});
		return;
	}

	if (testing)
		return;

	if (!tiled.activeAsset) {
		tiled.error("Nothing to test", () => {});
		return;
	}

	if (!tiled.activeAsset.isTileMap) {
		tiled.error(`Cannot test asset of type "${tiled.activeAsset.assetType}"`, () => {});
		return;
	}

	if (!tomlMap.write)
		return;

	tiled.log(`Running "${program} ${tiled.activeAsset.fileName}" as requested by test action`);

	//tiled.activeAsset.save();

	testing = true;

	const tempPath = getTempPath();
	tiled.log(`Temporary level path: ${tempPath}`);

	tomlMap.write(tiled.activeAsset as TileMap, tempPath);

	tiled.log(
		"Test Process: " + process.start(program, [tiled.activeAsset.fileName]).toString()
	);
}

function endTest(_action: Action) {
	if (!testing)
		return;

	process.terminate();

	testing = false;
}

function testMap(action: Action) {
	if (testing)
		endTest(action);

	tryTest(action);
}

// Setup Actions

const testAction: Action = tiled.registerAction("cog2d.LevelTest", testMap);

testAction.text = "Test Level/Map";
testAction.checkable = false;
testAction.shortcut = "F5";
testAction.enabled = false;

tiled.extendMenu("Map", [
    { action: "cog2d.LevelTest", before: "MapProperties" },
	{ separator: true, before: "MapProperties" },
]);

tiled.activeAssetChanged.connect((asset: Asset) => {
	// Only activate action for tilemaps
	testAction.enabled = asset && asset.isTileMap;
});

// Setup Settings

const EMPTY_PATH = tiled.filePath("");

if (tiled.project.property(SETTING_EXE_PATH) === undefined)
	tiled.project.setProperty(SETTING_EXE_PATH, EMPTY_PATH);

if (tiled.project.property(SETTING_MAP_COMPRESS) === undefined)
	tiled.project.setProperty(SETTING_MAP_COMPRESS, false);

if (tiled.project.property(SETTING_EMBED_TILESETS) === undefined)
	tiled.project.setProperty(SETTING_EMBED_TILESETS, true);
