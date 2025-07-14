----------

### Azalealang
This is a small, C-like, low-level language built specifically for the development of the Cobalt operating system. This language is not and probably never will be fit for general usage, it's tailored exactly to my needs and my programming style. The compiler is also mostly implemented in inline assembly, because why not.

> [!WARNING]
> This language, and its compiler, are under **very** early development. Changes should be expected, and should be expected to be frequent.

----------

#### Compatibility
Azalea will only ever have targets for x86_64, that's its usecase. It compiles directly to x86_64 assemblies of different varieties (NASM, FASM, etc.). A list of the targets that are (planned) is below.

- Linux: **Supported.**
- Cobalt: **In progress.**
- Windows: **On the roadmap.**
- BSD: **On the roadmap.**

----------

#### Compilation
You can compile Azalea via the CMake build system. You can build the Azalea compiler interface as either a shared or static library to link to your own compiler implementations (if, for some reaosn, you wish to create a compiler for this monstrous language).

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
> Until `v2.0.0.0`, this versioning system will be inconsistently followed. Breaking changes should be expected often.

----------
