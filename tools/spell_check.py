#!/usr/bin/env python3
from spell_checker.spell_checker import spell_check, Speller
import polib


pofile = polib.pofile('./lang/po/cataclysm-dda.pot')
for entry in pofile:
    typos = spell_check(entry.msgid)
    if typos:
        print("In message: ", entry.msgid.replace("\n", "\\n"))
        for typo in typos:
            print(typo, "=>", Speller.correction(typo), "(?)")
        print()
