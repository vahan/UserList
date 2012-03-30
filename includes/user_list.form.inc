<?php

/**
 * 
 * @file
 * An include file for user_list module
 * Containing functions for creating, validating and submitting forms of users lists.
 */

/**
 * 
 * Creates the form including users list, a checkbox for each user 
 * and a button to pass to the second page for entering the message.
 * After submition, a new page for entering the message opens.
 * @param array $form    the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 * @param array $users    list of users to be shown in the form
 * @return           the created form
 * @author Vahan, 25.03.2011
 */
function user_list_form($form, &$form_state, $users, $idSuffix) {
  global $user;
  
 // $idSuffix = substr($formId, strpos($formId, "list_form_") + strlen("list_form_"));
  
  if (isset($form_state['page_num']) && $form_state['page_num'] == 2) {
      return user_list_form_message($form, $form_state, $users);
  }
  // Otherwise we build page 1.
  $form_state['page_num'] = 1;
  $form['users'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#title' => t('Found Users'),
    '#prefix' => '<div id="visualscience_user_list">',
    '#suffix' => '</div>',
  );
  $form['users']['list'] = user_list_get_user_list($users, $idSuffix);
  
  $all_fields = user_list_get_all_fields();
  $options_fields = array();
  foreach ($all_fields as $field) {
    $options_fields[$field['field_id']] = $field['label'];
  }  
  $all_user_data = visualscience_get_user_data();
  $options_user_data = array();
  foreach ($all_user_data as $user_data) {
    $options_user_data[$user_data] = $user_data;
  }
  
  $form['users']['fields'] = array(
    '#type' => 'fieldset',
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
    '#title' => t('Choose fields to show'),
  );
  
  $form['users']['fields']['user_data'] = array(
    '#type' => 'checkboxes',
    '#title' => t(''),
    '#options' => $options_user_data,
  	'#default_value' => array_keys(user_list_get_shown_data(variable_get('visualscience_search_table', 'users'))),
    '#ajax' => array(
      'callback' => 'user_list_fields_callback',
      'wrapper' => 'user_list-list-'.$idSuffix,
      'progress' => array(),
    ),
  	'#prefix' => '<div style="max-height: 300px; overflow: auto"><table style="border: 0;"><tr><td valign="top" style="border: 0;">',
  	'#suffix' => '</td>',
  );
  
  $form['users']['fields']['field_list'] = array(
    '#type' => 'checkboxes',
    '#title' => t(''),
    '#options' => $options_fields,
    '#default_value' => array_keys(user_list_get_shown_fields()),
    '#ajax' => array(
      'callback' => 'user_list_fields_callback',
      'wrapper' => 'user_list-list-'.$idSuffix,
      'progress' => array(),
    ),
  	'#prefix' => '<td valign="top" style="border: 0;">',
  	'#suffix' => '</td></tr></table></div>',
  );
  
  $form['users']['submit'] = array(
  	'#type' => 'submit',
  	'#value' => t('Send a message >>'),
    '#ajax' => array(
      'callback' => 'user_list_load_form_message',
      'wrapper' => 'user-list-message-form-div-'.$idSuffix,
      'progress' => array(),
    ),
//  	'#submit' => array('user_list_form_submit'),
  	'#validate' => array('user_list_form_validate'),
    '#prefix' => '<table id="actions-table"><tr><td style="vertical-align:top;">',
  	'#suffix' => '</td>'
  );
  
  $form['users']['export'] = array(
  	'#type' => 'submit',
  	'#value' => t('Export to CSV'),
  	'#submit' => array('user_list_export_submit'),
  	'#validate' => array('user_list_form_validate'),
  	'#prefix' => '<td style="vertical-align:top;">',
    '#suffix' => '</td><td style="vertical-align:top;"><input id="ls-map-btn-'.$idSuffix.'" type="button" onclick="lsMapOpen(this)" class="form-submit ls-map-btn" value="LS Map"/></td>',
  );
 
  $form['users']['conference'] = array(
  	'#type' => 'submit',
  	'#value' => t('Conference'),
  	'#submit' => array(),
    '#ajax' => array(
      'callback' => 'user_list_conference_submit',
      'wrapper' => 'conference-details-'.$idSuffix,
      'progress' => array(),
    ),
    '#prefix' => '<td style="vertical-align:top;">',
    '#suffix' => '</td><td style="width: 100%;" rowspan="2"><div id="conference-details-'.$idSuffix.
    '"></div></td></tr><tr><td colspan="4" style="vertical-align: top;"><div id="user-list-message-form-div-'.$idSuffix.'"></div></td></tr></table>',
  );
  
  if (isset($form_state['values']['user_data'])) {
    user_list_save_hidden_user_data($form_state['values']['user_data']);
    $form['users']['list'] = user_list_get_user_list($users, $idSuffix);
  }
  if (isset($form_state['values']['field_list'])) {
    user_list_save_hidden_fields($form_state['values']['field_list']);
    $form['users']['list'] = user_list_get_user_list($users, $idSuffix);
  }
  
  return $form; 
}

/**
 * 
 * Ajax callback for the fields.
 * @param array $form    the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 * @return          renderable array
 */
function user_list_fields_callback($form, $form_state) {
  $_GET["q"] = "visualscience"; //Magic!
  return $form['users']['list'];
}

/**
 * 
 * Ajax callback for search saving 
 * @param array $form		the form for wich this function is used.
 * @param array $form_state	refernce on the array representing the state of the form
 * @return					modified form
 */
function user_list_search_save_callback($form, $form_state) {
  global $user;
  $search_name = $form_state['values']['search_name'] != '' ? $form_state['values']['search_name'] : 'default';
  $search_text = $form_state['values']['text'];
  $search_url = isset($_SESSION['userlist']['op']) ? $_SESSION['userlist']['op'] : "";
  $save_schema = drupal_get_schema(user_list_get_save_table_name());
  $result = user_list_get_search($search_name);
  if (count($result) == 0) {  
    db_query("INSERT INTO {".$save_schema['name']."} (uid, name, search, url) VALUES ($user->uid, '$search_name', '$search_text', '$search_url')");
  } else {
    db_query("UPDATE ".$save_schema['name']." SET search='$search_text', url='$search_url' WHERE name='$search_name'");
  }
  
  return $form['users']['save'];
}

/**
 * 
 * Returns an array of users ready to be shown in the form
 * @param $list	array of user object
 * @return		form renderable array of users in #tableselect element
 * @author Vahan
 */
function user_list_get_user_list($list, $idSuffix) {
  $table = variable_get("visualscience_search_table", "users");
  $statuses = array('blocked', 'active');
  $field_columns = array();
      
  $data = user_list_get_shown_data($table);
  $header = array();
  foreach ($data as $d) {
    $header[$d] = array("data" => $d, 'field' => 'u.'.$d); //TODO: rename 'u.' to include also nodes
    $user_data_columns[$d] = $d;
  }
  
  if ($table == "users") {
    $fields = user_list_get_shown_fields();
    foreach ($fields as $field) {
      $header[$field['label']] = array("data" => $field['label'], 'field' => 'u.data');
      $field_columns[$field['field_id']] = $field['label'];
    }
  }
  
  if (!empty($list)) {
    $options = array();
    foreach ($list as $item) {
      $u = user_load($item->uid);
      
      if ($table == "users") {
        $options[$item->uid] = array(
            'name' => $item->name,
            'mail' =>  $item->mail,
            'role' => implode("; ", user_load($item->uid)->roles),
            'status' =>  $statuses[$item->status],
            'created' => format_date($item->created, 'custom', 'd-m-Y'),
        );
        foreach ($field_columns as $fid => $fname) {
          $options[$item->uid][$fname] = implode(",", user_list_user_fields($item->uid, $fid));
        }
      } elseif ($table == "node") {
        $node_data = visualscience_get_node_data();
        foreach ($node_data as $d) {
          $options[$item->nid][$d] = $item->$d;
        }
      } else {
        $options = array();
      }    
    }
    return array(
      '#type' => 'tableselect',
      '#header' => $header,
      '#options' => $options,
      '#ajax' => array(
        'callback' => 'user_list_checked_callback',
      	'wrapper' => 'users_selected_div',
      	'event' => 'click',
      ),
      '#tree' => TRUE,
      '#multiple' => TRUE,
      '#js_select' => TRUE,
      '#input'=>true,
      '#attributes' => array("id"=>"user_list-list-".$idSuffix),
    );
  } else {
    return array(
      '#type' => 'container',
      '#prefix' => '<div id="user_list-list-'.$idSuffix.'">'.t('There are no users found'),
      '#suffix' => '</div>',
    );
  }
}

/**
 * 
 * Ajax callback for checking users
 * @param array $form    the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 * @return           'users_selected' element of the form
 */
function user_list_checked_callback($form, $form_state) {
  return $form['users_selected'];
}



/**
 * 
 * The second page to enter the message and send
 * @param array $form    the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 * @param array $users    TODO:UNUSED! 
 * @return array      modified form  
 */
/*
function user_list_form_message($form, &$form_state, $users)
{
  $page_one_values = $form_state['page_values'][1];
  $users = $page_one_values['list']; //complete list of users
  
  $user_list = array(); //list of users, to whom the message will be sent
  foreach ($users as $uid => $checked) {
    if ($checked == 0) continue;
    $user_list[$uid] = user_load($uid);
  }
  
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('mail subject'),
  );
  
  $form['message'] = array (
    '#type' => 'textarea',
    '#title' => t('Enter the message here'),
    '#suffix' => theme_user_list(array('users'=>$user_list, 'title'=>t('List of Users'))),
  );
  
  $form['submit_mail'] = array(
    '#type' => 'submit',
    '#value' => t('Send >>'),
    '#submit' => array('user_list_form_message_submit'),
    '#validate' => array('user_list_form_message_validate')
  );
  return $form;
}
*/
/**
 * 
 * Submit handler for user_list_form message button.
 * @param array $form    the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 */
function user_list_load_form_message($form, &$form_state) {
	$userlist = $form_state['values']['list'];
	$idSuffix = substr($form["#form_id"], strpos($form["#form_id"], "user_list_form_")+strlen("user_list_form_"));


	$selected_users_string = "";
    foreach ($userlist as $uid => $checked) {
      if ($checked != 0) {
        $selected_users_string .= $uid.",";
      }
    }
    $selected_users_string = substr_replace($selected_users_string ,"",-1);

    /* Uncomment if you want to pass an array instead of a string
    $list = array();
    foreach ($userlist as $uid => $checked) {
      if ($checked != 0) {
        $list[] = user_load($uid);
      }
    }
    */
    
	// getting the form for sending messages
	$message_form = drupal_get_form("user_list_message_form_".$idSuffix, $selected_users_string, $idSuffix);
	
	return $message_form;
}
	
/**
 * 
 * Validate handler for the message button on first page.	
 * @param array $form		the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 */
function user_list_form_validate($form, &$form_state)
{
  $users = $form_state['values']['list'];
  foreach($users as $u) {
      if ($u !== 0) {
        return;
    }
  }
  form_set_error('values', 'Select at least one user');
}

/**
 * 
 * Redirects to userlist page
 */
function user_list_redirect_to_userlist() {
  drupal_goto('visualscience/user_list');
}

?>
