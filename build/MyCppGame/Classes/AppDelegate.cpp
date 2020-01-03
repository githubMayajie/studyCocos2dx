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
#include "HelloWorldScene.h"

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

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(800, 480);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);
#include <string>
#include <thread>
#include <unordered_map>
using std::string;
using std::thread;
using std::unordered_map;



#define TAG "AppDelegate"
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#define myLog(...) __android_log_print(ANDROID_LOG_DEBUG, TAG, __VA_ARGS__)
#else
#include <stdio.h>
#define myLog(fm,...) CCLOG("%s\t" fm,TAG,__VA_ARGS__)
#endif
static void myPrint(void* content) {
	myLog("(Address): address = %p", content);
}
static void myPrint(const char* content) {
	myLog( "%s", content);
}

static void myPrint(const Vec2& content) {
	myLog("(Vec2): x = %f,y = %f", content.x, content.y);
}
static void myPrint(const Size& content) {
	myLog("(Size): width = %f,height = %f", content.width, content.height);
}
static void myPrint(const Color4F& content) {
	myLog("(Color4F): red = %f,gree = %f,blue = %f,alpha = %f", content.r,content.g,content.b, content.a);
}
static void myPrint(const Color4B& content) {
	myLog("(Color4B): red = %d,gree = %d,blue = %d,alpha = %d", content.r, content.g, content.b, content.a);
}
static void myPrint(const Mat4& content) {
	string result;
	for (size_t i = 0; i < 4;i++){
		for (size_t j = 0; j < 4; j++) {
			char buffer[100] = { 0 };
			snprintf(buffer, sizeof(buffer), "m[%d][%d] = %f , ", i,j,content.m[i * 4 + j]);
			result.append(buffer);
		}
	}
	myLog("(Mat4): %s",result.c_str());
}
static void myPrint(int content) {
	myLog("(int): %d", content);
}





class CRef : public Ref {
public:
	CRef() {
	}
};
#define myPrintCount(y) myLog("%s count is %d", #y,y->getReferenceCount())
static void* p1;
static void* p2;
static void* p3;
static void* p4;
static void TestRef() {
	const auto di = Director::getInstance();
	auto scene1 = Scene::create();
	di->runWithScene(scene1);

	//Node* newNode1 = new Node();
	//Node* newNode2 = Node::create();
	//myPrintCount(newNode1);
	//myPrintCount(newNode2);
	//newNode1->retain();
	//newNode2->retain();
	//myPrintCount(newNode1);
	//myPrintCount(newNode2);
	//p1 = newNode1;
	//p2 = newNode2;
	//thread newThread([](){
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Node* newNode1 = (Node*)p1;
	//		Node* newNode2 = (Node*)p2;
	//		myPrintCount(newNode1);
	//		myPrintCount(newNode2);
	//	});
	//});
	//newThread.detach();
	Node* newNode2 = Node::create();
	myPrintCount(newNode2);
	newNode2->retain();
	myPrintCount(newNode2);
	p2 = newNode2;
	thread newThread([]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Node* newNode2 = (Node*)p2;
			myPrintCount(newNode2);
			newNode2->autorelease();
			newNode2->retain();
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Node* newNode2 = (Node*)p2;
			myPrintCount(newNode2);
			newNode2->autorelease();
			newNode2->retain();
		});
	});
	newThread.detach();

}

static void TestDirector() {
	const auto di = Director::getInstance();

	auto glView = di->getOpenGLView();
	myPrint(glView);

	auto tc = di->getTextureCache();
	myPrint(tc);

	auto wsize = di->getWinSize();
	auto vsize = di->getVisibleSize();
	myPrint(wsize);
	myPrint(vsize);


	Vec2 newPos(1.0f, 1.0f);
	const auto& newGLPos = di->convertToGL(newPos);
	const auto& newUIPos = di->convertToGL(newGLPos);
	myPrint(newGLPos);
	myPrint(newUIPos);

	auto getSceneFunc = [](const string& title,const Color4B& backColor)->Scene* {
		auto ret = Scene::create();
		auto colorLayer = LayerColor::create(backColor);
		colorLayer->setPosition(0, 0);
		ret->addChild(colorLayer);
		auto label = Label::createWithSystemFont(title, "", 18);
		label->setName(title);
		colorLayer->addChild(label);
		label->setAnchorPoint(Vec2(0.5f,0.5f));
		label->setPosition(400, 240);
		colorLayer->setName("colorLayer");
		return ret;
	};
	auto scene1 = getSceneFunc("scene1", Color4B(100,0,0,255));
	di->runWithScene(scene1);

	//thread newThread([getSceneFunc]()->void {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([getSceneFunc]()->void {
	//		auto scene2 = getSceneFunc("scene2", Color4B(0, 100, 0, 255));
	//		Director::getInstance()->pushScene(scene2);

	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([getSceneFunc]()->void {
	//		Director::getInstance()->popScene();
	//	});

	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([getSceneFunc]()->void {
	//		auto scene2 = getSceneFunc("scene2", Color4B(0, 100, 0, 255));
	//		auto scene3 = getSceneFunc("scene3", Color4B(0, 0, 100, 255));
	//		Director::getInstance()->pushScene(scene2);
	//		Director::getInstance()->pushScene(scene3);
	//	});

	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([getSceneFunc]()->void {
	//		Director::getInstance()->popToRootScene();
	//	});

	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([getSceneFunc]()->void {
	//		Director::getInstance()->pause();
	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->resume();

	//});
	//newThread.detach();


	//const auto& cc = di->getClearColor();
	//myPrint(cc);
	//auto at1 = LayerColor::create(Color4B(0, 100, 0, 100), wsize.width / 2, wsize.height);
	//at1->setTag(200);
	//scene1->addChild(at1);

	//auto at2 = LayerColor::create(Color4B(0, 0, 100, 100), wsize.width / 2, wsize.height);
	//at2->setTag(100);
	//at2->setIgnoreAnchorPointForPosition(false);
	//at2->setAnchorPoint(Vec2(1,0));
	//at2->setPosition(Vec2(wsize.width,0));
	//scene1->addChild(at2);


	//thread newThread([]() {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()->void {
	//		auto scene1 = Director::getInstance()->getRunningScene();
	//		scene1->removeChildByTag(100);
	//		scene1->removeChildByTag(200);
	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]()->void {
	//		auto scene1 = Director::getInstance()->getRunningScene();
	//		auto wsize = Director::getInstance()->getWinSize();
	//		Director::getInstance()->setAlphaBlending(false);
	//		auto at1 = LayerColor::create(Color4B(0, 100, 0, 100), wsize.width / 2, wsize.height);
	//		scene1->addChild(at1);
	//		auto at2 = LayerColor::create(Color4B(0, 0, 100, 100), wsize.width / 2, wsize.height);
	//		at2->setIgnoreAnchorPointForPosition(false);
	//		at2->setAnchorPoint(Vec2(1, 0));
	//		at2->setPosition(Vec2(wsize.width, 0));
	//		scene1->addChild(at2);

	//	});
	//});
	//newThread.detach();


	auto sche1 = di->getScheduler();
	auto am = di->getActionManager();
	auto ed = di->getEventDispatcher();
	myPrint(sche1);
	myPrint(am);
	myPrint(ed);

	////(const ccSchedulerFunc& callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key)
	//sche1->schedule([](float)->void{
	//	auto di = Director::getInstance();
	//	auto size = di->getWinSize();
	//	static int count = 1;
	//	if (count == 1) {
	//		float zeye = di->getZEye();
	//		Mat4 matrixPerspective, matrixLookup;
	//		Mat4::createPerspective(80, (GLfloat)size.width / size.height, 10, zeye + size.height / 2, &matrixPerspective);
	//		Vec3 eye(size.width / 2, size.height / 2, zeye), center(size.width / 2, size.height / 2, 0.0f), up(0.0f, 1.0f, 0.0f);
	//		Mat4::createLookAt(eye, center, up, &matrixLookup);
	//		Mat4 proj3d = matrixPerspective * matrixLookup;
	//		di->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, proj3d);
	//		
	//	}
	//	else {
	//		float zeye = di->getZEye();
	//		Mat4 matrixperspective, matrixlookup;
	//		Mat4::createPerspective(60, (GLfloat)size.width / size.height, 10, zeye + size.height / 2, &matrixperspective);
	//		Vec3 eye(size.width / 2, size.height / 2, zeye), center(size.width / 2, size.height / 2, 0.0f), up(0.0f, 1.0f, 0.0f);
	//		Mat4::createLookAt(eye, center, up, &matrixlookup);
	//		Mat4 proj3d = matrixperspective * matrixlookup;
	//		di->loadMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION, proj3d);
	//	}
	//	count++;
	//},scene1,0.2,1,1.0f,false,"dddddddddddd");
}

#define CREATE_FIRST_SCENE const auto di = Director::getInstance();\
auto scene1 = Scene::create();\
di->runWithScene(scene1);

#define CREATE_LABLE(name) auto name = Label::createWithSystemFont(#name,"",24);\
scene1->addChild(name);

#define CREATE_SPRITE(name) auto name = Sprite::create("HelloWorld.png");\
scene1->addChild(name);

class NNode : public Node {
public:
	static NNode* create() {
		NNode * ret = new (std::nothrow) NNode();
		if (ret && ret->init())
		{
			ret->autorelease();
		}
		else
		{
			CC_SAFE_DELETE(ret);
		}
		return ret;

	}
	virtual void onEnter() {
		myPrint(this);
		myPrint("onEnter");
		Node::onEnter();
	}
	virtual void onEnterTransitionDidFinish() {
		myPrint(this);
		myPrint("onEnterTransitionDidFinish");
		Node::onEnterTransitionDidFinish();
	}
	
	void runTest(Node* parent) {
		this->setOnEnterCallback([this]() {
			myPrint(this);
			myPrint("setOnEnterCallback");
		});
		this->setonEnterTransitionDidFinishCallback([this]() {
			myPrint(this);
			myPrint("setonEnterTransitionDidFinishCallback");
		});
		parent->addChild(this);
	}
};

static void testNode() {
	CREATE_FIRST_SCENE;
	auto n1 = Node::create();
	auto t2 = Node::create();

	CREATE_SPRITE(label1);
	CREATE_SPRITE(label2);

	label1->setPosition(100, 100);
	label2->setPosition(200, 100);

	p1 = label1;
	p2 = label2;

	/*thread newThread([]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Sprite* label1 = (Sprite*)p1;
			Sprite* label2 = (Sprite*)p2;
			label1->setLocalZOrder(10);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Sprite* label1 = (Sprite*)p1;
			Sprite* label2 = (Sprite*)p2;
			label1->setScaleX(0.5);
			label2->setScaleY(0.5);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Sprite* label1 = (Sprite*)p1;
			Sprite* label2 = (Sprite*)p2;
			label1->setScaleX(1);
			label2->setScaleY(1);
			label1->setSkewX(120);
			label2->setSkewY(120);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Sprite* label1 = (Sprite*)p1;
			Sprite* label2 = (Sprite*)p2;
			label1->setSkewX(0);
			label2->setSkewY(0);
		});
	});*/
#define NEW_POS(node,x,y) node->setPosition(x,y)
#define NEW_ANC(node,x,y) node->setAnchorPoint(Vec2(x,y))
	//thread newThread([]() {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Sprite* label1 = (Sprite*)p1;
	//		Sprite* label2 = (Sprite*)p2;
	//		NEW_ANC(label1, 0.5f, 0.5f);
	//		NEW_ANC(label2, 0.5f, 0.5f);
	//		NEW_POS(label1, 100, 400);
	//		NEW_POS(label2, 400, 400);
	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Sprite* label1 = (Sprite*)p1;
	//		Sprite* label2 = (Sprite*)p2;
	//		NEW_ANC(label1, 0, 0);
	//		NEW_ANC(label2, 1, 1);
	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Sprite* label1 = (Sprite*)p1;
	//		Sprite* label2 = (Sprite*)p2;
	//		NEW_ANC(label1, 0.5f, 0.5f);
	//		NEW_ANC(label2, 0.5f, 0.5f);
	//		NEW_POS(label1, 100,100 );
	//		NEW_POS(label2, 200,100);
	//	});
	//});
#define NEW_ROT(node,x,y) node->setRotationX(x);node->setRotationY(y)
#define NEW_SKEROT(node,x,y) node->setRotationSkewX(x); node->setRotationSkewY(x)
	//thread newThread([]() {
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Sprite* label1 = (Sprite*)p1;
	//		Sprite* label2 = (Sprite*)p2;
	//		NEW_ANC(label1, 0, 0);
	//		NEW_ANC(label2, 0, 0);
	//		NEW_ROT(label1, 45,0);
	//		NEW_ROT(label2, 0, 45);
	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Sprite* label1 = (Sprite*)p1;
	//		Sprite* label2 = (Sprite*)p2;
	//		NEW_ROT(label1, 0, 0);
	//		NEW_ROT(label2, 0, 0);
	//		NEW_SKEROT(label1, 45, 0);
	//		NEW_SKEROT(label2, 0, 45);

	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Sprite* label1 = (Sprite*)p1;
	//		Sprite* label2 = (Sprite*)p2;
	//		NEW_ANC(label1, 0, 0);
	//		NEW_ANC(label2, 1, 1);
	//		NEW_ROT(label1, 45, 0);
	//		NEW_ROT(label2, 0, 45);
	//	});
	//	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	//	Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
	//		Sprite* label1 = (Sprite*)p1;
	//		Sprite* label2 = (Sprite*)p2;
	//		NEW_ANC(label1, 0, 0);
	//		NEW_ANC(label2, 1, 1);
	//		NEW_SKEROT(label1, 45, 0);
	//		NEW_SKEROT(label2, 0, 45);
	//	});
	//});
	//newThread.detach();


//	thread newThread([]() {
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
//			Sprite* label1 = (Sprite*)p1;
//			Sprite* label2 = (Sprite*)p2;
//
//			auto n1GLProgram = label1->getGLProgram();
//			auto n2GLProgramState = label2->getGLProgramState();
//			const char* vshader = R"(
//attribute vec4 a_position;
//attribute vec2 a_texCoord;
//attribute vec4 a_color;
//#ifdef GL_ES
//varying lowp vec4 v_fragmentColor;
//varying mediump vec2 v_texCoord;
//#else
//varying vec4 v_fragmentColor;
//varying vec2 v_texCoord;
//#endif
//
//void main()
//{
//    gl_Position = CC_PMatrix * a_position;
//    v_fragmentColor = a_color;
//    v_texCoord = a_texCoord;
//}			)";
//			const char* changefs = R"(
//#ifdef GL_ES
//precision lowp float;
//#endif
//
//varying vec4 v_fragmentColor;
//varying vec2 v_texCoord;
//uniform vec3 u_mask;
//
//void main()
//{
//	vec4 texColor = texture2D(CC_Texture0, v_texCoord);
//    texColor.rgb = vec3(texColor.r * u_mask.r , texColor.g * u_mask.g , texColor.b * u_mask.b);
//    gl_FragColor = texColor;
//}
//			)";
//			const char* notredfs = R"(
//#ifdef GL_ES
//precision lowp float;
//#endif
//varying vec2 v_texCoord;  
//void main()
//{
//    vec4 texColor = texture2D(CC_Texture0, v_texCoord);
//    texColor.r = 0;
//    gl_FragColor = texColor;
//}
//			)";
//			p3 = n1GLProgram;
//			p4 = n2GLProgramState;
//			auto program1 = GLProgram::createWithByteArrays(vshader, notredfs);
//			auto program2 = GLProgram::createWithByteArrays(vshader, changefs);
//			auto program2State = GLProgramState::getOrCreateWithGLProgram(program2);
//			program2State->setUniformVec3("u_mask", Vec3(0.5f,0.3f,0.8f));
//			label1->setGLProgram(program1);
//			label2->setGLProgramState(program2State);
//
//		});
//		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
//		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
//			Sprite* label1 = (Sprite*)p1;
//			Sprite* label2 = (Sprite*)p2;
//			GLProgram* program1 = (GLProgram*)p3;
//			GLProgramState* program2State = (GLProgramState*)p4;
//			label1->setGLProgram(program1);
//			label2->setGLProgramState(program2State);
//		});
//	});
//	newThread.detach();
	
	//auto testNode = NNode::create();
	//testNode->runTest(scene1);


	//auto ed = scene1->getEventDispatcher();
	//string eventName = "__0_1__";
	//auto et = EventListenerCustom::create(eventName, [](EventCustom* et) {
	//	const char* data = (const char*)et->getUserData();
	//	myPrint(data);
	//});
	//ed->addEventListenerWithFixedPriority(et, -1);
	//ed->dispatchCustomEvent(eventName, (void*)eventName.c_str());


	//auto ac1 = Sequence::create(MoveTo::create(1.0f, Vec2(400, 400)),RotateTo::create(1.0f,100,100), DelayTime::create(1.0f), CallFunc::create([]() {
	//	myPrint("dddd");
	//}),RemoveSelf::create(), nullptr);
	//auto ac2 = Spawn::create(MoveTo::create(1.0f, Vec2(400, 400)), RotateTo::create(1.0f, 100, 100), DelayTime::create(1.0f), CallFunc::create([]() {
	//	myPrint("eeeee");
	//}),  nullptr);
	//label1->runAction(ac1);
	//label2->runAction(ac2);

	//auto sche = label1->getScheduler();
	////void Scheduler::schedule(const ccSchedulerFunc& callback, void *target, float interval, unsigned int repeat, float delay, bool paused, const std::string& key)
	//sche->schedule([](float di) {
	//	Sprite* label1 = (Sprite*)p1;
	//	static int cout = 0;
	//	myPrint(cout);
	//	cout++;
	//	if (cout == 8) {
	//		auto sche = label1->getScheduler();
	//		sche->unschedule("_sche1_", sche);
	//	}
	//},sche,0,10,0,false,"_sche1_" );


}

static void testSprite() {
	CREATE_FIRST_SCENE;
	auto wsize = Director::getInstance()->getWinSize();
	auto at1 = LayerColor::create(Color4B(0, 100, 0, 100), wsize.width / 2, wsize.height);
	auto at2 = LayerColor::create(Color4B(0, 100, 0, 100), wsize.width / 2, wsize.height);
	scene1->addChild(at1);
	at1->addChild(at2);
	p3 = at1;
	p4 = at2;
	at1->setPosition(0, 0);
	at2->setPosition(200, 200);
	auto sp1 = Sprite::create("HelloWorld.png");
	auto sp2 = Sprite::create("HelloWorld.png");
	scene1->addChild(sp1);
	sp1->setPosition(100, 100);
	sp1->addChild(sp2);
	sp2->setAnchorPoint(Vec2(0,0));
	sp2->setPosition(50, 50);
	p1 = sp1;
	p2 = sp2;
	thread newThread([]() {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Sprite* sp1 = (Sprite*)p1;
			Sprite* sp2 = (Sprite*)p2;
			sp1->setFlippedX(true);
			sp2->setFlippedY(true);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			Sprite* sp1 = (Sprite*)p1;
			Sprite* sp2 = (Sprite*)p2;
			sp1->setFlippedX(false);
			sp2->setFlippedY(false);
			/** Blending disabled. Uses {GL_ONE, GL_ZERO} */
			//static const BlendFunc DISABLE;
			LayerColor* at1 = (LayerColor*)p3;
			at1->setBlendFunc(BlendFunc::DISABLE);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			/** Blending enabled for textures with Alpha premultiplied. Uses {GL_ONE, GL_ONE_MINUS_SRC_ALPHA} */
			//static const BlendFunc ALPHA_PREMULTIPLIED;
			LayerColor* at1 = (LayerColor*)p3;
			at1->setBlendFunc(BlendFunc::ALPHA_PREMULTIPLIED);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			/** Blending enabled for textures with Alpha NON premultiplied. Uses {GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA} */
			//static const BlendFunc ALPHA_NON_PREMULTIPLIED;
			LayerColor* at1 = (LayerColor*)p3;
			at1->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			/** Enables Additive blending. Uses {GL_SRC_ALPHA, GL_ONE} */
			//static const BlendFunc ADDITIVE;
			LayerColor* at1 = (LayerColor*)p3;
			at1->setBlendFunc(BlendFunc::ADDITIVE);
		});
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
			LayerColor* at1 = (LayerColor*)p3;
			at1->setBlendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED);
		});

	});
	newThread.detach();
}

#define RUN_ACTION_TEST(x) std::this_thread::sleep_for(std::chrono::milliseconds(1000));\
Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {\
Sprite* sp1 = (Sprite*)p1;\
Sprite* sp2 = (Sprite*)p2;\
x;\
})
static void testAction() {
	CREATE_FIRST_SCENE;
	auto sp1 = Sprite::create("HelloWorld.png");
	scene1->addChild(sp1);
	auto sp2 = Sprite::create("HelloWorld.png");
	scene1->addChild(sp2);

	p1 = sp1;
	p2 = sp2;
	sp1->setPosition(200, 300);
	sp2->setPosition(600, 300);
	thread newThread([]() {
		RUN_ACTION_TEST(
			auto action1 = MoveTo::create(0.3f,Vec2(100,100));
			auto action2 = action1->clone();
			auto speedAction2 = Speed::create(action2,0.5f);
			sp1->runAction(action1);
			sp2->runAction(speedAction2);
		);
		RUN_ACTION_TEST(
			sp1->setPosition(200, 300);
			sp2->setPosition(600, 300);
			auto action1 = MoveTo::create(1.0f, Vec2(100, 100));
			auto action2 = Follow::create(sp1);
			sp1->runAction(action1);
			sp2->runAction(action2);
		);
		RUN_ACTION_TEST(
			sp2->stopAllActions();
			sp1->setPosition(200, 300);
			sp2->setPosition(600, 300);
			auto action1 = MoveTo::create(1.0f, Vec2(100, 100));
			auto action2 = MoveTo::create(1.0f, Vec2(500, 100));
			sp1->runAction(EaseElasticInOut::create(action1));
			sp2->runAction(EaseCubicActionInOut::create(action2));
		);
		RUN_ACTION_TEST(
			sp1->setPosition(200, 300);
			sp2->setPosition(600, 300);
			auto action1 = Hide::create();
			auto action2 = FlipY::create(true);
			sp1->runAction(action1);
			sp2->runAction(action2);
		);
		RUN_ACTION_TEST(
			sp1->setVisible(true);
			sp2->setFlippedY(false);
			sp1->setPosition(200, 300);
			sp2->setPosition(600, 300);
			auto action1 = Blink::create(1.0f,20);
			auto action2 = RotateTo::create(1.0f,100,200);
			sp1->runAction(action1);
			sp2->runAction(action2);
		);

	});
	newThread.detach();

}

//c++14 convert enum to int
#include <type_traits>
#ifdef ANDROID
template <typename E>
constexpr int to_underlying(E e) noexcept
{
    return 1;
}
#else
template <typename E>
constexpr auto to_underlying(E e) noexcept
{
	return static_cast<std::underlying_type_t<E>>(e);
}
#endif

class TLayer : public Layer {
public:
	static TLayer* create() {
		TLayer *ret = new (std::nothrow) TLayer();
		if (ret && ret->init())
		{
			ret->autorelease();
			return ret;
		}
		else
		{
			CC_SAFE_DELETE(ret);
			return nullptr;
		}
	}
	virtual bool onTouchBegan(Touch *touch, Event *unused_event) {
		Vec2 glloc = touch->getLocation();
		Vec2 uiLoc = touch->getLocationInView();
		Vec2 relSlefLoc = convertTouchToNodeSpace(touch);
		myPrint(glloc);
		myPrint(uiLoc);
		myPrint(relSlefLoc);
		return true;
	}
	virtual void onTouchMoved(Touch *touch, Event *unused_event) {
	}
	virtual void onTouchEnded(Touch *touch, Event *unused_event) {
	
	}
	virtual void onTouchCancelled(Touch *touch, Event *unused_event) {
	}
	virtual void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
		myPrint(to_underlying<EventKeyboard::KeyCode>(keyCode));
		myPrint("pressed");
	}
	virtual void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
		myPrint(to_underlying<EventKeyboard::KeyCode>(keyCode));
		myPrint("released");
	}
	void useMyEvent() {
		auto listener = EventListenerTouchOneByOne::create();
		listener->setSwallowTouches(_swallowsTouches);

		listener->onTouchBegan = CC_CALLBACK_2(TLayer::onTouchBegan, this);
		listener->onTouchMoved = CC_CALLBACK_2(TLayer::onTouchMoved, this);
		listener->onTouchEnded = CC_CALLBACK_2(TLayer::onTouchEnded, this);
		listener->onTouchCancelled = CC_CALLBACK_2(TLayer::onTouchCancelled, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

		auto listener2 = EventListenerKeyboard::create();
		listener2->onKeyPressed = CC_CALLBACK_2(TLayer::onKeyPressed, this);
		listener2->onKeyReleased = CC_CALLBACK_2(TLayer::onKeyReleased, this);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener2, this);
	}

};
static void testLayer() {
	CREATE_FIRST_SCENE;
	auto layer = TLayer::create();
	scene1->addChild(layer);
	layer->useMyEvent();
	auto sp1 = Sprite::create("HelloWorld.png");
	layer->addChild(sp1);
	sp1->setPosition(400, 400);
}

static void testLabel() {
	CREATE_FIRST_SCENE;
	const char* content = R"(\x\\\\\\\\\xxxxx
dddd
bb
ddd
eee
)";
	auto l1 = Label::createWithSystemFont(content, "", 24);
	auto l2 = Label::createWithTTF(content, "fonts/arial.ttf", 24);
	l1->setAnchorPoint(Vec2(0.5f,0.5f));
	l2->setAnchorPoint(Vec2(0.5f, 0.5));
	scene1->addChild(l1);
	scene1->addChild(l2);
	l1->setPosition(200, 200);
	l2->setPosition(600, 200);
	l2->enableUnderline();
	l2->enableItalics();
	l2->setHorizontalAlignment(TextHAlignment::CENTER);
}

#include "../cocos2d/extensions/GUI/CCScrollView/CCTableView.h"
using cocos2d::extension::TableView;
using cocos2d::extension::TableViewCell;
using cocos2d::extension::TableViewDataSource;
using cocos2d::extension::TableViewDelegate;
class TableDataAndViewDel : public TableViewDataSource, public TableViewDelegate {
public:
	TableDataAndViewDel(int param1, Size param2)
		:num(param1)
		, size(param2)
	{
	}

	virtual void tableCellTouched(TableView* table, TableViewCell* cell) {

	}
	virtual void tableCellHighlight(TableView* table, TableViewCell* cell) {

	}
	virtual void tableCellUnhighlight(TableView* table, TableViewCell* cell) {
	}
	virtual void tableCellWillRecycle(TableView* table, TableViewCell* cell) {

	}
	virtual Size tableCellSizeForIndex(TableView* table, ssize_t idx) {
		return size;
	}
	virtual Size cellSizeForTable(TableView* table) {
		return size;
	}
	virtual TableViewCell* tableCellAtIndex(TableView *table, ssize_t idx) {
		auto cell = table->dequeueCell();
		if (cell == nullptr)
			cell = TableViewCell::create();
		LayerColor* bg = (LayerColor*)cell->getChildByName("bg");
		if (bg == nullptr) {
			bg = LayerColor::create(Color4B(100, 200, 200, 255), size.width, size.height - 5);
			bg->setPosition(0, 0);
			bg->setName("bg");
			cell->addChild(bg);
		}
		char buff[100] = { '\0' };
		snprintf(buff, sizeof(buff), "No ddddddd ==>>>>>>%d", idx + 1);
		Label* lebel = (Label*)cell->getChildByName("label");
		if (lebel == nullptr) {
			lebel = Label::createWithSystemFont(buff, "", 24);
			lebel->setAnchorPoint(Vec2(0, 0.5f));
			lebel->setPosition(0, size.height / 2);
			lebel->setName("label");
			lebel->setTextColor(Color4B(255,0,0,255));
			cell->addChild(lebel);
		}
		else {
			lebel->setString(buff);
		}
		ClippingNode* cp = (ClippingNode*)cell->getChildByName("cp");
		if (cp == nullptr) {
			cp = ClippingNode::create();
			cp->setName("cp");
			cp->setContentSize(Size(size.height - 20,size.height - 20));
			auto mask = DrawNode::create();
			mask->drawSolidCircle(Vec2(0, 0), 30, CC_DEGREES_TO_RADIANS(90), 50, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
			mask->setContentSize(Size(60, 60));
			mask->setPosition(Vec2(40,40));
			cp->setStencil(mask);
			cp->setContentSize(Size(80,80));
			cell->addChild(cp);
			auto sp_hello = Sprite::create("HelloWorld.png");
			sp_hello->setScale(0.5);
			sp_hello->setPosition(Vec2(40, 40));
			cp->addChild(sp_hello);
		}
		return cell;
	}
	virtual ssize_t numberOfCellsInTableView(TableView *table) {
		return num;
	}
private:
	int num;
	Size size;
};
static void testTableView() {
	CREATE_FIRST_SCENE;
	auto tvDel = new TableDataAndViewDel(100, Size(400, 80));
	auto tv = TableView::create(tvDel, Size(400, 300));
	tv->setIgnoreAnchorPointForPosition(false);
	tv->setAnchorPoint(Vec2(0, 0));
	tv->setPosition(0, 0);
	tv->setDelegate(tvDel);
	tv->setVerticalFillOrder(TableView::VerticalFillOrder::TOP_DOWN);
	scene1->addChild(tv);
}

static void runTest() {
	TestRef();
	//TestDirector();
	//testNode();
	//testSprite();
	//testAction();
	//testLayer();
	//testLabel();
	//testTableView();
}

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
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil,multisamplesCount
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8, 0};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("MyCppGame", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("MyCppGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	runTest();

    //// create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();

    //// run
    //director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
