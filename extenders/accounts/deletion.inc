<?php
/**
 * Accounts deletion extender
 *
 * @package    BardCanvas
 * @subpackage moderation
 * @author     Alejandro Caballero - lava.caballero@gmail.com
 *
 * Globals:
 * @var module $current_module accounts
 * @var module $this_module    self
 */

use hng2_base\account;
use hng2_base\module;

global $database, $account;

$id_account = $config->globals["deleting_id_account"];
$user       = new account($id_account);
if( empty($id_account) ) return;

if( empty($config->globals["deletions_log"]) ) $config->globals["deletions_log"] = array();
$config->globals["deletions_log"][] = "<b>Starting Moderation extender for account deletion</b>";

#
# Delete all messages where the user is present
#
$count = $database->exec("delete from moderation_messages where id_owner = '$id_account'");
if( $count > 0 ) $config->globals["deletions_log"][] = "$count <b>Moderation messages</b> deleted.";

$config->globals["deletions_log"][] = "<b>Finished Moderation extender for account deletion</b>";
if( $config->globals["notify_deletion_progress"] )
{
    send_notification(
        $account->id_account, "information", "Finished Moderation extender for account deletion"
    );
    sleep(1);
}
