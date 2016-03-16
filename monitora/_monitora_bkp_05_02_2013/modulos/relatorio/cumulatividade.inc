<?php
/**
 * Sistema Simec
 * Setor responsável: MEC
 * Desenvolvedor: Equipe Consultores Simec
 * Analista: Henrique
 * Programador: Gustavo Fernandes da Guarda
 */

ini_set("memory_limit","250M");
set_time_limit(0);

$cabecalho = array("Ação","Funcional","Localizador", "Programa","Unidade","Produto","Unidade Medida", "Cumulatividade");

$where = array();

if($_POST['visualizar'] || $_POST['visualizarXls']){
	extract($_POST);
	
	// Ação
	if( $acacod[0] && $acacod_campo_flag ){
		array_push($where, " a.acacod in ('" . implode( "','", $acacod ) . "') ");
	}
	// Programa
	if( $prgcod[0] && $prgcod_campo_flag ){
		array_push($where, " pr.prgcod in ('" . implode( "','", $prgcod ) . "') ");
	}
	
	// Produto
	if( $procod[0] && $procod_campo_flag ){
		array_push($where, " p.procod in ('" . implode( "','", $procod ) . "') ");
	}
	
	// Unidade de Medida
	if( $unmcod[0] && $unmcod_campo_flag ){
		array_push($where, " um.unmcod in ('" . implode( "','", $unmcod ) . "') ");
	}
	
	// Cumulaticidade
	if( $cumulatividade ){
		array_push($where, " acasnmetanaocumulativa =  $cumulatividade");
	}
}
	
$sql_lista = "select  
		        acadsc,
		        acacod||'.'||a.unicod||'.'||loccod as funcional,
		        sacdsc as localizador,
		        pr.prgdsc,
		        unidsc,
		        prodsc,
		        unmdsc,
		        Case when acasnmetanaocumulativa = true then 'Não' else 'Sim' end as cumulatividade
			from monitora.acao a
				inner join monitora.programa pr ON pr.prgid = a.prgid and pr.prgano = a.prgano and pr.prgcod = a.prgcod
				inner join public.produto p ON p.procod = a.procod
				inner join public.unidademedida um ON um.unmcod = a.unmcod
				inner join public.unidade u ON u.unicod = a.unicod
			where a.prgano = '{$_SESSION['exercicio']}' and a.acasnrap = false 
			" . ( is_array($where) && count($where) ? ' AND ' . implode(' AND ', $where) : '' ) ."
		";

if ( $_POST['visualizarXls'] ){
	ini_set("memory_limit","250M");
	set_time_limit(0);
	
	$arDados = $db->carregar($sql_lista);
	$arDados = ($arDados) ? $arDados : array();
	
	header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
	header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
	header ( "Pragma: no-cache" );
	header ( "Content-type: application/xls; name=SIMEC_Cumulatividade".date("Ymdhis").".xls");
	header ( "Content-Disposition: attachment; filename=SIMEC_Cumulatividade".date("Ymdhis").".xls");
	header ( "Content-Description: MID Gera excel" );
	$db->monta_lista_tabulado($arDados,$cabecalho,100000,5,'N','100%',$par2);
    die;
}

include  APPRAIZ."includes/cabecalho.inc";
echo'<br>';

monta_titulo( $titulo_modulo, '' );
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script type="text/javascript">

function onOffCampo( campo ) {
	var div_on  = document.getElementById( campo + '_campo_on' );
	var div_off = document.getElementById( campo + '_campo_off' );
	var input   = document.getElementById( campo + '_campo_flag' );
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

$(document).ready(function(){
	$('#formulario').submit(function(){
		if($('#acacod_campo_flag').val() == "1"){
			selectAllOptions( document.getElementById('acacod') );
		}
		
		if($('#prgcod_campo_flag').val() == "1"){
			selectAllOptions( document.getElementById('prgcod') );
		}
		
		if($('#procod_campo_flag').val() == "1"){
			selectAllOptions( document.getElementById('procod') );
		}
		
		if($('#unmcod_campo_flag').val() == "1"){
			selectAllOptions( document.getElementById('unmcod') );
		}
	});	
});
</script>
<form method="post" name="formulario" id="formulario">
<input type="hidden" name="pesquisa" value="1" />
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
	<?php
	 	// Programa
		$sql = "select prgcod as codigo, 
					   prgcod || ' - ' || prgdsc as descricao
				from monitora.programa
					where prgano = '{$_SESSION['exercicio']}' and prgstatus = 'A'
					   order by	prgdsc";
		$stSqlCarregados = "";
		if($_POST['prgcod'][0]){
			$stSqlCarregados = "select prgcod as codigo, 
					   				   prgcod || ' - ' || prgdsc as descricao
								from monitora.programa
									where prgcod in ('" . implode( "','", $_POST['prgcod'] ) . "') AND prgano = '{$_SESSION['exercicio']}'
						   		order by prgdsc";
		}
		mostrarComboPopup( 'Programa', 'prgcod',  $sql, $stSqlCarregados, 'Selecione o(s) Programa(s)' );
		
		// Ação
		$sql = "select acacod as codigo, 
					   acacod || ' - ' || acadsc as descricao
				from monitora.acao
					where prgano = '{$_SESSION['exercicio']}' and acasnrap = false and acastatus = 'A'
					   order by	acadsc";
		$stSqlCarregados = "";
		if($_POST['acacod'][0]){
			$stSqlCarregados = "select distinct acacod as codigo, 
					   				   acacod || ' - ' || acadsc as descricao
								from monitora.acao
									where acacod in ('" . implode( "','", $_POST['acacod'] ) . "') AND prgano = '{$_SESSION['exercicio']}' and acasnrap = false
						   		order by descricao";
		}
		mostrarComboPopup( 'Ação', 'acacod',  $sql, $stSqlCarregados, 'Selecione a(s) Ação(ões)' );
		
		// Produto
		$sql = "select procod as codigo, 
					   procod || ' - ' || prodsc as descricao
				from public.produto
					where prostatus = 'A'
					   order by	prodsc";
		$stSqlCarregados = "";
		if($_POST['procod'][0]){
			$stSqlCarregados = "select procod as codigo, 
					   				   procod || ' - ' || prodsc as descricao
								from public.produto
									where procod in ('" . implode( "','", $_POST['procod'] ) . "')
						   		order by prodsc";
		}
		mostrarComboPopup( 'Produto', 'procod',  $sql, $stSqlCarregados, 'Selecione o(s) Produto(s)' );
		
		// Unidade de Medida
		$sql = "select unmcod as codigo, 
					   unmcod || ' - ' || unmdsc as descricao
				from public.unidademedida
					where unmstatus = 'A'
					   order by	unmdsc";
		$stSqlCarregados = "";
		if($_POST['unmcod'][0]){
			$stSqlCarregados = "select unmcod as codigo, 
					   				   unmcod || ' - ' || unmdsc as descricao
								from public.unidademedida
									where unmcod in ('" . implode( "','", $_POST['unmcod'] ) . "')
						   		order by unmdsc";
		}
		mostrarComboPopup( 'Unidade de Medida', 'unmcod',  $sql, $stSqlCarregados, 'Selecione a(s) Unidade de Medida(s)' );
	?>
   <?php
		$arCumulatividade = array(array("codigo" => "false", "descricao" => "Sim"),
						array("codigo" => "true", "descricao" => "Não"));
	?>
	<tr>
	    <td align='right' class="SubTituloDireita">Cumulatividade:</td>
	    <td><?=$db->monta_combo('cumulatividade',$arCumulatividade,'S','Selecione...',"",'','','','N','','',$cumulatividade);?></td>
	</tr>
	<tr bgcolor="#D0D0D0">
		<td colspan="2" style="text-align: center;">
			<input type="submit" name="visualizar" id="visualizar" value="Visualizar" />
			<input type="submit" name="visualizarXls" id="visualizarXls" value="Visualizar XLS" />
		</td>
	</tr>
</table>
</form>
<?php 
$db->monta_lista($sql_lista,$cabecalho,50,5,'N','95%',$par2);