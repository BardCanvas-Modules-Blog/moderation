<?php
/**
 * Comments empty trash extender
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 */

global $database;

$boundary = date("Y-m-d 00:00:00", strtotime("today - 7 days"));

$database->exec("
    delete from moderation_messages
    where parent_type = 'comment'
    and parent_id in (
      select id_comment from comments where status = 'trashed'
      and creation_date < '$boundary'
    )
");
