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

type TomlTileSetDocument = TomlDocument & {
	data: TomlTileSetData
};

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
	data: {
		tilesets: TomlEmbeddedTileSet[];
		layers: TomlTileLayer[];
	}
};

let process = new Process();

function getTempPath(): string {
	switch (PLATFORM) {
	case "windows": return process.getEnv("APPDATA") + "/Temp/";
	case "unix": return "/tmp/";
	}
}

function getTilesetData(tileset: Tileset): TomlTileSetData {
	let imgpath = tileset.imageFileName;
	// TODO: regex to only get filename and not full path

	return {
		tilewidth: tileset.tileWidth,
		tileheight: tileset.tileHeight,
		columns: tileset.columnCount,
		imageheight: tileset.imageHeight,
		image: imgpath,
	};
};

let tomlSet: ScriptedTilesetFormat = {
	name: "cog2d TOML Tileset",
	extension: "toml",

	write: function (tileset: Tileset, fileName: string): string | undefined {
		let doc: TomlTileSetDocument = {
			type: "tileset",
			version: 1,
			data: getTilesetData(tileset),
		};

		let file = new TextFile(fileName, TextFile.WriteOnly);
		file.write(TOML.stringify(doc));
		file.commit();

		return undefined;
	}
};
tiled.registerTilesetFormat("c2tomlset", tomlSet);

function writeTiles(layer: TileLayer, data: number[]) {
	for (let y = 0; y < layer.height; ++y) {
		for (let x = 0; x < layer.width; ++x) {
			let id = layer.cellAt(x, y).tileId;

			if (id == -1)
				id = 0;

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
			let id = layer.cellAt(x, y).tileId;

			if (id == -1)
				id = 0;

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

// Map Formats
let tomlMap: ScriptedMapFormat = {
	name: "cog2d TOML Map",
	extension: "toml",

	write: function (map: TileMap, fileName: string): string | undefined {
		let fileDat: TomlTileMapDocument = {
			type: "tilemap",
			version: 1,
			data: {
				tilesets: [],
				layers: [],
			}
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
				// TODO: We only need the file name, not the full path.
				setData.source = activeSet.fileName
			}

			nextGid += activeSet.tileCount;

			fileDat.data.tilesets.push(setData)

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

			fileDat.data.layers.push(data);

			tiled.log(`Processed tilemap layer '${layerIndex}'`)
		}

		let file = new TextFile(fileName, TextFile.WriteOnly);
		file.write(TOML.stringify(fileDat));
		file.commit();

		return undefined;
	},
};

tiled.registerMapFormat("c2tomlmap", tomlMap);

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

	write_object(data: number, size: ObjectSize, unsigned: boolean) {
		this.write_array([data], size, unsigned);
	}

	write_string(data: string) {
		const arr = Array.from(data + "\0").map(char => char.charCodeAt(0));
		this.write_array(arr, 1, true);
	}
}

let binMap: ScriptedMapFormat = {
	name: "cog2d Binary Map",
	extension: "dat",

	write: function (map: TileMap, fileName: string): string | undefined {
		let buf = new ArrayBuffer(2);
		let out = new BinaryFileStream(new BinaryFile(fileName, BinaryFile.WriteOnly));

		// Header
		//                  C     2     M
		out.write_array([0x43, 0x32, 0x4D], 1, true);

		// Version
		out.write_object(1, 2, true);

		let nextGid = 1;
		let usedTilesets = map.usedTilesets();

		for (let setIndex = 0; setIndex < usedTilesets.length; setIndex++) {
			let activeSet = usedTilesets[setIndex];
			out.write_object(nextGid, 2, true);
			out.write_string(activeSet.fileName);
			nextGid += activeSet.tileCount;
		}
		out.write_object(0, 1, true);

		for (let layerIndex = 0; layerIndex < map.layerCount; layerIndex++) {
			let layer: Layer = map.layerAt(layerIndex);

			if (!layer.isTileLayer) {
				continue;
			}

			let tilelayer: TileLayer = layer as TileLayer;

			out.write_object(tilelayer.isTileLayer ? 0 : 1, 1, true);
			out.write_string(tilelayer.name)
			out.write_object(tilelayer.width, 4, true);
			out.write_object(tilelayer.height, 4, true);

			let tiles: number[] = [];
			let compress = tiled.project.property(SETTING_MAP_COMPRESS) as boolean;
			if (compress)
				writeTilesCompressed(tilelayer, tiles);
			else
				writeTiles(tilelayer, tiles);

			out.write_array(tiles, 2, !compress);
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
	testAction.enabled = asset.isTileMap;
});

// Setup Settings

const EMPTY_PATH = tiled.filePath("");

if (tiled.project.property(SETTING_EXE_PATH) === undefined)
	tiled.project.setProperty(SETTING_EXE_PATH, EMPTY_PATH);

if (tiled.project.property(SETTING_MAP_COMPRESS) === undefined)
	tiled.project.setProperty(SETTING_MAP_COMPRESS, false);

if (tiled.project.property(SETTING_EMBED_TILESETS) === undefined)
	tiled.project.setProperty(SETTING_EMBED_TILESETS, true);
