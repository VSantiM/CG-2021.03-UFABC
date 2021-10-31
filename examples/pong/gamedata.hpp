#ifndef GAMEDATA_HPP_
#define GAMEDATA_HPP_

#include <bitset>

enum class Input { DownR, UpR, Down, Up };
enum class State { Playing, Score, VermelhoWin, AzulWin };
enum class BallState { Playing, BallStop };

struct GameData {
  State m_state{State::Playing};
  BallState m_ballState{BallState::BallStop};
  std::bitset<4> m_input;  // [fire, up, down, left, right]
};

#endif