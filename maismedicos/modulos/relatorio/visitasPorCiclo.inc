<?php 

// Exibe consulta
if ( isset( $_REQUEST['form'] ) == true && !$_REQUEST['carregar']){

	if ( $_REQUEST['prtid'] ){

		$sql = sprintf(	"select prtobj from public.parametros_tela where prtid = " . $_REQUEST['prtid'] );

		$itens = $db->pegaUm( $sql );

		$dados = unserialize( stripslashes( stripslashes( $itens ) ) );

		$_REQUEST = $dados;

		unset( $_REQUEST['salvar'] );
	}

	include "visitasPorCiclo_result.inc";
	die;
}

if($_REQUEST['requisicao']=='mostraDetalhes'){
	include "visitasPorCiclo_detalhe.inc";
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
// include APPRAIZ . 'includes/Agrupador.php';
  
echo "<br>";
$titulo_modulo = "Frequência de Visitas por Ciclo";
monta_titulo( $titulo_modulo, 'Selecione os filtros desejados' );
$arPerfil = arrayPerfil();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

	function exibeRelatorio( tipo ){
		
		var formulario = document.formulario;
// 		var agrupador  = document.getElementById( 'agrupador' );
		
		// Tipo de relatorio
		formulario.pesquisa.value='1';
	
		prepara_formulario();

// 		selectAllOptions( agrupador );	

		selectAllOptions( document.getElementById( 'muncod' ) );
		selectAllOptions( document.getElementById( 'uniid' ) );
		selectAllOptions( document.getElementById( 'supervisor' ) );
		selectAllOptions( document.getElementById( 'medico' ) );
	
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
	
			formulario.target = '_self';
			formulario.action = 'maismedicos.php?modulo=relatorio/visitasPorCiclo&acao=A&salvar=1';
			formulario.submit();
				
		}else if ( tipo == 'relatorio' ){
			
			formulario.action = 'maismedicos.php?modulo=relatorio/visitasPorCiclo&acao=A';
			window.open( '', 'relatorio', 'width=780,height=460,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			formulario.target = 'relatorio';
			formulario.submit();
	
		} else {
		
			var formulario = document.formulario;
// 			var agrupador  = document.getElementById( 'agrupador' );
			
// 			if ( !agrupador.options.length ){
// 				alert( 'Favor selecionar ao menos um item para agrupar o resultado!' );
// 				return false;
// 			}
			
			formulario.target = 'resultadoGeral';
			var janela = window.open( '?modulo=relatorio/visitasPorCiclo&acao=A', 'resultadoGeral', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
			
			formulario.submit();
			janela.focus();		
		}		
	}

	function gerarRelatorioXLS(){
		document.getElementById('req').value = 'geraxls';
		exibeRelatorio();
	}

	function exibeRelatorioNormal()
	{
		document.getElementById('req').value = '';
		exibeRelatorio();
	}

	function tornar_publico( prtid ){
		document.formulario.publico.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}
		
	function excluir_relatorio( prtid ){
		if(confirm('Deseja realmente excluir este relatório?')){
			document.formulario.excluir.value = '1';
			document.formulario.prtid.value = prtid;
			document.formulario.target = '_self';
			document.formulario.submit();
		}
	}

	function carregar_consulta( prtid ){
		document.formulario.carregar.value = '1';
		document.formulario.prtid.value = prtid;
		document.formulario.target = '_self';
		document.formulario.submit();
	}

	function carregar_relatorio( prtid ){
		document.formulario.prtid.value = prtid;
		exibeRelatorio( 'relatorio' );
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

</script>
<form action="" method="post" name="formulario">

	<input type="hidden" name="form" value="1"/>
	<input type="hidden" id="req" name="req" value=""/>	
	<input type="hidden" name="pesquisa" value="1"/>
	<input type="hidden" name="publico" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado -->
	<input type="hidden" name="prtid" value=""/> <!-- indica se foi clicado para tornar o relatório público ou privado, passa o prtid -->
	<input type="hidden" name="carregar" value=""/> <!-- indica se foi clicado para carregar o relatório -->
	<input type="hidden" name="excluir" value=""/> <!-- indica se foi clicado para excluir o relatório já gravado -->
	
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">
		<?php if(!in_array(PERFIL_SUPERVISOR, $arPerfil) && !in_array(PERFIL_TUTOR, $arPerfil)): ?>
		<tr>
			<td class="subtitulodireita">UF</td>
			<td>
				<?php				
				$sql = "select 
							estuf as codigo,
							estdescricao as descricao
						from territorios.estado
						order by estuf";
				
				$db->monta_combo('estuf',$sql,'S','Selecione...',null,null,null,null,'N','estuf');
				?>
			</td>
		</tr>
		<?php 
		mostrarComboPopupMunicipios();
		?>
		<?php endif; ?>
		<?php 
		if(in_array(PERFIL_SUPERVISOR, $arPerfil) || in_array(PERFIL_TUTOR, $arPerfil)){
			$arWhere[] = "usr.usucpf = '{$_SESSION['usucpf']}'";
		}
		
		// -- Situação
		$sql_carregados = '';
		
		$stSql = "select 
						u.uniid as codigo, 
						u.uninome as descricao 
				from maismedicos.universidade u
				left join maismedicos.usuarioresponsabilidade usr on usr.uniid = u.uniid
					and usr.pflcod in (".implode(",",$arPerfil).")
				where unistatus = 'A'
				".($arWhere ? " and ".implode(" and ", $arWhere) : "")." 
				order by uninome asc";
		
		mostrarComboPopup('Instituição Supervisora', 'uniid', $stSql, $sql_carregados, 'Selecione a(s) Instituição(ões) Supervisora(s)');
		
		$sql_carregados = '';
		
		$stSql = "select distinct
					tutcpf as codigo,
					tutnome as descricao
				from maismedicos.tutor t
				left join maismedicos.usuarioresponsabilidade usr on usr.uniid = t.uniid
					and usr.pflcod in (".implode(",",$arPerfil).")
				where tuttipo = 'S'
				and tutstatus = 'A'
				".($arWhere ? " and ".implode(" and ", $arWhere) : "")."
				order by tutnome";
		
		$where = array(
				array("codigo"=>"tutnome","descricao"=>"Nome"),
				array("codigo"=>"tutcpf","descricao"=>"CPF")
		);
		mostrarComboPopup('Supervisores', 'supervisor', $stSql, $sql_carregados, 'Selecione o(s) Supervisor(es)', $where);
		
		$sql_carregados = '';
		
		$stSql = "select
	 				mdccpf as codigo,
	 				mdcnome as descricao
	 			from maismedicos.medico m
				left join maismedicos.universidade u on u.idunasus = m.coinstituicaosupervisora
				left join maismedicos.usuarioresponsabilidade usr on usr.uniid = u.uniid
					and usr.pflcod in (".implode(",",$arPerfil).")
	 			where mdcstatus = 'A'
				".($arWhere ? " and ".implode(" and ", $arWhere) : "")."
	 			order by removeacento(lower(trim(mdcnome))) asc";
		
		$where = array(
				array("codigo"=>"mdcnome","descricao"=>"Nome"),
				array("codigo"=>"mdccpf","descricao"=>"CPF")
		);
		mostrarComboPopup('Médicos', 'medico', $stSql, $sql_carregados, 'Selecione o(s) Médico(s)',$where);
		?>
		<tr>
			<td class="subtitulodireita">Período</td>
			<td>
				<?php
				$a=0;
				for($x=1;$x<13;$x++){
					$meses_periodo[$a]['codigo'] = $x;
					$meses_periodo[$a]['descricao'] = mes_extenso($x);
					$a++;
				}
				
				$db->monta_combo('mesvisitainicio',$meses_periodo,'S','Selecione...',null,null,null,null,'N','mesvisitainicio');
				
				$b=0;
				for($y=2013;$y<2018;$y++){
					$anos_periodo[$b]['codigo'] = $y;
					$anos_periodo[$b]['descricao'] = $y;
					$b++;
				}
				
				$db->monta_combo('anovisitainicio',$anos_periodo,'S','Selecione...',null,null,null,null,'N','anovisitainicio');
				
				echo "&nbsp;INÍCIO<br/>";
				
				$a=0;
				for($x=1;$x<13;$x++){
					$meses_periodo[$a]['codigo'] = $x;
					$meses_periodo[$a]['descricao'] = mes_extenso($x);
					$a++;
				} 
				
				$db->monta_combo('mesvisitafim',$meses_periodo,'S','Selecione...',null,null,null,null,'N','mesvisitafim');
				
				$b=0;
				for($y=2013;$y<2018;$y++){
					$anos_periodo[$b]['codigo'] = $y;
					$anos_periodo[$b]['descricao'] = $y;
					$b++;
				}
				
				$db->monta_combo('anovisitafim',$anos_periodo,'S','Selecione...',null,null,null,null,'N','anovisitafim');
				?>
				FIM
			</td>
		</tr>	
		<?php if(!in_array(PERFIL_SUPERVISOR, $arPerfil) && !in_array(PERFIL_TUTOR, $arPerfil)): ?>	
		<tr>
			<td class="subtitulodireita">Mostrar Data de Visita Vazias</td>
			<td><input type="checkbox" name="mostrardtvazia" /></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Mostrar Data de Visita com Erro</td>
			<td><input type="checkbox" name="mostrardterro" /></td>
		</tr>
		<?php endif; ?>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorioNormal();" style="cursor: pointer;"/>
				<input type="button" value="VisualizarXLS" onclick="gerarRelatorioXLS();">
				<input type="button" value="Salvar Consulta" onclick="exibeRelatorio('salvar');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>