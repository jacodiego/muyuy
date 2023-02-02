local Boot = nil
local timer = nil
local logo_image = nil
local background_image = nil

function initialize(boot_instance)
    Boot = boot_instance;
    background_image = videoManager:createImage('data/boot/background.jpg');
    logo_image = videoManager:createImage('data/boot/logo.png');
    timer = system.SystemTimer:new(15000, 0);
    background_image:draw();
    logo_image:setAlpha(0.0);
    logo_image:draw();
end

function update()
    timer:update();
    if(Boot:getState() == boot.BootState.Intro) then
        if(timer:getTimeXpirated() <= 4000) then
            logo_image:setAlpha(timer:getTimeXpirated() / 4000);
        end
        if(timer:getTimeXpirated() >= 11000) then
            logo_image:setAlpha( 1 - (timer:getTimeXpirated() - 11000) / 4000);
        end
        if (timer:isInitialized() and not timer:isRunning()) then
            timer:run();
        elseif (timer:isFinished()) then
            logo_image:undraw();
            Boot:changeState(boot.BootState.Menu);
        end
    else
        timer:finish();
    end
end

function draw()
end