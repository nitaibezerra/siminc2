<?php
$arOrgid = verificaAcessoEmOrgid();
//$userResp = new UsuarioResponsabilidade();
//$arOrgid = $userResp->pegaOrgidPermitido( $_SESSION['usucpf'] );
if ( !in_array( $_SESSION['obras2']['orgid'], $arOrgid ) ){
	$_SESSION['obras2']['orgid'] = '';
}
$_SESSION['obras2']['orgid'] = 3; //$_REQUEST['orgid'] ? $_REQUEST['orgid'] : $_SESSION['obras2']['orgid'];
$_SESSION['obras2']['orgid'] = ($_SESSION['obras2']['orgid'] ? $_SESSION['obras2']['orgid'] : current( $arOrgid ));
$orgid 						 = $_SESSION['obras2']['orgid'];

$_SESSION['obras2']['empid'] = '';
$_SESSION['obras2']['obrid'] = '';

#Requisicao Limpar
if ($_POST['req'] == 'limpar') {
    unset($_SESSION['obras2']['solicitacao']['filtros']);

    echo "<script>window.location.href = window.location.href;</script>";
    exit();
}

switch ($_REQUEST['ajax']) {
    case 'municipio':
        header('content-type: text/html; charset=ISO-8859-1');
        $estuf = $_REQUEST['estuf'];
        ?>
        <script>
            $1_11(document).ready(function () {
                $1_11('select[name="muncod[]"]').chosen();

            });
        </script>
        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
            <?php   $municipio = new Municipio();
            foreach ($municipio->listaComboMulti($estuf) as $key) {
                ?>
                <option
                    value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
            <?php } ?>
        </select>
        <?php
        exit;
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaorgaodesbloqueio');
echo montarAbasArray($arAba, "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . $orgid);

monta_titulo( 'Lista de Obras Solicitação', 'Filtre as Obras');
if(empty($_POST) && !empty($_SESSION['obras2']['solicitacao']['filtros']))
    $_POST = $_SESSION['obras2']['solicitacao']['filtros'];
else
    $_SESSION['obras2']['solicitacao']['filtros'] = $_POST;
extract( $_POST );

$pflcods = array(PFLCOD_SUPER_USUARIO, PFLCOD_GESTOR_MEC);
$permissaoFiltro = false;
if (possui_perfil($pflcods)) {
    $permissaoFiltro = true;
}
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.7.2.min.js"></script>

<form method="post" name="formListaObraCumprimento" id="formListaObraCumprimento">
	<input type="hidden" name="req" id="req" value="">
	<input type="hidden" name="obrid" id="obrid" value="">
	<input type="hidden" name="empid" id="empid" value="">
	
        <table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
            <tr>
                <td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
                <td>
                    <?= campo_texto('obrbuscatexto', 'N', 'S', '', 70, 100, '', '', '', '', '', 'id="obrbuscatexto"'); ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Situação:
                </td>
                <td>
                    <select name="strid[]" class="chosen-select" multiple data-placeholder="Selecione">
                        <?php $situacaoRegistro = new SituacaoRegistro();
                        foreach ($situacaoRegistro->listaCombo() as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($strid) && in_array($key['codigo'], $strid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Programa:
                </td>
                <td>
                    <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                        <?php  $programa = new ProgramaFonte();
                        $param = array("orgid" => $_SESSION['obras2']['orgid']);
                        foreach ($programa->listacombo($param, false) as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($prfid) && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">
                    Fonte:
                </td>
                <td>
                    <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                        <?php  $tipoOrigemObra = new TipoOrigemObra();
                        $param = array();
                        foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($tooid) && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>
                </td>
            </tr>
            <tr>

                <td class="SubTituloDireita">UF(s):</td>
                <td>

                    <select name="estuf[]" class="chosen-select estados" multiple data-placeholder="Selecione">
                        <?php  $uf = new Estado();
                        foreach ($uf->listaCombo() as $key) {
                            ?>
                            <option
                                value="<?php echo $key['codigo'] ?>" <?php if (isset($estuf) && in_array($key['codigo'], $estuf)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                        <?php } ?>
                    </select>


                </td>


            </tr>
            <tr>
                <td class="SubTituloDireita">Municípios(s):</td>
                <td class="td_municipio" id="td_municipio">
                    <?php if (!empty($estuf)) { ?>

                        <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                            <?php   $municipio = new Municipio();
                            foreach ($municipio->listaComboMulti($estuf) as $key) {
                                ?>
                                <option
                                    value="<?php echo $key['codigo'] ?>" <?php if (isset($muncod) && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                            <?php } ?>
                        </select>
                    <?php } ?>
                </td>
            </tr>
            <tr>
                <td class="SubTituloDireita">Esfera:</td>
                <td>
                    <?php
                    $sql = Array(Array('codigo' => 'E', 'descricao' => 'Estadual'),
                        Array('codigo' => 'M', 'descricao' => 'Municipal'));
                    $db->monta_combo('empesfera', $sql, 'S', 'Selecione...', '', '', '', 200, 'N', 'empesfera');
                    ?>
                </td>
            </tr>

            <tr>
                <td class="SubTituloDireita">Convênio/Termo:</td>
                <td>
                    Número:&nbsp;
                    <?php
                    echo campo_texto('convenio', 'N', 'S', '', 20, 20, '####################', '', 'right', '', 0, '');
                    ?>
                    Ano:&nbsp;
                    <?php
                    echo campo_texto('ano_convenio', 'N', 'S', '', 4, 4, '####', '', 'right', '', 0, '');
                    ?>
                </td>
            </tr>
		<tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
			</td>
		</tr>
	</table>
</form>
<?php
    $coluns = array();
    $join = array();
    $where = array();

    if ($_REQUEST['obrbuscatexto']) {
        $obrbuscatextoTemp = removeAcentos(str_replace("-", " ", (trim($_REQUEST['obrbuscatexto']))));
        $obrbuscatextoTemp = trim($obrbuscatextoTemp);

        if (!strpos($obrbuscatextoTemp, ',')) {
            $where['obrnome'] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $obrbuscatextoTemp . "%') OR
                        obr.obrid::CHARACTER VARYING ILIKE ('%" . trim($_REQUEST['obrbuscatexto']) . "%') ) ";
        } else {
            $campos = explode(',', $obrbuscatextoTemp);
            $w = array();
            foreach ($campos as $c) {
                $c = trim($c);
                $w[] = " ( ( UPPER(public.removeacento(obr.obrnome) ) ) ILIKE ('%" . $c . "%') OR
                        obr.obrid::CHARACTER VARYING ILIKE ('%" . $c . "%') ) ";
            }

            $w = '(' . implode('OR', $w) . ')';
            $where['obrnome'] = $w;
        }

    }

    if ($_REQUEST['strid'] && $_REQUEST['strid'][0] != '')
        $where['strid'] = "srd.strid IN(" . implode(',', $_REQUEST['strid']) . ")";
    if ($_REQUEST['prfid'] && $_REQUEST['prfid'][0] != '')
        $where['prfid'] = "emp.prfid IN(" . implode(',', $_REQUEST['prfid']) . ")";
    if ($_REQUEST['tooid'] && $_REQUEST['tooid'][0] != '')
        $where['tooid'] = "obr.tooid IN(" . implode(',', $_REQUEST['tooid']) . ")";
    if ($_REQUEST['estuf'] && $_REQUEST['estuf'][0] != '')
        $where['estuf'] = "mun.estuf IN ('" . implode("', '", $_REQUEST['estuf']) . "')";
    if ($_REQUEST['muncod'] && $_REQUEST['muncod'][0] != '')
        $where['muncod'] = "mun.muncod IN ('" . implode("', '", $_REQUEST['muncod']) . "')";
    if ($_REQUEST['empesfera'])
        $where['empesfera'] = "emp.empesfera IN('" . $_REQUEST['empesfera'] . "')";
    if ($_REQUEST['convenio'])
        $where['convenio'] = "p_conv.termo_convenio = '{$_REQUEST['convenio']}'";
    if ($_REQUEST['ano_convenio'])
        $where['ano_convenio'] = "p_conv.ano_termo_convenio = '{$_REQUEST['ano_convenio']}'";

    $cabecalho = array('Ação', 'ID Cumprimento Objeto', 'ID Obra', 'Obra', 'UF', 'Município', "Estado da Obra", 'Numero do Processo', 'Situação do Cumprimento do Objeto', 'Data de Cadastro');

    $sql = "
        select
            DISTINCT
            '<center>
                <div style=\"width:100%;\">
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: acessaCumprimento(\'' || obr.obrid || '\');\"
                        title=\"Acessar Cumprimento do Objeto\">
                </div>
            </center>' AS acao,
            co.coid,
            obr.obrid,
            '<a href=\"javascript: alterarObr(\'' || obr.obrid || '\');\">(' || obr.obrid || ') ' || obr.obrnome || '</a>' as dslcricao,
            ende.estuf,
            mun.mundescricao,
            esd2.esddsc as estado_obra,
            p_conv.pronumeroprocesso as processo,
            esd.esddsc as estado_cumprimento,
            TO_CHAR(doc.docdatainclusao,'DD/MM/YYYY') as data_inclusao
        from obras2.cumprimento_objeto co
        inner join obras2.obras obr ON (co.obrid = obr.obrid)
        inner join workflow.documento doc ON(co.docid = doc.docid)
        inner join workflow.estadodocumento esd ON(doc.esdid = esd.esdid)
        inner join workflow.documento doc2 ON(obr.docid = doc2.docid)
        inner join workflow.estadodocumento esd2 ON(doc2.esdid = esd2.esdid)
        inner join obras2.empreendimento emp on emp.empid = obr.empid
        left join entidade.endereco ende ON ende.endid = obr.endid
            and ende.endstatus = 'A'
            AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
        left join territorios.municipio mun ON mun.muncod = ende.muncod
        LEFT JOIN obras2.situacao_registro_documento srd     ON srd.esdid = doc2.esdid
        LEFT JOIN obras2.vm_termo_convenio_obras AS p_conv ON p_conv.obrid = obr.obrid
        WHERE 1 = 1
            " . (count($where) ? (' AND '.implode(' AND ',$where)) : "") . "
		    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "";
    
    $tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');
    $db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario",$tamanho);
?>
<script type="text/javascript">

$(document).ready(function (){
	$('.pesquisar').click(function (){
		$('#req').val('');
		$('#formListaObraCumprimento').submit();
	});
    $('#button_limpar').click(function() {
        $('#req').val('limpar');
        $('#formListaObraCumprimento').submit();
    });
});

$1_11(document).ready(function () {
    $1_11('select[name="strid[]"]').chosen();
    $1_11('select[name="prfid[]"]').chosen();
    $1_11('select[name="tooid[]"]').chosen();
    $1_11('select[name="estuf[]"]').chosen();
    $1_11('select[name="muncod[]"]').chosen();
    $1_11('select[name="esdid[]"]').chosen();
    $1_11('select[name="tpoid[]"]').chosen();
    $1_11('select[name="esdidsituaco[]"]').chosen();
    $1_11('select[name="tslid[]"]').chosen();

    $1_11(".estados").chosen().change(function (e, params) {
        values = $1_11(".estados").chosen().val();
        carregarMunicipio(values);
    });
});

function acessaCumprimento(obrid){
	location.href = '?modulo=principal/cadCumprimentoObjeto&acao=A&obrid=' + obrid;
}

function alterarObr( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function carregarMunicipio( estuf ){
	var td	= $('#td_municipio');
	if ( estuf != '' ){
		var url = location.href;
		$.ajax({
            url  		 : url,
            type 		 : 'post',
            data 		 : {ajax  : 'municipio', estuf : estuf},
            dataType   : "html",
            async		 : false,
            beforeSend : function (){
                divCarregando();
                td.find('select option:first').attr('selected', true);
            },
            error 	 : function (){
                divCarregado();
            },
            success	 : function ( data ){
                td.html( data );
                divCarregado();
            }
		});
	}else{
		td.find('select option:first').attr('selected', true);
		td.find('select').attr('selected', true).attr('disabled', true);
	}
}
</script>
<style>
    .chosen-container-multi {
        width: 400px !important;
    }

    .chosen-container-multi .chosen-choices {
        width: 400px !important;
    }

    label.btn.active {
        background-image: none;
        outline: 0;
        -webkit-box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        box-shadow: inset 0 3px 5px rgba(0, 0, 0, 0.125);
        color: #ffffff;
        background-color: #3276b1 !important;
        border-color: #285e8e;
    }
</style>
