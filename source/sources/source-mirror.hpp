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

#pragma once
#include <condition_variable>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>
#include "gfx/gfx-source-texture.hpp"
#include "obs/gs/gs-rendertarget.hpp"
#include "obs/gs/gs-sampler.hpp"
#include "obs/obs-source.hpp"
#include "plugin.hpp"

// OBS
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)
#endif
#include <obs-source.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace source {
	namespace mirror {
		class mirror_factory {
			obs_source_info _source_info;

			public: // Singleton
			static void                            initialize();
			static void                            finalize();
			static std::shared_ptr<mirror_factory> get();

			public:
			mirror_factory();
			~mirror_factory();

			static const char*       get_name(void*) noexcept;
			static void              get_defaults(obs_data_t*) noexcept;
			static bool              modified_properties(obs_properties_t*, obs_property_t*, obs_data_t*) noexcept;
			static obs_properties_t* get_properties(void*) noexcept;

			static void* create(obs_data_t*, obs_source_t*) noexcept;
			static void  destroy(void*) noexcept;

			static uint32_t get_width(void*) noexcept;
			static uint32_t get_height(void*) noexcept;

			static void update(void*, obs_data_t*) noexcept;
			static void activate(void*) noexcept;
			static void deactivate(void*) noexcept;
			static void video_tick(void*, float) noexcept;
			static void video_render(void*, gs_effect_t*) noexcept;
			static void enum_active_sources(void*, obs_source_enum_proc_t, void*) noexcept;
			static void load(void*, obs_data_t*) noexcept;
			static void save(void*, obs_data_t*) noexcept;
		};

		struct mirror_audio_data {
			obs_source_audio                  audio = {};
			std::vector<std::vector<float_t>> data;
		};

		class mirror_instance {
			obs_source_t* _self;
			bool          _active;
			float_t       _tick;

			// Video Rendering
			std::shared_ptr<obs::source>         _scene;
			std::shared_ptr<gfx::source_texture> _scene_texture_renderer;
			std::shared_ptr<gs::texture>         _scene_texture;
			bool                                 _scene_rendered;
			uint32_t                             _rescale_alignment;

			// Rescaling
			bool            _rescale_enabled;
			uint32_t        _rescale_width;
			uint32_t        _rescale_height;
			bool            _rescale_keep_orig_size;
			obs_scale_type  _rescale_type;
			obs_bounds_type _rescale_bounds;

			// Audio Rendering
			bool                                           _audio_enabled;
			std::condition_variable                        _audio_notify;
			std::thread                                    _audio_thread;
			bool                                           _audio_kill_thread;
			bool                                           _audio_have_output;
			std::mutex                                     _audio_lock_outputter;
			std::mutex                                     _audio_lock_capturer;
			std::queue<std::shared_ptr<mirror_audio_data>> _audio_data_queue;
			std::queue<std::shared_ptr<mirror_audio_data>> _audio_data_free_queue;
			speaker_layout                                 _audio_layout;

			// Input
			std::shared_ptr<obs::source> _source;
			obs_sceneitem_t*             _source_item;
			std::string                  _source_name;

			private:
			void release_input();
			void acquire_input(std::string source_name);

			public:
			mirror_instance(obs_data_t*, obs_source_t*);
			~mirror_instance();

			uint32_t get_width();
			uint32_t get_height();

			void update(obs_data_t*);
			void activate();
			void deactivate();
			void video_tick(float);
			void video_render(gs_effect_t*);
			void audio_output_cb() noexcept;
			void enum_active_sources(obs_source_enum_proc_t, void*);
			void load(obs_data_t*);
			void save(obs_data_t*);

			void on_source_rename(obs::source* source, std::string new_name, std::string old_name);
			void on_audio_data(obs::source* source, const audio_data* audio, bool muted);
		};
	} // namespace mirror
};    // namespace source
