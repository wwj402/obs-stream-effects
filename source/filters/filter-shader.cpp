/*
 * Modern effects for a modern Streamer
 * Copyright (C) 2017 Michael Fabian Dirks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#include "filter-shader.hpp"
#include <stdexcept>
#include "strings.hpp"
#include "utility.hpp"

#define ST "Filter.Shader"
#define ST_SCALE_LOCKED "Filter.Shader.Scale.Locked"
#define ST_SCALE_SCALE "Filter.Shader.Scale.Scale"
#define ST_SCALE_WIDTH "Filter.Shader.Scale.Width"
#define ST_SCALE_HEIGHT "Filter.Shader.Scale.Height"

static std::shared_ptr<filter::shader::shader_factory> factory_instance = nullptr;

void filter::shader::shader_factory::initialize()
{
	factory_instance = std::make_shared<filter::shader::shader_factory>();
}

void filter::shader::shader_factory::finalize()
{
	factory_instance.reset();
}

std::shared_ptr<filter::shader::shader_factory> filter::shader::shader_factory::get()
{
	return factory_instance;
}

static void get_defaults(obs_data_t* data)
{
	obs_data_set_default_string(data, S_SHADER_FILE, obs_module_file("shaders/filter/example.effect"));
	obs_data_set_default_string(data, S_SHADER_TECHNIQUE, "Draw");
	obs_data_set_default_bool(data, ST_SCALE_LOCKED, true);
	obs_data_set_default_double(data, ST_SCALE_SCALE, 1.0);
	obs_data_set_default_double(data, ST_SCALE_WIDTH, 1.0);
	obs_data_set_default_double(data, ST_SCALE_HEIGHT, 1.0);
}

filter::shader::shader_factory::shader_factory()
{
	memset(&_source_info, 0, sizeof(obs_source_info));
	_source_info.id           = "obs-stream-effects-filter-shader";
	_source_info.type         = OBS_SOURCE_TYPE_FILTER;
	_source_info.output_flags = OBS_SOURCE_VIDEO;
	_source_info.get_name     = [](void*) { return D_TRANSLATE(ST); };
	_source_info.get_defaults = get_defaults;

	_source_info.create = [](obs_data_t* data, obs_source_t* self) {
		try {
			return static_cast<void*>(new filter::shader::shader_instance(data, self));
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to create source, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to create source.");
		}
		return static_cast<void*>(nullptr);
	};
	_source_info.destroy = [](void* ptr) {
		try {
			delete reinterpret_cast<filter::shader::shader_instance*>(ptr);
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to delete source, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to delete source.");
		}
	};
	_source_info.get_properties = [](void* ptr) {
		obs_properties_t* pr = obs_properties_create();
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->properties(pr);
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to retrieve options, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to retrieve options.");
		}
		return pr;
	};
	_source_info.get_width = [](void* ptr) {
		try {
			if (ptr)
				return reinterpret_cast<filter::shader::shader_instance*>(ptr)->width();
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to retrieve width, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to retrieve width.");
		}
		return uint32_t(0);
	};
	_source_info.get_height = [](void* ptr) {
		try {
			if (ptr)
				return reinterpret_cast<filter::shader::shader_instance*>(ptr)->height();
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to retrieve height, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to retrieve height.");
		}
		return uint32_t(0);
	};
	_source_info.load = [](void* ptr, obs_data_t* data) {
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->load(data);
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to load, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to load.");
		}
	};
	_source_info.update = [](void* ptr, obs_data_t* data) {
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->update(data);
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to update, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to update.");
		}
	};
	_source_info.activate = [](void* ptr) {
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->activate();
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to activate, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to activate.");
		}
	};
	_source_info.deactivate = [](void* ptr) {
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->deactivate();
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to deactivate, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to deactivate.");
		}
	};
	_source_info.video_tick = [](void* ptr, float_t time) {
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->video_tick(time);
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to tick, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to tick.");
		}
	};
	_source_info.video_render = [](void* ptr, gs_effect_t* effect) {
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->video_render(effect);
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to render, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to render.");
		}
	};
	_source_info.enum_active_sources = [](void* ptr, obs_source_enum_proc_t enum_callback, void* param) {
		try {
			if (ptr)
				reinterpret_cast<filter::shader::shader_instance*>(ptr)->enum_active_sources(enum_callback, param);
		} catch (const std::exception& ex) {
			P_LOG_ERROR("<filter-shader> Failed to enumerate sources, error: %s", ex.what());
		} catch (...) {
			P_LOG_ERROR("<filter-shader> Failed to enumerate sources.");
		}
	};

	obs_register_source(&_source_info);
}

filter::shader::shader_factory::~shader_factory() {}

filter::shader::shader_instance::shader_instance(obs_data_t* data, obs_source_t* self)
	: _self(self), _active(true), _width(0), _height(0)
{
	_fx = std::make_shared<gfx::effect_source::effect_source>(self);
	_fx->set_valid_property_cb(std::bind(&filter::shader::shader_instance::valid_param, this, std::placeholders::_1));
	_fx->set_override_cb(std::bind(&filter::shader::shader_instance::override_param, this, std::placeholders::_1));

	_rt  = std::make_shared<gs::rendertarget>(GS_RGBA, GS_ZS_NONE);
	_rt2 = std::make_shared<gs::rendertarget>(GS_RGBA, GS_ZS_NONE);

	update(data);
}

filter::shader::shader_instance::~shader_instance() {}

uint32_t filter::shader::shader_instance::width()
{
	return _swidth;
}

uint32_t filter::shader::shader_instance::height()
{
	return _sheight;
}

bool modifiedcb3(obs_properties_t* props, obs_property_t* property, obs_data_t* settings) noexcept try {
	obs_property_set_visible(obs_properties_get(props, ST_SCALE_SCALE), obs_data_get_bool(settings, ST_SCALE_LOCKED));
	obs_property_set_visible(obs_properties_get(props, ST_SCALE_WIDTH), !obs_data_get_bool(settings, ST_SCALE_LOCKED));
	obs_property_set_visible(obs_properties_get(props, ST_SCALE_HEIGHT), !obs_data_get_bool(settings, ST_SCALE_LOCKED));
	return true;
} catch (const std::exception& ex) {
	P_LOG_ERROR("Unexpected exception in function '%s': %s.", __FUNCTION_NAME__, ex.what());
} catch (...) {
	P_LOG_ERROR("Unexpected exception in function '%s'.", __FUNCTION_NAME__);
	return false;
}

void filter::shader::shader_instance::properties(obs_properties_t* props)
{
	auto p = obs_properties_add_bool(props, ST_SCALE_LOCKED, D_TRANSLATE(ST_SCALE_LOCKED));
	obs_property_set_modified_callback(p, modifiedcb3);
	obs_properties_add_float(props, ST_SCALE_SCALE, D_TRANSLATE(ST_SCALE_SCALE), 0.01, 5.0, 0.01);
	obs_properties_add_float(props, ST_SCALE_WIDTH, D_TRANSLATE(ST_SCALE_WIDTH), 0.01, 5.0, 0.01);
	obs_properties_add_float(props, ST_SCALE_HEIGHT, D_TRANSLATE(ST_SCALE_HEIGHT), 0.01, 5.0, 0.01);

	_fx->properties(props);
}

void filter::shader::shader_instance::load(obs_data_t* data)
{
	update(data);
}

void filter::shader::shader_instance::update(obs_data_t* data)
{
	_scale_locked = obs_data_get_bool(data, ST_SCALE_LOCKED);
	if (_scale_locked) {
		_hscale = _wscale = obs_data_get_double(data, ST_SCALE_SCALE);
	} else {
		_wscale = obs_data_get_double(data, ST_SCALE_WIDTH);
		_hscale = obs_data_get_double(data, ST_SCALE_HEIGHT);
	}
	_fx->update(data);
}

void filter::shader::shader_instance::activate()
{
	_active = true;
}

void filter::shader::shader_instance::deactivate()
{
	_active = false;
}

bool filter::shader::shader_instance::valid_param(std::shared_ptr<gs::effect_parameter> param)
{
	if (strcmp(param->get_name().c_str(), "ImageSource") == 0)
		return false;
	if (strcmp(param->get_name().c_str(), "ImageSource_Size") == 0)
		return false;
	if (strcmp(param->get_name().c_str(), "ImageSource_Texel") == 0)
		return false;
	if (strcmp(param->get_name().c_str(), "ImageTarget_Size") == 0)
		return false;
	if (strcmp(param->get_name().c_str(), "ImageTarget_Texel") == 0)
		return false;
	return true;
}

void filter::shader::shader_instance::override_param(std::shared_ptr<gs::effect> effect)
{
	auto p_source       = effect->get_parameter("ImageSource");
	auto p_source_size  = effect->get_parameter("ImageSource_Size");
	auto p_source_texel = effect->get_parameter("ImageSource_Texel");
	if (p_source && (p_source->get_type() == gs::effect_parameter::type::Texture)) {
		p_source->set_texture(_rt_tex);
	}
	if (p_source_size && (p_source_size->get_type() == gs::effect_parameter::type::Float2)) {
		p_source_size->set_float2(_width, _height);
	}
	if (p_source_texel && (p_source_size->get_type() == gs::effect_parameter::type::Float2)) {
		p_source_texel->set_float2(1.0f / _width, 1.0f / _height);
	}

	auto p_target_size  = effect->get_parameter("ImageTarget_Size");
	auto p_target_texel = effect->get_parameter("ImageTarget_Texel");
	if (p_target_size && (p_target_size->get_type() == gs::effect_parameter::type::Float2)) {
		p_target_size->set_float2(_swidth, _sheight);
	}
	if (p_target_texel && (p_target_texel->get_type() == gs::effect_parameter::type::Float2)) {
		p_target_texel->set_float2(1.0f / _swidth, 1.0f / _sheight);
	}
}

void filter::shader::shader_instance::enum_active_sources(obs_source_enum_proc_t r, void* p)
{
	_fx->enum_active_sources(r, p);
}

void filter::shader::shader_instance::video_tick(float_t sec_since_last)
{
	obs_source_t* target = obs_filter_get_target(_self);

	{ // Update width and height.
		_width   = obs_source_get_base_width(target);
		_height  = obs_source_get_base_height(target);
		_swidth  = _width * _wscale;
		_sheight = _height * _hscale;
	}

	if (_fx->tick(sec_since_last)) {
		obs_data_t* data = obs_source_get_settings(_self);
		update(data);
		obs_data_release(data);
	}

	_rt_updated  = false;
	_rt2_updated = false;
}

void filter::shader::shader_instance::video_render(gs_effect_t* effect)
{
	// Grab initial values.
	obs_source_t* parent         = obs_filter_get_parent(_self);
	obs_source_t* target         = obs_filter_get_target(_self);
	gs_effect_t*  effect_default = obs_get_base_effect(obs_base_effect::OBS_EFFECT_DEFAULT);

	// Skip filter if anything is wrong.
	if (!_active || !parent || !target || !_width || !_height || !effect_default) {
		obs_source_skip_video_filter(_self);
		return;
	}

	if (!_rt_updated) {
		if (obs_source_process_filter_begin(_self, GS_RGBA, OBS_ALLOW_DIRECT_RENDERING)) {
			auto op = _rt->render(_width, _height);
			gs_blend_state_push();
			gs_reset_blend_state();
			gs_set_cull_mode(GS_NEITHER);
			gs_enable_color(true, true, true, true);
			gs_enable_blending(false);
			gs_enable_depth_test(false);
			gs_enable_stencil_test(false);
			gs_enable_stencil_write(false);
			gs_ortho(0, static_cast<float_t>(_width), 0, static_cast<float_t>(_height), -1., 1.);
			obs_source_process_filter_end(_self, effect_default, _width, _height);
			gs_blend_state_pop();
		}
		_rt_tex     = _rt->get_texture();
		_rt_updated = true;
	}

	if (!_rt2_updated) {
		try {
			auto op = _rt2->render(_swidth, _sheight);
			_fx->render();
		} catch (...) {
			obs_source_skip_video_filter(_self);
		}
		_rt2_tex     = _rt2->get_texture();
		_rt2_updated = true;
	}

	if (!_rt2_tex)
		return;

	gs_effect_t* ef = effect ? effect : effect_default;
	if (gs_eparam_t* prm = gs_effect_get_param_by_name(ef, "image"))
		gs_effect_set_texture(prm, _rt2_tex->get_object());

	while (gs_effect_loop(ef, "Draw")) {
		gs_draw_sprite(nullptr, 0, _swidth, _sheight);
	}
}
