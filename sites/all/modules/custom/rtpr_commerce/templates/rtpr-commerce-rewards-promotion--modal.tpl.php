<!-- Modal -->
<div class="modal fade" id="rewardsPromotionModal" tabindex="-1" role="dialog" aria-labelledby="rewardsPromotionModalLabel" aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title" id="rewardsPromotionModalLabel"><?php print variable_get('rtpr_commerce_rewards_promotion_modal_title'); ?></h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
        <?php $body = variable_get('rtpr_commerce_rewards_promotion_modal_body'); print $body['value']; ?>
        <ul>
          <strong>Available Offers:</strong>
          <?php foreach ($variables['available_offers'] as $sku): ?>
            <?php $product = commerce_product_load_by_sku($sku); ?>
            <?php $price = entity_metadata_wrapper('commerce_product', $product)->commerce_price->value(); ?>
            <?php $price_display = commerce_currency_format($price['amount'], $price['currency_code'], $product);?>
            <li><a href="/commerce/buck/<?php print $sku;?>"><?php print $product->title;?> - <?php print $price_display; ?></a></li>
          <?php endforeach; ?>
        </ul>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">No, thanks!</button>
      </div>
    </div>
  </div>
</div>