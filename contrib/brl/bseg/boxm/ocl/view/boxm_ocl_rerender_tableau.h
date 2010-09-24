// This is brl/bseg/boxm/ocl/view/boxm_ocl_rerender_tableau.h
#ifndef boxm_ocl_rerender_tableau_h
#define boxm_ocl_rerender_tableau_h
//:
// \file
// \brief A tableau to view octree models in real time
// \author Vishal Jain
// \date Aug 11, 2010
#include "boxm_ocl_include_glew.h"
#include <vgui/vgui_gl.h>
#include <vgui/vgui_event_condition.h>
#include <boxm/ocl/view/boxm_cam_tableau.h>
#include <boxm/ocl/view/boxm_ocl_rerender_tableau_sptr.h>
#include <boxm/ocl/boxm_ocl_scene.h>
#include <boxm/boxm_scene_base.h>
#include <vpgl/vpgl_perspective_camera.h>
#include <bocl/bocl_utils.h>
#include <vgui/vgui_statusbar.h>
#include <vil/vil_image_view.h>

class boxm_ocl_rerender_tableau : public boxm_cam_tableau
{
 public:
  boxm_ocl_rerender_tableau();
  virtual ~boxm_ocl_rerender_tableau() {}

  virtual bool handle( vgui_event const& e );

  bool init(boxm_ocl_scene * scene, unsigned ni, unsigned nj, 
            vpgl_perspective_camera<double> * cam,
            vil_image_view<float> & ext_image,
            vpgl_perspective_camera<double> * ext_cam);

  //;  set the GL buffer which needs to be displayed.
  void set_glbuffer(GLuint  pbuffer){ pbuffer_=pbuffer; }
  void set_statusbar(vgui_statusbar* status) { status_ = status; }

 protected:

  //func to render frame on GPU (returns gpu time)
  float render_frame();

  bool init_ocl();
  bool do_init_ocl_rerender;

  //shared GL_CL image buffer
  GLuint pbuffer_;

  boxm_ocl_scene * scene_;
  unsigned ni_;
  unsigned nj_;
  vil_image_view<float>  ext_image_;
  vpgl_perspective_camera<double> * ext_cam_;
  vgui_statusbar* status_;
};

//: Create a smart-pointer to a boxm_ocl_rerender_tableau tableau.
struct boxm_ocl_rerender_tableau_new : public boxm_ocl_rerender_tableau_sptr
{
  //: Constructor - create an empty vgui_easy3D_tableau.
  typedef boxm_ocl_rerender_tableau_sptr base;
  boxm_ocl_rerender_tableau_new() : base( new   boxm_ocl_rerender_tableau ) { }
};

#endif // boxm_ocl_rerender_tableau_h
