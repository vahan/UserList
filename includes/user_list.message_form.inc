<?php

/**
 * 
 * @file
 * An include file for user_list module
 * Containing functions for creating a form to send message to users
 */

/**
 * 
 * Creates a form to send messages to the users
 * @param array $form the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 * @param array $users list of users to whom the invitations is being sent
 * @param integer $idSuffix the id suffix of the opened search dialog in visualscience
 * @return $form the created form
 * @author vahe, 10.04.2012
 */
function user_list_message_form($form, &$form_state, $user_uid_list, $idSuffix) {
  // Otherwise we build page 1.
  $form['subject'] = array(
    '#type' => 'textfield',
    '#title' => t('The message subject'),
  	'#prefix' => '<div id="user-list-message-form-div-'.$idSuffix.'">'
  );
  
  $form['user_uid_list'] = array(
  	'#type' => 'hidden',
  	'#value' => $user_uid_list,
  );
  
  $form['message'] = array (
    '#type' => 'textarea',
    '#title' => t('Enter the message here'),
  );
  
  $form['submit_mail'] = array(
    '#type' => 'submit',
    '#value' => t('Send >>'),
    '#ajax' => array(
      'callback' => 'user_list_form_message_submit',
      'wrapper' => 'user-list-message-form-div-'.$idSuffix,
      'progress' => array(),
    ),
//    '#submit' => array('user_list_form_message_submit'),
//    '#validate' => array('user_list_form_message_validate'),
    '#prefix' => '<br />',
    '#suffix' => '</div>',
  );
  return $form; 
}

/**
 * 
 * The function responsible for sending emails
 * This is the final submit handler. Send the entered message to the selected users
 * @param array $form		the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 */
function user_list_form_message_submit($form, &$form_state) {
	global $user;

	$user_uid_list = $form_state['values']['user_uid_list'];
	$user_uids = explode(",", $user_uid_list);
	$recipients = array();
	foreach ($user_uids as $uid) {
		$recipients[] = user_load($uid);
	}
	
	// Creating coma separated list of user emails
	$recipientEmails = "";
	foreach ($recipients as $recipient) {
		$recipientEmails .= $recipient->mail.",";
	}
	$recipientEmails = substr_replace($recipientEmails, "", -1);		
  
	
	global $base_root;
	// getting the pure domain name
	$domain_name = str_replace(array('http://','https://', 'http://www.', 'https://www.'), '', $base_root);
		
	$from = 'no-reply@'.$domain_name;
	
	$subject = $form_state['values']['subject'];
	$message = $form_state['values']['message'];
//	user_list_send_message($users, $subject, $message, $from);

	// TODO handle progress and errors
	drupal_mail('user_list', 'custom', $recipientEmails, user_preferred_language($user), array('message' => $message, 'subject' => $subject), $from, TRUE);
	return "Message sent successfully";
/*	
  $page_one_values = $form_state['page_values'][1];
  $page_two_values = $form_state['values'];
  
  $users = $page_one_values['list'];
  $subject = $page_one_values['subject'];
  $message = $page_two_values['message'];
  $batch = array(
    'operations' => array(array('user_list_send_message', array($users, $subject, $message))),
    'title' => t('Your messages are being sent'),
  );
  
  batch_set($batch);
*/
}

/**
 * 
 * Sends the $message to the $users.
 * @param array $users    list of users to receive the message.
 * @param string $message  the message to be sent.
 */
/*
function user_list_send_message($users, $subject, $message, $from) {
  $successes = array();
  $failures = array();
  foreach($users as $user) {
    $params = array('message' => $message, 'user' => $user, 'subject' => $subject);
   	$result = drupal_mail('user_list', 'msg', $user->mail, user_preferred_language($user), $params, $from, true);
        
    if ($result['result']) {
      array_push($successes, $result);
    }
    else {
      array_push($failures, $result);
    }
  }
  $final_message = 'All message have been correctly sent.';
  if (count($failures) != 0) {
  	$emails ="";
    foreach ($failures as $f) {
      $emails .= $f['to'] . "/n";
    }
    $final_message = 'Error while sending the messages. The following emails have not been reached: \n'.$emails;
  }
  return $final_message;
}
*/
/**
 * 
 * Validates the entered message and if it's empty throws an error.
 * @param array $form    the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 */
function user_list_form_message_validate($form, &$form_state) {
  if(!isset($form_state['values']['message'])) {
    form_set_error('message', 'Type some text');
  }
}


