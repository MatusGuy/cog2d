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

// Map Format
var tomlMap: ScriptedMapFormat = {
	name: "Toml",
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

			for (var y = 0; y < layer.height; ++y) {
				for (var x = 0; x < layer.width; ++x) {
					var id = layer.cellAt(x, y).tileId;

					if (id == -1)
						id = 0;

					data.data.push(id);
				}
			}

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

/*
var actionsToUpdate: {
	inverted: boolean;
	action: Action;
}[] = [];*/

function tryTest(action: Action) {
	tiled.log(`Stub: action '${action.text}' has triggered!`);

	if (process != null ||
		!tiled.activeAsset ||
		!tiled.activeAsset.isTileMap ||
		!tomlMap.write)
		return;

	//tiled.activeAsset.save();

	process = new Process();

	const tempPath = TEMP_LEVEL_PATHS[PLATFORM_KEY]["mapExport"].replace('~',
	                                                                     process.getEnv("HOME"));

	tiled.log(tempPath);

	tomlMap.write(tiled.activeAsset as TileMap, tempPath);

	var program = tiled.project.property(SETTING_EXE_PATH) as string;
	tiled.log(
		"Test Process: " + process.start(program, [tiled.activeAsset.fileName]).toString()
	);

	/*
	for (var actin in actionsToUpdate) {
		var act = actionsToUpdate[actin];

		act.action.enabled = !act.inverted;
	}*/
}

function endTest(_action: Action) {
	if (process == null)
		return;

	process.terminate();

	process = null;

	/*
	for (var actin in actionsToUpdate) {
		var act = actionsToUpdate[actin];

		act.action.enabled = act.inverted;
	}*/
}

function testMap(action: Action) {
	if (process != null)
		endTest(action);

	tryTest(action);
}

// Setup Actions

const testAction: Action = tiled.registerAction(
	"LevelTest",
	testMap
);

testAction.text = "Test Map";
testAction.checkable = false;
testAction.shortcut = "F5";
testAction.enabled = false;

tiled.extendMenu("Map", [
	{ action: "LevelTest", before: "MapProperties" },
	{ separator: true, before: "MapProperties" },
]);

function onActiveAssetChanged(asset: Asset) {
	// Only activate action for tilemaps
	testAction.enabled = asset.isTileMap;
}

tiled.activeAssetChanged.connect(onActiveAssetChanged);

// Setup Settings

const EMPTY_PATH = tiled.filePath("");

if (tiled.project.property(SETTING_EXE_PATH) === undefined)
	tiled.project.setProperty(SETTING_EXE_PATH, EMPTY_PATH);
