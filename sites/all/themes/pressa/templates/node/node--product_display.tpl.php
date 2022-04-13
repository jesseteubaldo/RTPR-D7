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

<?php $custom_class = "selected"; ?>
<?php if(count($content['field_product']['#items']) > 1): ?>
<?php $custom_class = "not-selected"; ?>
<?php endif; ?>

<div id="node-<?php print $node->nid; ?>" class="shop-single <?php print $classes; ?> clearfix"<?php print $attributes; ?>>
    <div class='row'>
        
        <div class="shop-left col-lg-4 col-md-4 col-sm-12 not-selected">
            
            <?php print render($content['field_product_featured']);?>
  
            <?php print render($content['product:field_images']);?>
        </div>
        
        <div class="shop-right col-lg-8 col-md-8 col-sm-12">
            <div class='title widget-title'>
                <h3><?php print ($node->title);?></h3>    
                <hr/>
            </div>
          <?php print render($content['body']);?>
          
            <?php if(!isset($range['same']) && isset($range['min'])) : ?>
              <div class="price-range notranslate">
              <?php 
                print '<h3>Price Range</h3>';
                print '<div class="product-price">$' . $range['min'] . ' - $' . $range['max'] ."</div>";
                print '<p>Make your selection below</p>';
              ?>
            </div>
            <?php else: ?>
              <div class="price-range same notranslate <?php echo $custom_class;?>"><div class="product-price">$<?php echo $range['min']; ?></div></div>
            <?php endif;?>
    
            <div class="product-price item-price <?php echo $custom_class; ?>">
               <?php print render($content['product:commerce_price']);?>
            </div>
            <div class="add-to-wishlist">
            <?php 
            if (isset($node->field_product[$node->language])) {
              $product = $node->field_product[$node->language][0]['product_id'];
            } else {
              $product = $node->field_product['und'][0]['product_id'];
            }
            print flag_create_link('wishlist', $product);
            ?>
            </div>
            <div class="clearfix"></div>
            <div class="product-add-to-cart-form <?php echo $custom_class; ?>">
            <?php print render($content['field_product']);?>
            </div>
            <div class="clearfix"></div>
            
       </div>
    </div>

    <hr>
    <div class="product-details-wrap dexp_tab_wrapper">
        <ul class="nav nav-tabs" id="myTab">
            <?php if (!empty($variables['reward_title'])): ?>
            <li class="active">
                <a data-toggle="tab" class="fa fa-credit-card" href="#Retail"> <?php print $variables['reward_title']; ?></a>
            </li>
            <?php endif; ?>
            <li class=""><a data-toggle="tab" href="#Description">Description</a></li>
            <li class="">
                <a data-toggle="tab" href="#Reviews">Reviews<?php print $testimonial_count;?></a>
            </li>
  
        </ul>

        <div class="tab-content" id="myTabContent">
          <?php $active = "active in"; ?>
          <?php if (!empty($variables['reward_title'])): ?>
            <div id="Retail" class="tab-pane fade active in">
                <?php $active = ""; ?>
                <?php if ($variables['reward_retails']): ?>
                  <h3 class="bold">Base Rewards Points Value for this product(s): <?php print $variables['reward_points']; ?></h4>
                  <p><img src="/sites/all/themes/pressa/templates/images/RT-WRP-150.jpg" style="float: right; padding-left: 10px;"/>Earn 5 Rewards Points for every $1 you spend at RTPR.com. This item also counts towards earning MATCHING or even DOUBLE Matching Rewards Points! The Rewards Points never expire and are like money in the bank- redeemable on future purchases. Thank you for being a member of the RTPR Rewards Program! If you want more details on your Rewards Account, <a href="/rewards-program" target="_blank">Click Here</a></p>
                <?php elseif ($variables['non_reward_retails']): ?>
                  <h4 class="bold">Base Rewards Points Value for this product(s): <?php print $variables['reward_points']; ?></h4>
                  <p><img src="/sites/all/themes/pressa/templates/images/RT-WRP-150.jpg" style="float: right; padding-left: 10px;"/>Earn 5 Rewards Points for every $1 you spend at RTPR.com. This item also counts towards earning MATCHING or even DOUBLE Matching Rewards Points! The Rewards Points never expire and are like money in the bank- redeemable on future purchases. There are many benefits of being a member of the RTPR Rewards Program, and the best part is that it is completely FREE to join! As a Rewards Member, you have access to these additional perks: <li>Free Samples</li> <li>Sneak Peaks of new products</li> <li>Member Only Sales</li> <li>And more</li> If you are not enrolled already, check the box below to enroll into the Rewards Program and checkout and start earning rewards on this order!</p><p> <label for="enroll">Enroll Me For Rewards Program! <input type="checkbox" onchange="enrollRP(this)"></label> </p><p>If you want more details on this FREE program, <a href="/rewards-program" target="_blank">Click Here</a></p>
                <?php endif; ?>
                <div class="clearfix"></div>
            </div><!-- end tab pane Retail Program -->
          <?php endif; ?>
            <div id="Description" class="tab-pane fade <?php print $active;?>">
                <?php print $content['body']['#items'][0]['value'];?>                                
            </div><!-- col-tab pane -->

            <div id="Reviews" class="tab-pane fade">
              <?php print $testimonial; ?>
              <p>For more reviews, <a href="https://rtpr.com/testimonials" target="_blank">Click Here</a></p>
                <div class="clearfix"></div>
            </div><!-- end tab pane review -->
        </div><!-- end  tab content -->
    </div>

</div>
<hr>