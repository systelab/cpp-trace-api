#pragma once


namespace systelab { namespace trace {
	
	class IFileAgent
	{
	public:
		virtual ~IFileAgent() = default;

		virtual bool isEnabled() const = 0;
		virtual void enable(bool) = 0;

		virtual void flush() = 0;
		virtual void backup() = 0;
	};

}}

