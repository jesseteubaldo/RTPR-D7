<div class="referral-info table <?php print strtolower($data['is_sampler']); ?>">
  <div class="row index">
    <div class="col-md-12"><b>#<?php print $data['index']; ?></b></div>
  </div>
  <div class="row">
    <div class="col-md-4"><label>Name: </label>
      <?php 
      if(isset($data['field_first_name_value']) && isset($data['field_first_name_value']))
        print $data['field_first_name_value']  . ' ' . $data['field_last_name_value']; 
      ?>
    </div>
    <div class="col-md-5"><label>Online ID: </label><?php print $data['pap_id']; ?></div>
    <div class="col-md-3"><label>State: </label><?php print $data['state']; ?></div>
  </div>
  <div class="row">
    <div class="col-md-4"><label>Group: </label><?php print $data['is_sampler']; ?></div>
    <div class="col-md-5"><label>Customers: </label><?php print $data['retail_count']; ?></div>
    <div class="col-md-3"><label>Referrals: </label><?php print $data['ref_count']; ?></div>
  </div>
  <?php if($data['tier'] == 1): ?>
  <div class="row">
    <div class="col-md-4"><label>Email: </label><?php print $data['email']; ?></div>
    <div class="col-md-5"><label>Phone: </label><?php print $data['phone']; ?></div>
    <div class="col-md-3"></div>
  </div>
  <?php endif; ?>
  <div class="row image">
    <div class="col-md-12">
      <?php if($data['tier'] == 1): ?>
        <?php if($data['ref_count'] > 0): ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/Users_Group-64-64.png'; ?>">
        <?php else: ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/User-64-64.png'; ?>">
        <?php endif; ?>
      <?php else: ?>
        <?php if($data['ref_count'] > 0): ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/Users_Group-32-32.png'; ?>">
        <?php else: ?>
          <img src="/<?php print drupal_get_path('module', 'rtpr_pap') . '/images/User-32-32.png'; ?>">
        <?php endif; ?>
      <?php endif; ?>
        
    </div>
  </div>
 <?php if($data['ref_count'] > 0): ?>
  <div class="row last">
    <div class="col-md-12"><a href="#" class="pap-ajax-load not-selected" type="sampler" tier="<?php print $data['tier'];?>" pid="<?php print $data['pap_id']; ?>">+</a></div>
  </div>
  <?php endif; ?>
</div>
  <div class="wrap-<?php print $data['tier']; ?>" id="wrap-<?php print $data['pap_id']; ?>"></div>