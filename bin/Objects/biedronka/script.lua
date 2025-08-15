log_debug("Hello World from lua: biedronka")

counter = 0
direction = 1;

function onUpdate(deltaTime)
	counter = counter + 1;
	if counter > 500 then
		counter = 0
		direction = -direction

		jezioroObject = sceneManager:getSceneObject("jezioro")
		jezioroObject:rotate(math.rad(90), 0, 0)
	end

	-- print("Jetem biedronka: " .. counter)

	--biedronkaObject = sceneManager:getSceneObject("biedronka")
	biedronkaObject = self:getSceneObject()
	biedronkaObject:move(2 * deltaTime, 0, 0)
	biedronkaObject:rotate(0, 2 * direction * deltaTime, 0)
end

function onChangeTransform()
	-- print("Biedronka: change transform")
end

function onInit()
	log_debug("Biedronka: initialized")
end
