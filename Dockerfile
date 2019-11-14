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

