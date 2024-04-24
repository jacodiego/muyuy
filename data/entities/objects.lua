
objects = {
    campfire = {
        collisionable = {},
        position = {},
        sprite = {
            image_filename = "data/entities/objects/campfire.png",
            width = 64,
            height = 64,
            rows = 1,
            cols = 5
        },
        animation = {
            [0] = { id = 0, duration = 150 },
            [1] = { id = 1, duration = 150 },
            [2] = { id = 2, duration = 150 },
            [3] = { id = 3, duration = 150 },
            [4] = { id = 4, duration = 150 }
        }
    }
};

createObject = function(Map, name, x, y)
    if (objects[name] == nil) then
        print("Error: No object named: "..name.." found!!");
        return nil;
    end

    if (Map == nil) then
        print("Error: Function called with invalid Map object");
        return nil;
    end

    local object = Map:createObject(objects[name], x , y);

    return object;
end