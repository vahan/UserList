<?php

/**
 * 
 * @file
 * An include file for user_list module
 * Containing functions for creating, validating and submitting forms of users lists.
 */

/**
 * 
 * Creates a form to geather data for the conference invitation
 * @param array $form the form for wich this function is used.
 * @param array $form_state refernce on the array representing the state of the form
 * @param array $users list of users to whom the invitations is being sent
 * @param integer $idSuffix the id suffix of the opened search dialog in visualscience
 * @return the created form
 * @author vahe, 10.04.2012
 */
function user_list_conference_form($form, &$form_state, $users, $idSuffix) {
  
  // Otherwise we build page 1.
  $form['topic'] = array(
    '#type' => 'textfield',
    '#title' => t('The topic of the conference'),
  	'#prefix' => '<div id="conference-details-'.$idSuffix.'">'
  );
  
  $form['date'] = array(
    '#type' => 'textfield',
    '#title' => t('Conference date'),
  );
  
  $form['time'] = array(
    '#type' => 'textfield',
    '#title' => t('Starting time'),
  );
  
  $form['users'] =array(
  	'#type' => 'value',
  	'#value' => $users,
  );
  
  $form['message'] = array(
    '#type' => 'textarea',
    '#title' => t('Optional message'),
  );
  
  $form['send'] = array(
  	'#type' => 'submit',
  	'#value' => t('Create & Invite'),
    '#ajax' => array(
      'callback' => 'user_list_create_conference',
      'wrapper' => 'conference-details-'.$idSuffix,
      'progress' => array(),
    ),
    '#suffix' => '</div>'
  );
  return $form; 
}