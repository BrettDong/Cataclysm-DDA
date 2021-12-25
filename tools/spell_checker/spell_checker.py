#!/usr/bin/env python3
import re
from . import Speller, Tokenizer


def is_english(word):
    for w in word:
        if not ('a' <= w and w <= 'z'):
            return False
    return True


def sanitize_message(message):
    untagged = re.sub(r'<[0-9a-z_/]+>', '', message)
    unformatted = re.sub(r'%[0-9lz\.\s$]*[scfd]', '', untagged)
    return unformatted


def spell_check(message):
    words = filter(is_english, Tokenizer.findall(sanitize_message(message)))
    unknowns = Speller.unknown(words)
    return list(unknowns)
