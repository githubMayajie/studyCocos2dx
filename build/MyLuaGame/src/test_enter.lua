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

local Node1 = class("Node1",cc.Node)
local Node2 = class("Node2",cc.Node)
local function getNode1()
    local ret =  Node1:create()
    ret:enableNodeEvents()
    return ret
end
function Node2:ctor()
    self:enableNodeEvents()
end

function Node2:onEnter()
    print("------------------start")
    -- local parent = self:getParent()
    local parent = cc.Director:getInstance():getRunningScene()
    print(os.clock())
    for i=1,10 do
        local node1 = getNode1()
        for ii=1,10 do
            local node11 = getNode1()
            node11:addTo(node1)
        end
        node1:addTo(parent)
    end
    print("---------------------end")
end

-- 创建100个结点
local function test1(parent)
    local node2 = Node2:create()
    node2:addTo(parent)
end

local function testUIWidget()
    local _,layer,title = runScene(cc.c4b(255,0,0,255),"test UI Widget")
    local scheduler = cc.Director:getInstance():getScheduler()
    local sid1,sid2

    sid1 = scheduler:scheduleScriptFunc(function() 
        print(os.clock())
        scheduler:unscheduleScriptEntry(sid1)
        test1(layer)
    end,1,false)
end
testUIWidget()