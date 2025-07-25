![top_banner](https://raw.githubusercontent.com/israfiel-a/israfiel-a/main/azalea-banner.jpg)

----------

### Contributing
This is an open-source project. I enjoy having contributors, having a community. But this contributing must not be done uselessly, and it may not be done chaotically. The only person with permission to contribute directly to the master branch are maintainers, and only for extremely rare hotpatch cases. Except for said rare cases, contribution is to be done through pull requests. Once your pull request has been approved by a maintainer in its entirety it will be merged into the master branch.

----------

#### Approval
Approval comes in the way of a written acknowledgement of the pull request's validity and quality. This can *only* be issued by a maintainer, though having others review your code will certainly boost you in the merge priority queue. Complaints will do the opposite. If you don't like my process, fork (;3) off.

----------

#### Quality
All contributed code must have some manner of quality. This comes in the way of two major categories; readiness and cleanliness. Both are extremely simple, and can be boiled down to "does it build" and "can I understand why". 

- Firstly, yes, your code must build. It must handle all sensible edge cases, it must be stable, it must not throw any warnings. 
- Secondly, it should be performant. While the codebase at large is not particularly mission-critical, I still pride myself on its relative speed. This speed must be upheld; if something is slow, there's something you need to fix.
- Thirdly, it should be formatted correctly. The project ships with a `.clang-format` file, use it.
- Fourthly, you should name things well. A good name can seriously make the difference between understandable code and obfuscated code. I will not accept the latter.
- Fiftly, be sparing with nesting. Try to reduce it where you can, and pull it out into another function where you can't. It just helps readability immensely.
- Finally, please document your shit. If you have to comment every line to understand what it means, your code sucks. But please include the standard Doxygen-format documentation with any functions/macros/globals you declare.

----------

![bottom_banner](https://raw.githubusercontent.com/israfiel-a/israfiel-a/main/azalea-banner.jpg)
