# retro-rpg

This was originally the source code for my [C++ RPG tutorial](http://www.penguinprogrammer.co.uk/rpg-tutorial), but with the (on-going) addition of graphics, has grown into something not particularly suited for a tutorial. Any updates I make on this project that are applicable to the tutorial will be patched up(down?)-stream to the repo, but most changes will not be.

The rationale behind cloning the project like this was two-fold. Firstly, Github doesn't allow you to fork your own projects. That would have been ideal, and would have allowed the two to be nicely linked, with the tutorial repo acting as the main fork, and this an offshoot that occasionally donated patches back to the 'official' version. Secondly, Github doesn't report statistics about work on branches other than `master`. Since this fork was never going to be merged into `master`, the hundreds of commits would never be recorded. This bugs me somewhat, hence this new repository.

This project uses JsonBox for JSON manipulation just like the tutorial version, namely the file `src/JsonBox.h` and the files in the folder `src/JsonBox/`, which the license file `JsonBox-license.txt` applies exclusively to. All other code is under the MIT license as in the repo, as are the assets. Just give credit, or even better, just contribute instead.
