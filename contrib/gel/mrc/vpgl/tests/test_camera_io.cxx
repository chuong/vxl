#include <testlib/testlib_test.h>
#include <vcl_iostream.h>

#include <vpgl/vpgl_proj_camera.h>
#include <vpgl/vpgl_calibration_matrix.h>
#include <vpgl/vpgl_perspective_camera.h>

#include <vgl/algo/vgl_rotation_3d.h>
#include <vgl/algo/vgl_h_matrix_3d.h>
#include <vgl/vgl_homg_point_2d.h>
#include <vgl/vgl_homg_point_3d.h>
#include <vnl/vnl_math.h>
#include <vnl/vnl_double_3.h>
#include <vnl/vnl_double_3x3.h>
#include <vnl/vnl_double_3x4.h>
#include <vpl/vpl.h>
#include <vsl/vsl_binary_io.h>
#include <vsl/vsl_binary_loader.h>


static void test_camera_io()
{
  //--------------------------------------------------------------------------
  {
    vcl_cout << "Testing Projective camera" << vcl_endl;

    // Some matrices for testing.
    double random_list2[12] = { 10.6, 1.009, .676, .5, -13, -10, 8, 5, 88, -2, -100, 11 };
    vnl_double_3x4 random_matrix2( random_list2 );

    vcl_cout << "Matrix:\n" << random_matrix2 << vcl_endl;

    vpgl_proj_camera<double> P3( random_matrix2 );
    TEST("vnl_fixed_matrix constructor", P3.get_matrix(), random_matrix2 );

    vsl_b_ofstream bp_out("test_proj_camera_io.tmp");
    TEST("Created test_proj_camera_io.tmp for writing",(!bp_out), false);

    P3.b_write(bp_out);
    bp_out.close();

    // test input binary stream

    vsl_b_ifstream bp_in("test_proj_camera_io.tmp");
    TEST("Opened test_proj_camera_io.tmp for reading",(!bp_in), false);

    vpgl_proj_camera<double> P_in;
    P_in.b_read(bp_in);
    bp_in.close();

    vcl_cout << "Recovered Matrix:\n" << P_in.get_matrix() << vcl_endl;
    TEST("recovery from binary read", P_in.get_matrix(), random_matrix2 );
    // remove file:
    vpl_unlink("test_proj_camera_io.tmp");
  }

  //--------------------------------------------------------------------------
  {
    vcl_cout << "\nTesting Calibration Matrix" << vcl_endl;

    double focal_length = 3;
    vgl_homg_point_2d<double> principal_point(20,30,2);
    double x_scale = 2;
    double y_scale = 2;
    double skew = 0;
    vpgl_calibration_matrix<double> K1( focal_length, principal_point,
                                        x_scale, y_scale, skew );

    vsl_b_ofstream bp_out("test_calibration_matrix_io.tmp");
    TEST("Created test_calibration_matrix_io.tmp for writing",(!bp_out), false);

    K1.b_write(bp_out);
    bp_out.close();

    // test input binary stream

    vsl_b_ifstream bp_in("test_calibration_matrix_io.tmp");
    TEST("Opened test_calibration_matrix_io.tmp for reading",(!bp_in), false);

    vpgl_calibration_matrix<double> K_in;
    K_in.b_read(bp_in);
    bp_in.close();
    vpl_unlink("test_calibration_matrix_io.tmp");
    TEST("focal length from binary read", K_in.focal_length(), K1.focal_length());
    TEST("principal point from binary read", K_in.principal_point(), K1.principal_point());
    TEST("xscale from binary read", K_in.x_scale(), K1.x_scale());
    TEST("yscale from binary read", K_in.y_scale(), K1.y_scale());
    TEST("skew from binary read", K_in.skew(), K1.skew());
  }

  //--------------------------------------------------------------------------
  {
    vcl_cout << "\nTesting Perspective Camera" << vcl_endl;

    // ===========  Construct the camera

    double data[] = { 2000,    0, 512,
                         0, 2000, 384,
                         0,    0,   1 };
    vpgl_calibration_matrix<double> K = vnl_double_3x3(data);
    vgl_homg_point_3d<double>center(0,0,-10.0);

    // rotation angle in radians
    double theta = vnl_math::pi_over_4; // 45 degrees
    // y axis is the rotation axis
    vnl_double_3 axis(0.0, 1.0, 0.0);
    vgl_h_matrix_3d<double> R;
    R.set_identity().set_rotation_about_axis(axis, theta);
    vcl_cout <<"Rotation Matrix\n" << R << '\n';
    vpgl_perspective_camera<double> P(K, center, vgl_rotation_3d<double>(R));

    vcl_cout << "Camera " << P;

    // ===========  Test Simple I/O

    // test output

    vsl_b_ofstream bp_out("test_perspective_camera_io.tmp");
    TEST("Created test_perspective_camera_io.tmp for writing",(!bp_out), false);

    P.b_write(bp_out);
    bp_out.close();

    // test input binary stream

    vsl_b_ifstream bp_in("test_perspective_camera_io.tmp");
    TEST("Opened test_perspective_camera_io.tmp for reading",(!bp_in), false);

    vpgl_perspective_camera<double> P_in;

    P_in.b_read(bp_in);
    bp_in.close();

    vpl_unlink("test_perspective_camera_io.tmp");

    TEST("recovered camera", P, P_in);
    vcl_cout << "Recovered Camera " << P_in;

    // ===========  Test Polymorphic I/O

    vpgl_proj_camera<double> *Pbase = &P;

    vsl_add_to_binary_loader(vpgl_proj_camera<double>());
    vsl_add_to_binary_loader(vpgl_perspective_camera<double>());

    vsl_b_ofstream bp_out2("test_polymorphic_io.tmp");
    TEST("Created test_polymorphic_io.tmp for writing",(!bp_out2), false);

    vsl_b_write(bp_out2,Pbase);
    bp_out2.close();

    // input

    vsl_b_ifstream bp_in2("test_polymorphic_io.tmp");
    TEST("Opened test_polymorphic_io.tmp for reading",(!bp_in2), false);

    vpgl_proj_camera<double> *P_in2=0;

    vsl_b_read(bp_in2,P_in2);
    bp_in2.close();
    vpl_unlink("test_polymorphic_io.tmp");

    TEST("recovered camera type", vcl_string("vpgl_perspective_camera"),P_in2->is_a());
    vcl_cout << "Original type: "             << P.is_a() << '\n'
             << "Basepointer-referred type: " << Pbase->is_a() << '\n'
             << "Recovered camera type: "     << P_in2->is_a() << vcl_endl;

    vpgl_proj_camera<double> *Pdummy = Pbase->clone();
    vcl_cout << "Cloned camera type: " << Pdummy->is_a() << vcl_endl;
    TEST("Cloned equal to original",P,*(Pdummy->cast_to_perspective_camera()));

    TEST("recovered camera", P, *(P_in2->cast_to_perspective_camera()));
    vcl_cout << "Recovered Camera " << *P_in2;

    delete Pdummy;
    delete P_in2;
  }
}

TESTMAIN(test_camera_io);
