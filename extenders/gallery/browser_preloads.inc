<?php
/**
 * Gallery: browser function hooks
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 * 
 * Imported globals:
 * @var array          $per_column_function_hooks 2-dimensional: field keys, called function names
 * @var media_record[] $records
 */

use hng2_media\media_record;
use hng2_modules\moderation\moderation_messages_repository;

global $per_column_function_hooks, $records, $config, $account;
if( $account->level < $config::MODERATOR_USER_LEVEL ) return;

$ids = array();
foreach($records as $row) $ids[] = $row->id_media;

$repository = new moderation_messages_repository();
$config->globals["modules:moderation.messages_recordset"] = $repository->get_last_for_parents("media", $ids);
# echo "<pre>" . print_r($config->globals["modules:moderation.messages_recordset"], true) . "</pre>";
