FROM drupal:7

# Install Net_SmartIRC.
RUN pear install Net_SmartIRC

# Install mariadb-client (for Drush).
RUN apt-get update && apt-get install -y mariadb-client

# Install Drush.
ENV DRUSH_VERSION 8.3.1
RUN curl -fsSL -o /usr/local/bin/drush "https://github.com/drush-ops/drush/releases/download/${DRUSH_VERSION}/drush.phar" && \
  chmod +x /usr/local/bin/drush && \
  drush core-status

# Configure cron to keep the bot running.
RUN apt-get update && apt-get install -y cron \
  && echo "PATH=/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin" >> /etc/cron.d/bot-cron \
  && echo "* * * * * root cd /var/www/html/sites/all/modules/bot/scripts && sh bot_check.sh >> /dev/null 2>&1\n" >> /etc/cron.d/bot-cron \
  && chmod 0644 /etc/cron.d/bot-cron \
  && crontab /etc/cron.d/bot-cron

# Start the cron service via an entrypoint script.
COPY docker-entrypoint.sh /usr/local/bin/
RUN chmod u+x /usr/local/bin/docker-entrypoint.sh
ENTRYPOINT ["docker-entrypoint.sh"]
CMD ["apache2-foreground"]

