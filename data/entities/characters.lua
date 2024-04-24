
characters = {
    kumelen = {
        character = {
            name = "Kumelen"
        },
        movement = {},
        position = {},
        rotation = {
            direction = 'south'
        },
        sprite = {
            image_filename = "data/entities/sprites/kumelen.png",
            width = 24,
            height = 32,
            rows = 4,
            cols = 3
        },
        animation = {
            idle_north = {
                [0] = { id = 2, duration = 250 },
            },
            idle_east = {
                [0] = { id = 5, duration = 250 },
            },
            idle_south = {
                [0] = { id = 8, duration = 250 },
            },
            idle_west = {
                [0] = { id = 11, duration = 250 },
            },
            walking_north = {
                [0] = { id = 1, duration = 200 },
                [1] = { id = 2, duration = 200 },
                [2] = { id = 3, duration = 200 },
                [3] = { id = 2, duration = 200 },
            },
            walking_east = {
                [0] = { id = 4, duration = 200 },
                [1] = { id = 5, duration = 200 },
                [2] = { id = 6, duration = 200 },
                [3] = { id = 5, duration = 200 },
            },
            walking_south = {
                [0] = { id = 7, duration = 200 },
                [1] = { id = 8, duration = 200 },
                [2] = { id = 9, duration = 200 },
                [3] = { id = 8, duration = 200 },
            },
            walking_west = {
                [0] = { id = 10, duration = 200 },
                [1] = { id = 11, duration = 200 },
                [2] = { id = 12, duration = 200 },
                [3] = { id = 11, duration = 200 },
            },
            running_north = {
                [0] = { id = 1, duration = 100 },
                [1] = { id = 2, duration = 100 },
                [2] = { id = 3, duration = 100 },
                [3] = { id = 2, duration = 100 },
            },
            running_east = {
                [0] = { id = 4, duration = 100 },
                [1] = { id = 5, duration = 100 },
                [2] = { id = 6, duration = 100 },
                [3] = { id = 5, duration = 100 },
            },
            running_south = {
                [0] = { id = 7, duration = 100 },
                [1] = { id = 8, duration = 100 },
                [2] = { id = 9, duration = 100 },
                [3] = { id = 8, duration = 100 },
            },
            running_west = {
                [0] = { id = 10, duration = 100 },
                [1] = { id = 11, duration = 100 },
                [2] = { id = 12, duration = 100 },
                [3] = { id = 11, duration = 100 },
            },
        }
    }
    -- sayen = {
    --     character = {
    --         name = "Sayen"
    --     },
    --     position = {}
    -- }
};