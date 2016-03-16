<?php
/**
 * Created by PhpStorm.
 * User: VictorMachado
 * Date: 14/08/2015
 * Time: 10:08
 */
global $db;
$planodecarreira = new PlanodecarreiraEstado();
$filtros         = array();
$pfls            = arrayPerfil();

// Filtra os municipios
$sql = "select * from territorios.municipio";
if( $_REQUEST['estuf'] ) $sql .= " where estuf = '" . $_REQUEST['estuf'] . "' ";
$sql .= " order by mundescricao asc ";
$municipios = $db->carregar($sql);
if( $_GET['filtroAjax'] ){
    ob_clean(); ?>
    <select name="muncod" class="form-control" id="muncod">
        <option value="">Filtrar município:</option>
        <?php
        foreach( $municipios as $chave => $municipio ){ ?>
            <option value="<?=$municipio['muncod']?>"><?=$municipio['mundescricao']?></option>
        <?php }?>
    </select>
<?php die();}

?>
<script>
    function pesquisar(){
        console.log($('#muncod').val());
        jQuery('#acao').val('pesquisar');
        jQuery('[name=\'formPesquisaLista\']').submit()
    }

    function editarPlanodeCarreira(pceid){
        window.location.href = "/sase/sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=edicao&pceid="+pceid;
    }
</script>
<div id="formularioFiltros">
    <form class="form-horizontal" name="formPesquisaLista" id="formPesquisaLista" role="form" method="POST">
        <input type="hidden" name="acao" id="acao"/>
        <input name="formulario" value="1" type="hidden"/>
        <input type="hidden" name="funcao" id="funcao" value=""/>

        <div class="row">
            <div class="col-lg-offset-3 col-lg-6 col-lg-offset-2">
                <div class="well">
                    <div class="form-group" id="divFiltroEstado">
                        <label for="filtroEstado" class="col-lg-2 control-label">Estado:</label>
                        <div class="col-lg-9">
                            <select name="estuf" id="estuf" class="form-control">
                                <option value="">Filtrar estado</option>
                                <?php
                                $sql = "select estuf, estdescricao from territorios.estado order by estdescricao ASC";
                                $estados = $db->carregar($sql);
                                foreach( $estados as $chave => $estado ){ ?>
                                    <option <?=($estado['estuf']==$_POST['estuf'])?'selected':''?> value="<?=$estado['estuf']?>"><?=$estado['estdescricao']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroSituacao">
                        <label for="filtroSituacao" class="col-lg-2 control-label">Plano de Carreira:</label>
                        <div class="col-lg-9">
                            <select name="esdid" class="form-control" id="esdid">
                                <option value="">Filtrar situação</option>
                                <?php
                                $sql = "select esdid, spcdsc from sase.sitplancarprofessor where spcstatus = 'A' order by spcid ASC ";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$_POST['esdid'])?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spcdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group" id="divFiltroSituacao">
                        <label for="filtroSituacao" class="col-lg-2 control-label">Adequação:</label>
                        <div class="col-lg-9">
                            <select name="esdid2" class="form-control" id="esdid2">
                                <option value="">Filtrar situação</option>
                                <?php
                                $sql = "select esdid, spmdsc from sase.sitplanomunicipio where spcstatus = 'A' order by spmid ASC ";
                                $situacoes = $db->carregar( $sql );
                                foreach( $situacoes as $chave => $situacao ){ ?>
                                    <option <?=($situacao['esdid']==$_POST['esdid2'])?'selected':''?> value="<?=$situacao['esdid']?>"><?=$situacao['spmdsc']?></option>
                                <?php }?>
                            </select>
                        </div>
                    </div>

                    <div class="form-group">
                        <label class="col-lg-2 control-label">&nbsp;</label>
                        <div class="col-lg-9">
                            <input type="button" class="btn btn-primary" onclick="pesquisar()" value="Pesquisar" />
                            &nbsp;&nbsp;
                            <input type="button" class="btn btn-primary" onclick="jQuery('[name=formulario]').val('1'); window.location.href='sase.php?modulo=principal/planodecarreiraEstado&acao=A&aba=lista'" value="Listar Todos" />
            <!--                &nbsp;&nbsp;-->
            <!--                <input type="button" class="btn btn-primary" onclick="geraRelatorioXls()" value="Exportar XLS" />-->
                        </div>
                    </div>
                </div>
            </div>
        </div>

        <hr/>

    </form>
    <div id="lista">
        <?php $planodecarreira->getLista(); ?>
    </div>
</div>