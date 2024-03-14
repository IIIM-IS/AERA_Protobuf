# Installation and usage of Protobuf

## Installation of protobuf and protoc
Install protobuf using vcpkg.
### Install vcpkg:
Follow the instructions on the [official Microsoft website](https://docs.microsoft.com/en-us/cpp/build/install-vcpkg?view=msvc-160&tabs=windows) to install vcpkg. In short:

 1. Clone locally from [https://github.com/Microsoft/vcpkg](https://github.com/Microsoft/vcpkg). It is known to make problems, if there are spaces in the path to vcpkg. So I suggest installing it somewhere like `C:\Users\<username>\vcpkg\`. So for example:  
```
C:\Users\Username> git clone https://github.com/Microsoft/vcpkg
```
 2. Change to `vcpkg` directory:
  `C:\Users\Username> cd vcpkg`
  3. In the vcpkg root directory, run the vcpkg bootstrapper command:
  `C:\Users\Username\vcpkg> bootstrap-vcpkg.bat`

### Install protobuf for AERA (NOT Webots) using vcpkg:

#### Prerequisites:
In order to use protobuf in the debug version of AERA it needs to be compiled with `ITERATOR_DEBUG_LEVEL = 0`. Do do this you need to change the `x86-windows.cmake` triplets of vcpkg. **Make a backup of the original `x86-windows.cmake` before continuing**.
You can find the cmake-files in `C:\Users\<username>\vcpkg\triplets`. There open the `x86-windows.cmake` with an editor of choice (e.g. Notepad++) and add the following two lines to the end of the file:
```
set(VCPKG_C_FLAGS_DEBUG "/D_ITERATOR_DEBUG_LEVEL=0")
set(VCPKG_CXX_FLAGS_DEBUG "/D_ITERATOR_DEBUG_LEVEL=0")
```
The whole file should look like this:
```
set(VCPKG_TARGET_ARCHITECTURE x86)
set(VCPKG_CRT_LINKAGE dynamic)
set(VCPKG_LIBRARY_LINKAGE dynamic)
set(VCPKG_C_FLAGS_DEBUG "/D_ITERATOR_DEBUG_LEVEL=0")
set(VCPKG_CXX_FLAGS_DEBUG "/D_ITERATOR_DEBUG_LEVEL=0")
```
In order to use the installed protobuf version without including further linking the file name should stay the same (x86-windows.cmake).

#### Installation:
Go into the root directory of vcpkg with a terminal and run the following command:
```
C:\Users\<username>\vcpkg> vcpkg install protobuf protobuf:x86-windows
```
In order to use protobuf include files in Visual Studio without further linking you should additionally run
```
C:\Users\<username>\vcpkg> vcpkg integrate install
```
If it returns with an error message it probably is because of lacking administrator rights. For this open a new terminal as an administrator and rerun the previous command.

### Install protobuf for Webots (NOT AERA) using vcpkg:

#### Prerequisites:
To install Protobuf for usage on the Webots side, there are no prerequisits that you need to take care of. However, Webots is using x64, not x86 architecture. Pay close attention to this.

#### Installation:
If the x64 version of protobuf has not been installed automatically when installing the x86 version, all you have to do is follow the same isntructions as above, only changing the install commans to:
```
C:\Users\<username>\vcpkg> vcpkg install protobuf protobuf:x64-windows
```
vcpkg should take care of everything else.

## Compile proto files:
**Not necessary for simply building and using the TCP IODevice, skip if not changing the `tcp_data_message.proto` file.**

If you ever change the proto file you will have to recompile it.
For this use protoc which is included in the previously installed vcpkg protobuf installation.
If protoc is not in your path run the following from the `...\vcpkg\installed\x86-windows\tools\protobuf\` folder (for the compilation, it does not matter whether you use protoc located in the x86-windows or the x64-windows folder):
```
protoc -I=C:\Path\to\AERA_Protobuf --cpp_out=C:\Path\to\AERA_Protobuf  C:\Path\to\AERA_Protobuf\tcp_data_message.proto
```
If protoc is in your path simply `cd` into the `Proto folder` and run:
```
C:\Path\to\AERA_Protobuf> protoc -I=. --cpp_out=. tcp_data_message.proto
```

After recompiling the .proto file you must add the #ifdef ENABLE_PROTOBUF statement to the generated .pp.cc file in the same way as before.

## Add created libraries to Webots
Once everything is installed you need to add the created dlls to your project. For this simply copy-paste `libprotobuf.dll` and `libprotobuf-lite.dll` from `C:\Path\to\vcpkg\installed\x86-windows\bin` to `C:\Path\to\Webots\controllers\folder`. Now everything should be set up for running AERA with a TCP connection.


## Add created libraries to AERA (Should not be necessary)
Vcpkg should take care of copy pasting the generated dlls to AERA.

However, if this is not the case, do the same as above, copy-pasting the aforementioned dlls to the generated AERA Release folder. Additionally, copy-paste `libprotobufd.dll` and `libprotobuf-lited.dll` from `C:\Path\to\vcpkg\installed\x86-windows\debug\bin` to `C:\Path\to\replicode\Debug`. Since different dlls are used for Release/ Debug configuration.

## Enable protobuf in AERA
If you want to use the tcp-connection with AERA you have to enable it by setting the ENABLE_PROTOBUF flag in the AERA project properties. For this right-click on the AERA project (in the solution view on the left), select Properties -> C/C++ -> Preprocessor. There in the first line (Preprocessor Definitions) add a new entry called ENABLE_PROTOBUF.
