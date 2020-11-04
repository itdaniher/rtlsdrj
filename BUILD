load("@rules_foreign_cc//tools/build_defs:cmake.bzl", "cmake_external")

cmake_external(
    name = "rtlsdr",
    lib_name = "rtlsdr",
    env_vars = {"CFLAGS": "-Dredacted='\\\"redacted\\\"'"},
    make_commands = ["VERBOSE=1 make", "make install"],
    cache_entries = {
        "DETACH_KERNEL_DRIVER": "ON",
        },
    lib_source = "@librtlsdr//:all",
    static_libraries = ["librtlsdr.a"],
    shared_libraries = ["librtlsdr.so"],
    binaries = glob(["rtl_*"])
)
