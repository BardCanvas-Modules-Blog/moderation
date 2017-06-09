<?php
/**
 * Comments empty trash extender
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

use hng2_modules\moderation\moderation_messages_repository;

global $database;

$boundary = date("Y-m-d 00:00:00", strtotime("today - 7 days"));

new moderation_messages_repository();
$database->exec("
    delete from moderation_messages
    where parent_type = 'comment'
    and parent_id in (
      select id_comment from comments where status = 'trashed'
      and creation_date < '$boundary'
    )
");
