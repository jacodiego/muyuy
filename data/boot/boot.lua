local Boot = nil
local timer = nil
local logo_image = nil
local background_image = nil
local logo_splash_image = nil
local logo_splash_alpha = nil
local logo_alpha = nil
local logo_scale = nil
local logo_x = nil
local logo_y = nil
local intro_finished = nil

function initialize(boot_instance)
    Boot = boot_instance;

    background_image = videoManager:createImage('data/boot/background.jpg');
    logo_splash_image = videoManager:createImage('data/boot/logo_splash.png');
    logo_image = videoManager:createImage('data/boot/logo.png');

    logo_splash_alpha = 0;
    logo_alpha = 0;
    logo_scale = 1;
    logo_x = math.floor(videoManager:getWindowWidth() / 2 - logo_image:getWidth() / 2);
    logo_y = math.floor(videoManager:getWindowHeight() / 2 - logo_image:getHeight() /2);

    timer = system.SystemTimer:new(15000, 0);
    intro_finished = false;
end

function update()
    timer:update();
    if(Boot:getState() == boot.BootState.Intro) then        
        if (timer:isInitialized() and not timer:isRunning()) then
            timer:run();
        elseif (timer:isFinished()) then
            intro_finished = true;
            Boot:changeState(boot.BootState.Menu);
            return;
        end

        if(timer:getTimeXpirated() <= 2000) then
            logo_splash_alpha = timer:getTimeXpirated() / 2000;
        end
        if(timer:getTimeXpirated() >= 5000) then
            logo_splash_alpha = 1 - (timer:getTimeXpirated() - 5000) / 2000;
        end
        if(timer:getTimeXpirated() >= 7000 and timer:getTimeXpirated() <= 9000) then
            logo_alpha = (timer:getTimeXpirated() - 7000) / 2000;
        end

        if(timer:getTimeXpirated() >= 12000 and timer:getTimeXpirated() <= 13000) then
            logo_x = math.floor(videoManager:getWindowWidth() / 2 - logo_image:getWidth() / 2 - (videoManager:getWindowWidth() / 2 - logo_image:getWidth() / 2 + 130) * (timer:getTimeXpirated() - 12000) / 1000);
            logo_y = math.floor(videoManager:getWindowHeight() / 2 - logo_image:getHeight() /2 - (videoManager:getWindowHeight() / 2 - logo_image:getHeight() + 200) * (timer:getTimeXpirated() - 12000) / 1000);
            logo_scale = 1 - 0.3 * (timer:getTimeXpirated() - 12000) / 1000;
        end

    else
        timer:finish();
        if(not intro_finished) then
            logo_x = math.floor(videoManager:getWindowWidth() / 2 - logo_image:getWidth() / 2 - (videoManager:getWindowWidth() / 2 - logo_image:getWidth() / 2 + 130));
            logo_y = math.floor(videoManager:getWindowHeight() / 2 - logo_image:getHeight() /2 - (videoManager:getWindowHeight() / 2 - logo_image:getHeight() + 200));
            logo_scale = 0.7;
            logo_alpha = 1;
            intro_finished = true;
        end
    end
end

function draw()
    background_image:draw(video.ScreenPosition.Center, 0, 0, background_image:getWidth(), background_image:getHeight(), 0.8, 1.0, 0.0);
    if(Boot:getState() == boot.BootState.Intro) then       
        if(timer:getTimeXpirated() <= 7000) then
            logo_splash_image:draw(video.ScreenPosition.Center, 0, 0, logo_splash_image:getWidth(), logo_splash_image:getHeight(), logo_splash_alpha, 1.0, 0.0);
        else
             logo_image:draw(logo_x, logo_y, 0, 0, logo_image:getWidth(), logo_image:getHeight(), logo_alpha, logo_scale, 0.0);
        end
    else
        logo_image:draw(logo_x, logo_y, 0, 0, logo_image:getWidth(), logo_image:getHeight(), logo_alpha, logo_scale, 0.0);
    end

end