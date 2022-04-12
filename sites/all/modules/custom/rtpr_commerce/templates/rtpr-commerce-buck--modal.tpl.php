<!-- Modal -->
<div class="modal fade" id="buckModal" tabindex="-1" role="dialog" aria-labelledby="buckModalLabel" aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title" id="buckModalLabel"><?php print variable_get('rtpr_commerce_buck_modal_title', 'Best Offer'); ?></h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
        <?php foreach ($variables['upsell'] as $sku): ?>
          <?php $sku_message = variable_get($sku); print $sku_message['value']; ?>
        <?php endforeach; ?>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">No, thanks!</button>
      </div>
    </div>
  </div>
</div>