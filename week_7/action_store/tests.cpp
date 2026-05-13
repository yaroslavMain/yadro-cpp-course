#include <gtest/gtest.h>
#include <iostream>
#include <vector>

#include "action_store.h"

struct GameContext {
  std::string title = "Tanks";
  double height = 160;
  double width = 160;
  std::ostringstream os;
};

struct Tank {
  std::string name;

  void execute(GameContext &ctx) const {
    ctx.os << "Tank " << name << " executing!" << "\n";
  }
};

struct SoundAction {
  std::string sound_file;

  void execute(GameContext &ctx) const {
    ctx.os << "Sound file " << sound_file << " executing!" << "\n";
  }
};

TEST(ActionStoreTests, Basic) {
  std::vector<Action<GameContext>> vec;

  vec.emplace_back(Tank{"T1"});
  vec.emplace_back(SoundAction{"SoundFile"});
  vec.emplace_back(Tank{"T2"});

  GameContext ctx;
  ctx.os = std::ostringstream();
  for (auto &v : vec) {
    v(ctx);
  }

  const char *string_res = "Tank T1 executing!\nSound file SoundFile "
                           "executing!\nTank T2 executing!\n";
  ASSERT_EQ(ctx.os.str(), string_res);
}

TEST(ActionStoreTests, MoveSemantics) {
  GameContext ctx;

  Action<GameContext> a1(Tank{"Tiger"});
  Action<GameContext> a2 = std::move(a1);

  ASSERT_FALSE(static_cast<bool>(a1));
  ASSERT_TRUE(static_cast<bool>(a2));

  a2(ctx);
  ASSERT_EQ(ctx.os.str(), "Tank Tiger executing!\n");

  Action<GameContext> a3;
  a3 = std::move(a2);
  ASSERT_FALSE(static_cast<bool>(a2));
}

TEST(ActionStoreTests, EmptyActionCall) {
  Action<GameContext> empty_action;
  ASSERT_FALSE(static_cast<bool>(empty_action));

  GameContext ctx;
  EXPECT_NO_THROW(empty_action(ctx));
}
