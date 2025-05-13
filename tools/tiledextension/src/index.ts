/// <reference types="@mapeditor/tiled-api" />
import TOML from "smol-toml";

const paths = {
	windows: {
		mapExport: "~/AppData/Local/Temp/test.toml"
	},
	unix: {
		mapExport: "~/.cache/test.toml"
	}
};

// @ts-ignore WRONG FILE CLASS DUMB STUPID THING I WANNA SWEAR AT
const platformKey = File.exists("C:/Windows/System32/KERNELBASE.dll") ? "windows" : "unix";

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

	if (process != null || !tiled.activeAsset || !tiled.activeAsset.isTileMap || !tomlMap.write) return;

	//tiled.activeAsset.save();

	var tempPath = paths[platformKey]["mapExport"];

	process = new Process();

	tempPath = tempPath.replace('~', process.getEnv("HOME"));

	tiled.log(tempPath);

	tomlMap.write(tiled.activeAsset as TileMap, tempPath);

	tiled.log(
		"Test Process: " + process.start("/usr/bin/konsole", []).toString()
	);

	/*
	for (var actin in actionsToUpdate) {
		var act = actionsToUpdate[actin];

		act.action.enabled = !act.inverted;
	}*/
}

function endTest(action: Action) {
	if (process == null) return;

	process.terminate();

	process = null;

	/*
	for (var actin in actionsToUpdate) {
		var act = actionsToUpdate[actin];

		act.action.enabled = act.inverted;
	}*/
}

function reloadTest(action: Action) {
	if (process != null) endTest(action);

	tryTest(action);
}

/*
const testAction: Action = tiled.registerAction("TestLevel", tryTest);

testAction.text = "Test level";
testAction.checkable = false;
testAction.shortcut = "F9";

const endAction: Action = tiled.registerAction("EndLevelTest", endTest);

endAction.text = "End Level Test";
endAction.checkable = false;
endAction.shortcut = "Ctrl+F9";
endAction.enabled = false;
*/

const reloadAction: Action = tiled.registerAction(
	"ReloadLevelTest",
	reloadTest
);

reloadAction.text = "Test level";
reloadAction.checkable = false;
reloadAction.shortcut = "F9";
reloadAction.enabled = false;

/*
actionsToUpdate = [
	{inverted: true, action: testAction},
	{inverted: false, action: endAction},
	{inverted: false, action: reloadAction}
];
*/

tiled.extendMenu("Map", [
	/*	{ action: "TestLevel", before: "MapProperties" },
	{ action: "EndLevelTest", before: "MapProperties" },*/
	{ action: "ReloadLevelTest", before: "MapProperties" },
	{ separator: true, before: "MapProperties" },
]);

// Reload enable swapping
function onActiveAssetChangedCog2D(asset: Asset) {
	reloadAction.enabled = asset.isTileMap;
}

tiled.activeAssetChanged.connect(onActiveAssetChangedCog2D);