/*************************************************************************/
/* THIS FILE IS ADAPTED FROM THE GODOT PROJECT BY Tom Beckmann (c) 2022
 * ORIGINAL COPYRIGHT HEADER BELOW:
 */
/*************************************************************************/
/*                       This file is part of:                           */
/*                           GODOT ENGINE                                */
/*                      https://godotengine.org                          */
/*************************************************************************/
/* Copyright (c) 2007-2022 Juan Linietsky, Ariel Manzur.                 */
/* Copyright (c) 2014-2022 Godot Engine contributors (cf. AUTHORS.md).   */
/*                                                                       */
/* Permission is hereby granted, free of charge, to any person obtaining */
/* a copy of this software and associated documentation files (the       */
/* "Software"), to deal in the Software without restriction, including   */
/* without limitation the rights to use, copy, modify, merge, publish,   */
/* distribute, sublicense, and/or sell copies of the Software, and to    */
/* permit persons to whom the Software is furnished to do so, subject to */
/* the following conditions:                                             */
/*                                                                       */
/* The above copyright notice and this permission notice shall be        */
/* included in all copies or substantial portions of the Software.       */
/*                                                                       */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,       */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF    */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.*/
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY  */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,  */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE     */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                */
/*************************************************************************/

#include "marshalls.h"
#include <limits.h>
#include <string.h>
#include <stdio.h>

#define MAX_RECURSION_DEPTH 100

#define ERR_FAIL_ADD_OF(a, b, err) ERR_FAIL_COND_V(((int32_t)(b)) < 0 || ((int32_t)(a)) < 0 || ((int32_t)(a)) > INT_MAX - ((int32_t)(b)), err)
#define ERR_FAIL_MUL_OF(a, b, err) ERR_FAIL_COND_V(((int32_t)(a)) < 0 || ((int32_t)(b)) <= 0 || ((int32_t)(a)) > INT_MAX / ((int32_t)(b)), err)
#define ERR_FAIL_COND_V(cond, err)                                                 \
	if (cond)                                                                      \
	{                                                                              \
		fprintf(stderr, "ERR_FAIL_COND_V: %s:%d (%i)\n", __FILE__, __LINE__, err); \
		return err;                                                                \
	}

#define ENCODE_MASK 0xFF
#define ENCODE_FLAG_64 1 << 16
#define ENCODE_FLAG_OBJECT_AS_ID 1 << 16

static Error
_decode_string(const uint8_t **buf, int *len, int *r_len, godot_string *r_string)
{
	ERR_FAIL_COND_V(*len < 4, ERR_INVALID_DATA);

	int32_t strlen = decode_uint32(*buf);
	int32_t pad = 0;

	// Handle padding
	if (strlen % 4)
	{
		pad = 4 - strlen % 4;
	}

	*buf += 4;
	*len -= 4;

	// Ensure buffer is big enough
	ERR_FAIL_ADD_OF(strlen, pad, ERR_FILE_EOF);
	ERR_FAIL_COND_V(strlen < 0 || strlen + pad > *len, ERR_FILE_EOF);

	ERR_FAIL_COND_V(godot_string_parse_utf8_with_len(r_string, (const char *)*buf, strlen) != OK, ERR_INVALID_DATA);

	// Add padding
	strlen += pad;

	// Update buffer pos, left data count, and return size
	*buf += strlen;
	*len -= strlen;
	if (r_len)
	{
		(*r_len) += 4 + strlen;
	}

	return OK;
}

Error decode_variant(godot_variant *r_variant, const uint8_t *p_buffer, int p_len, int *r_len, bool p_allow_objects, int p_depth)
{
	ERR_FAIL_COND_V(p_depth > MAX_RECURSION_DEPTH, ERR_OUT_OF_MEMORY);
	const uint8_t *buf = p_buffer;
	int len = p_len;

	ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);

	uint32_t type = decode_uint32(buf);

	ERR_FAIL_COND_V((type & ENCODE_MASK) >= GODOT_VARIANT_TYPE_POOL_COLOR_ARRAY + 1, ERR_INVALID_DATA);

	buf += 4;
	len -= 4;
	if (r_len)
	{
		*r_len = 4;
	}

	// Note: We cannot use sizeof(real_t) for decoding, in case a different size is encoded.
	// Decoding math types always checks for the encoded size, while encoding always uses compilation setting.
	// This does lead to some code duplication for decoding, but compatibility is the priority.
	switch (type & ENCODE_MASK)
	{
	case GODOT_VARIANT_TYPE_NIL:
	{
		godot_variant_new_nil(r_variant);
	}
	break;
	case GODOT_VARIANT_TYPE_BOOL:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		bool val = decode_uint32(buf);
		godot_variant_new_bool(r_variant, val);
		if (r_len)
		{
			(*r_len) += 4;
		}
	}
	break;
	case GODOT_VARIANT_TYPE_INT:
	{
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V(len < 8, ERR_INVALID_DATA);
			int64_t val = decode_uint64(buf);
			godot_variant_new_int(r_variant, val);
			if (r_len)
			{
				(*r_len) += 8;
			}
		}
		else
		{
			ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
			int32_t val = decode_uint32(buf);
			godot_variant_new_int(r_variant, val);
			if (r_len)
			{
				(*r_len) += 4;
			}
		}
	}
	break;
	case GODOT_VARIANT_TYPE_REAL:
	{
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double), ERR_INVALID_DATA);
			double val = decode_double(buf);
			godot_variant_new_real(r_variant, val);
			if (r_len)
			{
				(*r_len) += sizeof(double);
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float), ERR_INVALID_DATA);
			float val = decode_float(buf);
			godot_variant_new_real(r_variant, val);
			if (r_len)
			{
				(*r_len) += sizeof(float);
			}
		}
	}
	break;
	case GODOT_VARIANT_TYPE_STRING:
	{
		godot_string str;
		Error err = _decode_string(buf, len, r_len, &str);
		godot_variant_new_string(r_variant, &str);
		godot_string_destroy(&str);
		if (err)
		{
			return err;
		}
	}
	break;

	// math types
	case GODOT_VARIANT_TYPE_VECTOR2:
	{
		godot_vector2 val;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 2, ERR_INVALID_DATA);
			godot_vector2_new(&val, decode_double(&buf[0]), decode_double(&buf[sizeof(double)]));

			if (r_len)
			{
				(*r_len) += sizeof(double) * 2;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 2, ERR_INVALID_DATA);
			godot_vector2_new(&val, decode_double(&buf[0]), decode_double(&buf[sizeof(float)]));

			if (r_len)
			{
				(*r_len) += sizeof(float) * 2;
			}
		}
		godot_variant_new_vector2(r_variant, &val);
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_VECTOR2I:
	{
		ERR_FAIL_COND_V(len < 4 * 2, ERR_INVALID_DATA);
		godot_vector2i val;
		val.x = decode_uint32(&buf[0]);
		val.y = decode_uint32(&buf[4]);
		r_variant = val;

		if (r_len)
		{
			(*r_len) += 4 * 2;
		}
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_RECT2:
	{
		godot_rect2 val;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 4, ERR_INVALID_DATA);
			godot_rect2_new(&val,
							decode_double(&buf[0]),
							decode_double(&buf[sizeof(double)]),
							decode_double(&buf[sizeof(double) * 2]),
							decode_double(&buf[sizeof(double) * 3]));

			if (r_len)
			{
				(*r_len) += sizeof(double) * 4;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 4, ERR_INVALID_DATA);
			godot_rect2_new(&val,
							decode_double(&buf[0]),
							decode_double(&buf[sizeof(float)]),
							decode_double(&buf[sizeof(float) * 2]),
							decode_double(&buf[sizeof(float) * 3]));

			if (r_len)
			{
				(*r_len) += sizeof(float) * 4;
			}
		}
		godot_variant_new_rect2(r_variant, &val);
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_RECT2I:
	{
		ERR_FAIL_COND_V(len < 4 * 4, ERR_INVALID_DATA);
		Rect2i val;
		val.position.x = decode_uint32(&buf[0]);
		val.position.y = decode_uint32(&buf[4]);
		val.size.x = decode_uint32(&buf[8]);
		val.size.y = decode_uint32(&buf[12]);
		r_variant = val;

		if (r_len)
		{
			(*r_len) += 4 * 4;
		}
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_VECTOR3:
	{
		godot_vector3 val;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 3, ERR_INVALID_DATA);
			godot_vector3_new(&val,
							  decode_double(&buf[0]),
							  decode_double(&buf[sizeof(double)]),
							  decode_double(&buf[sizeof(double) * 2]));

			if (r_len)
			{
				(*r_len) += sizeof(double) * 3;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 3, ERR_INVALID_DATA);
			godot_vector3_new(&val,
							  decode_double(&buf[0]),
							  decode_double(&buf[sizeof(float)]),
							  decode_double(&buf[sizeof(float) * 2]));

			if (r_len)
			{
				(*r_len) += sizeof(float) * 3;
			}
		}

		godot_variant_new_vector3(r_variant, &val);
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_VECTOR3I:
	{
		ERR_FAIL_COND_V(len < 4 * 3, ERR_INVALID_DATA);
		Vector3i val;
		val.x = decode_uint32(&buf[0]);
		val.y = decode_uint32(&buf[4]);
		val.z = decode_uint32(&buf[8]);
		r_variant = val;

		if (r_len)
		{
			(*r_len) += 4 * 3;
		}
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_TRANSFORM2D:
	{
		godot_transform2d val;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 6, ERR_INVALID_DATA);
			// FIXME(tobe) no idea about the order
			godot_vector2 x_axis, y_axis, origin;
			godot_vector2_new(&x_axis, decode_double(&buf[0]), decode_double(&buf[sizeof(double)]));
			godot_vector2_new(&y_axis, decode_double(&buf[sizeof(double) * 2]), decode_double(&buf[sizeof(double) * 3]));
			godot_vector2_new(&origin, decode_double(&buf[sizeof(double) * 4]), decode_double(&buf[sizeof(double) * 5]));
			godot_transform2d_new_axis_origin(&val, &x_axis, &y_axis, &origin);

			if (r_len)
			{
				(*r_len) += sizeof(double) * 6;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 6, ERR_INVALID_DATA);
			// FIXME(tobe) no idea about the order
			godot_vector2 x_axis, y_axis, origin;
			godot_vector2_new(&x_axis, decode_float(&buf[0]), decode_float(&buf[sizeof(float)]));
			godot_vector2_new(&y_axis, decode_float(&buf[sizeof(float) * 2]), decode_float(&buf[sizeof(float) * 3]));
			godot_vector2_new(&origin, decode_float(&buf[sizeof(float) * 4]), decode_float(&buf[sizeof(float) * 5]));
			godot_transform2d_new_axis_origin(&val, &x_axis, &y_axis, &origin);

			if (r_len)
			{
				(*r_len) += sizeof(float) * 6;
			}
		}
		godot_variant_new_transform2d(r_variant, &val);
	}
	break;
	case GODOT_VARIANT_TYPE_PLANE:
	{
		godot_plane val;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 4, ERR_INVALID_DATA);
			godot_plane_new_with_reals(&val,
									   decode_double(&buf[0]),
									   decode_double(&buf[sizeof(double)]),
									   decode_double(&buf[sizeof(double) * 2]),
									   decode_double(&buf[sizeof(double) * 3]));

			if (r_len)
			{
				(*r_len) += sizeof(double) * 4;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 4, ERR_INVALID_DATA);
			godot_plane_new_with_reals(&val,
									   decode_float(&buf[0]),
									   decode_float(&buf[sizeof(float)]),
									   decode_float(&buf[sizeof(float) * 2]),
									   decode_float(&buf[sizeof(float) * 3]));

			if (r_len)
			{
				(*r_len) += sizeof(float) * 4;
			}
		}
		godot_variant_new_plane(r_variant, &val);
	}
	break;
	case GODOT_VARIANT_TYPE_QUAT:
	{
		godot_quat val;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 4, ERR_INVALID_DATA);
			godot_quat_new(&val,
						   decode_double(&buf[0]),
						   decode_double(&buf[sizeof(double)]),
						   decode_double(&buf[sizeof(double) * 2]),
						   decode_double(&buf[sizeof(double) * 3]));

			if (r_len)
			{
				(*r_len) += sizeof(double) * 4;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 4, ERR_INVALID_DATA);
			godot_quat_new(&val,
						   decode_float(&buf[0]),
						   decode_float(&buf[sizeof(float)]),
						   decode_float(&buf[sizeof(float) * 2]),
						   decode_float(&buf[sizeof(float) * 3]));

			if (r_len)
			{
				(*r_len) += sizeof(float) * 4;
			}
		}
		godot_variant_new_quat(r_variant, &val);
	}
	break;
	case GODOT_VARIANT_TYPE_AABB:
	{
		godot_aabb val;
		godot_vector3 pos, size;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 6, ERR_INVALID_DATA);
			godot_vector3_new(&pos,
							  decode_double(&buf[0]),
							  decode_double(&buf[sizeof(double)]),
							  decode_double(&buf[sizeof(double) * 2]));
			godot_vector3_new(&size,
							  decode_double(&buf[sizeof(double) * 3]),
							  decode_double(&buf[sizeof(double) * 4]),
							  decode_double(&buf[sizeof(double) * 5]));
			godot_aabb_new(&val, &pos, &size);

			if (r_len)
			{
				(*r_len) += sizeof(double) * 6;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 6, ERR_INVALID_DATA);
			godot_vector3_new(&pos,
							  decode_float(&buf[0]),
							  decode_float(&buf[sizeof(float)]),
							  decode_float(&buf[sizeof(float) * 2]));
			godot_vector3_new(&size,
							  decode_float(&buf[sizeof(float) * 3]),
							  decode_float(&buf[sizeof(double) * 4]),
							  decode_float(&buf[sizeof(float) * 5]));
			godot_aabb_new(&val, &pos, &size);

			if (r_len)
			{
				(*r_len) += sizeof(float) * 6;
			}
		}
		godot_variant_new_aabb(r_variant, &val);
	}
	break;
	case GODOT_VARIANT_TYPE_BASIS:
	{
		godot_basis val;
		godot_vector3 x, y, z;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 9, ERR_INVALID_DATA);
			godot_vector3_new(&x,
							  decode_double(&buf[0]),
							  decode_double(&buf[sizeof(double)]),
							  decode_double(&buf[sizeof(double) * 2]));
			godot_vector3_new(&y,
							  decode_double(&buf[sizeof(double) * 3]),
							  decode_double(&buf[sizeof(double) * 4]),
							  decode_double(&buf[sizeof(double) * 5]));
			godot_vector3_new(&z,
							  decode_double(&buf[sizeof(double) * 6]),
							  decode_double(&buf[sizeof(double) * 7]),
							  decode_double(&buf[sizeof(double) * 8]));
			godot_basis_new_with_rows(&val, &x, &y, &z);

			if (r_len)
			{
				(*r_len) += sizeof(double) * 9;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 9, ERR_INVALID_DATA);
			godot_vector3_new(&x,
							  decode_float(&buf[0]),
							  decode_float(&buf[sizeof(float)]),
							  decode_float(&buf[sizeof(float) * 2]));
			godot_vector3_new(&y,
							  decode_float(&buf[sizeof(float) * 3]),
							  decode_float(&buf[sizeof(float) * 4]),
							  decode_float(&buf[sizeof(float) * 5]));
			godot_vector3_new(&z,
							  decode_float(&buf[sizeof(float) * 6]),
							  decode_float(&buf[sizeof(float) * 7]),
							  decode_float(&buf[sizeof(float) * 8]));
			godot_basis_new_with_rows(&val, &x, &y, &z);

			if (r_len)
			{
				(*r_len) += sizeof(float) * 9;
			}
		}
		godot_variant_new_basis(r_variant, &val);
	}
	break;
	case GODOT_VARIANT_TYPE_TRANSFORM:
	{
		godot_transform val;
		godot_vector3 x, y, z;
		godot_basis basis;
		godot_vector3 origin;
		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(double) * 12, ERR_INVALID_DATA);
			godot_vector3_new(&x,
							  decode_double(&buf[0]),
							  decode_double(&buf[sizeof(double)]),
							  decode_double(&buf[sizeof(double) * 2]));
			godot_vector3_new(&y,
							  decode_double(&buf[sizeof(double) * 3]),
							  decode_double(&buf[sizeof(double) * 4]),
							  decode_double(&buf[sizeof(double) * 5]));
			godot_vector3_new(&z,
							  decode_double(&buf[sizeof(double) * 6]),
							  decode_double(&buf[sizeof(double) * 7]),
							  decode_double(&buf[sizeof(double) * 8]));
			godot_basis_new_with_rows(&basis, &x, &y, &z);
			godot_vector3_new(&origin,
							  decode_double(&buf[sizeof(double) * 9]),
							  decode_double(&buf[sizeof(double) * 10]),
							  decode_double(&buf[sizeof(double) * 11]));
			godot_transform_new(&val, &basis, &origin);

			if (r_len)
			{
				(*r_len) += sizeof(double) * 12;
			}
		}
		else
		{
			ERR_FAIL_COND_V((size_t)len < sizeof(float) * 12, ERR_INVALID_DATA);
			godot_vector3_new(&x,
							  decode_float(&buf[0]),
							  decode_float(&buf[sizeof(float)]),
							  decode_float(&buf[sizeof(float) * 2]));
			godot_vector3_new(&y,
							  decode_float(&buf[sizeof(float) * 3]),
							  decode_float(&buf[sizeof(float) * 4]),
							  decode_float(&buf[sizeof(float) * 5]));
			godot_vector3_new(&z,
							  decode_float(&buf[sizeof(float) * 6]),
							  decode_float(&buf[sizeof(float) * 7]),
							  decode_float(&buf[sizeof(float) * 8]));
			godot_basis_new_with_rows(&basis, &x, &y, &z);
			godot_vector3_new(&origin,
							  decode_float(&buf[sizeof(float) * 9]),
							  decode_float(&buf[sizeof(float) * 10]),
							  decode_float(&buf[sizeof(float) * 11]));
			godot_transform_new(&val, &basis, &origin);

			if (r_len)
			{
				(*r_len) += sizeof(float) * 12;
			}
		}
		godot_variant_new_transform(r_variant, &val);
	}
	break;
	// misc types
	case GODOT_VARIANT_TYPE_COLOR:
	{
		ERR_FAIL_COND_V(len < 4 * 4, ERR_INVALID_DATA);
		godot_color val;
		godot_color_new_rgba(&val,
							 decode_float(&buf[0]),
							 decode_float(&buf[sizeof(float)]),
							 decode_float(&buf[sizeof(float) * 2]),
							 decode_float(&buf[sizeof(float) * 3]));
		godot_variant_new_color(r_variant, &val);

		if (r_len)
		{
			(*r_len) += 4 * 4; // Colors should always be in single-precision.
		}
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_STRING_NAME:
	{
		String str;
		Error err = _decode_string(buf, len, r_len, str);
		if (err)
		{
			return err;
		}
		r_variant = StringName(str);
	}
	break;
#endif

	case GODOT_VARIANT_TYPE_NODE_PATH:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t strlen = decode_uint32(buf);

		if (strlen & 0x80000000)
		{
			// new format
			ERR_FAIL_COND_V(len < 12, ERR_INVALID_DATA);
			godot_string path;
			godot_string_new(&path);

			uint32_t namecount = strlen &= 0x7FFFFFFF;
			uint32_t subnamecount = decode_uint32(buf + 4);
			uint32_t flags = decode_uint32(buf + 8);

			len -= 12;
			buf += 12;

			if (flags & 2)
			{ // Obsolete format with property separate from subpath
				subnamecount++;
			}

			uint32_t total = namecount + subnamecount;

			if (r_len)
			{
				(*r_len) += 12;
			}

			for (uint32_t i = 0; i < total; i++)
			{
				godot_string str;
				Error err = _decode_string(buf, len, r_len, &str);
				if (err)
				{
					return err;
				}

				if (i < namecount)
				{
					// check if absolute
					if (i > 0 || flags & i)
					{
						godot_string sep;
						godot_string_new_with_wide_string(&sep, L"/", 1);
						godot_string_insert(&path, godot_string_length(&path), sep);
						godot_string_destroy(&sep);
					}

					godot_string_insert(&path, godot_string_length(&path), str);
				}
				else
				{
					godot_string sep;
					godot_string_new_with_wide_string(&sep, L":", 1);
					godot_string_insert(&path, godot_string_length(&path), sep);
					godot_string_destroy(&sep);

					godot_string_insert(&path, godot_string_length(&path), str);
				}
				godot_string_destroy(&str);
			}

			godot_node_path val;
			godot_node_path_new(&val, &path);
			godot_variant_new_node_path(r_variant, &val);
		}
		else
		{
			// old format, just a string

			ERR_FAIL_COND_V(true, ERR_INVALID_DATA);
		}
	}
	break;
	case GODOT_VARIANT_TYPE_RID:
	{
		ERR_FAIL_COND_V(len < 8, ERR_INVALID_DATA);
		uint64_t id = decode_uint64(buf);
		if (r_len)
		{
			(*r_len) += 8;
		}

		godot_rid val;
		// FIXME(tobe) no access to setting the RID num :(
		ERR_FAIL_COND_V(true, ERR_BUG);
		// godot_rid_new(&val, id);
		godot_variant_new_rid(r_variant, &val);
	}
	break;
	case GODOT_VARIANT_TYPE_OBJECT:
	{
		if (type & ENCODE_FLAG_OBJECT_AS_ID)
		{
			// this _is_ allowed
			ERR_FAIL_COND_V(true, ERR_BUG);
			ERR_FAIL_COND_V(len < 8, ERR_INVALID_DATA);
#if MISSING
			ObjectID val = ObjectID(decode_uint64(buf));
			if (r_len)
			{
				(*r_len) += 8;
			}

			if (val.is_null())
			{
				r_variant = (Object *)nullptr;
			}
			else
			{
				Ref<EncodedObjectAsID> obj_as_id;
				obj_as_id.instantiate();
				obj_as_id->set_object_id(val);

				r_variant = obj_as_id;
			}
#endif
		}
		else
		{
			ERR_FAIL_COND_V(!p_allow_objects, ERR_UNAUTHORIZED);

			godot_string str;
			Error err = _decode_string(buf, len, r_len, &str);
			if (err)
			{
				return err;
			}

			if (godot_string_empty(&str))
			{
				godot_variant_new_object(r_variant, NULL);
			}
			else
			{
				ERR_FAIL_COND_V(true, ERR_BUG);
#if MISSING
				Object *obj = ClassDB::instantiate(str);

				ERR_FAIL_COND_V(!obj, ERR_UNAVAILABLE);
				ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);

				int32_t count = decode_uint32(buf);
				buf += 4;
				len -= 4;
				if (r_len)
				{
					(*r_len) += 4; // Size of count number.
				}

				for (int i = 0; i < count; i++)
				{
					str = String();
					err = _decode_string(buf, len, r_len, str);
					if (err)
					{
						return err;
					}

					Variant value;
					int used;
					err = decode_variant(value, buf, len, &used, p_allow_objects, p_depth + 1);
					if (err)
					{
						return err;
					}

					buf += used;
					len -= used;
					if (r_len)
					{
						(*r_len) += used;
					}

					obj->set(str, value);
				}

				if (Object::cast_to<RefCounted>(obj))
				{
					Ref<RefCounted> ref = Ref<RefCounted>(Object::cast_to<RefCounted>(obj));
					r_variant = ref;
				}
				else
				{
					r_variant = obj;
				}
#endif
			}
		}
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_CALLABLE:
	{
		r_variant = Callable();
	}
	break;
#endif
#if MISSING
	case GODOT_VARIANT_TYPE_SIGNAL:
	{
		String name;
		Error err = _decode_string(buf, len, r_len, name);
		if (err)
		{
			return err;
		}

		ERR_FAIL_COND_V(len < 8, ERR_INVALID_DATA);
		ObjectID id = ObjectID(decode_uint64(buf));
		if (r_len)
		{
			(*r_len) += 8;
		}

		r_variant = Signal(id, StringName(name));
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_DICTIONARY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		//  bool shared = count&0x80000000;
		count &= 0x7FFFFFFF;

		buf += 4;
		len -= 4;

		if (r_len)
		{
			(*r_len) += 4; // Size of count number.
		}

		godot_dictionary d;
		godot_dictionary_new(&d);

		for (int i = 0; i < count; i++)
		{
			godot_variant key, value;
			int used;
			Error err = decode_variant(&key, buf, len, &used, p_allow_objects, p_depth + 1);
			ERR_FAIL_COND_V(err != OK, err);

			buf += used;
			len -= used;
			if (r_len)
			{
				(*r_len) += used;
			}

			err = decode_variant(&value, buf, len, &used, p_allow_objects, p_depth + 1);
			ERR_FAIL_COND_V(err != OK, err);

			buf += used;
			len -= used;
			if (r_len)
			{
				(*r_len) += used;
			}

			godot_dictionary_set(&d, &key, &value);

			// FIXME(tobe) do we move ownership or not?
			godot_variant_destroy(&key);
			godot_variant_destroy(&value);
		}

		godot_variant_new_dictionary(r_variant, &d);
		godot_dictionary_destroy(&d);
	}
	break;
	case GODOT_VARIANT_TYPE_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		//  bool shared = count&0x80000000;
		count &= 0x7FFFFFFF;

		buf += 4;
		len -= 4;

		if (r_len)
		{
			(*r_len) += 4; // Size of count number.
		}

		godot_array varr;

		for (int i = 0; i < count; i++)
		{
			int used = 0;
			godot_variant v;
			Error err = decode_variant(&v, buf, len, &used, p_allow_objects, p_depth + 1);
			ERR_FAIL_COND_V(err != OK, err);
			buf += used;
			len -= used;
			godot_array_append(&varr, &v);
			// FIXME(tobe) do we move ownership?
			godot_variant_destroy(&v);
			if (r_len)
			{
				(*r_len) += used;
			}
		}

		godot_variant_new_array(r_variant, &varr);
	}
	break;

	// arrays
	case GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;
		ERR_FAIL_COND_V(count < 0 || count > len, ERR_INVALID_DATA);

		godot_pool_byte_array data;
		godot_pool_byte_array_new(&data);

		if (count)
		{
			godot_pool_byte_array_resize(&data, count);
			for (int32_t i = 0; i < count; i++)
			{
				godot_pool_byte_array_append(&data, buf[i]);
			}
		}

		godot_variant_new_pool_byte_array(r_variant, &data);

		if (r_len)
		{
			if (count % 4)
			{
				(*r_len) += 4 - count % 4;
			}
			(*r_len) += 4 + count;
		}
	}
	break;
	case GODOT_VARIANT_TYPE_POOL_INT_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;
		ERR_FAIL_MUL_OF(count, 4, ERR_INVALID_DATA);
		ERR_FAIL_COND_V(count < 0 || count * 4 > len, ERR_INVALID_DATA);

		godot_pool_int_array data;
		godot_pool_int_array_new(&data);

		if (count)
		{
			// const int*rbuf=(const int*)buf;
			godot_pool_int_array_resize(&data, count);
			for (int32_t i = 0; i < count; i++)
			{
				godot_pool_int_array_append(&data, decode_uint32(&buf[i * 4]));
			}
		}
		godot_variant_new_pool_int_array(r_variant, &data);
		if (r_len)
		{
			(*r_len) += 4 + count * sizeof(int32_t);
		}
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_PACKED_INT64_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;
		ERR_FAIL_MUL_OF(count, 8, ERR_INVALID_DATA);
		ERR_FAIL_COND_V(count < 0 || count * 8 > len, ERR_INVALID_DATA);

		Vector<int64_t> data;

		if (count)
		{
			// const int*rbuf=(const int*)buf;
			data.resize(count);
			int64_t *w = data.ptrw();
			for (int64_t i = 0; i < count; i++)
			{
				w[i] = decode_uint64(&buf[i * 8]);
			}
		}
		r_variant = Variant(data);
		if (r_len)
		{
			(*r_len) += 4 + count * sizeof(int64_t);
		}
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_POOL_REAL_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;
		ERR_FAIL_MUL_OF(count, 4, ERR_INVALID_DATA);
		ERR_FAIL_COND_V(count < 0 || count * 4 > len, ERR_INVALID_DATA);

		godot_pool_real_array data;

		if (count)
		{
			// const float*rbuf=(const float*)buf;
			godot_pool_real_array_resize(&data, count);
			for (int32_t i = 0; i < count; i++)
			{
				godot_pool_real_array_append(&data, decode_float(&buf[i * 4]));
			}
		}
		godot_variant_new_pool_real_array(r_variant, &data);

		if (r_len)
		{
			(*r_len) += 4 + count * sizeof(float);
		}
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_PACKED_FLOAT64_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;
		ERR_FAIL_MUL_OF(count, 8, ERR_INVALID_DATA);
		ERR_FAIL_COND_V(count < 0 || count * 8 > len, ERR_INVALID_DATA);

		Vector<double> data;

		if (count)
		{
			data.resize(count);
			double *w = data.ptrw();
			for (int64_t i = 0; i < count; i++)
			{
				w[i] = decode_double(&buf[i * 8]);
			}
		}
		r_variant = data;

		if (r_len)
		{
			(*r_len) += 4 + count * sizeof(double);
		}
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_POOL_STRING_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);

		godot_pool_string_array data;
		buf += 4;
		len -= 4;

		if (r_len)
		{
			(*r_len) += 4; // Size of count number.
		}

		for (int32_t i = 0; i < count; i++)
		{
			godot_string str;
			Error err = _decode_string(buf, len, r_len, &str);
			if (err)
			{
				return err;
			}

			godot_pool_string_array_append(&data, &str);
			// FIXME(tobe) do we move ownership?
			godot_string_destroy(&str);
		}

		godot_variant_new_pool_string_array(r_variant, &data);
		// FIXME(tobe) do we move ownership?
		godot_pool_string_array_destroy(&data);
	}
	break;
	case GODOT_VARIANT_TYPE_POOL_VECTOR2_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;

		godot_pool_vector2_array varray;
		godot_pool_vector2_array_new(&varray);

		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_MUL_OF(count, sizeof(double) * 2, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * sizeof(double) * 2 > (size_t)len, ERR_INVALID_DATA);

			if (r_len)
			{
				(*r_len) += 4; // Size of count number.
			}

			if (count)
			{
				godot_pool_vector2_array_resize(&varray, count);

				for (int32_t i = 0; i < count; i++)
				{
					godot_vector2 vec;
					godot_vector2_new(&vec,
									  decode_double(buf + i * sizeof(double) * 2 + sizeof(double) * 0),
									  decode_double(buf + i * sizeof(double) * 2 + sizeof(double) * 1));
					godot_pool_vector2_array_append(&varray, &vec);
				}

				int adv = sizeof(double) * 2 * count;

				if (r_len)
				{
					(*r_len) += adv;
				}
				len -= adv;
				buf += adv;
			}
		}
		else
		{
			ERR_FAIL_MUL_OF(count, sizeof(float) * 2, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * sizeof(float) * 2 > (size_t)len, ERR_INVALID_DATA);

			if (r_len)
			{
				(*r_len) += 4; // Size of count number.
			}

			if (count)
			{
				godot_pool_vector2_array_resize(&varray, count);

				for (int32_t i = 0; i < count; i++)
				{
					godot_vector2 vec;
					godot_vector2_new(&vec,
									  decode_float(buf + i * sizeof(float) * 2 + sizeof(float) * 0),
									  decode_float(buf + i * sizeof(float) * 2 + sizeof(float) * 1));
					godot_pool_vector2_array_append(&varray, &vec);
				}

				int adv = sizeof(float) * 2 * count;

				if (r_len)
				{
					(*r_len) += adv;
				}
			}
		}
		godot_variant_new_pool_vector2_array(r_variant, &varray);
		// FIXME(tobe) do we move ownership?
		godot_pool_vector2_array_destroy(&varray);
	}
	break;
	case GODOT_VARIANT_TYPE_POOL_VECTOR3_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;

		godot_pool_vector3_array varray;
		godot_pool_vector3_array_new(&varray);

		if (type & ENCODE_FLAG_64)
		{
			ERR_FAIL_MUL_OF(count, sizeof(double) * 3, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * sizeof(double) * 3 > (size_t)len, ERR_INVALID_DATA);

			if (r_len)
			{
				(*r_len) += 4; // Size of count number.
			}

			if (count)
			{
				godot_pool_vector3_array_resize(&varray, count);
				godot_vector3 vec;

				for (int32_t i = 0; i < count; i++)
				{
					godot_vector3_new(&vec,
									  decode_double(buf + i * sizeof(double) * 3 + sizeof(double) * 0),
									  decode_double(buf + i * sizeof(double) * 3 + sizeof(double) * 1),
									  decode_double(buf + i * sizeof(double) * 3 + sizeof(double) * 2));
					godot_pool_vector3_array_append(&varray, &vec);
				}

				int adv = sizeof(double) * 3 * count;

				if (r_len)
				{
					(*r_len) += adv;
				}
				len -= adv;
				buf += adv;
			}
		}
		else
		{
			ERR_FAIL_MUL_OF(count, sizeof(float) * 3, ERR_INVALID_DATA);
			ERR_FAIL_COND_V(count < 0 || count * sizeof(float) * 3 > (size_t)len, ERR_INVALID_DATA);

			if (r_len)
			{
				(*r_len) += 4; // Size of count number.
			}

			if (count)
			{
				godot_pool_vector3_array_resize(&varray, count);
				godot_vector3 vec;

				for (int32_t i = 0; i < count; i++)
				{
					godot_vector3_new(&vec,
									  decode_float(buf + i * sizeof(float) * 3 + sizeof(float) * 0),
									  decode_float(buf + i * sizeof(float) * 3 + sizeof(float) * 1),
									  decode_float(buf + i * sizeof(float) * 3 + sizeof(float) * 2));
					godot_pool_vector3_array_append(&varray, &vec);
				}

				int adv = sizeof(float) * 3 * count;

				if (r_len)
				{
					(*r_len) += adv;
				}
				len -= adv;
				buf += adv;
			}
		}
		godot_variant_new_pool_vector3_array(r_variant, &varray);
		// FIXME(tobe) do we move ownership?
		godot_pool_vector3_array_destroy(&varray);
	}
	break;
	case GODOT_VARIANT_TYPE_POOL_COLOR_ARRAY:
	{
		ERR_FAIL_COND_V(len < 4, ERR_INVALID_DATA);
		int32_t count = decode_uint32(buf);
		buf += 4;
		len -= 4;

		ERR_FAIL_MUL_OF(count, 4 * 4, ERR_INVALID_DATA);
		ERR_FAIL_COND_V(count < 0 || count * 4 * 4 > len, ERR_INVALID_DATA);

		godot_pool_color_array carray;
		godot_pool_color_array_new(&carray);

		if (r_len)
		{
			(*r_len) += 4; // Size of count number.
		}

		if (count)
		{
			godot_pool_color_array_resize(&carray, count);

			for (int32_t i = 0; i < count; i++)
			{
				// Colors should always be in single-precision.
				godot_color color;
				godot_color_new_rgba(&color,
									 decode_float(buf + i * 4 * 4 + 4 * 0),
									 decode_float(buf + i * 4 * 4 + 4 * 1),
									 decode_float(buf + i * 4 * 4 + 4 * 2),
									 decode_float(buf + i * 4 * 4 + 4 * 3));
				godot_pool_color_array_append(&carray, &color);
			}

			int adv = 4 * 4 * count;

			if (r_len)
			{
				(*r_len) += adv;
			}
		}

		godot_variant_new_pool_color_array(r_variant, &carray);
		godot_pool_color_array_destroy(&carray);
	}
	break;
	default:
	{
		ERR_FAIL_COND_V(true, ERR_BUG);
	}
	}

	return OK;
}

static void _encode_string(const godot_string *p_string, uint8_t **buf, int *r_len)
{
	godot_char_string utf8 = godot_string_utf8(p_string);
	godot_int len = godot_char_string_length(&utf8);

	if (buf)
	{
		encode_uint32(len, *buf);
		buf += 4;
		memcpy(*buf, godot_char_string_get_data(&utf8), len);
		buf += len;
	}

	*r_len += 4 + len;
	while (*r_len % 4)
	{
		(*r_len)++; // pad
		if (buf)
		{
			**(buf++) = 0;
		}
	}
}

Error encode_variant(godot_variant *p_variant, uint8_t *r_buffer, int *r_len, bool p_full_objects, int p_depth)
{
	if (p_depth > MAX_RECURSION_DEPTH)
	{
		fprintf(stderr, "Potential infinite recursion detected. Bailing.");
		return ERR_OUT_OF_MEMORY;
	}
	uint8_t *buf = r_buffer;

	*r_len = 0;

	uint32_t flags = 0;

	switch (godot_variant_get_type(p_variant))
	{
	case GODOT_VARIANT_TYPE_INT:
	{

		int64_t val = godot_variant_as_int(p_variant);
		if (val > (int64_t)INT_MAX || val < (int64_t)INT_MIN)
		{
			flags |= ENCODE_FLAG_64;
		}
	}
	break;
	case GODOT_VARIANT_TYPE_REAL:
	{
		double d = godot_variant_as_real(p_variant);
		float f = d;
		if (((double)f) != d)
		{
			flags |= ENCODE_FLAG_64;
		}
	}
	break;
	case GODOT_VARIANT_TYPE_OBJECT:
	{
// Test for potential wrong values sent by the debugger when it breaks.
#if MISSING
		Object *obj = p_variant.get_validated_object();
		if (!obj)
		{
			// Object is invalid, send a nullptr instead.
			if (buf)
			{
				encode_uint32(Variant::NIL, buf);
			}
			*r_len += 4;
			return OK;
		}
#endif

		if (!p_full_objects)
		{
			flags |= ENCODE_FLAG_OBJECT_AS_ID;
		}
	}
	break;
#ifdef REAL_T_IS_DOUBLE
	case GODOT_VARIANT_TYPE_VECTOR2:
	case GODOT_VARIANT_TYPE_VECTOR3:
	case GODOT_VARIANT_TYPE_PACKED_VECTOR2_ARRAY:
	case GODOT_VARIANT_TYPE_PACKED_VECTOR3_ARRAY:
	case GODOT_VARIANT_TYPE_TRANSFORM2D:
	case GODOT_VARIANT_TYPE_TRANSFORM3D:
	case GODOT_VARIANT_TYPE_QUATERNION:
	case GODOT_VARIANT_TYPE_PLANE:
	case GODOT_VARIANT_TYPE_BASIS:
	case GODOT_VARIANT_TYPE_RECT2:
	case GODOT_VARIANT_TYPE_AABB:
	{
		flags |= ENCODE_FLAG_64;
	}
	break;
#endif // REAL_T_IS_DOUBLE
	default:
	{
	} // nothing to do at this stage
	}

	if (buf)
	{
		encode_uint32(godot_variant_get_type(p_variant) | flags, buf);
		buf += 4;
	}
	*r_len += 4;

	switch (godot_variant_get_type(p_variant))
	{
	case GODOT_VARIANT_TYPE_NIL:
	{
		// nothing to do
	}
	break;
	case GODOT_VARIANT_TYPE_BOOL:
	{
		if (buf)
		{
			encode_uint32(godot_variant_as_bool(p_variant), buf);
		}

		*r_len += 4;
	}
	break;
	case GODOT_VARIANT_TYPE_INT:
	{
		if (flags & ENCODE_FLAG_64)
		{
			// 64 bits
			if (buf)
			{

				encode_uint64(godot_variant_as_int(p_variant), buf);
			}

			*r_len += 8;
		}
		else
		{
			if (buf)
			{
				encode_uint32(godot_variant_as_int(p_variant), buf);
			}

			*r_len += 4;
		}
	}
	break;
	case GODOT_VARIANT_TYPE_REAL:
	{
		if (flags & ENCODE_FLAG_64)
		{
			if (buf)
			{
				encode_double(godot_variant_as_real(p_variant), buf);
			}

			*r_len += 8;
		}
		else
		{
			if (buf)
			{
				encode_float(godot_variant_as_real(p_variant), buf);
			}

			*r_len += 4;
		}
	}
	break;
	case GODOT_VARIANT_TYPE_NODE_PATH:
	{
		godot_node_path np = godot_variant_as_node_path(p_variant);
		if (buf)
		{
			encode_uint32(((uint32_t)godot_node_path_get_name_count(&np)) | 0x80000000, buf); // for compatibility with the old format
			encode_uint32(godot_node_path_get_subname_count(&np), buf + 4);
			uint32_t np_flags = 0;
			if (godot_node_path_is_absolute(&np))
			{
				np_flags |= 1;
			}

			encode_uint32(np_flags, buf + 8);

			buf += 12;
		}

		*r_len += 12;

		int total = godot_node_path_get_name_count(&np) + godot_node_path_get_subname_count(&np);

		for (int i = 0; i < total; i++)
		{
			godot_string str;

			if (i < godot_node_path_get_name_count(&np))
			{
				str = godot_node_path_get_name(&np, i);
			}
			else
			{
				str = godot_node_path_get_subname(&np, i - godot_node_path_get_name_count(&np));
			}

			godot_char_string utf8 = godot_string_utf8(&str);

			int pad = 0;
			godot_int len = godot_char_string_length(&utf8);

			if (len % 4)
			{
				pad = 4 - len % 4;
			}

			if (buf)
			{
				encode_uint32(len, buf);
				buf += 4;
				memcpy(buf, godot_char_string_get_data(&utf8), len);
				buf += pad + len;
			}

			*r_len += 4 + len + pad;
		}
	}
	break;
	case GODOT_VARIANT_TYPE_STRING:
#if MISSING
	case GODOT_VARIANT_TYPE_STRING_NAME:
#endif
	{
		godot_string s = godot_variant_as_string(p_variant);
		_encode_string(&s, &buf, r_len);
	}
	break;

	// math types
	case GODOT_VARIANT_TYPE_VECTOR2:
	{
		if (buf)
		{
			godot_vector2 v2 = godot_variant_as_vector2(p_variant);
			encode_real(godot_vector2_get_x(&v2), &buf[0]);
			encode_real(godot_vector2_get_y(&v2), &buf[sizeof(real_t)]);
		}

		*r_len += 2 * sizeof(real_t);
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_VECTOR2I:
	{
		if (buf)
		{
			Vector2i v2 = p_variant;
			encode_uint32(v2.x, &buf[0]);
			encode_uint32(v2.y, &buf[4]);
		}

		*r_len += 2 * 4;
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_RECT2:
	{
		if (buf)
		{
			godot_rect2 r2 = godot_variant_as_rect2(p_variant);
			godot_vector2 pos = godot_rect2_get_position(&r2);
			godot_vector2 size = godot_rect2_get_size(&r2);
			encode_real(godot_vector2_get_x(&pos), &buf[0]);
			encode_real(godot_vector2_get_y(&pos), &buf[sizeof(real_t)]);
			encode_real(godot_vector2_get_x(&size), &buf[sizeof(real_t) * 2]);
			encode_real(godot_vector2_get_y(&size), &buf[sizeof(real_t) * 3]);
		}
		*r_len += 4 * sizeof(real_t);
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_RECT2I:
	{
		if (buf)
		{
			Rect2i r2 = p_variant;
			encode_uint32(r2.position.x, &buf[0]);
			encode_uint32(r2.position.y, &buf[4]);
			encode_uint32(r2.size.x, &buf[8]);
			encode_uint32(r2.size.y, &buf[12]);
		}
		*r_len += 4 * 4;
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_VECTOR3:
	{
		if (buf)
		{
			godot_vector3 v = godot_variant_as_vector3(p_variant);
			// FIXME(tobe) missing accessors?! let's hope the memory layout matches?
			encode_real(godot_quat_get_x((godot_quat *)&v), &buf[0]);
			encode_real(godot_quat_get_y((godot_quat *)&v), &buf[sizeof(real_t)]);
			encode_real(godot_quat_get_z((godot_quat *)&v), &buf[sizeof(real_t) * 2]);
		}

		*r_len += 3 * sizeof(real_t);
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_VECTOR3I:
	{
		if (buf)
		{
			Vector3i v3 = p_variant;
			encode_uint32(v3.x, &buf[0]);
			encode_uint32(v3.y, &buf[4]);
			encode_uint32(v3.z, &buf[8]);
		}

		*r_len += 3 * 4;
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_TRANSFORM2D:
	{
		if (buf)
		{
			ERR_FAIL_COND_V(true, ERR_BUG);
#if TODO
			godot_transform2d transform = godot_variant_as_transform2d(p_variant);
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 2; j++)
				{
					memcpy(&buf[(i * 2 + j) * sizeof(real_t)], &val.columns[i][j], sizeof(real_t));
				}
			}
#endif
		}

		*r_len += 6 * sizeof(real_t);
	}
	break;
	case GODOT_VARIANT_TYPE_PLANE:
	{
		if (buf)
		{
			godot_plane p = godot_variant_as_plane(p_variant);
			godot_vector3 normal = godot_plane_get_normal(&p);
			// FIXME(tobe) missing accessors?! let's hope the memory layout matches?
			encode_real(godot_quat_get_x((godot_quat *)&normal), &buf[0]);
			encode_real(godot_quat_get_y((godot_quat *)&normal), &buf[sizeof(real_t)]);
			encode_real(godot_quat_get_z((godot_quat *)&normal), &buf[sizeof(real_t) * 2]);
			encode_real(godot_plane_get_d(&p), &buf[sizeof(real_t) * 3]);
		}

		*r_len += 4 * sizeof(real_t);
	}
	break;
	case GODOT_VARIANT_TYPE_QUAT:
	{
		if (buf)
		{
			godot_quat q = godot_variant_as_quat(p_variant);
			encode_real(godot_quat_get_x(&q), &buf[0]);
			encode_real(godot_quat_get_y(&q), &buf[sizeof(real_t)]);
			encode_real(godot_quat_get_z(&q), &buf[sizeof(real_t) * 2]);
			encode_real(godot_quat_get_w(&q), &buf[sizeof(real_t) * 3]);
		}

		*r_len += 4 * sizeof(real_t);
	}
	break;
	case GODOT_VARIANT_TYPE_AABB:
	{
		if (buf)
		{
			godot_aabb aabb = godot_variant_as_aabb(p_variant);
			godot_vector3 position = godot_aabb_get_position(&aabb);
			godot_vector3 size = godot_aabb_get_size(&aabb);

			encode_real(godot_quat_get_x((godot_quat *)&position), &buf[0]);
			encode_real(godot_quat_get_y((godot_quat *)&position), &buf[sizeof(real_t)]);
			encode_real(godot_quat_get_z((godot_quat *)&position), &buf[sizeof(real_t) * 2]);
			encode_real(godot_quat_get_x((godot_quat *)&size), &buf[sizeof(real_t) * 3]);
			encode_real(godot_quat_get_y((godot_quat *)&size), &buf[sizeof(real_t) * 4]);
			encode_real(godot_quat_get_z((godot_quat *)&size), &buf[sizeof(real_t) * 5]);
		}

		*r_len += 6 * sizeof(real_t);
	}
	break;
	case GODOT_VARIANT_TYPE_BASIS:
	{
		if (buf)
		{
			ERR_FAIL_COND_V(true, ERR_BUG);
#if TODO
			Basis val = p_variant;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					memcpy(&buf[(i * 3 + j) * sizeof(real_t)], &val.rows[i][j], sizeof(real_t));
				}
			}
#endif
		}

		*r_len += 9 * sizeof(real_t);
	}
	break;
	case GODOT_VARIANT_TYPE_TRANSFORM:
	{
		if (buf)
		{
			ERR_FAIL_COND_V(true, ERR_BUG);
#if TODO
			Transform3D val = p_variant;
			for (int i = 0; i < 3; i++)
			{
				for (int j = 0; j < 3; j++)
				{
					memcpy(&buf[(i * 3 + j) * sizeof(real_t)], &val.basis.rows[i][j], sizeof(real_t));
				}
			}

			encode_real(val.origin.x, &buf[sizeof(real_t) * 9]);
			encode_real(val.origin.y, &buf[sizeof(real_t) * 10]);
			encode_real(val.origin.z, &buf[sizeof(real_t) * 11]);
#endif
		}

		*r_len += 12 * sizeof(real_t);
	}
	break;

	// misc types
	case GODOT_VARIANT_TYPE_COLOR:
	{
		if (buf)
		{
			godot_color c = godot_variant_as_color(p_variant);
			encode_float(godot_color_get_r(&c), &buf[0]);
			encode_float(godot_color_get_g(&c), &buf[4]);
			encode_float(godot_color_get_b(&c), &buf[8]);
			encode_float(godot_color_get_a(&c), &buf[12]);
		}

		*r_len += 4 * 4; // Colors should always be in single-precision.
	}
	break;
	case GODOT_VARIANT_TYPE_RID:
	{
		if (buf)
		{
			godot_rid rid = godot_variant_as_rid(p_variant);
			encode_uint64(godot_rid_get_id(&rid), buf);
		}
		*r_len += 8;
	}
	break;
	case GODOT_VARIANT_TYPE_OBJECT:
	{
		ERR_FAIL_COND_V(true, ERR_BUG);
#if TODO
		if (p_full_objects)
		{
			Object *obj = p_variant;
			if (!obj)
			{
				if (buf)
				{
					encode_uint32(0, buf);
				}
				*r_len += 4;
			}
			else
			{
				_encode_string(obj->get_class(), buf, r_len);

				List<PropertyInfo> props;
				obj->get_property_list(&props);

				int pc = 0;
				for (const PropertyInfo &E : props)
				{
					if (!(E.usage & PROPERTY_USAGE_STORAGE))
					{
						continue;
					}
					pc++;
				}

				if (buf)
				{
					encode_uint32(pc, buf);
					buf += 4;
				}

				*r_len += 4;

				for (const PropertyInfo &E : props)
				{
					if (!(E.usage & PROPERTY_USAGE_STORAGE))
					{
						continue;
					}

					_encode_string(E.name, buf, *r_len);

					int len;
					Error err = encode_variant(obj->get(E.name), buf, len, p_full_objects, p_depth + 1);
					ERR_FAIL_COND_V(err, err);
					ERR_FAIL_COND_V(len % 4, ERR_BUG);
					*r_len += len;
					if (buf)
					{
						buf += len;
					}
				}
			}
		}
		else
		{
			if (buf)
			{
				Object *obj = p_variant.get_validated_object();
				ObjectID id;
				if (obj)
				{
					id = obj->get_instance_id();
				}

				encode_uint64(id, buf);
			}

			*r_len += 8;
		}
#endif
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_CALLABLE:
	{
	}
	break;
#endif
#if MISSING
	case GODOT_VARIANT_TYPE_SIGNAL:
	{
		Signal signal = p_variant;

		_encode_string(signal.get_name(), buf, r_len);

		if (buf)
		{
			encode_uint64(signal.get_object_id(), buf);
		}
		*r_len += 8;
	}
	break;
#endif
	case GODOT_VARIANT_TYPE_DICTIONARY:
	{
		godot_dictionary d = godot_variant_as_dictionary(p_variant);

		if (buf)
		{
			encode_uint32((uint32_t)godot_dictionary_size(&d), buf);
			buf += 4;
		}
		*r_len += 4;

		godot_array keys = godot_dictionary_keys(&d);
		for (godot_int i = 0; i < godot_array_size(&keys); i++)
		{
			godot_variant key = godot_array_get(&keys, i);
			godot_variant value = godot_dictionary_get(&d, &key);

			int len;
			Error err = encode_variant(&key, buf, len, p_full_objects, p_depth + 1);
			ERR_FAIL_COND_V(err, err);
			ERR_FAIL_COND_V(len % 4, ERR_BUG);
			*r_len += len;
			if (buf)
			{
				buf += len;
			}

			err = encode_variant(&value, buf, len, p_full_objects, p_depth + 1);
			ERR_FAIL_COND_V(err, err);
			ERR_FAIL_COND_V(len % 4, ERR_BUG);
			*r_len += len;
			if (buf)
			{
				buf += len;
			}
		}
	}
	break;
	case GODOT_VARIANT_TYPE_ARRAY:
	{
		godot_array v = godot_variant_as_array(p_variant);

		if (buf)
		{
			encode_uint32((uint32_t)godot_array_size(&v), buf);
			buf += 4;
		}

		*r_len += 4;

		for (godot_int i = 0; i < godot_array_size(&v); i++)
		{
			godot_variant val = godot_array_get(&v, i);
			int len;
			Error err = encode_variant(&val, buf, len, p_full_objects, p_depth + 1);
			ERR_FAIL_COND_V(err, err);
			ERR_FAIL_COND_V(len % 4, ERR_BUG);
			*r_len += len;
			if (buf)
			{
				buf += len;
			}
		}
	}
	break;
	// arrays
	case GODOT_VARIANT_TYPE_POOL_BYTE_ARRAY:
	{
		godot_pool_byte_array data = godot_variant_as_pool_byte_array(p_variant);
		int datalen = godot_pool_byte_array_size(&data);
		int datasize = sizeof(uint8_t);

		if (buf)
		{
			encode_uint32(datalen, buf);
			buf += 4;
			godot_pool_byte_array_read_access *access = godot_pool_byte_array_read(&data);
			const uint8_t *r = godot_pool_byte_array_read_access_ptr(access);
			memcpy(buf, &r[0], datalen * datasize);
			buf += datalen * datasize;
			godot_pool_byte_array_read_access_destroy(access);
		}

		*r_len += 4 + datalen * datasize;
		while ((*r_len) % 4)
		{
			(*r_len)++;
			if (buf)
			{
				*(buf++) = 0;
			}
		}
	}
	break;
#if MISSING
	case GODOT_VARIANT_TYPE_PACKED_INT32_ARRAY:
	{
		Vector<int32_t> data = p_variant;
		int datalen = data.size();
		int datasize = sizeof(int32_t);

		if (buf)
		{
			encode_uint32(datalen, buf);
			buf += 4;
			const int32_t *r = data.ptr();
			for (int32_t i = 0; i < datalen; i++)
			{
				encode_uint32(r[i], &buf[i * datasize]);
			}
		}

		*r_len += 4 + datalen * datasize;
	}
	break;
	case GODOT_VARIANT_TYPE_PACKED_INT64_ARRAY:
	{
		Vector<int64_t> data = p_variant;
		int datalen = data.size();
		int datasize = sizeof(int64_t);

		if (buf)
		{
			encode_uint32(datalen, buf);
			buf += 4;
			const int64_t *r = data.ptr();
			for (int64_t i = 0; i < datalen; i++)
			{
				encode_uint64(r[i], &buf[i * datasize]);
			}
		}

		*r_len += 4 + datalen * datasize;
	}
	break;
	case GODOT_VARIANT_TYPE_PACKED_FLOAT32_ARRAY:
	{
		Vector<float> data = p_variant;
		int datalen = data.size();
		int datasize = sizeof(float);

		if (buf)
		{
			encode_uint32(datalen, buf);
			buf += 4;
			const float *r = data.ptr();
			for (int i = 0; i < datalen; i++)
			{
				encode_float(r[i], &buf[i * datasize]);
			}
		}

		*r_len += 4 + datalen * datasize;
	}
	break;
	case GODOT_VARIANT_TYPE_PACKED_FLOAT64_ARRAY:
	{
		Vector<double> data = p_variant;
		int datalen = data.size();
		int datasize = sizeof(double);

		if (buf)
		{
			encode_uint32(datalen, buf);
			buf += 4;
			const double *r = data.ptr();
			for (int i = 0; i < datalen; i++)
			{
				encode_double(r[i], &buf[i * datasize]);
			}
		}

		*r_len += 4 + datalen * datasize;
	}
	break;
	case GODOT_VARIANT_TYPE_PACKED_STRING_ARRAY:
	{
		Vector<String> data = p_variant;
		int len = data.size();

		if (buf)
		{
			encode_uint32(len, buf);
			buf += 4;
		}

		*r_len += 4;

		for (int i = 0; i < len; i++)
		{
			CharString utf8 = data.get(i).utf8();

			if (buf)
			{
				encode_uint32(utf8.length() + 1, buf);
				buf += 4;
				memcpy(buf, utf8.get_data(), utf8.length() + 1);
				buf += utf8.length() + 1;
			}

			*r_len += 4 + utf8.length() + 1;
			while ((*r_len) % 4)
			{
				(*r_len)++; // pad
				if (buf)
				{
					*(buf++) = 0;
				}
			}
		}
	}
	break;
	case GODOT_VARIANT_TYPE_PACKED_VECTOR2_ARRAY:
	{
		Vector<Vector2> data = p_variant;
		int len = data.size();

		if (buf)
		{
			encode_uint32(len, buf);
			buf += 4;
		}

		*r_len += 4;

		if (buf)
		{
			for (int i = 0; i < len; i++)
			{
				Vector2 v = data.get(i);

				encode_real(v.x, &buf[0]);
				encode_real(v.y, &buf[sizeof(real_t)]);
				buf += sizeof(real_t) * 2;
			}
		}

		*r_len += sizeof(real_t) * 2 * len;
	}
	break;
	case GODOT_VARIANT_TYPE_PACKED_VECTOR3_ARRAY:
	{
		Vector<Vector3> data = p_variant;
		int len = data.size();

		if (buf)
		{
			encode_uint32(len, buf);
			buf += 4;
		}

		*r_len += 4;

		if (buf)
		{
			for (int i = 0; i < len; i++)
			{
				Vector3 v = data.get(i);

				encode_real(v.x, &buf[0]);
				encode_real(v.y, &buf[sizeof(real_t)]);
				encode_real(v.z, &buf[sizeof(real_t) * 2]);
				buf += sizeof(real_t) * 3;
			}
		}

		*r_len += sizeof(real_t) * 3 * len;
	}
	break;
	case GODOT_VARIANT_TYPE_PACKED_COLOR_ARRAY:
	{
		Vector<Color> data = p_variant;
		int len = data.size();

		if (buf)
		{
			encode_uint32(len, buf);
			buf += 4;
		}

		*r_len += 4;

		if (buf)
		{
			for (int i = 0; i < len; i++)
			{
				Color c = data.get(i);

				encode_float(c.r, &buf[0]);
				encode_float(c.g, &buf[4]);
				encode_float(c.b, &buf[8]);
				encode_float(c.a, &buf[12]);
				buf += 4 * 4; // Colors should always be in single-precision.
			}
		}

		*r_len += 4 * 4 * len;
	}
	break;
#endif
	default:
	{
		ERR_FAIL_COND_V(true, ERR_BUG);
	}
	}

	return OK;
}
