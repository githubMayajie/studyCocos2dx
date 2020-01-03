local scheduler = cc.Director:getInstance():getScheduler()
local function delayCall(time,call)
    local sid1 = nil
    sid1 = scheduler:scheduleScriptFunc(function ()
        scheduler:unscheduleScriptEntry(sid1)
        call()
    end,time,false)
end

-- local downloader = cc.Downloader.new()
-- local writablePath = cc.FileUtils:getInstance():getWritablePath()
-- local function onTaskSuccess(task)
--     if task.identifier == "task_img" then
--         print("image download success")
--     elseif task.identifier == "task_txt" then
--         print("txt download success")
--     end
-- end
-- local function onProgress(task, bytesReceived, totalBytesReceived, totalBytesExpected)
--     if task.identifier == "task_img" then
--         print("image download progress " .. tostring(math.floor(totalBytesReceived/totalBytesExpected*100)))
--     elseif task.identifier == "task_txt" then
--         print("txt download progress " .. tostring(math.floor(totalBytesReceived/totalBytesExpected*100)))
--     end
-- end
-- local function onTaskError(task, errorCode, errorCodeInternal, errorStr)
--     if task.identifier == "task_img" then
--         print("image download error: "..errorStr)
--     elseif task.identifier == "task_txt" then
--         print("txt download error: "..errorStr)
--     end
-- end
-- downloader:setOnFileTaskSuccess(onTaskSuccess)
-- downloader:setOnTaskProgress(onProgress)
-- downloader:setOnTaskError(onTaskError)
-- downloader:createDownloadFileTask("https://user-images.githubusercontent.com/11390552/27276063-d0798c42-54d1-11e7-9695-940270e24d11.jpg", 
--                 writablePath .. "/fff.png", "task_img")
-- downloader:createDownloadFileTask("https://github.com/ARM-software/opengl-es-sdk-for-android/blob/master/README.md",
--                  writablePath .. "/fff.txt", "task_txt")

-- local http = cc.XMLHttpRequest:new()
-- http.responseType = cc.XMLHTTPREQUEST_RESPONSE_STRING
-- http:open("GET", "https://www.baidu.com")

-- local function onReadyStateChanged()
--     print(http.response)
--     if http.readyState == 4 and (http.status >= 200 and http.status < 207) then
--         print(http.statusText)
--     else
--         print("http.readyState is:", http.readyState, "http.status is: ",http.status)
--     end
--     http:unregisterScriptHandler()
-- end
-- http:registerScriptHandler(onReadyStateChanged)
-- http:send()




-- local luasocket = require "luasocket"
-- local ls = luasocket.new()
-- luasocket.dispatchEvent = function(self,data)
--     dump(data)
--     if data.name == luasocket.EVENT_CONNECTED then
--         ls:send("this is  a data from call")
--     elseif data.name == luasocket.EVENT_DATA then
--         ls:disconnect()
--     elseif data.name == luasocket.EVENT_CLOSED then
--         ls = nil
--     end
-- end
-- ls:connect("127.0.0.1",8001,false)





local wsSendText = cc.WebSocket:create("ws://echo.websocket.org/", {"myprotocol_1", "myprotocol_2"}, "cacert.pem")
-- local wsSendText = cc.WebSocket:create("127.0.0.1:8080", {"myprotocol_1", "myprotocol_2"}, "cacert.pem")
local function wsSendTextOpen(strData)
    wsSendText:sendString("Send Text WS was opened, url:" .. wsSendText.url .. ", protocol: " .. wsSendText.protocol)
    delayCall(1,function() 
        wsSendText:sendString("this is message from call")
    end)
    delayCall(5,function() 
        wsSendText:close()
    end)
end

local function wsSendTextMessage(strData)
    local strInfo= "response text msg: " .. strData
    print(strInfo)
end

local function wsSendTextClose(strData)
    print("wsiSendText websocket instance closed." .. tostring(strData))
    wsSendText = nil
end

local function wsSendTextError(strData)
    print("sendText Error was error" .. tostring(strData))
end
wsSendText:registerScriptHandler(wsSendTextOpen,cc.WEBSOCKET_OPEN)
wsSendText:registerScriptHandler(wsSendTextMessage,cc.WEBSOCKET_MESSAGE)
wsSendText:registerScriptHandler(wsSendTextClose,cc.WEBSOCKET_CLOSE)
wsSendText:registerScriptHandler(wsSendTextError,cc.WEBSOCKET_ERROR)