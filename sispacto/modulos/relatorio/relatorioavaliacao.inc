<?php

// exibe consulta
if($_REQUEST['tiporelatorio']){
	switch($_REQUEST['tiporelatorio']) {
		case 'html':
			include "relatorioavaliacao_resultado.inc";
			exit;
	}
	
}


include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
monta_titulo( "Relatório Avaliação", 'Selecione os filtros e agrupadores desejados' );

?>


<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>


<script type="text/javascript">

	
function exibeRelatorioGeral(tipo){
	
	var formulario = document.formulario;
	var agrupador  = document.getElementById( 'agrupador' );
	
	selectAllOptions( formulario.agrupador );
	
	// verifica se tem algum agrupador selecionado
	if ( !agrupador.options.length ) {
		alert( 'Escolha ao menos um item para agrupar o resultado.' );
		return false;
	}

	
    selectAllOptions(document.getElementById('uniid'));
    selectAllOptions(document.getElementById('estuf'));
    selectAllOptions(document.getElementById('muncod'));
    selectAllOptions(document.getElementById('fpbid'));
    selectAllOptions(document.getElementById('pflcod'));
    selectAllOptions(document.getElementById('esdid'));
	
	formulario.action = 'sispacto.php?modulo=relatorio/relatorioavaliacao&acao=A&tiporelatorio='+tipo;
	window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
	formulario.target = 'relatorio';

	
	formulario.submit();
	
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
				
/**
 * Alterar visibilidade de um bloco.
 * 
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
	
	
//-->
</script>


<form action="" method="post" name="formulario" id="filtro"> 

<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
<tr>
	<td class="SubTituloDireita" width="15%">Agrupadores:</td>
	<td>
	<?php
	// Início dos agrupadores
	$agrupador = new Agrupador('filtro','');
	
	// Dados padrão de destino (nulo)
	$destino = array();
	$destino = $agrupadorAux;
	
	$origem = array(
		'universidade' => array(
			'codigo'    => 'universidade',
			'descricao' => 'Universidade'
		),
		'perfilavaliador' => array(
			'codigo'    => 'perfilavaliador',
			'descricao' => 'Perfil avaliador'
		),
		'esfera' => array(
			'codigo'    => 'esfera',
			'descricao' => 'Esfera'
		),
		'municipio' => array(
			'codigo'    => 'municipio',
			'descricao' => 'Município'
		),
		'estado' => array(
			'codigo'    => 'estado',
			'descricao' => 'UF'
		),
		'avaliador' => array(
			'codigo'    => 'avaliador',
			'descricao' => 'Avaliador'
		)
		
		
	);
					
	// exibe agrupador
	$agrupador->setOrigem( 'naoAgrupador', null, $origem );
	$agrupador->setDestino( 'agrupador', null, $destino );
	$agrupador->exibir();
	?>
	</td>
</tr>
<?php
			

$perfis = pegaPerfilGeral();

if(in_array(PFL_COORDENADORIES,$perfis) && $_SESSION['sispacto']['universidade']['uncid']) {
	
	$universidade = $db->pegaLinha("SELECT uu.unisigla || ' / ' || uu.uninome as descricao, uu.uniid 
									FROM sispacto.universidadecadastro un 
									INNER JOIN sispacto.universidade uu ON uu.uniid = un.uniid 
									WHERE un.uncid='".$_SESSION['sispacto']['universidade']['uncid']."'");
	echo '<tr>';
	echo '<td class="SubTituloDireita">Universidades:</td>';
	echo '<td><input type="hidden" name="uniid_campo_flag" value="0"><select tipo="combo_popup" style="display:none;" multiple="multiple" name="uniid[]" id="uniid"><option value="'.$universidade['uniid'].'">'.$universidade['descricao'].'</option></select>'.$universidade['descricao'].'</td>';
	echo '</tr>';
	
} else {
	
	$stSql = "SELECT
					uniid AS codigo,
					uninome AS descricao
				FROM 
					sispacto.universidade
				ORDER BY
					2 ";
	mostrarComboPopup( 'Universidades:', 'uniid',  $stSql, '', 'Selecione a(s) Universidade(s)' );				
	
}

echo '<tr>';
echo '<td class="SubTituloDireita">Nome:</td>';
echo '<td>'.campo_texto('iusnome', "N", "S", "Nome", 45, 60, "", "", '', '', 0, 'id="iusnome"' ).'</td>';
echo '</tr>';



$stSql = "SELECT
				estuf AS codigo,
				estuf || ' / ' || estdescricao AS descricao
			FROM 
				territorios.estado
			ORDER BY
				2 ";
mostrarComboPopup( 'UFs:', 'estuf',  $stSql, '', 'Selecione a(s) UF(s)' );

$stSql = "SELECT
				muncod AS codigo,
				estuf || ' / ' || mundescricao AS descricao
			FROM 
				territorios.municipio
			ORDER BY
				2 ";
mostrarComboPopup( 'Municípios:', 'muncod',  $stSql, '', 'Selecione o(s) Município(s)' );				

$stSql = "SELECT f.fpbid as codigo, 'Parcela ( Ref. ' || m.mesdsc || ' / ' || fpbanoreferencia ||' )' as descricao 
			FROM sispacto.folhapagamento f 
			INNER JOIN public.meses m ON m.mescod::integer = f.fpbmesreferencia
			WHERE f.fpbstatus='A' ORDER BY fpbanoreferencia::text||m.mescod||'01'";

mostrarComboPopup( 'Parcelas:', 'fpbid',  $stSql, '', 'Selecione a(s) parcela(s)' );


$stSql = "SELECT p.pflcod as codigo, p.pfldsc as descricao 
			FROM seguranca.perfil p 
			INNER JOIN sispacto.pagamentoperfil pp ON pp.pflcod = p.pflcod 
			WHERE p.pflstatus='A' AND p.sisid=".SIS_SISPACTO." ORDER BY 2";

mostrarComboPopup( 'Perfis:', 'pflcod',  $stSql, '', 'Selecione o(s) perfil(s)' );
				

$stSql = "SELECT e.esdid as codigo, e.esddsc as descricao 
			FROM workflow.estadodocumento e 
			WHERE e.esdstatus='A' AND e.tpdid=".TPD_FLUXOMENSARIO." ORDER BY 2";

mostrarComboPopup( 'Situações das avaliações:', 'esdid',  $stSql, '', 'Selecione a(s) avaliacão(ões)' );

?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				 <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>

</form>