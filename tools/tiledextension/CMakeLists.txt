project(COG2D_TILED_EXTENSION)

find_program(PNPM_PATH pnpm)

file(GLOB COG2D_TILED_EXTENSION_SRC CONFIGURE_DEPENDS src/*.ts)
add_custom_target(cog2d_tiled_extension
	COMMAND ${PNPM_PATH} build
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
	BYPRODUCTS build/index.js dist/index.js
	SOURCES ${COG2D_TILED_EXTENSION_SRC} webpack.config.js package.json tsconfig.json
)

add_custom_target(cog2d_tiled_extension_test
	COMMAND ${PNPM_PATH} extinstall
	WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}"
	DEPENDS cog2d_tiled_extension
	COMMENT "Installing Tiled extension. After installation, you can open Tiled. \
If it's already opened, Tiled should reload the extension automatically."
)
set_target_properties(cog2d_tiled_extension_test PROPERTIES FOLDER "qtc_runnable")
