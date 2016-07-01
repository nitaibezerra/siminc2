<?php if($_REQUEST['requisicao'] == 'email'): ?>

	<?php include APPRAIZ.'maismedicos/modulos/relatorio/relatorioPagamento_email.inc'; die; ?>
	
<?php elseif($_REQUEST['requisicao'] == 'montaRelatorio'): ?>

	<?php if($_REQUEST['gerarxls']!='true'): ?>
	<html>
		<head>
			<title>Remessa de Crédito - Mais Médicos</title>
			<script language="JavaScript" src="../includes/funcoes.js"></script>
			<link rel="stylesheet" type="text/css" href="../includes/Estilo.css"/>
			<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
		</head>
		<body>
	<?php endif; ?>
			<?php
			
			extract($_REQUEST);
			
			$titulo_modulo = "Relatório de Pagamento";
			monta_titulo($titulo_modulo, '');
			
			if(!empty($_POST['tutcpf'])){
				$_POST['tutcpf'] = str_replace(array('.','-'),'', $_POST['tutcpf']);				
				$arWhere[] = "tut.tutcpf = '{$_POST['tutcpf']}'";
			}
			if(!empty($_POST['tutnome'])){
				$arWhere[] = "tut.tutnome ilike '%{$_POST['tutnome']}%'";
			}
			if( $strid[0] && $strid_campo_flag ){
				$arWhere[] = " sit.strid ". (!$strid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $strid )." ) ";
			}
			if( $uniid[0] && $uniid_campo_flag ){
				$arWhere[] = " uni.uniid ". (!$uniid_campo_excludente ? ' IN ' : ' NOT IN ') . " ( ".implode( ",", $uniid )." ) ";
			}
			if(!empty($_POST['tuttipo']) && $_POST['tuttipo'] != 'A'){	
				$operador = $_POST['tuttipo'] == 'T' ? '=' : '<>';			
				$arWhere[] = "tut.tuttipo {$operador} 'T'";
			}
			if(!empty($_POST['mesperiodo']) && !empty($_POST['anoperiodo'])){
				$_POST['mesperiodo'] = str_pad($_POST['mesperiodo'], 2, "0", STR_PAD_LEFT);
				$mesAno = $_POST['anoperiodo'].$_POST['mesperiodo'];
				$arWhere[] = "dt_ini_periodo ilike '{$mesAno}%' AND det.dt_fim_periodo ilike '{$mesAno}%'";
			}
			if($_REQUEST['dt_pagamento']){
				$dt_pagamento = formata_data_sql($_REQUEST['dt_pagamento']);
				$arWhere[] = "to_char(dt_envio_email,'YYYY-MM-DD') = '{$dt_pagamento}'";
			}
			
			$sqlErro = "select
						tut.tutcpf,
		   				tut.tutnome,
		   				case when tut.tuttipo = 'T'
		   					then 'Tutor'
		   					else 'Supervisor'
		   				end as funcao,
		   				case when tut.tuttipo = 'T'
		   					then '5000'
		   					else '4000'
		   				end as valor,
						det.nu_nib,
						to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'MM/YYYY') as periodo,
						to_char(dt_envio_email, 'DD/MM/YYYY') as dt_envio_email,
		   				uni.uninome,
		   				coalesce(sit.strdsc,'N/A') as tpodsc
		   			from
		   				maismedicos.tutor tut
		   			inner join
		   				maismedicos.universidade uni ON uni.uniid = tut.uniid
		   			inner join
		   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid and det.cs_ocorrencia != '0000'
		   			inner join
		   				maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
		   			inner join
		   				maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
					left join
						maismedicos.autorizacaopagamento apg on apg.rmdid = det.rmdid and apg.apgstatus = 'A'
		   			left join
		   				maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia
		   			".($arWhere ? ' where '.implode(' and ', $arWhere) : '')."
		   			order by
		   				tut.tutnome, to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'YYYYMM') asc";
			
			$sql = "select
						tut.tutcpf,
		   				tut.tutnome,
		   				case when tut.tuttipo = 'T'
		   					then 'Tutor'
		   					else 'Supervisor'
		   				end as funcao,
		   				case when tut.tuttipo = 'T'
		   					then '5000'
		   					else '4000'
		   				end as valor,
						det.nu_nib,
						to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'MM/YYYY') as periodo,
						to_char(dt_envio_email, 'DD/MM/YYYY') as dt_envio_email,
		   				uni.uninome,
		   				coalesce(sit.strdsc,'N/A') as tpodsc
		   			from
		   				maismedicos.tutor tut
		   			inner join
		   				maismedicos.universidade uni ON uni.uniid = tut.uniid
		   			inner join
		   				maismedicos.remessadetalhe det ON det.tutid = tut.tutid and det.cs_ocorrencia = '0000'
		   			inner join
		   				maismedicos.remessacabecalho cab ON cab.rmcid = det.rmcid
		   			inner join
		   				maismedicos.folhapagamento fpg ON fpg.fpgid = cab.fpgid
					left join 
						maismedicos.autorizacaopagamento apg on apg.rmdid = det.rmdid and apg.apgstatus = 'A'
		   			left join
		   				maismedicos.situacaoregistro sit ON sit.strcod = det.cs_ocorrencia   			
		   			".($arWhere ? ' where '.implode(' and ', $arWhere) : '')."
		   			order by
		   				tut.tutnome, to_char(to_date(dt_ini_periodo,'YYYYMMDD'), 'YYYYMM') asc";
			
// 			ver($sql);

		   	$arrCab = array("CPF","Nome","Função","Valor (R$)","Número do Benefício","Mês","Pagamento","Instituição","Situação");
		   	
		   	if($_REQUEST['gerarxls']=='true'){
		   		
				header ( "Expires: Mon, 1 Apr 1974 05:00:00 GMT");
				header ( "Last-Modified: " . gmdate("D,d M YH:i:s") . " GMT" );
				header ( "Pragma: no-cache" );
				header ( "Content-type: application/xls; name=SIMEC_RELATORIO_PAGAMENTO_".$_GET['rmcid'].".xls");
				header ( "Content-Disposition: attachment; filename=SIMEC_RELATORIO_PAGAMENTO_".$_GET['rmcid'].".xls");
				header ( "Content-Description: MID Gera excel" );
				
// 				$db->monta_lista_tabulado($sql, $arrCab, 100000000, 1000);
				gerar_excel($sql, $arrCab, 'SIMEC_RELATORIO_PAGAMENTO_'.date("Ymdhis"));
				die;
				
			}else{

				echo '<table class="tabela" align="center"><tr><td class="subtitulocentro">Pago</td></tr></table>';
		   		$db->monta_lista($sql, $arrCab, 10000000, 10, "S", "center", "");
		   		
	   			echo '<p>&nbsp;</p><table class="tabela" align="center"><tr><td class="subtitulocentro">Erro</td></tr></table>';
		   		$db->monta_lista($sqlErro, $arrCab, 10000000, 10, "S", "center", "");
		   	}
		   	
			?>	
		</body>
	</html>
	<?php die; ?>
<?php endif; ?>

<?php 
include APPRAIZ . 'includes/cabecalho.inc';
echo "<br/>";

$titulo_modulo = "Relatório de Pagamento";
monta_titulo($titulo_modulo, 'Selecione os filtros e agrupadores desejados');
?>
<!-- <script type="text/javascript" src="../includes/JQuery/jquery-1.4.2.js"></script> -->
<!-- <script type="text/javascript" src="../includes/calendario.js"></script> -->
<script language="javascript" type="text/javascript" src="../includes/JsLibrary/date/displaycalendar/displayCalendar.js"></script>
<link href="../includes/JsLibrary/date/displaycalendar/displayCalendar.css" type="text/css" rel="stylesheet"></link>
<script>
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

	function enviarFormularioPagamento(tipo)
	{
		selectAllOptions(document.getElementById('uniid'));
		selectAllOptions(document.getElementById('strid'));
		document.getElementById('requisicao').value = tipo;
		document.formulario.submit();
	}
	
	function enviarFormularioPagamentoXLS(tipo, xls)
	{
		selectAllOptions(document.getElementById('uniid'));
		selectAllOptions(document.getElementById('strid'));
		document.getElementById('requisicao').value = tipo;
		document.getElementById('gerarxls').value = 'true';
		document.formulario.submit();
	}

	function limpaFormulario(rmcid, periodo)
	{
		document.location.href = '/maismedicos/maismedicos.php?modulo=relatorio/relatorioPagamento&acao=A';
	}
</script>

<?php if($_POST) extract($_POST);  ?>

<form id="formulario" name="formulario" method="post" target="POPUPW"
    onsubmit="POPUPW = window.open('about:blank','POPUPW',
   'width=600,height=400,scrollbars=yes,top=50,left=200');">
	<input type="hidden" name="requisicao" id="requisicao" value="" />
	<input type="hidden" name="gerarxls" id="gerarxls" value="" />
	<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
		<tr>
			<td class="subtitulodireita">CPF</td>
			<td><?=campo_texto('tutcpf','','','',16,14,'###.###.###-##','', '', '', '', '', '', '', "this.value=mascaraglobal('###.###.###-##',this.value);");?></td>
		</tr>
		<tr>
			<td class="subtitulodireita">Nome</td>
			<td><?=campo_texto('tutnome','','','',50,50,'','', '', '', '', '', '', $tutnome);?></td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">Período de referência</td>
			<td>
				<?php
				$a=0;
				for($x=1;$x<13;$x++){
					$meses_periodo[$a]['codigo'] = $x;
					$meses_periodo[$a]['descricao'] = mes_extenso($x);
					$a++;
				} 
				
				$db->monta_combo('mesperiodo',$meses_periodo,'S','Selecione...',null,null,null,null,'N','mesperiodo');
				
				$b=0;
				for($y=2008;$y<2018;$y++){
					$anos_periodo[$b]['codigo'] = $y;
					$anos_periodo[$b]['descricao'] = $y;
					$b++;
				}
				
				$db->monta_combo('anoperiodo',$anos_periodo,'S','Selecione...',null,null,null,null,'N','anoperiodo');
				?>
			</td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">Data de pagamento</td>
			<td><?php echo campo_data2("dt_pagamento","N","S","","","","",""); ?></td>
		</tr>
		
		<tr>
			<td class="subtitulodireita">Função</td>
			<td>
				<input type="radio" name="tuttipo" value="T" <?php echo $_REQUEST['tuttipo'] == 'T' ? 'checked' : ''; ?> />&nbsp;Tutor&nbsp;
				<input type="radio" name="tuttipo" value="S" <?php echo $_REQUEST['tuttipo'] == 'S' ? 'checked' : ''; ?>/>&nbsp;Supervisor&nbsp;
				<input type="radio" name="tuttipo" value="A" <?php echo (empty($_REQUEST['tuttipo']) || $_REQUEST['tuttipo'] == 'A') ? 'checked' : ''; ?>/>&nbsp;Ambos
			</td>
		</tr>
		<?php 
				// -- Universidades
				$sql_carregados = '';
				if ($_REQUEST['uniid'] && $_REQUEST['uniid'][0] != '') {
					$sql_carregados = "SELECT 
										uniid as codigo, 
										uninome as descricao 
									FROM maismedicos.universidade where unistatus = 'A'
                                   AND uniid IN(" . implode(',', $_REQUEST['uniid']) . ")
                                 ORDER BY uninome asc";
				}
				$stSql = "select uniid as codigo, uninome as descricao from maismedicos.universidade where unistatus = 'A' order by uninome asc";
				mostrarComboPopup( 'Instituição', 'uniid', $stSql, $sql_carregados, 'Selecione a(s) Instituição(ões)');
				
				// -- Situação
				$sql_carregados = '';
				if ($_REQUEST['strid'] && $_REQUEST['strid'][0] != '') {
					$sql_carregados = "select 
										strid as codigo, 
										strdsc as descricao 
									from maismedicos.situacaoregistro
                                   where strid IN(" . implode(',', $_REQUEST['strid']) . ")
                                 ORDER BY strcod asc";
				}
				
				$stSql = "select strid as codigo, strcod || ' - ' || strdsc as descricao from maismedicos.situacaoregistro where strstatus = 'A' order by strcod asc";
				mostrarComboPopup('Situação', 'strid', $stSql, $sql_carregados, 'Selecione a(s) Situação(ões)');
				?>				
		<tr>
			<td colspan="2" class="subtituloesquerda">
				<input type="button" value="Pesquisar" onclick="enviarFormularioPagamento('montaRelatorio')" />
				<input type="button" value="Gerar XLS" onclick="enviarFormularioPagamentoXLS('montaRelatorio', 'gerarxls')" />							
				<input type="button" value="Limpar" onclick="limpaFormulario('<?php echo $_REQUEST['rmcid']; ?>', '<?php echo $_REQUEST['periodo']?>')" />
				<input type="button" value="Enviar e-mail" onclick="enviarFormularioPagamento('email')" />
			</td>					
		</tr>
	</table>
</form>