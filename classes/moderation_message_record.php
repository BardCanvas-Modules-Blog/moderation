<?php
namespace hng2_modules\moderation;

use hng2_repository\abstract_record;

class moderation_message_record extends abstract_record
{
    public $id_message;
    public $created;
    
    /**
     * @var string post|media|comment
     */
    public $parent_type;
    
    public $parent_id;
    public $id_owner;
    public $message;
    
    public function set_new_id()
    {
        $this->id_message = make_unique_id("");
    }
}
