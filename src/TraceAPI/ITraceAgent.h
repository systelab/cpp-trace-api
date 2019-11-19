#pragma once


namespace systelab { namespace trace {
	
	class ITraceAgent
	{
	public:
		virtual ~ITraceAgent() = default;

		virtual bool isEnabled() const = 0;
		virtual void enable(bool) = 0;

		virtual void backup() = 0;
	};

}}

