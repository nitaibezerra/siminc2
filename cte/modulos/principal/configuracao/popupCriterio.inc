<?php
if (!$_GET[indid])
	die("<script>window.close()</script>");

$_POST[indid] ? $indid = $_POST[indid] : $indid = $_GET[indid];	
$_POST[crtid] ? $crtid = $_POST[crtid] : $crtid = $_GET[crtid];

if ($_POST):
	if (!$crtid):
		$sql = sprintf("INSERT INTO
							cte.criterio (
							indid,
							crtdsc,
							ctrpontuacao,
							ctrord,
							ctrindpla						
						)VALUES(
							%d,
							'%s',
							%d,
							%d,
							'%s'
						)", $_POST[indid], 
							$_POST[crtdsc], 
							$_POST[ctrpontuacao], 
							$_POST[ctrord], 
							$_POST[ctrindpla]);
	else:
		$sql = sprintf("UPDATE
							cte.criterio
						SET							
							ctrpontuacao = %d,
							crtdsc = '%s',
							ctrord = %d,
							ctrindpla = '%s'
						WHERE
							crtid = %d", $_POST[ctrpontuacao],
										 $_POST[crtdsc],	 
										 $_POST[ctrord], 
										 $_POST[ctrindpla],
										 $crtid);	
	endif;
	$db->executar($sql);
	$db->commit();
	
	die ("<script>
			alert('Operação executada com sucesso!');
			window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");	
endif;

if ($crtid):
	$sql = sprintf("SELECT
						dimdsc,
						arddsc,
						inddsc,
						crtdsc,
						ctrpontuacao,
						ctrord,
						CASE
							WHEN ctrindpla = 't' THEN 'TRUE'
							ELSE 'FALSE'
						END as ctrindpla
					FROM
						cte.criterio crt INNER JOIN cte.indicador USING(indid) 
						INNER JOIN cte.areadimensao USING(ardid)
						INNER JOIN cte.dimensao USING(dimid)
					WHERE
						crtid = %d ",$crtid);
else:	
	$sql = sprintf("SELECT
						dimdsc,
						arddsc,
						inddsc,
						MAX(ctrord)+1 AS ctrord
					FROM
						cte.indicador LEFT JOIN cte.criterio USING(indid) 
						INNER JOIN cte.areadimensao USING(ardid)
						INNER JOIN cte.dimensao USING(dimid)
					WHERE
						indid = %d
					GROUP BY
						dimdsc,
						arddsc,
						inddsc",$indid);
endif;
$dados = $db->carregar($sql);

foreach ($dados[0] as $k => $val)
	${$k} = $k == 'ctrord' ?  $val+1 : $val;				

unset($sql,$dados,$k,$val)
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formCrt">
<div id="sp_acoes">
<?php
monta_titulo( 'Manutenção do Critério de Pontuação', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Dimensão:</td>
		<td><?= campo_texto( 'dimdsc', 'S', 'N', '', 75, 500, '', '' ); ?></td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" align="right">Área:</td>
		<td><?= campo_texto( 'arddsc', 'S', 'N', '', 75, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Indicador:</td>
		<td><?= campo_texto( 'inddsc', 'S', 'N', '', 75, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Critério:</td>
		<td><?= campo_textarea( 'crtdsc', 'S', 'S' , '', 80, 6, 2000 ); ?></td>
	</tr>				
	<tr>
		<td class="SubTituloDireita" align="right">Plano de Ação:</td>
		<td>
		<?php
			if (!$ctrindpla)
				$ctrindpla = 'FALSE';

			$itens = array(array("codigo" => "TRUE",
							     "descricao" => "Sim"),
						   array("codigo" => "FALSE",
						   		 "descricao" => "Não"));
			
			$db->monta_combo( "ctrindpla", $itens, 'S', '', '', '', '', '', 'S' );  
	    ?>
	   </td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Pontuação:</td>
		<td><?= campo_texto( 'ctrpontuacao', 'S', 'S', '', 10, 75, '', '' ); ?></td>
	</tr>				
	<tr>
		<td class="SubTituloDireita" align="right">Ordem:</td>
		<td><?= campo_texto( 'ctrord', 'S', 'S', '', 10, 75, '', '' ); ?></td>
	</tr>		
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<?
			if($crtid){
				echo("<td><input type='button' class='botao' value='Alterar' id='btalterarestado' name='btalterarestado' onclick=\"validar()\" />");
			}else{
				echo("<td><input type='button' class='botao' name='consultar' value='Salvar' onclick=\"validar()\" />");
			}
		?>
			<input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='window.opener.location.replace(window.opener.location); window.close();' />
			<input type='hidden' value='<?php echo $indid;?>' id='indid' name='indid'/>
			<input type='hidden' value='<?php echo $crtid;?>' id='crtid' name='crtid'/>
		</td>			
	</tr>
</table>
</form>
<script type="text/javascript">
function validar() {
	d = document.formCrt;
	
	if (d.crtdsc.value == ''){
		d.crtdsc.focus();
		d.crtdsc.select();
		alert('O campo Critério é obrigatório!');
		return false;
	}
	
	if (d.ctrpontuacao.value == '') {
		d.ctrpontuacao.focus();
		d.ctrpontuacao.select();
		alert('O campo Pontuação é obrigatório!');
		return false;		
	}
		
	if (d.ctrord.value == '' || isNaN(d.ctrord.value)) {
		d.ctrord.focus();
		d.ctrord.select();
		alert('O campo Ordem é obrigatório e numérico!');
		return false;		
	}	
	d.submit();
}
</script>
</body>