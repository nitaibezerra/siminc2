<?
include_once APPRAIZ . "agendagm/classes/modelo/ProgramacaoExercicio.class.inc";
require_once APPRAIZ . 'includes/library/simec/Listagem.php';

$modelProgramacaoExercicio = new ProgramacaoExercicio($_REQUEST['prsano']);

switch ($_REQUEST['action']) {

}

$_SESSION['sislayoutbootstrap'] = 'zimec';
// Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
?>

<?php  echo $simec->title('Bem-vindo'); ?>

<div class="wrapper wrapper-content animated fadeInRight">
    <div class="ibox float-e-margins">
        <div class="ibox-title">
            <h5>Filtros de pesquisa</h5>
            <div class="ibox-tools">
                <a class="collapse-link">
                    <i class="fa fa-chevron-up"></i>
                </a>
            </div>
        </div>
        <div class="ibox-content">
            <form method="post" name="formulario" id="formulario" class="form form-horizontal">
                <?php echo $simec->select('prsano', 'Ano de Referência', $modelProgramacaoExercicio->prsano, $modelProgramacaoExercicio->getAnos(), array('placeholder' => 'Selecione o ano', 'required')); ?>
                <?php echo $simec->input('prsano', 'Ano de Referência', $modelProgramacaoExercicio->prsano, array('required')); ?>
                <?php echo $simec->data('prsano', 'Ano de Referência', $modelProgramacaoExercicio->prsano, array('required')); ?>
            </form>
        </div>
    </div>
</div>


<script type="text/javascript">
    $(function(){
        $('div.date').datepicker({
            todayBtn: "linked",
            format: 'dd/mm/yyyy',
            language: 'pt-BR',
            keyboardNavigation: false,
            forceParse: false,
            calendarWeeks: true,
            autoclose: true
        });
    });
</script>