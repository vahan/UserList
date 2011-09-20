<?php

/**
 * 
 * Saves the changed field list into the hidden fields table in the DB 
 * @param $field_id array of {field_id => value} of the saving fields, if value is 0 then delete, otherwise add
 */
function user_list_save_hidden_fields($fields) {
  global $user;
  $saved = user_list_get_hidden_fields();
  foreach ($fields as $fid => $value) {
    if ($value == 0 && !in_array($fid, $saved)) {
      $nid = db_insert(user_list_get_field_table_name())
        ->fields(array(
          'fid' => $fid,
          'uid' => $user->uid,
        ))
        ->execute();
    } elseif ($value > 0 && in_array($fid, $saved)) {
      $num_deleted = db_delete(user_list_get_field_table_name())
        ->condition('fid', $fid)
        ->condition('uid', $user->uid)
        ->execute();
    }
  }
  
}

/**
 * 
 * Saves the changed user data list into the hidden user data table in the DB 
 * @param $data array of {data => value} of the saving fields, if value is 0 then delete, otherwise add
 */
function user_list_save_hidden_user_data($user_data) {
  global $user;
  $saved = user_list_get_hidden_user_data();
  foreach ($user_data as $data => $value) {
    if ($value === 0 && !in_array($data, $saved)) {
      $nid = db_insert(user_list_get_user_data_table_name())
        ->fields(array(
          'data' => $data,
          'uid' => $user->uid,
        ))
        ->execute();
    } elseif ($value !== 0 && in_array($data, $saved)) {
      $num_deleted = db_delete(user_list_get_user_data_table_name())
        ->condition('data', $data)
        ->condition('uid', $user->uid)
        ->execute();
    }
  }
}

/**
 * returns an array containing all fields
 */
function user_list_get_all_fields() {  
  return field_info_instances('user', 'user');
}

/**
 * 
 * Returns a list of the current user's data which are hidden
 */
function user_list_get_hidden_user_data() {
  global $user;
  $field_schema = drupal_get_schema(user_list_get_user_data_table_name());
  $result = db_select($field_schema['name'], 's')
        ->fields('s', array('data', 'uid'))
        ->condition("uid", $user->uid)
        ->execute();

  $data = array();
  foreach ($result as $row) {
    array_push($data, $row->data);
  }
  return $data;
}

/**
 * 
 * Returns a list of the current user's data which are shown
 */
function user_list_get_shown_user_data() {
  $data_all = user_list_get_all_user_data();
  $data_hidden = user_list_get_hidden_user_data();
  $data_shown = array();
  foreach ($data_all as $data) {
    if (!in_array($data, $data_hidden)) {
      $data_shown[$data] = $data;
    }
  }
  return $data_shown;
}

/**
 * 
 * Returns a list of the current user's field IDs which are hidden
 */
function user_list_get_hidden_fields() {
  global $user;
  $field_schema = drupal_get_schema(user_list_get_field_table_name());
  $result = db_select($field_schema['name'], 's')
        ->fields('s', array('fid', 'uid'))
        ->condition("uid", $user->uid)
        ->execute();

  $fields = array();
  foreach ($result as $row) {
    array_push($fields, $row->fid);
  }
  return $fields;
}

/**
 * 
 * Returns a list of the current user's field IDs which are shown
 */
function user_list_get_shown_fields() {
  $fields_all = user_list_get_all_fields();
  $fields_hidden = user_list_get_hidden_fields();
  $fields_shown = array();
  foreach ($fields_all as $field) {
    if (!in_array($field['field_id'], $fields_hidden)) {
      $fields_shown[$field['field_id']] = $field;
    }
  }
  return $fields_shown;
}

/**
 * 
 * returns the name of the table where the list of fields to hide is stored
 */
function user_list_get_user_data_table_name() {
  return 'user_list_user_data';
}


/**
 * 
 * returns the name of the table where the list of fields to hide is stored
 */
function user_list_get_field_table_name() {
  return 'user_list_fields';
}



?>