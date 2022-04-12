<?php
  
/**
 * Extra commerce features.
 */
 
 
/**
 * Shipping department to update tracking number.
 */
function rtpr_extras_shipment_form($form, $form_state) {

  drupal_set_title(t('Administer Shipment'));
  
  $form['options'] = array(
        '#type' => 'container',
        '#attributes' => array(
            'class' => array('container-inline'),
        ),
  );
  
  $form['options']['order_id'] = array(
    '#type' => 'textfield',
    '#title' => t('Order ID'),
    '#required' => TRUE,
  );
  
  $form['options']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Search'),
  );

  return $form;
}

/**
 * Validate tracking handler.
 */
function rtpr_extras_shipment_form_validate($form, &$form_state) {
  $values = $form_state['values'];
  
  $order_id = $values['order_id'];
  
  $order = commerce_order_load($order_id);
  if(!$order)  {
    form_set_error('order_id', t('Order #@id does not exist!', array('@id' => $order_id)));
  } else if ($order->status != 'processing') {
    form_set_error('order_id', t('The order number you searched for is not in a processing status. Please verify the order number and search again.'));
    
  }
}

/**
 * Add tracking submit handler.
 */
function rtpr_extras_shipment_form_submit($form, &$form_state) {
  $values = $form_state['values'];
  
  $order_id = $values['order_id'];
  
  $form_state['redirect'] = 'admin/commerce/orders/'.$order_id.'/shipment';
}

