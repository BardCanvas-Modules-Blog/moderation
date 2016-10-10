<?php
/**
 * Comments extender: after-saving moderation checks (when adding comments)
 * 
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * Imported globals:
 * @var comments_repository $repository
 * @var comment_record      $comment
 * @var post_record         $post
 * 
 * Trailing vars:
 * @var module $this_module    self (moderation)
 * @var module $current_module comments
 */

use hng2_base\config;
use hng2_base\module;
use hng2_modules\comments\comment_record;
use hng2_modules\comments\comments_repository;
use hng2_modules\posts\post_record;
use hng2_modules\moderation\toolbox;

global $template, $language, $settings, $account, $comment, $post, $repository;

if( $account->level >= config::MODERATOR_USER_LEVEL ) return;

$toolbox = new toolbox();
$input   = implode("\t", array(
    $comment->author_display_name,
    $comment->author_url,
    $comment->content
));

# Greylist: Submission accepted but kept for review and notification sent to mods/admins
$detected = $toolbox->probe_in_words_list($input, "words_greylist");
if( ! empty($detected) )
{
    if( $account->_exists )
    {
        $contents = $this_module->language->messages->entries_in_greylist_found;
        send_notification( $account->id_account, "warning", $contents );
    }
    
    $contents = replace_escaped_vars(
        $this_module->language->messages->greylist_notifications_to_mods->for_comments,
        array('{$user}', '{$post_title}', '{$author}', '{$detected_words_list}', '{$comments_mod_url}'),
        array(
            $account->_exists ? $account->display_name : $comment->author_display_name,
            $post->get_processed_title(false),
            $post->get_processed_author_display_name(),
            implode("</code>, <code>", $detected),
            "{$config->full_root_url}/comments/?search_status=reviewing"
        )
    );
    broadcast_to_moderators("warning", $contents);
    
    $repository->change_status($comment->id_comment, "reviewing");
    $comment->status = "reviewing";
    
    return;
}
