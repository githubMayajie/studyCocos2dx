local scheduler = cc.Director:getInstance():getScheduler()
local sid1 = nil
local sid2 = nil
local sid3 = nil

-- sid1 = scheduler:scheduleScriptFunc(function() 
--     scheduler:unscheduleScriptEntry(sid1)
--     print("s1111")
-- end,0,false)
-- sid2 = scheduler:scheduleScriptFunc(function() 
--     scheduler:unscheduleScriptEntry(sid1)
--     print("s22222")
-- end,0,false)

-- sid3 = scheduler:scheduleScriptFunc(function() 
--     scheduler:unscheduleScriptEntry(sid3)
--     print("s333333")
-- end,0,false)



local myScheduler = {}
local delayIDs = nil
local function add(func)
	myScheduler[#myScheduler + 1] = func
end
local function remove(func)
	if delayIDs then
		delayIDs[#delayIDs + 1] = func
		return
	end
	for i,v in ipairs(myScheduler) do
		if func == v then
			table.remove(myScheduler,i)
			break
		end
	end
end
local function tick(dt)
	delayIDs = {}
	for i,v in ipairs(myScheduler) do
		v(dt)
	end
	for _,v in ipairs(delayIDs) do
		for i,v1 in ipairs(myScheduler) do
			if v == v1 then
				table.remove(myScheduler,i)
				break
			end
		end
	end
	delayIDs = nil
end
sid1 = function()
	print("s11111")
	remove(sid1)
end
sid2 = function()
	print("s22222")
	remove(sid2)
end
sid3 = function()
	print("s333333")
	remove(sid3)
end
add(sid3)
add(sid2)
add(sid1)
tick(0)