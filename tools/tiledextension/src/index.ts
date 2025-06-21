/// <reference types="@mapeditor/tiled-api" />
import TOML from "smol-toml";

// Constants
const TEMP_LEVEL_PATHS = {
	windows: {
		mapExport: "~/AppData/Local/Temp/test.toml"
	},
	unix: {
		mapExport: "~/.cache/test.toml"
	}
};

// @ts-ignore
const PLATFORM_KEY = File.exists("C:/Windows/System32/KERNELBASE.dll") ? "windows" : "unix";

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

// Map Format
let tomlMap: ScriptedMapFormat = {
	name: "cog2d TOML Map",
	extension: "toml",

	write: function (map: TileMap, fileName: string): string | undefined {
		let fileDat: TomlTileMapDocument = {
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

// Playtest
let process: Process | null = null;

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

	if (process != null)
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

	process = new Process();

	const tempPath = TEMP_LEVEL_PATHS[PLATFORM_KEY]["mapExport"].replace('~',
	                                                                     process.getEnv("HOME"));
	tiled.log(`Temporary level path: ${tempPath}`);

	tomlMap.write(tiled.activeAsset as TileMap, tempPath);

	tiled.log(
		"Test Process: " + process.start(program, [tiled.activeAsset.fileName]).toString()
	);
}

function endTest(_action: Action) {
	if (process == null)
		return;

	process.terminate();

	process = null;
}

function testMap(action: Action) {
	if (process != null)
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
