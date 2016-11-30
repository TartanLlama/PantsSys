#pragma once

#include <array>

static constexpr size_t n_chars = 240;
static constexpr char undef = 7; // system bell

const std::array<char, n_chars> decode_table{
    '\0',  '\b',  'a',   'b',   '\t',  'c',   '\n',  'd',   '\r',  ' ',
    '!',   'e',   '\"',  'f',   '#',   '$',   '%',   'g',   '&',   'h',
    '\'',  '(',   '*',   'i',   ')',   '+',   ',',   '-',   '.',   'j',
    '/',   'k',   ':',   ';',   '<',   '=',   '>',   'l',   '?',   '@',
    '[',   'm',   ']',   'n',   '\\',  '^',   '_',   'o',   '`',   '{',
    '|',   '}',   '~',   'p',   127, /*delete*/
    undef, undef, undef, undef, 'q',   undef, 'r',   undef, undef, undef,
    undef, undef, 's',   undef, undef, undef, 't',   undef, 'u',   undef,
    undef, undef, undef, undef, 'v',   undef, undef, undef, 'w',   undef,
    undef, undef, undef, undef, 'x',   undef, undef, undef, undef, undef,
    undef, undef, 'y',   undef, undef, undef, 'z',   undef, 'A',   undef,
    undef, undef, 'B',   undef, 'C',   undef, undef, undef, 'D',   '0',
    '1',   '2',   '3',   '4',   '5',   '6',   '7',   '8',   '9',   undef,
    undef, undef, 'E',   undef, undef, undef, 'F',   undef, undef, undef,
    undef, undef, 'G',   undef, 'H',   undef, undef, undef, undef, undef,
    undef, undef, undef, undef, 'I',   undef, 'J',   undef, undef, undef,
    undef, undef, 'K',   undef, undef, undef, undef, undef, 'L',   undef,
    undef, undef, 'M',   undef, undef, undef, undef, undef, 'N',   undef,
    undef, undef, undef, undef, 'O',   undef, 'P',   undef, undef, undef,
    undef, undef, undef, undef, undef, undef, 'Q',   undef, 'R',   undef,
    undef, undef, 'S',   undef, 'T',   undef, undef, undef, undef, undef,
    undef, undef, undef, undef, undef, undef, 'U',   undef, undef, undef,
    undef, undef, undef, undef, undef, undef, undef, undef, 'V',   undef,
    undef, undef, 'W',   undef, 'X',   undef, undef, undef, 'Y',   undef,
    undef, undef, undef, undef, 'Z'};
