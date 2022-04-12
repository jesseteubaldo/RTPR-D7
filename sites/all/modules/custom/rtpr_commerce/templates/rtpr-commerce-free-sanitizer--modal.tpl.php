<!-- Modal -->
<div class="modal fade" id="gopakModal" tabindex="-1" role="dialog" aria-labelledby="gopakModalLabel" aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title" id="gopakModalLabel"><?php print variable_get('rtpr_commerce_sanitizer_modal_title'); ?></h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
        <?php $sanitizer_message = variable_get('rtpr_commerce_sanitizer_modal_body'); print $sanitizer_message['value']; ?>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">No, thanks</button>
      </div>
    </div>
  </div>
</div>