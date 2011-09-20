<?php

/**
 * 
 * Implements hook_help()
 * 
 */
function user_list_help($path, $arg) {
  switch ($path) {
    case 'admin/help#user_list':
      return t(
        '<p>User lists may be displayed alphabetically, beginning with A-Z, by newest, or as a list of users who have posted content of a certain type.</p>
        <p>Go to !admin to change the global settings for user lists, and to !block to configure specific user listing blocks if desired. Note that all blocks provided by this module are appended with "User List:", so they may be differentiated by similar blocks provided by other modules.</p>
        <p>Note that you must enable access to the user listings with the \'access user lists\' permission at !access, which controls viewing both blocks and pages. If enabled and allowed, the main user listing page may be viewed at !user_list.</p>
      	<p><h4>Search</h4></p>
      	<p><b>Search Box</b><br/>
		Use <i>Refine Search</i> for filtering the list according to the given criteria. The search syntax is the following:</br>
			FIELD_NAME_1=[TERM_11 AND TERM_12 OR TERM_13...] OR FIELD_NAME_2=[TERM_21] AND ...
		FIELD_NAME corresponds either to the name of the field in the database, declared as field_FIELD_NAME, or one of the following fields of the user table
			Name, email, role, date* (of registration)
		The same FIELD_NAME can be used several times. For AND and OR "&&", "&" and "||", "|" can be also used accordingly. The search and also the logical operators are not case sensitive. 
		*For the date a special syntax is used: date=[START_DATE, END_DATE]. If the START_DATE is skipped, i.e. date=[,END_DATE], there won\'t be a START_DATE used, and if END_DATE is skipped, i.e. date=[START_DATE,], as the upper limit the current date will be used. If only one date is given, i.e. date=[DATE], then it will be used as the starting date. All the PHP supported syntaxes can be used to specify a date.
		NOT logical operation can be also used with the terms, in the following form:
			NOT(TERM)
		While typing in the search box, smart autocomplete is provided. After typing FIELD_NAME=, "[" is added automatically. In the list provided by the autocomplete, multiple options can be selected by checking (SPACEBAR must be used) the checkboxes in front of the terms. To confirm the selection press ENTER. The terms are added with AND logical operator. For the "date" a calendar is provided.
		<br/><b>Save/Load</b><br/>
		In the Save field you can save the current save or load a previously saved search. Type the name of the search in the given text field and press Save Search button to save the current search in the database. To load a previously saved search, just select it from the given drop-down list.
		<br><b>Found Users</b></br>
		In this field a list of the users found by the search is given. You can choose which fields to show/hide by checking/unchecking the appropriate checkbox in the Choose fields to show tab.
		<br/><b>Operations with the selected users</b><br/>
		You can select users from the list and do several operations with them. Send an email by clicking the Enter the message button; you can enter the message in the next page. Export to CSV button exports the list of the selected users to a CSV file.
		<br/><b>Settings</b><br/>
		You can use the Settings link to go to the admin page and change the settings.
		</p>',
		array('!admin' => l('admin/settings/userlist', 'admin/settings/userlist'), '!block' => l('admin/structure/block', 'admin/structure/block'), '!access' => l('admin/people/permissions', 'admin/people/permissions'), '!user_list' => l('userlist', 'userlist'))
      );
  }
}

/**
 * Implementes hook_perm()
 * 
 */
function user_list_perm() {
  return array('access user lists', 'administer user lists');
}


function user_list_admin_settings() {
  $form = array();

  $form['user_list_default_number'] = array(
    '#type' => 'select',
    '#title' => t('Default Number of Users to List'),
    '#default_value' => variable_get('user_list_default_number', 10),
    '#options' => drupal_map_assoc(array(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 15, 20, 25, 30, 40, 50, 100, 125, 150, 200, 250, 500)),
    '#description' => t('Choose the default number of users to list on the various user list pages. Certain blocks, modules, or themes may override this value.'),
  );
  return system_settings_form($form);
}

