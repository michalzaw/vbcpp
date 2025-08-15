print("Hello World from lua. Script 2")

counter = 0
direction = 1;

function update(deltaTime)
	counter = counter + 1;
	if counter > 500 then
		counter = 0
		direction = -direction
	end

	print("Jezioro update: " .. counter)
	print("sinus=" .. math.sin(counter))

	biedronkaObject = sceneManager:getSceneObject("jezioro")
	biedronkaObject:move(2 * deltaTime, 0, 0)
end
