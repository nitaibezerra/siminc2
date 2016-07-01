<script language="JavaScript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="/library/bootstrap-switch/stylesheets/bootstrap-switch.css">
<script src="/library/bootstrap-switch/js/bootstrap-switch.min.js" type="text/javascript"></script>
<script type="text/javascript">
    function tudoMaiusculo(obj)
    {
        $(obj).val($(obj).val().toUpperCase());
    }
    function editarRelatorio(id) {
        window.location.assign('acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A&requisicao=relatorio&periodo='+$('#periodo').val()+'&rlgid='+id);
    }
    function exportarItem(params, tipo)
    {
        window.open('acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A&periodo='+$('#periodo').val()+'&' + tipo + '=true&rlgid=' + params[0]);
    }
    function apagarRelatorio(id)
    {
        bootbox.confirm('Tem certeza que quer apagar este relatório?', function(result){
            if (result) {
                var $form = $('<form />')
                    .attr({method: 'post', action:'acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A'})
                    .appendTo('body');
                $('<input />').attr({type:'hidden', name:'requisicao', value:'apagar'}).appendTo($form);
                $('<input />').attr({type:'hidden', name:'rlgid', value:id}).appendTo($form);
                $form.submit();
            }
        });
    }

    $(function(){
        $(".btn-acoes").on("click", function(){
            $("#acao-loa-container, #acacod-container").hide();
            if ($(this).attr("data-value") == 'R') {
                $("#acao-loa-container").show();
            } else {
                $("#acacod-container").show();
            }
        });

        // -- Correção paliativa do bug #9920 do bootstrap.
        // -- Correção agendada para a versão 3.0.3 do bootstrap.
        // -- <https://github.com/twbs/bootstrap/issues/9920>
        $("input:radio").change(function() {
            $(this).prop('checked', true);
        });

        $('#unicod').change(function(){
            var unicod = $(this).val();
            $.post(window.location.url, {unicod:unicod, requisicao:'consultar-acoes'}, function(data){
                $('#acacod').html(data);
                $('#acacod').trigger('chosen:updated');
            });
        });

        $('#btnBuscar').click(function(){
            var acatipo = 'N';
            var acao = 'acacod';
            if($($('[name=acatipo]')[1]).is(':checked')){
                acatipo = 'R';
                acao = 'acaofloa';
            };
            window.location.href = 'acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A&requisicao=filtro&periodo='+$('#periodo').val()+'&unicod='+$('#unicod').val()+'&acatipo='+acatipo+'&'+acao+'='+$('#'+acao).val();
        });

        $('#inserir').on('click',function(){
           if($('#periodo').val() == ''){
               bootbox.alert('Favor selecionar um Período.');
               return false;
           }
           if($('#unicod').val() == ''){
               bootbox.alert('Favor selecionar uma Unidade Orçamentária.');
               return false;
           }
           if($('#acacod').val() == '' && $('#acaofloa').val() == ''){
               bootbox.alert('Favor selecionar uma Ação.');
               return false;
           }
           $('#formPrincipal').submit();

        });
    });
</script>
<div class="well">
    <form id="formPrincipal" name="formPrincipal" method="GET" class="form-horizontal" enctype="multipart/form-data">
        <input type="hidden" name="requisicao" id="requisicao" value="cadastrar">
        <div class="form-group">
            <label for="periodo" class="col-lg-2 control-label">Período de Referência:</label>
            <div class="col-lg-10">
                <?php
                inputCombo('periodo', sprintf(retornaQueryComboPeriodo(),$_SESSION['exercicio'],$tipo), $prfid, 'periodo',array('mantemSelecaoParaUm' => false));
                ?>
            </div>
        </div>
        <div class="form-group">
            <div class="col-lg-02">
                <label class="col-lg-2 control-label" for="unicod">Unidade Orçamentária:</label>
            </div>
            <div class="col-lg-10">
            <?php
            $unicod = $_REQUEST['unicod'];
            inputCombo('unicod', retornaComboUnidadeOrcamentaria($perfis), $unicod, 'unicod');
            ?>
            </div>
        </div>
        <div class="form-group">
            <div class="col-lg-02">
                <label class="col-lg-2 control-label">Tipo:</label>
            </div>
            <div class="col-lg-10">
                <div class="btn-group" data-toggle="buttons">
                    <label class="btn btn-default btn-acoes <?php echo ($_GET['acatipo'] != 'R')?'active':''; ?>" data-value="N">
                        <input type="radio" class="radio_acaorap" id="radio_acaorap_N" value="N" name="acatipo"
                               <?php echo ($_GET['acatipo'] != 'R')?'checked="checked"':''; ?> /> Ações LOA <?=$_SESSION['exercicio']?>
                    </label>
                    <label class="btn btn-default btn-acoes <?php echo ($_GET['acatipo'] == 'R')?'active':''; ?>" data-value="R">
                        <input type="radio" class="radio_acaorap" id="radio_acaorap_R" value="R" name="acatipo"
                               <?php echo ($_GET['acatipo'] == 'R')?'checked="checked"':''; ?>/> Ações não Previstas LOA <?=$_SESSION['exercicio']?>
                    </label>
                </div>
            </div>
        </div>
        <div class="form-group" id="acao-loa-container" style="display:none">
            <div class="col-lg-02">
                <label class="col-lg-2 control-label" for="acaofloa">Ações não previstas<br /> na LOA <?= date('Y'); ?>:</label>
            </div>
            <div class="col-lg-10">
                <?= inputTexto('acaofloa', isset($_GET['acaofloa']) ? $_GET['acaofloa'] : '', 'acaofloa', 4, false, array('evtkeyup' => 'tudoMaiusculo(this);')); ?>
            </div>
        </div>
        <div class="form-group" id="acacod-container">
            <div class="col-lg-02">
                <label class="col-lg-2 control-label" for="acacod">Ação:</label>
            </div>
            <div class="col-lg-10">
            <?php filtroAcao($perfis, $_GET['acacod'], $_GET['unicod']); ?>
            </div>
        </div>
        <div class="col-lg-12">
            <button class="btn btn-warning" type="reset" onclick="window.location = 'acomporc.php?modulo=principal/relatoriogestao/relatorio&acao=A';">Limpar</button>
            <?php if (!in_array(PFL_RELATORIO_TCU, $perfis)): ?>
            <button class="btn btn-success" id="inserir" type="button">
                <span class="glyphicon glyphicon-ok"></span> Avançar
            </button>
            <button class="btn btn-primary" id="btnBuscar" type="button">
                <span class="glyphicon glyphicon-search"></span> Buscar
            </button>
            <?php else: ?>
            <button class="btn btn-primary" id="btnBuscar" type="button">
                <span class="glyphicon glyphicon-search"></span> Buscar
            </button>
            <?php endif; ?>
        </div>
    </form>
</div>
<?php
    require(dirname(__FILE__) . "/listarRelatorio.inc");
