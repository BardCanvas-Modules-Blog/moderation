<?php
namespace hng2_modules\moderation;

class toolbox
{
    /**
     * @param string $key Name of list to find in without the "modules:xxx." prefix
     * 
     * @return array of PCRE patterns to use in searches
     */
    public function get_words_list($key)
    {
        global $object_cache, $settings;
        
        if( $object_cache->exists("modules:moderation", $key) ) return $object_cache->get("modules:moderation", $key);
        
        $val = trim($settings->get("modules:moderation.$key"));
        if( empty($val) ) return array();
        
        $return = array();
        foreach(explode("\n", $val) as $entry)
        {
            $entry = trim($entry);
            if( empty($entry) ) continue;
            if( substr($entry, 0, 1) == "#" ) continue;
            
            if( stristr($entry, "|") !== false )
            {
                list($entry, $start_date, $ttl) = preg_split('/\s+\|\s+/', $entry);
                $max_date = date("Y-m-d 23:59:59", strtotime("$start_date + $ttl days"));
                if( date("Y-m-d H:i:s") > $max_date ) continue;
            }
            
            $return[] = "/\\b(" . str_replace(array("?", "*", "/"), array(".?", ".*", "\\/"), $entry) . ")\\b/i";
        }
        
        $object_cache->set("modules:moderation", $key, $return);
        return $return;
    }
    
    /**
     * @param string $input    Contents to search in
     * @param string $list_key Keyname of the list
     *
     * @return array of words found
     */
    public function probe_in_words_list($input, $list_key)
    {
        $patterns    = $this->get_words_list($list_key);
        $detected    = array();
        
        if( empty($patterns) ) return array();
        
        foreach($patterns as $pattern)
        {
            if( ! preg_match($pattern, $input, $matches) ) continue;
        
            $detected[] = $matches[1];
        }
        
        $detected = array_unique($detected);
        return $detected;
    }
}
