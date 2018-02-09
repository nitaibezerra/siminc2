<?php

ini_set("memory_limit", "2048M");
//ver( $_POST,d );
switch ($_REQUEST["requisicao"]) {
	case "pesquisa":
		$filtros = geraFiltroEmendas( $_REQUEST );
	break;
	
	case "libera":
		liberaEntidadeDetalhe( $_REQUEST );
	break;
	
}

////////////////////////////COMBO AUTOR DO RECURSO/////////////////////////////////////////////
if($_REQUEST['comboautid']) { 
	$sql = "SELECT DISTINCT 
								ea.autid as codigo,
								ea.autnome as descricao
							FROM
								emenda.autor ea
							INNER JOIN
								emenda.emenda ee ON ea.autid = ee.autid
							ORDER BY
								ea.autnome";
	
	$opt6 = $db->carregar($sql);
	if($opt6[0]) {
		echo ""."##"."Todos"."&&"; 
		foreach($opt6 as $op6) {
			echo $op6['codigo']."##".$op6['descricao']."&&";
		}
	}
	exit;
}
/////////////////////////////////CONSULTA DA COMBO AÇÃO ORÇAMENTÁRIA/////////////////////////////////////////////
if($_REQUEST['comboacaoid']) { 
	$sql ="SELECT DISTINCT 
				f.acacod as codigo2,
				f.acacod || ' - ' || f.acadsc as descricao2
			FROM
				emenda.emenda e
			INNER JOIN
				emenda.v_funcionalprogramatica f ON f.acaid = e.acaid
			ORDER BY f.acacod"; 

	$opt2 = $db->carregar($sql);
	if($opt2[0]) {
		echo ""."##"."Todas"."&&"; 
		foreach($opt2 as $op2) {
			echo $op2['codigo2']."##".$op2['descricao2']."&&";
		}
	}
	exit;
}
/////////////////////////////////////////////////////////////////////////////////////////////

require_once APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';

$db->cria_aba( $abacod_tela, $url, '' );

monta_titulo( 'Disponibilizar para PTA', '');

?>

<script type="text/javascript" src="js/emenda.js"></script>

<script>
function pesquisaDisPTA(){
	enbnome	=	document.getElementsByName("enbnome")[0];
	
	if(enbnome.value.length > 0 && enbnome.value.length < 3) {
		alert("Para realizar a busca por nome do município é necessário informar pelo menos 3 caracteres.");
		enbnome.focus();
		btnPesquisar.disabled 	= false;
		return;
	}

	document.getElementById('formulario').submit();
}
function selecionaTodos() {

	var selecionaTodos 	= document.getElementById("seleciona_todos");
	var edeid 			= document.getElementsByName("edeid[]");
	
	for(var i=0; i<edeid.length; i++) {
		if(selecionaTodos.checked) {
			if(!edeid[i].checked && !edeid[i].disabled) edeid[i].checked = true;
		} else {
			if(edeid[i].checked) edeid[i].checked = false;
		}
	}
}
</script>

<form action="" method="post" id="formulario" name="formulario">
	<input type="hidden" value="pesquisa" name="requisicao"/>
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr>
			<td class="subtitulodireita">Código:</td>
			<td >
				<?php 
					$emecod = $_REQUEST["emecod"];
					echo campo_texto( 'emecod', 'N', 'S', '', 12, 8, '', '', 'left', '', 0, '');
				?>
			</td>
			<td class="subtitulodireita" width="180px">Responsável Pelo Recurso:</td>
			<td >
				<?php 
					$resid = $_REQUEST["resid"];
					echo comboResponsavel($_SESSION['usucpf'], $resid);
					
					/*$residPossiveis = recuperaResponsaveis( $_SESSION["usucpf"] );
					$filtroResid    = count($residPossiveis) > 0 ? " WHERE er.resid in (" . implode( ", ", $residPossiveis ) . ")" : "";
					
					$resid    = count($residPossiveis) == 1 ? $residPossiveis[0] : $_REQUEST["resid"];
					$habResid = count($residPossiveis) == 1 ? "N" : "S";
					
					$sql = "SELECT DISTINCT 
								er.resid as codigo,
								er.resdsc as descricao
							FROM
								emenda.responsavel er
							LEFT JOIN
								emenda.emenda ee ON er.resid = ee.resid
							{$filtroResid}
							ORDER BY
								er.resdsc";
					
					$db->monta_combo("resid", $sql, $habResid, "Todos", '', '', '', '', 'N','resid');*/
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Ação Orçamentária:</td>
			<td colspan="3">
				<?php 
					
					$acacod = $_REQUEST["acacod"];
					
					$sql = array();
					/*$sql = "SELECT DISTINCT 
								f.acacod as codigo,
								f.acacod || ' - ' || f.acadsc as descricao
							FROM
								emenda.emenda e
							INNER JOIN
								emenda.v_funcionalprogramatica f ON f.acaid = e.acaid";*/
					
					$db->monta_combo("acacod", $sql, "S", "Todas", '', '', '', '300', 'N','acacod');
					
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">GND:</td>
			<td >
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
			<td class="subtitulodireita">Autor do Recurso:</td>
			<td >
				<?php 
					
					$autid = $_REQUEST["autid"];
					
					$sql = array();
					/*$sql = "SELECT DISTINCT 
								ea.autid as codigo,
								ea.autnome as descricao
							FROM
								emenda.autor ea
							INNER JOIN
								emenda.emenda ee ON ea.autid = ee.autid
							ORDER BY
								ea.autnome";*/
					
					$db->monta_combo("autid", $sql, "S", "Todos", '', '', '', '', 'N','autid');
					
				?>
			</td>
		</tr>		
		<tr>
			<td class="subtitulodireita">CNPJ:</td>
			<td>
			<?php
					$enbcnpj = $_REQUEST['enbcnpj']; 
					echo campo_texto( 'enbcnpj', 'N', 'S', '', 27, 20, '', '', '', '', 0, '', "this.value=mascaraglobal('##.###.###/####-##',this.value);", $enbcnpj );
				?>
			</td>
			<td class="subtitulodireita" width="190px">Nome do Órgão ou Entidade:</td>
			<td>
				<?php
					$entnome = $_REQUEST['enbnome']; 
					echo campo_texto('enbnome', 'N', 'S', '', 60, 50, '', '', '', '', 0, '', '', $entnome );
				?>
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Com Responsável:</td>
			<td >
				<input type="radio" value="S" id="responsavel" name="responsavel" <? if($_REQUEST["responsavel"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="responsavel" name="responsavel" <? if($_REQUEST["responsavel"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="responsavel" name="responsavel" <? if($_REQUEST["responsavel"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita">Com iniciativa:</td>
			<td >
				<input type="radio" value="S" id="iniciativa" name="iniciativa" <? if($_REQUEST["iniciativa"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="iniciativa" name="iniciativa" <? if($_REQUEST["iniciativa"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="iniciativa" name="iniciativa"<? if($_REQUEST["iniciativa"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Possui Pendências:</td>
			<td >
				<input type="radio" value="S" id="pendencia" name="pendencia" <? if($_REQUEST["pendencia"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="pendencia" name="pendencia" <? if($_REQUEST["pendencia"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="pendencia" name="pendencia"  <? if($_REQUEST["pendencia"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita">Disponibilizado:</td>
			<td >
				<input type="radio" value="S" id="liberado" name="liberado" <? if($_REQUEST["liberado"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="liberado" name="liberado" <? if($_REQUEST["liberado"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="liberado" name="liberado"  <? if($_REQUEST["liberado"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr>
			<td class="subtitulodireita">Descentralização:</td>
			<td >
				<input type="radio" value="S" id="descentralizacao" name="descentralizacao" <? if($_REQUEST["descentralizacao"] == "S") { echo "checked"; } ?> /> Sim
				<input type="radio" value="N" id="descentralizacao" name="descentralizacao" <? if($_REQUEST["descentralizacao"] == "N") { echo "checked"; } ?> /> Não
				<input type="radio" value="" id="descentralizacao" name="descentralizacao"  <? if($_REQUEST["descentralizacao"] == "") { echo "checked"; } ?> /> Todos
			</td>
			<td class="subtitulodireita">Tipos de Recursos</td>
			<td >
				<input type="radio" value="emenda" id="tipo" name="tipo" <? if($_REQUEST["tipo"] == "emenda") { echo "checked"; } ?> /> Emenda
				<input type="radio" value="extra" id="tipo" name="tipo" <? if($_REQUEST["tipo"] == "extra") { echo "checked"; } ?> /> Complementos Orçamentários
				<input type="radio" value="" id="tipo" name="tipo"  <? if($_REQUEST["tipo"] == "") { echo "checked"; } ?> /> Todos
			</td>
		</tr>
		<tr bgcolor="#D0D0D0">
			<td align="center">
				<input type='checkbox' id='seleciona_todos' onclick='selecionaTodos();' /> <b>Selecionar Todos</b>
			</td>
			<td colspan="3">
				<input type="button" value="Pesquisar" id="btnPesquisar" name="btnPesquisar" onclick="pesquisaDisPTA();" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<form action="" method="post" name="formLiberacao" id="formLiberacao">
	<input type="hidden" value="libera" name="requisicao"/>	
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding=3 align="center">
		<tr bgcolor="#D0D0D0">
			<td colspan="2">
				<input type="button" value="Salvar" onclick="liberaEntidades();" style="cursor: pointer;" <?=disabled()?> />
				<input type="button" value="Voltar" onclick="history.back(-1);" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>
<?php listaEntidadeLiberacao( $filtros ); ?>
<script>
		/* Função para substituir todos */
		function replaceAll6(str, de, para){
		    var pos6 = str.indexOf(de);
		    while (pos6 > -1){
				str = str.replace(de, para);
				pos6 = str.indexOf(de);
			}
		    return (str);
		}
		/* Função para adicionar linha nas tabelas */
		
		/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
		function criarrequisicao6() {
			return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
		}
		/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
		
		//CAMPO AUTOR DO RECURSO
		
				/* FUNÇÃO QUE TRATA O RETORNO */
				var pegarretorno6 = function () {
					try {
							if ( evXmlHttp6.readyState == 4 ) {
								if ( evXmlHttp6.status == 200 && evXmlHttp6.responseText != '' ) {
									// criando options
									var x6 = evXmlHttp6.responseText.split("&&");
									for(i6=1;i6<(x6.length-1);i6++) {
										var dados6 = x6[i6].split("##");
										document.getElementById('autid').options[i6] = new Option(dados6[1],dados6[0]);
									}
									var dados6 = x6[0].split("##");
									document.getElementById('autid').options[0] = new Option(dados6[1],replaceAll6(dados6[0], ' ',''));
									document.getElementById('autid').value = comboselecionado6;
								}
								if ( evXmlHttp6.dispose ) {
									evXmlHttp6.dispose();
								}
								evXmlHttp6 = null;
							}
						}
					catch(e) {}
				};
				/* FIM - FUNÇÃO QUE TRATA O RETORNO */
				
				/* VARIAVEIS GLOBAIS */
				var comboselecionado6 = document.getElementById('autid').value;
				var carregar6 = false;
				var evXmlHttp6 = null;
				/* FIM - VARIAVEIS GLOBAIS */
				
				/* ATRIBUINDO FUNÇÃO DE CLICK AO COMBO DE AUTOR DO RECURSO */
				document.getElementById('autid').onclick = function () {
					if(!carregar6) {
						carregar6 = true;
																		// limpar o select
						var tot6=document.getElementById('autid').options.length
						for (i=0;i<tot6;i++) {
							document.getElementById('autid').options[i]=null
						}
						document.getElementById('autid').options.length=0;
						
						document.getElementById('autid').options[0] = new Option("Aguarde, carregando dados...","aguar");
						document.getElementById('autid').value = "aguar";
						if ( evXmlHttp6 != null ) {
							return;
						}
						
						try {
							evXmlHttp6 = criarrequisicao6();
							var enderecohtml6 = replaceAll6(window.location.href,"#","");
							evXmlHttp6.open( 'GET', enderecohtml6+'&comboautid=true', true );
							evXmlHttp6.onreadystatechange = pegarretorno6;
							evXmlHttp6.send( null );
						}
						catch (e) {}// tratar exceção
						
				
					}// fim do if(carregar)
				}
				/* FIM - ATRIBUINDO FUNÇÃO DE CLICK AO COMBO */
				
		//FIM CAMPO Autor do Recurso
		

//CAMPO AÇÃO ORÇAMENTÁRIA

		/* Função para substituir todos */
		function replaceAll3(str, de, para){
		    var pos3 = str.indexOf(de);
		    while (pos3 > -1){
				str = str.replace(de, para);
				pos3 = str.indexOf(de);
			}
		    return (str);
		}

		/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
		function criarrequisicao3() {
			return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
		}
		
		/* FUNÇÃO QUE TRATA O RETORNO */
		var pegarretorno3 = function () {
			try {
					if ( evXmlHttp3.readyState == 4 ) {
						if ( evXmlHttp3.status == 200 && evXmlHttp3.responseText != '' ) {
							// criando options
							var x3 = evXmlHttp3.responseText.split("&&");
							for(i3=1;i3<(x3.length-1);i3++) {
								var dados3 = x3[i3].split("##");
								document.getElementById('acacod').options[i3] = new Option(dados3[1],dados3[0]);
							}
							var dados3 = x3[0].split("##");
							document.getElementById('acacod').options[0] = new Option(dados3[1],replaceAll3(dados3[0], ' ',''));
							document.getElementById('acacod').value = comboselecionado3;
						}
						if ( evXmlHttp3.dispose ) {
							evXmlHttp3.dispose();
						}
						evXmlHttp3 = null;
					}
				}
			catch(e) {}
		};
		/* FIM - FUNÇÃO QUE TRATA O RETORNO */
		
		/* VARIAVEIS GLOBAIS */
		var comboselecionado3 = document.getElementById('acacod').value;
		var carregar3 = false;
		var evXmlHttp3 = null;
		/* FIM - VARIAVEIS GLOBAIS */
		
		/* ATRIBUINDO FUNÇÃO DE CLICK AO COMBO DE AÇÃO ORÇAMENTÁRIA */
		document.getElementById('acacod').onclick = function () {
			if(!carregar3) {
				carregar3 = true;
																// limpar o select
				var tot3=document.getElementById('acacod').options.length;
				for (i=0;i<tot3;i++) {
					document.getElementById('acacod').options[i]=null;
				}
				document.getElementById('acacod').options.length=0;
				
				document.getElementById('acacod').options[0] = new Option("Aguarde, carregando dados...","agu");
				document.getElementById('acacod').value = "agu";
				if ( evXmlHttp3 != null ) {
					return;
				}
				
				try {
					evXmlHttp3 = criarrequisicao3();
					var enderecohtml3 = replaceAll3(window.location.href,"#","");
					evXmlHttp3.open( 'GET', enderecohtml3+'&comboacaoid=true', true );
					evXmlHttp3.onreadystatechange = pegarretorno3;
					evXmlHttp3.send( null );
				}
				catch (e) {}// tratar exceção
			}// fim do if(carregar)
		}
		/* FIM - ATRIBUINDO FUNÇÃO DE CLICK AO COMBO */
//FIM CAMPO AÇÃO ORÇAMENTÁRIA		
		
		
		
		
</script>


