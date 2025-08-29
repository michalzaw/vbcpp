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

		--jezioroObject = sceneManager:getSceneObject("jezioro")
		--jezioroObject:rotate(math.rad(90), 0, 0)

		local scriptComponent = self:getSceneObject():getScriptComponent()
		--scriptComponent:setIsActive(false)

		if scriptComponent == self then
			log_debug("wszystko ok")
			if scriptComponent:isInitialized() then
				log_debug("isInitialized: true")
			else
				log_debug("isInitialized: false")
			end
		else
			log_debug("cos to nie dziala")
		end
	end

	-- print("Jetem biedronka: " .. counter)

	--biedronkaObject = sceneManager:getSceneObject("biedronka")
	biedronkaObject = self:getSceneObject()
	--biedronkaObject:move(2 * deltaTime, 0, 0)
	--biedronkaObject:rotate(0, 2 * direction * deltaTime, 0)

	local position = biedronkaObject:getPosition()
	position.x = position.x + 2 * deltaTime;
	--biedronkaObject:setPosition(position);

	local rotation = biedronkaObject:getRotation()
	rotation.y = rotation.y + 2 * direction * deltaTime
	--biedronkaObject:setRotation(rotation.x, rotation.y, rotation.z)

	if input:isKeyPressed(Keys.KEY_K) then
		log_debug("KKKKKK")

		local renderObject = self:getSceneObject():getRenderObject()
		--local renderObjectComponent = renderObject:as(RenderObject);
		if renderObject == nil then
			log_error("NULL")
		else
			log_error("NIE NULL")
			log_error("type: " .. type(renderObject))
			renderObject:setCastShadows(not renderObject:isCastShadows())
		end
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

	log_debug("Object id: " .. self:getSceneObject():getId())

	log_debug("Components counts: " .. self:getSceneObject():getComponentsCount())

	for i = 0, self:getSceneObject():getComponentsCount() - 1 do
		local component = self:getSceneObject():getComponent(i)
		log_debug("- " .. component:getType())
	end

	local position = self:getSceneObject():getPosition()
	log_debug("position.x=" .. position.x)
	log_debug("position.y=" .. position.y)
	log_debug("position.z=" .. position.z)

	local rotation = self:getSceneObject():getRotation()
	log_debug("rotation.x=" .. math.deg(rotation.x))
	log_debug("rotation.y=" .. math.deg(rotation.y))
	log_debug("rotation.z=" .. math.deg(rotation.z))

	local scale = self:getSceneObject():getScale()
	log_debug("scale.x=" .. scale.x)
	log_debug("scale.y=" .. scale.y)
	log_debug("scale.z=" .. scale.z)

	local normal = vec3.new(1.0, 2.0, 3.0)
	log_debug("Dot: " .. dot(normal, vec3.new(2.0, 2.0, 2.0)))
	local cross = cross(normal, vec3.new(2.0, 2.0, 2.0))
	log_debug("Cross: (" .. cross.x .. ", " .. cross.y .. ", " .. cross.z .. ")")
	log_debug("Length: " .. length(normal))
	log_debug("Distance: " .. distance(normal, vec3.new(2.0, 2.0, 2.0)))

	normal = normalize(normal)
	log_debug("Normal.x:" .. normal.x)
	log_debug("Normal.y:" .. normal.y)
	log_debug("Normal.z:" .. normal.z)
end

function onDestroy()
	log_debug("Biedronka: usuwamy obiekt")
end
      