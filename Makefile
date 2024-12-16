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

COLOR_RESET = \033[0m
COLOR_RED = \033[31m
COLOR_YELLOW = \033[33m

all: build

build: client server

client:
	@make -C Client/
	@mv Client/r-type_client-package.zip .
	@unzip -o r-type_client-package.zip

server:
	@make -C Server/
	@mv Server/r-type_server-package.zip .
	@unzip -o r-type_server-package.zip


clean:
	@echo -e "$(COLOR_YELLOW)Cleaning temporary files...$(COLOR_RESET)"
	@make -C Client/ clean
	@make -C Server/ clean
	@rm -f r-type_client-package.zip
	@rm -f r-type_server-package.zip
	@echo -e "$(COLOR_YELLOW)Temporary files cleaned.$(COLOR_RESET)"

fclean:	clean
	@echo -e "$(COLOR_RED)Fully cleaning build directory and executable...$(COLOR_RESET)"
	@make -C Client/ fclean
	@make -C Server/ fclean
	@rm -f r-type_client
	@rm -f r-type_server
	@rm -rf assets
	@rm -rf config
	@rm -rf scripts
	@echo -e "$(COLOR_RED)Full clean complete.$(COLOR_RESET)"

.PHONY: all build server client fclean
