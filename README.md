# jni-rtlsdrj

Java JNI wrapper for the Osmocom rtl-sdr library.

## Eclipse IDE run configuration for tests

Set the VM arguments:

    -Djava.library.path=${project_loc}/lib
    -Xcheck:jni

Set the PATH environment:

    PATH=${project_loc}/lib;%PATH%
  
