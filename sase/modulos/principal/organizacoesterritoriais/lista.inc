<?php
// --------------- Dependências
include_once '../../sase/classes/Territorio.class.inc';
include APPRAIZ . 'includes/cabecalho.inc';
// --



global $db;
$Territorio = new Territorio();


// tratamento de filtros -------------------------
// estado
$sql = "select * from territorios.estado order by estdescricao ASC";
$estados = $db->carregar($sql);
// estado /
// mesorregião
$sql = "select mes.mesid, mes.mesdsc from sase.mesoregiao mes inner join sase.orgaoestado oes on oes.oesid = mes.oesid inner join sase.orgao org on org.orgid = oes.orgid";
if ($_REQUEST['filtroEstado'])
    $sql .= " where oes.estuf = '" . $_REQUEST['filtroEstado'] . "' ";
$sql .= " order by mesdsc asc ";
$mes = $db->carregar($sql);
if ($_GET['filtroAjax'] && $_GET['filtroMesorregiao']) {
    ob_clean();
    ?>
    <select name="filtroMesdsc" id="filtroMesdsc" onchange="javascript:if (this.value != '') {
                    filtraMunicipiosMesorregiao(this.value);
                }">
        <option value="">Filtrar mesorregião</option>
        <?php foreach ($mes as $chave => $meso) { ?>
            <option value="<?= $meso['mesid'] ?>"><?= $meso['mesdsc'] ?></option>
        <?php } ?>
    </select>
        <?php
        die();
    }
// mesorregião

// municipio
    $sql = "select * from territorios.municipio mun ";
    if ($_REQUEST['filtroEstado'] && !$_REQUEST['filtroMesdsc']) {
        $sql .= " where estuf = '" . $_REQUEST['filtroEstado'] . "' ";
    }
    if ($_REQUEST['filtroEstado'] && $_REQUEST['filtroMesdsc']) {
        $sql .= " inner join sase.territorio sat on sat.muncod = mun.muncod
        inner join sase.mesoregiao mes on sat.mesid = mes.mesid
where mes.mesid = '{$_REQUEST['filtroMesdsc']}' and estuf = '" . $_REQUEST['filtroEstado'] . "' ";
    }
    if (!$_REQUEST['filtroEstado'] && $_REQUEST['filtroMesdsc']) {
        $sql .= " inner join sase.territorio sat on sat.muncod = mun.muncod
inner join sase.mesoregiao mes on sat.mesid = mes.mesid
where mes.mesid = '{$_REQUEST['filtroMesdsc']}'";
    }
    $sql .= " order by mundescricao asc ";
    $municipios = $db->carregar($sql);
    //ver($municipios);
    
    if ($_GET['filtroAjax'] && $_GET['filtroMunicipio']) {
        ob_clean();
        ?>
    <select name="filtroMunicipio" id="filtroMunicipio">
        <option value="">Filtrar município</option>
        <?php  foreach ($municipios as $chave => $municipio) { ?>
            <option value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
        <?php } ?>
    </select>
    <?php
    die();
}

// municipio /
// mesoregiao
$sql = " select * from sase.mesoregiao order by mesdsc asc ";
$mesoregioes = $db->carregar($sql);
// mesoregiao /
// para lista
if ($_POST['formulario'])
    foreach ($_POST as $chave => $valor)
        if ($valor != '')
            switch ($chave) {
                case 'filtroMesdsc':
                    $filtros['mes.mesid'] = $valor;
                    break;
                case 'filtroMunicipio':
                    $filtros['t.muncod'] = $valor;
                    break;
                case 'filtroTerstatus':
                    $filtros['t.terstatus'] = $valor;
                    break;
                case 'filtroEstado':
                    $filtros['e.estuf'] = $valor;
                    break;
            }
// para lista /
// para exclusao
if ($_GET['excluir'])
    if ($Territorio->excluirTerritorio($_GET['terid'])) {
        echo "
			<script>
				alert('Território excluído com sucesso!');
				window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A';
			</script>";
        exit;
    }
// para exclusao /
// tratamento de filtros ------------------------- /
?>

<div id="container">

    <!-- ------------------------------- FORM ------------------------------- -->

    <div id="formularioFiltros">
        <form class="form-horizontal" name="formPesquisaLista" role="form" method="POST">
            <input name="formulario" value="1" type="hidden"/>

            <div class="form-group" id="divFiltroEstado">
                <label for="filtroEstado" class="col-sm-3 control-label">Estado:</label>
                <div class="col-sm-8 inputarea">
                    <select name="filtroEstado" id="filtroEstado" onchange="javascript:if (this.value != '') {
                                filtrarMesorregiao(this.value);
                                filtraMunicipios(this.value);
                            }">
                        <option value="">Filtrar estado</option>
                        <?php foreach ($estados as $chave => $estado) { ?>
                            <option <?= ($estado['estuf'] == $_POST['filtroEstado']) ? 'selected' : '' ?> value="<?= $estado['estuf'] ?>"><?= $estado['estdescricao'] ?></option>
                        <?php } ?>
                    </select>
                </div>
            </div>

            <div class="form-group" id="divFiltroTerritorio">
                <label for="filtroMesdsc" class="col-sm-3 control-label">Mesorregião:</label>
                <div class="col-sm-8 inputarea">
                    <select name="filtroMesdsc" id="filtroMesdsc" onchange="javascript:if (this.value != '') {
                                                    filtraMunicipiosMesorregiao(this.value);
                                                }">
                        <option value="">Filtrar mesorregião</option>
<?php foreach ($mes as $chave => $mesoregiao) { ?>
                            <option <?= ($mesoregiao['mesid'] == $_POST['filtroMesdsc']) ? 'selected' : '' ?> value="<?= $mesoregiao['mesid'] ?>"><?= $mesoregiao['mesdsc'] ?></option>
                        <?php } ?>
                    </select>
                </div>
            </div>

            <div class="form-group" id="divFiltroMunicipio">
                <label for="filtroMunicipio" class="col-sm-3 control-label">Município:</label>
                <div class="col-sm-8 inputarea">
<?php if (!$_POST['filtroEstado']) {
    echo "Selecione um Estado";
} else { ?>
                        <select name="filtroMunicipio" id="filtroMunicipio">
                            <option value="">Filtrar município</option>
    <?php foreach ($municipios as $chave => $municipio) { ?>
                                <option <?= ($municipio['muncod'] == $_POST['filtroMunicipio']) ? 'selected' : '' ?> value="<?= $municipio['muncod'] ?>"><?= $municipio['mundescricao'] ?></option>
                        <?php } ?>
                        </select>
                        <?php } ?>
                </div>
            </div>

            <div class="form-group" id="divFiltroSituacao">
                <label for="filtroTerstatus" class="col-sm-3 control-label">Status:</label>
                <div class="col-sm-8 inputarea">
                    <select name="filtroSqpstatus" id="filtroSqptatus">
                        <option value="">Status</option>
                        <option <?= ($_POST['filtroTerstatus'] == 'A') ? 'selected' : '' ?> value="A">Ativo</option>
                        <option <?= ($_POST['filtroTerstatus'] == 'I') ? 'selected' : '' ?> value="I">Inativo</option>
                    </select>
                </div>
            </div>

            <div class="form-group">
                <label class="col-sm-3 control-label">&nbsp;</label>
                <div class="col-sm-8">
                    <input type="button" class="btn btn-primary" onclick="jQuery('[name=\'formPesquisaLista\']').submit()" value="Pesquisar" />
                    &nbsp;&nbsp;&nbsp;
                    <a href="/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A" class="btn btn-primary">Listar Todos</a>
                    &nbsp;&nbsp;&nbsp;
                    <a href="/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao" class="btn btn-primary">Novo Território</a>
                </div>
            </div>
        </form>
    </div>

    <!-- / ------------------------------- FORM ------------------------------- -->

<?php
$cabecalho = array("Açao", "Mesoregião", "Estado", "Município", "Status");
$alinhamento = array('center', 'left', 'left', 'left', 'left');
$larguras = array('5%', '35%', '20%', '20%', '20%');
$db->monta_lista($Territorio->montaListaQuery($filtros), $cabecalho, 30, 5, 'N', '', 'N', 'listaTerritorio', $larguras, $alinhamento);
?>

</div>

<div id="footer"></div>
<!-- /html -->

<!-- script js -->
<script>
//      jQuery(document).ready(function() {
//        if(jQuery('#filtroEstado').val()){
//            filtrarMesorregiao(jQuery('#filtroestado').val());
//        }  
//       });
    /**
     * Filtro de municipios com a mudanca do estado
     */
    function filtraMunicipios(estuf) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroEstado: estuf, filtroMunicipio: true},
            success: function(resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
            }
        });
    }

    /**
     * Filtro de municipios com a mudanca do estado
     */
    function filtraMunicipiosMesorregiao(mesorregiao) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroMesdsc: mesorregiao, filtroMunicipio: true},
            success: function(resposta) {
                jQuery('#divFiltroMunicipio .inputarea').html(resposta);
            }
        });
    }

    function filtrarMesorregiao(estuf) {
        jQuery.ajax({
            url: '',
            type: 'GET',
            data: {filtroAjax: true, filtroEstado: estuf, filtroMesorregiao: true},
            success: function(resposta) {
                jQuery('#divFiltroTerritorio .inputarea').html(resposta);
            }
        });
    }

    /**
     * Manda para edicao de questoespontuais
     */
    function editarTerritorio(terid) {
        window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&aba=edicao&terid=' + terid;
    }

    /**
     * Excluir logicamente questoespontuais
     */
    function excluirTerritorio(terid) {
        if (confirm("Deseja realmente inativar essa Situação de Questões Pontuais?")) {
            window.location.href = '/sase/sase.php?modulo=principal/organizacoesterritoriais&acao=A&excluir=1&terid=' + terid;
        }
    }
</script>
<!-- script js / -->