![top_banner](https://raw.githubusercontent.com/israfiel-a/israfiel-a/main/azalea-banner.jpg)

----------

### Azalealang
![lines of code badge](https://img.shields.io/endpoint?url=https://ghloc.vercel.app/api/israfiel-a/azalea/badge?filter=.az$,.c$,.h$,.txt$&style=flat&color=blue&label=Lines%20of%20Code)

This is a small, [C-like](https://en.wikipedia.org/wiki/List_of_C-family_programming_languages), [low-level language](https://en.wikipedia.org/wiki/Low-level_programming_language) built specifically for the development of the [Cobalt operating system](https://github.com/israfiel-a/cobalt). This language is not and probably never will be fit for general usage, it's tailored exactly to my needs and my programming style. The compiler is also mostly implemented in [inline assembly](https://en.wikipedia.org/wiki/Inline_assembler), entirely without the [C standard library](https://en.wikipedia.org/wiki/C_standard_library), because why not.

Azalea is not fast, and I figure it's only pretty to me. It's more or less a toy language. But I still love it, and I would greatly appreciate if you tried it out, or at least took a look at it. Let me know what you think!

> [!WARNING]
> This language, and its compiler, are under **very** early development. Changes, especially breaking changes, should be expected, and should be expected to be frequent.

----------

#### Compatibility
Azalea will only ever have targets for the [x86_64 architecture](https://en.wikipedia.org/wiki/X86-64)&mdash;that's its use case. It compiles directly to x86_64 assemblies of different varieties (NASM, FASM, etc.). A list of the targets that are (planned) is below.

- [ ] [Linux](https://en.wikipedia.org/wiki/Linux):
  - [ ] [NASM](https://en.wikipedia.org/wiki/Netwide_Assembler): **In progress.**
  - [ ] [FASM](https://en.wikipedia.org/wiki/FASM): **On the roadmap.**
- [ ] [Cobalt](https://github.com/israfiel-a/cobalt): **On the roadmap.**
- [ ] [BSD](https://en.wikipedia.org/wiki/Berkeley_Software_Distribution): **On the roadmap.**

----------

#### Compilation
You can compile Azalea via the [CMake](https://en.wikipedia.org/wiki/CMake) build system. You can build the Azalea compiler interface as either a [shared](https://en.wikipedia.org/wiki/Shared_library) or [static](https://en.wikipedia.org/wiki/Static_library) library to link to your own compiler implementations (if, for some reason, you wish to create a compiler for this monstrous language).

```bash
BUILD_TYPE=Release
cmake --B build -DCMAKE_BUILD_TYPE=$BUILD_TYPE        # Build type must be set, or assumed debug.
cmake --build build --config $BUILD_TYPE --parallel 9 # Parallelization is not required, but faster.

# Output:
# build/libAzalea.a <- Compiler interface
# build/AzaleaC     <- Compiler client
```

----------

#### Versioning
Azalea is versioned in a `major.minor.patch.tweak` system. Each "tweak" is a commit; something was changed. This rolls over when a "patch" is hit, being a fix to a certain bug or feature. These roll over ever "minor" version, which occur every time a new large feature is implemented. This only rolls over every "major" version, which are milestones decided by maintainers.

> [!WARNING]
> This versioning system will not be followed properly until the compiler is in a good state. Until then, all is listed as "since v1.0.0.0".

----------

#### Roadmap
- [x] Loader
- [x] Tokenizer
- [ ] Generator
- [ ] Analyzer
- [ ] Interpreter
- [ ] Compiler

----------

![top_banner](https://raw.githubusercontent.com/israfiel-a/israfiel-a/main/azalea-banner.jpg)
