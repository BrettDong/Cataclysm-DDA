#!/usr/bin/env python3
import sys


def findall(p, s):
    i = s.find(p)
    while i != -1:
        yield i
        i = s.find(p, i + 1)


def is_format_string(char):
    if char.isnumeric():
        return True
    if char in ["s", "d", "f", "l", "."]:
        return True
    return False


def ignore_block(block, start, end):
    for line_idx in range(start, end):
        line = block[line_idx]
        indices = list(findall("%", line))
        for i in range(len(indices)):
            if indices[i] == len(line) - 1:
                break
            if line[indices[i] + 1] == "%":
                i += 1
                continue
            if not is_format_string(line[indices[i] + 1]):
                return True
    return False


current_block = []
blocks = []
for line in sys.stdin:
    line = line[:-1]  # Remove trailing '\n'
    if line == "":
        if len(current_block) > 0:
            blocks.append(current_block)
        current_block = []
    else:
        current_block.append(line)
if len(current_block) > 0:
    blocks.append(current_block)
if len(blocks) == 0:
    exit()
blocks.pop(0)  # Remove PO header
retval = 0
for block in blocks:
    if "#, no-python-format" not in block:
        skip = False
        msgid = -1
        msgstr = -1
        for line_idx in range(len(block)):
            # TODO: implement proper check for plural entries
            if "msgid_plural" in block[line_idx]:
                skip = True
                break
            if "msgid" in block[line_idx]:
                msgid = line_idx
            elif "msgstr" in block[line_idx]:
                msgstr = line_idx
        if msgid == -1 or msgstr == -1 or skip:
            continue
        if ignore_block(block, msgid, msgstr) or \
           ignore_block(block, msgstr, len(block)):
            continue
        retval = 1
        for line in block:
            print(line)
        print()
exit(retval)
