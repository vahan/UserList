<?php

/**
 * 
 * @file
 * This file contains several helping functions that are used for the module 
 */

/**
 *  
 * Returns an array|object|boolean of role table
 * @param string $op
 *         'get' => return a $role array for a $role_type or an array of roles from role table
 *         'base' => return whether a role exists in role table
 *         'name' => return just the name of the role specified in $role_type
 * @param string $role_type:
 *          if role_type is null gets all roles, if role_type is specified grabs information 
 *          for only that role from role table
 * @return     role table
 */
function _user_list_get_roles($op = 'get', $role_type = NULL) {
  
  // Gather Data from {role} table
  if (!is_null($role_type)) {
    // need to make sure we translate back to spaces
    $role_type = str_replace('_', ' ', $role_type);
    $result = db_query("SELECT r.rid, r.name FROM {role} r WHERE r.name = :role", array(':role' => $role_type))->fetchAssoc(); //Vahan 11.02.2011, 18.03.2011
  }
  else {
    // Role: 1 anonymous
    // Role: 2 authenticated
    // Are Skipped    
    $result = db_query("SELECT r.rid, r.name FROM {role} r WHERE r.rid > 2")->fetchAll();
  }
  
  if (!isset($result)) {
    return FALSE;
  }
  
  switch($op) {
    
    case 'base':
      return TRUE;
      
    case 'get':
      
      if (!is_null($role_type)){
        return $result;
      }
      
      $roles = array();
      foreach ($result as $record) {      
        // take care of spaces for url's
        $rname = str_replace(' ', '_', $record->name);
        $roles[$rname] = array('rid'=>$record->rid,'name'=>$rname);
      }
      return $roles;

    case 'name':
      
      if (is_null($role_type)) {
        return FALSE;
      } 
      return $result['name']; 
  }
}

/**
 * 
 * Checks if the user has the role
 * @param int $uid    user's id
 * @param string $role  the role name to check
 * @return         true if the user has the role
 */
function user_list_has_role($uid, $role) {
  $roles = user_load($uid)->roles;
  foreach ($roles as $r) {
    if (stripos($r, $role) !== FALSE) return true;
  }
  return false;
}

/**
 * 
 * @param int $uid  the user's id
 * @param int $fid  id of the field in wich to search
 * @return       field_id => label array of fields assosiated with the user
 */
function user_list_user_fields($uid, $fid) {
  $fields = array();
  $all_fields = user_list_get_all_fields();
  $user = user_load($uid);
  foreach ($all_fields as $fname => $field) {
    if ($fid != $field['field_id']) continue;
    $user_field = $user->$fname;
    $field_name = null;
    if (count($user_field) > 0) {
      $field_name = isset($user_field['und'][0]['value']) ? $user_field['und'][0]['value'] : null; //TODO: Notice: Undefined index: tid
    }
    if ($field_name == null) continue;
    $fields[$fname] = $field_name;
    /*$term = taxonomy_term_load($tid); //Loads the term, from where we can get the appropriate vid 
    $taxonomy_tree = taxonomy_get_tree($term->vid);
    foreach ($taxonomy_tree as $tax) {
      if ($tid == $tax->tid) {
        $fields[$fname] = $tax->name;
      }
    }*/
  }
  
  return $fields;
}

/**
 * 
 * @param array $users  list of the users among whom the role is searched
 * @param int  $uid  id of the user whose role id is returned
 * @return         corresponding role rid of the given user, FALSE if not found 
 * @author Vahe 02.05.2011
 */
function user_list_get_user_role_id($users, $uid) {
  $roles = array_map('check_plain', user_roles(TRUE));
  foreach ($users as $user) {
    $users_roles = array();
      $roles_result = db_query('SELECT rid, uid FROM {users_roles} WHERE uid = :uid', array(':uid' => $user->uid));
      foreach ($roles_result as $user_role) {
        if ($uid == $user_role->uid) {
          return $user_role->rid;
        }
      }
  }
  return FALSE;
}

/**
 * 
 * Returns the intersection of arrays of users comparing them by their ids
 * @param array $array1  first array to intersect
 * @param array $array2  second array to intersect
 * @return         the intersection of arrays of users
 */
function user_list_user_array_intersect_by_uid($array1, $array2) {
  if (count($array1) == 0 || count($array2) == 0) {
    return array();
  }
  $res = array();
  foreach ($array1 as $u1) {
    foreach ($array2 as $u2) {
      if ($u1->uid == $u2->uid) {
        $res[] = $u1;
      }
    }
  }
  return $res;
}

/**
 * 
 * Returns the difference of arrays of users comparing them by their ids
 * @param array $array1  from wich elements the is subtracted
 * @param array $array2  which elements are subtracted
 * @return         the difference of arrays of users
 */
function user_list_user_array_diff_by_uid($array1, $array2) {
  
  $res = $array1;
  foreach ($array1 as $k1=>$u1) {
    foreach ($array2 as $u2) {
      if ($u1->uid == $u2->uid) {
        unset($res[$k1]);
      }
    }
  }  
  return $res;
}

/**
 * 
 * Returns true if any of the elements of $array contains $string, and false otherwise
 * @param array  $array    array of string elements
 * @param string $string  string to be searched in the array
 * @return           true if string is found, false - otherwise
 */
function user_list_contains_array($string, $array) {
  foreach ($array as $val) {
    if (stripos($val, $string) !== FALSE) return true;
  }
  return false;
}

/**
 * 
 * @param string $timestamp  timestamp for the date
 * @param string $interval  string in start,end format, where start and end are both in any PHP supported date format
 * @return      true if the given $date is in $interval, otherwise false is returned
 */
function user_list_date_is_in_interval($timestamp, $interval) {
  //TODO: php does not support dd/mm/yyyy format, but this replacement can break other formats
  //$interval = str_replace(array("/","."), "-", $interval); 
  if (stripos($interval, ",") === FALSE) {
    $interval .= ",";
  }
  $date = getdate($timestamp);
  $start_end = explode(',', $interval);
  
  if (strtotime($start_end[0])) {
    $start = getdate(strtotime($start_end[0]));
  } elseif (strtotime($start_end[1]) === FALSE) {
    return false;
  }
  $end = getdate(strlen(strtotime($start_end[1])) > 0 ? strtotime($start_end[1]) : time());
    
  return  (user_list_compare_dates($date, $start) >= 0 && user_list_compare_dates($date, $end) <= 0);
}

/**
 * 
 * Compares the given date arrays
 * @param array $date1  first date to be compared
 * @param array $date2  second date to be compared
 * @return         1 if $date1 > $date2, 0 if $date1 == $date2, -1 otherwise
 */
function user_list_compare_dates($date1, $date2) {
  if ($date1['year'] > $date2['year']) return 1;
  elseif ($date1['year'] == $date2['year'] && $date1['mon'] > $date2['mon']) return 1;
  elseif ($date1['year'] == $date2['year'] && $date1['mon'] == $date2['mon'] && $date1['mday'] > $date2['mday']) return 1;
  elseif ($date1['year'] == $date2['year'] && $date1['mon'] == $date2['mon'] && $date1['mday'] == $date2['mday']) return 0;
  else return -1;
}

/**
 * 
 * Returns the field_id of the field biven by its name
 * @param string $fname the field's name to check
 * @return         the field's id
 */
function user_list_get_fid_by_fname($fname) {
  $all_fields = user_list_get_all_fields();
  foreach ($all_fields as $field) {
    if ($field['field_name'] == $fname) {
      return $field['field_id'];
    }
  }
}

/**
 * 
 * Returns the keyword for the given field
 * @param array $field  field for which a keyword is generated
 * @return         keyword for the given field
 */
function user_list_get_field_keyword($field) {
  return strtoupper(substr($field['field_name'], strlen('field_')));
}
