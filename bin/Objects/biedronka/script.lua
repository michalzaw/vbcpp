log_debug("Hello World from lua: biedronka")

counter = 0
direction = 1;

MouseButtons.MIDDLE_BUTTON2 = 21

log_debug("Left button: " .. MouseButtons.LEFT_BUTTON)
log_debug("Right button: " .. MouseButtons.RIGHT_BUTTON)
log_debug("Middle button: " .. MouseButtons.MIDDLE_BUTTON)

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

	if input:isKeyDown(Keys.KEY_K) then
		log_debug("KKKKKK")
	end

	if input:isMouseButtonPressed(MouseButtons.LEFT_BUTTON) then
		log_info("LLLLLL")
	end
end

function onChangeTransform()
	-- print("Biedronka: change transform")
end

function onInit()
	log_debug("Biedronka: initialized")

	log_debug("Type: " .. self:getType())
	if self:isActive() then
		log_debug("IsActive: true")
	else
		log_debug("IsActive: false")
	end
end

function onDestroy()
	log_debug("Biedronka: usuwamy obiekt")
end
      