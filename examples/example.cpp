#include <idlib/math.hpp>

using namespace id::math::type;

int main(int argc, char** argv)
{
	mat<vecd_t, 5, 4> a = {{1,2,3,4},{5,6,7,8},{9,10,11,12},{14,15,16,17}};
	vec4d_t v4 = load3<vecd_t>(&a[0][0], 5);
	vec3d_t v3 = store3<vecd_t>(v4);

	v4 = permute<vecd_t,4>(v4, 3,1,2,0);
	v3 = permute<vecd_t,3>(v3, 2,1,0);
	col::u16<col::rgb565> c = { 23, 34, 12 };
	vec4f_t cf = (vec4f_t)c;
	byte_vec4_t cfb = (byte_vec4_t)c;
	printf("color: [%f %f %f %f]: %zu/%zu\n", cf[0], cf[1], cf[2], cf[3], sizeof(v4), alignof(v4));
	printf("color: [%hhu %hhu %hhu %hhu]: %zu/%zu\n", cfb[0], cfb[1], cfb[2], cfb[3], sizeof(v4), alignof(v4));
	printf("color: [%hhu %hhu %hhu]: %zu/%zu\n", c[0], c[1], c[2], sizeof(c), alignof(c));
	
	printf("v3: [%f %f %f]: %zu/%zu\n", v3[0], v3[1], v3[2], sizeof(v3), alignof(v3));
	printf("v4: [%f %f %f %f]: %zu/%zu\n", v4[0], v4[1], v4[2], v4[3], sizeof(v4), alignof(v4));
	exit(EXIT_SUCCESS);
}
