<?php

$users = array();

/**
 * 
 * Submit handler for user_list_form export button.
 */
function user_list_conference_submit($form, &$form_state) {
	// these should be used for sending the invitation
	$userlist = $form_state['values']['list'];
	$idSuffix = substr($form["#form_id"], strpos($form["#form_id"], "user_list_form_")+strlen("user_list_form_"));

	$selected_users_string = "";
    foreach ($userlist as $uid => $checked) {
      if ($checked != 0) {
        $selected_users_string .= $uid.",";
      }
    }
    $selected_users_string = substr_replace($selected_users_string ,"",-1);
	// loading the 
	module_load_include('inc', 'node', 'node.pages');
	
	// getting the conference room creation form
	$opentok_form = node_add('opentok_conference');
	
	// setting the correct submit action
	$opentok_form['#action'] = url('node/add/opentok-conference');
	
//  	$opentok_form['field_participants']['#defaults_loaded'] = FALSE;
//  	$opentok_form['field_participants']['#processed'] = FALSE;
//  	$opentok_form['field_participants'][LANGUAGE_NONE]['#after_build_done'] = FALSE;
//  	$opentok_form['field_participants'][LANGUAGE_NONE][0]['value']['#defaults_loaded'] = FALSE;
//  	$opentok_form['field_participants'][LANGUAGE_NONE][0]['value']['#processed'] = FALSE;
// TODO handle this in a correct way..
	// Setting the default value for the participants based on the selection from user_list
  	$opentok_form['opentok_participants'][LANGUAGE_NONE][0]['value']['#value'] = $selected_users_string;
//  	$opentok_form['field_participants'][LANGUAGE_NONE][0]['value']['#attributes']['disabled'] = TRUE;
  	
  	
//  	$form['field_participants'][LANGUAGE_NONE][0]['value']['#defaults_loaded'] = FALSE;
//  	$opentok_form['field_participants'][LANGUAGE_NONE][0]['value']['#default_value'] = $selected_users_string;
//  	$form['field_participants'][LANGUAGE_NONE][0]['value']['#disabled'] = TRUE;
//  	$form['field_participants']['#disabled'] = TRUE;
  	hide($form['opentok_participants']);
	return $opentok_form;
}

/**
 * 
 */
function user_list_create_conference($form, &$form_state) {
	global $user;
	// Getting the selected users as objects from the list of uids
    $selected_users = array();
    foreach ($form_state['values']['users'] as $uid => $checked) {
      if ($checked != 0) {
        $selected_users[] = user_load($uid);
      }  
    }
    
    // Creating the html list of users
	$usernameList = "<ul>";
	foreach ($selected_users as $tempuser) {
		$usernameList .= "<li>".$tempuser->name."</li>";
	}
	$usernameList .= "</ul>";
	
    // TODO Create the conference room according to the link sent to the users
	
	$topic = $form_state['values']['topic'];
	$date = $form_state['values']['date'];
	$time = $form_state['values']['time'];
	$opt_message = $form_state['values']['message'];

	$conferenceUrl = create_opentok_conference_room($topic, $opt_message, $date, $time);
	
	$message = "Dear Sir/Madam, <br /><br /> You have been invited to an online conference by ".$user->name." with a topic '".$topic."'. It will take place on ".$date.
				" at ".$time.". The message from ".$user->name." follows: <br /><br />".$opt_message."<br /><br /> To join the conference, please follow the provided link below: <br /><br />".
	"<a href='".$conferenceUrl."'>".$conferenceUrl."</a><br /><br />Best regards, <br />QScience team";
	
    user_list_send_message($form_state['values']['users'], "QScience: Conference Invitation", $message);

    $selfMessage = "<p>You just invited</p>".$usernameList."<p>to a conference with a topic '".$topic."'. The link to the conference is:</p><a href='".$conferenceUrl."'>".$conferenceUrl."</a><p>Best regards, <br />QScience team</p>";
    user_list_send_message(array($user->uid => $user->uid), "QScience: Your Conference Invitation", $selfMessage);

    return "<p>You just invited</p>".$usernameList."<p>to a conference. The link to the conference room was sent to your email address.</p>";
}












/*

function create_opentok_conference_room($topic, $message, $date, $time) {
	global $user;
	// creating a bare node
	$node = new stdClass();
	
	//giving it type of the conference room TODO should not be hardcoded
	$node->type = 'conference_room';

	//give it a published staus
	$node->status = 1;

	//gives title
	$node->title = "Conference: ".$topic;

	// Setting the owner of the conference (the logged in user)
	$node->uid = $user->uid;
	
	// Enabling public comments
	$node->comment = 2;
	
	// do not show on the homepage
	$node->promote = 0;
	
	$description = $message." Start on ".$date." at ".$time;
	// setting the body
	$node->body[LANGUAGE_NONE][0]['value'] = $description;
	$node->body[LANGUAGE_NONE][0]['summary'] = "";
	$node->body[LANGUAGE_NONE][0]['format'] = "filtered_html";
	
	// TODO this needs to be changed to correctly match the language
	$node->language = LANGUAGE_NONE;
	
	$node->field_opentok[LANGUAGE_NONE][0]['roles'] = array(1 => "Subscriber",2 => "Publisher",3 => "Moderator",4 => "Moderator");
	$node->field_opentok[LANGUAGE_NONE][0]['auto_publish'] = 1;
	$node->field_opentok[LANGUAGE_NONE][0]['auto_connect'] = 1;
	$node->field_opentok[LANGUAGE_NONE][0]['push_talk'] = 1;
	// save it
	node_object_prepare($node);
	node_save($node);
	
	$options = array('absolute' => TRUE);
	$nid = $node->nid; // Node ID
	$url = url('node/' . $nid, $options);
//	$node->field_opentok_api_key[0][value] = 12695721;
//	$node->field_opentok_api_staging[0][value] = 1;
//		field_opentok_session_id 	field_opentok_auto_connect ield_opentok_auto_publish 	field_opentok_push_talk 	field_opentok_roles 
	
	// getting the id of the newly created node
	return $url;
};

*/
