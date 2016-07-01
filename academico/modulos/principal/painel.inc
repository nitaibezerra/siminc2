<?php
//header('Content-Type: text/plain; charset=ISO-8859-1');
include APPRAIZ. 'includes/classes/relatorio.class.inc';

ini_set("memory_limit", "1024M");

// atualiza painel com as unidades da UF selecionada
if ( $_REQUEST['listarunidades'] == 1 ){
	if ($_REQUEST['estuf']){
		
		// cria o join e o filtro por região, caso exista
		switch( $_REQUEST['estuf'] ){
			case 'norte':
				$filtro['regcod'] = 1;
			break;
			case 'nordeste':
				$filtro['regcod'] = 2;
			break;
			case 'sudeste':
				$filtro['regcod'] = 3;
			break;
			case 'sul':
				$filtro['regcod'] = 4;
			break;
			case 'centrooeste':
				$filtro['regcod'] = 5;
			break;
			default:
				$filtro['estuf'] = $_REQUEST['estuf']; 
			break;
		}
		
		// lista de todas as undiades (país) 
		academico_lista_unidades_painel( $_REQUEST['estuf'], $_REQUEST['orgid'], $_REQUEST['filtrocmp'] );
		
		?>
		<div id="quadrosituacaoatual" class="caixa_redefederal"/>	
			<table cellspacing="1" cellpadding="3" style="width: 100%;">
				<tr>
					<td style="text-align: center; background-color: #dedede; font-weight: bold; width:75%;"> Situação Atual (até 2009)</td>
					<td style="text-align: center; background-color: #dedede; font-weight: bold; width:25%;">
					Até 
					<?
					$dados = array(0 => array('codigo' => '2010', 'descricao' => '2010'),
								   1 => array('codigo' => '2011', 'descricao' => '2011'),
								   2 => array('codigo' => '2012', 'descricao' => '2012')); 
					$db->monta_combo('anosit', $dados, 'S', '', 'selecionaranocomparacao', '', '', '', 'N', 'anosit'); 
					?>
					</td>
				</tr>
				<tr>
					<td style="padding: 0px; margin: 0px;">
						<center>TOTAL</center>
						<?php academico_situacao_atual( $_SESSION['academico']['orgid'], $filtro, 1, $_REQUEST['filtrocmp'] ); ?>
						<br />
						<? if($_SESSION['academico']['orgid'] == TPENSSUP) { ?>
						<center>REUNI</center>
						<?php academico_situacao_atual( $_SESSION['academico']['orgid'], $filtro, 0, $_REQUEST['filtrocmp'] ); ?>
						
						<? } ?>

					</td>
					<td style="padding: 0px; margin: 0px;">
						<center>TOTAL</center>
							<? academico_situacao_atual_comparacao( $_SESSION['academico']['orgid'], $filtro, 1, 'x', $_REQUEST['filtrocmp'] ); ?>
						<br />
						<? if($_SESSION['academico']['orgid'] == TPENSSUP) { ?>
						<center>REUNI</center>
							<?php academico_situacao_atual_comparacao( $_SESSION['academico']['orgid'], $filtro, 0, 'x', $_REQUEST['filtrocmp'] ); ?>
						<? } ?>
					</td>									
				</tr>
			</table>
		</div>

		<!-- Quadro de Situação da Obra -->
		<div id="quadrosituacao" class="caixa_redefederal"/>	
			<table cellspacing="1" cellpadding="3" style="width: 100%;">
				<tr>
					<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Obras </td>
				</tr>
				<tr>
					<td style="padding: 0px; margin: 0px;">
						<?php academico_situacao_obras( $_SESSION['academico']['orgid'], $_REQUEST['estuf'], '', $_REQUEST['filtrocmp'] ); ?>
					</td>
				</tr>
			</table>
		</div>

		
		<?php
		/*
			$titulo_quadro = $_SESSION['academico']['orgid'] == ACA_ORGAO_SUPERIOR ? "Expansão da Educação Superior" 
																				   : "Expansão da Educação Profissional e Tecnológica";
		*/
		?>
		<!-- 
		<div id="quadrosituacaoatual" class="caixa_redefederal"/>	
				<table cellspacing="1" cellpadding="3" style="width: 100%;">
					<tr>
						<td style="text-align: center; background-color: #dedede; font-weight: bold;"> <?php echo $titulo_quadro ?> </td>
					</tr>
					<tr>
						<td style="padding: 0px; margin: 0px;">
							<?php 
							//academico_painel_dados_sig( $_SESSION['academico']['orgid'], $_REQUEST['estuf'] ); 
							?>
						</td>									
					</tr>
				</table>
			</div>
		</div>
		 -->
		       	       	
		<?
		die;
		
	}
}
// Abre árvore com os campi das unidades
if( $_REQUEST['carga'] ){
	$_SESSION['academico']['orgid'] = !empty($_SESSION['academico']['orgid']) ? $_SESSION['academico']['orgid'] : 1;
	academico_lista_campus_painel( $_REQUEST['carga'], $_SESSION['academico']['orgid'] );
	die;
	
}

if ( $_REQUEST["cargaexpansao"] ){
	academico_painel_campus_dados_sig ( $_SESSION['academico']['orgid'], $_REQUEST["cargaexpansao"], $_REQUEST["estuf"] );
	die;
	
}

//echo $_REQUEST['requisicaoajax'];
// atualiza quadro/mapa do painel de acordo com a requisicaoajax
academico_atualiza_quadro( $_REQUEST['requisicaoajax'], $_REQUEST['dado'], $_SESSION['academico']['orgid'], $_REQUEST['estuf'], $_REQUEST['entid']);

if(!$_GET["linkExterno"]){
	// monta o cabeçalho
	include APPRAIZ . "includes/cabecalho.inc";
	echo "<br/>";
}else{ ?>
	<script language="JavaScript" src="../includes/funcoes.js"></script>
	<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
	<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>	
<? }
// Monta as abas com os orgão de responsabilidade do usuário (se nao for superUser)
$res = academico_pega_orgao_permitido( 'painel' );

if ($res[0]["id"] != null){
	
	$org = $_REQUEST['orgid'] ? $_REQUEST['orgid'] : $res[0]['id'];
	$_SESSION['academico']['orgid'] = $org;
	
	if(!$_GET["linkExterno"]){
		$link = '/academico/academico.php?modulo=principal/painel&acao=A&orgid=' . $org;	
		echo montarAbasArray($res, $link );
		monta_titulo( "Painel da Rede Federal de Educação", "Clique no estado desejado para filtrar as unidades");
	}
	$titulo_quadro = $org == ACA_ORGAO_SUPERIOR ? "Expansão da Educação Superior" 
												: "Expansão da Educação Profissional e Tecnológica";
	
?>

	<script type="text/javascript" src="/includes/prototype.js"></script>
	<script type="text/javascript" src="../includes/JQuery/jquery2.js"></script>
	<script>
	jQuery.noConflict();
	</script>
	<script src="/academico/geral/js/academico.js"></script>
	<script type="text/javascript" src="/includes/ModalDialogBox/modal-message.js"></script>
	<script type="text/javascript" src="/includes/ModalDialogBox/ajax-dynamic-content.js"></script>
	<script type="text/javascript" src="/includes/ModalDialogBox/ajax.js"></script>
	
	<script type="text/javascript">
	
		var params;
	
		function formatarParametros(){
		    //params = Form.serialize($('pesquisar'));
		}
		
		function desabilitarConteudo( id ){
			var url = 'academico.php?modulo=principal/painel&acao=A&carga='+id;
			if ( document.getElementById('img'+id).name == '-' ) {
				url = url + '&subAcao=retirarCarga';
				var myAjax = new Ajax.Request(
					url,
					{
						method: 'post',
						asynchronous: false
					}
				);
			}
		}
		
		
		messageObj = new DHTML_modalMessage();	// We only create one object of this class
		messageObj.setShadowOffset(5);	// Large shadow
		
		function displayMessage(url){
			
			messageObj.setSource(url);
			messageObj.setCssClassMessageBox(false);
			messageObj.setSize(444,357);
			messageObj.setShadowDivVisible(true);	// Enable shadow for these boxes
			messageObj.display();
		}
		
		function displayStaticMessage(messageContent,cssClass){
			messageObj.setHtmlContent(messageContent);
			messageObj.setSize(400,250);
			messageObj.setCssClassMessageBox(cssClass);
			messageObj.setSource(false);	// no html source since we want to use a static message here.
			messageObj.setShadowDivVisible(false);	// Disable shadow for these boxes	
			messageObj.display();
			
			
		}
		
		function closeMessage(){
			messageObj.close();	
		}
		
		function carregar_relatorio_novo(prtid, unidade) {
			var janela = window.open( '/financeiro/financeiro.php?modulo=relatorio/relatorio_financeiro_academico&acao=A&unicod=' + unidade + '&prtid=' + prtid, 'relatorio', 'width=1000,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();
			return false;
		}
		
		function carregar_relatorio( acao, unidade ){
			
			var link = '';

			switch( acao ){
				case 'execucao':
					link = 'execucao'
				break;
			}

			// relatório execução REUNI
			if(acao == "execucaoreuni") {
				var janela = window.open( 'academico.php?modulo=relatorio/relatorio_execucao_reuni_resultado&acao=A&entid=' + unidade + '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
				janela.focus();
				return;
			}
			
			var url = 'academico.php?modulo=principal/financeiro_painel&acao=A';
			var parametros = '&relatorioajax=' + link + '&entid=' + unidade;
			
			var myAjax = new Ajax.Request(
			url,
				{
					method: 'post',
					parameters: parametros,
					asynchronous: false,
					onComplete: function(resp) {
						window.open( 'academico.php?modulo=principal/financeiro_painel&acao=A', 'relatorio', 'width=800,height=700,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
					}
				}
			);
						
		}
		
		function abreMapa(obrid){
			
//			var obrid = window.document.getElementById("obrid").value;
//			alert(obrid);
			var graulatitude = window.document.getElementById(obrid + "graulatitude").value;
			var minlatitude  = window.document.getElementById(obrid + "minlatitude").value;
			var seglatitude  = window.document.getElementById(obrid + "seglatitude").value;
			var pololatitude = window.document.getElementById(obrid + "pololatitude").value;
			
			var graulongitude = window.document.getElementById(obrid + "graulongitude").value;
			var minlongitude  = window.document.getElementById(obrid + "minlongitude").value;
			var seglongitude  = window.document.getElementById(obrid + "seglongitude").value;
			
			var latitude  = ((( Number(seglatitude) / 60 ) + Number(minlatitude)) / 60 ) + Number(graulatitude);
			var longitude = ((( Number(seglongitude) / 60 ) + Number(minlongitude)) / 60 ) + Number(graulongitude);
			
			var janela=window.open('academico.php?modulo=principal/mapa&acao=A&longitude='+longitude+'&latitude='+latitude+'&polo='+pololatitude+'&obrid='+obrid, 'mapa','height=620,width=570,status=no,toolbar=no,menubar=no,scrollbars=no,location=no,resizable=no').focus();
		}
		
		function selecionaranocomparacao(value) {
			if(document.getElementById('tabelax0')) {
				var t = document.getElementById('tabelax0');
				for(i=1;i<t.rows.length;i++) {
					if(t.rows[i].id == value) {
						t.rows[i].style.display = '';
					} else {
						t.rows[i].style.display = 'none';
					}
				}
			}
			
			var t = document.getElementById('tabelax1');
			for(i=1;i<t.rows.length;i++) {
				if(t.rows[i].id == value) {
					t.rows[i].style.display = '';
				} else {
					t.rows[i].style.display = 'none';
				}
			}

		}
		
		function selecionaranocomparacao2(value) {
			if(document.getElementById('tabelaxx0')) {
				var t = document.getElementById('tabelaxx0');
				for(i=1;i<t.rows.length;i++) {
					if(t.rows[i].id == value) {
						t.rows[i].style.display = '';
					} else {
						t.rows[i].style.display = 'none';
					}
				}
			}
			
			var t = document.getElementById('tabelaxx1');
			for(i=1;i<t.rows.length;i++) {
				if(t.rows[i].id == value) {
					t.rows[i].style.display = '';
				} else {
					t.rows[i].style.display = 'none';
				}
			}

		}
		
		
	</script>
	
	<link rel="stylesheet" href="/includes/ModalDialogBox/modal-message.css" type="text/css" media="screen" />
	<style>
		
		.caixa_redefederal{
			float: left; 
			margin: 2px;
			height: 170px; 
			width: 97%; 
			border: 1px solid #cccccc;
			overflow: auto;
		}
		
		.caixa_redefederal2{
			float: left; 
			margin: 5px;
			height: 170px; 
			width: 47%; 
			border: 1px solid #cccccc;
			overflow: auto;
		}		
		
		.titulounidade {
			background-color:#CDCDCD;
			border-top:2px solid #000000;
			border-bottom:2px solid #000000;
			text-align:center;
			font-weight: bold;
		}
		
	</style>
	
	<table id="containerSuperior" class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center" width="95%">
		<tbody>
		
			<tr>
				<td align="center" valign="top" width="45%">
				<table class=listagem width=100%>
				<tr>
					<td>Existência do campus/uned:</td>
					<td>Situação do campus/uned:</td>
					<td>Instalações:</td>
					<td rowspan=2 align=center><input type="button" name="enviarfiltrocmp" value="Ok" onclick="divCarregando();academico_listaUnidades(document.getElementById('filtrogeral').value, <? echo $_SESSION['academico']['orgid']; ?> );divCarregado();"></td>
				</tr>
				<tr>
					<td>
					<select name="exiid" id="exiid">
					<?
					$sql = "SELECT * FROM academico.existencia WHERE existatus='A'";
					$exi = $db->carregar($sql);
					echo "<option value=>Todos</option>";
					if($exi[0]) {
						foreach($exi as $ex) {
							echo "<option value=\"".$ex['exiid']."\">".$ex['exidsc']."</option>";
						}
					}
					?>
					</select> 
					</td>
					<td>
					 <select name="cmpsituacao" id="cmpsituacao">
						<option value="">Todos</option>
					 	<option value="F">Funcionando</option>
						<option value="N">Não Funcionando</option>
					 </select>
					</td>
					<td>
					<select name="cmpinstalacao" id="cmpinstalacao">
						<option value="">Todos</option>
						<option value="P">Instalações Provisórias</option>
						<option value="D">Instalações Definitivas</option>
						<option value="S">Sem Instalações</option>
					</select>
					</td>
				</tr>
				</table>
				<input type="hidden" name="filtrogeral" id="filtrogeral" value="todos">
					<div id="conteudolistaunidades" style="width:100%;"/>
							
						<?php academico_lista_unidades_painel( 'todos', $_SESSION['academico']['orgid'] ); ?>
						
						
						<div id="quadrosituacaoatual" class="caixa_redefederal"/>	
							<table cellspacing="1" cellpadding="3" style="width: 100%;">
								<tr>
									<td style="text-align: center; background-color: #dedede; font-weight: bold; width:75%;"> Situação Atual (2009)</td>
									<td style="text-align: center; background-color: #dedede; font-weight: bold; width:25%;">
									Até 
									<?
									$dados = array(0 => array('codigo' => '2010', 'descricao' => '2010'),
												   1 => array('codigo' => '2011', 'descricao' => '2011'),
												   2 => array('codigo' => '2012', 'descricao' => '2012')); 
									$db->monta_combo('anosit', $dados, 'S', '', 'selecionaranocomparacao', '', '', '', 'N', 'anosit'); 
									?>
									</td>
								</tr>
								<tr>
									<td style="padding: 0px; margin: 0px;" valign="top">
									<center>TOTAL</center>
										<?php academico_situacao_atual( $_SESSION['academico']['orgid'] ); ?>
									<br />
									<? if($_SESSION['academico']['orgid'] == TPENSSUP) { ?>
									<center>REUNI</center>
										<?php academico_situacao_atual( $_SESSION['academico']['orgid'], null, 0 ); ?>
									<? } ?>
									</td>									
									<td style="padding: 0px; margin: 0px;" valign="top">
									<center>TOTAL</center>
										<? academico_situacao_atual_comparacao( $_SESSION['academico']['orgid'] ); ?>
									<br />
									<? if($_SESSION['academico']['orgid'] == TPENSSUP) { ?>
									<center>REUNI</center>
										<?php academico_situacao_atual_comparacao( $_SESSION['academico']['orgid'], null, 0 ); ?>
									<? } ?>

									</td>
								</tr>
							</table>
						</div>
					
						<!-- Quadro de situação da Obra -->
						<div id="quadrosituacao" class="caixa_redefederal"/>	
							<table cellspacing="1" cellpadding="3" style="width: 100%;">
								<tr>
									<td style="text-align: center; background-color: #dedede; font-weight: bold;"> Obras </td>
								</tr>
								<tr>
									<td style="padding: 0px; margin: 0px;">
										<?php academico_situacao_obras( $_SESSION['academico']['orgid'] ); ?>
									</td>
								</tr>
							</table>
						</div>

						<!-- 
						<div id="quadrosig" class="caixa_redefederal"/>
							<table cellspacing="1" cellpadding="3" style="width: 100%;">
								<tr>
									<td style="text-align: center; background-color: #dedede; font-weight: bold;"> <?php echo $titulo_quadro ?> </td>
								</tr>
								<tr>
									<td style="padding: 0px; margin: 0px;">
										<?php
										// academico_painel_dados_sig( $_SESSION['academico']['orgid'], '', '' ); 
										?>
									</td>									
								</tr>
							</table>
						</div>
						 -->
						</fieldset>		
					</div>
				</td>		
				<td align="center" valign="top" width="45%">
				<fieldset style="background: #ffffff; width: 90%"  >
					<legend> Painel </legend>
					
					<div style="width: 100%;" id="containerMapa" >
						<img src="/imagens/mapa_brasil.png" width="444" height="357" border="0" usemap="#mapaBrasil" />
						<map name="mapaBrasil" id="mapaBrasil">
							<area shape="rect" coords="388,15,427,48"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('todos', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Brasil"/>							
							<area shape="rect" coords="48,124,74,151"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('AC', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Acre"/>
							<area shape="rect" coords="364,147,432,161" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('AL', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Alagoas"/>
							<area shape="rect" coords="202,27,233,56"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('AP', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Amapá"/>
							<area shape="rect" coords="89,76,133,107"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('AM', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Amazonas"/>
							<area shape="rect" coords="294,155,320,183" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('BA', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Bahia"/>
							<area shape="rect" coords="311,86,341,114"  style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('CE', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Ceará"/>
							<area shape="rect" coords="244,171,281,197" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('DF', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Distrito Federal"/>
							<area shape="rect" coords="331,215,369,242" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('ES', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Espírito Santo"/>
							<area shape="rect" coords="217,187,243,218" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('GO', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Goiás"/>
							<area shape="rect" coords="154,155,210,186" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('MT', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Mato Grosso"/>
							<area shape="rect" coords="156,219,202,246" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('MS', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Mato Grosso do Sul"/>
							<area shape="rect" coords="248,80,301,111"  style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('MA', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Maranhão"/>
							<area shape="rect" coords="264,206,295,235" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('MG', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Minas Gerais"/>
							<area shape="rect" coords="188,84,217,112"  style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('PA', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Pará"/>
							<area shape="rect" coords="368,112,433,130" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('PB', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Paraíba"/>
							<area shape="rect" coords="201,262,231,289" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('PR', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Paraná"/>
							<area shape="rect" coords="369,131,454,147" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('PE', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Pernambuco"/>
							<area shape="rect" coords="285,116,313,146" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('PI', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Piauí"/>
							<area shape="rect" coords="349,83,383,108"  style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('RN', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Rio Grande do Norte"/>
							<area shape="rect" coords="189,310,224,337" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('RS', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Rio Grande do Sul"/>
							<area shape="rect" coords="302,250,334,281" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('RJ', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Rio de Janeiro"/>
							<area shape="rect" coords="98,139,141,169"  style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('RO', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Rondônia"/>
							<area shape="rect" coords="112,24,147,56"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('RR', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Roraima"/>
							<area shape="rect" coords="228,293,272,313" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('SC', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Santa Catarina"/>
							<area shape="rect" coords="233,243,268,270" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('SP', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="São Paulo"/>
							<area shape="rect" coords="337,161,401,178" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('SE', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Sergipe"/>
							<area shape="rect" coords="227,130,270,163" style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('TO', <?=$_SESSION['academico']['orgid']?>);" title="Tocantins"/>
							<area shape="rect" coords="17,264,85,282"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('norte', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Norte" />
							<area shape="rect" coords="16,281,94,296"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('nordeste', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Nordeste" />
							<area shape="rect" coords="15,296,112,312"  style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('centrooeste', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Centro-Oeste" />
							<area shape="rect" coords="14,312,100,329"  style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('sudeste', <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Sudeste" />
							<area shape="rect" coords="13,329,68,344"   style="cursor:pointer;" onclick="divCarregando();academico_listaUnidades('sul',  <?=$_SESSION['academico']['orgid']?>);divCarregado();" title="Sul" />
						</map>
					</div>
					</fieldset>
				</td>
			</tr>
		</tbody>
	</table>
	
<?php } ?>