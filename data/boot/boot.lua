local Boot = nil
local Script = nil
local timer = nil

function initialize(boot_instance)
    Boot = boot_instance;
    Script = Boot:getScriptSupervisor();
    timer = system.SystemTimer:new(7000, 0);
    --print('boot initialized from lua: ', timer);
end

function update()
    timer:update();
    if(Boot:getState() == boot.BootState.Intro) then
        if (timer:isInitialized() and not timer:isRunning()) then
            timer:run();
        elseif (timer:isFinished()) then
            Boot:changeState(boot.BootState.Menu);
        end
    else
        timer:finish();
    end

    --print('update state: ', Boot:getState());
end