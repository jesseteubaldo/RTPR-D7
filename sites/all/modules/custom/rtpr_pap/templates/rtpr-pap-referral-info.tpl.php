<div class="referral-info table <?php echo $res_data['commission_group_key']; ?>">
  <div class="row index">
    <div class="col-md-12"><b>#<?php print $res_data['index']; ?></b></div>
  </div>
  <div class="row">
    <div class="col-md-4"><label>Name: </label>
      <?php 
      if(isset($res_data['field_first_name_value']) && isset($res_data['field_first_name_value']))
        print $res_data['field_first_name_value']  . ' ' . $res_data['field_last_name_value']; 
      ?>
    </div>
    <div class="col-md-5"><label>Online ID: </label><?php print $pap_data->userid; ?></div>
    <div class="col-md-3"><label>State: </label><?php print strtoupper($pap_data->data5); ?></div>
  </div>
  <div class="row">
    <div class="col-md-4"><label>Group: </label><?php print $res_data['commission_group']; ?></div>
    <div class="col-md-5">
        <?php  if(isset($res_data['mail']) && $res_data['tier'] == 1): ?>
        <label>Email: </label><?php print $res_data['mail']; ?>
        <?php endif; ?>
    </div>
    <div class="col-md-3">
      <?php if($res_data['tier'] == 1): ?>
      <label>Phone: </label><?php print $pap_data->data8; ?>
      <?php endif; ?>
      </div>
  </div>
  <div class="row">
    <div class="col-md-4"><label>Current MSP: </label><?php print $res_data['msp']; ?></div>
    <div class="col-md-5"><label>Date Joined: </label><?php print $pap_data->dateinserted; ?></div>
    <div class="col-md-3"><label>Referrals: </label><?php print $res_data['ref_count']; ?></div>
  </div>
  <div class="row image">
    <div class="col-md-12">
      <?php if($res_data['tier'] == 1): ?>
        <?php if($res_data['ref_count'] > 0): ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/Users_Group-64-64.png'; ?>">
        <?php else: ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/User-64-64.png'; ?>">
        <?php endif; ?>
      <?php else: ?>
        <?php if($res_data['ref_count'] > 0): ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/Users_Group-32-32.png'; ?>">
        <?php else: ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/User-32-32.png'; ?>">
        <?php endif; ?>
      <?php endif; ?>
        
    </div>
  </div>
  <?php if($res_data['ref_count'] > 0): ?>
  <div class="row last">
    <div class="col-md-12"><a href="#" class="pap-ajax-load not-selected" type="aff" tier="<?php print $res_data['tier'];?>" pid="<?php print $pap_data->userid;?>">+</a></div>
  </div>
  <?php endif; ?>
</div>
  <div class="wrap-<?php print $res_data['tier']; ?>" id="wrap-<?php print $pap_data->userid; ?>"></div>