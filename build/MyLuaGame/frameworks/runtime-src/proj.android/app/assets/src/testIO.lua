local tools = cc.FileUtils:getInstance()
local usetDefault = cc.UserDefault:getInstance()
local sep = device.directorySeparator
local writePath = tools:getWritablePath()
if string.byte(writePath,#writePath) == string.byte(sep) then
    writePath = string.sub( writePath,1,-2)
end
local testDir = "test_IO"
local testWritePath = writePath .. sep .. testDir
print("writePath" .. testWritePath)
local isExits = tools:isDirectoryExist(testWritePath)
if isExits then
    tools:removeDirectory(testWritePath)
end
tools:createDirectory(testWritePath)

print(tools:isDirectoryExist(testWritePath))

local function getTime()
    return socket.gettime()
end
local lastTime
local function recordStartTime()
    lastTime = getTime()
end
local info = "\n"
local function recordCostTime(action)
    local cost = getTime() - lastTime
    info = info ..  "do " .. action .. " cost time: " .. tostring(cost) .. "\n"
end


local function fileUtils()
    local readStrFileName = "CMakeLists.txt"
    local readDataFileName = "HelloWorld.png"
    local readStrFileName_fullPath = tools:fullPathForFilename(readStrFileName)
    local readDataFileName_fullPath = tools:fullPathForFilename(readDataFileName)
    recordStartTime()
    local lastStr
    local lastData
    for i=1,1000 do
        lastStr = tools:getStringFromFile(readStrFileName_fullPath)
    end
    for i=1,1000 do
        lastData = tools:getDataFromFile(readDataFileName_fullPath)
    end
    recordCostTime("cc.FileUtils.read")
    --
    recordStartTime()
    for i=1,1000 do
        tools:writeStringToFile(lastStr,testWritePath .. sep .. "CMakeLists" .. tostring(i) .. ".txt")
    end
    for i=1,1000 do
        tools:writeStringToFile(lastData,testWritePath .. sep .. "HelloWorld" .. tostring(i) .. ".png")
    end
    recordCostTime("cc.FileUtils.write")
end

local function userDefault()
    local key1 = "CMakeLists.txt"
    local value1 = "HelloWorld.png"
    local key2 = "CMakeLists2.txt"
    local value2 = "HelloWorld2.png"
    usetDefault:setStringForKey(key1, value1)
    usetDefault:setStringForKey(key2, value2)
    recordStartTime()
    for i=1,1000 do
        usetDefault:getStringForKey(key1)
    end
    for i=1,1000 do
        usetDefault:getStringForKey(value2)
    end
    recordCostTime("cc.UsetDefault.read")
    --
    recordStartTime()
    for i=1,1000 do
        usetDefault:setStringForKey(key1, value1)
        usetDefault:flush()
    end
    for i=1,1000 do
        usetDefault:setStringForKey(key2, value2)
        usetDefault:flush()
    end
    recordCostTime("cc.UsetDefault.write")
end

local function luaio()
    local readStrFileName = testWritePath .. sep  .. "CMakeLists1.txt"
    local readDataFileName = testWritePath .. sep  .. "HelloWorld1.png"
    recordStartTime()
    local lastStr
    local lastData
    for i=1,1000 do
        local fp,err = io.open(readStrFileName,"rb")
        lastStr = fp:read("*a")
        fp:close()
    end
    for i=1,1000 do
        local fp = io.open(readDataFileName,"rb")
        lastData = fp:read("*a")
        fp:close()
    end
    recordCostTime("lua.io.read")
    --
    recordStartTime()
    for i=1,1000 do
        local fp = io.open(testWritePath .. sep .. "CMakeLists_lua" .. tostring(i) .. ".txt","w")
        fp:write(lastStr)
        fp:close()
    end
    for i=1,1000 do
        local fp = io.open(testWritePath .. sep .. "HelloWorld_lua" .. tostring(i) .. ".png","wb")
        fp:write(lastData)
        fp:close()
    end
    recordCostTime("lua.io.write")
end

local function sql()
    local sqlite3 = require "sqlite3"
    local db = sqlite3.open(testWritePath .. sep .. "sqlTest.db")
    local sql = [[create table if not exists table1 (id integer primary key, key_1 text, value_1 text);]]
    db:exec(sql)
    sql = [[
        insert into table1 values(NULL,'CMakeLists.txt','HelloWorld.png');
        insert into table1 values(NULL,'CMakeLists2.txt','HelloWorld2.png');
    ]]
    db:exec(sql)
    recordStartTime()
    for i=1,1000 do
        for row in db:nrows("select * from table1 where key_1 = 'CMakeLists.txt'") do
            local xx = row.value_1
        end
    end
    for i=1,1000 do
        for row in db:nrows("select * from table1 where key_1 = 'CMakeLists2.txt'") do
            local xx = row.value_1
        end
    end
    for i=1,1 do
        for row in db:nrows("select * from table1 where key_1 = 'CMakeLists.txt'") do
            dump(row)
        end
    end
    for i=1,1 do
        for row in db:nrows("select * from table1 where key_1 = 'CMakeLists2.txt'") do
            dump(row)
        end
    end
    recordCostTime("sqlite3.read")
    recordStartTime()
    sql = [[
        update table1 set value_1 = 'HelloWorld2.png' where key_1 = 'CMakeLists.txt';
    ]]
    for i=1,20 do
        db:exec(sql)
    end
    sql = [[
        update table1 set value_1 = 'HelloWorld1.png' where key_1 = 'CMakeLists2.txt';
    ]]
    for i=1,20 do
        db:exec(sql)
    end
    recordCostTime("sqlite3.write")
    for i=1,1 do
        for row in db:nrows("select * from table1 where key_1 = 'CMakeLists.txt'") do
            dump(row)
        end
    end
    for i=1,1 do
        for row in db:nrows("select * from table1 where key_1 = 'CMakeLists2.txt'") do
            dump(row)
        end
    end
    print(db:close() == sqlite3.OK)
end

local tests = {
    ["fileUtils"] = fileUtils,
    ["userDefault"] = userDefault,
    ["luaio"] = luaio,
    ["sql"] = sql,
}
for k,v in pairs(tests) do
    v()
end
print(info)
