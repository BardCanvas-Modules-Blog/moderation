<?php
/**
 * Gallery extender: browser principal data addon.
 * Grabs from the preloaded collection set on browser_preloads.inc
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * Imported globals:
 * @var array        $principal_data_addons
 * @var media_record $row
 * 
 * Insiders:
 * @var moderation_message_record[] $messages
 */

use hng2_media\media_record;
use hng2_modules\moderation\moderation_message_record;

global $principal_data_addons, $config, $row, $account;
if( $account->level < $config::MODERATOR_USER_LEVEL ) return;

$messages = $config->globals["modules:moderation.messages_recordset"];
if( isset($messages[$row->id_media]) )
    $principal_data_addons[] = "
        <div class='framed_content state_ko' style='margin: 2px 0;'>
            <i class='fa fa-warning'></i>
            {$messages[$row->id_media]->message}
        </div>
    ";
