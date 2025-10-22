#pragma once

#include <array>
#include <cstdint>
#include <variant>

namespace lshl::soup_bin::packet {

using std::array;
using std::size_t;
using std::uint16_t;
using std::uint8_t;
using std::variant;
using packet_length_t = uint16_t;

// helper type for the visitor, see std::visit documentation
template <class... Ts>
struct VisitorOverloads : Ts... {
  using Ts::operator()...;
};

enum class PacketType : char {
  DEBUG = '+',
  LOGIN_ACCEPTED = 'A',
  LOGIN_REJECTED = 'J',
  SEQUENCED_DATA = 'S',
  UNSEQUENCED_DATA = 'U',
  SERVER_HEARTBEAT = 'H',
  END_OF_SESSION = 'Z',
  LOGIN_REQUEST = 'L',
  CLIENT_HEARTBEAT = 'R',
  LOGOUT_REQUEST = 'O',
};

struct PacketHeader {
  packet_length_t packet_length;
  PacketType packet_type;
} __attribute__((packed));

template <packet_length_t N>
struct Debug {
  array<char, N> text;
} __attribute__((packed));

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

struct LoginRejected {
  RejectReason reject_reason_code;
} __attribute__((packed));

template <packet_length_t N>
struct SequencedData {
  array<uint8_t, N> message;
} __attribute__((packed));

template <packet_length_t N>
struct UnsequencedData {
  array<uint8_t, N> message;
} __attribute__((packed));

struct ServerHeartbeat {
} __attribute__((packed));

struct EndOfSession {
} __attribute__((packed));

//NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)
struct LoginRequest {
  array<char, 6> username;
  array<char, 10> password;
  array<char, 10> requested_session;
  array<char, 20> requested_sequence_number;
  array<char, 5> heartbeat_timeout_ms;
} __attribute__((packed));
//NOLINTEND(cppcoreguidelines-avoid-magic-numbers, readability-magic-numbers)

struct ClientHeartbeat {
} __attribute__((packed));

struct LogoutRequest {
} __attribute__((packed));

template <packet_length_t N>
using ServertPacket = variant<
    Debug<N>,
    LoginAccepted,
    LoginRejected,
    SequencedData<N>,
    UnsequencedData<N>,
    ServerHeartbeat,
    EndOfSession>;

template <packet_length_t N>
using ClientPacket = variant<Debug<N>, LoginRequest, UnsequencedData<N>, ClientHeartbeat, LogoutRequest>;

}  // namespace lshl::soup_bin::packet