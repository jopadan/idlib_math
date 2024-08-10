#include <idlib/math.hpp>

using namespace id::math::types;

int main(int argc, char** argv)
{
	mat<f64<1>, 5, 4> a = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{14,15,16,17}};
	f64<3> v3 = { 1, 2, 3 };
	f64<4> v4 = load3<f64<1>>(v3.data(), 5);
	v3 = store3<f64<1>>(v4);

	printf("v3: [%f %f %f]: %zu/%zu\n", v3[0], v3[1], v3[2], sizeof(v3), alignof(v3));
	printf("v4: [%f %f %f %f]: %zu/%zu\n", v4[0], v4[1], v4[2], v4[3], sizeof(v4), alignof(v4));
	exit(EXIT_SUCCESS);
}
