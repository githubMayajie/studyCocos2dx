--color cc.c4b
--title string
local function runScene(color,title)
    local scene = display.newScene()
    local layer = display.newLayer(color)
    display.runScene(scene)
    layer:addTo(scene):move(0,0)
    if title then
        local label = cc.Label:createWithSystemFont("Hello World", "Arial", 40)
        label:addTo(layer):move(display.cx, display.cy + 200)
        return scene,layer,label
    else
        return scene,layer
    end
end

local function getAddTestAct(node)
    local actions = {}
    local currentAct = 0
    local delay = 1
    node:runAction(cc.RepeatForever:create(cc.Sequence:create(
                                                cc.DelayTime:create(delay),
                                                cc.CallFunc:create(function() 
                                                    if currentAct > #actions then
                                                    else
                                                        currentAct = currentAct + 1
                                                        local tab = actions[currentAct]
                                                        if tab then
                                                            delay = tab[1]
                                                            tab[2]()
                                                        end
                                                    end
                                                end)
                                            )))
    return function (delay,action)
        if action == nil then
            action = delay
            delay = 1
        end
        actions[#actions + 1] = {delay,action}
    end
end


local function testUIWidget()
    local _,layer,title = runScene(cc.c4b(255,0,0,255),"test UI Widget")
    local test = ccui.Widget:create()
    test:setContentSize(cc.size(100,100))
    test:align(cc.p(0.5,0.5),100,100)
    test:addTo(layer)
    test:setEnabled(true)
    test:setBright(true)
    test:setTouchEnabled(true)
    test:setHighlighted(true)
    local addAct = getAddTestAct(test)
    addAct(function() 
        title:setString("nnnnnn")
    end)
    addAct(function() 
        title:setString("2222")
    end)
    addAct(function() 
        title:setString("nn3333nnnn")
    end)
    addAct(function() 
        title:setString("ddddd")
    end)
end
testUIWidget()