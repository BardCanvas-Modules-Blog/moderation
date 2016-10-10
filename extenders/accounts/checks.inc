<?php
/**
 * Accounts extender: registration/edition checks
 * In both cases, this extension is called after initializing the account being added/edited,
 * which is set on $config->globals["accounts:processing_account"]
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Imported globals:
 * @var array $errors
 * 
 * Trailing vars:
 * @var module $this_module    self (moderation)
 * @var module $current_module posts
 */

use hng2_base\account;
use hng2_base\config;
use hng2_base\module;
use hng2_modules\moderation\toolbox;

global $template, $language, $settings, $account, $config, $errors;

if( $account->level >= config::MODERATOR_USER_LEVEL ) return;

/** @var account $editing_account */
$editing_account = $config->globals["accounts:processing_account"];

$toolbox = new toolbox();

$input    = $editing_account->user_name;
$detected = $toolbox->probe_in_words_list($input, "usernames_blacklist");
if( ! empty($detected) )
    $errors[] = unindent(replace_escaped_vars(
        $this_module->language->messages->entries_in_usernames_blacklist_found,
        '{$detected_words_list}',
        implode(", ", $detected)
    ));

$input    = $editing_account->display_name;
$detected = $toolbox->probe_in_words_list($input, "displaynames_blacklist");
if( ! empty($detected) )
    $errors[] = unindent(replace_escaped_vars(
        $this_module->language->messages->entries_in_displaynames_blacklist_found,
        '{$detected_words_list}',
        implode(", ", $detected)
    ));
