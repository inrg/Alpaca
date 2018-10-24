## Alpaca 2.0.3 [1.13d]
##### By Jonathan Vasquez (fearedbliss)
##### Released on Wednesday, October 24, 2018

## Synopsis

Alpaca is an Infinite Stash Mod for Diablo II: Lord of Destruction
that is based off Yohann Nicolas' PlugY 11.02. The main goal of Alpaca
is to be a clean and maintainable core with no extra features. The Alpaca
core has been significantly redesigned by providing a completely new module
offset loader, drastically less code, and has been massively refactored.
Changes to the Alpaca architecture will continue to improve over time as more
code is refactored, removed, and re-architected. Another major change is that
in order to provide an optimal gaming experience and improved maintainability,
Alpaca only supports a single version of Diablo II. If people want to use Alpaca
on a different version, they only need to focus on the particular version they
want to support without having to worry about any sort of backwards or forward
compatibility.

Alpaca IS compatible with my Cactus (Version Switcher). If you want to use
Alpaca for your 1.13d install but also continue to play other mods, versions,
etc, you can use my version switcher to easily do this. When using Alpaca with
Cactus, Simply point your Cactus profile to the "Alpaca.exe" when launching.
Cactus also automatically isolates your character save files and their stashes
as well (Including the Alpaca Stashes).

## Requirements

- Microsoft Visual C++ 2017 Redistributable (x86, x64)

## License

All code is licensed under the GPLv3+ since that was the license that Yohann released PlugY 11.02 under.

## Features

### Core Features

- Infinite Personal and Shared Stashes
- Increased Stash Size [10x10]

### Regular Commands

- `/save` : Saves the character without having to Save and Exit.

### Stash Management Commands

- `/rename <name>` : Renames the current stash page. (If left blank, deletes the page name).
- `/set` : Marks the current stash page with an index.
- `/unset` : Removes the current stash page's index.
- `/setmain` : Marks the current stash page as the main index.
- `/insert` : Inserts a new page after the current page.
- `/delete` : Deletes the current stash page if empty.
- `/page <page #>` : Takes you to the corresponding page.
- `/swap <page #>` : Swaps the content of the current page with the content of the target page.
- `/toggle <page #>` : Swaps the content of the current page with the content of
                       the target page in the opposing shared/personal stash.

## Limitations

### Forwarding characters from previous versions of Diablo II (1.09b -> 1.13d)

If you get a "Unable to enter game. Bad generic file." when entering a game, it means
that you were trying to forward either a character from a previous version of the game
(Such as 1.09b), or a shared stash that was from a previous version, into this current version.
This is not supported. Even if you forward the character with Vanilla D2 (and are able to
play the char with Vanilla D2), the char will not open with this mod. Patches are welcomed
if you really want to support this.

## Links

- [Discord](https://discord.gg/B59qDKy)
- [Cactus](https://github.com/fearedbliss/Cactus)
- [Succulent](https://github.com/fearedbliss/Succulent)
- [Bliss Complete Collection](https://xyinn.org/diablo/Bliss_Complete_Collection.7z)