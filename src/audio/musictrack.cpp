// Copyright (C) 2025 MatusGuy
// SPDX-License-Identifier: LGPL-3.0-only
#include "musictrack.hpp"

namespace cog2d {

MusicTrack::MusicTrack()
    : m_music(nullptr, &Mix_FreeMusic),
      m_metadata()
{
}

}  //namespace cog2d
