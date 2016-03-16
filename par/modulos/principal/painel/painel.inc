<?php
#registrarSessaoGrafico
if ($_POST['requisicao'] == 'registrarSessaoPainel') {
    unset($_SESSION['sessaoPainel']);
    $_SESSION['sessaoPainel'] = array(
        $_POST['idGrafico'] => $_POST['idArea']
    );
    echo "registrado";
    exit;
}

#Includes
include APPRAIZ . "includes/cabecalho.inc";
include APPRAIZ . "includes/funcoesspo_componentes.php";
require APPRAIZ . 'par/modulos/principal/painel/classes/controle/PainelControle.class.inc';
require APPRAIZ . 'par/modulos/principal/painel/classes/controle/GraficoControle.class.inc';
?>

<script type="text/javascript">
    
    //Realiza Consulta de Obras
    consultarObras = function (arrPost) {
        $.post("par.php?modulo=principal/painel/resultado&acao=A", {arrPost: arrPost},
        function (data) {
            $('.resultado').show();
            $('.filtro').show();
            $('#panel-element-3').addClass('in');
            $("#divConteudo").html(data);
        }, "html");
    };    
    
    //Registra sessao do grafico
    function registrarSessaoPainel(idPainel, idGrafico, idArea) {
        $('.resultado').hide();
        $('.filtro').hide();
        $obj = $(this);
        
        $.post("par.php?modulo=principal/painel/painel&acao=A", {requisicao: 'registrarSessaoPainel', idPainel: idPainel, idGrafico: idGrafico, idArea: idArea},
        function (data) {
            if (data === 'registrado') {
                //Fecha Painel nao clicado.
                $('.painelId').each(function(){
                    if (idPainel !== $obj.attr('id')) {
                        $obj.click();
                    }
                });
                consultarObras();
            }
        }, "html");
    };    
</script>

<div class="container-fluid">
    <div class="row clearfix" style="margin-bottom: 15px">
        <div class="col-md-12 column">
            <div class="page-header">
                <h3>
                    Painel de Empenho / Pagamento de Obras
                </h3>
            </div>
        </div>
    </div>
    <?php
    #Carrega Paineis
    $obPainel = new PainelControle();
    $arrPainel = $obPainel->getPainelByUsuario();
    foreach ($arrPainel as $painel):
        ?>
        <!-- Painel Graficos <?php echo $painel['idPainel'] ?>-->
        <div class="row clearfix" style="margin-bottom: 15px">
            <div class="col-md-12 column">
                <div class="panel-group" id="panel-<?php echo $painel['idPainel'] ?>">
                    <div class="panel panel-default">
                        <div class="panel-heading">
                            <a class="panel-title painelId" id="<?php echo $painel['idPainel'] ?>" data-toggle="collapse" data-parent="#panel-<?php echo $painel['idPainel'] ?>" href="#panel-element-<?php echo $painel['idPainel'] ?>">
                                Painel: <?php echo $painel['nome'] ?>
                            </a>
                        </div>
                        <div id="panel-element-<?php echo $painel['idPainel'] ?>" class="panel-collapse collapse in">
                            <div class="panel-body">
                                <div class="row clearfix" style="margin-bottom: 15px">
                                    <?php
                                    #Carrega Graficos
                                    $obGrafico = new GraficoControle();
                                    $arrGrafico = $obGrafico->getGraficoByPainel();

                                    #@TODO Na versao 2.0 mander o sql no banco
                                    $obPreObra = new PreObraControle();
                                    $intCount = 0;
                                    foreach ($arrGrafico as $grafico):
                                        ?>
                                        <div class="col-md-3 column">
                                            <div class="panel-group" id="subpanel-<?php echo $painel['idPainel'] . '-' . $grafico['idGrafico'] ?>">
                                                <div class="panel panel-default">
                                                    <div class="panel-heading">
                                                        <a class="panel-title" data-toggle="collapse" data-parent="#subpanel-<?php echo $painel['idPainel'] . '-' . $grafico['idGrafico'] ?>" href="#subpanel-element-<?php echo $painel['idPainel'] . '-' . $grafico['idGrafico'] ?>">
                                                            <?php echo $grafico['nome'] ?>
                                                        </a>
                                                    </div>
                                                    <div id="subpanel-element-<?php echo $painel['idPainel'] . '-' . $grafico['idGrafico'] ?>" class="panel-collapse collapse in">
                                                        <div class="panel-body">
                                                            <?php
                                                            $arrDados = $obPreObra->$grafico['funcao']();
                                                            $obGrafico = new Grafico();
                                                            $obGrafico->setId($painel['idPainel'] . '-' . $grafico['idGrafico'])
                                                                    ->setEvent(array('click' => "registrarSessaoPainel('" . $painel['idPainel'] . "','" . $grafico['idGrafico'] . "', event.point.name);"))
                                                                    ->gerarGrafico($arrDados);
                                                            ?>
                                                        </div>
                                                    </div>
                                                </div>
                                            </div>
                                        </div>
                                        <?php
                                        #Cria Quebra de linha quando estiver 4 graficos
                                        $intCount++;
                                        if ($intCount == 4) {
                                            echo "</div><div class=\"row clearfix\" style=\"margin-bottom: 15px\">";
                                            $intCount = 0;
                                        }
                                    endforeach;
                                    ?>
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>    
        <!-- /Painel Graficos <?php echo $painel['idPainel'] ?>-->
    <?php endforeach; ?>
    <!-- Filtro -->
    <?php include APPRAIZ . "par/modulos/principal/painel/filtro.inc"; ?>
    <!-- /Filtro -->

    <!-- Listagem-->
    <div class="row clearfix resultado" style="margin-bottom: 15px; display: none">
        <div class="col-md-12 column">
            <div class="panel-group" id="panel-3">
                <div class="panel panel-default">
                    <div class="panel-heading">
                        <a class="panel-title" data-toggle="collapse" data-parent="#panel-3" href="#panel-element-3">
                            Resultados
                        </a>
                    </div>
                    <div id="panel-element-3" class="panel-collapse collapse">
                        <div class="panel-body">
                            <!--                            <div class="row clearfix" style="margin-bottom: 15px">
                                                            <div class="col-md-12 column">
                                                                <div class="btn-group">
                                                                    <button href="#modal" role="button" name="addTarefa" class="btn btn-primary" data-toggle="modal">Atribuir Tarefa</button>
                                                                    <button type="button" class="btn btn-primary">Exportar</button>
                                                                </div>
                                                            </div>
                                                        </div>-->
                            <div class="row clearfix">
                                <div class="col-md-12 column" id="divConteudo">
                                </div>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    </div>    
    <!-- /Listagem-->
</div>

<!-- Modal -->
<div class="modal fade" id="modal" role="dialog" aria-labelledby="myModalLabel" aria-hidden="true">
    <div class="modal-dialog">
        <div class="modal-content">
            <div class="modal-header">
                <button type="button" class="close" data-dismiss="modal" aria-hidden="true">×</button>
                <h4 class="modal-title" id="myModalLabel">
                    Atribuir Tarefas
                </h4>
            </div>
            <div class="modal-body"></div>
            <div class="modal-footer">
                <button type="button" class="btn btn-default" data-dismiss="modal">Fechar</button>
                <button type="button" class="btn btn-primary">Salvar</button>
            </div>
        </div>
    </div>
</div>
<!-- Modal -->
<script src="/library/simec/js/listagem.js" type="text/javascript"></script>
<script type="text/javascript">
    $(function(){
        //Buscar Filtros
        $('button[name=addTarefa]').click(function () {
            alert(123);
            $('.modal-body').html($('#divConteudo').html());
        });

        //Buscar Filtros
        $('button[name=btnBuscar]').click(function () {
            consultarObras($('#frmPesquisar').serialize());
        });

        //Abrir Aba Filtro
        $('a[href=#panel-element-filtro]').click(function () {
            $('.chosen-container').each(function () {
                $(this).css('width', '100%');
            });
        });
    });

    //Realiza Consulta de Obras
    consultarObrsas = function (arrPost) {
        $.post("par.php?modulo=principal/painel/resultado&acao=A", {arrPost: arrPost},
        function (data) {
            $('.resultado').show();
            $('.filtro').show();
            $('#panel-element-3').addClass('in');
            $("#divConteudo").html(data);
        }, "html");
    };
    
    //Atualizar resultado
    atualizaResultado = function (arrPost, listagem) {
        $('#divConteudo').html('');
        $.post('par.php?modulo=principal/painel/resultado&acao=A', {arrPost: arrPost, listagem}, function (data) {
            $("#divConteudo").html(data);
        });
    };

    //Paginacao
    delegatePaginacao = function () {
        $('body').on('click', 'li[class="pgd-item"]:not(".disabled")', function () {
            var novaPagina = $(this).attr('data-pagina');
            var listagem = {};
            listagem['p'] = novaPagina;
            atualizaResultado($('#frmPesquisar').serialize(), listagem);
        });
    };

    // Abre Popup de Empenho ou Pagamento
    openPopUp = function (processo, proid, tipo) {
        window.open('par.php?modulo=principal/' + tipo + '&acao=A&processo=' + processo + '&proid=' + proid, 'Empenho', 'scrollbars=yes,status=no,toolbar=no,menubar=no,location=no');
    };
</script>