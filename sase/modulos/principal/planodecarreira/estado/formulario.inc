<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 15:34
 */

global $db;
$pceid = $_REQUEST['pceid'];
if(!$pceid){
    echo "
            <script>
                alert('Selecione um estado para acessar esta funcionalidade.');
                window.location.href = 'sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista';
            </script>
        ";
    die;
}
$planodecarreira = new PlanodecarreiraEstado();
$planodecarreira->carregarPorId($pceid);
$pfls = arrayPerfil();

if(!$planodecarreira->docid && $pceid){
    $docid = wf_cadastrarDocumento(TPDID_SASE_PLANODECARREIRA, 'Documento Plano de Carreira');
    $planodecarreira->docid = $docid;
    $planodecarreira->alterar();
    $planodecarreira->commit();
}

if(!$planodecarreira->docid2 && $pceid){
    $docid = wf_cadastrarDocumento(TPDID_SASE_SITUACAO_PLANOCARREIRA, 'Documento Situação Adequação Plano de Carreira');
    $planodecarreira->docid2 = $docid;
    $planodecarreira->alterar();
    $planodecarreira->commit();
}

if($planodecarreira->docid) {
    $sql = "select esdid from workflow.documento where docid = " . $planodecarreira->docid;
    $esdid = $db->pegaUm($sql);
}

if($planodecarreira->docid2) {
    $sql = "select esdid from workflow.documento where docid = " . $planodecarreira->docid2;
    $esdid2 = $db->pegaUm($sql);
}

?>
<style>
    #formulario{
        padding-top: 20px;
    }
    .btn{
        margin-left: auto;
        margin-right: auto;
    }
</style>
<script>
    function voltar(){
        window.location.href = "sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista";
    }
</script>
<div id="formulario">
    <form class="form-horizontal" name="formCadastroLista" role="form" method="POST" class=form-vertical enctype="multipart/form-data">
        <input type="hidden" id="acao" name="acao" value=""/>
        <input type="hidden" id="pcpid" name="pcpid" value="<?= $pceid ?>"/>
        <div class="row">
            <div class="col-lg-offset-3 col-lg-6">
                <div class="well">
                    <legend>Plano de Carreira</legend>

                    <div class="form-group">
                        <label class="col-lg-2 col-md-2 control-label" for="estuf">Estado:</label>
                        <div class="col-lg-9 col-md-9">
                            <select name="estuf" id="estuf" disabled class="form-control">
                                <?php
                                $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                                $estados = $db->carregar($sql);
                                foreach( $estados as $chave => $estado ){ ?>
                                    <option <?=($estado['estuf']==$planodecarreira->estuf)?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroSituacao">
                        <label for="filtroSituacao" class="col-lg-2 control-label">Plano de carreira:</label>
                        <div class="col-lg-9">
                            <select name="esdid" disabled class="form-control" id="esdid">
                                <option value="">Filtrar situação</option>
                                <?php
                                $sql = "select esdid, spcdsc from sase.sitplancarprofessor where spcstatus = 'A' order by spcid ASC ";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$esdid)?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spcdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroAdequacao">
                        <label for="filtroAdequacao" class="col-lg-2 control-label">Adequação</label>
                        <div class="col-lg-9">
                            <select name="filtroAdequacao" id="filtroAdequacao" disabled class="form-control">
                                <?php
                                $sql = "select esdid, spmdsc from sase.sitplanomunicipio where spcstatus = 'A' order by spmid ASC";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$esdid2)?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spmdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <div class="col-lg-12 text-center">
                            <input onclick="javascript:window.location.href='sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista'"
                                   value="Voltar" type="button" class="btn btn-primary"/>
                        </div>
                    </div>

                </div>
            </div>
            <div class="col-lg-1">
                <?php
                    if($planodecarreira->docid && $pceid){
                        wf_desenhaBarraNavegacao($planodecarreira->docid, array('docid'=>$planodecarreira->docid), null, 'Plano de Carreira');
                    }
                ?>
            </div>
            <div class="col-lg-1">
                <?php
                    if($planodecarreira->docid2 && $pceid){
                        wf_desenhaBarraNavegacao($planodecarreira->docid2, array('docid'=>$planodecarreira->docid2), null, 'Adequação');
                    }
                ?>
            </div>
        </div>
    </form>
</div>