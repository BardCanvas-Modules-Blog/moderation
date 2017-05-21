<?php
/**
 * Comments extender: toolbox / after publishing
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var module $current_module comments
 * @var module $this_module    self
 * 
 * Globals:
 * @var comment_record $comment
 */

use hng2_base\module;
use hng2_modules\comments\comment_record;
use hng2_modules\moderation\moderation_messages_repository;

global $comment, $repository, $account;

$messages_repository = new moderation_messages_repository();
$messages_repository->delete_for_parent("comment", $comment->id_comment);
