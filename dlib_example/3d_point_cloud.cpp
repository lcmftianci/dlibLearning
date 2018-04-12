#include "3d_point_cloud.h"

using namespace dlib;
using namespace std;

#pragma comment(lib, "dlib_release_64bit_msvc1900.lib")

void PointCloud3D()
{
	std::vector<perspective_window::overlay_dot> points;
	dlib::rand rnd;
	for (double i = 0; i < 20; i += 0.001)
	{
		dlib::vector<double> val(sin(i), cos(i), i / 4);
		dlib::vector<double> temp(rnd.get_random_gaussian(), rnd.get_random_gaussian(), rnd.get_random_gaussian());
		val += temp / 20;
		rgb_pixel color = colormap_jet(i, 0, 20);
		points.push_back(perspective_window::overlay_dot(val, color));
	}

	perspective_window win;
	win.set_title("perspective_window3D point cloud");
	HWND wnd = GetDesktopWindow();
	HDC hdc = GetWindowDC(wnd);
	int DPMX = GetSystemMetrics(SM_CXSCREEN);// / GetDeviceCaps(hdc, HORZSIZE);
	int DPMY = GetSystemMetrics(SM_CYSCREEN);// / GetDeviceCaps(hdc, VERTSIZE);
	win.set_size(DPMX, DPMY);
	win.add_overlay(points);
	win.wait_until_closed();
}
