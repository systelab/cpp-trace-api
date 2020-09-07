#pragma once

#include "Channel.h"

#include <map>


namespace systelab { namespace trace {

	class ChannelMgr
	{
	private:
		ChannelMgr();
		virtual ~ChannelMgr();

	public:
		static ChannelMgr& get();
		Channel& name(const std::string&) const;

	protected:
		void createDummySink();

	private:
		mutable std::map<std::string, std::unique_ptr<Channel> > m_channels;
	};

}}
