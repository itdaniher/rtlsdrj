# jni-rtlsdrj

Java JNI wrapper for the Osmocom rtl-sdr library.

## Eclipse IDE run configuration for tests

Set the VM arguments:

    -Djava.library.path=${workspace_loc}/rtlsdrj/lib
    -Xcheck:jni

Set the PATH environment:

    PATH=${workspace_loc}/rtlsdrj/lib;%PATH%
  
