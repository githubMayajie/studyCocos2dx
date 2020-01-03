/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "AppDelegate.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "cocos2d.h"
#include "scripting/lua-bindings/manual/lua_module_register.h"
#include "sqlite3/lsqlite3.h"

// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

#include <vector>
#include <cmath>
#include <cstdlib>
#include "ui/UILayout.h"
#define PI 3.141592653589793238
NS_CC_BEGIN
namespace TESTUI {

#define CREATE_INS(className) \
static className* create(){\
	auto ret = new (std::nothrow) className();\
	if (ret && ret->init())\
	{\
		ret->autorelease();\
		return ret;\
	}\
	delete ret; \
	return nullptr;\
}


	class InfiniteCycleView : public Node {
	public:
		CREATE_INS(InfiniteCycleView);

		InfiniteCycleView() 
			:mX(0.0f),
			mY(0.0f),
			mZ(0.0f),
			mRadius(100.0f),
			needCalcPos(false),
			offset(270.0f),
			maxChildIndex(0),
			maxChildAngle(360.0f),
			mMaxScale(1.0f),
			mMinScale(1.0f),
			mMaxAngle(0.0f),
			autoScrollTime(0.12f),
			touchRect()
		{}
		virtual ~InfiniteCycleView() {	}

		virtual bool init()override {
			if (!Node::init()) {
				return false;
			}

			auto touchListener = EventListenerTouchOneByOne::create();
			touchListener->onTouchBegan = std::bind(&InfiniteCycleView::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
			touchListener->onTouchMoved = std::bind(&InfiniteCycleView::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
			touchListener->onTouchEnded = std::bind(&InfiniteCycleView::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
			touchListener->onTouchCancelled = std::bind(&InfiniteCycleView::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
			return true;
		}

		virtual void setProp(const float x,const float y,const float z,const float radius,const Rect& touch) {
			mX = x;
			mY = y;
			mZ = z;
			mX = checkAngle(mX);
			mY = checkAngle(mY);
			mZ = checkAngle(mZ);
			mRadius = radius;
			touchRect = touch;
		}

		virtual void setOffsetProp(const float value) {
			offset += value;
			offset = checkAngle(offset);
		}

		float checkAngle(float value) {
			if (value < 0) {
				value = 360 - ((int)(0 - value) % 360);
			}
			else if (value > 360) {
				value = (int)(value) % 360;
			}
			return value;
		}

		virtual void setScaleProp(const float minScale, const float maxScale,const float maxAngle) {
			if (minScale < 0 || maxScale < 0) {
				return;
			}
			mMinScale = minScale;
			mMaxScale = maxScale;
			mMaxAngle = maxAngle;
		}

		virtual void updatePos() {
			needCalcPos = true;
			calcNewPos();
		}

		virtual void clearChildOrder() {
			childVec.clear();
		}

		virtual void addChildOrder(Node* child) {
			childVec.push_back(child);
		}

		virtual void initChildOrder() {
			childVec.clear();
			for (auto index = 0; index != _children.size(); index++) {
				childVec.push_back(_children.at(index));
			}
		}
		
	protected:
		virtual void autoScroll() {
			auto gap = mMaxAngle - maxChildAngle;
			int times = autoScrollTime * 60;
			auto angle = gap / times;
			schedule([this, angle](float dt) {
				setOffsetProp(angle);
				updatePos();
			},0 , times, 0, "__inner_autoscrollview_scheduler");
		}
		virtual void stopAutoScroll() {
			unschedule("__inner_autoscrollview_scheduler");
		}
		virtual void calcNewPos() {
			if (!needCalcPos)
				return;
			needCalcPos = false;

			auto childNum = _children.size();
			if (childNum <= 0) {
				return;
			}

			auto radiamX = 2 * PI * (mX / 360.0f);
			auto radiamY = 2 * PI * (mY / 360.0f);
			auto height = mRadius * std::abs(std::cos(radiamX));
			auto width = mRadius * std::abs(std::cos(radiamY));
			auto minChildAngle = 360.0f;
			for (auto index = 0; index != childNum; index++) {
				float angle = ((int)(offset + index * 360.0f / childNum)) % 360;
				auto child = childVec.at(index);
				auto radiamAngle = 2 * PI * (angle / 360.0f);
				auto newX = mRadius * std::cos(radiamAngle) * std::cos(radiamY);
				auto newY = mRadius * std::sin(radiamAngle) * std::cos(radiamX);
				auto len = std::sqrt(newX * newX + newY * newY);
				float radiamTempAngle = std::acos(newX / len) * 180.0f / PI;
				if (newX >= 0 && newY >= 0) {
				}
				else if (newX <= 0 && newY >= 0) {
				}
				else if (newX <= 0 && newY <= 0) {
					radiamTempAngle = radiamTempAngle + (180 - radiamTempAngle) * 2;
				}
				else {
					radiamTempAngle = 360 - radiamTempAngle;
				}
				float finalAngle = (int)(mZ + radiamTempAngle) % 360;
				auto radiamAngle2 = 2 * PI * (finalAngle / 360.0f);
				newX = len * std::cos(radiamAngle2);
				newY = len * std::sin(radiamAngle2);
				child->setPosition( newX,newY);

				float angleGap = std::abs(finalAngle - mMaxAngle);
				float finalScale = mMinScale + (mMaxScale - mMinScale) * ((std::cos(2 * PI * angleGap / 360) + 1) / 2.0);
				child->setScale(finalScale);
				child->setLocalZOrder((int)(finalScale * 10000));
				if (angleGap < minChildAngle) {
					minChildAngle = angleGap;
					maxChildAngle = finalAngle;
					maxChildIndex = index;
				}
			}
		}
	
		virtual bool onTouchBegan(Touch *touch, Event* event) {
			auto touchInNodeView = convertTouchToNodeSpace(touch);
			if (touchRect.containsPoint(touchInNodeView)) {
				stopAutoScroll();
				return true;
			}
			return false;
		}
		virtual void onTouchMoved(Touch *touch, Event* event) {
			auto gap = touch->getDelta();
			auto len = std::sqrt(gap.x * gap.x + gap.y * gap.y);
			float touchAngle = std::acos(gap.x / len) * 180.0f / PI;
			if (gap.x >= 0 && gap.y >= 0) {
			}
			else if (gap.x <= 0 && gap.y >= 0) {
			}
			else if (gap.x <= 0 && gap.y <= 0) {
				touchAngle = touchAngle + (180 - touchAngle) * 2;
			}
			else {
				touchAngle = 360 - touchAngle;
			}
			//float gapAngle = (int)(touchAngle - mZ + 360) % 360;
			//auto moved = len * std::cos(2 * PI * gapAngle / 360);
			if (std::abs(len) > 0.5f * mRadius / 100) {
				float increase = 1.0f;
				auto angleGap = std::abs(touchAngle - mZ);
				if (angleGap <= 270 && angleGap > 90) {
					increase = -1.0f;
				}
				setOffsetProp(increase * len * 100 / mRadius);
				updatePos();
				event->stopPropagation();
			}
		}
		virtual void onTouchEnded(Touch *touch, Event* event) {
			autoScroll();
		}
		virtual void onTouchCancelled(Touch *touch, Event* event) {
			autoScroll();
		}
		float mX, mY, mZ, mRadius;
		float mMinScale, mMaxScale,mMaxAngle;
		float offset;
		int maxChildIndex;
		float maxChildAngle;
		float autoScrollTime;
		bool needCalcPos;
		Rect touchRect;
		std::vector<Node*> childVec;
	};

	
	class RecyclerView : public Node {
	public:
		CREATE_INS(RecyclerView);

		RecyclerView() {}

		virtual ~RecyclerView(){}

		virtual bool init()override {
			if (!Node::init()) {
				return false;
			}
			auto touchListener = EventListenerTouchOneByOne::create();
			touchListener->onTouchBegan = std::bind(&RecyclerView::onTouchBegan, this, std::placeholders::_1, std::placeholders::_2);
			touchListener->onTouchMoved = std::bind(&RecyclerView::onTouchMoved, this, std::placeholders::_1, std::placeholders::_2);
			touchListener->onTouchEnded = std::bind(&RecyclerView::onTouchEnded, this, std::placeholders::_1, std::placeholders::_2);
			touchListener->onTouchCancelled = std::bind(&RecyclerView::onTouchCancelled, this, std::placeholders::_1, std::placeholders::_2);
			_eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
			return true;
		}

		virtual void setContentSize(const Size& contentSize)override {
			Node::setContentSize(contentSize);
		}

	protected:
		virtual bool onTouchBegan(Touch *touch, Event *unusedEvent) {
			return true;
		}
		virtual void onTouchMoved(Touch *touch, Event *unusedEvent) {}
		virtual void onTouchEnded(Touch *touch, Event *unusedEvent) {}
		virtual void onTouchCancelled(Touch *touch, Event *unusedEvent) {}
		float offset;
	};
}



NS_CC_END


USING_NS_CC;
using namespace std;



AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif

#if (COCOS2D_DEBUG > 0) && (CC_CODE_IDE_DEBUG_SUPPORT > 0)
    // NOTE:Please don't remove this call if you want to debug with Cocos Code IDE
    RuntimeEngine::getInstance()->end();
#endif

}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}
static int register_pauseStack(lua_State* L) {
	return 0;
}
// if you want to use the package manager to install more packages, 
// don't modify or remove this function
static int register_all_packages()
{
	auto engine = LuaEngine::getInstance();
	lua_State* L = engine->getLuaStack()->getLuaState();
	lua_register(L, "pauseStack", register_pauseStack);

	luaopen_lsqlite3(L);
    return 0; //flag for packages manager
}


bool AppDelegate::applicationDidFinishLaunching()
{
    // set default FPS
    Director::getInstance()->setAnimationInterval(1.0 / 60.0f);

    // register lua module
    auto engine = LuaEngine::getInstance();
    ScriptEngineManager::getInstance()->setScriptEngine(engine);
    lua_State* L = engine->getLuaStack()->getLuaState();
    lua_module_register(L);

    register_all_packages();

    LuaStack* stack = engine->getLuaStack();
    stack->setXXTEAKeyAndSign("2dxLua", strlen("2dxLua"), "XXTEA", strlen("XXTEA"));

    //register custom function
    //LuaStack* stack = engine->getLuaStack();
    //register_custom_function(stack->getLuaState());
    
#if CC_64BITS
    FileUtils::getInstance()->addSearchPath("src/64bit");
#endif
    FileUtils::getInstance()->addSearchPath("src");
    FileUtils::getInstance()->addSearchPath("res");

	if (false) {
		auto sc = Scene::create();
		Director::getInstance()->runWithScene(sc);
		if (true) {
			auto testView = TESTUI::InfiniteCycleView::create();
			sc->addChild(testView);
			testView->setPosition(Vec2(120, 300));
			Color4B colors[4] = {
				Color4B::RED,Color4B::GREEN,Color4B::BLUE,Color4B::WHITE
			};
			for (int i = 0; i < 12; i++)
			{
				auto lc = LayerColor::create(colors[i % 4], 10, 10);
				testView->addChild(lc);
			}
			testView->initChildOrder();
			testView->setProp(0, 60, 0, 100,Rect(-100,-100,200,200));
			testView->setScaleProp(1, 4, 0);
			testView->updatePos();
		}

		if (false) {
			auto testView = TESTUI::InfiniteCycleView::create();
			sc->addChild(testView);
			testView->setPosition(Vec2(580, 300));
			Color4B colors[4] = {
				Color4B::RED,Color4B::GREEN,Color4B::BLUE,Color4B::WHITE
			};
			for (int i = 0; i < 12; i++)
			{
				auto lc = LayerColor::create(colors[i % 4], 10, 10);
				testView->addChild(lc);
			}
			testView->initChildOrder();
			testView->setProp(60, 0, 45, 100, Rect(-100, -100, 200, 200));
			testView->setScaleProp(1, 6, 270);
			testView->updatePos();
			testView->runAction(RepeatForever::create(
				Sequence::create(DelayTime::create(0.1f), CallFunc::create([testView]() {
				testView->setOffsetProp(10);
				testView->updatePos();
			}), nullptr)
			));
		}
	}
	else {
		if (engine->executeScriptFile("main.lua"))
		{
			return false;
		}
	}
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground()
{
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground()
{
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
