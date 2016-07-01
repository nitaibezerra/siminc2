<?
#
# Sistema Simec
# Setor responsável: SPO-MEC
# Desenvolvedor: Equipe Consultores Simec
# Analista: Gilberto Arruda Cerqueira Xavier
# Programador: Gilberto Arruda Cerqueira Xavier (e-mail: gacx@ig.com.br)
# Programador: Thiago Mata (e-mail thiago.henrique.mata@gmail.com )
# Módulo:relinconsistencias_aca.inc
# Finalidade: permitir a construção de relatorio de inconsistencias em acao
#
// as inconsistencias em ação são por exemplo propostas de exclusão ao memso tempo que há porposta de fusão ou alteração de atributos e migrações

include APPRAIZ . 'includes/cabecalho.inc';
print '<br/>';
monta_titulo( 'Relatório de Inconsistências em Ações', '' );
	
?>

<form action="" method="post" name="formulario">
<input type="hidden" name="act" value="0"> 

<table  width="95%"  class="listagem"  bgcolor="#f5f5f5" cellSpacing="1" cellPadding="3" align="center">
  <tr>
	<td>
		<b>	Relatórios de Inconsistências em Ações</b>
	</td>
 </tr>
</table>
<table width='95%' align='center' border="0" cellspacing="0" cellpadding="2" class="listagem">
<thead>
	<tr>
		<td>
			<b> Ação (clique na ação para ver todas as propostas)</b>
		</td>
	</tr>
</thead>
<tbody>
	<?
	$sql = " SELECT prg.prgcod||'-'||p.acacod||'.'||p.acadsc as descricao, va.* FROM elabrev.v_total_propostas_acoes va inner join elabrev.ppaacao_proposta p on p.acaid=va.acaid and p.prsano= '". $_SESSION[ 'exercicio' ] . "' inner join elabrev.ppaprograma_proposta prg on prg.prgid=p.prgid WHERE va.TOTAL >1 order by descricao " ;

	$acoes=$db->carregar($sql);
	$i=0;
	
	if ( $acoes ) {
		foreach ($acoes as $acao)
		{
	 		// a query acima me tras as ações com inconsistências
			$boolPar = ( ( $i % 2 ) == 0 );
			$i++;
			if 	($boolPar )	$strBgColor = '';
			else $strBgColor = '#F7F7F7';
			?>
	    	<tr bgcolor="<?= $strBgColor ?>" onmouseover="this.bgColor='#ffffcc';" 
						onmouseout="this.bgColor='<?= $strBgColor ?>';">
			   <td onclick="abre_rel(<?=$acao['acaid']?>)">
				<?=$acao['descricao']?>
			</td>
		</tr>
		<?	
		
		}
	}
	?>	
</tbody>
</table>
</form>

<script type="text/javascript">

function abre_rel(cod)
{
	e = '<?=$_SESSION['sisdiretorio']?>.php?modulo=relatorio/acao/geraproposta&acao=A&acod='+cod;
      window.open(e, "Relatorio","menubar=no,location=no,resizable=no,scrollbars=yes,status=yes,left=0,top=0,width=600,height=450"); 
}
	

</script>