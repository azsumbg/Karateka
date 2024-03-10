#pragma once

#ifdef KARENGINE_EXPORTS
#define KARENGINE_API _declspec(dllexport)
#else
#define KARENGINE_API _declspec(dllimport)
#endif

constexpr float scr_width = 816.0f; // SCREEN 800 x 600
constexpr float scr_height = 639.0f;

constexpr int DL_OK = 0;
constexpr int DL_FAIL = 1;
constexpr int DL_UNSUPPORTED = 2;

enum class types { no_type = 0, hero = 1, evil1 = 2, evil2 = 3, evil3 = 4, boss = 5 };
enum class states { no_state = 0, walk = 1, punch = 2, kick = 3, fall = 4 };
enum class dirs { stop = 0, left = 1, right = 2, up = 3, down = 4 };

struct HITPLOT
{
	float x = 0;
	float y = 0;
	float ex = 0;
	float ey = 0;
	int cooldown = 80;
};

class KARENGINE_API OBJECT
{
	protected:
		float width = 0;
		float height = 0;

		dirs my_dir = dirs::stop;

		OBJECT(float _x, float _y, float _width, float _height)
		{
			x = _x;
			y = _y;
			width = _width;
			height = _height;
			ex = x + width;
			ey = y + height;
		}

	public:
		float x = 0;
		float y = 0;
		float ex = 0;
		float ey = 0;

		virtual ~OBJECT() {};
		virtual void Release()
		{
			delete this;
		}

		static OBJECT* CreateObject(float _objectx, float _objecty, float _object_width, float _object_height);
		
		void SetEdges()
		{
			ex = x + width;
			ey = y + height;
		}
		void NewDims(float _new_width, float _new_height)
		{
			width = _new_width;
			height = _new_height;
			ex = x + width;
			ey = y + height;
		}
		
		void SetDir(dirs _new_dir)
		{
			my_dir = _new_dir;
		}
		dirs GetDir() const
		{
			return my_dir;
		}
		float GetWidth() const
		{
			return width;
		}
		float GetHeight() const
		{
			return height;
		}
};

class KARENGINE_API PERSON :public OBJECT
{
	protected:
		types my_type = types::no_type;
		states my_state = states::no_state;
		
		int state_cooldown = 20;
		int state_stage = 0;
		int attack = 0;
		
	public:
		int lifes = 0;
		float speed = 0.8f;
		float hit_distance = 10.0f;

		HITPLOT HitArea;

		PERSON(float _where_x, float _where_y, float _Pwidth, float _pHeight, types _type) 
			:OBJECT(_where_x, _where_y, _Pwidth, _pHeight)
		{
			my_type = _type;
			my_state = states::walk;
		}
		virtual ~PERSON() {};
		
		types GetType() const
		{
			return my_type;
		}
		
		void SetAttack(int _strenght)
		{
			attack = _strenght;
		}
		int GetHit()
		{
			if (my_state == states::walk || my_state == states::no_state)return 0;
			return attack;
		}
		
		void SetHitArea(float _where_x, float _where_y)
		{
			HitArea.x = _where_x;
			HitArea.y = _where_y;
			HitArea.ex = _where_x + 15.0f;
			HitArea.ey = _where_y + 13.0f;
			HitArea.cooldown = 50;
		}
		int HitAreaCooldown()
		{
			HitArea.cooldown--;
			return HitArea.cooldown;
		}

		states GetState() const
		{
			return my_state;
		}
		BOOL Move(float _gear = 1.0f) 
		{
			if (my_state != states::walk)return DL_FAIL;

			speed *= _gear;

			switch (my_dir)
			{
			case dirs::left:
				if (ex + speed > scr_width)return DL_FAIL;
				x += speed;
				SetEdges();
				return DL_OK;
				break;

			case dirs::right:
				if (x - speed < 0)return DL_FAIL;
				x -= speed;
				SetEdges();
				return DL_OK;
				break;
			}
			return DL_UNSUPPORTED;
		}

		virtual void Release() = 0;
		virtual int GetStateFrame(states _what_stage) = 0;
		BOOL SetState(states to_what)
		{
			if (my_state != states::walk && my_state != states::no_state)return DL_FAIL;
			my_state = to_what;
			state_stage = 0;
			if (my_state == states::fall)
			{
				switch (my_type)
				{
				case types::hero:
					NewDims(100.0f, 27.0f);
					break;

				case types::evil1:
					NewDims(100.0f, 33.0f);
					break;

				case types::evil2:
					NewDims(100.0f, 92.0f);
					break;

				case types::evil3:
					NewDims(100.0f, 72.0f);
					break;

				case types::boss:
					NewDims(100.0f, 27.0f);
					break;
				}
			}
			return DL_OK;
		}
};

typedef OBJECT* objPtr;
typedef PERSON* perPtr;

extern KARENGINE_API perPtr PersFactory(float _where_x, float _where_y, types _what_to_create);