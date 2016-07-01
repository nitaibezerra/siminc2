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

<form method="post" name="formListaObraDesbloqueio" id="formListaObraDesbloqueio">
	<input type="hidden" name="req" id="req" value="">
	<input type="hidden" name="obrid" id="obrid" value="">
	<input type="hidden" name="empid" id="empid" value="">
	<table align="center" bgcolor="#f5f5f5" border="0" class="tabela" cellpadding="3" cellspacing="1">
		<tr>
			<td class="SubTituloDireita" width="15%">Nome da Obra / ID:</td>
			<td>
				<?=campo_texto('obrbuscatexto','N','S','',70,100,'','', '', '', '', 'id="obrbuscatexto"');?>
			</td>
		</tr>
        <tr>
            <td class="SubTituloDireita" width="15%">ID da Solicitação:</td>
            <td>
                <input type="text" name="slcid" size="71" maxlength="100" value="<?=$slcid?>" onmouseover="MouseOver(this);" onfocus="MouseClick(this);this.select();" onmouseout="MouseOut(this);" onblur="MouseBlur(this);" id="sldid" title="" class=" normal">
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Situação do Obra:</td>
            <td>
                <select name="esdid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    $sql = "SELECT esdid as codigo, esddsc as descricao FROM workflow.estadodocumento WHERE tpdid='" . TPDID_OBJETO . "' AND esdstatus='A' ORDER BY esdordem";
                    $dados = $db->carregar($sql);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($esdid && in_array($key['codigo'], $esdid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Tipologia:</td>
            <td>
                <select name="tpoid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php   $tipologiaObra = new TipologiaObra();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    $dados = $tipologiaObra->listaCombo($param, false);
                    foreach ($dados as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($tpoid && in_array($key['codigo'], $tpoid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Programa:</td>
            <td>
                <select name="prfid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $programa = new ProgramaFonte();
                    $param = array("orgid" => $_SESSION['obras2']['orgid']);
                    foreach ($programa->listacombo($param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($prfid && in_array($key['codigo'], $prfid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
        <tr>
            <td class="SubTituloDireita">Fonte:</td>
            <td>
                <select name="tooid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php  $tipoOrigemObra = new TipoOrigemObra();
                    $param = array();
                    foreach ($tipoOrigemObra->listaCombo(true, $param, false) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($tooid && in_array($key['codigo'], $tooid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
            </td>
        </tr>
		<tr>
			<td class="SubTituloDireita">Esfera:</td>
			<td>
				<?php 
				$sql = Array(Array('codigo'=>'E', 'descricao'=>'Estadual'),
							 Array('codigo'=>'M', 'descricao'=>'Municipal'));
				$db->monta_combo('empesfera',$sql, 'S','Selecione...','','','',200,'N', 'empesfera');
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">UF(s):</td>
			<td>
                <select name="estuf[]" class="chosen-select estados" multiple data-placeholder="Selecione">
                <?php
                $uf = new Estado();
                foreach($uf->listaCombo() as $key) {
                    ?>
                    <option
                        value="<?php echo $key['codigo'] ?>" <?php if ($estuf && in_array($key['codigo'], $estuf)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                <?php } ?>
                </select>
			</td>
		</tr>
        <tr>
			<td class="SubTituloDireita">Município(s):</td>
			<td id="td_municipio">
                <?if($estuf):?>
                <select name="muncod[]" class="chosen-select municipios" multiple data-placeholder="Selecione">
                    <?php   $municipio = new Municipio();
                    foreach ($municipio->listaComboMulti($estuf) as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($muncod && in_array($key['codigo'], $muncod)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
                <?endif;?>
			</td>
		</tr>
        <? if($permissaoFiltro): ?>
        <tr>
			<td class="SubTituloDireita" style="width: 190px;">Situação:</td>
			<td>
                <?php
                $dado = $db->carregar('SELECT esdid codigo, esddsc descricao FROM workflow.estadodocumento WHERE tpdid = '.TPDID_SOLICITACOES.' ');
                ?>
                <select name="esdidsituaco[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($esdidsituaco && in_array($key['codigo'], $esdidsituaco)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
			</td>
		</tr>
        <? endif; ?>
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
            <td class="SubTituloDireita" style="width: 190px;">% Execução:</td>
            <td>
                <table>
                    <tr>
                        <th>Mínimo</th>
                        <th>Máximo</th>
                    </tr>
                    <tr>
                        <?php
                        for ($i = 0; $i <= 100; $i++) {
                            $arPercentual[] = array('codigo' => "$i", 'descricao' => "$i%");
                        }
                        $percentualinicial = $_POST['percentualinicial'];
                        $percentualfinal = $_POST['percentualfinal'];
                        $percfinal = $percentualfinal == '' ? 100 : $percentualfinal;
                        print '<td>';
                        $db->monta_combo("percentualinicial", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualinicial');
                        print '</td><td>';
                        $db->monta_combo("percentualfinal", $arPercentual, 'S', '', 'validarPercentual', '', '', '', 'N', 'percentualfinal', false, $percfinal);
                        print '</td>';
                        ?>
                    </tr>
                </table>
            </td>
        </tr>
		<tr>
			<td class="SubTituloDireita" style="width: 15%;">Tipo solicitação:</td>
			<td>
                <?php
                $dado = $db->carregar('SELECT tslid as codigo, tslnome as descricao FROM obras2.tiposolicitacao WHERE tslstatus = \'A\' ');
                ?>
                <select name="tslid[]" class="chosen-select" multiple data-placeholder="Selecione">
                    <?php
                    foreach ($dado as $key) {
                        ?>
                        <option
                            value="<?php echo $key['codigo'] ?>" <?php if ($tslid && in_array($key['codigo'], $tslid)) { ?> <?php echo "selected='selected'"; ?> <?php } ?>><?php echo $key["descricao"] ?></option>
                    <?php } ?>
                </select>
			</td>
		</tr>
        <? if($permissaoFiltro): ?>
        <tr>
            <td class="SubTituloDireita" style="width: 15%;">Estado:</td>
            <td>
                <input type="radio" name="solicitacao" id="" value="S" <?=$solicitacao == 'S' ? 'checked=checked' : ''?>> Aprovado
                <input type="radio" name="solicitacao" id="" value="N" <?=$solicitacao == 'N' ? 'checked=checked' : ''?>> Não Aprovado
                <input type="radio" name="solicitacao" id="" value="" <?= !isset($solicitacao) ? 'checked=checked' : ''?>> Todas
            </td>
        </tr>
        <? endif; ?>

		<tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
                <input id="button_limpar" type="button" value="Limpar Filtros"/>
			</td>
		</tr>
	</table>
</form>
<?php
$where = array("obrstatus = 'A'");
$where[] = "emp.orgid = " . $orgid;
if ( $obrbuscatexto ){
	$where[] = " ( UPPER(public.removeacento(o.obrnome)) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') OR
        public.removeacento(o.obrid::CHARACTER VARYING) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') ) ";
}

if( $slcid ){
	$where[] = "p.slcid = " .$slcid;
}

if($esdid) {
    $esdid = (array) $esdid;
    $where[] = "dc.esdid IN('" . implode("', '", $esdid) . "')";
}

if($tpoid) {
    $tpoid = (array) $tpoid;
    $where[] = "o.tpoid IN('" . implode("', '", $tpoid) . "')";
}

if($prfid) {
    $prfid = (array) $prfid;
    $where[] = "emp.prfid IN('" . implode("', '", $prfid) . "')";
}

if($tooid) {
    $tooid = (array) $tooid;
    $where[] = "emp.tooid IN('" . implode("', '", $tooid) . "')";
}

if( $empesfera ){
	$empesfera = (array) $empesfera;
	$where[] = "emp.empesfera IN('" . implode("', '", $empesfera) . "')";
}

if( $estuf ){
	$estuf = (array) $estuf;
	$where[] = "ende.estuf IN('" . implode("', '", $estuf) . "')";
}

if( $muncod ){
	$muncod = (array) $muncod;
	$where[] = "ende.muncod IN('" . implode("', '", $muncod) . "')";
}

if($esdidsituaco){
	$esdidsituaco = (array) $esdidsituaco;
    $where[] = "p.esdid IN('" . implode("', '", $esdidsituaco) . "')";
}

if ($convenio) {
    $where[] = "p_conv.termo_convenio = '".$convenio."'";
}

if ($ano_convenio) {
    $where[] = "p_conv.ano_termo_convenio = '".$ano_convenio."'";
}

if ($percentualinicial != '') {
    $where[] = "COALESCE(o.obrpercentultvistoria, 0) >= " . $percentualinicial;
}

if ($percentualfinal != '') {
    if ($percentualfinal < 100) {
        $where[] = "COALESCE(o.obrpercentultvistoria, 0) <= " . $percentualfinal;
    }
}

if($tslid) {
    $tslid = (array) $tslid;
    $where[] = "p.slcid IN (SELECT slcid FROM obras2.tiposolicitacao_solicitacao WHERE tslid IN (".implode(", ", $tslid).")) ";
}

if($solicitacao){
    $where[] = "p.aprovado = '$solicitacao'";
    if($solicitacao == 'N'){
        $where[] = "p.esdid IN(".ESDID_SOLICITACOES_DEFERIDO.",".ESDID_SOLICITACOES_INDEFERIDO.",".ESDID_SOLICITACOES_DILIGENCIA.")";
    }
}
$cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Obra', 'UF', 'Município', "Tipo Solicitação", 'Justificativa', 'Inserido Por', 'Data de Cadastro',
				   'Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação', 'Estado');
$aprovado = ", CASE WHEN p.aprovado = 'S' THEN '<b style=\"color:green;\">APROVADO</b>' WHEN p.reprovado = 'S' THEN '<b style=\"color:red;\">REPROVADO</b>' WHEN (p.aprovado = 'N' AND p.esdid IN(".ESDID_SOLICITACOES_DEFERIDO.",".ESDID_SOLICITACOES_INDEFERIDO.")) THEN '
<button type=\"button\" onclick=\"aprovaSolicitacao(this,'||p.slcid||')\">APROVAR</button>
<button type=\"button\" style=\"background-color:#EF3939;border-color:#F00;margin-top:10px;\" onclick=\"reprovaSolicitacao(this,'||p.slcid||')\">REPROVAR</button>' ELSE '' END AS aprovado";
$situacao = "p.esddsc";

if (!possui_perfil(array(PFLCOD_SUPER_USUARIO))) {
    $cabecalho = array('Ação', 'ID Solicitação', 'ID Obra', 'Obra', 'UF', 'Município', "Tipo Solicitação", 'Justificativa', 'Inserido Por', 'Data de Cadastro','Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Observação');
    $aprovado = "";
    $situacao = "CASE WHEN p.aprovado = 'N' THEN (CASE WHEN p.esdid = ".ESDID_SOLICITACOES_CADASTRAMENTO." THEN '<b>Em Cadastramento</b>' WHEN p.esdid = ".ESDID_SOLICITACOES_DILIGENCIA." THEN '<b>Diligência</b>' ELSE '<b>Aguardando Análise</b>' END) ELSE '<b style=\"color:green;\">'||p.esddsc||'</b>' END AS esddsc";

    $arrUni = Array(PFLCOD_GESTOR_UNIDADE);

    $resp = array();
    $arPflcod = array();
    $orWhere = array();
    if (possui_perfil($arrUni)) {
        $arPflcod = array_merge($arPflcod, $arrUni);
        $orWhere['entidunidade'] = "emp.entidunidade IN ( SELECT urs.entid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    urs.entid = emp.entidunidade)";
    }

    if (possui_perfil(Array(PFLCOD_SUPERVISOR_UNIDADE))) {
        $usuarioResp = new UsuarioResponsabilidade();
        $arEmpid = $usuarioResp->pegaEmpidPermitido($_SESSION['usucpf']);
        $arEmpid = ($arEmpid ? $arEmpid : array(0));

        $arPflcod[] = PFLCOD_SUPERVISOR_UNIDADE;
        $arPflcod[] = PFLCOD_CONSULTA_UNIDADE;

        $orWhere['sup'] = "emp.empid IN ( SELECT urs.empid FROM obras2.usuarioresponsabilidade urs WHERE urs.rpustatus = 'A' AND
                                                                    urs.usucpf = '" . $_SESSION['usucpf'] . "' AND
                                                                    urs.pflcod IN (" . implode(', ', $arPflcod) . ") AND
                                                                    /*urs.entid = e.entidunidade AND*/ urs.empid IN('" . implode("', '", $arEmpid) . "'))";
    }

}

$sql = "

        WITH tmp_obras2_solicitacao AS (
                    SELECT
                        slcid,
                        obrid,
                        slcjustificativa,
                        u.usunome,
                        slcdatainclusao,
                        ed.esddsc,
                        ed.esdid,
                        p.aprovado,
                        cd.cmddsc,
                        p.reprovado,
                        u2.usunome AS usunomedesbloqueio,
                        htd.htddata
                    FROM obras2.solicitacao p
                    LEFT JOIN workflow.documento d ON d.docid = p.docid
                    LEFT JOIN workflow.estadodocumento ed ON ed.esdid = d.esdid
                    LEFT JOIN workflow.comentariodocumento cd ON (d.hstid = cd.hstid AND cd.cmdstatus = 'A')
                    LEFT JOIN workflow.historicodocumento htd ON (d.hstid = htd.hstid)
                    LEFT JOIN seguranca.usuario u2 ON (htd.usucpf = u2.usucpf)
                    JOIN seguranca.usuario u ON u.usucpf = p.usucpf
                    WHERE slcstatus = 'A'
            )

        SELECT
			'<center>
                <div style=\"width:100%;\">
                    <img align=\"absmiddle\" src=\"/imagens/icone_lupa.png\" style=\"cursor: pointer\" onclick=\"javascript: desbloqueioObr(\'' || o.obrid || '\', \'' || p.slcid  || '\');\"
                        title=\"Visualizar Solicitação\"> 
                    <img align=\"absmiddle\" src=\"/imagens/alterar.gif\" style=\"cursor: pointer\" onclick=\"javascript: acessaSolicitacao(\'' || o.obrid || '\');\"
                        title=\"Acessar Solicitações\">
                </div>
	 		 </center>' AS acao,
            p.slcid,
	 		o.obrid,
			'<a href=\"javascript: alterarObr(\'' || o.obrid || '\');\">(' || o.obrid || ') ' || o.obrnome || '</a>' as dslcricao,
			ende.estuf,
			mun.mundescricao,
			(SELECT array_to_string(array_agg(tslnome), ',') FROM obras2.tiposolicitacao_solicitacao ts
                JOIN obras2.tiposolicitacao t ON t.tslid = ts.tslid
                WHERE ts.slcid = p.slcid),
			p.slcjustificativa,
			p.usunome AS usunomepedido,
			TO_CHAR(p.slcdatainclusao, 'dd/mm/YYYY') AS slcdatainclusao,
			$situacao,
			p.usunomedesbloqueio,
			TO_CHAR(p.htddata, 'dd/mm/YYYY') AS dsldata,
			p.cmddsc as observacao
            $aprovado
		FROM obras2.obras o
        LEFT JOIN (SELECT * FROM obras2.vm_termo_convenio_obras WHERE termo_convenio IS NOT NULL) as p_conv ON p_conv.obrid = o.obrid
        LEFT JOIN workflow.documento dc ON (o.docid = dc.docid)
		INNER JOIN obras2.empreendimento emp on emp.empid = o.empid
		LEFT JOIN entidade.endereco ende ON ende.endid = o.endid
            AND ende.endstatus = 'A'
            AND ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
		LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
		JOIN tmp_obras2_solicitacao p ON p.obrid = o.obrid
		WHERE
            " . (count($where) ? implode(' AND ',$where) : "") . "
		    " . (count($orWhere) ? ' AND (' . implode(' OR ', $orWhere) . ')' : "") . "
		";
$tamanho = Array('4%', '', '', '', '', '', '', '', '', '', '');
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario",$tamanho);
?>
<script type="text/javascript">

$(document).ready(function (){
	$('.pesquisar').click(function (){
		$('#req').val('');
		$('#formListaObraDesbloqueio').submit();
	});
    $('#button_limpar').click(function() {
        $('#req').val('limpar');
        $('#formListaObraDesbloqueio').submit();
    });
});

$1_11(document).ready(function () {
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

function desbloqueioObr(obrid, slcid){
	windowOpen('?modulo=principal/solicitacao&acao=A&obrid=' + obrid + '&slcid=' + slcid,'telaDesbloqueio','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
}

function acessaSolicitacao(obrid){
	location.href = '?modulo=principal/solicitacoes&acao=A&obrid=' + obrid;
}

function alterarObr( obrid ){
	location.href = '?modulo=principal/cadObra&acao=A&obrid=' + obrid;
}

function aprovaSolicitacao(objeto,slcid){
    if(confirm('Você deseja prosseguir com a aprovação?')){
        jQuery.ajax({
            type: "POST",
            url: 'obras2.php?modulo=principal/solicitacoes&acao=A',
            data: "requisicao=aprova_solicitacao&slcid=" + slcid,
            async: false,
            dataType : 'json',
            success: function(res) {
                alert(res.mensagem);
                if(res.retorno){
                    $(objeto).parent().html('<b style="color:green;">APROVADO</b>');
                }

            }
        });
    }
}

function reprovaSolicitacao(objeto,slcid){
    if(confirm('Você deseja prosseguir com a reprovação?')){
        jQuery.ajax({
            type: "POST",
            url: 'obras2.php?modulo=principal/solicitacoes&acao=A',
            data: "requisicao=reprova_solicitacao&slcid=" + slcid,
            async: false,
            dataType : 'json',
            success: function(res) {
                alert(res.mensagem);
                location.reload();
            }
        });
    }
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
