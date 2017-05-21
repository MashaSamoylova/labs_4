#!/bin/bash

var0=0
LIMIT=1000

while [ "$var0" -lt "$LIMIT" ]
do
#  echo  "$var0 "        # -n подавляет перевод строки.
  date +%s | sha256sum | base64 | head -c 32 | ./arithmetic > lol; diff lol check
  var0=`expr $var0 + 1`   # допускается var0=$(($var0+1)).
done

echo

exit 0
