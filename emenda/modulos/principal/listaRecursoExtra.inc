<?php

$perfilEmenda = pegaPerfilArray( $_SESSION["usucpf"] );

if($_REQUEST['requisicao'] == 'verificaIndicacoesSIOP'){
	header('content-type: text/html; charset=ISO-8859-1');
		
	$sql = "select
				coalesce(count(ede.enbid), 0) as total,
			    e.emecod,
			    case when e.emerelator = 'S' then a.autnome||' - Relator Geral' else a.autnome end as autnome
			from
				emenda.emenda e
				inner join emenda.emendadetalhe ed on ed.emeid = e.emeid
			    left join emenda.emendadetalheentidade ede on ede.emdid = ed.emdid and ede.edestatus = 'A'
			    inner join emenda.autor a on a.autid = e.autid and a.autstatus = 'A'
			where
				ed.emeid = {$_REQUEST['emeid']}
			group by
				e.emecod, e.emerelator, a.autnome";
	$arEmenda = $db->pegaLinha($sql);
	
	if( in_array( ADMINISTRADOR_INST, $perfilEmenda) || in_array( SUPER_USUARIO, $perfilEmenda) ){
		echo 'tem';
	} else {
		if( (int)$arEmenda['total'] > (int)0 ){
			echo 'tem';
		} else {
			echo "É necessário fazer a indicação no sistema SIOP. <br>Até o momento não existem indicações para: <br><b>Emenda:</b> {$arEmenda['emecod']}, <br><b>Parlamentar:</b> {$arEmenda['autnome']}";
		}
	}
	exit;
}

if($_REQUEST['unidAjax'] == '1'){
	header('content-type: text/html; charset=ISO-8859-1');
	listaUnidade();
	exit;
}
if($_REQUEST['carregaEmendaDetalhe']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaEmendasDetalhe( $_POST["emeid"] );
	exit;
}
if($_REQUEST['carregaEmendaDetalheEntidade']){
	header('content-type: text/html; charset=ISO-8859-1');
	listaDetalheEntidade( $_POST["emeid"] );
	exit;
}
unset( $_SESSION["emenda"]["emeid"] );

if($_SESSION['emenda']['emetipo'] == 'E'){
	$tipo = 'emenda';
} else {
	$tipo = 'extra';
}

/*if ( $_REQUEST["carga"] ){
	listaEmendasDetalhe( $_REQUEST["carga"] );
	die;
}

if ( $_REQUEST["cargaEntidade"] ){
	listaDetalheEntidade( $_REQUEST["cargaEntidade"] );
	die;
}*/

function listaUnidade(){
	global $db;
	
	$sql = "SELECT DISTINCT 
								u.unicod as codigo,
								u.unidsc as descricao
							FROM
								emenda.emenda e
							INNER JOIN
								emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							INNER JOIN
								public.unidade u ON u.unicod = f.unicod";
					
	$db->monta_combo("unicod", $sql, "S", "Todas",'','', '', '250', 'N','unicod');
	exit;
}

switch ($_REQUEST["requisicao"]) {
	
	case 'pesquisa':
		$filtros = geraFiltroEmendas( $_REQUEST );
	break;
	case 'excluir':
		if( !verificaEmendaDetalhePTA($_POST['emeid']) ){
			$sql = "SELECT emdid FROM emenda.emendadetalhe WHERE emeid = {$_POST['emeid']}";					  
			$arEmdid = $db->pegaUm($sql);
			
			$sql = "DELETE FROM emenda.iniciativadetalheentidade WHERE edeid in (select edeid from emenda.emendadetalheentidade where emdid  = $arEmdid)";
			$db->executar($sql);
			
			$sql = "DELETE FROM emenda.emendadetalheentidade WHERE emdid = $arEmdid";
			$db->executar($sql);
			
			$sql = "DELETE FROM emenda.iniciativaemendadetalhe WHERE emdid = $arEmdid";
			$db->executar($sql);

			$sql = "DELETE FROM emenda.emendadetalhe WHERE emeid = {$_POST['emeid']}";
			$db->executar($sql);

			$sql = "DELETE FROM emenda.emenda WHERE emeid = {$_POST['emeid']}";
			$db->executar($sql);

			if($db->commit() == 1){
				//$db->sucesso('principal/listaRecursoExtra');
				echo "<script>
					  	  alert('Registro excluido com sucesso!');
					  	  window.location.href = 'emenda.php?modulo=principal/listaDeExtraOrcamento&acao=A';
					  </script>";
				exit();
			}
		} else {
			echo "<script>
			  	  alert('Operação não realizada, esta emenda esta vinculada com o plano de trabalho!');
			  	  window.location.href = 'emenda.php?modulo=principal/listaDeExtraOrcamento&acao=A';
			  </script>";
			exit();
		}
	break;	
}

function verificaEmendaDetalhePTA($emeid){
	global $db;
	
	$sql = "SELECT
				count(e.emeid)
			FROM
				emenda.emenda e
			    inner join emenda.emendadetalhe ed
			    	on ed.emeid = e.emeid
			    inner join emenda.emendadetalheentidade ede
			    	on ede.emdid = ed.emdid
			    inner join emenda.ptemendadetalheentidade pede
			    	on pede.edeid = ede.edeid
			WHERE
				e.emeid = $emeid";

	if($db->pegaUm($sql) != 0){
		return true;
	} else {
		return false;
	}
	
}

if( !$db->testa_superuser() && empty($filtros) ) {
	$sql = "select count(a.arqid) as c
		 	from emenda.anexo de
			inner join emenda.planotrabalho pt on pt.ptrid = de.ptrid
			inner join emenda.entidadebeneficiada eb on eb.enbid = pt.enbid
			inner join public.arquivo a on a.arqid=de.arqid 
			-- inner join emenda.emendadetalheentidade ee on ee.enbid = eb.enbid
			inner join emenda.usuarioresponsabilidade ur on ur.enbid = eb.enbid
			inner join seguranca.usuario u ON u.usucpf = ur.usucpf 
			where a.arqid/1000 between 647 and 725 and 
			a.arqid not in(select arqid from public.arquivo_recuperado) and a.sisid=57 and a.usucpf='".$_SESSION['usucpf']."'";
	
	$carq = $db->pegaUm($sql);
}
if($carq > 0 ) {
	$texto = "<center>
				<img src=\"../imagens/alerta_sistema.gif\" />
			    <p><font size=3 color=red><b>Nota do Sistema!</b></font></p>
           	     <p style=\"font-weight:bold\" ><font size=3>Alguns arquivos anexados por você no SIMEC foram corrompidos,<br />
           	     para corrigir o problema você poderá enviá-los novamente.<br />
           	     Clique abaixo para ver a relação e proceder a correção.</p>
           	     <input type=\"button\" value=\"Ver a relação de arquivos\" style=\"cursor:pointer;font:16px Trebuchet Ms,Arial,Tahoma,Verdana,Helvetica,Sans-Serif;height:33px\" onclick=\"window.location='emenda.php?modulo=principal/emenda_arquivo&acao=A'\" />
          	  </center>";
	popupAlertaGeral($texto,"580px","260px");
}

require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( $titulo_modulo, '');

if( in_array( AUTOR_EMENDA, $perfilEmenda) && $_SESSION['exercicio'] < date('Y') ){
	
	$texto = "<div>
				<center>
				<img src=\"../imagens/alerta_sistema.gif\" />
			    <p><font size=3 color=red><b>Nota do Sistema!</b></font></p></center>
           	     <p style=\"font-weight:bold; text-aling='justify'\" ><font size=3>Senhor(a) parlamentar, <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Solicitamos que informe a redução no valor da emenda com base em 1,2% da RCL de 2013.
           	     <br>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Para tanto, entre na aba onde é feita a distribuição do recurso e informe o valor de redução clicando no icone <img src='/imagens/editar_nome.gif' title='Reduzir' style='cursor: pointer;'>.</p>
          	  </div>";
	popupAlertaGeral($texto,"580px","260px");
}


if($_POST['check_expandir']){
	$checked = "checked=\"checked\"";
	$funcao = "carregaDadosEmendaDetalhe(imgEmenda'|| ee.emeid ||',' || ee.emeid || ', \'emendaDetalhe\');";
	$_SESSION['emenda']['funcaoCarregaEntidade'] = "carregaDadosEmendaDetalhe(imgEmendaEntidade'|| ed.emeid ||',' || ed.emeid || ', \'emendaDetalheEntidade\');";
} else {
	$checked = "";
	$funcao = "";
	unset($_SESSION['emenda']['funcaoCarregaEntidade']);
}
?>


<script type="text/javascript" src="js/emenda.js"></script>
<script type="text/javascript" src="/includes/prototype.js"></script>

<script type="text/javascript" src="../includes/jquery-ui-1.8.18.custom/js/jquery-ui-1.8.18.custom.min.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/jquery-ui-1.8.18.custom/css/ui-lightness/jquery-ui-1.8.18.custom.css"/>

<script>
//	var params;	
//	
//	function desabilitarConteudo( id ){
//	
//		var url = '?modulo=principal/listaDeEmendas&acao=A&emeid=' + id;
//		if ( document.getElementById('img'+id).name == '-' ) {
//			url = url + '&subAcao=retirarCarga';
//			var myAjax = new Ajax.Request(
//				url,
//				{
//					method: 'post',
//					asynchronous: false
//				});
//		}
//	}
	
	function pesquisaRecursoExtra(){
		enbnome	=	document.getElementsByName("enbnome")[0];
		
		if(enbnome.value.length > 0 && enbnome.value.length < 3) {
			alert("Para realizar a busca por nome do município é necessário informar pelo menos 3 caracteres.");
			enbnome.focus();
			btnPesquisar.disabled 	= false;
			return;
		}

		document.getElementById('formulario').submit();
	}
	
	function carregaDadosEmendaDetalhe(idImg, emeid, tipo){	
		var img 	 = $( idImg );

		if( tipo == "emendaDetalhe" ){
			var tr_nome = 'trEmenda_'+ emeid;
			var td_nome  = 'tdEmenda_'+ emeid;
			params = '&carregaEmendaDetalhe=true&emeid='+emeid;
		} else {
			var tr_nome = 'trEmendaEntidade_'+ emeid;
			var td_nome  = 'tdEmendaEntidade_'+ emeid;
			params = '&carregaEmendaDetalheEntidade=true&emeid='+emeid;
		}
		
		if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML == ""){
			$(td_nome).innerHTML = 'Carregando...';
			img.src = '../imagens/menos.gif';
			carregaEmendaDetalhe(td_nome, emeid, params);
		}if($(tr_nome).style.display == 'none' && $(td_nome).innerHTML != ""){
			$(tr_nome).style.display = '';
			img.src = '../imagens/menos.gif';
		} else {
			$(tr_nome).style.display = 'none';
			img.src = '/imagens/mais.gif';
		}
	}
	function carregaEmendaDetalhe(td_nome, emeid, params){
		var myajax = new Ajax.Request('emenda.php?modulo=principal/listaDeEmendas&acao=A', {
			        method:     'post',
			        parameters: params,
			        asynchronous: false,
			        onComplete: function (res){
						$(td_nome).innerHTML = res.responseText;
			        }
			  });
	}
	
	function ExcluirEmenda(emeid){
		if(confirm('Deseja realmente excluir este recurso?')) {
			document.getElementById('emeid').value = emeid;
			document.getElementById('requisicao').value = 'excluir';
			document.getElementById('formulario').submit();
		}
	}
	
	function alterarRecursos(emeid){
		document.getElementById('emeid').value = emeid;
		document.getElementById('formulario').action = 'emenda.php?modulo=principal/insereRecursoExtra&acao=A';
		document.getElementById('formulario').submit();
	}
	
	function expandirArvore(){
		$('formulario').submit();
	}

	

</script>

<form action="" method="post" name="formulario" id="formulario">
	<input type="hidden" name="requisicao" value="pesquisa" id="requisicao"/>
	<input type="hidden" value="<?php echo $_SESSION['exercicio']; ?>" name="exercicio" id="exercicio"/>
	<input type="hidden" value="<?php echo date('Y'); ?>" name="anoatual" id="anoatual"/>
	<input type="hidden" name="emeid" value="" id="emeid"/>
	<input type="hidden" name="tipo" value="<?=$tipo; ?>" id="tipo"/>
	
<?php if (  !in_array( AUTOR_EMENDA, $perfilEmenda) ): ?>

	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td colspan="4" class="subtitulocentro">Filtros de Pesquisa</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="15%">Código:</td>
			<td width="33%">
				<?php 
					$emecod = $_REQUEST["emecod"];
					echo campo_texto( 'emecod', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
			<td class="subtitulodireita" width="15%">Responsável Pelo Recurso:</td>
			<td width="33%">
				<?php 
					$resid = $_REQUEST["resid"];
					echo comboResponsavel($_SESSION['usucpf'], $resid);
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita" width="190px">Unidade Orçamentária:</td>
				<td>
				<?php 
					
					$unicod = $_REQUEST["unicod"];
					$sql = array();
					$sql = "SELECT DISTINCT 
								u.unicod as codigo,
								u.unicod||' - '||u.unidsc as descricao
							FROM
								emenda.emenda e
								INNER JOIN emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
								INNER JOIN public.unidade u ON u.unicod = f.unicod
							ORDER BY descricao";
					
					$db->monta_combo("unicod",$sql,'S',"Todas",'','','','500','N','unicod', '', '', '', 'class="sel_chosen"');
				?>
			</td>
			<td class="subtitulodireita" width="190px">Origem Emenda:</td>
				<td>
				<?php 
					
					$etoid = $_REQUEST["etoid"];
					
					$sql = "select 
								o.etoid as codigo,
								o.etodescricao as descricao
							 from emenda.emendatipoorigem o
							 where o.etostatus = 'A'
							 order by o.etodescricao";
					
					$db->monta_combo("etoid",$sql,'S',"Todas",'','','','300','N','etoid');
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ação Orçamentária:</td>
			<td>
				<?php 
					
					$acacod = $_REQUEST["acacod"];
					
					$sql = array();
					$sql ="SELECT DISTINCT 
								f.acacod as codigo,
								f.acacod || ' - ' || f.acadsc as descricao
							FROM
								emenda.emenda e
								INNER JOIN emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
							ORDER BY f.acacod";
					
					$db->monta_combo("acacod", $sql, "S", "Todas", '', '', '', '500', 'N','acacod', '', '', '', 'class="sel_chosen"');
					
				?>
			</td>
			<td class="subtitulodireita">Autor do Recurso:</td>
			<td>
				<?php 
					
					$autid = $_REQUEST["autid"];
					
					$sql = array();
					$sql ="SELECT DISTINCT 
							ea.autid as codigo,
							ea.autnome as descricao
						FROM
							emenda.autor ea
							INNER JOIN emenda.emenda ee ON ea.autid = ee.autid and ea.autstatus = 'A'
						ORDER BY
							ea.autnome";
					
					$db->monta_combo("autid", $sql, "S", "Todos", '', '', '', '', 'N','autid', '', '', '', 'class="sel_chosen"');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">GND:</td>
			<td>
				<?php 
					
					$gndcod = $_REQUEST["gndcod"];
				
					$sql = "SELECT DISTINCT 
								ed.gndcod as codigo,
								ed.gndcod as descricao
							FROM
								emenda.emendadetalhe ed
							INNER JOIN
								emenda.emenda ee ON ee.emeid = ed.emeid";
					
					$db->monta_combo("gndcod", $sql, "S", "Todos", '', '', '', '', 'N','gndcod');
					
				?>
			</td>
			<td class="subtitulodireita" width="190px">Nome do Órgão ou Entidade:</td>
			<td>
				<?php
					$enbnome = $_REQUEST['enbnome']; 
					echo campo_texto( 'enbnome', 'N', 'S', '', 35, 50, '', '', '', '', 0, '' );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">CNPJ:</td>
			<td>
			<?php
					$enbcnpj = $_REQUEST['enbcnpj']; 
					echo campo_texto( 'enbcnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);" );
				?>
			</td>			
			<td class="subtitulodireita">Emenda Relator:</td>
			<td>
				<input type="radio" value="S" id="emenda_relator" name="emenda_relator" <? if($_REQUEST["emenda_relator"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="emenda_relator" name="emenda_relator" <? if($_REQUEST["emenda_relator"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="emenda_relator" name="emenda_relator"<? if($_REQUEST["emenda_relator"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Com Responsável:</td>
			<td>
				<input type="radio" value="S" id="responsavel" name="responsavel" <? if($_REQUEST["responsavel"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="responsavel" name="responsavel" <? if($_REQUEST["responsavel"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="responsavel" name="responsavel" <? if($_REQUEST["responsavel"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita">Com iniciativa:</td>
			<td>
				<input type="radio" value="S" id="iniciativa" name="iniciativa" <? if($_REQUEST["iniciativa"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="iniciativa" name="iniciativa" <? if($_REQUEST["iniciativa"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="iniciativa" name="iniciativa"<? if($_REQUEST["iniciativa"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Possui Pendências:</td>
			<td>
				<input type="radio" value="S" id="pendencia" name="pendencia" <? if($_REQUEST["pendencia"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="pendencia" name="pendencia" <? if($_REQUEST["pendencia"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="pendencia" name="pendencia"  <? if($_REQUEST["pendencia"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita">Com Entidade Beneficiada Definida:</td>
			<td>
				<input type="radio" value="S" id="entidade" name="entidade" <? if($_REQUEST["entidade"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="entidade" name="entidade" <? if($_REQUEST["entidade"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="entidade" name="entidade"  <? if($_REQUEST["entidade"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Descentralização:</td>
			<td>
				<input type="radio" value="S" id="descentralizacao" name="descentralizacao" <? if($_REQUEST["descentralizacao"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="descentralizacao" name="descentralizacao" <? if($_REQUEST["descentralizacao"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="descentralizacao" name="descentralizacao"  <? if($_REQUEST["descentralizacao"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita">Emenda Impositivo:</td>
			<td colspan="3">
				<input type="radio" value="S" id="emdimpositiva" name="emdimpositiva" <? if($_REQUEST["emdimpositiva"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="emdimpositiva" name="emdimpositiva" <? if($_REQUEST["emdimpositiva"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="emdimpositiva" name="emdimpositiva"  <? if($_REQUEST["emdimpositiva"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td style="width: 100px;"><label for="check_expandir"><input type="checkbox" <?=$checked; echo $disab; ?> name="check_expandir" id="check_expandir" onclick="expandirArvore();">Expandir Todos</label></td>
			<td colspan="2">
				<input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisaRecursoExtra();" style="cursor: pointer;"/>
			</td>
			<td>
				<?
				
				if( ( in_array( AUTOR_EMENDA, $perfilEmenda ) ||  in_array( SUPER_USUARIO, $perfilEmenda ) || in_array( LIBERAR_SENHA, $perfilEmenda ) ) && $tipo == 'emenda' ){
					echo '<img src="/imagens/pendente.gif" border=0 title="Pendente" style="cursor:pointer;"> Existe pendência e o recurso não pode ser disponibilizado.';
				}?>
			</td>
		</tr>
	</table>
	<?
	if($tipo == 'extra'){
		?>
		
		<table style="border:0px" class="tabela" cellSpacing="1" cellPadding="3" align="center">
			<tr>
				<td>
				<? 
					if( $tipo == 'extra' ){
						if( in_array( GESTOR_EMENDAS, $perfilEmenda ) ||  in_array( SUPER_USUARIO, $perfilEmenda ) ){
							echo '<a href="emenda.php?modulo=principal/insereRecursoExtra&acao=A">
									<img src="/imagens/gif_inclui.gif" border="0">
									Inserir Recurso Extra
									</a>';
						} else {
							echo '<img src="/imagens/gif_inclui_d.gif" border="0">Inserir Recurso Extra';							
						}
					} else {
						if(possuiPermissao()){ 
							echo '<a href="emenda.php?modulo=principal/insereRecursoExtra&acao=A">
									<img src="/imagens/gif_inclui.gif" border="0">Inserir Recurso Extra</a>';
						} else { 
							echo '<img src="/imagens/gif_inclui_d.gif" border="0">Inserir Recurso Extra';
						} 
					}?>
				</td>
			</tr>
		</table><?
	}
endif; ?>
	
</form>
<div id="dialog_acoes" title="Indicações SIOP" style="display: none" >
	<div style="padding:5px;text-align:justify;" id="mostraRetorno"></div>
</div>

<?php listaEmendas( $filtros, $perfilEmenda, $tipo, $funcao ); ?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
	<tr bgcolor="#D0D0D0">
		<td colspan="2">
			<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
		</td>
	</tr>
</table>

<script lang="javascript">	
	jq('.sel_chosen').chosen({allow_single_deselect:true});
</script>

