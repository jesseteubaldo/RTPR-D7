<?php

/**
 * @file
 */

/**
 *
 */
class PAPUserData {
  const ROLE_AFFILIATE_RID = 4;
  const ROLE_RETAIL_RID = 7;
  const ROLE_VENDOR_RID = 8;
  const ROLE_GOLD_RID = 9;
  const ROLE_PLATINUM_RID = 22;
  const ROLE_OVERRIDE_RID = 30;
  const ROLE_VENDOR_OWNER_RID = 36;
  const ROLE_DIAMOND_CLUB_RID = 40;
  const TRANSACTION_STATUS_SUCCESS = 'success';
  private $pap_id;
  private $month;
  private $year;
  private $account;
  private $entity_wrapper;

  /**
   * PAPUserData constructor.
   *
   * @param $pap_id
   * @param $month
   * @param $year
   * @param $account
   * @param $entity_wrapper
   */
  function __construct($pap_id, $month = NULL, $year = NULL, $account = NULL, $entity_wrapper = NULL) {
    $this->pap_id = $pap_id;
    $this->month = $month ? $month : date('m');
    $this->year = $year ? $year : date('Y');
    $this->account = $account;
    $this->entity_wrapper = $entity_wrapper;
  }

  /**
   *
   */
  function getMonthlyRetailTotal() {
    db_set_active('pap');
    $start_date = $this->year . '-' . $this->month . '-01 00:00:00';
    $end_date = $this->year . '-' . $this->month . '-31 23:59:59';

    $results = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $this->pap_id, '=')
          ->condition('rtype', 'R', '!=')
          ->condition('campaignid', '11111111', '=')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
          ->execute();

    $total = 0;
    foreach ($results as $row) {
      if ($row->totalcost) {
        $total += $row->totalcost;
      }
    }

    db_set_active();

    return $total;
  }

  /**
   *
   */
  function hasRole($rid) {
    $roles = $this->account->roles;
    $result = 'NO';

    if (array_key_exists($rid, $roles)) {
      $result = 'YES';
    }

    return $result;
  }

  /**
   *
   */
  function getVolumeTotal($type, $pap_id = '') {
    if (!$pap_id) {
      $pap_id = $this->pap_id;
    }
    $bonus = array(
      'msp' => array(
        'tiers' => array(1),
        'campaigns' => array('c5d380ba', 'c602cb08', '39e673de'),
      ),
      'sav' => array(
        'tiers' => array(1, 2),
        'campaigns' => array('c5d380ba', 'c602cb08', '39e673de', '33fadfd9'),
      ),
      'rav' => array(
        'tiers' => array(1, 2, 3, 4),
        'campaigns' => array('c5d380ba', 'c602cb08', '39e673de', '33fadfd9'),
      ),
      'nav' => array(
        'tiers' => array(1, 2, 3, 4, 5, 6, 7, 8),
        'campaigns' => array('c5d380ba', 'c602cb08', '39e673de', '33fadfd9'),
      ),
    );

    db_set_active('pap');
    $tiers = $bonus[$type]['tiers'];
    $campaigns = $bonus[$type]['campaigns'];
    $start_date = $this->year . '-' . $this->month . '-01';
    $end_date = $this->year . '-' . $this->month . '-31 23:59:59';

    $results = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $pap_id, '=')
          ->condition('tier', $tiers, 'IN')
          ->condition('rtype', 'R', '!=')
          ->condition('campaignid', $campaigns, 'IN')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
          ->execute();

    $correct_total = 0;
    foreach ($results as $row) {
      if ($row->totalcost) {
        $correct_total += $row->totalcost;
      }
    }

    db_set_active();
    return $correct_total;
  }

  /**
   * Return the primary role of the user.
   */
  function getPrimaryRole($username = '') {
    $roles = array();
    if (!$username) {
      $roles = $this->account->roles;
    }
    else {
      $account = user_load_by_mail($username);
      if ($account) {
        $roles = $account->roles;
      }
    }

    return rtpr_user_get_primary_role($roles);
  }

  /**
   *
   */
  function getCustomerReferralData($pap_id = '') {
    if (!$pap_id) {
      $pap_id = $this->pap_id;
    }
    $start_date = $this->year . '-' . $this->month . '-01';
    $end_date = $this->year . '-' . $this->month . '-31';
    $start_date = $start_date . ' 00:00:00';
    $end_date = $end_date . ' 23:59:59';

    $query = db_select('field_data_field_pap_referral_id', 'r');
    $query->innerJoin('users_roles', 'u', 'r.entity_id = u.uid');
    $query->fields('r')->fields('u', array('rid'))
          ->condition('r.field_pap_referral_id_value', $pap_id, '=')
          ->condition('u.rid', 7, '=');
    $results = $query->execute()->fetchAllAssoc('entity_id');

    $total_amount = 0;
    $purchase = array();
    foreach ($results as $entity_id => $result) {
      $query = db_select('commerce_order', 'o');
      $query->innerJoin('field_data_commerce_order_total', 't', 'o.order_id = t.entity_id');
      $query->fields('t')->fields('o')
              ->condition('o.status', rtpr_offer_order_success_statuses(), 'IN')
              ->condition('o.uid', $entity_id, '=');
      // $alias = $query->addExpression('SUM(commerce_order_total_amount)', 'total_amount');.
      $orders = $query->execute()->fetchAllAssoc('order_id');
      foreach ($orders as $order_id => $data) {
        $order = commerce_order_load($order_id);
        $purchase[$order->uid] = $order->uid;
        $wrapper = entity_metadata_wrapper('commerce_order', $order);
        $line_items = $wrapper->commerce_line_items;
        $total = commerce_line_items_total($line_items, array('product'));
        $total_amount += $total['amount'];
      }
    }

    db_set_active('pap');
    $query = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $pap_id)
          ->condition('campaignid', '11111111')
          ->condition('payoutstatus', 'U')
          ->condition('rtype', 'R', '!=')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
          ->condition('tier', 1, '=');
    $query->addExpression('SUM(totalcost)', 'monthly_unpaid');;
    $transactions = $query->execute()->fetchAll();
    $tier_1_total = 0;

    if (isset($transactions[0])) {
      $tier_1_total = $transactions[0]->monthly_unpaid;
      $tier_1_total *= 100;
    }

    $query = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $pap_id)
          ->condition('campaignid', 'c602cb08')
          ->condition('payoutstatus', 'U')
          ->condition('rtype', 'R', '!=')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
          ->condition('tier', 2, '=');
    $query->addExpression('SUM(totalcost)', 'monthly_unpaid');;
    $transactions = $query->execute()->fetchAll();
    $tier_2_total = 0;

    if (isset($transactions[0])) {
      $tier_2_total = $transactions[0]->monthly_unpaid;
      $tier_2_total *= 100;
    }

    $query = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $pap_id)
          ->condition('campaignid', array('11111111', 'c602cb08'), 'IN')
          ->condition('payoutstatus', 'U')
          ->condition('rtype', 'R', '!=')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN');
    $query->addExpression('SUM(commission)', 'monthly_commission');;
    $transactions = $query->execute()->fetchAll();
    $total_commission = 0;

    if (isset($transactions[0])) {
      $total_commission = $transactions[0]->monthly_commission;
      $total_commission *= 100;
    }

    db_set_active();

    return array(
      'count' => count($results),
      'purchase' => count($purchase),
      'tier1' => commerce_currency_format($tier_1_total, 'USD'),
      'tier2' => commerce_currency_format($tier_2_total, 'USD'),
      'total' => $tier_1_total + $tier_2_total,
      'total_commission' => commerce_currency_format($total_commission, 'USD'),
    );
  }

  /**
   *
   */
  function getTotalAPV($pap_id = '') {
    if (!$pap_id) {
      $pap_id = $this->pap_id;
    }

    db_set_active('pap');
    $start_date = $this->year . '-' . $this->month . '-01 00:00:00';
    $end_date = $this->year . '-' . $this->month . '-31 23:59:59';
    $correct_total = 0;
    // Tier 1 Retail (campaign 11111111) total cost divided by 2.
    $results = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $pap_id, '=')
          ->condition('tier', '1', '=')
          ->condition('rtype', 'R', '!=')
          ->condition('campaignid', '11111111', '=')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
          ->execute();
    foreach ($results as $row) {
      if ($row->totalcost) {
        $correct_total += $row->totalcost;
      }
    }
    $correct_total = $correct_total / 2;
    $results = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $pap_id, '=')
          ->condition('tier', '1', '=')
          ->condition('rtype', 'R', '!=')
          ->condition('campaignid', 'c5d380ba', '=')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
          ->execute();

    foreach ($results as $row) {
      if ($row->totalcost) {
        $correct_total += $row->totalcost;
      }
    }

    $results = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $pap_id, '=')
          ->condition('tier', '2', '=')
          ->condition('rtype', 'R', '!=')
          ->condition('campaignid', 'c602cb08', '=')
          ->condition('dateinserted', array($start_date, $end_date), 'BETWEEN')
          ->execute();

    foreach ($results as $row) {
      if ($row->totalcost) {
        $correct_total += ($row->totalcost / 2);
      }
    }

    db_set_active();
    return $correct_total;
  }

  /**
   *
   */
  function getPlatinumReferrals($pap_id = '') {
    $count = 0;
    if (!$pap_id) {
      $pap_id = $this->pap_id;
    }
    db_set_active('pap');

    $query = db_select('qu_pap_users', 'pu')
          ->fields('pu', array('userid'))
          ->condition('pu.parentuserid', $pap_id, '=');

    $results = $query->execute()->fetchAllAssoc('userid');
    $user_ids = array_keys($results);
    db_set_active();

    if (!empty($user_ids)) {
      $query = db_select('field_data_field_pap_affiliate_id', 'pap');
      $query->join('users_roles', 'u', 'pap.entity_id = u.uid');

      $query->fields('pap')
              ->condition('field_pap_affiliate_id_value', $user_ids, 'IN')
              ->condition('rid', array(PAPUserData::ROLE_VENDOR_RID, PAPUserData::ROLE_GOLD_RID, PAPUserData::ROLE_PLATINUM_RID), 'IN');
      $referrals = $query->execute()->fetchAllAssoc('entity_id');
      foreach ($referrals as $referral) {
        //$msp = $this->getVolumeTotal('msp', $referral->field_pap_affiliate_id_value);
        $total_apv = $this->getTotalAPV($referral->field_pap_affiliate_id_value);
        if ($total_apv >= 498) {
          $count++;
        }
      }
    }

    return $count;
  }

  /**
   *
   */
  function getNewReferrals($pap_id = '') {
    if (!$pap_id) {
      $pap_id = $this->pap_id;
    }

    // Get all retail customer.
    $query = db_select('field_data_field_pap_referral_id', 'pap');
    $query->join('users_roles', 'r', 'pap.entity_id = r.uid');
    $query->fields('pap')
        ->condition('field_pap_referral_id_value', $pap_id, '=')
        ->condition('rid', 7, '=');
    $results = $query->execute()->fetchAllAssoc('entity_id');

    // Get all orders by retail customer.
    $user_ids = array_keys($results);
    if (!empty($user_ids)) {
      $query = db_select('commerce_order', 'o')
        ->fields('o', array('order_id', 'uid', 'created'))
        ->condition('uid', $user_ids, 'IN');

      $orders = $query->execute()->fetchAllAssoc('order_id');

      $order_ids = array_keys($orders);
    }
    // Count retail orders in given month/year.
    $count = array();
    if (!empty($order_ids)) {
      db_set_active('pap');

      $query = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('rtype', 'R', '!=')
          ->condition('orderid', $order_ids, 'IN')
          ->condition('userid', $pap_id, '=')
          ->condition('orderid', '156790', '!=')
          ->condition('dateinserted', $this->getEndDate(), '<');

      $results = $query->execute()->fetchAllAssoc('orderid');

      foreach ($results as $order_id => $result) {
        $order = $orders[$order_id];
        $uid = $order->uid;

        $count[$uid][] = $order_id;
      }

      db_set_active();
    }
    $temp = $count;
    foreach ($temp as $uid => $orders) {
      if ($status = _rtpr_commerce_has_buck_order_and_others($uid)) {
        if ($status === 'BO') {
          unset($count[$uid]);
        }
      }
      if (count($orders) == 1 && $results[$orders[0]]->dateinserted < $this->getStartDate()) {
        unset($count[$uid]);
      }
      elseif (count($orders) > 1) {
        foreach ($orders as $id) {
          if ($results[$id]->dateinserted < $this->getStartDate()) {
            unset($count[$uid]);
          }
        }
      }
    }
    return count($count);
  }

  /**
   *
   */
  function getStartDate() {
    return $this->year . '-' . $this->month . '-01 00:00:00';
  }

  /**
   *
   */
  function getEndDate() {
    return $this->year . '-' . $this->month . '-31 23:59:59';
  }

  /**
   *
   */
  function getParentData($pap_id = '') {
    if (!$pap_id) {
      $pap_id = $this->pap_id;
    }
    db_set_active('pap');

    $query = db_select('qu_pap_users', 'pu');
    $query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
    $query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
    $query->fields('pu', array('parentuserid'))
          ->condition('pu.userid', $pap_id, '=');

    $user_result = $query->execute()->fetchAssoc();

    $sub_query = db_select('qu_pap_users', 'pu');
    $sub_query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
    $sub_query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
    $sub_query->fields('ga', array('username', 'firstname', 'lastname'))
          ->fields('pu', array('userid', 'data5'))
          ->condition('pu.userid', $user_result['parentuserid'], '=');
    $result = $sub_query->execute()->fetchAssoc();

    db_set_active();
    return $result;
  }

  /**
   *
   */
  function getCommissions($status = 'A') {
    db_set_active('pap');
    $query = db_select('qu_pap_transactions', 't')
          ->fields('t')
          ->condition('userid', $this->pap_id, '=')
          ->condition('payoutstatus', 'U')
          ->condition('rtype', 'R', '!=')
          ->condition('rstatus', $status);
    // ->condition('dateinserted', array($this->getStartDate(), $this->getEndDate()), 'BETWEEN');.
    $alias = $query->addExpression('SUM(commission)', 'total_commission');

    $results = $query->execute()->fetchAll();

    db_set_active();

    return $results[0]->total_commission;
  }

  /**
   *
   */
  function getPayoutHistory($limit = 6) {
    db_set_active('pap');
    $query = db_select('qu_pap_payout', 't');
    $query->join('qu_pap_payouthistory', 'h', 'h.payouthistoryid = t.payouthistoryid');
    $query->fields('t', array('invoicenumber', 'amount', 'affiliatenote'));
    $query->fields('h', array('dateinserted'));
    $query->condition('userid', $this->pap_id, '=')
          ->condition('dateinserted', date('Y-m-d H:i:s', strtotime('-6 months')), '>')
          ->orderBy('dateinserted', 'DESC');

    $results = $query->execute()->fetchAll();

    db_set_active();

    return $results;
  }

  /**
   * Get monthly retail data.
   */
  public function getMonthlyRetailData($tier = 1) {
    db_set_active('pap');
    $start_date = $this->year . '-' . $this->month . '-01 00:00:00';
    $end_date = $this->year . '-' . $this->month . '-31 23:59:59';

    $query = db_select('qu_pap_transactions', 't')
      ->fields('t')
      ->condition('t.userid', $this->pap_id, '=')
      ->condition('t.rtype', 'R', '!=')
      ->condition('t.campaignid', '11111111', '=')
      ->condition('t.tier', $tier)
      ->condition('t.dateinserted', array($start_date, $end_date), 'BETWEEN');
    $results = $query->execute();
    db_set_active();
    $count = 0;
    $new = 0;
    $redemption = 0;
    $output = array();
    foreach ($results as $row) {
      if ($row->totalcost) {
        $order = commerce_order_load($row->orderid);
        if ($order) {
          $uid = $order->uid;
          $account = user_load($uid);
          if (($account->created >= strtotime($start_date) && $account->created <= strtotime($end_date)) || $row->data4 == 'Co-Op') {
            $new++;
          }
          $payments = commerce_payment_transaction_load_multiple(array(), array('order_id' => $order->order_id));
          if (!$payments) {
            $redemption++;
          }
          $count++;
        }
      }
    }

    $output['count'] = $count;
    $output['new'] = $new;
    $output['redemption'] = $redemption;
    return $output;
  }

  /**
   * Number of people they are the parent of in PAP.
   */
  public function getTotalReferral() {
    db_set_active('pap');

    $query = db_select('qu_pap_users', 'pu');
    $query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
    $query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
    $query->fields('pu', array('userid'))
      ->condition('pu.parentuserid', $this->pap_id, '=');

    $user_result = $query->execute()->fetchAll();

    db_set_active();

    return count($user_result);
  }

  /**
   * Total New Personal Wholesale Referrals this month.
   */
  public function getNewMonthlyReferralTotal() {
    db_set_active('pap');

    $start_date = $this->year . '-' . $this->month . '-01 00:00:00';
    $end_date = $this->year . '-' . $this->month . '-31 23:59:59';
    $query = db_select('qu_pap_users', 'pu');
    $query->innerjoin('qu_g_users', 'gu', 'pu.accountuserid=gu.accountuserid');
    $query->innerjoin('qu_g_authusers', 'ga', 'ga.authid = gu.authid');
    $query->innerjoin('qu_pap_userincommissiongroup', 'c', 'c.userid = pu.userid');
    $query->fields('pu', array('userid'))
      ->condition('pu.dateinserted', array($start_date, $end_date), 'BETWEEN')
      ->condition('pu.parentuserid', $this->pap_id, '=')
      ->condition('c.commissiongroupid', array('45609ce9', '93abaa31', 'd22f9795', '12b433fc', '69b77e47', 'defe8bfe'), 'IN');

    $user_result = $query->execute()->fetchAll();

    db_set_active();

    return count($user_result);
  }

/**
   * Total New Personal Wholesale Referrals this month.
   */
  public function getGccUsed() {
    $total = 0;
    db_set_active('pap');
    $start_date = $this->year . '-' . $this->month . '-01 00:00:00';
    $end_date = $this->year . '-' . $this->month . '-31 23:59:59';
    $query = db_select('qu_pap_transactions', 't')
      ->fields('t')
      ->condition('t.userid', $this->pap_id, '=')
      ->condition('t.orderid', 'GCC%', 'LIKE')
      ->condition('t.tier', 1)
      ->condition('t.dateinserted', array($start_date, $end_date), 'BETWEEN');
    $results = $query->execute();
    $user_result = $query->execute()->fetchAll();
    $total = count($user_result);
    db_set_active();

    return $total;
  }

}
