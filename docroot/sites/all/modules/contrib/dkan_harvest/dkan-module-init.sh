set -e

# Name of the current module.
DKAN_MODULE="dkan_harvest"

# DKAN branch to use
DKAN_BRANCH="dev-dkan-ahoy-smarter"

COMPOSER_PATH=".composer/vendor/bin"

#Fix for probo not setting the composer path.
if [[ "$PATH" != *"$COMPOSER_PATH"* ]]; then
  echo "> Composer PATH is not set. Adding temporarily.. (you should add to your .bashrc)"
  echo "PATH (prior) = $PATH"
  export PATH="$PATH:~/$COMPOSER_PATH"
fi

wget -q -O - https://raw.githubusercontent.com/NuCivic/dkan/dev-dkan-ahoy-smarter/dkan-init.sh | bash /dev/stdin $DKAN_MODULE $@ --skip-reinstall

ahoy dkan module-link $DKAN_MODULE
ahoy dkan module-make $DKAN_MODULE
ahoy dkan reinstall
ahoy drush en migrate dkan_migrate_base $DKAN_MODULE -y

echo ""
echo "-------> Run harvester:"
echo ""
ahoy drush cc all
ahoy drush dkan-hr
