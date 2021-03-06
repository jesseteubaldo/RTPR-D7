<?php
/**
* @file
* Default theme implementation to display a node.
*
* Available variables:
* - $title: the (sanitized) title of the node.
* - $content: An array of node items. Use render($content) to print them all,
* or print a subset such as render($content['field_example']). Use
* hide($content['field_example']) to temporarily suppress the printing of a
* given element.
* - $user_picture: The node author's picture from user-picture.tpl.php.
* - $date: Formatted creation date. Preprocess functions can reformat it by
* calling format_date() with the desired parameters on the $created variable.
* - $name: Themed username of node author output from theme_username().
* - $node_url: Direct URL of the current node.
* - $display_submitted: Whether submission information should be displayed.
* - $submitted: Submission information created from $name and $date during
* template_preprocess_node().
* - $classes: String of classes that can be used to style contextually through
* CSS. It can be manipulated through the variable $classes_array from
* preprocess functions. The default values can be one or more of the
* following:
* - node: The current template type; for example, "theming hook".
* - node-[type]: The current node type. For example, if the node is a
* "Blog entry" it would result in "node-blog". Note that the machine
* name will often be in a short form of the human readable label.
* - node-teaser: Nodes in teaser form.
* - node-preview: Nodes in preview mode.
* The following are controlled through the node publishing options.
* - node-promoted: Nodes promoted to the front page.
* - node-sticky: Nodes ordered above other non-sticky nodes in teaser
* listings.
* - node-unpublished: Unpublished nodes visible only to administrators.
* - $title_prefix (array): An array containing additional output populated by
* modules, intended to be displayed in front of the main title tag that
* appears in the template.
* - $title_suffix (array): An array containing additional output populated by
* modules, intended to be displayed after the main title tag that appears in
* the template.
*
* Other variables:
* - $node: Full node object. Contains data that may not be safe.
* - $type: Node type; for example, story, page, blog, etc.
* - $comment_count: Number of comments attached to the node.
* - $uid: User ID of the node author.
* - $created: Time the node was published formatted in Unix timestamp.
* - $classes_array: Array of html class attribute values. It is flattened
* into a string within the variable $classes.
* - $zebra: Outputs either "even" or "odd". Useful for zebra striping in
* teaser listings.
* - $id: Position of the node. Increments each time it's output.
*
* Node status variables:
* - $view_mode: View mode; for example, "full", "teaser".
* - $teaser: Flag for the teaser state (shortcut for $view_mode == 'teaser').
* - $page: Flag for the full page state.
* - $promote: Flag for front page promotion state.
* - $sticky: Flags for sticky post setting.
* - $status: Flag for published status.
* - $comment: State of comment settings for the node.
* - $readmore: Flags true if the teaser content of the node cannot hold the
* main body content.
* - $is_front: Flags true when presented in the front page.
* - $logged_in: Flags true when the current user is a logged-in member.
* - $is_admin: Flags true when the current user is an administrator.
*
* Field variables: for each field instance attached to the node a corresponding
* variable is defined; for example, $node->body becomes $body. When needing to
* access a field's raw values, developers/themers are strongly encouraged to
* use these variables. Otherwise they will have to explicitly specify the
* desired field language; for example, $node->body['en'], thus overriding any
* language negotiation rule that was previously applied.
*
* @see template_preprocess()
* @see template_preprocess_node()
* @see template_process()
*
* @ingroup themeable
*/
?>
<article id="node-<?php print $node->nid; ?>" class="post grid <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
  <?php print render($title_prefix); ?>
  <?php print render($title_suffix); ?>

    <!-- Begin media blog -->
    <div class="media-blog">
      <?php print render($content['field_media']);?>
    </div> 
    <!-- End media blog -->

  <section class="image-caption">
    <a class="title" href="<?php print $node_url;?>"><?php print $title;?></a>
    <div class="meta">
      <i class="fa fa-folder-open-o"></i>
      <?php if (isset($blog_categories)) print $blog_categories;?>
    </div>
    <?php $hit = 0;
      if (isset($content['links']['statistics'])) {
        $hit = $content['links']['statistics']['#links']['statistics_counter']['title'];
      }
      $hover_hit = "Total ".$hit;
    ?>
    <div class="views">
      <a class="dtooltip" data-placement="bottom" data-toggle="tooltip" data-original-title="<?php print $hover_hit;?>" title="" href="#">
        <i class="fa fa-eye"></i>
      </a>
    </div>
    <div class="read-more">
      <a class="dtooltip" data-placement="bottom" data-toggle="tooltip" data-original-title="Read this post" href="<?php print $node_url;?>">
        <i class="fa fa-external-link"></i>
      </a>
    </div>
    <div class="post-type">
      <?php 
        $icon = "fa-picture-o";
        $post_type = "gallery";
        $language = $node->language;
        if (isset($node->field_type)) {
          if (isset($node->field_type[$language])) {
            $icon = $node->field_type[$node->language][0]['value'];
          } else {
            $icon = $node->field_type["und"][0]['value'];
          }
        }
        switch ($icon) {
          case "fa-pencil":
            $post_type = "text";
            break;
          case "fa-video-camera":
            $post_type = "video";
            break;
          case "fa-music":
            $post_type = "music";
            break;
          case "fa-quote-left":
            $post_type = "quote";
            break;
          default:
            break;
        }
      ?>
      <a class="dtooltip" data-placement="bottom" data-toggle="tooltip" data-original-title="Post under <?php print $post_type; ?> post type" href="#">
        <i class="fa <?php print $icon;?>"></i>
      </a>
    </div>
  </section>

 </article> 