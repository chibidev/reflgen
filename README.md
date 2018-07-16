# reflgen

![Build Status](https://chibidev.visualstudio.com/b6382831-a06f-40c7-9641-6747bdf14803/_apis/build/status/1)
A command-line tool to generate reflection bindings to your classes based on foonathan's excellent [cppast](https://github.com/foonathan/cppast) library

## Usage

```bash
$ reflgen file.cpp
```

It expects to find a clang compilation database ``compile_commands.json`` in the current directory to look up file.cpp's compiler switches. You can override the directory by specifying the ``-d directory`` parameter.
