<!-- Modal -->
<div class="modal fade" id="userAgreementModal" tabindex="-1" role="dialog" aria-labelledby="userAgreementModal" aria-hidden="true">
  <div class="modal-dialog" role="document">
    <div class="modal-content">
      <div class="modal-header">
        <h5 class="modal-title">Agreement</h5>
        <button type="button" class="close" data-dismiss="modal" aria-label="Close">
          <span aria-hidden="true">&times;</span>
        </button>
      </div>
      <div class="modal-body">
        <p><?php print $modal_body; ?></p>
      </div>
      <div class="modal-footer">
        <button type="button" class="btn btn-secondary" data-dismiss="modal">Close</button>
        <button type="button" class="btn btn-primary"  data-dismiss="modal" data-rid="<?php print $rid;?>">I agree to the terms</button>
      </div>
    </div>
  </div>
</div>