# jni-rtlsdrj

Java JNI wrapper for the Osmocom rtl-sdr library.

With Bazel!

## Run test
```bash
bazel test --cache_test_results=no --verbose_failures --test_output=streamed //src/de/rtlsdr/tests:TestSettings
```


### Build jar

```bash
SPX:~/rtlsdrj $ bazel build //src/de/rtlsdr:rtlsdr
INFO: Analyzed target //src/de/rtlsdr:rtlsdr (27 packages loaded, 529 targets configured).
INFO: Found 1 target...
INFO: From CcCmakeMakeRule native/rtlsdr/include:
Target //src/de/rtlsdr:rtlsdr up-to-date:
  bazel-bin/src/de/rtlsdr/librtlsdr.jar

INFO: Elapsed time: 10.187s, Critical Path: 9.78s
INFO: 17 processes: 6 internal, 10 linux-sandbox, 1 worker.
INFO: Build completed successfully, 17 total actions
```
