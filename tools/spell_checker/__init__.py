#!/usr/bin/env python3
import os
import re
from spellchecker import SpellChecker


Speller = SpellChecker(language=None)
Tokenizer = re.compile(r'\w+')
Speller.word_frequency.load_dictionary(
    os.path.join(os.path.dirname(__file__), 'dictionary.json.gz'))
