## Alpaca 2.1.2 | 1.13d
##### By Jonathan Vasquez (fearedbliss)
##### Released on Sunday, November 11, 2018

## Synopsis

Alpaca is an Infinite Stash Mod for Diablo II: Lord of Destruction
that is based off Yohann Nicolas' PlugY 11.02. It is a cleaned up,
re-architected, and stabilized version of PlugY. Due to the significant
code differences and required time investment, Alpaca will not be updated
to 1.14+.

Finally, Alpaca IS compatible with my [Diablo II Version Switcher: Cactus](https://github.com/fearedbliss/Cactus).
If you want to use Alpaca for your 1.13d install (but also want to continue to play
other mods, versions, etc), you can use Cactus. When using Alpaca with Cactus,
Simply point your Cactus Platform to the "Alpaca.exe" when launching. Cactus also
automatically isolates your character save files and their stashes as well
(Including the Alpaca Stashes).

## Requirements

- Microsoft Visual C++ 2017 Redistributable (x86, x64)

## License

All code is licensed under the GPLv3+ since that was the license that Yohann released PlugY 11.02 under.

## Features

### Alpaca Features

- Infinite Personal and Shared Stashes (Expansion)
- Increased Stash Size [10x10] (Expansion)
- Items will now display their Item Level.

### [Singling](https://github.com/fearedbliss/Singling) Features

- You can now run multiple clients of Diablo II.
- The Battle.net button has been disabled for safety reasons.
- Fixed CPU usage bug in the Main Menu, Single Player, and LAN games.
- You are now able to quickly join LAN games.
- The introduction cinematics are now automatically skipped.
- The game will now work with the MPQ files included in the new Blizzard Installer.
- The ambiguity between the number "5" and "6" has been fixed.
- Ladder Runewords are now enabled on Single Player.

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

## Links

- [Discord](https://discord.gg/B59qDKy)
- [Cactus](https://github.com/fearedbliss/Cactus)
- [Succulent](https://github.com/fearedbliss/Succulent)
- [Alpaca](https://github.com/fearedbliss/Alpaca)
- [Singling](https://github.com/fearedbliss/Singling)
- [Bliss Complete Collection](https://xyinn.org/diablo/Bliss_Complete_Collection.7z)
