<?php
  
/**
 * @file
 *
 * User reward page.
 */

// Summary form
function rtpr_ebp_user_summary_form($form, &$form_state, $uid) {
  $ebp = rtpr_ebp_get_total($uid, 'ebp');
  $multi_factor = rtpr_ebp_get_total($uid, 'multi_factor');
  $super_bonus = rtpr_ebp_get_total($uid, 'super_bonus');
  
  if(!$multi_factor) {
    $multi_factor = 1;
  }
  
  if($ebp) {
    $total = ($ebp * $multi_factor) + $super_bonus;
    
    $form['vo_help'] = array(
      '#markup' => '<h4><strong>Your Total Points for the Vendor-Owner Program = ' . number_format($total). '</strong></h4>',
    );
    
    $form['vo_formula'] = array(
      '#markup' => t('Your Total Points <b>(@total)</b> are calculated by taking your Total EBP <b>(@ebp)</b> and multiplying them by your Total Multiplication Factor Points <b>(@multi)</b> then adding in your Total Super Bonus Points <b>(@bonus)</b>. You can see an itemization of your points below', array('@total' => number_format($total), '@ebp' => number_format($ebp), '@multi' => number_format($multi_factor), '@bonus' => number_format($super_bonus))),
    );
    
    $form['vo_sep'] = array(
      '#markup' => '<hr class="dm" />',
    );
    
  }
  
  $form['#attached']['css'] = array(
  drupal_get_path('module', 'rtpr_reward') . '/rtpr_reward.css',
  );
  
  $ebp_options = array(
    'all' => t('All'),
    'ebp' => t('EBP'),
    'super_bonus' => t('Super Bonus Points'),
    'multi_factor' => t('Multiplication Factor Points')
  );
  
  $form['type'] = array(
    '#type' => 'select',
    '#options' => $ebp_options,
    '#default_value' => 'ebp',
    '#ajax' => array(
      'callback' => 'rtpr_ebp_dropdown_callback',
      'wrapper' =>  'ebp_summary_form',
    )
  );
  
  $header = array(
    'month' => t('Month'),
    'year' => t('Year'),
    'comment' => t('Comment'),
    'points' => t('Points'),
  );
  
  $rows = rtpr_ebp_get_rows($uid);
  $form_state['uid'] = $uid;
  $form_state['header'] = $header;
/*
  $per_page = 50;
  
  $current_page = pager_default_initialize(count($rows), $per_page);
  // split array into page sized chunks
  $chunks = array_chunk($rows, $per_page, TRUE);
  
*/
  $form['summary'] = array(
    '#prefix' => '<div id="ebp_summary_form">',
    '#suffix' => '</div>',
  );

  $form['summary']['table'] = array(
    '#theme' => 'table',
    '#header' => $header,
//     '#rows' => isset($chunks[$current_page]) ? $chunks[$current_page] : array(),
    '#rows' => $rows,
    '#empty' => t('No available data.'),
  );
  
/*
  $form['pager'] = array(
    '#theme' => 'pager',
  );
*/

  
  return $form;
}
