<?php
class Model_DemandasEquipe_DemandaAnotacao extends Simec_Db_Table
{
    protected $_schema = 'demandasequipe';
    protected $_name   = 'demandaanotacao';

    public function getCamposValidacao($dados = array())
    {
        return array();
    }
}
