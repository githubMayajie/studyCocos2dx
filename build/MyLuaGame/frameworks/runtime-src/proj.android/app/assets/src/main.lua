
cc.FileUtils:getInstance():setPopupNotify(false)
require("socket")
require "config"
require "cocos.init"


local function main()
    -- require("app.MyApp"):create():run()
    -- require "test"
    -- require "testui"
    -- local ComClass = require "testRef"
    -- local c1 = ComClass:create()
    -- local c2 = ComClass:create()
    -- c1:setX(1000)
    -- c2:setX(20000)
    -- print(c1:getX(),c2:getX())
    -- c1:printPrivate()

    -- package.loaded["testRef"] = nil
    -- collectgarbage("collect")

    -- ComClass = require "testRef"
    -- c1 = ComClass:create()
    -- c2 = ComClass:create()
    -- c1:setX(1001)
    -- c2:setX(20002)
    -- print(c1:getX(),c2:getX())
    -- c1:printPrivate()
    -- require "testRef"
    require "testIO"
end

local status, msg = xpcall(main, __G__TRACKBACK__)
if not status then
    print(msg)
end
