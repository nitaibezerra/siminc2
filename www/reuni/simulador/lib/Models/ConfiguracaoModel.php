<?php
/*
 * Created on 03/09/2007 by MOC
 *
 */

class ConfiguracaoModel extends Model{

    public function __construct($dao) {
    	parent::__construct($dao);
        $this->dao = $dao;
    }

}

?>
