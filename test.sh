#!/bin/bash
try() {
  expected="$1"
  input="$2"

  ./9cc "$input" > tmp.s
  gcc -static -o tmp tmp.s
  ./tmp
  actual="$?"

  if [ "$actual" == "$expected" ]; then
    echo "$input" => "$actual"
  else
    echo "$input expected, but got $actual"
    exit
  fi
}

try 0 0
try 42 42
tty 21 '5+20-4'

echo OK
