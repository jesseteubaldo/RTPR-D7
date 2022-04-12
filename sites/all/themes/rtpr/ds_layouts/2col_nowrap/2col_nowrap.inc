<?php
//http://drupalcode.org/viewvc/drupal/contributions/modules/ds/ds.api.php?revision=1.1.2.4&view=markup&pathrev=DRUPAL-7--1
// from around line 122

function ds_2col_nowrap() {
  return array(
    'label' => t('2 col - No Wrap'),
    'regions' => array(
      // The key of this region name is also the variable used in
      // the template to print the content of that region.  
      'top' => t('top'),
      'left' => t('left'), 
      'right' => t('right'), 
      'bottom' => t('bottom'),
    ),
    // Add this if there is a default css file.
    //'css' => TRUE,
  );
}

