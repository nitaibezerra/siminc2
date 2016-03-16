<?php
/**
 * Formulário de filtro de listagem e gestão de regras.
 * $Id: formRegra.inc 78244 2014-04-01 18:26:00Z maykelbraz $
 */
?>
<script type="text/javascript" language="javascript">
    $(document).ready(function(){
        $('#cadastroPeriodo').on('click',function(){
            if(trim($('#titulo').val()) == ''){
                bootbox.alert('Campo Titulo obrigatório.');
                $('#titulo').focus();
                return false;
            }
            if($('#inicio').val().trim() == '' ){
                bootbox.alert('Campo Início da Vigência obrigatória.');
                $('#inicio').focus();
                return false;
            }
            if($('#fim').val().trim() == '' ){
                bootbox.alert('Campo Fim da Vigência obrigatória.');
                $('#fim').focus();
                return false;
            }
            if($('#iniciop').val().trim() == '' ){
                bootbox.alert('Campo Inicio Período Preenchimento obrigatório.');
                $('#iniciop').focus();
                return false;
            }
            if($('#fimp').val().trim() == '' ){
                bootbox.alert('Campo Fim Período Preenchimento obrigatório.');
                $('#fimp').focus();
                return false;
            }
        });
    });
</script>
<section class="well">
    <form name="regra" id="form-regra" method="POST" class="form-horizontal" role="form" novalidate="novalidate">
        <input type="hidden" name="requisicao" id="requisicao" value="cadastrarPeriodo" />
        <input type="hidden" name="prftipo" value="<?=$tipo?>" />
        <input type="hidden" name="prfid" value="<?=$dados['prfid']?>" />
        <input type="hidden" name="prsano" value="<?=$_SESSION['exercicio']?>" />

        <div class="form-group">
            <label for="tipo" class="col-lg-2 control-label">Tipo de Período:</label>
            <div class="col-lg-10">
                <p class="form-control-static">
                    <span class="label label-info"><?=$gerenciador->__get('typeName')?></span>
                </p>
            </div>
        </div>

        <div class="form-group">
            <label for="ano" class="col-lg-2 control-label">Exercício:</label>
            <div class="col-lg-10">
                <p class="form-control-static">
                    <span class="label label-info"><?=$_SESSION['exercicio']?></span>
                </p>
            </div>
        </div>

        <div class="form-group">
            <label for="titulo" class="col-lg-2 control-label">Título:</label>
            <div class="col-lg-10">
                <?php
                $complemento =  "placeholder = 'Digite o título para o período.' required";
                inputTexto('prftitulo', $dados['prftitulo'], 'titulo', 250, FALSE, array('complemento' => $complemento));
                ?>
            </div>
        </div>

        <div class="form-group">
            <label for="descricao" class="col-lg-2 control-label">Descrição:</label>
            <div class="col-lg-10">
                <?php
                inputTextArea('prfdescricao', $dados['prfdescricao'], 'descricao', 500, array());
                ?>
            </div>
        </div>

        <div class="row">
            <div class="col-lg-2">
            </div>
            <div class="col-lg-10" style="padding-right: 0;padding-left: 4px;">
                <div class="panel panel-default">
                    <div class="panel-heading" style="background-color: white;"><strong>Vigência</strong></div>
                    <div class="panel-body">
                        <div class="col-lg-3">
                            <label for="inicio" class="control-label">Início</label>
                            <?
                            inputData('prfinicio', $dados['prfinicio'], 'inicio');
                            ?>
                        </div>

                        <div class="col-lg-3">
                            <label for="fim" class="control-label">Fim</label>
                            <?
                            inputData('prffim', $dados['prffim'], 'fim');
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>

         <div class="row">
            <div class="col-lg-2">
            </div>
            <div class="col-lg-10" style="padding-right: 0;padding-left: 4px;">
                <div class="panel panel-default">
                    <div class="panel-heading" style="background-color: white;"><strong>Período de Preenchimento</strong></div>
                    <div class="panel-body">
                        <div class="col-lg-3">
                            <label for="iniciop" class="control-label">Início</label>
                            <?
                            inputData('prfpreenchimentoinicio', $dados['prfpreenchimentoinicio'], 'iniciop');
                            ?>
                        </div>

                        <div class="col-lg-3">
                            <label for="fimp" class="control-label">Fim</label>
                            <?
                            inputData('prfpreenchimentofim', $dados['prfpreenchimentofim'], 'fimp');
                            ?>
                        </div>
                    </div>
                </div>
            </div>
        </div>
        <div class="form-group">
            <div class="col-md-2"></div>
            <div class="col-md-10">
                <button type="button" class="btn btn-warning" id="buttonBack" data-url="acomporc.php?modulo=principal/periodo/periodo&acao=A&tipo=<?=$tipo?>"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
                <button type="submit" class="btn btn-success" id="cadastroPeriodo"><span class="glyphicon glyphicon-plus-sign"></span> <?= $_GET['requisicao'] == 'new' ? 'Cadastrar' : 'Atualizar' ?></button>
            </div>
        </div>
    </form>
</section>