## ----------------------------------------------------------------------------------- ##
##                                                                                     ##
## EPITECH PROJECT - Wed, Oct, 2024                                                    ##
## Title           - Client                                                            ##
## Description     -                                                                   ##
##     Makefile                                                                        ##
##                                                                                     ##
## ----------------------------------------------------------------------------------- ##
##                                                                                     ##
##             ███████╗██████╗ ██╗████████╗███████╗ ██████╗██╗  ██╗                    ##
##             ██╔════╝██╔══██╗██║╚══██╔══╝██╔════╝██╔════╝██║  ██║                    ##
##             █████╗  ██████╔╝██║   ██║   █████╗  ██║     ███████║                    ##
##             ██╔══╝  ██╔═══╝ ██║   ██║   ██╔══╝  ██║     ██╔══██║                    ##
##             ███████╗██║     ██║   ██║   ███████╗╚██████╗██║  ██║                    ##
##             ╚══════╝╚═╝     ╚═╝   ╚═╝   ╚══════╝ ╚═════╝╚═╝  ╚═╝                    ##
##                                                                                     ##
## ----------------------------------------------------------------------------------- ##

BUILD_DIR = build
EXECUTABLE = my_hunter

COLOR_RESET = \033[0m
COLOR_RED = \033[31m
COLOR_GREEN = \033[32m
COLOR_YELLOW = \033[33m
COLOR_BLUE = \033[34m

all: cmake_build

debug:
	@$(MAKE) cmake_build CMAKE_BUILD_TYPE=Debug

cmake_build:
	@echo -e "$(COLOR_GREEN)Building the project...$(COLOR_RESET)"
	@mkdir -p $(BUILD_DIR)
	@cd $(BUILD_DIR) && cmake .. && $(MAKE)
	@echo -e "$(COLOR_GREEN)Build complete! Executable moved to root directory.$(COLOR_RESET)"

clean:
	@echo -e "$(COLOR_YELLOW)Cleaning temporary files...$(COLOR_RESET)"
	@cd $(BUILD_DIR) && $(MAKE) clean
	@rm -f r-type_client-package.zip
	@echo -e "$(COLOR_YELLOW)Temporary files cleaned.$(COLOR_RESET)"

fclean:
	@echo -e "$(COLOR_RED)Fully cleaning build directory and executable...$(COLOR_RESET)"
	@rm -rf $(BUILD_DIR)
	@rm -f $(EXECUTABLE)
	@echo -e "$(COLOR_RED)Full clean complete.$(COLOR_RESET)"

.PHONY: all debug cmake_build clean fclean
