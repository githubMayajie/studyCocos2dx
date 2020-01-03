-- 无限循环View
local InfiniteCycleView = class("InfiniteCycleView",cc.Node)
local sin = math.sin
local cos = math.cos
local abs = math.abs
local acos = math.acos
local rad = math.rad
local sqrt = math.sqrt
local PI = math.pi

-- 私有方法
local function InfiniteCycleView_autoScroll(view)
    local gap = view.mMaxAngle - view.maxChildAngle
    local times = view.autoScrollTime * 60
    local angle = gap / times
    local scheduler = cc.Director:getInstance():getScheduler()
    if view.autoScrollSheduler ~= nil then
        scheduler:unscheduleScriptEntry(view.autoScrollSheduler)
    end
    view.autoScrollSheduler = scheduler:scheduleScriptFunc(function(dt) 
    	view:setOffsetProp(angle);
        view:updatePos();
        times = times - 1
        if times <= 0 then
            scheduler:unscheduleScriptEntry(view.autoScrollSheduler)
            view.autoScrollSheduler = nil
        end
    end,0,false)
end
local function InfiniteCycleView_stopAutoScroll(view)
    if view.autoScrollSheduler ~= nil then
        local scheduler = cc.Director:getInstance():getScheduler()
        scheduler:unscheduleScriptEntry(view.autoScrollSheduler)
        view.autoScrollSheduler = nil
    end
end
local function InfiniteCycleView_calcNewPos(view)
    if view.needCalcPos == false then
        return
    end
    view.needCalcPos = false
    local childNum = view:getChildrenCount()
    if childNum <= 0 then
        return
    end

    local radiamX = rad(view.mX)
    local radiamY = rad(view.mY)
    local mRadius = view.mRadius
    local height = mRadius * abs(cos(radiamX))
    local width = mRadius * abs(cos(radiamY))
    local minChildAngle = 360.0
    for index = 1,#view.childVec do
        local angle = ((view.offset + index * 360.0 / childNum)) % 360
        local child = view.childVec[index]
        local radiamAngle = rad(angle)
        local newX = mRadius * cos(radiamAngle) * cos(radiamY)
        local newY = mRadius * sin(radiamAngle) * cos(radiamX)
        local len = sqrt(newX * newX + newY * newY)
        local radiamTempAngle = acos(newX / len) * 180.0 / PI
        if newX >= 0 and newY >= 0 then
        elseif newX <= 0 and newY >= 0 then
        elseif newX <= 0 and newY <= 0 then
            radiamTempAngle = radiamTempAngle + (180 - radiamTempAngle) * 2
        else
            radiamTempAngle = 360 - radiamTempAngle
        end
        local finalAngle = (view.mZ + radiamTempAngle) % 360
        local radiamAngle2 = rad(finalAngle)
        newX = len * cos(radiamAngle2)
        newY = len * sin(radiamAngle2)
        child:setPosition(newX,newY)

        local angleGap = abs(finalAngle - view.mMaxAngle)
        local finalScale =  view.mMinScale + (view.mMaxScale - view.mMinScale) * ((cos(rad(angleGap)) + 1)/ 2.0)
        child:setScale(finalScale)
        child:setLocalZOrder(finalScale * 10000)
        if angleGap < minChildAngle then
            minChildAngle = angleGap
            view.maxChildAngle = finalAngle
            view.maxChildIndex = index
        end
    end
end

-- 公开方法
function InfiniteCycleView:ctor(...)
    self.mX = 0.0
    self.mY = 0.0
    self.mZ = 0.0
    self.mRadius = 100.0
    self.needCalcPos = false
    self.offset = 270.0
    self.maxChildIndex = 0
    self.maxChildAngle = 360.0
    self.mMaxScale = 1.0
    self.mMinScale = 1.0
    self.mMaxAngle = 0.0
    self.autoScrollTime = 0.12
    self.touchRect = cc.rect(0,0,0,0)
    self.childVec = {}
    local function onTouchBegan(touch, event)
        local touchInNodeView = self:convertTouchToNodeSpace(touch)
        if cc.rectContainsPoint(self.touchRect,touchInNodeView) then
            InfiniteCycleView_stopAutoScroll(self)
            return true
        end
        return false
    end
    local function onTouchMoved(touch, event)
        local gap = touch:getDelta()
        local len = sqrt(gap.x * gap.x + gap.y * gap.y)
        local touchAngle = acos(gap.x / len) * 180.0 / PI
        if gap.x >= 0 and gap.y >= 0 then
        elseif gap.x <= 0 and gap.y >= 0 then
        elseif gap.x <= 0 and gap.y <= 0 then
            touchAngle = touchAngle + (180 - touchAngle) * 2
        else
            touchAngle = 360 - touchAngle
        end
        if abs(len) > 0.5 * self.mRadius / 100 then
            local increase = 1.0
            local angleGap = abs(touchAngle - self.mZ)
            if angleGap <= 270 and angleGap > 90 then
                increase = -1.0
            end
            self:setOffsetProp(increase * len * 100 / self.mRadius)
            self:updatePos()
            event:stopPropagation()
        end
    end
    local function onTouchEnded(touch, event)
        InfiniteCycleView_autoScroll(self)
    end
    local function onTouchCancelled(touch, event)
        InfiniteCycleView_autoScroll(self)
    end
    --bind touch event
	local touchListener = cc.EventListenerTouchOneByOne:create()
    touchListener:registerScriptHandler(onTouchBegan,cc.Handler.EVENT_TOUCH_BEGAN )
    touchListener:registerScriptHandler(onTouchMoved,cc.Handler.EVENT_TOUCH_MOVED )
    touchListener:registerScriptHandler(onTouchEnded,cc.Handler.EVENT_TOUCH_ENDED )
    touchListener:registerScriptHandler(onTouchCancelled,cc.Handler.EVENT_TOUCH_CANCELLED )
    self:getEventDispatcher():addEventListenerWithSceneGraphPriority(touchListener, self)		
end

function InfiniteCycleView:setProp(x,y,z,radius,touch)
    self.mX = x
    self.mY = y
    self.mZ = z
    self.mRadius = radius
    self.touchRect = touch
end

function InfiniteCycleView:setOffsetProp(value)
    self.offset = self.offset + value
    if self.offset < 0 then
        self.offset = 360 - ((0 - self.offset) % 360)
    elseif self.offset > 360 then
        self.offset = self.offset % 360
    end
end

function InfiniteCycleView:setScaleProp(minScale,maxScale,maxAngle)
    if minScale < 0 or maxScale < 0  then
        return
    end
    self.mMinScale = minScale
    self.mMaxScale = maxScale
    self.mMaxAngle = maxAngle
end

function InfiniteCycleView:updatePos()
    self.needCalcPos = true
    InfiniteCycleView_calcNewPos(self)
end

function InfiniteCycleView:clearChildOrder()
    self.childVec = {}
end

function InfiniteCycleView:addChildOrder(child)
    self.childVec[#self.childVec + 1] = child
end

function InfiniteCycleView:initChildOrder()
    self.childVec = self:getChildren()
end
-- InfiniteCycleView finish


local function createTestLayer(color,title)
    local scene = display.newScene()
    local layer = display.newLayer(color)
    display.runScene(scene)
    layer:addTo(scene):move(0,0)
    local label = cc.Label:createWithSystemFont(title, "Arial", 40)
    label:addTo(layer,100):move(display.cx, display.cy + 200)
    return scene,layer,label
end


local function createInfiniteCycleView()
    local scene,layer,label = createTestLayer(cc.c4b(122,122,122,255),"test InfiniteCycleView")
    if true then
        local testView = InfiniteCycleView:create()
        testView:addTo(layer):move(200,300)
        local colors = {cc.c4b(255,0,0,255),cc.c4b(0,255,0,255),cc.c4b(0,0,255,255),cc.c4b(255,255,255,255)}
        for i=1,12 do
            local lc = cc.LayerColor:create(colors[(i -1) % 4 + 1], 10, 10)
            testView:addChild(lc)
        end
        testView:initChildOrder()
        testView:setProp(60,0,0,100,cc.rect(-100,-100,200,200))
        testView:setScaleProp(1,1,270)
        testView:updatePos()
        local angle = 0
        testView:runAction(cc.RepeatForever:create(cc.Sequence:create(
                                                cc.DelayTime:create(0.01),
                                                cc.CallFunc:create(function() 
                                                    testView:setProp(60,0,angle,100,cc.rect(-100,-100,200,200))
                                                    testView:updatePos()
                                                    angle = (angle + 1) % 360
                                                end)
                                            )))
    end

    if true then
        local testView = InfiniteCycleView:create()
        testView:addTo(layer):move(600,300)
        local colors = {cc.c4b(255,0,0,255),cc.c4b(0,255,0,255),cc.c4b(0,0,255,255),cc.c4b(255,255,255,255)}
        for i=1,12 do
            local lc = cc.LayerColor:create(colors[(i -1) % 4 + 1], 10, 10)
            testView:addChild(lc)
        end
        testView:initChildOrder()
        testView:setProp(80,0,0,100,cc.rect(-100,-100,200,200))
        testView:setScaleProp(1,1,270)
        testView:updatePos()
        local angle = 0
        testView:runAction(cc.RepeatForever:create(cc.Sequence:create(
                                                cc.DelayTime:create(0.01),
                                                cc.CallFunc:create(function() 
                                                    testView:setProp(0,angle,0,100,cc.rect(-100,-100,200,200))
                                                    testView:updatePos()
                                                    angle = (angle + 1) % 360
                                                end)
                                            )))
    end
end
createInfiniteCycleView()
