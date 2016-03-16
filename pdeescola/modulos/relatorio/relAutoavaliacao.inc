<?php

ini_set( "memory_limit", "-1" );
set_time_limit(0);

// transforma consulta em pública
if ( $_REQUEST['prtid'] && $_REQUEST['publico'] ){
	$sql = sprintf(
		"UPDATE public.parametros_tela SET prtpublico = case when prtpublico = true then false else true end WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
	<script type="text/javascript">
		location.href = '?modulo=<?= $modulo ?>&acao=A';
	</script>
	<?
	die;
}
// FIM transforma consulta em pública

// remove consulta
if ( $_REQUEST['prtid'] && $_REQUEST['excluir'] == 1 ) {
	$sql = sprintf(
		"DELETE from public.parametros_tela WHERE prtid = %d",
		$_REQUEST['prtid']
	);
	$db->executar( $sql );
	$db->commit();
	?>
		<script type="text/javascript">
			//location.href = '?modulo=<?= $modulo ?>&acao=A';
			location.href = 'pdeescola.php?modulo=relatorio/relMaisEducacao&acao=A';
		</script>
	<?
	die;
}
// FIM remove consulta

// remove flag de submissão de formulário
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] ){
	unset( $_REQUEST['form'] );
}
// FIM remove flag de submissão de formulário

// exibe consulta
if ( isset( $_REQUEST['form'] ) == true ){
	if ( $_REQUEST['prtid'] ){
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_REQUEST['salvar'] );
	}
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "relAutoavaliacao_resultado.inc";
			exit;
		case '2':
			include "relAutoavaliacaoXls_resultado.inc";
			exit;
	}
	
}


if ($_POST || $_GET['entid']){
	include 'relAutoavaliacao_resultado.inc';
	exit;
}

include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';

print '<br/>';
monta_titulo( 'Relatório Geral', 'Relatório Geral' );

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript">

<?
//exibe consulta
if ( isset( $_REQUEST['form'] ) == true ){
	if ( $_REQUEST['prtid'] ){
		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );
		$itens = $db->pegaUm( $sql );
		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
		$_REQUEST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_REQUEST['salvar'] );
	}
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "relAutoavaliacao_resultado.inc";
			exit;
		case '2':
			include "relAutoavaliacaoXls_resultado.inc";
			exit;
	}
	
}
?>

function gerarRelatorioAutoavaliacaoXLS(){		
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );
	
	// Tipo de relatorio
	formulario.pesquisa.value='2';
	
	prepara_formulario();
	selectAllOptions( formulario.agrupador );
	
	if ( !agrupador.options.length ){
		alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
		agrupador.focus();
		return false;
	}	
		
    //verificando checked, tornando variável existQapid = true para validação.
	var existQapid = false;
	var total = document.getElementsByName('qapid[]').length;
	for(var i=0; i < total; i++){
		if ( document.getElementsByName('qapid[]')[i].checked ){
			existQapid = true;
			break;//parando o loop
		}
	}
	
	if ( !existQapid ){
		alert('Você deve selecionar pelo menos uma questão');
		return false;
	}
	
	selectAllOptions( agrupador );	
	formulario.submit();
	
}

function gerarRelatorioAutoavaliacao(tipo){
	
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'colunas' );

	// Tipo de relatorio
	formulario.pesquisa.value='1';

	
	prepara_formulario();
	selectAllOptions( formulario.agrupador );
	
	if ( tipo == 'relatorio' ){
		
		formulario.action = 'pdeescola.php?modulo=relatorio/relAutoavaliacao&acao=A';
		window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		formulario.target = 'relatorio';
		
	}else {
	
		if ( tipo == 'planilha' ){


			if (formulario.elements['agrupador'][0] == null){
				alert('Selecione pelo menos um agrupador!');
				return false;
			}
			
		    //verificando checked, tornando variável existQapid = true para validação.
			var existQapid = false;
			var total = document.getElementsByName('qapid[]').length;
			for(var i=0; i < total; i++){
				if ( document.getElementsByName('qapid[]')[i].checked ){
					existQapid = true;
					break;//parando o loop
				}
			}
			
			if ( !existQapid ){
				alert('Você deve selecionar pelo menos uma questão');
				return false;
			}

			
			formulario.action = 'pdeescola.php?modulo=relatorio/relAutoavaliacao_resultado&acao=A&tipoRelatorio=xls';
			
		}else if ( tipo == 'salvar' ){
			
			if ( formulario.titulo.value == '' ) {
				alert( 'É necessário informar a descrição do relatório!' );
				formulario.titulo.focus();
				return;
			}
			var nomesExistentes = new Array();
			<?php
				$sqlNomesConsulta = "SELECT prtdsc FROM public.parametros_tela";
				$nomesExistentes = $db->carregar( $sqlNomesConsulta );
				if ( $nomesExistentes ){
					foreach ( $nomesExistentes as $linhaNome )
					{
						print "nomesExistentes[nomesExistentes.length] = '" . str_replace( "'", "\'", $linhaNome['prtdsc'] ) . "';";
					}
				}
			?>
			var confirma = true;
			var i, j = nomesExistentes.length;
			for ( i = 0; i < j; i++ ){
				if ( nomesExistentes[i] == formulario.titulo.value ){
					confirma = confirm( 'Deseja alterar a consulta já existente?' );
					break;
				}
			}
			if ( !confirma ){
				return;
			}
			formulario.action = 'pdeescola.php?modulo=relatorio/relAutoavaliacao&acao=A&salvar=1';
			formulario.target = '_self';
	
		}else if( tipo == 'exibir' ){
			if (formulario.elements['agrupador'][0] == null){
				alert('Selecione pelo menos um agrupador!');
				return false;
			}
			 
		    //verificando checked, tornando variável existQapid = true para validação.
			var existQapid = false;
			var total = document.getElementsByName('qapid[]').length;
			for(var i=0; i < total; i++){
				if ( document.getElementsByName('qapid[]')[i].checked ){
					existQapid = true;
					break;//parando o loop
				}
			}
			
			if ( !existQapid ){
				alert('Você deve selecionar pelo menos uma questão');
				return false;
			}

			
			selectAllOptions( formulario.agrupador );
			selectAllOptions( formulario.f_regiao );	
			selectAllOptions( formulario.f_estuf );	
			selectAllOptions( formulario.f_municipio );	
			selectAllOptions( formulario.f_inep );
			
			formulario.target = 'relAutoavaliacao_resultado';
			var janela = window.open( '', 'relAutoavaliacao_resultado', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			janela.focus();
		}
	}
	
	formulario.submit();
	
}

function carregar_relatorio( prtid ){
	document.filtro.prtid.value = prtid;
	gerarRelatorioAutoavaliacao( 'relatorio' );
}

/* Função para substituir todos */
function replaceAll(str, de, para){
    var pos = str.indexOf(de);
    while (pos > -1){
		str = str.replace(de, para);
		pos = str.indexOf(de);
	}
    return (str);
}
/* Função para adicionar linha nas tabelas */

/* CRIANDO REQUISIÇÃO (IE OU FIREFOX) */
function criarrequisicao() {
	return window.XMLHttpRequest ? new XMLHttpRequest() : new ActiveXObject( 'Msxml2.XMLHTTP' );
}
/* FIM - CRIANDO REQUISIÇÃO (IE OU FIREFOX) */

/* FUNÇÃO QUE TRATA O RETORNO */
var pegarretorno = function () {
	try {
			if ( evXmlHttp.readyState == 4 ) {
				if ( evXmlHttp.status == 200 && evXmlHttp.responseText != '' ) {
					// criando options
					var x = evXmlHttp.responseText.split("&&");
					for(i=1;i<(x.length-1);i++) {
						var dados = x[i].split("##");
						document.getElementById('usrs').options[i] = new Option(dados[1],dados[0]);
					}
					var dados = x[0].split("##");
					document.getElementById('usrs').options[0] = new Option(dados[1],dados[0]);
					document.getElementById('usrs').value = cpfselecionado;
				}
				if ( evXmlHttp.dispose ) {
					evXmlHttp.dispose();
				}
				evXmlHttp = null;
			}
		}
	catch(e) {}
};
/* FIM - FUNÇÃO QUE TRATA O RETORNO */

//function gerarRelatorio(){
//	var formulario = document.formulario;
//	
//	if (formulario.elements['agrupador'][0] == null){
//		alert('Selecione pelo menos um agrupador!');
//		return false;
//	}	
//
//    //verificando checked, tornando variável existQapid = true para validação.
//	var existQapid = false;
//	var total = document.getElementsByName('qapid[]').length;
//	for(var i=0; i < total; i++){
//		if ( document.getElementsByName('qapid[]')[i].checked ){
//			existQapid = true;
//			break;//parando o loop
//		}
//	}
//	
//	if ( !existQapid ){
//		alert('Você deve selecionar pelo menos uma questão');
//		return false;
//	}
//
//
//	
//	selectAllOptions( formulario.agrupador );
//	selectAllOptions( formulario.f_regiao );	
//	selectAllOptions( formulario.f_estuf );	
//	selectAllOptions( formulario.f_municipio );	
//	selectAllOptions( formulario.f_inep );
//	
//	var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
//	formulario.target = 'relatorio';	
//	formulario.submit();
//	janela.focus();
//}

/**
 * Alterar visibilidade de um campo.
 * 
 * @param string indica o campo a ser mostrado/escondido
 * @return void
 */
function onOffCampo( campo )
{
	var div_on = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input = document.getElementById( campo + '_campo_flag' );
	if ( div_on.style.display == 'none' )
	{
		div_on.style.display = 'block';
		div_off.style.display = 'none';
		input.value = '1';
	}
	else
	{
		div_on.style.display = 'none';
		div_off.style.display = 'block';
		input.value = '0';
	}
}

function selecionar_tudo(id){
	$("input[id^='qapid_"+id+"']").each(function(){
		this.checked = true;
	});
	}

function deselecionar_tudo(id){
	$("input[id^='qapid_"+id+"']").each(function(){
		this.checked = false;
	});
	} 
</script>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
</head>
<body>
<!--<form name="formulario" id="formulario" action="pdeescola.php?modulo=relatorio/relAutoavaliacao&acao=A" method="post">	-->
<!--<input type="hidden" name="pesquisa" value="1"/>-->


<form action="" method="post" name="formulario" id="formulario"> 
	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
		<tr>
			<td class="SubTituloDireita" valign="top">Agrupadores</td>
			<td>
				<?
				$matriz = agrupador();
				$campoAgrupador = new Agrupador( 'formulario' );
				$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
				$campoAgrupador->setDestino( 'agrupador');
				$campoAgrupador->exibir();
				?>
			</td>
		</tr>
		<!-- FILTROS -->	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'regiao' );">
				Região
				<input
					type="hidden"
					id="regiao_campo_flag"
					name="regiao_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="regiao_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'regiao' );">
					<img src="../imagens/combo-todos.gif" border="0" align="middle">
				</div>
				<div id="regiao_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo = "select
										regcod as codigo,
										regdescricao as descricao
									  from territorios.regiao
									  order by
										regdescricao";
						
						combo_popup(
							'f_regiao',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione a(s) Região(ões)',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							false,							// habilitar busca por código
							true							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>				
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'estuf' );">
				Estado
				<input
					type="hidden"
					id="estuf_campo_flag"
					name="estuf_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="estuf_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'estuf' );"
				><img src="../imagens/combo-todos.gif" border="0" align="middle"></div>
				<div id="estuf_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT 
										estuf AS codigo,
										estuf || ' - ' || estdescricao AS descricao
									FROM
									    territorios.regiao r   
										INNER JOIN territorios.estado e ON e.regcod = r.regcod	 									
									ORDER BY
										estuf";
						
						combo_popup(
							'f_estuf',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Estado(s)',	// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							false,							// habilitar busca por código
							true,							// habilitar flag contém
							10,
							400,
							'',
							'',
							'',
							array(
								  array(
								  		"codigo" 	=> "r.regdescricao",
								  		"descricao" => "Região"
								  		)
								  )							
						);
					?>
				</div>
			</td>
		</tr>	
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'municipio' );">
				Município
				<input
					type="hidden"
					id="municipio_campo_flag"
					name="municipio_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="municipio_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'municipio' );">
					<img src="../imagens/combo-todos.gif" border="0" align="middle">
				</div>
				<div id="municipio_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php
						$sql_combo ="SELECT
										--DISTINCT
										m.muncod AS codigo,
										m.estuf || ' - ' || mundescricao AS descricao
									FROM
										territorios.municipio m
									ORDER BY
										m.estuf";
						
						combo_popup(
							'f_municipio',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) município(s)',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							false,							// habilitar busca por código
							true,
							10,
							400,
							'',
							'',
							'',
							array(
								  array(
								  		"codigo" 	=> "m.estuf",
								  		"descricao" => "Unidade de federação"
								  		)
								  )							
							// habilitar flag contém
						);
					?>
				</div>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top">Dependência administrativa</td>
			<td>
				<?
					$sql = "SELECT
								'1,3' AS codigo,
								'Todas' AS descricao
							UNION ALL
							SELECT
								'1' AS codigo,
								'Estadual' AS descricao									
							UNION ALL
							SELECT 
								'3' AS codigo,
								'Municipal' AS descricao";
					$db->monta_combo( "tpcid", $sql, 'S', 'Selecione o Tipo', '', '', '', 215, 'S' );
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita">Questões</td>
			<td>
				<?php

					// Exibe questões
						
						$questoesI = $db->carregar("SELECT 
														qapid, 
														qapcodigo 
													FROM 
														pdeescola.questaoavaliacaoplano 
													WHERE 
														qapidpai in ('2','8','14','21') 
													ORDER BY 
														qapcodigo");
						
						$count = count($questoesI);
						
						echo'<b>Parte I</b> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:selecionar_tudo(1)">Marcar todas as questões Parte I</a> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:deselecionar_tudo(1)">Desmarcar</a> <br>';
						for($i = 0; $i < $count; $i++){
							echo '<input type="checkbox" id="qapid_1" name="qapid[]" value="' . $questoesI[$i]['qapid'] . '"/> ' . $questoesI[$i]["qapcodigo"] . '</label>&nbsp;';
							
						}
						
						echo '<br><br>';
						
						$questoesII = $db->carregar("SELECT 
														qapid, 
														qapcodigo 
													 FROM 
													 	pdeescola.questaoavaliacaoplano 
													 WHERE 
													 	qapidpai in ('29','37','44','56') 
													 ORDER BY 
													 	qapcodigo");
						
						$count = count($questoesII);
						echo'<b>Parte II</b> &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:selecionar_tudo(2)">Marcar todas as questões Parte II</a>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;<a href="javascript:deselecionar_tudo(2)">Desmarcar</a> <br>';
						for($i = 0; $i < $count; $i++){
							echo '<input type="checkbox" id="qapid_2" name="qapid[]" value="' . $questoesII[$i]['qapid'] . '"/> ' . $questoesII[$i]["qapcodigo"] . '</label>&nbsp;';
							
						}
						echo '<br>';
				?>
			</td>
		</tr>
		<tr>
			<td class="SubTituloDireita" valign="top" onclick="javascript:onOffCampo( 'inep' );">
				Código INEP
				<input
					type="hidden"
					id="inep_campo_flag"
					name="inep_campo_flag"
					value="<?php echo $possuiSelecionado ? "1" : "0" ?>"
				/>
			</td>
			<td>
				<div
					id="inep_campo_off"
					style="color:#a0a0a0;display:<?php echo !$possuiSelecionado ? "block" : "none" ?>"
					onclick="javascript:onOffCampo( 'inep' );">
					<img src="../imagens/combo-todos.gif" border="0" align="middle">
				</div>
				<div id="inep_campo_on" style="display:<?php echo $possuiSelecionado ? "block" : "none" ?>;">
					<?php

						$sql_combo = " SELECT 
											pdecodinep as codigo,
											pdecodinep ||'-'|| pdenome as descricao
									   FROM
											entidade.entidade as ee
									   INNER JOIN
											pdeescola.pdeescola as pp ON pp.pdecodinep = ee.entcodent
									   ORDER BY
									   		pdecodinep";
						
						combo_popup(
							'f_inep',						// nome do campo
							$sql_combo,						// sql de captura dos valores
							'Selecione o(s) Código(s) INEP',		// label
							'400x400',						// tamanho do popup
							0,								// quantidade máxima de itens
							array(),						// valores fixos
							'',								// mensagem
							'S',							// habilitado
							true,							// habilitar busca por código
							true
							);
					?>
				</div>
			</td>
		</tr>		
					
		<tr>
			<td align="center" colspan="2">
				<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="gerarRelatorioAutoavaliacao('exibir');"/>
				<input type="button" value="Visualizar XLS" onclick="gerarRelatorioAutoavaliacaoXLS();" style="cursor: pointer;"/>
			</td>
		</tr>
</table>		
</form>
</body>
</html>
<?php
function agrupador(){
	return array(
				array('codigo' => 'regiao',
					  'descricao' => 'Regiao'),
				array('codigo' => 'estado',
					  'descricao' => 'Estado'),
				array('codigo' => 'municipio',
					  'descricao' => 'Município'),	
				array('codigo' => 'inep',
					  'descricao' => 'Codigo INEP')						
			);
}
?>