/* 
	Copyright 2010 OpenRTMFP
 
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License received along this program for more
	details (or else see http://www.gnu.org/licenses/).

	This file is a part of Cumulus.
*/

#pragma once

#include "Cumulus.h"
#include "PacketReader.h"
#include "Peer.h"
#include "FlowWriter.h"
#include "Invoker.h"

namespace Cumulus {

class Packet;
class Fragment;
class Flow {
public:
	Flow(Poco::UInt64 id,const std::string& signature,const std::string& name,Peer& peer,Invoker& invoker,BandWriter& band);
	virtual ~Flow();

	const Poco::UInt64		id;

	virtual void		fragmentHandler(Poco::UInt64 stage,Poco::UInt64 deltaNAck,PacketReader& fragment,Poco::UInt8 flags);
	
	void				commit();

	void				fail(const std::string& error);

	bool				consumed();
	const std::string&	error();
	void				complete();

protected:
	virtual void messageHandler(const std::string& name,AMFReader& message);
	virtual void rawHandler(Poco::UInt8 type,PacketReader& data);
	virtual void audioHandler(PacketReader& packet);
	virtual void videoHandler(PacketReader& packet);

	virtual void lostFragmentsHandler(Poco::UInt32 count);

	Peer&					peer;
	FlowWriter&				writer;
	Invoker&				invoker;
	const Poco::UInt64		stage;
	
private:
	void		 fragmentSortedHandler(Poco::UInt64 stage,PacketReader& fragment,Poco::UInt8 flags);
	
	virtual void		commitHandler();
	Message::Type		unpack(PacketReader& reader);

	bool				_completed;
	BandWriter&			_band;
	std::string			_error;

	// Receiving
	Packet*								_pPacket;
	std::map<Poco::UInt64,Fragment*>	_fragments;
};

inline const std::string& Flow::error() {
	return _error;
}

inline bool Flow::consumed() {
	return _completed;
}

inline void Flow::commitHandler() {
}

} // namespace Cumulus
