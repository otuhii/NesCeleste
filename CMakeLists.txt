cmake_minimum_required(VERSION 3.20)

project(nesGame NONE)

find_program(CA65 ca65 REQUIRED)
find_program(LD65 ld65 REQUIRED)


set(OBJECT_DIR
    ${CMAKE_BINARY_DIR}/obj
)

set(OUTPUT_DIR
    ${CMAKE_BINARY_DIR}/output
)

file(MAKE_DIRECTORY
    ${OBJECT_DIR}
    ${OUTPUT_DIR}
)

set(SOURCE
    ${CMAKE_SOURCE_DIR}/src/main.s
)

set(MEMORY_LAYOUT
    ${CMAKE_SOURCE_DIR}/src/config/nes.cfg
)

set(OBJECT
    ${OBJECT_DIR}/main.o
)

set(ROM
    ${OUTPUT_DIR}/game.nes
)

set(MAP
    ${OUTPUT_DIR}/game.map.txt
)

set(LABELS
    ${OUTPUT_DIR}/game.labels.txt
)

set(DEBUG
    ${OUTPUT_DIR}/game.dbg
)




add_custom_command(
    OUTPUT ${OBJECT}

    COMMAND
        ${CA65}
        ${SOURCE}
        -g
        -o ${OBJECT}

    DEPENDS
        ${SOURCE}

    COMMENT "Assembling main.s"
)

add_custom_command(
    OUTPUT ${ROM}

    BYPRODUCTS
        ${MAP}
        ${LABELS}
        ${DEBUG}

    COMMAND
        ${LD65}
        ${OBJECT}
        -C ${MEMORY_LAYOUT}
        -o ${ROM}
        -m ${MAP}
        -Ln ${LABELS}
        --dbgfile ${DEBUG}

    DEPENDS
        ${OBJECT}
        ${MEMORY_LAYOUT}

    COMMENT "Linking game.nes"
)

add_custom_target(
    game ALL
    DEPENDS ${ROM}
)
