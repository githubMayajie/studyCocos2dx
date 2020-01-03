local itemHeight = 100

local function test_UIButton()
    local btn = ccui.Button:create("HelloWorld.png")
    return btn
end

local function test_checkBox()
    local cb = ccui.CheckBox:create("checkbox_off_background.png","checkbox_on_background.png")
    cb:addEventListener(function(ref,eventType)
        print(ref,eventType)
    end)
    return cb
end

local function test_HBox()
    local hb = ccui.HBox:create(cc.size(display.width / 2,itemHeight))
    hb:setAnchorPoint(cc.p(0,0))
    if true then
        local btn = ccui.Button:create("checkbox_on_background.png")
        btn:addTo(hb)
    end
    if true then
        local btn = ccui.Button:create("checkbox_on_background.png")
        btn:addTo(hb)
    end
    if true then
        local btn = ccui.Button:create("checkbox_off_background.png")
        btn:addTo(hb)
    end
    return hb
end
local function test_VBox()
    local vb = ccui.VBox:create(cc.size(display.width / 2,itemHeight))
    vb:setAnchorPoint(cc.p(0,0))
    if true then
        local btn = ccui.Button:create("checkbox_on_background.png")
        btn:addTo(vb)
    end
    if true then
        local btn = ccui.Button:create("checkbox_on_background.png")
        btn:addTo(vb)
    end
    if true then
        local btn = ccui.Button:create("checkbox_off_background.png")
        btn:addTo(vb)
    end
    return vb
end
local function test_ImageView()
    local iv = ccui.ImageView:create("HelloWorld.png")
    return iv
end

local function test_Layout()
    local ly = ccui.Layout:create()
    ly:setContentSize(cc.size(display.width / 2,itemHeight))
    ly:setBackGroundColor(cc.c3b(255,255,0))
    ly:setBackGroundColorType(ccui.LayoutBackGroundColorType.solid)
    ly:setAnchorPoint(cc.p(0,0))
    return ly
end

local function test_ListView()
    local lv = ccui.ListView:create()
    lv:setDirection(ccui.ScrollViewDir.horizontal)
    lv:setContentSize(cc.size(display.width / 2,itemHeight))
    lv:setScrollBarEnabled(false)
    lv:setBounceEnabled(true)
    lv:addEventListener(function(ref,eventType) 
        print(ref,eventType)
    end)
    for i=1,20 do
        local btn = ccui.Button:create("checkbox_off_background.png")
        lv:pushBackCustomItem(btn)
    end
    return lv
end

local function test_LoadingBar()
    local lb = ccui.LoadingBar:create("HelloWorld.png")
    lb:setDirection(ccui.LoadingBarDirection.LEFT)
    local ang = 90
    lb:runAction(cc.RepeatForever:create(
                    cc.Sequence:create(
                        cc.DelayTime:create(0.1),
                        cc.CallFunc:create(function() 
                            lb:setPercent(math.abs( 100 * math.sin(math.rad(ang)) ))
                            ang = ang + 5
                        end)
                    )
                ))
    return lb
end

local function test_PageView()
    local pv = ccui.PageView:create()
    pv:setDirection(ccui.ScrollViewDir.vertical)
    local ivsize = ccui.ImageView:create("sliderThumb.png")
    local size = ivsize:getContentSize()
    pv:setContentSize(size)
    pv:addEventListener(function(ref,eventType) 
        print(ref,eventType,pv:getCurrentPageIndex())
    end)
    for i=1,3 do
        local iv = ccui.ImageView:create("sliderThumb.png")
        pv:addPage(iv)
    end
    pv:setAnchorPoint(cc.p(0,0))
    return pv
end

local function test_RadioButtonGroup()
    local group = ccui.RadioButtonGroup:create()
    group:setContentSize(cc.size(display.width / 2,itemHeight))
    for i=1,3 do
        local rb = ccui.RadioButton:create("checkbox_off_background.png","checkbox_on_background.png")
        rb:addEventListener(function(ref,event) 
            print("click RadioButton",i,ref,event)
        end)
        rb:addTo(group):move(50 + i * 50,itemHeight / 2)
        group:addRadioButton(rb)
    end
    group:addEventListener(function(radioButton,clickIndex,event) 
        print("click RadioButtonGroup",radioButton,clickIndex,event)
    end)
    return group
end

local function test_RelativeBox()
    local rl = ccui.RelativeBox:create()
    rl:setContentSize(cc.size(display.width / 2,itemHeight))

    local iv = ccui.ImageView:create("checkbox_off_background.png")
    iv:addTo(rl)
    local lp = iv:getLayoutParameter()
    lp:setAlign(ccui.RelativeAlign.alignParentLeftBottom)

    if true then
        local iv = ccui.ImageView:create("checkbox_off_background.png")
        iv:addTo(rl)
        local lp = iv:getLayoutParameter()
        lp:setAlign(ccui.RelativeAlign.alignParentTopRight)
    end
    return rl
end

local function test_RichText()
    local rt = ccui.RichText:create()
    if true then
        local ele = ccui.RichElementText:create(10,cc.c3b(255,255,120),255,"RichElementText","",16)
        rt:pushBackElement(ele)
    end
    if true then
        local ele = ccui.RichElementImage:create(10,cc.c3b(255,255,120),255,"checkbox_off_background.png")
        rt:pushBackElement(ele)
    end
    if true then
        local customNode = display.newLayer(cc.c4b(122,122,0,255),cc.size(display.width / 8,itemHeight))
        local ele = ccui.RichElementCustomNode:create(10,cc.c3b(255,255,120),255,customNode)
        rt:pushBackElement(ele)
    end
    rt:formatText()
    return rt
end

local function test_Scale9Sprite()
    -- local ss = ccui.Scale9Sprite:create(cc.rect(14,14,2,2),"checkbox_off_background.png")
    -- ss:setContentSize(cc.size(itemHeight - 10,itemHeight - 10))
    local ss = ccui.Scale9Sprite:create("HelloWorld.png")
    if true then
        local ss2 = ccui.Scale9Sprite:create("HelloWorld.png")
        ss2:setState(1)
        ss2:addTo(ss):move(195 * 2,270 / 2)
    end
    return ss
end

local function test_ScrollView()
    local sv = ccui.ScrollView:create()
    sv:setContentSize(cc.size(display.width / 2,itemHeight))
    sv:setDirection(ccui.ScrollViewDir.horizontal)
    sv:setInnerContainerSize(cc.size(display.width,itemHeight))
    sv:setScrollBarEnabled(false)
    sv:setBounceEnabled(true)
    sv:addEventListener(function(ref,eventType) 
        print(ref,eventType)
    end)
    for i=1,10 do
        local ss = ccui.Scale9Sprite:create(cc.rect(14,14,2,2),"checkbox_off_background.png")
        ss:setContentSize(cc.size(itemHeight - 10,itemHeight - 10))
        ss:move(display.width / 10 * i,itemHeight / 2)
        sv:addChild(ss)
    end
    return sv
end

local function test_Slider()
    local s = ccui.Slider:create("sliderProgress.png","sliderThumb.png")
    s:setPercent(20)
    s:addEventListener(function(ref,event) 
        print(s:getPercent(),ref,event) 
    end)
    return s
end

local function test_Text()
    local t = ccui.Text:create("Test","",52)
    t:setTextColor(cc.c4b(255,250,122,255))
    return t
end

local function test_EditBox()
    local size = cc.size(display.width / 2,itemHeight)
    local ss = ccui.Scale9Sprite:create(cc.rect(14,14,2,2),"checkbox_off_background.png")
    ss:setContentSize(size)
    local eb = ccui.EditBox:create(size,ss)
    eb:registerScriptEditBoxHandler(function(eventType) 
        print(eventType)
        if eventType == "ended" then
            print("result",eb:getText())
        end
    end)
    return eb
end

local function test_TextField()
    local size = cc.size(display.width / 2,itemHeight)
    local tf = ccui.TextField:create("plactholder","",24)
    tf:setContentSize(size)
    tf:setTouchSize(size)
    tf:addEventListener(function(ref,eventType) 
        print(ref,eventType)
    end)
    return tf
end

local function test_Widget()
    local w = ccui.Widget:create()
    w:setContentSize(cc.size(itemHeight,itemHeight))
    w:setTouchEnabled(true)
    w:addTouchEventListener(function(ref,eventType)
        print("touch",ref,eventType)
    end)
    w:addClickEventListener(function(ref)
        print("click",ref)
    end)
    return w
end

local allColor = {
    cc.c4b(255,0,0,255),
    cc.c4b(0,255,255,255),
    cc.c4b(255,0,255,255),
    cc.c4b(0,255,0,255),
}
local data = {
    {"Button",test_UIButton},
    {"CheckBox",test_checkBox},
    {"HBox",test_HBox},
    {"VBox",test_VBox},
    {"ImageView",test_ImageView},
    {"Layout",test_Layout},
    {"ListView",test_ListView},
    {"LoadingBar",test_LoadingBar},
    {"PageView",test_PageView},
    {"RadioButtonGroup",test_RadioButtonGroup},
    {"RelativeBox",test_RelativeBox},
    {"RichText",test_RichText},
    {"Scale9Sprite",test_Scale9Sprite},
    {"ScrollView",test_ScrollView},
    {"Slider",test_Slider},
    {"test",test_Text},
    {"EditBox",test_EditBox},
    {"TextField",test_TextField},
    {"widget",test_Widget},
}


local function createBase(color)
    local scene = display.newScene()
    local layer = display.newLayer(color)
    display.runScene(scene)
    layer:addTo(scene):move(0,0)
    local label = cc.Label:createWithSystemFont("", "Arial", 40)
    label:addTo(layer,100):move(display.cx, display.cy + 200)

    local tableView = cc.TableView:create(display.size);
    local function tablecellSizeForIndex(tbview, idx)
        return display.width, itemHeight;
    end

    local function numberOfCellsInTableView() 
    	return #data
    end

    local function tablecellSizeAtIndex(tbview, idx) 
        local index = idx + 1;
        local cell = tbview:dequeueCell();
        if not cell then
            cell = cc.TableViewCell:create();  
        else
            cell:removeAllChildren(true)
        end
        
        local layer = display.newLayer(allColor[index % 4 + 1],cc.size(display.width,itemHeight))
        layer:addTo(cell):move(0,0)

        local itemData = data[index] 
        local itemTitle = cc.Label:createWithSystemFont(itemData[1], "Arial", 40)
        itemTitle:addTo(cell):align(cc.p(display.LEFT_CENTER),100, itemHeight / 2)

        local itemAct = itemData[2]()
        local size = itemAct:getContentSize()
        local scale = 1
        if size.height > itemHeight then
            scale = itemHeight / size.height
            itemAct:setScale(scale)
        end
        itemAct:addTo(cell):align(cc.p(display.CENTER),display.cx + size.width * scale * 0.5, itemHeight / 2)
        return cell
    end
    tableView:setDirection(cc.SCROLLVIEW_DIRECTION_VERTICAL) --设置滑动方向
    tableView:setVerticalFillOrder(cc.TABLEVIEW_FILL_TOPDOWN) --设置填充方向
    tableView:registerScriptHandler(tablecellSizeForIndex, cc.TABLECELL_SIZE_FOR_INDEX)
    tableView:registerScriptHandler(numberOfCellsInTableView, cc.NUMBER_OF_CELLS_IN_TABLEVIEW)
    tableView:registerScriptHandler(tablecellSizeAtIndex, cc.TABLECELL_SIZE_AT_INDEX)
    tableView:addTo(layer)
    tableView:reloadData();
end
createBase(cc.c4b(122,122,122,255))

