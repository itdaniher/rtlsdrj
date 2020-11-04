
package(default_visibility = ["//visibility:public"])

java_library(
    name = "tests",
    srcs = glob(["*.java"]),
    data = ["//native:librtlsdrj.so"],
    deps = ["//src/de/rtlsdr/utils:utils", "//src/de/rtlsdr:rtlsdr"]
)

java_test(
    name = "TestSettings",
    runtime_deps = [":tests"],
    main_class = "de.rtlsdr.tests.TestSettings"
)
