#pragma once

#include <array>
#include <cstdint>

namespace lshl::soup_bin {

using std::array;
using std::uint16_t;
using std::uint8_t;
using packet_length_t = uint16_t;

enum class PacketType : char {
  /// @brief Sent from the server to the client in lieu of a data packet to
  /// indicate that the connection is still alive.
  SERVER_HEARTBEAT = 'H',

  /// @brief Sent from the client to the server in response to a Server Heartbeat
  /// Packet to indicate that the connection is still alive.
  CLIENT_HEARTBEAT = 'R',

  /// @brief Sent from the server to the client at the end of the session to
  /// indicate that the session is over and the server is about to disconnect.
  END_OF_SESSION = 'Z',

  /// @brief Sent by either the client or the server to transmit a debug message.
  /// The payload is a variable-length text string.
  DEBUG = '+',

  /// @brief Sent from the client to the server to initiate a session.
  LOGIN_REQUEST = 'L',

  /// @brief Sent from the server to the client to indicate that the Login Request
  /// has been accepted.
  LOGIN_ACCEPTED = 'A',

  /// @brief Sent from the server to the client to indicate that the Login Request
  /// has been rejected.
  LOGIN_REJECTED = 'J',

  /// @brief Sent from the client to the server to gracefully terminate the
  /// session.
  LOGOUT_REQUEST = 'O',

  /// @brief A Sequenced Data Packet contains a message that is part of a
  /// sequenced stream of messages. The payload is a variable-length application
  /// message.
  SEQUENCED_DATA = 'S',

  /// @brief An Unsequenced Data Packet contains a message that is not part of a
  /// sequenced stream. The payload is a variable-length application message.
  UNSEQUENCED_DATA = 'U',
};

/// @brief Common header for all SoupBinTCP packets.
/// The Packet Length is the number of bytes following this field (i.e., Packet
/// Type + Payload). It is a 2-byte unsigned integer in network byte order
/// (big-endian).
struct PacketHeader {
  packet_length_t packet_length;
  PacketType packet_type;
} __attribute__((packed));

/// @brief Packet Type: 'L'
/// Sent from the client to the server to initiate a session.
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
struct LoginRequest {
  array<char, 6> username;
  array<char, 10> password;
  array<char, 10> requested_session;
  array<char, 20> requested_sequence_number;
} __attribute__((packed));
//NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

/// @brief Packet Type: 'A'
/// Sent from the server to the client to indicate that the Login Request has
/// been accepted.
//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
struct LoginAccepted {
  array<char, 10> session;
  array<char, 20> sequence_number;
} __attribute__((packed));
//NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

enum class RejectReason : char {
  /// @brief Not authorized to login.
  NOT_AUTHORIZED = 'A',

  /// @brief Session is not available.
  SESSION_NOT_AVAILABLE = 'S',
};

/// @brief Packet Type: 'J'
/// Sent from the server to the client to indicate that the Login Request has
/// been rejected.
struct LoginRejected {
  RejectReason reject_reason_code;
} __attribute__((packed));

/// @brief Packet Type: 'S'
/// A Sequenced Data Packet contains a message that is part of a sequenced
/// stream of messages. The payload is a variable-length application message
/// that follows this struct.
struct SequencedData {
  // The application message follows this header.
  // The length of the message is PacketHeader.packet_length -
  // sizeof(SequencedData). In this case, the message starts immediately after
  // the packet_type.
} __attribute__((packed));

/// @brief Packet Type: 'U'
/// An Unsequenced Data Packet contains a message that is not part of a
/// sequenced stream. The payload is a variable-length application message that
/// follows this struct.
struct UnsequencedData {
  // The application message follows this header.
  // The length of the message is PacketHeader.packet_length -
  // sizeof(UnsequencedData). In this case, the message starts immediately after
  // the packet_type.
} __attribute__((packed));

/// @brief Packet Type: '+'
/// Sent by either the client or the server to transmit a debug message.
/// The payload is a variable-length text string that follows this struct.
struct Debug {
  // The debug text follows this header.
  // The length of the text is PacketHeader.packet_length - sizeof(Debug).
  // In this case, the text starts immediately after the packet_type.
} __attribute__((packed));

/// @brief Packet Type: 'H'
/// Sent from the server to the client in lieu of a data packet to indicate that
/// the connection is still alive. No payload.
struct ServerHeartbeat {
} __attribute__((packed));

/// @brief Packet Type: 'R'
/// Sent from the client to the server in response to a Server Heartbeat Packet
/// to indicate that the connection is still alive. No payload.
struct ClientHeartbeat {
} __attribute__((packed));

/// @brief Packet Type: 'Z'
/// Sent from the server to the client at the end of the session to indicate
/// that the session is over and the server is about to disconnect. No payload.
struct EndOfSession {
} __attribute__((packed));

/// @brief Packet Type: 'O'
/// Sent from the client to the server to gracefully terminate the session.
/// No payload.
struct LogoutRequest {
} __attribute__((packed));

}  // namespace lshl::soup_bin