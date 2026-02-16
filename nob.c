#define NOB_IMPLEMENTATION
#include "nob.h"
#include <string.h>

#define SRC_FOLDER   "src/"
#define BUILD_FOLDER "build/"
#define OBJ_FOLDER   "build/obj/"
#define LIB_NAME     "build/libgame.a"
#define OUT_EXE      "build/main.exe"

int main(int argc, char **argv) {
    NOB_GO_REBUILD_URSELF(argc, argv);

#ifndef _WIN32
#error "Windows only"
#endif

    if (!nob_mkdir_if_not_exists(BUILD_FOLDER)) return 1;
    if (!nob_mkdir_if_not_exists(OBJ_FOLDER)) return 1;

    Nob_File_Paths sources = {0};
    if (!nob_read_entire_dir(SRC_FOLDER, &sources)) return 1;

    Nob_File_Paths objects = {0};
    int any_object_rebuilt = 0;

    for (size_t i = 0; i < sources.count; i++) {
        const char *file = sources.items[i];

        Nob_String_View file_sv = { strlen(file), file };
        if (!nob_sv_end_with(file_sv, ".c")) continue;

        const char *src = nob_temp_sprintf("%s%s", SRC_FOLDER, file);
        const char *obj = nob_temp_sprintf("%s%s.o", OBJ_FOLDER, file);

        nob_da_append(&objects, obj);

        if (!nob_needs_rebuild1(obj, src)) continue;

        any_object_rebuilt = 1;

        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd,
            "gcc",
            "-Iinclude",
            "-c", src,
            "-O3",
            "-Wall",
            "-Wextra",
            "-pedantic",
            "-o", obj);

        if (!nob_cmd_run(&cmd)) return 1;
    }

    if (any_object_rebuilt ||
        nob_needs_rebuild(LIB_NAME,
            (const char **)objects.items, objects.count)) {

        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd, "ar", "rcs", LIB_NAME);

        for (size_t i = 0; i < objects.count; i++) {
            nob_cmd_append(&cmd, objects.items[i]);
        }

        if (!nob_cmd_run(&cmd)) return 1;
    }

    const char *link_inputs[2] = {
        LIB_NAME,
        "include/libraylib.a"
    };

    if (nob_needs_rebuild(OUT_EXE, link_inputs, 2)) {
        Nob_Cmd cmd = {0};
        nob_cmd_append(&cmd,
            "gcc",
            "-o", OUT_EXE,
            LIB_NAME,
            "-Linclude",
            "-lraylib",
            "-lgdi32",
            "-lwinmm",
            "-lopengl32");

        if (!nob_cmd_run(&cmd)) return 1;
    }

    return 0;
}

