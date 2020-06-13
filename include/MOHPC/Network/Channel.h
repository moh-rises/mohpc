#pragma once

#include "../Global.h"
#include "Types.h"
#include <stdint.h>
#include "../Utilities/SharedPtr.h"

namespace MOHPC
{
	class IMessageStream;

	namespace Network
	{
		class IUdpSocket;

		class BadFragmentLengthException : public NetworkException
		{
		private:
			size_t length;

		public:
			BadFragmentLengthException(size_t inLength)
				: length(inLength)
			{}

			size_t getLength() const { return length; }
		};

		class INetchan
		{
		private:
			IUdpSocketPtr socket;

		public:
			INetchan(const IUdpSocketPtr& inSocket);
			virtual ~INetchan() = default;

			/** Read data from the socket to the stream. */
			virtual bool receive(IMessageStream& stream) = 0;

			/** Transmit data from stream to the socket. */
			virtual bool transmit(const netadr_t& to, IMessageStream& stream) = 0;

			/** Return the socket of this channel. */
			IUdpSocketPtr getSocket() const;

			/** Return the socket of this channel. */
			IUdpSocket* getRawSocket() const;

			/** Return the outgoing sequence number */
			virtual uint16_t getOutgoingSequence() const;
		};

		using INetchanPtr = SharedPtr<INetchan>;

		class Netchan : public INetchan
		{
		public:
			// max size of a network packet
			static constexpr size_t	MAX_PACKETLEN = 1400;

			static constexpr size_t	FRAGMENT_SIZE = (MAX_PACKETLEN - 100);
			// two ints and a short
			static constexpr size_t	PACKET_HEADER = 10;
			static constexpr size_t FRAGMENT_BIT = (1 << 31);

			static constexpr size_t MAX_MSGLEN = 49152;

		protected:
			uint16_t qport;
			uint16_t incomingSequence;
			uint32_t outgoingSequence;
			uint16_t dropped;
			uint16_t fragmentSequence;
			uint16_t fragmentLength;
			uint8_t* fragmentBuffer;
			bool unsentFragments;
			size_t unsentFragmentStart;
			size_t unsentLength;
			uint8_t* unsentBuffer;
			netadr_t from;

		public:
			Netchan(const IUdpSocketPtr& existingSocket, const netadr_t& from, uint16_t inQport);
			~Netchan();

			virtual bool receive(IMessageStream& stream);
			virtual bool transmit(const netadr_t& to, IMessageStream& stream);
			virtual uint16_t getOutgoingSequence() const override;

		private:
			void transmitNextFragment(const netadr_t& to);
		};

		class ConnectionlessChan : public INetchan
		{
		public:
			ConnectionlessChan();
			virtual bool receive(IMessageStream& stream);
			virtual bool transmit(const netadr_t& to, IMessageStream& stream);
		};
	}
}