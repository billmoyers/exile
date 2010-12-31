function createWorld(world)
	world.name = "Test World"
	world.desc = "A test world."
	
	local map = Exile.Model.Map(10, 10)
	
	map:addTile(Exile.Model.Tile("grass", Exile.Model.Tile.GRASS, 0))
	map:addTile(Exile.Model.Tile("mud", Exile.Model.Tile.MUD, 0))
	map:addTile(Exile.Model.Tile("snow-heavy", Exile.Model.Tile.SNOW, 0))
	map:addTile(Exile.Model.Tile("snow-light", Exile.Model.Tile.SNOW, 1))
	map:addTile(Exile.Model.Tile("stone-floor", Exile.Model.Tile.PAVED, 0))
	map:addTile(Exile.Model.Tile("stone-wall", Exile.Model.Tile.WALL, 0))
	map:generate()
	
	world:addMap(map)
	
	for i = 0, 9 do
		local e = Exile.Model.Entity()
		map:setObjectLocation(e, i, i)
	end
	
	return world
end