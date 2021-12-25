#!/usr/bin/env python3
import json
from . import Tokenizer
from .spell_checker import is_english, sanitize_message
import os
import polib
import io
import gzip


def gen_dictionary():
    po_path = os.path.join(os.path.dirname(__file__),
                           "../../lang/po/cataclysm-dda.pot")
    pofile = polib.pofile(po_path)
    occurrences = dict()
    for entry in pofile:
        words = filter(is_english,
                       Tokenizer.findall(sanitize_message(entry.msgid)))
        for word in words:
            if word in occurrences:
                occurrences[word] += 1
            else:
                occurrences[word] = 1
    dict_path = os.path.join(os.path.dirname(__file__), "dictionary.json.gz")
    jsonized = io.StringIO()
    json.dump(occurrences, fp=jsonized, sort_keys=True)
    with gzip.open(dict_path, "wb") as gzfp:
        gzfp.write(jsonized.getvalue().encode('utf-8'))
