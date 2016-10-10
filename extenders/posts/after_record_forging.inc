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
use hng2_modules\posts\post_record;
use hng2_modules\posts\posts_repository;
use hng2_modules\moderation\toolbox;

global $template, $language, $settings, $account, $post, $repository;

if( $account->level >= config::MODERATOR_USER_LEVEL ) return;

$toolbox = new toolbox();
$input   = implode("\t", array(
    $post->title,
    $post->content,
    $post->excerpt,
));

# Blacklist: rejected
$detected = $toolbox->probe_in_words_list($input, "words_blacklist");
if( ! empty($detected) )
    die(unindent(replace_escaped_vars(
        $this_module->language->messages->entries_in_blacklist_found,
        '{$detected_words_list}',
        implode(", ", $detected)
    )));
