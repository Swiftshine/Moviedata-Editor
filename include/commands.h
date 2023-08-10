#pragma once
typedef unsigned int u32;
typedef unsigned char u8;
struct Vec3u {
	u32 x;
	u32 y;
	u32 z;
};

/*
* 
moviedata/moveiesound documentation by CLF78, though I clarified a few things.

moviedata and moviesound are headerless sets of command/parameter pairs.
- Each command is 0x24 bytes in size, regardless of how much of it is used, if at all.
	-> moviedata will NEVER use command 3, and moviesound will ONLY use command 3.
- The amount of commands (regardless of type) per file is hardcoded to 500.
	-> 0x24 (36) byte commands * 500 == 0x4650 (18,000), or the size of the file.
- All values in the structure are 4 bytes long.

Base command structure:
0x0 - Frame at which the command must be executed.
0x4 - Command type.

Command 1 goes to the next scene (this is managed by dMdActorSuper_c)
0x8 - Scene ID (value 0 to 5).

Command 2 moves the camera (this is partially managed by dMdCamera_c).
0x8-0xC-0x10 - Starting position (in integer form)
0x14-0x18-0x1C - Ending position (in integer form)
0x20 - Duration of the movement. (units currently unknown)

Command 3 plays a sound (this command is exclusive to moviesound, hinting that the two files used to be merged).
0x8 - Sound ID.

Command 4 spawns an effect.
0x8 - Effect ID (this is an index to an hardcoded array at 8098D0E0)
0xC-0x10-0x14 - Effect Position (in integer form)

Command 5 ends the cutscene.
No parameters.

Command 6 shakes the screen.
0x8 - Shake Type (this is passed as is to dQuake_c)
*/

enum CommandType {
	NONE,
	SWITCH_SCENE,
	MOVE_CAMERA,
	PLAY_SOUND,
	SPAWN_EFFECT,
	END_SCENE,
	SHAKE_SCREEN
};


struct Command_None {
	u32 frame;
	u32 commandType; // 0
	u8	padding[0x1C];
};

struct Command_SceneSwitch {
	u32 frame;
	u32 commandType; // 1
	
	u32 targetScene;
	u8	padding[0x18];
};


struct Command_MoveCamera {
	u32 frame;
	u32 commandType; // 2

	Vec3u startPos;
	Vec3u endPos;
	u32 duration;
};

struct Command_PlaySound {
	u32 frame;
	u32 commandType; // 3

	u32 soundID;
	u8 padding[0x18];
};

struct Command_SpawnEffect {
	u32 frame;
	u32 commandType;

	u32 effectID;
	u8 padding[0x18];
};

struct Command_EndCutscene {
	u32 frame;
	u32 commandType;

	u8 padding[0x1C];
};

struct Command_ShakeScreen {
	u32 frame;
	u32 commandType;

	u32 shakeType;
	u8 padding[0x18];
};