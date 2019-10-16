/*
 * Modern effects for a modern Streamer
 * Copyright (C) 2018 Michael Fabian Dirks
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

#ifndef OBS_STREAM_EFFECTS_OBS_SOURCE_HPP
#define OBS_STREAM_EFFECTS_OBS_SOURCE_HPP
#pragma once

#include <cinttypes>
#include <string>
#include "util-event.hpp"

// OBS
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4201)
#endif
#include <obs.h>
#ifdef _MSC_VER
#pragma warning(pop)
#endif

namespace obs {
	class source {
		obs_source_t* _self;
		bool          _track_ownership = false;

		static void handle_destroy(void* p, calldata_t* calldata) noexcept;
		static void handle_remove(void* p, calldata_t* calldata) noexcept;
		static void handle_save(void* p, calldata_t* calldata) noexcept;
		static void handle_load(void* p, calldata_t* calldata) noexcept;
		static void handle_activate(void* p, calldata_t* calldata) noexcept;
		static void handle_deactivate(void* p, calldata_t* calldata) noexcept;
		static void handle_show(void* p, calldata_t* calldata) noexcept;
		static void handle_hide(void* p, calldata_t* calldata) noexcept;
		static void handle_enable(void* p, calldata_t* calldata) noexcept;
		static void handle_push_to_mute_changed(void* p, calldata_t* calldata) noexcept;
		static void handle_push_to_mute_delay(void* p, calldata_t* calldata) noexcept;
		static void handle_push_to_talk_changed(void* p, calldata_t* calldata) noexcept;
		static void handle_push_to_talk_delay(void* p, calldata_t* calldata) noexcept;
		static void handle_rename(void* p, calldata_t* calldata) noexcept;
		static void handle_update_properties(void* p, calldata_t* calldata) noexcept;
		static void handle_update_flags(void* p, calldata_t* calldata) noexcept;
		static void handle_mute(void* p, calldata_t* calldata) noexcept;
		static void handle_volume(void* p, calldata_t* calldata) noexcept;
		static void handle_audio_sync(void* p, calldata_t* calldata) noexcept;
		static void handle_audio_mixers(void* p, calldata_t* calldata) noexcept;
		static void handle_audio_data(void* p, obs_source_t* source, const audio_data* audio, bool muted) noexcept;
		static void handle_filter_add(void* p, calldata_t* calldata) noexcept;
		static void handle_filter_remove(void* p, calldata_t* calldata) noexcept;
		static void handle_reorder_filters(void* p, calldata_t* calldata) noexcept;
		static void handle_transition_start(void* p, calldata_t* calldata) noexcept;
		static void handle_transition_video_stop(void* p, calldata_t* calldata) noexcept;
		static void handle_transition_stop(void* p, calldata_t* calldata) noexcept;

		public:
		virtual ~source();

		source();

		source(std::string name, bool track_ownership = true, bool add_reference = true);

		source(obs_source_t* source, bool track_ownership = true, bool add_reference = false);

		public /*copy*/:
		source(source const& other);
		source& operator=(source const& other);

		public /*move*/:
		source(source&& other);
		source& operator=(source&& other);

		public:
		obs_source_type type();

		void* type_data();

		uint32_t width();
		uint32_t height();

		bool destroyed();

		public: // Unsafe Methods
		void clear();

		obs_source_t* get();

		public: // Events
		struct {
			// Destroy and Remove
			util::event<obs::source*> destroy;
			util::event<obs::source*> remove;

			// Saving, Loading and Update
			util::event<obs::source*> save;
			util::event<obs::source*> load;
			util::event<obs::source*> update_properties;

			// Activate, Deactivate
			util::event<obs::source*> activate;
			util::event<obs::source*> deactivate;

			// Show Hide
			util::event<obs::source*> show;
			util::event<obs::source*> hide;

			// Other
			util::event<obs::source*, bool>                     enable;
			util::event<obs::source*, std::string, std::string> rename;
			util::event<obs::source*, long long>                update_flags;

			// Hotkeys (PtM, PtT)
			util::event<obs::source*, bool>      push_to_mute_changed;
			util::event<obs::source*, long long> push_to_mute_delay;
			util::event<obs::source*, bool>      push_to_talk_changed;
			util::event<obs::source*, long long> push_to_talk_delay;

			// Audio
			util::event<obs::source*, bool>                    mute;
			util::event<obs::source*, double&>                 volume;
			util::event<obs::source*, long long&>              audio_sync;
			util::event<obs::source*, long long&>              audio_mixers;
			util::event<obs::source*, const audio_data*, bool> audio_data;

			// Filters
			util::event<obs::source*, obs_source_t*> filter_add;
			util::event<obs::source*, obs_source_t*> filter_remove;
			util::event<obs::source*>                reorder_filters;

			// Transition
			util::event<obs::source*> transition_start;
			util::event<obs::source*> transition_video_stop;
			util::event<obs::source*> transition_stop;
		} events;
	};
} // namespace obs

#endif
