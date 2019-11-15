#!/bin/sh

# This is one simple approach to ensuring your bot is always available.
# Here, we're just checking to see if "bot-start" is in the process list -
# if it isn't, we'll restart the bot using Drush. Since it's likely that
# the bot has crashed, we force a status reset to first clear the internal
# "connected" state. A script like this could be run through cron with:
#
#   * * * * * cd /path/to/bot/scripts && sh bot_check.sh >> /dev/null 2>&1

#if ! ps ax | grep -v grep | grep bot-start
if ! ps ax | grep -v grep | grep bot_start
then
  #drush -y bot-status-reset
  drush -r /var/www/html -l http://irc.farmos.org bot-status-reset
  #nohup drush bot-start >> bot.log &
  nohup php /var/www/html/sites/all/modules/bot/scripts/bot_start.php --root /var/www/html --url http://irc.farmos.org >> bot.log &
fi

