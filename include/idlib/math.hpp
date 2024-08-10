#include <cstdlib>
#include <cstdio>
#include <cstdint>
#include <utility>
#include <type_traits>
#include <ranges>
#include <array>
#include <bit>
#include <algorithm>
#include <experimental/simd>

#include <GL/gl.h>
#include <GL/glu.h>
#ifdef HAVE_GLCOREARB_H
#include <GL/glcorearb.h>
#endif

namespace stdx = std::experimental;
using namespace stdx::parallelism_v2;

namespace id::math::types
{

/* arithmetic type */
template<typename T>
concept ari = std::integral<T> || std::floating_point<T>;

/* scalar type */
template<typename T>
concept sca = ari<T>;

template<size_t N>
concept power_of_two = (std::popcount(N) == 1);

/* floating point scalar type */
template<sca T = float>
using vec_t = T;

/* element aligned vector type */
template<sca T, size_t ROWS>
using element_aligned = std::array<T,ROWS>;

/* 2^N sized wide aligned vector type */
template<sca T, size_t ROWS = 4, size_t N = std::bit_ceil(ROWS)>
using vector_aligned __attribute__((vector_size(N * sizeof(T)))) = T;

template<sca T, size_t ROWS>
using vec = std::conditional_t<power_of_two<ROWS>, vector_aligned<T, ROWS>, element_aligned<T,ROWS>>;

template<sca T, size_t ROWS = 4, size_t COLS = 1>
using mat = std::conditional_t<COLS == 1, vec<T, ROWS>, vec<T, ROWS>[COLS]>;

template<size_t N = 1>
using f32 = std::conditional_t<N == 1, GLfloat, vec<GLfloat, N>>;
template<size_t N = 1>
using f64 = std::conditional_t<N == 1, GLdouble, vec<GLdouble, N>>;
template<size_t N = 1>
using i8  = std::conditional_t<N == 1, GLbyte, vec<GLbyte, N>>;
template<size_t N = 1>
using s8  = i8<N>;
template<size_t N = 1>
using u8  = std::conditional_t<N == 1, GLubyte, vec<GLubyte, N>>;
template<size_t N = 1>
using i16 = std::conditional_t<N == 1, khronos_int16_t, vec<khronos_int16_t, N>>;
template<size_t N = 1>
using s16 = i16<N>;
template<size_t N = 1>
using u16 = std::conditional_t<N == 1, khronos_uint16_t, vec<khronos_uint16_t, N>>;
template<size_t N = 1>
using i32 = std::conditional_t<N == 1, khronos_int32_t, vec<khronos_int32_t, N>>;
template<size_t N = 1>
using s32 = i32<N>;
template<size_t N = 1>
using u32 = std::conditional_t<N == 1, khronos_uint32_t, vec<khronos_uint32_t, N>>;
template<size_t N = 1>
using i64 = std::conditional_t<N == 1, khronos_int64_t, vec<khronos_uint64_t, N>>;
template<size_t N = 1>
using s64 = i64<N>;
template<size_t N = 1>
using u64 = std::conditional_t<N == 1, khronos_uint64_t, vec<khronos_uint64_t, N>>;
using qboolean = khronos_boolean_enum_t;
const qboolean qfalse = KHRONOS_FALSE;
const qboolean qtrue  = KHRONOS_TRUE;

using vec3f_t  = f32<3>;
using vec3d_t  = f64<3>;
using vec5f_t  = f32<5>;
using vec5d_t  = f64<5>;

using vec2f_t  = f32<2>;
using vec2d_t  = f64<2>;
using vec2i_t  = i32<2>;
using vec2u_t  = u32<2>;
using vec2l_t  = i64<2>;
using vec2lu_t = u64<2>;

using vec4f_t  = f32<4>;
using vec4d_t  = f64<4>;
using vec4i_t  = i32<4>;
using vec4u_t  = u32<4>;
using vec4l_t  = i64<4>;
using vec4lu_t = u64<4>;

template<sca T>
vector_aligned<T,4> load3(T src[3], T w) { return (vector_aligned<T,4>){ src[0], src[1], src[2], w }; }

template<sca T>
element_aligned<T,3> store3(vector_aligned<T, 4> src, T w = 1)
{
	src[3] *= w;
	if(src[3] != 0 && src[3] != 1)
		src /= src[3];
	return (element_aligned<T,3>){ src[0], src[1], src[2] };
}

};

