// This is brl/vvid/vvid_harris_corner_process.h
#ifndef vvid_harris_corner_process_h_
#define vvid_harris_corner_process_h_
//----------------------------------------------------------------------------
//:
// \file
// \brief Computes Harris corners on each video frame
// \author
//   J.L. Mundy
//
// \verbatim
//  Modifications:
//   J.L. Mundy February 26, 2002    Initial version.
// \endverbatim
//---------------------------------------------------------------------------
#include <sdet/sdet_harris_detector_params.h>
#include <vvid/vvid_video_process.h>

class vvid_harris_corner_process : public vvid_video_process, public sdet_harris_detector_params
{
 public:
  vvid_harris_corner_process(sdet_harris_detector_params & hdp);
  ~vvid_harris_corner_process();
  virtual process_data_type get_output_type() const { return SPATIAL_OBJECT; }

  //: compute edges on the input image
  virtual bool execute();
  virtual bool finish() { return true; }
};

#endif // vvid_harris_corner_process_h_
