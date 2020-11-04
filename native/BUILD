package(default_visibility = ["//visibility:public"])

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
    static_libraries = [],
    shared_libraries = ["librtlsdr.so"],
    binaries = glob(["rtl_*"])
)

cc_library(
    name = "rtlsdrj-lib",
    srcs = ["rtlsdrj.c"],
    hdrs = ["rtlsdrj.h"],
    deps = ["//tools/jni:copy_jni_hdr_lib", ":rtlsdr"],
    alwayslink = True,
)

cc_binary(
    name = "librtlsdrj.so",
    linkshared = True,
    linkstatic = False,
    deps = [":rtlsdrj-lib", ":rtlsdr"],
)
