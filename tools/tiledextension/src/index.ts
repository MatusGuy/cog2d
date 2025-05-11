/// <reference types="@mapeditor/tiled-api" />
import TOML from "smol-toml";

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

		for (var seti in map.tilesets) {
			var activeSet = map.tilesets[seti];

			fileDat.tilesets.push({
				firstgid: nextGid,
				tilewidth: activeSet.tileWidth,
				tileheight: activeSet.tileHeight,
				columns: activeSet.columnCount,
				imageheight: activeSet.imageHeight,
				image: activeSet.imageFileName,
			});

			nextGid += activeSet.tileCount;

			tiled.log(`Processed tileset '${seti}'`)
		}

		for (var layeri = 0; layeri < map.layerCount; layeri++) {
			var fakelayer: Layer = map.layerAt(layeri);

			if (!fakelayer.isTileLayer) {
				tiled.log(`Processed tileset '${layeri}' with early return`)
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
					data.data.push(layer.cellAt(x, y).tileId);
				}
			}

			fileDat.layers.push(data);

			tiled.log(`Processed tileset '${layeri}'`)
		}

		tiled.log(TOML.stringify(fileDat));

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

	if (process != null) return;

	process = new Process();

	tiled.log(
		"Test Process: " + process.start("/usr/bin/konsole", []).toString()
	);

	tiled.log("Test Process Exit Code Check: " + process.exitCode);

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
reloadAction.enabled = true;

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
