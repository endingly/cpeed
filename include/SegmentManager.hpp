#pragma once

#include <vector>

#include "Logger.hpp"
#include "Segment.hpp"

#define SEGMENT_FILE_EXTENSION ".df"

/**
 * This class holds the download progress of the one download entry.
 */
class SegmentMan {
 private:
  void  read(FILE* file);
  FILE* openSegFile(std::string segFilename, std::string mode);

 public:
  /**
   * The total number of bytes to download.
   * If Transfer-Encoding is Chunked or Content-Length header is not provided,
   * then this value is set to be 0.
   */
  long long int totalSize;
  /**
   * Represents whether this download is splittable.
   * In Split download(or segmented download), http client establishes
   * more than one connections to the server, and downloads sevral parts of
   * a file at the same time. This boosts download speed.
   * This value is true by default. If total number of bytes is not known or
   * Chunked transfer encoding is used, then this value is set to be 0 by
   * DownloadCommand class.
   */
  bool isSplittable;
  /**
   * Represents whether the download is start or not.
   * The default value is false.
   */
  bool downloadStarted;
  /**
   * Holds segments.
   */
  std::vector<Segment> segments;
  /**
   * Respresents the file name of the downloaded file.
   * If the URL does not contain file name part(http://www.rednoah.com/, for
   * example), this value may be 0 length string.
   * The default value is 0 length string.
   */
  std::string filename;
  /**
   * directory to store a file
   */
  std::string dir;
  /**
   * User defined file name for downloaded content
   */
  std::string ufilename;

  Logger* logger;

  SegmentMan();
  ~SegmentMan();

  /**
   * Returns dir+"/"+filename.
   * If filename is empty, then returns dir+"/"+"inex.html";
   */
  std::string getFilePath() {
    return (dir == "" ? "." : dir) + "/" +
           (ufilename == "" ? (filename == "" ? "index.html" : filename) : ufilename);
  }

  std::string getSegmentFilePath() { return getFilePath() + SEGMENT_FILE_EXTENSION; }

  /**
   * Sets the cuid of the holded segments with specified cuid to 0.
   */
  void unregisterId(int cuid);
  /**
   * There is a segment available for DownloadCommand specified by cuid,
   * fills segment and returns true.
   * There is no segment available, then returns false and segment is
   * undefined in this case.
   *
   * @param segment segment to attach for cuid.
   * @param cuid cuid of DownloadCommand.
   * @returns true: there is a segment available, false: there is no segment
   * available.
   */
  bool getSegment(Segment& segment, int cuid);
  /**
   * Updates the ds value of the specified segment.
   * Only a segment x is updated where x.sp == sgment.sp &amp;&amp; x.ep ==
   * segment.ep &amp;&amp; x.ds == segment.ds &amp;&amp;x.cuid == segment.cuid
   * is true.
   *
   * @param segment segment to update
   */
  void updateSegment(const Segment& segment);
  /**
   * Returns true only if the segment data file exists.
   * The file name of the segment data is filename appended by ".aria2".
   * If isSplittable is false, then returns simply false without any operation.
   */
  bool segmentFileExists();
  /**
   * Loads the segment data file.
   * If isSplittable is false, then returns without any operation.
   */
  void load();
  /**
   * Saves the segment data file.
   * If isSplittable is false, then returns without any operation.
   */
  void save();
  /**
   * Removes the segment data file.
   * If isSplittable is false, then returns without any operation.
   */
  void remove();
  /**
   * Returs true when the download has finished.
   * If downloadStarted is false or the number of the segments of this object
   * holds is 0, then returns false.
   */
  bool finished();
  /**
   * if finished() is true, then call remove()
   */
  void removeIfFinished();
  /**
   * Returns the total number of bytes to be downloaded.
   */
  long long int getDownloadedSize();
};
