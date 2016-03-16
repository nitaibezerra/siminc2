<?php
$pflcod = $modeloPrelimites->pegarPerfilAtual($_SESSION['usucpf']);
$id = $_GET['id'];
if(!$id){
    $id = $modeloPrelimites->pegaPrpidPorUnidade($_GET['unicod']);
}
if ($id) {
    $docid = $modeloPrelimites->pegaDocid($id);
    $dados = $modeloPrelimites->capturaDados($id);
}

if($_GET['unicod'] || $dados['unicod']){
    $unicod = $_GET['unicod'] ? $_GET['unicod'] : $dados['unicod'];
    $unidade = $modeloPrelimites->pegarUO($unicod);
}
?>
<script type="text/javascript" src="../library/bootstrap-3.0.0/js/bootstrap.file-input.js"></script>
<script>
    $(document).ready(function(){
        $('#dl-modelo').on('click',function(){
            window.location = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A&requisicao=download-modelo&id='+ $(this).attr('data-prpid');
        });
        $('#dl-preenchimento').on('click',function(){
            window.location = 'proporc.php?modulo=principal/prelimite/pessoal&acao=A&requisicao=download-modelo-preenchimento&id='+ $(this).attr('data-prpid');
        });
        $('[data-toggle="popover"]').popover({trigger:'hover',placement:'left'});
        $("input[type=file]").bootstrapFileInput();
        $('#workflow_body table').css('width','100%');
        //$('#workflow_body table').css('width','100%').css('border-color','#bce8f1');
        //$('#workflow_body table tr[style="background-color: #c9c9c9; text-align:center;"]').css('background-color','#d9edf7').css('color','#3a87ad').css('border-color','#bce8f1');
    });
</script>
<section class="col-md-10 col-md-offset-1">
    <section class="panel panel-info">
        <section class="panel-heading"><h3 class="panel-title">Informações</h3></section>
        <table class="table">
            <thead>
                <tr>
                    <th class="text-left">Unidade</th>
                    <th class="text-left">Última Atualização</th>
                    <th class="text-left">Modelo</th>
                    <th class="text-left">Modelo Preenchido</th>
                    <th></th>
                </tr>
            </thead>
            <tbody>
                <tr>
                    <td><?=$unidade?></td>
                    <td><?=$dados['dataultimaatualizacao'] ? $dados['dataultimaatualizacao'] : '-'?></td>
                    <td>
                        <? $add = $modeloPrelimites->recuperaModelo($id);?>
                        <? if($add): ?>
                        <button class="btn btn-primary" data-prpid="<?=$id?>" id="dl-modelo" type="button">
                            <span class="glyphicon glyphicon-download-alt"></span>
                            Download
                        </button>
                        <? else: ?>
                        -
                        <? endif; ?>
                    </td>
                    <td>
                        <? $add = $modeloPrelimites->recuperaModeloPreenchido($id); ?>
                        <? if($add): ?>
                        <button class="btn btn-primary" data-prpid="<?=$id?>" id="dl-preenchimento" type="button">
                            <span class="glyphicon glyphicon-download-alt"></span>
                            Download
                        </button>
                        <? else: ?>
                        -
                        <? endif; ?>
                    </td>
                    <td></td>
                </tr>
                <tr>
                    <td><strong>Responsável:</strong></td>
                    <td><?= $dados['usunome'] ? $dados['usunome'] : '-'?></td>
                    <td>
                        <span class="label label-success"><span class="glyphicon glyphicon-earphone"></span></span>
                        <span id="user-tel"><?= $dados['usufonenum'] ? "({$dados['usufoneddd']}) {$dados['usufonenum']}" : '' ?></span>
                    </td>
                    <td>
                        <span class="label label-success"><span class="glyphicon glyphicon-envelope"></span></span>
                        <span id="user-mail"><?= $dados['usuemail'] ? $dados['usuemail'] : ''?></span>
                    </td>
                    <td style="width:5px!important">
                        <button type="button" <?=$modeloPrelimites->verificaPerfil() ? 'disabled' : ''?>
                                class="btn btn-warning btn-sm" id="altUsuario" data-usucpf="<?=$dados['usucpfresponsavel']?>"
                                data-prpid="<?=$dados['prpid']?>" data-toggle="popover" data-content="Atribuir novo responsável"
                                onclick="modalAltUsuario();" data-original-title="" title="">
                            <span class="glyphicon glyphicon-user"></span>
                        </button>
                    </td>
                </tr>
            </tbody>
        </table>
    </section>
</section>
<section class="row">
    <section class="col-md-10">
        <section class="well">
            <form method="post" class="form-horizontal" id="formPrelimites" enctype="multipart/form-data" role="form">
                <input type="hidden" name="requisicao" value="salvarPrelimite"/>
                <input type="hidden" name="unicod" value="<?=$unicod ? $unicod : $dados['unicod'] ?>"/>
                <input type="hidden" name="prpid" value="<?=$id?>"/>
                <? if($pflcod == PFL_CGO_EQUIPE_ORCAMENTARIA || $pflcod == PFL_ADMINISTRADOR): ?>
                <section class="form-group">
                    <label class="control-label col-md-2">Modelo:</label>
                    <section class="col-md-9">
                        <input type="file" class="btn-primary" name="modelo" value="" title="Adicionar arquivo" data-filename-placement="inside">
                        <p class="help-block">Extensões Permitidas: xls, xlsx</p>
                    </section>
                </section>
                <? endif;?>
                <section class="form-group">
                    <label class="control-label col-md-2">Modelo Preenchido:</label>
                    <section class="col-md-9">
                        <input type="file" class="btn-primary" name="preenchimento" value="" title="Adicionar arquivo" data-filename-placement="inside">
                        <p class="help-block">Extensões Permitidas: xls, xlsx</p>
                    </section>
                </section>

                <section class="form-group">
                    <section class="col-md-2"></section>
                    <section class="col-md-10">
                        <button class="btn btn-warning" type="button" id="buttonBack" data-url="proporc.php?modulo=principal/prelimite/pessoal&acao=A"><span class="glyphicon glyphicon-arrow-left"></span> Voltar</button>
                        <? if(podeSalvarPrelimite()): ?>
                        <button class="btn btn-success" type="submit" id="salvar"><span class="glyphicon glyphicon-ok"></span> Salvar</button>
                        <? else: ?>
                        <br>
                        <? gravacaoDesabilitada('Perído para preenchimento expirado. Data limite: 22/05/2015'); ?>
                        <? endif; ?>
                    </section>
                </section>
            </form>
        </section>
    </section>
    <section class="col-md-2">
        <section class="panel panel-info">
            <section class="panel-heading"><strong>Workflow</strong></section>
            <section class="panel-body" id="workflow_body">
                <?
                if($id){
                    wf_desenhaBarraNavegacao($docid , array('docid' => $docid, 'prpid' => $id));
                }
                ?>
            </section>
        </section>

    </section>
</section>