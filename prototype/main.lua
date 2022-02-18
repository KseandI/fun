

local win =
   {
      ["width"] = 0,
      ["height"] = 0,
   }

local player =
   {
      ["x"] = 0,
      ["y"] = 0,
   }

function love.load()
   
   win.width  = love.graphics.getWidth()
   win.height = love.graphics.getHeight()

end

function love.keypressed(key, unicode)
   
end

function love.update(dt)
   
end

function love.draw()
   love.graphics.setBackgroundColor(0, 255, 255)

   love.graphics.setColor(255, 0, 0)
   love.graphics.rectangle("fill", player.x, player.y, 32, 32)
end
