<?php
/**
 * Posts extender: toolbox / after publishing
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Trailing vars:
 * @var module $current_module posts
 * @var module $this_module    self
 * 
 * Globals:
 * @var post_record $post
 */

use hng2_base\module;
use hng2_modules\moderation\moderation_messages_repository;
use hng2_modules\posts\post_record;

global $post, $repository, $account;

$messages_repository = new moderation_messages_repository();
$messages_repository->delete_for_parent("post", $post->id_post);
