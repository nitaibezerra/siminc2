<?php 

//Filtra municípios
if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}
 
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

if( $_REQUEST['requisicao'] ){
	$_REQUEST['requisicao']();
	die();
}
function filtraMunicipio($estuf){
	global $db;
	$sql = "SELECT
				ter.muncod AS codigo,
				ter.mundescricao AS descricao
			FROM
				territorios.municipio ter
			WHERE
				ter.estuf = '$estuf'
			ORDER BY ter.mundescricao";

	echo $db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','id="muncod"');

}

include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo = "PROJOVEM CAMPO";
monta_titulo( $titulo, '' );
echo '<br />';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo('Lista de Municípios', 'Filtro de Pesquisa');

// unset($_SESSION['projovemcampo']);
unset($_SESSION['projovemcampo']['estuf']);
unset($_SESSION['projovemcampo']['pjuid']);

$where = "";
if ($_POST){

	$where.= ($_POST["estuf"]) ? " AND m.estuf = '".$_POST["estuf"]."'" : "";
	$where.= ($_POST["muncod"]) ? " AND m.muncod = '".$_POST["muncod"]."'":"";
		
}
?>
		<form id="formPesquisaMun" name="formPesquisaMun" method="post" action="">
		<input type="hidden" name="municipio" id="municipio">
		<input type="hidden" name="termoCompromisso" id="termoCompromisso">
		<table id="pesquisa" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
			<tr>
				<td class="SubTituloDireita" valign="top"><b>UF:</b></td>
				<td>
					<?
						$estuf = ($uf==''?$_REQUEST['estuf']:$uf); 
						$sql = "SELECT
									estuf AS codigo,
									estdescricao AS descricao
								FROM
									territorios.estado
								ORDER BY
									estdescricao";
						$db->monta_combo( "estuf", $sql, 'S', 'Selecione...', 'filtraMunicipio', '', '', '215','','estuf' );
					?>
				</td>
			</tr>
			<tr>
				<td class="SubTituloDireita" valign="top"><b>Município:</b></td>
				<td id="td_municipio">
				<? 
					$muncod = ($muncod==''?$_REQUEST['muncod']:$muncod); 
					$sql = "SELECT
								ter.muncod AS codigo,
								ter.mundescricao AS descricao
							FROM
								territorios.municipio ter
							WHERE
								ter.estuf = '$estuf' 
							ORDER BY ter.estuf, ter.mundescricao"; 
					$db->monta_combo( "muncod", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','muncod');
				?>
				</td>
			</tr>
			<tr>
				<td bgcolor="#c0c0c0"></td>
				<td align="left" bgcolor="#c0c0c0">
					<input type="button" id="bt_pesquisar" value="Pesquisar" onclick="pesquisar()" />
				</td>
			</tr>
		</table>
		</form>

	<?php
	$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&muncod='||m.muncod||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
	$cancelar = "" . ((! $habilitado) ? "'<img src=../imagens/excluir.gif border=0 width=13px
                      title=\"Cancelar Adesão\" style=cursor:pointer; 
                      id='||c.apcid ||' class=cancelar mun = \"'||c.apcdesc||'\" />'" : "&nbsp;") . "";
	$reativar = "" . ((! $habilitado) ? "'<img src=../imagens/alterar.gif border=0 width=13px
                      title=\"Reativar Adesão\" style=cursor:pointer;
                      id='||c.apcid ||' class=reativar mun = \"'||c.apcdesc||'\" />'" : "&nbsp;") . "";
	if(!$db->testa_superuser()) {
	
		$perfis = pegaPerfilGeral();
	
		if(in_array(PFL_SECRETARIO_MUNICIPAL, $perfis) ) {
			$where = "AND c.apcstatus = 't'";
			$inner_municipio = "inner join projovemcampo.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.muncod=m.muncod AND rpustatus = 'A'";
			$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&muncod='||m.muncod||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>' ";
			$cancelar = "''";
			$reativar = "''";
		}elseif(in_array(PFL_COORDENADOR_MUNICIPAL, $perfis)) {
			$where = "AND c.apcstatus = 't'";
			$inner_municipio = "inner join projovemcampo.secretaria sec on sec.secaid = c.secaid and sec.secoordcpf='".$_SESSION['usucpf']."'";
			$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/indexPoloNucleo&acao=A&muncod='||m.muncod||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			$cancelar = "''";
			$reativar = "''";
		}elseif(in_array(PFL_CONSULTA,$perfis)){
			$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemcampo.php?modulo=principal/instrucao&acao=A&muncod='||m.muncod||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			$cancelar = "''";
			$reativar = "''";
		}
		
	}
	
	$sql = "select distinct
				CASE
					WHEN c.apcstatus = 'f'
						THEN $reativar
						ELSE $acao||'&nbsp;&nbsp;'||$cancelar
				END as acao,
				CASE
					WHEN c.apcstatus = 'f'
						THEN '<label style=\"color: red\">'||m.estuf||'</label>'
						ELSE m.estuf
				END as estuf,
				CASE
					WHEN c.apcstatus = 'f'
						THEN '<label style=\"color: red\">'||c.apcdesc||'</label>'
						ELSE c.apcdesc
				END as apcdesc,
	   			CASE
					WHEN c.apcstatus = 'f'
						THEN '<label style=\"color: red\">'||'Cancelado'||'</label>'
						ELSE
				   			CASE 
								WHEN esd.esddsc IS NULL THEN 
									CASE 
										WHEN c.apctermoaceito=TRUE THEN 'Em elaboração' 
										ELSE 'Sem adesão' 
									END 
								ELSE esd.esddsc
							END 
				END as situacao,
				CASE WHEN ht.htddata IS NULL THEN 'Não enviado' ELSE to_char(ht.htddata,'dd/mm/YYYY HH24:MI') END as data
	 	FROM territorios.municipio as m
		INNER JOIN projovemcampo.adesaoprojovemcampo c ON c.apccodibge::character(7) = m.muncod and apcesfera in ('M') 
	    $inner_municipio						
		left join workflow.documento dd on dd.docid = c.docid
		left join workflow.historicodocumento ht on ht.hstid = dd.hstid
		left join workflow.estadodocumento esd on esd.esdid = dd.esdid
		WHERE 1 > 0
				{$where}
				
		order by estuf,apcdesc";
	$alinhamento	= array( 'center','center','center','center','center' );
	$db->monta_lista( $sql, array( "Ação", "UF",  "Municipio" , "Situação","Data Tramitação" ), 30, 10, 'N', 'center', '','', $tamanho,$alinhamento);
	
	?>
	<form id="form" enctype="multipart/form-data" name="form" method="POST" >
	<input type="hidden" value="" name="requisicao" id="requisicao" />
	<input type="hidden" value="" name="apcid" id="apcid" />
	</form>
	
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script type="text/javascript">

	jQuery(document).ready(function() {
		 jQuery('.cancelar').live('click',function(){
		        var apcid = jQuery(this).attr('id');
		        var municipio = jQuery(this).attr('mun');
		        if ( confirm( "Deseja cancelar a adesão ao Projovem Campo do município de "+ municipio +"?" ) ) {
			        jQuery('#requisicao').val('cancelarAdesao');
			        jQuery('#apcid').val(apcid);
			        jQuery('#form').submit();
		     	}
		    });
		 jQuery('.reativar').live('click',function(){
		     var apcid = jQuery(this).attr('id');
		     var municipio = jQuery(this).attr('mun');
		     if ( confirm( "Deseja reativar a adesão ao Projovem Campo do município de "+ municipio +"?" ) ) {
			     jQuery('#requisicao').val('reativarAdesao');
			     jQuery('#apcid').val(apcid);
			     jQuery('#form').submit();
		     }
		 });
	});
	function filtraMunicipio(estuf) {
		if(estuf!=''){
			var destino = document.getElementById("td_municipio");
			var myAjax = new Ajax.Request(
				window.location.href,
				{
					method: 'post',
					parameters: "filtraMunicipio=true&" + "estuf=" + estuf,
					asynchronous: false,
					onComplete: function(resp) {
						if(destino) {
							destino.innerHTML = resp.responseText;
						} 
					},
					onLoading: function(){
						destino.innerHTML = 'Carregando...';
					}
				});
		}
	}
	
	
	var btPesquisa	= document.getElementById("bt_pesquisar");
	
	function pesquisar() {
		btPesquisa.disabled = true;
		document.formPesquisaMun.submit();
	}
	
	
	function cadTermo(muncod) {
		document.getElementById("municipio").value=muncod;
		document.getElementById("termoCompromisso").value=1;
		document.formPesquisaMun.submit();
	}
	</script>

