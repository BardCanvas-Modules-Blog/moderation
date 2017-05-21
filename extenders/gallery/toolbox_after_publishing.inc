<?php
/**
 * Gallery extender: toolbox / after publishing
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var module $current_module gallery
 * @var module $this_module    self
 * 
 * Globals:
 * @var media_record $item
 */

use hng2_base\module;
use hng2_media\media_record;
use hng2_modules\moderation\moderation_messages_repository;

global $item, $repository, $account;

$messages_repository = new moderation_messages_repository();
$messages_repository->delete_for_parent("media", $item->id_media);
