#pragma once

#include "ChannelMgr.h"

#define TRACE_CHANNEL(channelName) \
	BOOST_LOG(systelab::trace::ChannelMgr::get().name(channelName).getLogger("", ""))

#define TRACE_CHANNEL_TAG(channelName, tag) \
	BOOST_LOG(systelab::trace::ChannelMgr::get().name(channelName).getLogger("", tag))

#define TRACE_CHANNEL_SEVERITY(channelName, severity) \
	BOOST_LOG(systelab::trace::ChannelMgr::get().name(channelName).getLogger(severity, ""))

#define TRACE_CHANNEL_SEVERITY_TAG(channelName, severity, tag) \
	BOOST_LOG(systelab::trace::ChannelMgr::get().name(channelName).getLogger(severity, tag))
