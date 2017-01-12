set(LIBRARY_NAME R3BNeulandSimulation)
set(LINKDEF NeulandSimulationLinkDef.h)


set(SRCS
	R3BNeuland.cxx
	R3BNeulandMCMon.cxx
)
change_file_extension(*.cxx *.h HEADERS "${SRCS}")


set(DEPENDENCIES
	R3Bbase R3BData R3BNeuland
)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wformat-security -pthread -march=native -O3")


set(SYSTEM_INCLUDE_DIRECTORIES
	${SYSTEM_INCLUDE_DIRECTORIES}
	${BASE_INCLUDE_DIRECTORIES}
)
include_directories(SYSTEM ${SYSTEM_INCLUDE_DIRECTORIES})


set(INCLUDE_DIRECTORIES
	${R3BROOT_SOURCE_DIR}/neuland/
	${R3BROOT_SOURCE_DIR}/neuland/shared
	${R3BROOT_SOURCE_DIR}/neuland/simulation
	${R3BROOT_SOURCE_DIR}/r3bdata
	${R3BROOT_SOURCE_DIR}/r3bdata/neulandData
	${R3BROOT_SOURCE_DIR}/r3bbase
)
include_directories(${INCLUDE_DIRECTORIES})


set(LINK_DIRECTORIES
	${ROOT_LIBRARY_DIR}
	${Boost_LIBRARY_DIRS}
	${FAIRROOT_LIBRARY_DIR}
)
link_directories(${LINK_DIRECTORIES})


generate_library()
