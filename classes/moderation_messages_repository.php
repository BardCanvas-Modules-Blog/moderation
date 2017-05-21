<?php
namespace hng2_modules\moderation;

use hng2_repository\abstract_repository;

class moderation_messages_repository extends abstract_repository
{
    protected $row_class       = 'hng2_modules\moderation\moderation_message_record';
    protected $table_name      = "moderation_messages";
    protected $key_column_name = "id_message";
    
    public function __construct()
    {
        parent::__construct();
        
        $this->check_create_tables();
    }
    
    private function check_create_tables()
    {
        global $settings, $database;
        
        if( $settings->get("modules:moderation.messages_table_created") == "true" ) return;
        
        $database->exec("
            create table if not exists moderation_messages (
                `id_message`  bigint unsigned not null default 0,
                `created`     datetime,
                `parent_type` varchar(16) not null default '',
                `parent_id`   bigint unsigned not null default 0,
                `id_owner`    bigint unsigned not null default 0,
                `message`     varchar(255) not null default '',
                
                primary key             ( id_message ),
                unique  index by_parent ( parent_type, parent_id ),
                index   by_owner        ( id_owner )
                
            ) engine=InnoDB default charset=utf8mb4 collate='utf8mb4_unicode_ci'
        ");
        
        $settings->set("modules:moderation.messages_table_created", "true");
    }
    
    /**
     * @param $id
     *
     * @return moderation_message_record|null
     */
    public function get($id)
    {
        return parent::get($id);
    }
    
    /**
     * @param array  $where
     * @param int    $limit
     * @param int    $offset
     * @param string $order
     *
     * @return moderation_message_record[]
     */
    public function find($where, $limit, $offset, $order)
    {
        return parent::find($where, $limit, $offset, $order);
    }
    
    /**
     * @param $parent_type
     * @param $parent_ids
     *
     * @return moderation_message_record[]
     */
    public function get_last_for_parents($parent_type, array $parent_ids)
    {
        global $database;
        
        $ids = array();
        foreach($parent_ids as $id) $ids[] = "'$id'";
        $ids = implode(", ", $ids);
        
        $res = $database->query("
            select * from {$this->table_name}
            where parent_type = '$parent_type'
            and   parent_id in ($ids)
            group by parent_id
            order by created desc
        ");
        
        if( $database->num_rows($res) == 0 ) return array();
        
        $return = array();
        while($row = $database->fetch_object($res))
            $return[$row->parent_id] = new moderation_message_record($row);
        
        return $return;
    }
    
    public function delete_for_parent($parent_type, $parent_id)
    {
        global $database;
        
        return $database->exec("
            delete from {$this->table_name}
            where parent_type = '$parent_type'
            and   parent_id   = '$parent_id'
        ");
    }
    
    /**
     * @param moderation_message_record $record
     *
     * @return int
     */
    public function save($record)
    {
        global $database;
        
        $this->validate_record($record);
        $record->set_new_id();
        $record->created = date("Y-m-d H:i:s");
        
        $obj = $record->get_for_database_insertion();
        $res = $database->exec("
            insert into {$this->table_name} (
                `id_message` ,
                `created`    ,
                `parent_type`,
                `parent_id`  ,
                `id_owner`   ,
                `message`    
            ) values (
                '{$obj->id_message }',
                '{$obj->created    }',
                '{$obj->parent_type}',
                '{$obj->parent_id  }',
                '{$obj->id_owner   }',
                '{$obj->message    }'
            ) on duplicate key update
                `created`     = '{$obj->created    }',
                `message`     = '{$obj->message    }'
        ");
        
        $this->last_query = $database->get_last_query();
        return $res;
    }
    
    /**
     * @param moderation_message_record $record
     *
     * @throws \Exception
     */
    public function validate_record($record)
    {
        if( ! $record instanceof moderation_message_record )
            throw new \Exception(
                "Invalid object class! Expected: {$this->row_class}, received: " . get_class($record)
            );
    }
}
