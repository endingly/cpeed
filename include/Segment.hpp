#pragma once

/**
 * Segment represents a download segment.
 * sp, ep is a offset from a begining of a file.
 * Therefore, if a file size is x, then 0 <= sp <= ep <= x-1.
 * sp, ep is used in Http Range header.
 * e.g. Range: bytes=sp-ep
 * ds is downloaded bytes.
 * If a download of this segement is complete, finish must be set to true.
 */
typedef struct {
  int           cuid;
  long long int sp;
  long long int ep;
  long long int ds;
  bool          finish;
} Segment;

#define SEGMENT_EQUAL(X, Y)                                                                 \
  (X.cuid == Y.cuid && X.sp == Y.sp && X.ep == Y.ep && X.ds == Y.ds && X.finish == Y.finish \
       ? true                                                                               \
       : false)
