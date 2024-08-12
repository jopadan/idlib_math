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

namespace std
{
template<typename T, typename U>
constexpr bool is_decay_equ = std::is_same_v<std::decay_t<T>,U>;
};

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

/* floating point scalar type */
using vecf_t   = f32<1>;
using vecd_t   = f64<1>;

using vec_t = vecf_t;

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
inline vector_aligned<T,4> load3(T src[3], T w) { return (vector_aligned<T,4>){ src[0], src[1], src[2], w }; }

template<sca T>
inline element_aligned<T,3> store3(vector_aligned<T, 4> src, T w = 1)
{
	src[3] *= w;
	if(src[3] != 0 && src[3] != 1)
		src /= src[3];
	return (element_aligned<T,3>){ src[0], src[1], src[2] };
}

template<sca T, size_t SRC, size_t DST, T NET = (T)0> 
auto load(const element_aligned<T, SRC>& src)
{
	static const size_t LEN = power_of_two<DST> ? DST : std::bit_ceil(DST);
	vector_aligned<T,LEN> dst;
	std::copy_n(&src[0], LEN <= SRC ? LEN : SRC, &dst[0]);
	return dst;
}

template<sca T, size_t SRC, size_t DST = SRC, T NET = (T)0>
auto store(const vector_aligned<T,SRC>& src)
{
	element_aligned<T,DST> dst;
	if(DST > SRC)
		std::fill(&dst[SRC], &dst[DST - 1], NET);
	std::copy_n(&src[0], DST <= SRC ? DST : SRC, &dst[0]);
	return dst;
}

template<sca T, size_t N, typename... I>
inline auto permute(const vec<T, N>& src, const I... args)
{
	return (vec<T,sizeof...(I)>){ src[args % N]... };
}

};

