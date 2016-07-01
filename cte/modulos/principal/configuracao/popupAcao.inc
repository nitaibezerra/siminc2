<?php
if (!$_GET[crtid])
	die("<script>window.close()</script>");

$_POST[crtid] ? $crtid = $_POST[crtid] : $crtid = $_GET[crtid];
$_POST[ppaid] ? $ppaid = $_POST[ppaid] : $ppaid = $_GET[ppaid];	

if ($_POST):
	if (!$_POST[ppaid]):
		$sql = sprintf("INSERT INTO
							cte.proposicaoacao(
							ppadsc,
							crtid
						)VALUES(
							'%s',
							%d
						)", $_POST[ppadsc],
							$crtid);
	else:
		$sql = sprintf("UPDATE
							cte.proposicaoacao
						SET
							crtid = %d,
							ppadsc = '%s'
						WHERE
							ppaid = %d",$crtid,
										$_POST[ppadsc],
										$ppaid);
	
	endif;
	$db->executar($sql);
	$db->commit();
	
	die ("<script>
			alert('Operação executada com sucesso!');
			window.opener.location.replace(window.opener.location);
			window.close();
		  </script>");	
endif;

if ($ppaid):
	$sql = sprintf("SELECT
						ppadsc,
						dimdsc,
						arddsc,
						inddsc,
						crtdsc
					FROM
						cte.proposicaoacao pp JOIN cte.criterio ct USING(crtid)
						JOIN cte.indicador USING(indid)
						JOIN cte.areadimensao USING(ardid)
						JOIN cte.dimensao USING(dimid)
					WHERE
						pp.ppaid = %d",$ppaid);
else:
	$sql = sprintf("SELECT
						dimdsc,
						arddsc,
						inddsc,
						crtdsc
					FROM
						cte.criterio ct JOIN cte.indicador USING(indid)
						JOIN cte.areadimensao USING(ardid)
						JOIN cte.dimensao USING(dimid)
					WHERE
						ct.crtid = %d",$crtid);
endif;
$dados = $db->carregar($sql);

foreach ($dados[0] as $k => $val)
	${$k} = $val;				
	
unset($sql,$dados,$k,$val)	
?>
<script type="text/javascript" src="../includes/funcoes.js"></script>
<link rel="stylesheet" type="text/css" href="../includes/Estilo.css" />
<link rel='stylesheet' type='text/css' href='../includes/listagem.css'/>
<body leftmargin="0" topmargin="0" bottommargin="0" marginwidth="0">
<form action="" method="post" name="formPPA">
<div id="sp_acoes">
<?php
monta_titulo( 'Manutenção da Ação', '<img src="../imagens/obrig.gif" border="0"> Indica Campo Obrigatório.'  );
?>
<table class="tabela" bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3"
	align="center">
	<tr>
		<td class="SubTituloDireita" align="right">Dimensão:</td>
		<td><?= campo_texto( 'dimdsc', 'S', 'N', '', 85, 500, '', '' ); ?></td>
	</tr>		
	<tr>
		<td class="SubTituloDireita" align="right">Área:</td>
		<td><?= campo_texto( 'arddsc', 'S', 'N', '', 85, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Indicador:</td>
		<td><?= campo_texto( 'inddsc', 'S', 'N', '', 85, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Critério:</td>
		<td><?= campo_texto( 'crtdsc', 'S', 'N', '', 85, 500, '', '' ); ?></td>
	</tr>	
	<tr>
		<td class="SubTituloDireita" align="right">Descrição:</td>
		<td><?= campo_texto( 'ppadsc', 'S', 'S', '', 85, 500, '', '' ); ?></td>
	</tr>
	<tr bgcolor="#C0C0C0">
		<td>&nbsp;</td>
		<?
			if($ppaid){
				echo("<td><input type='button' class='botao' value='Alterar' id='btalterarestado' name='btalterarestado' onclick=\"validar()\" />");
			}else{
				echo("<td><input type='button' class='botao' name='consultar' value='Salvar' onclick=\"validar()\" />");
			}
		?>
			<input type='button' class='botao' value='Voltar' id='btFechar' name='btFechar' onclick='window.opener.location.replace(window.opener.location); window.close();' />
			<input type='hidden' value='<?php echo $ppaid;?>' id='ppaid' name='ppaid'/>
			<input type='hidden' value='<?php echo $crtid;?>' id='crtid' name='crtid'/>
		</td>			
	</tr>			
</table>
</form>
<script type="text/javascript">
function validar() {
	d = document.formPPA;
	
	if (d.ppadsc.value == ''){
		d.ppadsc.focus();
		d.ppadsc.select();
		alert('O campo Descrição é obrigatório!');
		return false;
	}
	d.submit();
}
</script>
</body>	