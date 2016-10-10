<script>
    $(document).ready(function(){
        $('#filtro').on('click',function(){
            var unicod = $('#unicod').val();
            if(unicod != undefined && unicod != '' && unicod != null){
                //window.location = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A&unicod='+unicod;
            }
            bootbox.alert('Selecione uma Unidade Orçamentária.');
        });
    });
</script>
<?php
$form = new Simec_View_Form('formPrelimite', Simec_View_Form::GET);
$form->setRequisicao('filtro');
$whereUO = '';
if (in_array(PFL_UO_EQUIPE_TECNICA, pegaPerfilGeral($_SESSION['usucpf']))) {

    $where = <<<DML
        AND EXISTS (SELECT 1
            FROM proporc.usuarioresponsabilidade rpu
            WHERE rpu.usucpf = '%s'
            AND rpu.pflcod = '%s'
            AND rpu.unicod = uni.unicod
            AND rpu.rpustatus = 'A')
DML;
    $whereUO = sprintf($where, $_SESSION['usucpf'], PFL_UO_EQUIPE_TECNICA);

}
$query = "
    SELECT
        uni.unicod AS codigo,
        uni.unicod || ' - ' || uni.unidsc AS descricao
    FROM public.unidade uni
    WHERE uni.unistatus = 'A'
        AND (uni.orgcod = '". CODIGO_ORGAO_SISTEMA. "' OR uni.unicod IN('74902', '73107'))
        {$whereUO}
    ORDER BY uni.unicod
";
$form->addInputCombo('unicod', $query, $_GET['unicod'], 'unicod', array('flabel' => 'Unidade Orçamentária (UO)'));
$query = <<<DML
    SELECT
        esdid AS codigo,
        esddsc AS descricao
    FROM workflow.estadodocumento
    WHERE tpdid = 224
        AND esdstatus = 'A'
    ORDER BY esdordem
DML;
$form->addInputCombo('esdid', $query, $_GET['esdid'], 'esdid', array('flabel' => 'Status do Documento'));
$form->addBotao('avancado',array('id' => 'buttonBack', 'label' => 'Voltar', 'class' => 'btn-warning', 'extra' => 'data-url="proporc.php?modulo=inicio&acao=C"', 'span' => 'glyphicon glyphicon-arrow-left'));
$form->addBotao('avancado',array('id' => 'buttonClean', 'label' => 'Limpar', 'class' => 'btn-warning', 'extra' => 'data-url="proporc.php?modulo=principal/prelimite/pessoal&acao=A"', 'span' => 'glyphicon glyphicon-refresh'));
#$form->addBotao('avancado',array('id' => 'buttonSend', 'label' => 'Novo', 'class' => 'btn-success', 'extra' => 'data-url="proporc.php?modulo=principal/prelimite/pessoal&acao=A"', 'span' => 'glyphicon glyphicon-plus'));
$form->addBotao('avancado',array('id' => 'buttonFind', 'label' => 'Buscar', 'class' => 'btn-primary', 'extra' => 'data-url="proporc.php?modulo=principal/prelimite/pessoal&acao=A"', 'span' => 'glyphicon glyphicon-search'));
$form->render();

include_once APPRAIZ . "proporc/modulos/principal/prelimite/prelimite/listarPrelimite.inc";