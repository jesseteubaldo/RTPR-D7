<?php

/**
 * @file
 * Services related to user.
 */

 function rtpr_commerce_services_shipstation() {
  $post_data = file_get_contents('php://input');
  $json = drupal_json_decode($post_data);

  if (isset($json['resource_type']) && $json['resource_type'] == 'SHIP_NOTIFY') {
    $options = array(
      'headers' => array(
        'Content-Type' => 'application/json',
        'Authorization' => _rtpr_commerce_get_shipstation_key()
      ),
    );
    $result = drupal_http_request($json['resource_url'], $options);
    if ($result->code == '200') {
      $result = drupal_json_decode($result->data);
      foreach ($result['shipments'] as $shipment) {
        if (isset($shipment['orderNumber'])) {
          $order_id = $shipment['orderNumber'];
          if ($order = commerce_order_load($order_id)) {
            $tracking_number = $shipment['trackingNumber'];
            $service_code = $shipment['serviceCode'];
            $carriers = simple_package_tracking_get_carriers();

            if (strpos($service_code, 'usps') === 0) {
              $carrier = 'USPS';
              $carrier_id = 2;
            }
            elseif (strpos($service_code, 'fedex') === 0) {
              $carrier = 'Fedex';
              $carrier_id = 3;
            }
            elseif (strpos($service_code, 'ups') === 0) {
              $carrier = 'UPS';
            }
            $carrier = $carriers[$carrier_id];
            $tracking_id = simple_package_tracking_save_tracking(
              $order_id, $tracking_number, $carrier['url'], $carrier['name'], ''
            );
            if ($tracking_id) {
              $order->status = 'completed';
              commerce_order_save($order);
            }
          }
        }
      }
    }
  }
}