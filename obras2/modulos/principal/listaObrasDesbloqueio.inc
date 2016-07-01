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

switch ($_REQUEST['ajax']){
	case 'municipio':
		header('content-type: text/html; charset=ISO-8859-1');

		$municipio = new Municipio();
		echo $db->monta_combo("muncod", $municipio->listaCombo( array('estuf' => $_POST['estuf']) ), 'S', 'Selecione...', '', '', '', 200, 'N', 'muncod');
		exit;		
}

//Chamada de programa
include  APPRAIZ."includes/cabecalho.inc";
echo "<br>";

$arAba = getArAba('listaorgaodesbloqueio');
echo montarAbasArray($arAba, "?modulo=principal/listaObrasDesbloqueio&acao=A&orgid=" . $orgid);

monta_titulo( 'Lista de Obras Desbloqueio', 'Filtre as Obras');

extract( $_POST );
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
			<td class="SubTituloDireita">UF:</td>
			<td>
			<?php
			$uf = new Estado();					
			$db->monta_combo("estuf", $uf->listaCombo(), 'S','Selecione...','carregarMunicipio', '', '',200,'N','estuf'); 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Município:</td>
			<td id="td_municipio">
			<?php 
			if ($estuf){
				$municipio = new Municipio();
				$dado 	   = $municipio->listaCombo( array('estuf' => $estuf) );	
				$habMun    = 'S';
			}else{
				$dado   = array();
				$habMun = 'N';
			}		
			$habMun = ($disable == 'N' ? $disable : $habMun);
			echo $db->monta_combo("muncod", $dado, $habMun,'Selecione...','', '', '',200,'N','muncod'); 
			?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" style="width: 190px;">Situação:</td>
			<td>
				<input type="radio" name="situacaobloqueio" id="" value="N" <?=( $_POST["situacaobloqueio"] == "N" ? "checked='checked'" : "" ) ?>/> Não avaliado
				<input type="radio" name="situacaobloqueio" id="" value="D" <?=( $_POST["situacaobloqueio"] == "D" ? "checked='checked'" : "" ) ?>/> Deferido
				<input type="radio" name="situacaobloqueio" id="" value="I" <?=( $_POST["situacaobloqueio"] == "I" ? "checked='checked'" : "" ) ?>/> Indeferido
				<input type="radio" name="situacaobloqueio" id="" value=""  <?=( $_POST["situacaobloqueio"] == ""  ? "checked='checked'" : "" ) ?> /> Todas
			</td>
		</tr>
		<tr>
			<td style="background-color:#DCDCDC" width="15%" colspan="2" align="center">
				<input type="button" name="pesquisar" class="pesquisar" value="Pesquisar"/>
			</td>
		</tr>
	</table>
</form>
<?php
$where = array("obrstatus = 'A'");

if ( $obrbuscatexto ){
	$where[] = " ( UPPER(public.removeacento(o.obrnome)) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') OR
				   public.removeacento(o.obrid::CHARACTER VARYING) ILIKE ('%" . removeAcentos($obrbuscatexto) . "%') ) ";
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

$where[] = "emp.orgid = " . $orgid;

switch ($situacaobloqueio){
	case 'N': 
		$where[] = "d.destipodesbloqueio IS NULL OR d.destipodesbloqueio = ''";
		break;
	case 'D': 
		$where[] = "d.destipodesbloqueio = 'D'";
		break;
	case 'I': 
		$where[] = "d.destipodesbloqueio = 'I'";
		break;
}

$sql = "SELECT
			'<center>
				<img
	 				align=\"absmiddle\"
	 				src=\"/imagens/alterar.gif\"
	 				style=\"cursor: pointer\"
	 				onclick=\"javascript: desbloqueioObr(\'' || o.obrid || '\', \'' || p.pdoid  || '\');\"
	 				title=\"Alterar Obra\">
	 		 </center>' AS acao,
	 		o.obrid,
			'<a href=\"javascript: alterarObr(\'' || o.obrid || '\');\">(' || o.obrid || ') ' || o.obrnome || '</a>' as descricao,
			ende.estuf,
			mun.mundescricao,
			p.pdojustificativa,
			p.usunome AS usunomepedido,
			TO_CHAR(p.pdodatainclusao, 'dd/mm/YYYY') AS pdodatainclusao,
		
			d.deferimento,
			d.usunome AS usunomedesbloqueio,
			TO_CHAR(d.desdata, 'dd/mm/YYYY') AS desdata,
			TO_CHAR(d.desdatainicio, 'dd/mm/YYYY') AS desdatainicio,
			d.desdias,
			TO_CHAR(d.destermino, 'dd/mm/YYYY') AS destermino
		FROM
			obras2.obras o
		INNER JOIN obras2.empreendimento emp on emp.empid = o.empid
		LEFT JOIN entidade.endereco 	ende ON ende.endid = o.endid AND 
									        	ende.endstatus = 'A' AND
									        	ende.tpeid = " . TIPO_ENDERECO_OBJETO . "
		LEFT JOIN territorios.municipio mun ON mun.muncod = ende.muncod
		JOIN (
			SELECT
				pdoid,
				obrid,
				pdojustificativa,
				u.usunome,
				pdodatainclusao
			FROM
				obras2.pedidodesbloqueioobra p
			JOIN seguranca.usuario u ON u.usucpf = p.usucpf
			WHERE
				pdostatus = 'A' and 
				pdoid IN (SELECT
							MAX(pdoid)
						  FROM
							obras2.pedidodesbloqueioobra a
						  WHERE
							pdostatus = 'A' and a.obrid = p.obrid )
		     ) p ON p.obrid = o.obrid 
		LEFT JOIN (
			SELECT
				pdoid,
				desid,
				CASE 
					WHEN d.destipodesbloqueio IS NULL OR d.destipodesbloqueio = '' THEN 'Não analisado'
					WHEN d.destipodesbloqueio = 'D' THEN 'Deferido'
					WHEN d.destipodesbloqueio = 'I' THEN 'Indeferido'
				END AS deferimento,
				u.usunome,
				desdata,
				destermino,
				desdias,
				desdatainicio,
				d.destipodesbloqueio
			FROM
				obras2.desbloqueioobra d
			JOIN seguranca.usuario u ON u.usucpf = d.usucpf
			WHERE
				desid IN (SELECT
							MAX(desid)
						  FROM
							obras2.desbloqueioobra where pdoid = d.pdoid)
		     ) d ON d.pdoid = p.pdoid 
		WHERE " . 
			(count($where) ? implode(' AND ',$where) : "");

$cabecalho = array('Ação', 'ID', 'Obra', 'UF', 'Município', 'Justificativa do Desbloqueio', 'Inserido Por', 'Data de Cadastro',
				   'Situação do Deferimento', 'Inserido Por', 'Data de Cadastro do Deferimento', 'Data de Início', 'Dias', 'Data de Término');			
$db->monta_lista($sql,$cabecalho,100,5,'N','center',$par2, "formulario");
?>
<script type="text/javascript">
<!--
$(document).ready(function (){
	$('.pesquisar').click(function (){
		$('#req').val('');
		$('#formListaObraDesbloqueio').submit();
	});
});

function desbloqueioObr(obrid, pdoid){
	windowOpen('?modulo=principal/dadosDesbloqueio&acao=A&obrid=' + obrid + '&pdoid=' + pdoid,'telaDesbloqueio','height=700,width=700,status=yes,toolbar=no,menubar=no,scrollbars=yes,location=no,resizable=yes' );
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
			  data 		 : {ajax  : 'municipio', 
			  		  	    estuf : estuf},
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
		td.find('select').attr('selected', true)
						 .attr('disabled', true);
	}			
}
//-->
</script>