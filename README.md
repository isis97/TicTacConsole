![Made by Styczynsky Digital Systems][badge sts]

![Platform - Windows][badge support windows]

# TickTackConsole game
Based on [WACCGL Library](link waccgl).

**NOTE: This software supports ONLY Windows**

## Installation/Usage
**TODO Write installation/usage guide**


## Building
Build process involves invoking  `make`, `gcc` (or `g++`) and `git` tools.

Build process is supported on Windows (on Linux it could be possible to easily cross-compile sources, but we recommend **building on Windows**)

To build release do the following steps:
1. Go to the location where build will take place by using `cd <path>` command.
2. `git clone https://github.com/isis97/TicTacConsole`
3. `cd TicTacConsole`
4. `make`
5. `make start` to execute compiled files
6. `make clean` (if you want to rebuild everything)
7. `make commit` (when you want to commit changes to the remote repo)

After doing `make` you can find in the `out/` directory all release files and in the `release/` zipped release-ready version of built software.  

[badge support windows]: https://img.shields.io/badge/platform-windows-blue.svg?style=flat-square&logoWidth=20&logo=data%3Aimage%2Fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAABgAAAAYCAYAAADgdz34AAAABmJLR0QA%2FwD%2FAP%2BgvaeTAAAACXBIWXMAAA7EAAAOxAGVKw4bAAAAB3RJTUUH4AgSEisSipueyAAAAHBJREFUSMdjZKA2WPv%2BPzKXkSxDiuf%2FZ7AKIEopbgsW3v%2FPwCOA4AcLMqK7jhjAQo4mUgATA43BqAWjFlADiCvQ1HjsuXNJIwPD%2BgmMtLMAGyCzqBhNRaMWDAELWBiCBRmJrcDJy2hUaj1Q3wIiLQcAUjQgoD1kMJYAAAAASUVORK5CYII%3D

[badge sts]: https://img.shields.io/badge/-styczynsky_digital_systems-blue.svg?style=flat-square&logoWidth=20&logo=data%3Aimage%2Fpng%3Bbase64%2CiVBORw0KGgoAAAANSUhEUgAAABYAAAAXCAYAAAAP6L%2BeAAAABmJLR0QA%2FwD%2FAP%2BgvaeTAAAACXBIWXMAAA7DAAAOwwHHb6hkAAAAB3RJTUUH4AgSEh0nVTTLngAAAB1pVFh0Q29tbWVudAAAAAAAQ3JlYXRlZCB3aXRoIEdJTVBkLmUHAAAAm0lEQVQ4y2Pc%2Bkz2PwMNAAs2wVMzk4jSbJY%2BD6ccEwONACMsKIh1JSEgbXKeQdr4PO1cPPQMZiGkoC7bkCQD7%2Fx7znDn35AOClK9PEJSBbNYAJz999UGrOLocsM0KHB5EZ%2FXPxiVMDAwMDD8SP3DwJA6kFka5hJCQOBcDwMDAwPDm3%2FbGBj%2BbR8tNrFUTbiAB8tknHI7%2FuTilAMA9aAwA8miDpgAAAAASUVORK5CYII%3D

[link sts]: https://github.com/isis97
[link waccgl]: https://github.com/isis97/waccgl
