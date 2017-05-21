<?php
/**
 * Posts extender: browser function hooks
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * Imported globals:
 * @var array         $per_column_function_hooks 2-dimensional: field keys, called function names
 * @var post_record[] $records
 */

use hng2_modules\posts\post_record;
use hng2_modules\moderation\moderation_message_record;
use hng2_modules\moderation\moderation_messages_repository;

global $per_column_function_hooks, $config, $records, $account;
if( $account->level < $config::MODERATOR_USER_LEVEL ) return;

# Data preload
$ids = array();
foreach($records as $row) $ids[] = $row->id_post;
$repository = new moderation_messages_repository();
$config->globals["modules:moderation.messages_recordset"] = $repository->get_last_for_parents("post", $ids);

# Function defs
$per_column_function_hooks["details"][] = "render_moderation_messages_for_post";

/**
 * @param post_record $row
 */
function render_moderation_messages_for_post($row)
{
    global $config;
    
    /** @var moderation_message_record[] $messages */
    $messages = $config->globals["modules:moderation.messages_recordset"];
    
    if( isset($messages[$row->id_post]) )
        echo "
        <hr><div class='framed_content state_ko' style='margin: 2px 0;'>
            <i class='fa fa-warning'></i>
            {$messages[$row->id_post]->message}
        </div>
    ";
}
