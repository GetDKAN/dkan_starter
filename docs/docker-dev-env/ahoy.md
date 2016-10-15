# Ahoy

<img src="https://camo.githubusercontent.com/3c5f11de213d31ad57f73be305d05f37d3feade3/687474703a2f2f6936352e74696e797069632e636f6d2f76717277676c2e706e67" alt="ahoy logo"/>

[Ahoy](https://github.com/DevinciHQ/ahoy) is a small open source tool for creating cli apps like drush, drupal-console, or custom bash scripts. Written in Go and compiled down to a single binary, it has no dependencies which make it fast and easy to install.

See [Ahoy's documentation](https://github.com/DevinciHQ/ahoy) for more details.

## Why Ahoy?
We use ahoy to help us do the following:
* Standardize workflow steps so that everyone is doing things exactly the same way
* Abstract more complex workflow steps and scripts behind higher level commands, which:
* Makes it easier for developers, as they can see a list of commands available along with descriptions of each
* Makes it easier for the devops team to improve the processes without having to retrain developers or change documentation
* Reuse abstracted commands in scripts like CircleCI, or even other ahoy commands.
* Make it easier for anyone to add or tweak the commands without knowledge of bash, php, etc
* Allow projects (or users) to have their own custom commands in addition to the defaults.

## DKAN and Ahoy

DKAN's ahoy commands are stored in the ``.ahoy/`` folder in DKAN.




## DKAN Starter and Ahoy

## Basic Tips
Some things to keep in mind when using ahoy:
You always need a .ahoy.yml file - This is where ahoy gets it's configuration. If the current directory doesn't have that file, it will recursively look at all the parent directories for one until it either finds it, or fails with an error. This means that each project should have an ahoy file at it's root to work, but you can be in any subdirectory and ahoy will still find the right file.
Commands are always run from the directory where .ahoy.yml is - That's really helpful because no matter where you run ahoy from, the commands will be run from a consistent directory.
Bash is what is actually running the commands - everything that's within a "cmd" definition is piped into bash, so whatever you can do with bash, you can do in an ahoy command if you want to create something more complex than a single one-line command. This also means that each command runs in a bash subshell, which is usually fine since all environment variables are copied in, but you won't be able to affect the parent shell.. for example, changing the user's current directory or ENV variables.
Easily debug using --verbose - You can always get the details of what's actually being run in a command with the -v or the --verbose flag.
Subcommands come from imported ahoy.yml files - You can import another command files that use the ahoy yaml format as subcommands. This is useful to split up types of commands into different files and so the list of commands isn't as long. For example, we do this with the dkan command, which just imports dkan/.ahoy/dkan.ahoy.yml. All those commands are then listed by typing `ahoy dkan`
Ahoy uses the {{args}} placeholder with a commands arguments - Similar to Drupal templates, ANY arguments added after a command are passed into {{args}}. If you use {{args}} in your command, the actual arguments will be swapped out before the command is run. If {{args}} is used multiple times in a command, all instances are replaced. This is necessary so we can pass arguments along into the script, but adds a lot of flexibility.
You can use ahoy commands within other commands - This is really powerful! You can define helper commands to further abstract where commands are run (ie. locally vs ssh, vs docker), or simple utilities like ahoy confirm "question that will prompt the user for a yes or no answer" . You can think of these kind of like reusable functions. If you want to hide these utility commands, you can set `hide: true` in your ahoy file.
Quotes can be tricky - Sometimes when passing one command into subcommands, you might "loose" your quotes. Try using --verbose to debug what's happening first, and experiment with both single and double quotes. Keep in mind how the yaml spec processes and escapes quotes. We recommend not starting your command with quotes unless necessary. Multi-line commands (scripts) are best done using `cmd: |` which allows you to use multiple lines without worrying about quotes.
Using Environment variables - You can use environment variables from within ahoy commands, but you sometimes need to pay attention to quotes, especially if the ENV variable you intend to use is from another machine (docker, ssh).
Check your yaml formatting - The script will check your yaml formatting and throw an error if it's not right, but it doesn't check everything. Make sure your whitespace and structure are correct if you get yaml errors.
Examples
Note that the best place for up to date ahoy documentation is on the ahoy wiki: https://github.com/devinci-code/ahoy/wiki
We'll show some examples here to get a feel for things though.
Creating a simple command
The simplest way to start using ahoy is to create a basic .ahoy.yml file in your current directory like so:
ahoyapi: v1
usage: DKAN cli app for development using ahoy.
commands:
  echo:
    usage: Simply echo all the arguments
    # Note that {{args}} will be replaced with the string of all arguments passed
    cmd: echo "{{args}}"

Now if we simply run ahoy, it will find that file and output the help text and a list of commands.
$ ahoy
NAME:
   ahoy - DKAN cli app for development using ahoy.
USAGE:
   ahoy [global options] command [command options] [arguments...]
COMMANDS:
   echo	Simply echo all the arguments
   init	Initialize a new .ahoy.yml config file in the current directory.
GLOBAL OPTIONS:
   --verbose, -v		Output extra details like the commands to be run. [$AHOY_VERBOSE]
   --file, -f 			Use a specific ahoy file.
   --help, -h			show help
   --version			print the version
   --generate-bash-completion
VERSION:
   0.0.0
Now if we call `ahoy -v echo "Do I hear an echo?"`
ahoy -v echo "Do I hear an echo?"
2016/01/13 14:03:54 ===> AHOY echo from  : echo "Do I hear an echo?"
Do I hear an echo?

Writing More Complex Commands
Let's show an example of using bash scripts AND reusing ahoy commands
ahoyapi: v1
commands:
  confirm:
    cmd: |
      read -r -p "{{args}} [y/N] " response
      if [ $response = y ]; then
        true
      else
        false
      fi
    # This will keep the confirm command from showing up in the help text.
    hide: true
  meaning-of-life:
    cmd: |
      ahoy confirm "Are you sure you want to know?" &&
      # Run this if confirm returns true
      echo The meaning of life is 42 ||
      # Run this if confirm returns false
      echo "OK, you don't want to know, skipping..."

$ ahoy meaning-of-life
Are you sure you want to know? [y/N] y
The meaning of life is 42

$ ahoy meaning-of-life
Are you sure you want to know? [y/N] n
OK, you don't want to know, skipping...

Importing commands from other ahoy files.
Another powerful feature is importing commands from other files.
Subcommands
Ahoy allows you to import an entire yml file full of commands by using `import: relative path to file` instead of `cmd`. This is useful to organize commands into groups.
Direct import
You can also import single commands by calling ahoy and setting the path of the .ahoy file you want to use using the -f flag.
ahoyapi: v1
commands:
  whoami:
    #Simple unix command that displays the logged in user.
    cmd: whoami

ahoyapi: v1
commands:
  # Imports a single ahoy command called whoami and changes the name to direct example
  direct-example:
    usage: Runs the whoami command directly
    cmd: ahoy -f .ahoy/3-imports-sub.ahoy.yml whoami
  # Imports all commands in the file
  import-example:
    usage: Loads all commands in a subfile.
    import: .ahoy/3-imports-sub.ahoy.yml

$ ahoy direct-example
fcarey #or whatever your user name is

$ ahoy import-example whoami
fcarey #or whatever your user name is

#Shows help text for imported subcommands
$ahoy import-example
NAME:
   ahoy - Creates a configurable cli app for running commands.
USAGE:
   ahoy [global options] command [command options] [arguments...]
COMMANDS:
   whoami
   init		Initialize a new .ahoy.yml config file in the current directory.
GLOBAL OPTIONS:
   --verbose, -v		Output extra details like the commands to be run. [$AHOY_VERBOSE]
   --file, -f 			Use a specific ahoy file.
   --help, -h			show help
   --version			print the version
   --generate-bash-completion
VERSION:
   0.0.0
