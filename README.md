# What's this?
Several utilities for markdown. Currently it's all about markdown table formatting.

# How to use in terminal?
For example you have a markdown table that is written a bit ugly in file t.txt:
```
|  |1,2|2,3|4,5|
|-|-|-|-|
|sum|3|5|9|
|multiplication|2|6|20|
```
You may use `cat t.txt | md_tf` to get nice table:
```
|                | 1,2 | 2,3 | 4,5 |
| -------------- | --- | --- | --- |
| sum            | 3   | 5   | 9   |
| multiplication | 2   | 6   | 20  |
```
Or even transposed one with `cat t.txt | md_tt`:
```
|     | sum | multiplication |
| --- | --- | -------------- |
| 1,2 | 3   | 2              |
| 2,3 | 5   | 6              |
| 4,5 | 9   | 20             |
```

# How to use in your editor?
Just select a table and press **Ctrl+Shift+F8** to format a table and get a nice table like in example above, or press **Ctrl+Shift+F7** to get transposed formatted table. The result table replaces original in both cases, you may undo the changes in your ordinal way.

Requires `xclip` and `xdotool` to installed on your computer.

Custom global hotkeys are used for that, you may change hotkeys using standard utilities.

# How to install?
```
git clone https://github.com/alyapunov/MarkDown
cd ./MarkDown
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=RelWithDebInfo
make -j
make test
./install.sh
```
Command line utilities are installed to `~/bin` using symlink. Global hotkeys also use binaries and scripts that were built, so you should not remove 'em.

# How to uninstall?
```
./install.sh -u
```
