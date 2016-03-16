<?php
global $prfref;

$formGd = new Simec_View_Form('grupodespesa');
$formGd->addHidden('prfid', $prfref->prfid, 'prfid')
    ->addHidden('gdpid', null, 'gdpid')
    ->addInputTexto('gdpnome', null, 'gdpnome', 255, false, array('flabel' => 'Nome'))
    ->addInputTexto('gdpordem', null, 'gdpordem', 3, false, array('flabel' => 'Ordem', 'masc' => '###'))
    ->addInputTextarea('gdpobservacao', null, 'gdpobservacao', 300, array('flabel' => 'Observações'))
    ->setRequisicao('salvarGrupodespesa')
    ->render();
