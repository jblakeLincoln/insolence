#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include <string>
#include <unordered_map>

template<typename... CallbackArgs>
struct CommandParser {
protected:
	char *last_command = nullptr;
	std::vector<std::string> keys;
	std::unordered_map<std::string, int(*)(CallbackArgs..., char*)> callbacks;

public:
	~CommandParser() {
		if(last_command == nullptr)
			free(last_command);
	}

	const char* GetLastCommand() const {
		return last_command;
	}

	void Register(const char *cmd, int(*func)(CallbackArgs..., char*)) {
		keys.push_back(cmd);
		callbacks[keys.back()] = func;
	}

	/**
	 * Split a provided console line into the command and arguments, and send them
	 * forward.
	 */
	int ParseCommand(const char *line, CallbackArgs ...other_args) {
		const char *args_temp = strchr(line, ' ');
		char *args = nullptr;
		char *cmd = nullptr;

		if(args_temp != NULL) {
			size_t cmd_len = args_temp - line;
			args = strdup(args_temp);
			cmd = static_cast<char*>(malloc(sizeof(char) * (cmd_len + 1)));

			strncpy(cmd, line, cmd_len);
			cmd[cmd_len] = '\0';
		}
		else
			cmd = strdup(line);

		if(last_command != nullptr)
			free(last_command);
		last_command = cmd;

		if(callbacks.count(cmd) == 0)
			return -1;

		char *args_ptr = args;
		while(*args_ptr == ' ')
			++args_ptr;

		int ret = callbacks[cmd](other_args..., args_ptr);

		free(args);

		return ret;
	}
};

struct ParseArgument {
	char err_buf[128];

	static bool Long(long *ret, char **args);
};

#endif
