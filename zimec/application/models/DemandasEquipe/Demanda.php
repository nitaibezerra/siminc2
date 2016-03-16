<?php
class Model_DemandasEquipe_Demanda extends Simec_Db_Table
{
    protected $_schema = 'demandasequipe';
    protected $_name   = 'demanda';

    public function getCamposValidacao($dados = array())
    {
        $exclude = empty($dados['dmdid']) ? null : array('field' => 'dmdid', 'value' => $dados['dmdid']);
        return array(
            'dmdid' => array('allowEmpty' => true, 'Digits'),
//            'dmdtitulo' => array(new Zend_Validate_StringLength(array('max' => 50)), new Zend_Validate_Db_NoRecordExists(array(
//                'table'  => 'demandasequipe.demanda',
//                'field'  => 'dmdtitulo',
//                'exclude' => $exclude
//            ))),
        );
    }
}
