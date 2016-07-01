<?php

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
		var tipo = '<?=$_REQUEST['tipo']?>';
		location.href = '?modulo=relatorio/formRelPreen&acao=A&tipo=' + tipo;
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
			var tipo = '<?=$_REQUEST['tipo']?>';
			location.href = '?modulo=relatorio/formRelPreen&acao=A&tipo=' + tipo;
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
		$_POST = $dados;//array_merge( $_REQUEST, $dados );
		unset( $_POST['salvar'] );
	}
	switch($_POST['pesquisa']) {
		case '1':
			include "resultRelPreen.inc";
			exit;
		case '2':
			include "resultRelPreenxls.inc";
			exit;
	}
	
}

// carrega consulta do banco
if ( $_REQUEST['prtid'] && $_REQUEST['carregar'] == 1 ){
	
	$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = ".$_REQUEST['prtid'] );
	$itens = $db->pegaUm( $sql );
	$dados = unserialize( stripslashes( stripslashes( $itens ) ) );
	extract( $dados );
	$_REQUEST = $dados;
	unset( $_REQUEST['form'] );
	unset( $_REQUEST['pesquisa'] );
	$titulo = $_REQUEST['titulo'];
	
	$agrupador2 = array();
	
	if ( $_REQUEST['agrupador'] ){
		
		foreach ( $_REQUEST['agrupador'] as $valorAgrupador ){
			array_push( $agrupador2, array( 'codigo' => $valorAgrupador, 'descricao' => $valorAgrupador ));
		}
		
	}
	
}


if ( isset( $_REQUEST['pesquisa'] ) || isset( $_REQUEST['tipoRelatorio'] ) ){
	switch($_REQUEST['pesquisa']) {
		case '1':
			include "resultRelPreen.inc";
			exit;
		case '2':
			include "resultRelPreenxls.inc";
			exit;
	}
}




/*
if ($_POST['agrupador']){
	ini_set("memory_limit","256M");
	include("resultRelPreen.inc");
	exit;
}
*/
include APPRAIZ. '/includes/Agrupador.php';
include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';

monta_titulo( 'Relatório de Preenchimento - PAR', '&nbsp;' );
?>
<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<meta http-equiv="Content-Type" content="text/html; charset=ISO-8859-1">
<title>Relatório</title>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<script type="text/javascript" src="../includes/prototype.js"></script>
<script type="text/javascript"><!--

function gerarRelatorioXLS(){
		
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );
	
	// Tipo de relatorio
	formulario.pesquisa.value='2';
	
	prepara_formulario();
	selectAllOptions( formulario.agrupador );

	
	if ( !agrupador.options.length ){
		alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
		return false;
	}
	
	selectAllOptions( formulario.agrupador );
	selectAllOptions( formulario.estado );
	selectAllOptions( formulario.municipio );
	selectAllOptions( formulario.subacao );
	selectAllOptions( formulario.itemcomposicao );
	
	formulario.submit();
	
}

function gerarRelatorio( tipo ){
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );

	// Tipo de relatorio
	formulario.pesquisa.value='1';

		
	prepara_formulario();
	selectAllOptions( formulario.agrupador );

	if ( tipo == 'relatorio' ){
		
			formulario.action = 'par.php?modulo=relatorio/formRelPreen&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
					
	} else {

		if ( tipo == 'salvar' ){
				
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
			formulario.action = 'par.php?modulo=relatorio/formRelPreen&acao=A&salvar=1';
			formulario.target = '_self';
			
		}else if( tipo == 'exibir' ){
			
			if (formulario.elements['agrupador'][0] == null){
				alert('Selecione pelo menos um agrupador!');
				return false;
			}	

			selectAllOptions( formulario.agrupador );
			selectAllOptions( formulario.estado );
			selectAllOptions( formulario.municipio );
			selectAllOptions( formulario.subacao );
			selectAllOptions( formulario.itemcomposicao );
					
			
		
			var janela = window.open( '', 'relatorio', 'width=900,height=600,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';	
			janela.focus();
				
		}
	}
	formulario.submit();
}

/**
	 * Alterar visibilidade de um bloco.	 
	 * @param string indica o bloco a ser mostrado/escondido
	 * @return void
	 */
	function onOffBloco( bloco )
	{
		var div_on = document.getElementById( bloco + '_div_filtros_on' );
		var div_off = document.getElementById( bloco + '_div_filtros_off' );
		var img = document.getElementById( bloco + '_img' );
		var input = document.getElementById( bloco + '_flag' );
		if ( div_on.style.display == 'none' )
		{
			div_on.style.display = 'block';
			div_off.style.display = 'none';
			input.value = '0';
			img.src = '/imagens/menos.gif';
		}
		else
		{
			div_on.style.display = 'none';
			div_off.style.display = 'block';
			input.value = '1';
			img.src = '/imagens/mais.gif';
		}
	}
	
	/**
	 * Alterar visibilidade de um campo.	 
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

function carregaTipo(tipo) {
	window.location.href='/par/par.php?modulo=relatorio/formRelPreen&acao=A&tipo='+tipo;
}

function carregaFrmid(frmid, tipo) {
	window.location.href='/par/par.php?modulo=relatorio/formRelPreen&acao=A&tipo='+tipo+'&frmid='+frmid;
}

function tornar_publico( prtid ){
	document.formulario.publico.value = '1';
	document.formulario.prtid.value = prtid;
	document.formulario.target = '_self';
	document.formulario.submit();
}

function excluir_relatorio( prtid ){
	document.formulario.excluir.value = '1';
	document.formulario.prtid.value = prtid;
	document.formulario.target = '_self';
	document.formulario.submit();
}

function carregar_consulta( prtid ){
	document.formulario.carregar.value = '1';
	document.formulario.prtid.value = prtid;
	document.formulario.target = '_self';
	document.formulario.submit();
}

function carregar_relatorio( prtid ){
	document.formulario.prtid.value = prtid;
	gerarRelatorio( 'relatorio' );
}

//function ajaxRelatorio(){
//	var formulario = document.formulario;
//	var divRel 	   = document.getElementById('resultformulario');
//
//	divRel.style.textAlign='center';
//	divRel.innerHTML = 'carregando...';
//
//	var agrupador = new Array();	
//	for(i=0; i < formulario.agrupador.options.length; i++){
//		agrupador[i] = formulario.agrupador.options[i].value; 
//	}	
//	
//	var tipoensino = new Array();
//	for(i=0; i < formulario.f_tipoensino.options.length; i++){
//		tipoensino[i] = formulario.f_tipoensino.options[i].value; 
//	}		
//	
//	var param =  '&agrupador=' + agrupador + 
//				 '&f_tipoensino=' + tipoensino +
//				 '&f_tipoensino_campo_excludente=' + formulario.f_tipoensino_campo_excludente.checked +
//				 '&f_tipoensino_campo_flag=' + formulario.f_tipoensino_campo_flag.value;
//	 
//    var req = new Ajax.Request('academico.php?modulo=inicio&acao=C', {
//						        method:     'post',
//						        parameters: param,
//						        onComplete: function (res)
//						        {
//							    	divRel.innerHTML = res.responseText;
//						        }
//							});
//	
//}
--></script>
</head>
<body>
<?
	if( $_REQUEST['tipo'] == 'est' ){
		$ck1 = "checked=checked";
	} else if( $_REQUEST['tipo'] == 'mun' ){
		$ck2 = "checked=checked";
	}

	if( $_REQUEST['frmid'] == '1' ){
		$ck3 = "checked=checked";
	} else if( $_REQUEST['frmid'] == '2' ){
		$ck4 = "checked=checked";
	}
?>
<form name="formulario" id="formulario" action="" method="post">	
<input type="hidden" name="form" value="1"/>
<input type="hidden" name="pesquisa" value="1"/>
<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Selecione o Tipo de Relatório</td>
		<td><input type="radio" name="tipo" <? echo $ck1; ?> onclick="javascript:carregaTipo(this.value);" value="est">Estadual<input type="radio" name="tipo" <? echo $ck2; ?> onclick="javascript:carregaTipo(this.value);" value="mun">Municipal</td>
	</tr>
	<?php if( $_REQUEST['tipo'] ){ ?>
	<tr>
		<td class="SubTituloDireita" valign="top">Forma de Execução</td>
		<td><input type="radio" name="frmid" <? echo $ck3; ?> onclick="javascript:carregaFrmid(this.value,'<? echo $_REQUEST['tipo']; ?>');" value="1">Técnica<input type="radio" name="frmid" <? echo $ck4; ?> onclick="javascript:carregaFrmid(this.value,'<? echo $_REQUEST['tipo']; ?>');" value="2">Financeira</td>
	</tr>
	<?php if( $_REQUEST['frmid'] ){ ?>
	<tr>
		<td class="SubTituloDireita" valign="top">Execução Própria</td>
		<td><input type="radio" name="execpropria" value="1">Sim<input type="radio" name="execpropria" checked=checked value="2">Não
	</tr>
	<tr>
		<td class="SubTituloDireita">Título</td>
		<td>
			<?= campo_texto( 'titulo', 'N', 'S', '', 65, 60, '', '', 'left', '', 0, 'id="titulo"'); ?>
		</td>
	</tr>
	<tr>
		<td class="SubTituloDireita" valign="top">Agrupadores</td>
		<td>
			<?
			$matriz = agrupador();
			$campoAgrupador = new Agrupador( 'formulario' );
			$campoAgrupador->setOrigem( 'agrupadorOrigem', null, $matriz );
			$campoAgrupador->setDestino( 'agrupador', null);
			$campoAgrupador->exibir();
			?>
		</td>
	</tr>
</table>
<!-- MINHAS CONSULTAS -->
		
		<table class="tabela" align="center" bgcolor="#e0e0e0" cellspacing="1" cellpadding="3" style="border-bottom:none;border-top:none;">
			<tr>
				<td onclick="javascript:onOffBloco( 'minhasconsultas' );" >
					<!-- -->  
					<img border="0" src="/imagens/mais.gif" id="minhasconsultas_img"/>&nbsp;
					Minhas Consultas
					<input type="hidden" id="minhasconsultas_flag" name="minhasconsultas_flag" value="0" />					
				</td>
			</tr>
		</table>		
		<div id="minhasconsultas_div_filtros_off">
		</div>
		<div id="minhasconsultas_div_filtros_on" style="display:none;">
			<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-top:none;">
					<tr>
						<td width="195" class="SubTituloDireita" valign="top">Consultas</td>
						<?php
							
							$sql = sprintf(
								"SELECT 
									CASE WHEN prtpublico = false THEN '<img border=\"0\" src=\"../imagens/grupo.gif\" title=\" Publicar \" onclick=\"tornar_publico(' || prtid || ')\">&nbsp;&nbsp;
																	   <img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
																	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
																 ELSE '<img border=\"0\" src=\"../imagens/preview.gif\" title=\" Carregar consulta \" onclick=\"carregar_relatorio(' || prtid || ')\">&nbsp;&nbsp;
																 	   <img border=\"0\" src=\"../imagens/excluir.gif\" title=\" Excluir consulta \" onclick=\"excluir_relatorio(' || prtid || ');\">' 
									END as acao, 
									'' || prtdsc || '' as descricao 
								 FROM 
								 	public.parametros_tela 
								 WHERE 
								 	mnuid = %d AND usucpf = '%s'",
								$_SESSION['mnuid'],
								$_SESSION['usucpf']
							);
							
							$cabecalho = array('Ação', 'Nome');
						
						?>
						<td>
							<?php $db->monta_lista_simples( $sql, $cabecalho, 50, 50, 'N', '80%', null ); ?>
						</td>
					</tr>
			</table>
		</div>		
<!-- FIM MINHAS CONSULTAS -->
<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3" style="border-bottom:none;">
	<tr>
		<td class="SubTituloDireita" valign="top">Anos</td>
		<td>
			<input type="checkbox" name="anos[]" value="2011">2011
			<input type="checkbox" name="anos[]" value="2012">2012
			<input type="checkbox" name="anos[]" value="2013">2013
			<input type="checkbox" name="anos[]" value="2014">2014
		</td>
	</tr>
		<?php
			
				// Filtros do relatório
				$stSql = "SELECT
							estuf AS codigo,
							estdescricao AS descricao
						  FROM
						  	territorios.estado";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Estado', 'estado',  $stSql, $stSqlCarregados, 'Selecione o(s) Estado(s)', array(), '' , '', '', array('descricao'), array('descricao') );

			if( $_REQUEST['tipo'] == 'mun' ){
				$stSql = "SELECT
							muncod AS codigo,
							estuf || ' - ' || replace(mundescricao, '\'', '') AS descricao
						  FROM
						  	territorios.municipio	
						  WHERE
						  	1=1
						  ORDER BY
						  	descricao";
				$stSqlCarregados = "";
				$where = array(
								array(
										"codigo" 	=> "estuf",
										"descricao" => "Estado <b style='size: 8px;'>(sigla)</b>",
										"numeric"	=> false
							   		  )
							   );
				mostrarComboPopup( 'Município', 'municipio',  $stSql, $stSqlCarregados, 'Selecione o(s) Município(s)', $where, '' , '', '', array('descricao'), array('descricao') );
				
				$stSql = "select * from (
							(select 
								tpmid as codigo,
								case when tpmid = 140 then 'Municípios de até 10.000 habitantes' when tpmid = 141 then 'Municípios de 10.001 a 20.000 habitantes' else tpmdsc end as descricao
							from  
								territorios.tipomunicipio
							where 
								tpmid IN (1, 16, 17, 139, 140, 141, 150, 151, 152, 154, 162, 167)
							)								
							union
							(select 
								gtmid as codigo,
								gtmdsc as descricao 
							from  
								territorios.grupotipomunicipio
							where 
								gtmid = 5)
							order by descricao) as tbl";
					$stSqlCarregados = "";

				mostrarComboPopup( 'Grupo de Municípios', 'grupo',  $stSql, $stSqlCarregados, 'Selecione o(s) Grupo(s) de Município(s)', $where);
			}
			
			$itrid = $_REQUEST['tipo'] == 'mun' ? 2 : 1;
			
				$stSql = "SELECT 
							ppsid as codigo,
							d.dimcod || '.' || a.arecod || '.' || i.indcod || '.' || pps.ppsordem || ' - ' || pps.ppsdsc as descricao
						FROM par.dimensao d
						INNER JOIN par.area a ON a.dimid = d.dimid
						INNER JOIN par.indicador i ON i.areid = a.areid
						INNER JOIN par.propostasubacao pps ON pps.indid = i.indid
						WHERE
							1=1 AND
							d.itrid = {$itrid}
						ORDER BY
							descricao";
				$stSqlCarregados = "";
				mostrarComboPopup( 'Subação', 'subacao',  $stSql, $stSqlCarregados, 'Selecione a(s) Subação(ões)');

				if( $_REQUEST['frmid'] == '2' ){
					$stSql = "SELECT 
								picid as codigo,
								picdescricao as descricao 
							FROM 
								par.propostaitemcomposicao
							WHERE
								picstatus = 'A'
							ORDER BY
								picdescricao";
					$stSqlCarregados = "";
					mostrarComboPopup( 'Itens de Composição', 'itemcomposicao',  $stSql, $stSqlCarregados, 'Selecione o(s) Item(ns) de Composição', array(), '' , '', '', array('descricao'), array('descricao') ); 
				}
		?>	
	<tr>
		<td align="center" colspan="2">
			<input type="button" name="Gerar Relatório" value="Gerar Relatório" onclick="javascript:gerarRelatorio('exibir');" 	style="cursor: pointer;"/>
			<input type="button" value="Visualizar XLS"  onclick="gerarRelatorioXLS();" 										style="cursor: pointer;"/>
			<input type="button" value="Salvar Consulta" onclick="javascript:gerarRelatorio('salvar');" 						style="cursor: pointer;"/>
		</td>
	</tr>
	<?php } } ?>
</table>
</form>
</body>
</html>
<?
function agrupador(){
	if( $_REQUEST['tipo'] == 'est' && $_REQUEST['frmid'] == '1' ){
		return array(
			array('codigo' 	  => 'estado',
				  'descricao' => 'Estado'),	
			array('codigo' 	  => 'subacao',
				  'descricao' => 'Subação'),
			array('codigo' 	  => 'ano',
				  'descricao' => 'Ano')
			);
	} elseif( $_REQUEST['tipo'] == 'est' && $_REQUEST['frmid'] == '2' ) {
		return array(
			array('codigo' 	  => 'estado',
				  'descricao' => 'Estado'),	
			array('codigo' 	  => 'subacao',
				  'descricao' => 'Subação'),
			array('codigo' 	  => 'item',
				  'descricao' => 'Item de Composição'),
			array('codigo' 	  => 'ano',
				  'descricao' => 'Ano')
			);
	} elseif( $_REQUEST['tipo'] == 'mun' && $_REQUEST['frmid'] == '1' ){
		return array(
			array('codigo' 	  => 'estado',
				  'descricao' => 'Estado'),	
			array('codigo' 	  => 'municipio',
				  'descricao' => 'Município'),
			array('codigo' 	  => 'subacao',
				  'descricao' => 'Subação'),
			array('codigo' 	  => 'ano',
				  'descricao' => 'Ano')
			);
	} else {
		return array(
			array('codigo' 	  => 'estado',
				  'descricao' => 'Estado'),	
			array('codigo' 	  => 'municipio',
				  'descricao' => 'Município'),
			array('codigo' 	  => 'subacao',
				  'descricao' => 'Subação'),
			array('codigo' 	  => 'item',
				  'descricao' => 'Item de Composição'),
			array('codigo' 	  => 'ano',
				  'descricao' => 'Ano')
			);
	}
}
?>