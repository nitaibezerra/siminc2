<?php
header('content-type: text/html; charset=ISO-8859-1');

if ( isset( $_REQUEST['pesquisa'] ) ){
	
	if($_REQUEST['pesquisa'] == '1') include "resultado_tramite.inc";
	if($_REQUEST['pesquisa'] == '2') include "resultado_tramite_xls.inc";
	
	exit();
}

include APPRAIZ . 'includes/cabecalho.inc';

echo "<br>";
$db->cria_aba($abacod_tela,$url,'');
$titulo_modulo = "Relatório de Tramites";
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

		if ( !document.getElementById('origemd').options[0].value ){
			alert( 'Favor selecionar ao menos uma Origem!' );
			return false;
		}

		if(formulario.dtinicio.value != '' && formulario.dtfim.value != ''){ 
			if (!validaDataMaior(formulario.dtinicio, formulario.dtfim)){
				alert("O Período de Abertura Início não pode ser maior que o Período de Abertura Fim.");
				formulario.dtfimsit.focus();
				return;
			}
		}
		else{
			alert( 'Informe o Período de Abertura da Demanda!' );
			return false;
		}

		selectAllOptions( document.getElementById( 'origemd' ) );
		selectAllOptions( document.getElementById( 'celula' ) );
		selectAllOptions( document.getElementById( 'sidid' ) );
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
			
			
				if(!$db->testa_superuser()){					 
			
					$ordidx = $db->carregarColuna("SELECT distinct ordid from demandas.usuarioresponsabilidade 
										where rpustatus='A' and usucpf = '".$_SESSION['usucpf']."' and ordid is not null
										union
										SELECT c.ordid from demandas.usuarioresponsabilidade ur
										inner join demandas.celula c on c.celid = ur.celid
										where rpustatus='A' and ur.celid is not null and usucpf = '".$_SESSION['usucpf']."'");
					if($ordidx){
						$whereOrdid = " AND ordid IN ('".implode("','",$ordidx)."')";
					}
					
					$celidx = $db->carregarColuna(" SELECT distinct celid from demandas.usuarioresponsabilidade where rpustatus = 'A' and usucpf = '".$_SESSION['usucpf']."' and not celid is null");
					if($celidx){
						$whereCelid = " AND ur.celid IN ('".implode("','",$celidx)."')";
					}
					
				}
								
				

				// Origem
				$origemd = array();
				if ( $_REQUEST['origemd'] && $_REQUEST['origemd'][0] != '' )
				{
					$sql_carregados = "SELECT
								ordid AS codigo,
								orddescricao AS descricao
							FROM 
								demandas.origemdemanda
							WHERE ordstatus = 'A'
							and ordid in (".implode(",",$_REQUEST['origemd']).")
							ORDER BY
								2 ";
					$origemd=$db->carregar( $sql_carregados );
				}
				$stSql = " SELECT
								ordid AS codigo,
								orddescricao AS descricao
							FROM 
								demandas.origemdemanda
							WHERE ordstatus = 'A'
							$whereOrdid
							ORDER BY
								2 ";
				mostrarComboPopup( 'Origem:', 'origemd',  $stSql, '', 'Selecione a(s) Origem(ns)' );


				//if( $db->testa_superuser() || in_array(1,$ordidx) ){
					// Célula
					$celula = array();
					if ( $_REQUEST['celula'] && $_REQUEST['celula'][0] != '' )
					{
						$sql_carregados = "SELECT
								 c.celid AS codigo,
								 c.celnome || ' - Gerente: ' || 
								 CASE 
								 	 WHEN u.usunome != '' THEN u.usunome
									 ELSE ' - '
								 END as DESCRICAO
								FROM
								 demandas.celula c
					 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
					 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
								WHERE
								 c.celstatus = 'A'
								and c.celid in (".implode(",",$_REQUEST['celula']).")
								ORDER BY
									c.celnome ";
						$celula=$db->carregar( $sql_carregados );
					}
					$stSql = " SELECT
								 c.celid AS codigo,
								 c.celnome || ' - Gerente: ' || 
								 CASE 
								 	 WHEN u.usunome != '' THEN u.usunome
									 ELSE ' - '
								 END as DESCRICAO
								FROM
								 demandas.celula c
					 			 LEFT JOIN demandas.usuarioresponsabilidade ur ON ur.celid = c.celid AND ur.pflcod = ".DEMANDA_PERFIL_GERENTE_PROJETO." AND ur.rpustatus = 'A' 
					 			 LEFT JOIN seguranca.usuario u ON u.usucpf = ur.usucpf 
								WHERE
								 c.celstatus = 'A'
								 $whereCelid
								ORDER BY
								 c.celnome; ";
					mostrarComboPopup( 'Célula:', 'celula',  $stSql, '', 'Selecione a(s) Célula(s)' );
					
					
					// Sistema
					$sidid = array();
					if ( $_REQUEST['sidid'] && $_REQUEST['sidid'][0] != '' )
					{
						$sql_carregados = "select  
												s.sidid  AS codigo, 
												upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
											from 
												demandas.sistemadetalhe s
											left join demandas.sistemacelula c on s.sidid = c.sidid  
											where  s.sidstatus = 'A'
											and s.sidid in (".implode(",",$_REQUEST['sidid']).")
											order by s.sidabrev ";
						
						$sidid=$db->carregar( $sql_carregados );
					}
					$stSql = " select  
									s.sidid  AS codigo, 
									upper(s.sidabrev) || ' - ' || s.siddescricao AS descricao 
								from 
									demandas.sistemadetalhe s
								left join demandas.sistemacelula ur on s.sidid = ur.sidid  
								where  s.sidstatus = 'A'
								$whereCelid
								order by s.sidabrev ";
					mostrarComboPopup( 'Sistema:', 'sidid',  $stSql, '', 'Selecione o(s) Sistema(s)' );
				//}					
					
				
				// Situação
				$situacao = array();
				if ( $_REQUEST['situacao'] && $_REQUEST['situacao'][0] != '' )
				{
					for($i=0; $i<count($_REQUEST['situacao']); $i++){
						if($_REQUEST['situacao'][$i] == '91,107') $sql_carregados3 .= "select '91,107' as codigo, 'Em Análise' as descricao";
						if($_REQUEST['situacao'][$i] == '92,108') $sql_carregados3 .= "select '92,108' as codigo, 'Em Atendimento' as descricao";
						if($_REQUEST['situacao'][$i] == '100,110') $sql_carregados3 .= "select '100,110' as codigo, 'Cancelada' as descricao";
						if($_REQUEST['situacao'][$i] == '93,111') $sql_carregados3 .= "select '93,111' as codigo, 'Aguardando validação' as descricao";
						if($_REQUEST['situacao'][$i] == '135,136') $sql_carregados3 .= "select '135,136' as codigo, 'Invalidada' as descricao";
						if($_REQUEST['situacao'][$i] == '772') $sql_carregados3 .= "select '772' as codigo, 'Auditada' as descricao";
						if($_REQUEST['situacao'][$i] == '95,109,170' || $_REQUEST['situacao'][$i] == '95,109') $sql_carregados3 .= "select '95,109,170' as codigo, 'Validada / Validada Sem Pausa / Finalizada' as descricao";
						if($i != count($_REQUEST['situacao'])-1) $sql_carregados3 .= " union all ";
						 
					}
					
					if($sql_carregados3) $situacao = $db->carregar( $sql_carregados3 );					
				}
				$stSql = " select '91,107' as codigo, 'Em Análise' as descricao
  						   union all
						   select '92,108' as codigo, 'Em Atendimento' as descricao
						   union all
						   select '100,110' as codigo, 'Cancelada' as descricao
						   union all
						   select '93,111' as codigo, 'Aguardando validação' as descricao
						   union all
						   select '135,136' as codigo, 'Invalidada' as descricao
						   union all
						   select '772' as codigo, 'Auditada' as descricao
						   union all
						   select '95,109,170' as codigo, 'Validada / Validada Fora do Prazo / Finalizada' as descricao ";
				mostrarComboPopup( 'Situação da Demanda:', 'situacao',  $stSql, '', 'Selecione a(s) Situação(ões)' );				
				
				
			?>
		<tr>
			<td class="SubTituloDireita" width="35%">Período de Abertura da Demanda:</td>
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
				<!--
					<input type="button" value="Visualizar" onclick="exibeRelatorioProcesso();" style="cursor: pointer;"/>
				 
					<input type="button" value="Gerar Arquivo XLS" onclick="" style="cursor: pointer;"/>
				 -->
				
				 <input type="button" value="Visualizar" onclick="exibeRelatorioGeral('html');" style="cursor: pointer;"/>
				  &nbsp;&nbsp;&nbsp;
				 <input type="button" value="Visualizar XLS" onclick="exibeRelatorioGeral('xls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>

</form>

