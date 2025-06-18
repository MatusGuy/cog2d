/// <reference types="@mapeditor/tiled-api" />
import TOML from "smol-toml";

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

// Settings
const SETTING_EXE_PATH = "cog2d.executable"
const SETTING_MAP_COMPRESS = "cog2d.mapCompress"

type TomlTileset = Object & {
	firstgid: number;
	tilewidth: number;
	tileheight: number;
	columns: number;
	imageheight: number;
	image: string;
};

type TomlLayer = Object & {
	name: string;
	type: "tilelayer";
	width: number;
	height: number;
	
	data: number[];
};

type TomlData = Object & {
	tilesets: TomlTileset[];
	layers: TomlLayer[];
};

function writeTiles(layer: TileLayer, data: number[]) {
	for (var y = 0; y < layer.height; ++y) {
		for (var x = 0; x < layer.width; ++x) {
			var id = layer.cellAt(x, y).tileId;

			if (id == -1)
				id = 0;

			data.push(id);
		}
	}
}

function writeTilesCompressed(layer: TileLayer, data: number[]) {
	// Don't trust this function
	var repeatid = 0;
	var repeats = 1;

	for (var y = 0; y < layer.height; ++y) {
		for (var x = 0; x < layer.width; ++x) {
			var id = layer.cellAt(x, y).tileId;

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
}

// Map Format
var tomlMap: ScriptedMapFormat = {
	name: "cog2d TOML Map",
	extension: "toml",

	write: function (map: TileMap, fileName: string): string | undefined {
		var fileDat: TomlData = {
			tilesets: [],
			layers: [],
		};

		var nextGid = 1;

		var usedTilesets = map.usedTilesets();

		for (var setIndex = 0; setIndex < usedTilesets.length; setIndex++) {
			var activeSet = usedTilesets[setIndex];

			fileDat.tilesets.push({
				firstgid: nextGid,
				tilewidth: activeSet.tileWidth,
				tileheight: activeSet.tileHeight,
				columns: activeSet.columnCount,
				imageheight: activeSet.imageHeight,
				image: activeSet.imageFileName,
			});

			nextGid += activeSet.tileCount;

			tiled.log(`Processed tileset '${setIndex}'`)
		}

		for (var layerIndex = 0; layerIndex < map.layerCount; layerIndex++) {
			var fakelayer: Layer = map.layerAt(layerIndex);

			if (!fakelayer.isTileLayer) {
				tiled.log(`Processed tilemap layer '${layerIndex}' with early return`)
				continue;
			}

			var layer: TileLayer = fakelayer as TileLayer;

			var data: TomlLayer = {
				name: layer.name,
				type: "tilelayer",
				width: layer.width,
				height: layer.height,
				data: []
			};

			if (tiled.project.property(SETTING_MAP_COMPRESS))
				writeTilesCompressed(layer, data.data);
			else
				writeTiles(layer, data.data);

			fileDat.layers.push(data);

			tiled.log(`Processed tilemap layer '${layerIndex}'`)
		}

		var file = new TextFile(fileName, TextFile.WriteOnly);
		file.write(TOML.stringify(fileDat));
		file.commit();

		return undefined;
	},
};

tiled.registerMapFormat("toml", tomlMap);

// Playtest
var process: Process | null = null;

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

const testAction: Action = tiled.registerAction("LevelTest", testMap);

testAction.text = "Test Level/Map";
testAction.checkable = false;
testAction.shortcut = "F5";
testAction.enabled = false;

tiled.extendMenu("Map", [
	{ action: "LevelTest", before: "MapProperties" },
	{ separator: true, before: "MapProperties" },
]);

tiled.activeAssetChanged.connect((asset: Asset) => {
	// Only activate action for tilemaps
	testAction.enabled = asset.isTileMap;
});

// Setup Settings

const EMPTY_PATH = tiled.filePath("");

if (tiled.project.property(SETTING_EXE_PATH) == undefined)
	tiled.project.setProperty(SETTING_EXE_PATH, EMPTY_PATH);

if (tiled.project.property(SETTING_MAP_COMPRESS) == undefined)
	tiled.project.setProperty(SETTING_MAP_COMPRESS, false);
