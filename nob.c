#define NOB_IMPLEMENTATION
#include "nob.h"

int main(int argc, char **argv) {
	NOB_GO_REBUILD_URSELF(argc, argv);
	const char *CC = "clang";
	const char *OUTPUT = "irc";

	Nob_File_Paths cflags = {0};
	nob_da_append_many(&cflags,
	((const char*[]){
			"-Wall",
			"-Wextra",
			"-pedantic",
			"-std=c99",
			"-Iinclude",
			"-D_POSIX_C_SOURCE=200809L",
			"-D_DEFAULT_SOURCE"
		}),
		7
	);

	Nob_File_Paths entries = {0};
	if (!nob_read_entire_dir("src", &entries)) {
		nob_log(NOB_ERROR, "Could not read src/ directory");
		return 1;
	}

	Nob_File_Paths cfiles = {0};
	for (size_t i = 0; i < entries.count; ++i) {
		Nob_String_View sv = nob_sv_from_cstr(entries.items[i]);
		if (nob_sv_end_with(sv, ".c")) {
			Nob_String_Builder path = {0};
			nob_sb_append_cstr(&path, "src/");
			nob_sb_append_cstr(&path, entries.items[i]);
			nob_da_append(&cfiles, path.items);
		}
	}

	Nob_Cmd cmd = {0};
	nob_cmd_append(&cmd, CC);
	for (size_t i = 0; i < cflags.count; ++i) {
		nob_cmd_append(&cmd, cflags.items[i]);
	}
	for (size_t i = 0; i < cfiles.count; ++i) {
		nob_cmd_append(&cmd, cfiles.items[i]);
	}
	nob_cmd_append(&cmd, "-o", OUTPUT, "-lpthread");

	if (!nob_cmd_run_sync(cmd)) return 1;

	nob_log(NOB_INFO, "Build successful: %s", OUTPUT);
	return 0;
}
