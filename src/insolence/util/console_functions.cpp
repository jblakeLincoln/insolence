static int Console_CreateEntity(char *args) {
	if(args != nullptr) {
		Game::Get()->console->Print("Expected no arguments");
		return 0;
	}

	Entity *e = Game::Get()->CreateEntity();
	Game::Get()->console->Print("Created entity with ID '%llu'", e->GetID());

	return 0;
}

static int Console_EntityHandler(char *args)
{
	if(args == nullptr) {
		Game::Get()->console->Print("Expected argument of entity ID");
		return 0;
	}

	long id;

	if(ParseArgument::Long(&id, &args) == false) {
		Game::Get()->console->Print("Unable to parse entity ID");
		return 0;
	}

	if(args == nullptr || strlen(args) == 0) {
		Game::Get()->console->Print("Expected entity function");
		return 0;
	}

	int ret = Game::Get()->console->entity_parser->ParseCommand(args, id);
	if(ret == -1)
	{
		Game::Get()->console->Print("Entity command '%s' not found",
				Game::Get()->console->entity_parser->GetLastCommand());
		ret = 0;
	}

	return ret;
}

static void RegisterConsoleFunctions()
{
	Game::Get()->console->parser->Register("create_entity", &Console_CreateEntity);
	Game::Get()->console->parser->Register("e", &Console_EntityHandler);
}
