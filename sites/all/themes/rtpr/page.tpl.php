<?php
//kpr(get_defined_vars());
//kpr($theme_hook_suggestions);
//template naming
//page--[CONTENT TYPE].tpl.php
?>

<div class="page">
<?php if( theme_get_setting('mothership_poorthemers_helper') ){ ?>
<!-- page.tpl.php-->
<?php } ?>

<?php print $mothership_poorthemers_helper; ?>

<header role="banner">


  <?php if($page['header_top']): ?>
    <div class="header-top">
    <div class="inner clearfix">
      <?php print render($page['header_top']); ?>
    </div>
    </div>
  <?php endif; ?>

  <?php if($page['header']): ?>
    <div class="header-center">
    <div class="inner clearfix">
      <?php print render($page['header']); ?>
    </div>
    </div>
  <?php endif; ?>



    <?php if($page['header_bottom']): ?>
    <div class="header-bottom">
      <div class="inner clearfix">
      <?php print render($page['header_bottom']); ?>
    </div>
    </div>
  <?php endif; ?>

</header>


  <?php if ($page['splash']): ?>
    <div class="splash">
      <?php print render($page['splash']); ?>
    </div>
  <?php endif; ?>





<div class="content-container clearfix">

 <?php if ($page['container_top']): ?>
    <div class="container-top">
      <?php print render($page['container_top']); ?>
    </div>
  <?php endif; ?>

  <?php if ($page['sidebar_first']): ?>
    <div class="sidebar-first">
     <div class="inner">
    <?php print render($page['sidebar_first']); ?>
     </div>    
    </div>
  <?php endif; ?>

<div role="main" id="main-content" class="main-content">

    <?php print render($title_prefix); ?>
    <?php if ($title): ?>
      <h1><?php print $title; ?></h1>
    <?php endif; ?>
    <?php print render($title_suffix); ?>


    <?php if ($action_links): ?>
      <ul class="action-links"><?php print render($action_links); ?></ul>
    <?php endif; ?>

    <?php if($page['highlighted'] OR $messages){ ?>
      <div class="drupal-messages">
      <?php print render($page['highlighted']); ?>
      <?php print $messages; ?>
      </div>
    <?php } ?>

    <?php print render($page['content_top']); ?>
    <?php print render($page['content_pre']); ?>
    <?php print render($page['content']); ?>
    <?php print render($page['content_post']); ?>

    <?php print render($page['content_bottom']); ?>


  </div><!-- /main-->



  <?php if ($page['sidebar_second']): ?>
    <div class="sidebar-second">
      <?php print render($page['sidebar_second']); ?>
    </div>
  <?php endif; ?>


</div><!-- /page-->
</div>
<footer role="contentinfo" class="clearfix">
  <?php print render($page['footer']); ?>
</footer>

