<?php

/**
 * @file
 * Features integration on behalf of 'field' module.
 */

/**
 * Implements hook_features_api().
 */
function field_features_api() {
  return array(
    'field' => array(
      // This is deprecated by field_base and field_instance
      // but retained for compatibility with older exports.
      'name' => t('Fields'),
      /* @see \hook_field_default_fields() */
      /* @see \hook_field_default_fields_alter() */
      'default_hook' => 'field_default_fields',
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
      'feature_source' => FALSE,
    ),
    'field_base' => array(
      'name' => t('Field Bases'),
      /* @see \hook_field_default_field_bases() */
      /* @see \hook_field_default_field_bases_alter() */
      'default_hook' => 'field_default_field_bases',
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
      'feature_source' => TRUE,
      'supersedes' => 'field',
    ),
    'field_instance' => array(
      'name' => t('Field Instances'),
      /* @see \hook_field_default_field_instances() */
      /* @see \hook_field_default_field_instances_alter() */
      'default_hook' => 'field_default_field_instances',
      'default_file' => FEATURES_DEFAULTS_INCLUDED,
      'feature_source' => TRUE,
      'supersedes' => 'field',
    ),
  );
}

/**
 * Implements hook_features_export_options().
 */
function field_base_features_export_options() {
  $options = array();
  $fields = field_info_fields();
  foreach ($fields as $field_name => $field) {
    $options[$field_name] = $field_name;
  }
  return $options;
}

/**
 * Implements hook_features_export_options().
 */
function field_instance_features_export_options() {
  $options = array();
  foreach (field_info_fields() as $field_name => $field) {
    foreach ($field['bundles'] as $entity_type => $bundles) {
      foreach ($bundles as $bundle) {
        $identifier = "{$entity_type}-{$bundle}-{$field_name}";
        $options[$identifier] = $identifier;
      }
    }
  }
  ksort($options);
  return $options;
}

/**
 * Implements hook_features_export().
 */
function field_base_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  $map = features_get_default_map('field_base');

  // The field_default_field_bases() hook integration is provided by the
  // features module so we need to add it as a dependency.
  $export['dependencies']['features'] = 'features';

  foreach ($data as $identifier) {
    if ($base = features_field_base_load($identifier)) {
      // If this field is already provided by another module, remove the field
      // and add the other module as a dependency.
      if (isset($map[$identifier]) && $map[$identifier] != $module_name) {
        if (isset($export['features']['field_base'][$identifier])) {
          unset($export['features']['field_base'][$identifier]);
        }
        $module = $map[$identifier];
        $export['dependencies'][$module] = $module;
      }
      // If the field has not yet been exported, add it.
      else {
        $export['features']['field_base'][$identifier] = $identifier;
        $export['dependencies'][$base['module']] = $base['module'];
        if ($base['storage']['type'] != variable_get('field_storage_default', 'field_sql_storage')) {
          $export['dependencies'][$base['storage']['module']] = $base['storage']['module'];
        }
        // If taxonomy field, add in the vocabulary.
        if ($base['type'] == 'taxonomy_term_reference' && !empty($base['settings']['allowed_values'])) {
          foreach ($base['settings']['allowed_values'] as $allowed_values) {
            if (!empty($allowed_values['vocabulary'])) {
              $pipe['taxonomy'][] = $allowed_values['vocabulary'];
            }
          }
        }
      }
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export().
 */
function field_instance_features_export($data, &$export, $module_name = '') {
  $pipe = array('field_base' => array());
  $map = features_get_default_map('field_instance');

  // The field_default_field_instances() hook integration is provided by the
  // features module so we need to add it as a dependency.
  $export['dependencies']['features'] = 'features';

  foreach ($data as $identifier) {
    if ($instance = features_field_instance_load($identifier)) {
      // If this field is already provided by another module, remove the field
      // and add the other module as a dependency.
      if (isset($map[$identifier]) && $map[$identifier] != $module_name) {
        if (isset($export['features']['field_instance'][$identifier])) {
          unset($export['features']['field_instance'][$identifier]);
        }
        $module = $map[$identifier];
        $export['dependencies'][$module] = $module;
      }
      // If the field has not yet been exported, add it.
      else {
        $export['features']['field_instance'][$identifier] = $identifier;
        $export['dependencies'][$instance['widget']['module']] = $instance['widget']['module'];
        foreach ($instance['display'] as $key => $display) {
          if (isset($display['module'])) {
            $export['dependencies'][$display['module']] = $display['module'];
            // @TODO: handle the pipe to image styles
          }
        }
        $pipe['field_base'][] = $instance['field_name'];
      }
    }
  }
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function field_base_features_export_render($module, $data, $export = NULL) {
  $translatables = $code = array();
  $code[] = '  $field_bases = array();';
  $code[] = '';
  foreach ($data as $identifier) {
    if ($field = features_field_base_load($identifier)) {
      unset($field['columns']);
      unset($field['foreign keys']);
      // Only remove the 'storage' declaration if the field is using the default
      // storage type.
      if ($field['storage']['type'] == variable_get('field_storage_default', 'field_sql_storage')) {
        unset($field['storage']);
      }
      // If we still have a storage declaration here it means that a non-default
      // storage type was altered into to the field definition. And no one would
      // never need to change the 'details' key, so don't render it.
      if (isset($field['storage']['details'])) {
        unset($field['storage']['details']);
      }

      _field_instance_features_export_sort($field);
      $field_export = features_var_export($field, '  ');
      $field_prefix = '  // Exported field_base: ';
      $field_identifier = features_var_export($identifier);
      if (features_field_export_needs_wrap($field_prefix, $field_identifier)) {
        $code[] = rtrim($field_prefix);
        $code[] = "  // {$field_identifier}.";
      }
      else {
        $code[] = $field_prefix . $field_identifier . '.';
      }
      $code[] = "  \$field_bases[{$field_identifier}] = {$field_export};";
      $code[] = "";
    }
  }
  $code[] = '  return $field_bases;';
  $code = implode("\n", $code);
  return array('field_default_field_bases' => $code);
}

/**
 * Implements hook_features_export_render().
 */
function field_instance_features_export_render($module, $data, $export = NULL) {
  $translatables = $code = array();

  $code[] = '  $field_instances = array();';
  $code[] = '';

  foreach ($data as $identifier) {
    if ($instance = features_field_instance_load($identifier)) {
      _field_instance_features_export_sort($instance);
      $field_export = features_var_export($instance, '  ');
      $instance_prefix = '  // Exported field_instance: ';
      $instance_identifier = features_var_export($identifier);
      if (features_field_export_needs_wrap($instance_prefix, $instance_identifier)) {
        $code[] = rtrim($instance_prefix);
        $code[] = "  // {$instance_identifier}.";
      }
      else {
        $code[] = $instance_prefix . $instance_identifier . '.';
      }
      $code[] = "  \$field_instances[{$instance_identifier}] = {$field_export};";
      $code[] = "";

      if (!empty($instance['label'])) {
        $translatables[] = $instance['label'];
      }
      if (!empty($instance['description'])) {
        $translatables[] = $instance['description'];
      }
    }
  }
  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }
  $code[] = '  return $field_instances;';
  $code = implode("\n", $code);
  return array('field_default_field_instances' => $code);
}

/**
 * Normalizes and sorts a field or field instance definition, recursively.
 *
 * Some layers of the nested structure are exempt from sorting, but their
 * children might again be sorted.
 *
 * @param array $field
 *   Field or field instance definition.
 *   In recursive calls, this is a nested element within that structure.
 * @param bool $sort
 *   TRUE, if this (nested) element should be sorted.
 */
function _field_instance_features_export_sort(&$field, $sort = TRUE) {

  // Some arrays are not sorted to preserve order (for example allowed_values).
  static $sort_blacklist = array(
    'allowed_values',
    'format_handlers',
  );

  if ($sort) {
    uksort($field, 'strnatcmp');
  }
  foreach ($field as $k => $v) {
    if (is_array($v)) {
      _field_instance_features_export_sort($field[$k], !in_array($k, $sort_blacklist));
    }
  }
}

/**
 * Implements hook_features_revert().
 */
function field_base_features_revert($module) {
  field_base_features_rebuild($module);
}

/**
 * Implements hook_features_revert().
 */
function field_instance_features_revert($module) {
  field_instance_features_rebuild($module);
}

/**
 * Implements of hook_features_rebuild().
 * Rebuilds fields from code defaults.
 */
function field_base_features_rebuild($module) {
  if ($fields = features_get_default('field_base', $module)) {
    field_info_cache_clear();

    // Load all the existing field bases up-front so that we don't
    // have to rebuild the cache all the time.
    $existing_fields = field_info_fields();

    foreach ($fields as $field) {
      // Create or update field.
      if (isset($existing_fields[$field['field_name']])) {
        $existing_field = $existing_fields[$field['field_name']];
        $array_diff_result = drupal_array_diff_assoc_recursive($field + $existing_field, $existing_field);
        if (!empty($array_diff_result)) {
          try {
            field_update_field($field);
          }
          catch (FieldException $e) {
            watchdog('features', 'Attempt to update field %label failed: %message', array('%label' => $field['field_name'], '%message' => $e->getMessage()), WATCHDOG_ERROR);
          }
        }
      }
      else {
        try {
          field_create_field($field);
        }
        catch (FieldException $e) {
          watchdog('features', 'Attempt to create field %label failed: %message', array('%label' => $field['field_name'], '%message' => $e->getMessage()), WATCHDOG_ERROR);
        }
        $existing_fields[$field['field_name']] = $field;
      }
      variable_set('menu_rebuild_needed', TRUE);
    }
  }
}

/**
 * Implements of hook_features_rebuild().
 * Rebuilds field instances from code defaults.
 */
function field_instance_features_rebuild($module) {
  if ($instances = features_get_default('field_instance', $module)) {
    field_info_cache_clear();

    // Load all the existing instances up-front so that we don't
    // have to rebuild the cache all the time.
    $existing_instances = field_info_instances();

    foreach ($instances as $field_instance) {
      // If the field base information does not exist yet, cancel out.
      if (!field_info_field($field_instance['field_name'])) {
        continue;
      }

      // Create or update field instance.
      if (isset($existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']])) {
        $existing_instance = $existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']];
        if ($field_instance + $existing_instance != $existing_instance) {
          try {
            field_update_instance($field_instance);
          }
          catch (FieldException $e) {
            watchdog('features', 'Attempt to update field instance %label (in %entity entity type %bundle bundle) failed: %message', array('%label' => $field_instance['field_name'], '%entity' => $field_instance['entity_type'], '%bundle' => $field_instance['bundle'], '%message' => $e->getMessage()), WATCHDOG_ERROR);
          }
        }
      }
      else {
        try {
          field_create_instance($field_instance);
        }
        catch (FieldException $e) {
          watchdog('features', 'Attempt to create field instance %label (in %entity entity type %bundle bundle) failed: %message', array('%label' => $field_instance['field_name'], '%entity' => $field_instance['entity_type'], '%bundle' => $field_instance['bundle'], '%message' => $e->getMessage()), WATCHDOG_ERROR);
        }
        $existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']] = $field_instance;
      }
    }

    if ($instances) {
      variable_set('menu_rebuild_needed', TRUE);
    }
  }
}

/**
 * Loads a field base configuration by a field_name identifier.
 *
 * @param string $field_name
 *   Field name to load.
 *
 * @return array|false
 *   The field base definition array as from field_info_field().
 *   The auto-increment keys 'id' and 'field_id' are removed, to reduce noise
 *   in the export.
 *   If the field base is not found, FALSE is returned.
 */
function features_field_base_load($field_name) {
  if ($field_info = field_info_field($field_name)) {
    unset($field_info['id']);
    unset($field_info['bundles']);
    return $field_info;
  }
  return FALSE;
}

/**
 * Loads a field instance definition array suitable for export.
 *
 * @param string $identifier
 *   Format: "$entity_type-$bundle-$field_name".
 *
 * @return array|false
 *   Field instance definition array as from field_info_instance().
 *   The auto-increment keys 'id' and 'field_id' are removed, to reduce noise
 *   in the export.
 *   If the field instance is not found, FALSE is returned.
 */
function features_field_instance_load($identifier) {
  list($entity_type, $bundle, $field_name) = explode('-', $identifier);
  if ($instance_info = field_info_instance($entity_type, $field_name, $bundle)) {
    unset($instance_info['id']);
    unset($instance_info['field_id']);
    return $instance_info;
  }
  return FALSE;
}

/* ----- DEPRECATED FIELD EXPORT -----
 * keep this code for backward compatibility with older exports
 * until v3.x
 */

/**
 * Implements hook_features_export_options().
 */
function field_features_export_options() {
  $options = array();
  $instances = field_info_instances();
  foreach ($instances as $entity_type => $bundles) {
    foreach ($bundles as $bundle => $fields) {
      foreach ($fields as $field) {
        $identifier = "{$entity_type}-{$bundle}-{$field['field_name']}";
        $options[$identifier] = $identifier;
      }
    }
  }
  return $options;
}

/**
 * Implements hook_features_export().
 */
function field_features_export($data, &$export, $module_name = '') {
  $pipe = array();
  // Convert 'field' to 'field_instance' on features-update.
  $pipe['field_instance'] = $data;
  return $pipe;
}

/**
 * Implements hook_features_export_render().
 */
function field_features_export_render($module, $data, $export = NULL) {
  $translatables = $code = array();

  $code[] = '  $fields = array();';
  $code[] = '';
  foreach ($data as $identifier) {
    if ($field = features_field_load($identifier)) {
      unset($field['field_config']['columns']);
      // Only remove the 'storage' declaration if the field is using the default
      // storage type.
      if ($field['field_config']['storage']['type'] == variable_get('field_storage_default', 'field_sql_storage')) {
        unset($field['field_config']['storage']);
      }
      // If we still have a storage declaration here it means that a non-default
      // storage type was altered into to the field definition. And no one would
      // never need to change the 'details' key, so don't render it.
      if (isset($field['field_config']['storage']['details'])) {
        unset($field['field_config']['storage']['details']);
      }

      _field_features_export_sort($field);
      $field_export = features_var_export($field, '  ');
      $field_identifier = features_var_export($identifier);
      $code[] = "  // Exported field: {$field_identifier}.";
      $code[] = "  \$fields[{$field_identifier}] = {$field_export};";
      $code[] = "";

      // Add label and description to translatables array.
      if (!empty($field['field_instance']['label'])) {
        $translatables[] = $field['field_instance']['label'];
      }
      if (!empty($field['field_instance']['description'])) {
        $translatables[] = $field['field_instance']['description'];
      }
    }
  }
  if (!empty($translatables)) {
    $code[] = features_translatables_export($translatables, '  ');
  }
  $code[] = '  return $fields;';
  $code = implode("\n", $code);
  return array('field_default_fields' => $code);
}

/**
 * Helper to enforce consistency in field export arrays.
 *
 * @param array $field
 *   Nested array containing field base and field instance configuration.
 *   In recursive calls, this is a nested element within that structure.
 * @param bool $sort
 *   TRUE, if this (nested) element should be sorted.
 *
 * @see \_field_instance_features_export_sort()
 */
function _field_features_export_sort(&$field, $sort = TRUE) {

  // Some arrays are not sorted to preserve order (for example allowed_values).
  static $sort_blacklist = array(
    'allowed_values',
    'format_handlers',
  );

  if ($sort) {
    ksort($field);
  }
  foreach ($field as $k => $v) {
    if (is_array($v)) {
      _field_features_export_sort($field[$k], !in_array($k, $sort_blacklist));
    }
  }
}

/**
 * Implements hook_features_revert().
 */
function field_features_revert($module) {
  field_features_rebuild($module);
}

/**
 * Implements of hook_features_rebuild().
 * Rebuilds fields from code defaults.
 */
function field_features_rebuild($module) {
  if ($fields = features_get_default('field', $module)) {
    field_info_cache_clear();

    // Load all the existing fields and instance up-front so that we don't
    // have to rebuild the cache all the time.
    $existing_fields = field_info_fields();
    $existing_instances = field_info_instances();

    foreach ($fields as $field) {
      // Create or update field.
      $field_config = $field['field_config'];
      if (isset($existing_fields[$field_config['field_name']])) {
        $existing_field = $existing_fields[$field_config['field_name']];
        $array_diff_result = drupal_array_diff_assoc_recursive($field_config + $existing_field, $existing_field);
        if (!empty($array_diff_result)) {
          try {
            field_update_field($field_config);
          }
          catch (FieldException $e) {
            watchdog('features', 'Attempt to update field %label failed: %message', array('%label' => $field_config['field_name'], '%message' => $e->getMessage()), WATCHDOG_ERROR);
          }
        }
      }
      else {
        try {
          field_create_field($field_config);
        }
        catch (FieldException $e) {
          watchdog('features', 'Attempt to create field %label failed: %message', array('%label' => $field_config['field_name'], '%message' => $e->getMessage()), WATCHDOG_ERROR);
        }
        $existing_fields[$field_config['field_name']] = $field_config;
      }

      // Create or update field instance.
      $field_instance = $field['field_instance'];
      if (isset($existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']])) {
        $existing_instance = $existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']];
        if ($field_instance + $existing_instance != $existing_instance) {
          field_update_instance($field_instance);
        }
      }
      else {
        field_create_instance($field_instance);
        $existing_instances[$field_instance['entity_type']][$field_instance['bundle']][$field_instance['field_name']] = $field_instance;
      }
    }

    if ($fields) {
      variable_set('menu_rebuild_needed', TRUE);
    }
  }
}

/**
 * Loads a combined array with field base and field instance definition.
 *
 * @param string $identifier
 *   Format: "$entity_type-$bundle-$field_name".
 *
 * @return array|false
 *   Array with information about the field base and the field instance.
 */
function features_field_load($identifier) {
  list($entity_type, $bundle, $field_name) = explode('-', $identifier);
  $field_info = field_info_field($field_name);
  $instance_info = field_info_instance($entity_type, $field_name, $bundle);
  if ($field_info && $instance_info) {
    unset($field_info['id']);
    unset($field_info['bundles']);
    unset($instance_info['id']);
    unset($instance_info['field_id']);
    return array(
      'field_config' => $field_info,
      'field_instance' => $instance_info,
    );
  }
  return FALSE;
}

/**
 * Determine if a field export line needs to be wrapped.
 *
 * Drupal code standards specify that comments should wrap at 80 characters or
 * less.
 *
 * @param string $prefix
 *   The prefix to be exported before the field identifier.
 * @param string $identifier
 *   The field identifier.
 *
 * @return bool
 *   TRUE if the line should be wrapped after the prefix, else FALSE.
 *
 * @see https://www.drupal.org/node/1354
 */
function features_field_export_needs_wrap($prefix, $identifier) {
  // Check for 79 characters, since the comment ends with a full stop.
  return (strlen($prefix) + strlen($identifier) > 79);
}
