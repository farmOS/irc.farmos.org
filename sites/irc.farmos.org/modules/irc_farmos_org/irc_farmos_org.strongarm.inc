<?php

/**
 * @file
 * irc_farmos_org.strongarm.inc
 */

/**
 * Implements hook_strongarm().
 */
function irc_farmos_org_strongarm() {
  $export = array();

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_auto_reconnect';
  $strongarm->value = 1;
  $export['bot_auto_reconnect'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_auto_retry';
  $strongarm->value = 1;
  $export['bot_auto_retry'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_channels';
  $strongarm->value = '#farmOS';
  $export['bot_channels'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_debugging';
  $strongarm->value = 0;
  $export['bot_debugging'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_ghost';
  $strongarm->value = '/msg NickServ GHOST !bot_nickname !bot_password';
  $export['bot_ghost'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_identify';
  $strongarm->value = '/msg NickServ IDENTIFY !bot_password';
  $export['bot_identify'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_log_channels';
  $strongarm->value = array(
    '#farmOS' => '#farmOS',
  );
  $export['bot_log_channels'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_nickname';
  $strongarm->value = 'farmBOT';
  $export['bot_nickname'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_real_sockets';
  $strongarm->value = 1;
  $export['bot_real_sockets'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_server';
  $strongarm->value = 'irc.oftc.net';
  $export['bot_server'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'bot_server_port';
  $strongarm->value = '6667';
  $export['bot_server_port'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_frontpage';
  $strongarm->value = 'bot/log/farmOS';
  $export['site_frontpage'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_mail';
  $strongarm->value = 'mike@farmier.com';
  $export['site_mail'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'site_name';
  $strongarm->value = 'irc.farmos.org';
  $export['site_name'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_bootstrap_settings';
  $strongarm->value = array(
    'bootstrap__active_tab' => 'edit-general',
    'bootstrap_anchors_fix' => '0',
    'bootstrap_anchors_smooth_scrolling' => '0',
    'bootstrap_breadcrumb' => '1',
    'bootstrap_breadcrumb_home' => 0,
    'bootstrap_breadcrumb_title' => 1,
    'bootstrap_button_colorize' => 1,
    'bootstrap_button_iconize' => 1,
    'bootstrap_button_size' => 'btn-lg',
    'bootstrap_cdn_custom_css' => 'https://cdn.jsdelivr.net/bootstrap/3.3.7/css/bootstrap.css',
    'bootstrap_cdn_custom_css_min' => 'https://cdn.jsdelivr.net/bootstrap/3.3.7/css/bootstrap.min.css',
    'bootstrap_cdn_custom_js' => 'https://cdn.jsdelivr.net/bootstrap/3.3.7/js/bootstrap.js',
    'bootstrap_cdn_custom_js_min' => 'https://cdn.jsdelivr.net/bootstrap/3.3.7/js/bootstrap.min.js',
    'bootstrap_cdn_jsdelivr_theme' => 'yeti',
    'bootstrap_cdn_jsdelivr_version' => '3.3.7',
    'bootstrap_cdn_provider' => 'jsdelivr',
    'bootstrap_fluid_container' => 1,
    'bootstrap_forms_has_error_value_toggle' => 1,
    'bootstrap_forms_required_has_error' => 0,
    'bootstrap_forms_smart_descriptions' => 1,
    'bootstrap_forms_smart_descriptions_allowed_tags' => 'b, code, em, i, kbd, span, strong',
    'bootstrap_forms_smart_descriptions_limit' => '250',
    'bootstrap_image_responsive' => 1,
    'bootstrap_image_shape' => '',
    'bootstrap_navbar_inverse' => 0,
    'bootstrap_navbar_position' => 'static-top',
    'bootstrap_pager_first_and_last' => 1,
    'bootstrap_popover_animation' => 1,
    'bootstrap_popover_container' => 'body',
    'bootstrap_popover_content' => '',
    'bootstrap_popover_delay' => '0',
    'bootstrap_popover_enabled' => 1,
    'bootstrap_popover_html' => 0,
    'bootstrap_popover_placement' => 'right',
    'bootstrap_popover_selector' => '',
    'bootstrap_popover_title' => '',
    'bootstrap_popover_trigger' => array(
      'click' => 'click',
      'focus' => 0,
      'hover' => 0,
      'manual' => 0,
    ),
    'bootstrap_popover_trigger_autoclose' => 1,
    'bootstrap_region_well-content' => '',
    'bootstrap_region_well-footer' => '',
    'bootstrap_region_well-header' => '',
    'bootstrap_region_well-help' => '',
    'bootstrap_region_well-highlighted' => '',
    'bootstrap_region_well-navigation' => '',
    'bootstrap_region_well-page_bottom' => '',
    'bootstrap_region_well-page_top' => '',
    'bootstrap_region_well-sidebar_first' => 'well',
    'bootstrap_region_well-sidebar_second' => '',
    'bootstrap_table_bordered' => 0,
    'bootstrap_table_condensed' => 0,
    'bootstrap_table_hover' => 1,
    'bootstrap_table_responsive' => '-1',
    'bootstrap_table_striped' => 1,
    'bootstrap_toggle_jquery_error' => 0,
    'bootstrap_tooltip_animation' => 1,
    'bootstrap_tooltip_container' => 'body',
    'bootstrap_tooltip_delay' => '0',
    'bootstrap_tooltip_enabled' => 1,
    'bootstrap_tooltip_html' => 0,
    'bootstrap_tooltip_placement' => 'auto left',
    'bootstrap_tooltip_selector' => '',
    'bootstrap_tooltip_trigger' => array(
      'click' => 0,
      'focus' => 'focus',
      'hover' => 'hover',
      'manual' => 0,
    ),
    'default_favicon' => 1,
    'default_logo' => 1,
    'favicon_path' => '',
    'favicon_upload' => '',
    'global__active_tab' => 'edit-theme-settings',
    'logo_path' => '',
    'logo_upload' => '',
    'toggle_comment_user_picture' => 1,
    'toggle_comment_user_verification' => 1,
    'toggle_favicon' => 0,
    'toggle_logo' => 0,
    'toggle_main_menu' => 1,
    'toggle_name' => 1,
    'toggle_node_user_picture' => 1,
    'toggle_secondary_menu' => 0,
    'toggle_slogan' => 1,
  );
  $export['theme_bootstrap_settings'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'theme_default';
  $strongarm->value = 'bootstrap';
  $export['theme_default'] = $strongarm;

  $strongarm = new stdClass();
  $strongarm->disabled = FALSE; /* Edit this to true to make a default strongarm disabled initially */
  $strongarm->api_version = 1;
  $strongarm->name = 'user_register';
  $strongarm->value = '0';
  $export['user_register'] = $strongarm;

  return $export;
}
