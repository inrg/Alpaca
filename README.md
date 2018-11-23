## Alpaca 2.1.2 ([Discord](https://discord.gg/B59qDKy), [Cactus](https://github.com/fearedbliss/Cactus))
##### By Jonathan Vasquez (fearedbliss)
##### Released on Friday, November 23, 2018

## Description

Alpaca is an Infinite Stash Mod for **`Diablo II: Lord of Destruction - 1.13d`**
that is based off Yohann Nicolas' **`PlugY 11.02`**. It is a cleaned up,
re-architected, and stabilized version of the Core of PlugY (Infinite Stash).
Due to the significant code differences and required time investment,
Alpaca will not be updated to **`1.14+`**.

Alpaca is also compatible with my Diablo II Version Switcher: [Cactus](https://github.com/fearedbliss/Cactus).
If you want to use Alpaca, and still play other Mods, Versions, etc, you can use Cactus.
When using Alpaca with Cactus, Simply point your Cactus Platform to the **`Alpaca.exe`** when
launching. Cactus automatically isolates your character save files and their stashes as well.

Cactus also includes my MPQ Fixer along with other non gameplay enhancements. If you get an empty box
during launch or an error related to MPQ corruption, it is because you installed Diablo II with the
new Battle.net installer. Use the MPQ fixer included in Cactus to fix this issue.

## License

Released under the GNU General Public License v3 or Later.

## Requirements

- Microsoft Visual C++ 2017 Redistributable (x86, x64)

## Features

- Infinite Personal and Shared Stashes (Expansion)
- Increased Stash Size [10x10] (Expansion)
- Items will now display their Item Level
- Command Support

## Commands

### Regular
- `/save` : Saves the character without having to Save and Exit.

### Stash Management

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

## Known Bugs

### Picked up item will be lost during character load (If stash contains an item)

If you have items in your stash, and then you pick up an item (and don't place it anywhere),
and save and exit, when you load the game it will be lost. This is an existing PlugY bug that
I wasn't able to fix. I spent a few hours looking around but it will take a way deeper
dive in order to resolve.

### Forwarding characters from previous versions of Diablo II (1.09b -> 1.13d)

If you get a "Unable to enter game. Bad generic file." when entering a game, it means
that you were trying to forward either a character from a previous version of the game
(Such as 1.09b), or a shared stash that was from a previous version, into this current version.
This is not supported. Even if you forward the character with Vanilla D2 (and are able to
play the char with Vanilla D2), the char will not open with this mod. Patches are welcomed
if you really want to support this. An empty character does forward, but I think there is something
going on with the items that character has since normally the normal code will migrate those items
forward (especially 1.09 -> 1.10+).
