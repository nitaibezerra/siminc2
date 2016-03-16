<?php

include_once APPLICATION_PATH . '/../library/Simec/legacy/Listagem.php';

import('models.demandasequipe.DemandaAnotacao');

class DemandasEquipe_DemandaAnotacaoController extends Simec_Controller_MultiCadastro
{
    protected $campoPai = 'dmdid';
    protected $modelName = 'Model_DemandasEquipe_DemandaAnotacao';
}