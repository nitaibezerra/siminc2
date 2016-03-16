<?php

$areas = array(
    array('codigo' => PFL_PROCURADOR_FEDERAL, 'descricao' => 'Procuradoria Federal'),
    array('codigo' => PFL_NUCLEO_JURIDICO, 'descricao' => 'DIGEF'),
    array('codigo' => PFL_DTI_MEC, 'descricao' => 'DTI/MEC'),
    array('codigo' => PFL_4_NIVEL, 'descricao' => '4º Nível'),
    array('codigo' => PFL_GESTOR_FIES, 'descricao' => 'Gestor FIES'),
);

$areas = getAreas();

if( $_POST['action'] == 'pesquisar' ){
    $modelDemanda->popularDadosObjeto();
}

if ($_POST['destalharDemandas'] == 1) :
    $sql = $modelDemanda->getSqlListaRelatorioPorCampo($_POST['cpf'], $_POST['campo']);
    $dados = $modelDemanda->carregar($sql);
    $dados = $dados ? $dados : array();
    ?>
<div class="row">
    <div class="col-lg-12">
        <?= $modelDemanda->montarTabelaListagem($dados); ?>
    </div>
</div>
<?php exit; endif;  ?>
