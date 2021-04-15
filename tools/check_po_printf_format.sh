#!/bin/bash
retval=0
for i in lang/po/*.po;
do
    if [ $i == "lang/po/en.po" ]; then
        continue;
    fi
    lang=${i#lang/po/}
    lang=${lang%.po}
    pofilter --test printf --input $i --output $lang.check --progress=none
    if ! [ -f $lang.check ]; then
        continue
    fi
    if ! tools/postprocess_pofilter_check.py < $lang.check > $lang.postcheck; then
        echo "============================================================"
        echo "Error detected in translation to $lang:"
        cat $lang.postcheck
        echo "============================================================"
        echo
        retval=1
    fi
    [ -f $lang.check ] && rm $lang.check;
    [ -f $lang.postcheck ] && rm $lang.postcheck;
done
if (( $retval == 0 )); then
    echo "No error detected";
fi
exit $retval