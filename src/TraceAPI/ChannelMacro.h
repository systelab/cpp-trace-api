#pragma once

#include "ChannelMgr.h"

#define TRACE_CHANNEL(channelName) \
	BOOST_LOG(systelab::trace::ChannelMgr::get().name(channelName).getLogger())

#define TRACE_CHANNEL_SEVERITY(channelName, severity) \
	BOOST_LOG(systelab::trace::ChannelMgr::get().name(channelName).getSeverityLogger(severity))
