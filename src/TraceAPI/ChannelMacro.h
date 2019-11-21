#pragma once

#include "ChannelMgr.h"

#define TRACE_CHANNEL(channelName) \
	BOOST_LOG(systelab::trace::ChannelMgr::get().name(channelName).getLogger())

