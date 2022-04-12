<!-- Modal -->
<div class="modal fade" id="introductoryModal" tabindex="-1" role="dialog" aria-labelledby="introductoryModalLabel" aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title" id="introductoryModalLabel"><?php print variable_get('rtpr_commerce_introductory_modal_title', 'Introductory'); ?></h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
        <?php $message = variable_get('rtpr_commerce_introductory_modal_body'); print $message['value']; ?>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">No, thanks!</button>
      </div>
    </div>
  </div>
</div>