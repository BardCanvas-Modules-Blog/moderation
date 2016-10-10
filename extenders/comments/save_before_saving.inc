<?php
/**
 * Comments extender: before-saving moderation checks (when editing comments)
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Imported globals:
 * @var comment_record $comment
 * @var post_record    $post
 *
 * Trailing vars:
 * @var module $this_module    self (moderation)
 * @var module $current_module comments
 */

use hng2_base\config;
use hng2_base\module;
use hng2_modules\comments\comment_record;
use hng2_modules\posts\post_record;
use hng2_modules\moderation\toolbox;

global $template, $language, $settings, $account, $comment, $post;

if( $account->level >= config::MODERATOR_USER_LEVEL ) return;

$toolbox = new toolbox();
$input   = implode("\t", array(
    $comment->author_display_name,
    $comment->author_url,
    $comment->content
));

# Blacklist: rejected on hit.
$detected = $toolbox->probe_in_words_list($input, "words_blacklist");
if( ! empty($detected) )
    die(unindent(replace_escaped_vars(
        $this_module->language->messages->entries_in_blacklist_found,
        '{$detected_words_list}',
        implode(", ", $detected)
    )));

# Greylist: rejected on hit.
$detected = $toolbox->probe_in_words_list($input, "words_greylist");
if( ! empty($detected) && $comment->status != "reviewing" )
    die(unindent(replace_escaped_vars(
        $this_module->language->messages->greylist_hit_warning_on_editing,
        '{$detected_words_list}',
        implode(", ", $detected)
    )));
