# Looks for .cpp or .hpp files in incorrect directories and corrects them

file(GLOB_RECURSE MISHEADERS RELATIVE ${SOURCE_DIR}/include/cog2d ${SOURCE_DIR}/include/cog2d/*.cpp)
foreach(_currfile ${MISHEADERS})
	file(RENAME ${SOURCE_DIR}/include/cog2d/${_currfile} ${SOURCE_DIR}/src/${_currfile})
endforeach()

file(GLOB_RECURSE MISSOURCES RELATIVE ${SOURCE_DIR}/src ${SOURCE_DIR}/src/*.hpp)
foreach(_currfile ${MISSOURCES})
	file(RENAME ${SOURCE_DIR}/include/cog2d/${_currfile} ${SOURCE_DIR}/src/${_currfile})
endforeach()
