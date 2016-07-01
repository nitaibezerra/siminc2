<?php 

//Filtra municípios
// checkAno();
if ($_REQUEST['filtraMunicipio'] && $_REQUEST['estuf']) {
	header('content-type: text/html; charset=ISO-8859-1');
	filtraMunicipio($_REQUEST['estuf']);
	exit;
}
header("Cache-Control: no-store, no-cache, must-revalidate");
header("Cache-Control: post-check=0, pre-check=0", false);
header("Pragma: no-cache");

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
$perfis = pegaPerfilGeral();
include  APPRAIZ . 'includes/cabecalho.inc';
echo '<br />';
$titulo = "PROJOVEM URBANO";
monta_titulo( $titulo, '' );
echo '<br />';
$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo('Lista de Municípios', 'Filtro de Pesquisa');

if( $_REQUEST['requisicao'] ){
	$_REQUEST['requisicao']();
	die();
}
// unset($_SESSION['projovemurbano']);
unset($_SESSION['projovemurbano']['estuf']);
unset($_SESSION['projovemurbano']['pjuid']);

$where = "";
if ($_POST){
	
	$where.= ($_POST["estuf"]) ? " AND m.estuf = '".$_POST["estuf"]."'" : "";
	$where.= ($_POST["muncod"]) ? " AND m.muncod = '".$_POST["muncod"]."'":"";
		
	if($_POST["esdid"]) {
		if($_POST["esdid"]==ESD_EMELABORACAO) $where.= " AND (esd.esdid = '".$_POST["esdid"]."' OR esd.esdid IS NULL) AND p.adesaotermo=TRUE";
		else $where.= " AND esd.esdid = '".$_POST["esdid"]."'";
	}
	
}
include_once APPRAIZ.'projovemurbano/modulos/principal/filtroAnoExercicio.inc';
?>

<?php if($_SESSION['projovemurbano']['ppuano'] && $_SESSION['projovemurbano']['ppuid']): ?>
	<?php if(in_array(616, $perfis) ||in_array(683, $perfis)): ?>
		
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
				<td class="SubTituloDireita" valign="top"><b>Situação:</b></td>
				<td id="td_municipio">
				<? 
					$esdid = ($esdid==''?$_REQUEST['esdid']:$esdid); 
					$sql = "SELECT
								esdid AS codigo,
								esddsc AS descricao
							FROM
								workflow.estadodocumento esd
							WHERE
								tpdid = '".TPD_PROJOVEMURBANO."' 
							ORDER BY esdordem"; 
					$db->monta_combo( "esdid", $sql, 'S', 'Selecione...', '', '', '', '215', 'N','esdid');
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
	<?php endif; ?>
	
	<?php
	
	$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemurbano.php?modulo=principal/instrucao&acao=A&muncod='||m.muncod||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
	$cancelar = "'-'";
	$reativar = "'-'";
	if(in_array(612,$perfis) || in_array(683, $perfis) ||
		in_array(616, $perfis) ){
		$cancelar = "" . ((! $habilitado) ? "'<img src=../imagens/excluir.gif border=0 width=13px
	                      title=\"Cancelar Adesão\" style=cursor:pointer;
	                      id='||p.pjuid ||' class=cancelar est = \"'||c.cmedesc||'\" />'" : "&nbsp;") . "";
		
		$reativar = "" . ((! $habilitado) ? "'<img src=../imagens/alterar.gif border=0 width=13px
	                      title=\"Reativar Adesão\" style=cursor:pointer;
	                      id='||p.pjuid ||' class=reativar est = \"'||c.cmedesc||'\" />'" : "&nbsp;") . "";
	}
	if(!$db->testa_superuser()) {
		if(in_array(646, $perfis)||(in_array(648, $perfis) && $_SESSION['projovemurbano']['ppuid'] != '1' )) {
			$inner_municipio = "inner join projovemurbano.usuarioresponsabilidade ur on ur.usucpf='".$_SESSION['usucpf']."' and ur.muncod=m.muncod AND rpustatus='A'";
			$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemurbano.php?modulo=principal/instrucao&acao=A&muncod='||m.muncod||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			$cancelar = "'-'";
			$reativar = "'-'";
		}elseif(in_array(648, $perfis)) {
			$inner_municipio = "inner join projovemurbano.projovemurbano pr on pr.muncod=m.muncod 
							    inner join projovemurbano.coordenadorresponsavel cr on pr.pjuid=cr.pjuid and cr.corcpf='".$_SESSION['usucpf']."'";
			$acao = "'<a href=\"#\" onclick=\"window.location=\'projovemurbano.php?modulo=principal/instrucao&acao=A&muncod='||m.muncod||'\';\" title=\"Ir para o termo de compromisso\"><img src=\"../imagens/alterar.gif\" width=\"13px\" height=\"16px\" style=\"cursor:pointer;\" border=\"0\"></a>'";
			$cancelar = "'-'";
			$reativar = "'-'";
		}
		
	}
	if(!in_array(694,$perfis) ){
		$case = "CASE
					WHEN p.pjustatus = 'A'
					THEN
						$acao||'&nbsp;&nbsp;'||$cancelar
				ELSE $reativar
				END as acao";
	}else{
		$case = "$acao as acao";
	}
	$vlr = '';
	if( $_SESSION['projovemurbano']['ppuid'] == '2' ){
		$vlr = " AND cmemeta::integer > 0 ";
	}
	
	$sqllista = '';
	$sqllista = "
            Select distinct 
            	$case,  
				CASE
					WHEN p.pjustatus = 'A'
					THEN m.estuf
					ELSE '<label style=\"color: red\">'||m.estuf||'</label>'
				END as estuf, 
                --m.mundescricao,
                CASE
					WHEN p.pjustatus = 'A'
					THEN c.cmedesc
					ELSE '<label style=\"color: red\">'||c.cmedesc||'</label>'
				END as cmedesc, 
                CASE
					WHEN p.pjustatus = 'I'
					THEN '<label style=\"color: red\">'||'Cancelado'||'</label>'
					ELSE
			   			CASE 
							WHEN esd.esddsc IS NULL 
							THEN 
								CASE 
									WHEN p.adesaotermo=TRUE THEN 'Em elaboração' 
									ELSE 'Sem adesão' 
								END 
							ELSE esd.esddsc 
						END 
				END as situacao,
				CASE WHEN ht.htddata IS NULL 
					THEN 'Não enviado' 
					ELSE to_char(ht.htddata,'dd/mm/YYYY HH24:MI')
				END as data
			From territorios.municipio m
			{$inner_municipio} 
			inner join projovemurbano.cargameta c on c.cmecodibge::character(7) = m.muncod and cmetipo in('C','M') 
			left join projovemurbano.projovemurbano p on p.muncod = c.cmecodibge::character(7) AND p.ppuid = {$_SESSION['projovemurbano']['ppuid']} 
            left join workflow.documento dd on dd.docid = p.docid
            left join workflow.historicodocumento ht on ht.hstid = dd.hstid
            left join workflow.estadodocumento esd on esd.esdid = dd.esdid
            WHERE 
	            c.ppuid = {$_SESSION['projovemurbano']['ppuid']}
            {$vlr}
            AND m.muncod is not null 
			$where
			ORDER BY 
				estuf, 
				cmedesc,
				situacao		
        ";
//        ver($sqllista,d);
	$db->monta_lista( $sqllista, array( "Ação", "UF", "Município","Situação","Data Tramitação"), 50, 10, 'N', 'center', '','', $tamanho,'','',array('ordena'=>false));
	
	?>
	<form id="form" enctype="multipart/form-data" name="form" method="POST" >
	<input type="hidden" value="" name="requisicao" id="requisicao" />
	<input type="hidden" value="" name="pjuid" id="pjuid" />
	<input type="hidden" value="" name="cmedesc" id="cmedesc" />
	</form>
	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script type="text/javascript">
	jQuery(document).ready(function() {
		 jQuery('.cancelar').live('click',function(){
		        var pjuid = jQuery(this).attr('id');
		        var cmedesc = jQuery(this).attr('est');
		        if ( confirm( "Deseja cancelar a adesão ao Projovem Urbano do Município de "+ cmedesc +"?" ) ) {
			        jQuery('#requisicao').val('cancelarAdesao');
			        jQuery('#pjuid').val(pjuid);
			        jQuery('#form').submit();
		     	}
		    });
		 jQuery('.reativar').live('click',function(){
		     var pjuid = jQuery(this).attr('id');
		     var cmedesc = jQuery(this).attr('est');
		     if ( confirm( "Deseja reativar a adesão ao Projovem Urbano do Município de "+ cmedesc +"?" ) ) {
			     jQuery('#requisicao').val('reativarAdesao');
			     jQuery('#pjuid').val(pjuid);
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
<?php endif; ?>
