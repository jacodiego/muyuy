local Boot = nil
local timer = nil
local logo_image = nil
local background_image = nil
local logoX = nil
local logoY = nil
local logoWidth = nil
local logoHeight = nil

function initialize(boot_instance)
    Boot = boot_instance;
    background_image = videoManager:createImage('data/boot/background.jpg');
    logo_splash_image = videoManager:createImage('data/boot/logo_splash.png');
    logo_image = videoManager:createImage('data/boot/logo.png');
    timer = system.SystemTimer:new(19000, 0);
    background_image:draw();
    logo_splash_image:setAlpha(0.0);
    logo_splash_image:draw();
end

function update()
    timer:update();
    if(Boot:getState() == boot.BootState.Intro) then        
        if (timer:isInitialized() and not timer:isRunning()) then
            timer:run();
        elseif (timer:isFinished()) then
            Boot:changeState(boot.BootState.Menu);
            return;
        end

        if(timer:getTimeXpirated() <= 3000) then
            logo_splash_image:setAlpha(timer:getTimeXpirated() / 3000);
        end
        if(timer:getTimeXpirated() >= 7000) then
            logo_splash_image:setAlpha( 1 - (timer:getTimeXpirated() - 7000) / 3000);
        end

        if(timer:getTimeXpirated() == 10000) then
            logo_splash_image:undraw();
            logo_image:setAlpha(0.0);
            logo_image:draw();
            logoX = logo_image:getX();
            logoY = logo_image:getY();
            logoWidth = logo_image:getWidth();
            logoHeight = logo_image:getHeight();
        end

        if(timer:getTimeXpirated() <= 12000) then
            logo_image:setAlpha((timer:getTimeXpirated() - 10000) / 2000);
        end

        if(timer:getTimeXpirated() >= 18000) then
            logo_image:move( (logoX - (logoX - 30) * (timer:getTimeXpirated() - 18000) / 1000), ( logoY - (logoY - 25) * (timer:getTimeXpirated() - 18000) / 1000 ) );
            logo_image:resize( math.floor(logoWidth - (logoWidth - logoWidth/1.5) * (timer:getTimeXpirated() - 18000) / 1000), math.floor(logoHeight - (logoHeight - logoHeight/1.5) * (timer:getTimeXpirated() - 18000) / 1000) );
        end

    else
        timer:finish();
    end
end

function draw()
end