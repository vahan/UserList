<?php

$users = array();

/**
 * 
 * Submit handler for user_list_form export button.
 */
function user_list_export_submit ($form, &$form_state) {

  $selected_users = array();
  foreach ($form_state['values']['list'] as $uid => $checked) {
    if ($checked != 0) {
      $selected_users[] = $form['users']['list']['#options'][$uid];
    }  
  }

  user_list_export($selected_users);
  exit(0); // to make sure no further header is sent. Probably, there is a better way to do it.
}

/**
 * 
 * Creates CSV file with the users data and sends to the users to download
 * @param array $userlist	list of users to export
 */
function user_list_export ($userlist = FALSE) {
  $filename = 'userlist_'.date("Ymd").'.csv';
  //Load all usernames if data is empty
  if (!is_array($userlist) || empty($userlist)) {
     $query = db_select('users','u')
            ->fields('u', array('name'))
            ->condition('u.status',0,'<>')
            ->condition('u.uid',1,'>');

        $result = $query->execute();
    $userlist = array();
    foreach ($result as $account) {
      $userlist[] = array($account->name);
    }
  }
  $user_data = user_list_get_shown_user_data();
  $fields = user_list_get_shown_fields();
  $field_names = array();
  foreach ($fields as $field_id => $field) {
    $field_names[$field['label']] = $field['label'];
  }
  $columns = array_merge($user_data, $field_names);
  //Write the column headers
  $out = implode(",", $columns)."\n";
  foreach($userlist as $row) {
    if (!empty($row)) {
      $show_data = array();
      $show_columns = array_intersect(array_keys($row), $columns);
      foreach ($show_columns as $col) {
        $show_data[] = $row[$col];
      }
      $out .= implode(",", $show_data);
      $out .= "\n";
    }
  }
  
  //prepare to give the user a Save/Open dialog...
  drupal_add_http_header ("Content-type", "application/octet-stream");
  drupal_add_http_header ("Content-Disposition", "attachment; filename=".$filename);
  print $out;
}

?>