#pragma once

enum {
    PIECE_I,
    PIECE_J,
    PIECE_L,
    PIECE_O,
    PIECE_S,
    PIECE_T,
    PIECE_Z,
    MAX_PIECE
};

#define PIECE_SIZE    4

const Uint8 pieceColor[MAX_PIECE][4] = {
    // PIECE_I
    {  237,   28,   36, 0xFF },
    // PIECE_J
    {  255,  127,   39, 0xFF },
    // PIECE_L
    {  255,  242,    0, 0xFF },
    // PIECE_O
    {   34,  177,   76, 0xFF },
    // PIECE_S
    {    0,  162,  232, 0xFF },
    // PIECE_T
    {   63,   72,  204, 0xFF },
    // PIECE_Z
    {  163,   73,  164, 0xFF }
};

const int pieceBlocks[MAX_PIECE][PIECE_SIZE][PIECE_SIZE] = {
// PIECE_I
{ { 0, 0, 1, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 1, 0 },
  { 0, 0, 1, 0 } },
// PIECE_J
{ { 0, 0, 1, 0 },
  { 0, 0, 1, 0 },
  { 0, 1, 1, 0 },
  { 0, 0, 0, 0 } },
// PIECE_L
{ { 0, 1, 0, 0 },
  { 0, 1, 0, 0 },
  { 0, 1, 1, 0 },
  { 0, 0, 0, 0 } },
// PIECE_O
{ { 0, 0, 0, 0 },
  { 0, 1, 1, 0 },
  { 0, 1, 1, 0 },
  { 0, 0, 0, 0 } },
// PIECE_S
{ { 0, 0, 0, 0 },
  { 0, 1, 1, 0 },
  { 1, 1, 0, 0 },
  { 0, 0, 0, 0 } },
// PIECE_T
{ { 0, 0, 0, 0 },
  { 0, 1, 0, 0 },
  { 1, 1, 1, 0 },
  { 0, 0, 0, 0 } },
// PIECE_Z
{ { 0, 0, 0, 0 },
  { 1, 1, 0, 0 },
  { 0, 1, 1, 0 },
  { 0, 0, 0, 0 } }
};
