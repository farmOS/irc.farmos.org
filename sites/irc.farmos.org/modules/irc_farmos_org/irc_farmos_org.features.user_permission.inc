<?php
/**
 * @file
 * irc_farmos_org.features.user_permission.inc
 */

/**
 * Implements hook_user_default_permissions().
 */
function irc_farmos_org_user_default_permissions() {
  $permissions = array();

  // Exported permission: 'view bot logs'.
  $permissions['view bot logs'] = array(
    'name' => 'view bot logs',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bot_log',
  );

  // Exported permission: 'view bot logs: #farmOS'.
  $permissions['view bot logs: #farmOS'] = array(
    'name' => 'view bot logs: #farmOS',
    'roles' => array(
      'anonymous user' => 'anonymous user',
      'authenticated user' => 'authenticated user',
    ),
    'module' => 'bot_log',
  );

  return $permissions;
}
