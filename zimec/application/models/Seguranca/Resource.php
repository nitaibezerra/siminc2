<?php
class Model_Seguranca_Resource extends Simec_Db_Table
{
    protected $_primary = 'rscid';
	protected $_schema  = 'seguranca';
    protected $_name    = 'resource';

    public function getCamposValidacao($dados = array())
    {
        return array(
            'rscid' => array('allowEmpty' => true, 'Digits'),
            'rscdsc' => array(new Zend_Validate_StringLength(array('max' => 200))),
            'rscmoludo' => array(new Zend_Validate_StringLength(array('max' => 200))),
            'rsccontroller' => array(new Zend_Validate_StringLength(array('max' => 200))),
            'rscaction' => array(new Zend_Validate_StringLength(array('max' => 200))),
           // 'rsctipo' => array(new Zend_Validate_StringLength(array('max' => 100))),
        );
    }
}