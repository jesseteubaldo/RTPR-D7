<?php
//kpr(get_defined_vars());
 ?>
<article <?php if($classes){ ?>class="<?php print $classes;?>"<?php } ?> role="article">
  <?php
    if (isset($title_suffix['contextual_links'])){
       print render($title_suffix['contextual_links']);
   }
  ?>

  <?php if ($top): ?>
    <?php if($top_classes){ ?><div class="<?php print $top_classes; ?>"><?php } ?>
      <?php print $top; ?>
    <?php if($top_classes){ ?></div><?php } ?>
  <?php endif; ?>

  <div class="content">
    <?php if ($left): ?>
      <div <?php if($left_classes){ ?>class="<?php print $left_classes; ?>"<?php } ?>>
        <?php print $left; ?>
      </div>
    <?php endif; ?>

    <?php if ($right): ?>
      <div <?php if($right_classes){ ?>class="<?php print $right_classes; ?>"<?php } ?>>
        <?php print $right; ?>
      </div>
    <?php endif; ?>
  </div>

  <?php if ($bottom): ?>
    <div <?php if($bottom_classes){ ?>class="<?php print $bottom_classes; ?>"<?php } ?>>
      <?php print $bottom; ?>
    </div>
  <?php endif; ?>
</article>




