<?php
/**
 * Gallery extender: after-saving moderation checks (when adding/saving media items)
 * Data comes in $_POST
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Imported globals:
 * @var media_record     $item
 * @var media_repository $repository
 *
 * Trailing vars:
 * @var module $this_module    self (moderation)
 * @var module $current_module gallery
 */

use hng2_base\config;
use hng2_base\module;
use hng2_media\media_record;
use hng2_media\media_repository;
use hng2_modules\moderation\toolbox;

global $template, $language, $settings, $account, $item, $repository;

if( $account->level >= config::MODERATOR_USER_LEVEL ) return;

$toolbox = new toolbox();
$input   = implode("\t", array(
    $item->title,
    $item->description
));

# Blacklist: rejected
$detected = $toolbox->probe_in_words_list($input, "words_blacklist");
if( ! empty($detected) )
    die(unindent(replace_escaped_vars(
        $this_module->language->messages->entries_in_blacklist_found,
        '{$detected_words_list}',
        implode(", ", $detected)
    )));

# Greylist: Submission accepted, but item flagged for review,
# thus, not visible on indexes
$detected = $toolbox->probe_in_words_list($input, "words_greylist");
if( ! empty($detected) )
{
    $contents = $this_module->language->messages->entries_in_greylist_found_for_media;
    send_notification( $account->id_account, "warning", $contents );
    
    $contents = replace_escaped_vars(
        $this_module->language->messages->greylist_notifications_to_mods->for_media,
        array('{$user}', '{$title}', '{$detected_words_list}', '{$gallery_browser_url}'),
        array(
            $account->display_name,
            trim(stripslashes($_POST["title"])),
            implode("</code>, <code>", $detected),
            "{$config->full_root_url}/gallery/?search_status=reviewing"
        )
    );
    broadcast_to_moderators("warning", $contents);
    
    $repository->change_status($item->id_media, "reviewing");
    
    return;
}
