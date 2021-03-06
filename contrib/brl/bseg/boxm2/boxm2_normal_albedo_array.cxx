#include <iostream>
#include <vector>
#include <vcl_compiler.h>
#include <vnl/vnl_math.h>
#include <vgl/vgl_vector_3d.h>
#include "boxm2_normal_albedo_array.h"

const double boxm2_normal_albedo_array_constants::sigma_albedo = 0.02;
const double boxm2_normal_albedo_array_constants::sigma_airlight= 5.0;
const double boxm2_normal_albedo_array_constants::sigma_optical_depth = 0.04;
const double boxm2_normal_albedo_array_constants::sigma_skylight = 10.0;

std::vector<vgl_vector_3d<double> > boxm2_normal_albedo_array::get_normals()
{
  std::vector<vgl_vector_3d<double> > normals;
  normals.push_back(vgl_vector_3d<double>(0.0, 0.0, 1.0));
  unsigned int num_az[] = {8,7};
  for (unsigned int e=0; e<2; ++e) {
    for (unsigned int a=0; a<num_az[e]; ++a) {
      double azimuth = a*vnl_math::twopi/num_az[e];
      double elevation = e*vnl_math::pi_over_2/2;
      double x = std::sin(azimuth)*std::cos(elevation);
      double y = std::cos(azimuth)*std::cos(elevation);
      double z = std::sin(elevation);
      normals.push_back(vgl_vector_3d<double>(x,y,z));
    }
  }
  return normals;
}

