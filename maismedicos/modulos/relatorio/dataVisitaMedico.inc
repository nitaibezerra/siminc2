<?php 

if($_REQUEST['requisicao']){
	include 'dataVisitaMedico_result.inc';
	die;
}

include APPRAIZ . 'includes/cabecalho.inc';
include APPRAIZ . 'includes/Agrupador.php';

echo "<br>";
$titulo_modulo = "Data de Visitas por Médico";
monta_titulo( $titulo_modulo, 'Selecione os filtros e agrupadores desejados' );
$arPerfil = arrayPerfil();
?>
<script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script>
<script>

	function exibeRelatorioListaNominal(tipo)
	{
		document.filtro.requisicao.value = tipo;

		selectAllOptions(document.getElementById('muncod'));
		selectAllOptions(document.getElementById('uniid'));
		selectAllOptions(document.getElementById('medico'));
	
		document.filtro.target = 'resultadoListaNominal';
		var janela = window.open( '', 'resultadoListaNominal', 'width=780,height=465,status=1,menubar=1,toolbar=0,scrollbars=1,resizable=1' );
		janela.focus();
	
		document.filtro.submit();
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
<form action="" method="post" name="filtro">
	<input type="hidden" name="requisicao" value="" />
	<table class="tabela" align="center" bgcolor="#f5f5f5" cellspacing="1" cellpadding="3">		
		<tr>
			<td class="subtitulodireita">CPF</td>
			<td><?php echo campo_texto('cpf', 'N', 'S', '', 25, 255, '###.###.###-##', '', '')?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nome</td>
			<td><?php echo campo_texto('nome', 'N', 'S', '', 30, 255, '', '', '')?></td>
		</tr>
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
		<tr>
			<td class="subtitulodireita">Mostrar DSEI</td>
			<td><input type="checkbox" name="incluirdsei" checked/></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Mostrar Data de Visita Vazias</td>
			<td><input type="checkbox" name="mostrardtvazia" /></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Mostrar Data de Visita com Erro</td>
			<td><input type="checkbox" name="mostrardterro" /></td>
		</tr>
		<tr>
			<td bgcolor="#CCCCCC"></td>
			<td bgcolor="#CCCCCC">
				<input type="button" value="Visualizar" onclick="exibeRelatorioListaNominal('exibir');" style="cursor: pointer;"/>
				<input type="button" value="Visualizar XLS" onclick="exibeRelatorioListaNominal('exibirxls');" style="cursor: pointer;"/>
			</td>
		</tr>
	</table>
</form>