<?

// Requisição Ajax ----------------------------------------------
if( isset($_POST['requisicaoAjax']) ){
	comboCurso();
	exit;
}


function pega_sql_busca(){
	$sql_busca = "
		SELECT 
			p.curdesc || ' - ' || p.ncudesc || ' - ' || p.moddesc as cursonivelmodalidade,
			sum(p.privagassolicitadas) as demanda
		FROM 
			snf.prioridadecursoescola p
		INNER JOIN snf.prioridadedocumento pd using(prdid)
		INNER JOIN workflow.documento d using(docid)
		WHERE 
			p.pristatus = 'A'
			and d.esdid = '464'
			and p.estuf = '". $_POST['estuf'] ."'
	";
	if( !empty($_POST['ateid']) ){
		$sql_busca .= " and p.ateid = '".$_POST['ateid']."' ";
	}
	if( 
		( 
			is_array($_POST['curid']) 
			&& ( 
				count($_POST['curid']) > 0 
				&& !empty($_POST['curid'][0]) 
			) 
		)
		|| !is_array($_POST['curid'])
	){
		$sql_busca .= ' and ';
		$contador = 0;
		foreach ( $_POST['curid'] as $key => $value ){
			if( $contador > 0 ){ $sql_busca .= " or "; }
			$sql_busca .= " p.curid = '".$value."' ";
			$contador++;
		}
	}
	// print_r($_POST['curid']);exit();
	$sql_busca .= "
		GROUP BY cursonivelmodalidade
		ORDER BY cursonivelmodalidade ;
	";
	return $sql_busca;
}

// Gera XLS -------------------------------------------------------------
if( $_GET['gera_xls'] ){
	
	if( $_GET['gera_xls'] ){ // construcao de post para geracao de XLS
		$_POST = $_SESSION['form_xls'];
	}

	global $db;
	$cabecalho = array( "Curso - Nível - Modalidade", "Demanda");
	ob_clean();
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_RelatME".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_RelatME".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$sql_busca = pega_sql_busca();
	$db->monta_lista_tabulado($sql_busca,$cabecalho,1000000000,5,'N','100%', 'S');
	exit;
}


// Chamada de programa DEFAULT ---------------------------------------
include  APPRAIZ."includes/cabecalho.inc";
echo '<br>';
$db->cria_aba($abacod_tela,$url,'');
// ver($_SESSION['favurl']);exit;

// Listas -----------------------------------------------------------
function comboUFs($estuf = null) {
	global $db;
	$sql = "
		SELECT estuf AS codigo,
		       estdescricao AS descricao
		FROM territorios.estado
		ORDER BY estuf
	";
	$db->monta_combo('estuf', $sql, 'S', 'Selecione', 'comboMunicipios', '', '', '140', 'S', 'estuf', false, $estuf);
}
// print_r($_POST);exit;
$ateid = (isset($_POST['ateid']))?$_POST['ateid']:'';
function comboAreaTematica( $ateid = '' ){
	global $db;
	$sql = "
		SELECT ateid as codigo,
		       atedesc as descricao
		FROM catalogocurso.areatematica
		WHERE atestatus = 'A'
		ORDER BY atedesc
	";
	$db->monta_combo('ateid', $sql, 'S', 'Selecione', 'comboCursos', '', '', '300', 'N', 'ateid', false, $ateid);
}

function comboCurso(  ){
	global $db;
	$sql = "
		SELECT curid as codigo,
		       curdesc as descricao
		FROM catalogocurso.curso
		WHERE curstatus = 'A'
		AND   ateid     = ".$_POST['ateid']."
		ORDER BY curdesc
	";
	combo_popup("curid", $sql, "Curso", "400x400", 0, '', "", "S", false, false, 5, 400);
}


// Validação server-side ---------------------------------------
if( isset($_POST['estuf']) &&  empty($_POST['estuf']) ){
	$aviso_estuf = 'campo obrigatório';
	unset($_POST['estuf']);
}
//print_r($_POST);exit;

 
// Busca ---------------------------------------
if( isset($_POST['estuf']) ){

	$sql_busca = pega_sql_busca();
	$cabecalho = array(
		'Curso - Nível - Modalidade', 'Demanda');
	$pesquisa = true;
}



monta_titulo( 'Relatório de Demandas por Estado', '&nbsp;' );
?>
<script type='text/javascript' src='../includes/JQuery/jquery-1.7.2.min.js'></script>
<script language="javascript" type="text/javascript">

function funcao_valida(){
	
	if( document.getElementById('curid') ){
		selectAllOptions( document.getElementById('curid') );
	}

	if( $('#estuf').val() == '' ){
		$('#aviso_estuf').html('campo obrigatório');
		return false;
	}else{
		return true;
	}

	return false;
	
}

function comboCursos(ateid){
	var url = window.location;
	var data = { requisicaoAjax:1, ateid:ateid };

	$.ajax({
		type: "POST",
		url: url,
		data: data,
		success: function(retorno){
			$('#td_curid').html(retorno);
		}
	});
	
}

</script>

<form action='' onsubmit='javascript:return funcao_valida()' method='post' id='form_filtro' name='form_filtro'>
<table id="" class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
	<tr>
		<td class="SubTituloDireita" width="10%">Unidade Federativa:</td>
		<td>
			<?php comboUFs( $_POST['estuf'] ); ?>
		<span id='aviso_estuf'><?= (isset($aviso_estuf))?$aviso_estuf:'' ?></span></td>
	</tr>
	<tr>
		<td class="SubTituloDireita" width="10%">Área Temática:</td>
		<td><?php comboAreaTematica(); ?></td>
	</tr>
	<tr>
		<td class="SubTituloDireita">Curso:</td>
		<td id="td_curid"><?php if( isset($_POST['ateid']) ) comboCurso(); ?></td>
	</tr>
	<tr>
		<td colspan='2' style='text-align:right;'>

			<?php if( $_POST['estuf'] ){ 
				$_SESSION['form_xls'] = $_POST;?>
				<a href='<?= $_SESSION['favurl'] ?>&gera_xls=1' target='_blank'>Extrair Planilha Excel</a>
			<?php } ?>

		</td>
	</tr>
	<tr>
		<td align="center" colspan="2">
			<input type="submit" name="btn_pesquisar" value="Pesquisar" />
		</td>
		<td></td>
	</tr>
</table>
</form>

<?php if( isset($pesquisa) ){ $db->monta_lista($sql_busca, $cabecalho, 20, 20, 'N', '70%'); } ?>