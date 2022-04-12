<?php

/**
 * @file
 */
 
/**
 * Implements hook_views_data().
 */
function rtpr_reward_views_data() {
    $data = array();

    $data['rtpr_reward']['table']['group'] = t('RTPR Reward');
    $data['rtpr_reward']['table']['base'] = array(
        'title' => t('RTPR Reward'),
        'help' => t('RTPR Reward table.'),
    );
    $data['rtpr_reward']['table']['join'] = array(
      'user' => array(
        'left_field' => 'uid',
        'field' => 'uid',
      ),
    );
    $base_table = 'rtpr_reward';
    $data[$base_table]['date_filter'] = array(
      'group' => t('Date'),
      'title' => t('Date (!base_table)', array('!base_table' => $base_table)),
      'help' => t('Filter any Views !base_table date field.', array('!base_table' => $base_table)),
      'filter' => array(
        'handler' => 'date_views_filter_handler',
        'empty field name' => t('Undated'),
        'is date' => TRUE,
        //'skip base' => $base_table,
      ),
    );
        
    // The ID field
    $data['rtpr_reward']['id'] = array(
        'title' => t('ID'),
        'help' => t('Reward ID.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
    // The Type field
    $data['rtpr_reward']['type'] = array(
        'title' => t('Type'),
        'help' => t('Reward type.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    
    // The Point field
    $data['rtpr_reward']['point'] = array(
        'title' => t('Point'),
        'help' => t('Reward Point.'),
        'field' => array(
            'handler' => 'views_handler_field_user',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
    // The UID field
    $data['rtpr_reward']['uid'] = array(
        'title' => t('User ID'),
        'help' => t('Reward UID.'),
        'field' => array(
            'handler' => 'views_handler_field_user',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
        'relationship' => array(
          'base' => 'users',
          'field' => 'uid',
          'handler' => 'views_handler_relationship',
          'label' => t('User'),
        ),
        'argument' => array(
          'handler' => 'views_handler_argument_user_uid',
          'numeric' => TRUE,
          'validate type' => 'uid',
      ),

    );
    
    // The Email field
    $data['rtpr_reward']['email'] = array(
        'title' => t('Email'),
        'help' => t('Reward Email.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    
    // The Month field
    $data['rtpr_reward']['month'] = array(
        'title' => t('Month'),
        'help' => t('Reward Month.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
    // The Year field
    $data['rtpr_reward']['year'] = array(
        'title' => t('Year'),
        'help' => t('Reward Year.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
    // The Comment field
    $data['rtpr_reward']['comment'] = array(
        'title' => t('Comment'),
        'help' => t('Reward comment.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );
    
    // The Timestamp field
    $data['rtpr_reward']['timestamp'] = array(
        'title' => t('Timestamp'),
        'help' => t('Reward timestamp.'),
        'field' => array(
        'handler' => 'views_handler_field_date',
          'click sortable' => TRUE,
        ),
        'sort' => array(
          'handler' => 'views_handler_sort_date',
        ),
        'filter' => array(
          'handler' => 'views_handler_filter_date',
        ),
  );
    
    // The Batch field
    $data['rtpr_reward']['batch'] = array(
        'title' => t('Batch'),
        'help' => t('Reward Batch ID.'),
        'field' => array(
            'handler' => 'views_handler_field_numeric',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_numeric',
        ),
    );
    
    // The Point Type field
    $data['rtpr_reward']['point_type'] = array(
        'title' => t('Point Type'),
        'help' => t('Reward Point Type.'),
        'field' => array(
            'handler' => 'views_handler_field',
        ),
        'sort' => array(
            'handler' => 'views_handler_sort',
        ),
        'filter' => array(
            'handler' => 'views_handler_filter_string',
        ),
    );

    return $data;

} 