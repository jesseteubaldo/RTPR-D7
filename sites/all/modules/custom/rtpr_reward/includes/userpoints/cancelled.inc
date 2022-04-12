<?php
/**
 * @file
 */
 
function rtpr_reward_userpoints_cancelled_order_form($form, $form_state) {
	$query = db_select('userpoints_txn', 't');
	$query->join('commerce_order', 'o', 't.reference = o.order_id');

	$query->fields('t');
	$query->condition('reference', '', '!=');
	$query->condition('points', '0', '>');
	$query->condition('o.status', 'canceled');
	$query->isNotNull('reference');
	
	$results = $query->execute()->fetchAllAssoc('reference');
	
	$header = array(
		'id' => t('ID'),
		'credit' => t('Credit User'),
		'referrer' => t('Referrer'),
		'points' => t('Points'),
		'date' => t('Date'),
		'desc' => t('Description'),
		'op' => t('Operation'),
	);
	foreach($results as $order_id => $transaction) {
		$order = commerce_order_load($order_id);
		$credit_account = user_load($transaction->uid);
		$referrer_account = user_load($order->uid);
		$id = $transaction->txn_id;
		$rows[] = array(
			$id,
			l($credit_account->mail, '/user/' . $credit_account->uid . '/points'),
			$referrer_account->mail,
			$transaction->points,
			date('m/d/Y H:i:s', $transaction->time_stamp),
			$transaction->description,
			l('edit', 'admin/config/people/userpoints/transaction/' . $id . '/edit')
		);
	}
	
	$form['table'] = array(
	  '#theme' => 'table',
	  '#header' => $header,
	  '#rows' => $rows,
	  '#empty' => t('No transaction available.'),
	);
	return $form;
}