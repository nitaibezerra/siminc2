<?php
header('content-type: text/html; charset=ISO-8859-1');

if ( isset( $_REQUEST['pesquisa'] ) ){
	
	if($_REQUEST['pesquisa'] == '1') include "resultado_tramite_os.inc";
	if($_REQUEST['pesquisa'] == '2') include "resultado_tramite_os_xls.inc";
	
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Tramites OS";
monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );


//recupera perfil do usuário
$perfil = arrayPerfil();
?>


<script src="../includes/calendario.js"></script>
<script language="JavaScript" src="../includes/funcoes.js"></script>


<script type="text/javascript">
<!--


	function exibeRelatorioGeral(tipo){
		
		var formulario = document.formulario;

		// Tipo de relatorio
		if(tipo=='html') formulario.pesquisa.value='1';
		if(tipo=='xls') formulario.pesquisa.value='2';

		if(formulario.dtinicio.value != '' && formulario.dtfim.value != ''){ 
			if (!validaDataMaior(formulario.dtinicio, formulario.dtfim)){
				alert("O Período de Abertura Início não pode ser maior que o Período de Abertura Fim.");
				formulario.dtfimsit.focus();
				return;
			}
		}
		else{
			alert( 'Informe o Período de Abertura da OS!' );
			return false;
		}

		selectAllOptions( document.getElementById( 'tipoos' ) );
		selectAllOptions( document.getElementById( 'situacao' ) );
		
		formulario.target = 'resultadoGeral';
		var janela = window.open( '', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
			
		
		
		formulario.submit();
		
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

	<input type="hidden" name="form" value="1"/>
	<input type="hidden" name="pesquisa" value=""/>

		<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		
			<?php
			
			
				// Tipo da Ordem de Serviço
				$tipoos = array();
				if ( $_REQUEST['tipoos'] && $_REQUEST['tipoos'][0] != '' )
				{
					$sql_carregados = "SELECT
								tosid AS codigo,
								tosdsc AS descricao
							FROM 
								fabrica.tipoordemservico
							WHERE
								tosstatus = 'A' 
							and tosid in (".implode(",",$_REQUEST['tipoos']).")
							ORDER BY
								2 ";
					$tipoos=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								tosid AS codigo,
								tosdsc AS descricao
							FROM 
								fabrica.tipoordemservico
							WHERE
								tosstatus = 'A' 
							ORDER BY
								2 ";
				mostrarComboPopup( 'Tipo da Ordem de Serviço:', 'tipoos',  $stSql, '', 'Selecione o(s) Tipo(s)' );			
			
				
				// situação OS
				$requisitante = array();
				if ( $_REQUEST['situacao'] && $_REQUEST['situacao'][0] != '' )
				{
					$sql_carregados = "SELECT esd.esdid as codigo,esd.esddsc as descricao
							FROM workflow.estadodocumento esd
							WHERE tpdid in (select tpdid from workflow.tipodocumento where tpdid = ".WORKFLOW_ORDEM_SERVICO." and sisid = ".SISID_FABRICA.")
                            AND esd.esdstatus = 'A'
                            and esd.esdid in (".implode(",",$_REQUEST['situacao']).")
							order by esd.esdordem";
					$requisitante=$db->carregar( $sql_carregados );
				}
				
				$stSql = "SELECT esd.esdid as codigo,esd.esddsc as descricao
							FROM workflow.estadodocumento esd
							WHERE tpdid in (select tpdid from workflow.tipodocumento where tpdid = ".WORKFLOW_ORDEM_SERVICO." and sisid = ".SISID_FABRICA.")
                            AND esd.esdstatus = 'A'
							order by esd.esdordem";
				
				mostrarComboPopup( 'Situação da OS:', 'situacao',  $stSql, '', 'Selecione a(s) Situação(ões)' );			
				
				
			?>
		<tr>
			<td class="SubTituloDireita" width="35%">Período Prev. Início da OS:</td>
			<td>
				<?= campo_data( 'dtinicio', 'S', 'S', '', '' ); ?>
				&nbsp;&nbsp;
				a
				&nbsp;&nbsp;&nbsp;
				<?= campo_data( 'dtfim', 'S', 'S', '', '' ); ?>
			</td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				 <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/>
				  &nbsp;&nbsp;&nbsp;
				 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>

</form>

