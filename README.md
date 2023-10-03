# CS474 - Image Processing and Interpretation
A WebAssembly/Native Windows application for testing and visualizing Image Processing and Interpretation theory.

Stephen Foster: [@Stehfyn](https://github.com/Stehfyn)<br>
Joey Paschke [@DuckieOnQuacks](https://github.com/DuckieOnQuacks)

## Live Version
 A live version can be found at: [stehfyn.github.io/cs474/](stehfyn.github.io/cs474/)

## How to build (GNU Make)
### Build Steps

First, you can clone the repository and its submodule:
```bash
git clone --recursive https://github.com/Stehfyn/cs474
```
Or do the following if you cloned the repo non-recursively already:
```bash
cd cs474
git submodule update --init --recursive
```
Then bootstrap `emsdk` with:
```
cd cs474
.\setup.bat --bypass
```

Next, you may compile with:
```bash
cd cs474
.\scripts\CI\make_dirs.bat && make Release
```

Lastly,
```
.\scripts\CI\copy_release.bat && .\scripts\Build\copy_index.bat Release
```

### Locally Hosting the Build
To do so, first `cd` into `Release` and then invoke the `Python` http.server:
```bash
cd Release
python3 -m http.server 8080
```
Then launch any browser and navigate to `localhost:8080/index.html`, and voila!

## How to build (Visual Studio)
Build support is provided for `Visual Studio 2022`. The build system dependencies are as follows:
- `Visual Studio 2022`
- [`VSExtForEmscripten`](https://github.com/nokotan/VSExtForEmscripten/) v0.8.0 or higher.
  - The specific extension is `Emscripten.Build.Definition.vsix`, and can be found [here](https://github.com/nokotan/VSExtForEmscripten/releases/download/v0.8.0/Emscripten.Build.Definition.vsix).

### Build Steps
*Note* - It is recommended to first install [`Emscripten.Build.Definition.vsix`](https://github.com/nokotan/VSExtForEmscripten/releases/download/v0.8.0/Emscripten.Build.Definition.vsix) manually, as opposed to using `setup.bat` which may take awhile.

First, you can clone the repository and its submodule:
```bash
git clone --recursive https://github.com/Stehfyn/cs474
```
Or do the following if you cloned the repo non-recursively already:
```bash
cd cs474
git submodule update --init --recursive
```
Then, if you have already installed `Emscripten.Build.Definition.vsix`, just
```
cd cs474
.\setup.bat --bypass
```
Or, if you wish to to have it automatically installed:
```
cd cs474
.\setup.bat
```
Finally, you may open `cs474.sln` with `Visual Studio 2022` and build for platform `Emscripten`.
Or, with the `Developer Command Prompt for VS`, just do:
```bash
cd cs474
msbuild /m /p:Configuration=Release /p:Platform=Emscripten .
```

### Locally Hosting the Build
The [`VSExtForEmscripten`](https://github.com/nokotan/VSExtForEmscripten/) provides support via the Visual Studio Debugger UI for testing builds on `localhost`, but this is not recommended as it is currently far from stable. A suitable alternative is just using `Python`'s http package:

To do so, first `cd` into either a Debug or Release configuration of a `cs474-client` build directory with a successful build (The build directory pattern is `bin\cs474-client\Emscripten\{Configuration}\`), then invoke the `Python` http.server:
```bash
cd bin\cs474-client\Emscripten\Release\
python3 -m http.server 8080
```
Then launch any browser and navigate to `localhost:8080/index.html`, and voila!
