<?php
/**
 * Posts extender: after-saving moderation checks
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Imported globals:
 * @var posts_repository $repository
 * @var post_record $post
 *
 * Trailing vars:
 * @var module $this_module    self (moderation)
 * @var module $current_module posts
 */

use hng2_base\config;
use hng2_base\module;
use hng2_modules\moderation\moderation_message_record;
use hng2_modules\moderation\moderation_messages_repository;
use hng2_modules\posts\post_record;
use hng2_modules\posts\posts_repository;
use hng2_modules\moderation\toolbox;

global $template, $language, $settings, $account, $post, $repository;

$messages_repository = new moderation_messages_repository();
if( $account->level >= config::MODERATOR_USER_LEVEL )
{
    $messages_repository->delete_for_parent("post", $post->id_post);
    
    return;
}
if( $post->status != "published" ) return;

$toolbox = new toolbox();
$input   = implode("\t", array(
    $post->title,
    $post->content,
    $post->excerpt,
));

# Greylist: Submission accepted but kept for review and notification sent to mods/admins
$detected = $toolbox->probe_in_words_list($input, "words_greylist");
if( ! empty($detected) )
{
    $messages_repository->save(new moderation_message_record(array(
        "parent_type" => "post",
        "parent_id"   => $post->id_post,
        "id_owner"    => $post->id_author,
        "message"     => replace_escaped_objects(
            $this_module->language->messages->greylist_for_listings,
            array('{$detected_words_list}' => implode("</code>, <code>", $detected))
        )
    )));
    
    $contents = $this_module->language->messages->entries_in_greylist_found;
    send_notification( $account->id_account, "warning", $contents );
    
    $contents = replace_escaped_vars(
        $this_module->language->messages->greylist_notifications_to_mods->for_posts,
        array('{$user}', '{$post_title}', '{$detected_words_list}', '{$posts_mod_url}'),
        array(
            $account->display_name,
            $post->get_processed_title(false),
            implode("</code>, <code>", $detected),
            "{$config->full_root_url}/posts/?search_status=reviewing"
        )
    );
    broadcast_to_moderators("warning", $contents);
    
    $repository->change_status($post->id_post, "reviewing");
    $post->status = "reviewing";
    
    return;
}
