local debug = debug
-- local n1 = cc.Node:create()
-- local meta = getmetatable(n1)
-- n1.aaa = 200
-- local env = debug.getfenv(n1)
-- dump(env)
-- print(n1.aaa)

-- local s1 = n1:getScheduler()
-- local sid1 = nil
-- sid1 = s1:scheduleScriptFunc(function ()
--     s1:unscheduleScriptEntry(sid1)
--     local env = debug.getfenv(n1)
--     dump(env) 
--     print(n1.aaa)
-- end,0.1,false)

-- local user = cc.Node:create()
-- dump(user)
-- dump(cc.Node)
-- local usetMeta = debug.getmetatable(user)
-- local nodeMeta = debug.getmetatable(cc.Node)
-- print(user,cc.Node,usetMeta,nodeMeta)

-- local AClass = class("a")
-- local BClass = class("b",function() return {["name"] = "b" } end)
-- local CClass = class("c",cc.Node)
-- local DClass = class("d",{["name"] = "d"})

-- local a = AClass:create()
-- local b = BClass:create()
-- local c = CClass:create()
-- local d = DClass:create()
-- print(a,b,c,d)
-- dump(a)
-- dump(b)
-- dump(c)
-- dump(d)
-- print(a,b.name,d,d.name)

-- local AClass = class("AClass")
-- function AClass:ctor(...)
--     print("AClass",...)
-- end
-- function AClass:funCom(...)
--     print("AClass:funCom",...)
--     self.acom = ...
-- end

-- local BClass = class("BClass",AClass)
-- function BClass:ctor(...)
--     print("BClass",...)
--     BClass.super.ctor(self,...)
-- end
-- function BClass:funCom(...)
--     print("BClass:funCom",...)
--     self.bcom = ...
--     BClass.super.funCom(self,...)
-- end

-- local CClass = class("CClass",AClass,BClass)
-- function CClass:ctor(...)
--     print("CClass ctor",...)
--     for i=1,#CClass.__supers do
--         CClass.__supers[i]["create"](self,...)
--     end
-- end
-- function CClass:funCom(...)
--     print("CClass:funCom",...)
--     self.ccom = ...
--     for i=1,#CClass.__supers do
--         CClass.__supers[i]["funCom"](self,...)
--     end
-- end
-- local cins = CClass:create("create param")
-- cins:funCom("call funcom value")
-- print(cins.ccom)
-- print(cins.bcom)
-- print(cins.acom)

-- local ComClass = class("com")
-- --private func
-- --private field
-- local function private_func(view)
--     print("call private_func",view)
-- end
-- local private_filed_x = {}
-- function ComClass:ctor(...)
--     private_func(self)
--     private_filed_x[self] = "init"
--     self.not_private_filed_x = {}
-- end

-- function ComClass:setX(value)
--     private_filed_x[self] = value
-- end
-- function ComClass:getX(value)
--     return private_filed_x[self]
-- end
-- function ComClass:printPrivate()
--     print("__private",private_func,private_filed_x)
-- end

-- return ComClass

-- local c1 = ComClass:create()
-- local c2 = ComClass:create()
-- c1:setX(1000)
-- c2:setX(20000)
-- print(c1:getX(),c2:getX())
-- dump(ComClass)
-- dump(c1)
-- dump(c2)

-- local n1 = cc.Node:create()
-- local function success(...)
--     n1:setName("success")
--     n1:relase()

--     n1:runAction(cc.Sequence:create(cc.DelayTime:create(1.0),cc.CallFunc:create() 
--         eventDispatch:dispath("finish")
--     end))

--     local s1 = n1:getScheduler()
--     local sid1 = nil
--     sid1 = s1:scheduleScriptFunc(function ()
--         s1:unscheduleScriptEntry(sid1)
--         eventDispatch:dispath("finish")
--     end,1.0,false)
-- end
-- local function failure(...)
--     n1:setName("success")
--     n1:relase()
-- end
-- n1:retain()
-- n1:addTo(scene)
-- http:request("http://www.baidu.com",success,failure)

local newNode = cc.Node:create()
local name = newNode:getName()
newNode:enableNodeEvents()
print(1,newNode)
newNode.onEnter = function(self)
    print(2,self)
    pauseStack()
end
local scene = display.newScene()
display.runScene(scene)
newNode:addTo(scene)